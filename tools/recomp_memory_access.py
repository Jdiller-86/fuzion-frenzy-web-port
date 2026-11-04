"""Conservative memory-access instrumentation for the clean pinned C emitter.

This observes/validates accesses; it does not replace guest instructions. Only
recognized lvalue, stack and bulk-pointer surfaces are admitted. Unknown pointer
escapes/helpers fail instead of silently bypassing guest memory permissions.

Every emitted scalar access carries the guest address of the translated basic
block that contains it. The pinned emitter emits ``loc_%08X:`` for every block
that can be entered (its entry block plus every successor), so the nearest
preceding label is the containing block; it is never a host address, a
hard-coded routine or a caller return address. A leading access before the first
label belongs to the function entry block. Emitting that site lets the device
aperture record which original code performed an access instead of claiming one
routine for every access. Stack and bulk helpers deliberately pass zero; the
host admits zero only against owned RAM, never a device window. A scalar write
whose value expression probes memory is evaluated into a temporary before the
staged store; the pinned movs mem-to-mem forms are split the same way.
"""
import bisect
import re

ACCESSORS = {"MEM8": "U8", "MEM16": "U16", "MEM32": "U32", "MEM64": "U64",
             "SMEM8": "I8", "SMEM16": "I16", "SMEM32": "I32", "SMEM64": "I64",
             "MEMF": "F32", "MEMD": "F64"}
TEMP_TYPES = {"U8": "uint8_t", "U16": "uint16_t", "U32": "uint32_t", "U64": "uint64_t",
              "I8": "int8_t", "I16": "int16_t", "I32": "int32_t", "I64": "int64_t",
              "F32": "float", "F64": "double"}
# Pure SSE helpers never touch guest memory. Anything else matching XMM_* is
# rejected unless the memory branches above rewrote it. Keep this explicit.
PURE_XMM = {"XMM_ZERO", "XMM_SCALAR", "XMM_SCALAR_DOUBLE", "XMM_SCALAR_BITS",
            "XMM_MOVE_LOW_TO_HIGH", "XMM_MOVE_HIGH_TO_LOW", "XMM_CMP_PRED",
            "XMM_MOVEMASK", "XMM_SHUFFLE", "XMM_UNPACK_LOW", "XMM_UNPACK_HIGH",
            "XMM_CVT_PS2DQ", "XMM_FROM_PI", "XMM_ADD", "XMM_SUB", "XMM_MUL",
            "XMM_DIV", "XMM_MIN", "XMM_MAX", "XMM_SQRT", "XMM_RSQRT", "XMM_RCP",
            "XMM_AND", "XMM_OR", "XMM_XOR", "XMM_ANDN", "XMM_CMP_EQ",
            "XMM_CMP_LT", "XMM_CMP_LE", "XMM_CMP_NEQ"}
# Vector moves whose memory operand must be 16-byte aligned. Unaligned forms are
# movups/movupd/movdqu (and lddqu) and keep the plain checked access.
ALIGNED_XMM = {"movaps", "movapd", "movdqa", "movdqa64", "movntps", "movntpd", "movntdq"}
BLOCK_LABEL = re.compile(r"(?<![0-9A-Za-z_])loc_([0-9A-Fa-f]{8})\s*:")
MEMORY_TOKEN = re.compile(r"\b(?:S?MEM\w*|PUSH32|POP32|XMM_(?:MEM|LOAD|STORE))\b")


def statement_end(mask, start):
    """Return the offset of the statement terminator after a checked access."""
    depth = 0
    for index in range(start, len(mask)):
        char = mask[index]
        if char in "({[":
            depth += 1
        elif char in ")}]":
            if not depth:
                return index
            depth -= 1
        elif char == ";" and not depth:
            return index
    return len(mask)


def code_mask(text):
    """Keep token offsets while excluding comments and quoted literals."""
    result, i = list(text), 0
    while i < len(text):
        start = i
        if text.startswith("//", i):
            end = text.find("\n", i)
            i = len(text) if end < 0 else end
        elif text.startswith("/*", i):
            end = text.find("*/", i + 2)
            if end < 0: raise ValueError("Unterminated generated comment")
            i = end + 2
        elif text[i] in "\"'":
            quote = text[i]; i += 1
            while i < len(text) and text[i] != quote:
                i += 2 if text[i] == "\\" else 1
            if i >= len(text): raise ValueError("Unterminated generated literal")
            i += 1
        else:
            i += 1; continue
        result[start:i] = " " * (i - start)
    return "".join(result)


def checked_block_sites(text, site, limit):
    """Return sorted ``(offset, guest address)`` labels for one function.

    The emitter writes blocks in address order, so labels must increase. A
    trailing fallthrough label may name the next function; it is kept so an
    access after it fails instead of being attributed to this function.
    """
    if type(site) is not int or not 0 < site <= 0xFFFFFFFF:
        raise ValueError("A checked guest block site is required")
    if limit is not None and (type(limit) is not int or not site < limit <= 0x100000000):
        raise ValueError("A checked guest function end is required")
    labels = []
    for match in BLOCK_LABEL.finditer(code_mask(text)):
        address = int(match.group(1), 16)
        if address < site:
            raise ValueError("Generated block label precedes its checked function")
        if labels and address <= labels[-1][1]:
            raise ValueError("Generated block labels are not strictly increasing")
        labels.append((match.start(), address))
    return labels


def instrument_memory_accesses(text, site, limit=None):
    if type(site) is not int or not 0 < site <= 0xFFFFFFFF:
        raise ValueError("A checked guest block site is required")
    stats = {"reads": 0, "writes": 0, "readWrites": 0, "stack": 0, "bulkRanges": 0, "sites": 0}
    # The pinned lifter emits movs as one mem-to-mem statement. Split the read
    # and write so the staged write is followed only by its own store.
    for width, ctype in (("8", "uint8_t"), ("16", "uint16_t"), ("32", "uint32_t")):
        text = text.replace(
            f"MEM{width}(edi) = MEM{width}(esi);",
            f"{{ {ctype} _ff_copy{width} = MEM{width}(esi); MEM{width}(edi) = _ff_copy{width}; }}")
    # These two native bulk paths are emitted by the pinned string lifter.
    # Validate the full extent before it obtains either host pointer.
    pair = "uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);"
    start = 0
    while (index := text.find(pair, start)) >= 0:
        prefix = code_mask(text[max(0, index - 600):index])
        if not re.search(r"uint64_t\s+_n\s*=\s*\(uint64_t\)ecx\s*\*\s*[124]u;", prefix):
            raise ValueError("Unestablished bulk-copy extent")
        replacement = "uint8_t *_d = FF_MEMORY_POINTER(edi, _n, 2u), *_s = FF_MEMORY_POINTER(esi, _n, 1u);"
        text = text[:index] + replacement + text[index + len(pair):]
        start = index + len(replacement); stats["bulkRanges"] += 2
    fill = "memset((void*)XBOX_PTR(edi), (uint8_t)eax, ecx)"
    stats["bulkRanges"] += text.count(fill)
    text = text.replace(fill, "memset(FF_MEMORY_POINTER(edi, ecx, 2u), (uint8_t)eax, ecx)")

    labels = checked_block_sites(text, site, limit)
    label_offsets = [offset for offset, _ in labels]
    used_sites = set()
    write_temps = [0]
    xmm_temps = [0]

    def site_at(offset):
        index = bisect.bisect_right(label_offsets, offset) - 1
        if index < 0:
            return site
        address = labels[index][1]
        if limit is not None and address >= limit:
            raise ValueError("Generated access follows a label outside its checked function")
        return address

    def rewrite(source, base):
        mask = code_mask(source); pieces, cursor, scan = [], 0, 0
        while match := re.search(r"\b(?:S?MEM\w*|PUSH32|POP32|XMM_(?:MEM|LOAD|STORE))\b", mask[scan:]):
            begin, end = scan + match.start(), scan + match.end()
            name = source[begin:end]
            opening = end
            while opening < len(mask) and mask[opening].isspace(): opening += 1
            if opening >= len(mask) or mask[opening] != '(':
                raise ValueError("Unexpected generated memory token")
            if name in ("PUSH32", "POP32"):
                pieces += [source[cursor:begin], "FF_MEMORY_PUSH" if name == "PUSH32" else "FF_MEMORY_POP"]
                cursor = scan = end; stats["stack"] += 1; continue
            depth, closing = 1, opening + 1
            while closing < len(mask) and depth:
                depth += (mask[closing] == '(') - (mask[closing] == ')'); closing += 1
            if depth: raise ValueError("Unclosed generated memory expression")
            if name in ("XMM_MEM", "XMM_LOAD"):
                before, after = mask[:begin].rstrip(), mask[closing:].lstrip()
                write_or_escape = (
                    (after.startswith(('+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<=', '>>=', '++', '--')) or
                     (after.startswith('=') and not after.startswith('==')) or
                     before.endswith(('++', '--', '&'))))
                if write_or_escape:
                    raise ValueError("Vector load used in a write or escaped-pointer context")
                address = rewrite(source[opening + 1:closing - 1], base + opening + 1).strip()
                if not address: raise ValueError("Empty generated vector address")
                block = site_at(base + begin)
                # The translator appends the original mnemonic as a trailing
                # comment. Alignment-requiring moves (movaps/movapd/movdqa and the
                # non-temporal stores) fault on a misaligned guest address, so they
                # keep an explicit alignment guard; movups/movupd/movdqu do not.
                line_end = source.find('\n', closing)
                tail = source[closing:line_end if line_end >= 0 else len(source)]
                mnemonic = re.search(r"/\*\s*([A-Za-z0-9_.]+)\s*\*/", tail)
                aligned = bool(mnemonic and mnemonic.group(1).lower() in ALIGNED_XMM)
                access = (f"FF_MEMORY_SITE_VECTOR(({address}), 1u, 0x{block:08X}u)" if aligned
                          else f"FF_MEMORY_SITE_POINTER(({address}), 16u, 1u, 0x{block:08X}u)")
                # A generated read must be structurally non-writable: the const
                # lvalue makes any store through it a compile error instead of a
                # silent mutation of guest memory from a read instruction.
                pieces += [source[cursor:begin], f"(*(const RecompXmm*){access})"]
                stats["aligned" if aligned else "unaligned"] = stats.get("aligned" if aligned else "unaligned", 0) + 1
                stats["reads"] += 1; used_sites.add(block); cursor = scan = closing
                continue
            if name == "XMM_STORE":
                inner_start, inner = opening + 1, mask[opening + 1:closing - 1]
                depth = 0; split = None
                for index, char in enumerate(inner):
                    if char == '(': depth += 1
                    elif char == ')': depth -= 1
                    elif char == ',' and depth == 0: split = index; break
                if split is None: raise ValueError("Malformed generated vector store")
                address = rewrite(source[inner_start:inner_start + split], base + inner_start).strip()
                value_start = inner_start + split + 1
                value = rewrite(source[value_start:closing - 1], base + value_start).strip()
                if not address or not value: raise ValueError("Empty generated vector store")
                block = site_at(base + begin)
                line_end = source.find('\n', closing)
                tail = source[closing:line_end if line_end >= 0 else len(source)]
                mnemonic = re.search(r"/\*\s*([A-Za-z0-9_.]+)\s*\*/", tail)
                aligned = bool(mnemonic and mnemonic.group(1).lower() in ALIGNED_XMM)
                lhs = (f"(*(RecompXmm*)FF_MEMORY_SITE_VECTOR(({address}), 2u, 0x{block:08X}u))" if aligned
                       else f"(*(RecompXmm*)FF_MEMORY_SITE_POINTER(({address}), 16u, 2u, 0x{block:08X}u))")
                stats["aligned" if aligned else "unaligned"] = stats.get("aligned" if aligned else "unaligned", 0) + 1
                if "FF_MEMORY_SITE_POINTER" in value or MEMORY_TOKEN.search(value):
                    temp = f"_ff_xmmtmp{xmm_temps[0]}"; xmm_temps[0] += 1
                    pieces += [source[cursor:begin],
                               f"{{ RecompXmm {temp} = {value}; {lhs} = {temp}; }}"]
                else:
                    pieces += [source[cursor:begin], f"{lhs} = ({value})"]
                stats["writes"] += 1; used_sites.add(block); cursor = scan = closing
                continue
            if name not in ACCESSORS: raise ValueError("Unsupported generated memory width")
            before, after = mask[:begin].rstrip(), mask[closing:].lstrip()
            if before.endswith('&'):
                # The pinned integer AND lifter emits "eax = eax & MEM32(...)".
                # Admit only that exact register-update form as a scalar read;
                # unary address-taking and unknown pointer contexts still fail.
                register_and = r"\b(?P<reg>e(?:ax|bx|cx|dx|si|di|bp|sp))\s*=\s*(?P=reg)\s*&$"
                if not re.search(register_and, before):
                    raise ValueError("Generated guest pointer may escape its checked access")
            if after.startswith(('+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<=', '>>=', '++', '--')) or before.endswith(('++', '--')):
                access, key = 3, "readWrites"
            elif after.startswith('=') and not after.startswith('=='):
                access, key = 2, "writes"
            else: access, key = 1, "reads"
            argument = rewrite(source[opening + 1:closing - 1], base + opening + 1)
            if not argument.strip(): raise ValueError("Empty generated memory address")
            block = site_at(base + begin)
            end = statement_end(mask, closing)
            split = key in ("writes", "readWrites") and MEMORY_TOKEN.search(mask[closing:end])
            if split:
                op_start = closing
                while op_start < end and mask[op_start].isspace(): op_start += 1
                operator = next((candidate for candidate in (
                    "<<=", ">>=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "=")
                    if mask.startswith(candidate, op_start)), None)
                if operator is None:
                    snippet = " ".join(source[begin:end].split())
                    raise ValueError(f"Generated write has no split operator: {snippet}")
                rhs_start = op_start + len(operator)
                while rhs_start < end and mask[rhs_start].isspace(): rhs_start += 1
                rewritten_rhs = rewrite(source[rhs_start:end], base + rhs_start)
                if not rewritten_rhs.strip(): raise ValueError("Empty generated write value")
                temp = f"_ff_wtmp{write_temps[0]}"; write_temps[0] += 1
                lhs = f"FF_MEMORY_SITE_{ACCESSORS[name]}(({argument}), {access}u, 0x{block:08X}u)"
                pieces += [source[cursor:begin],
                           f"{{ {TEMP_TYPES[ACCESSORS[name]]} {temp} = {rewritten_rhs}; {lhs} {operator} {temp}; }}"]
                stats[key] += 1; used_sites.add(block)
                cursor = scan = end
                continue
            pieces += [source[cursor:begin],
                       f"FF_MEMORY_SITE_{ACCESSORS[name]}(({argument}), {access}u, 0x{block:08X}u)"]
            stats[key] += 1; used_sites.add(block); cursor = scan = closing
        pieces.append(source[cursor:]); return "".join(pieces)

    converted = rewrite(text, 0)
    scalar = stats["reads"] + stats["writes"] + stats["readWrites"]
    if limit is not None and scalar and not labels:
        raise ValueError("Generated function lacks checked basic-block labels")
    stats["sites"] = len(used_sites)
    for match in re.finditer(r"\b([Xx][Mm][Mm]_[A-Za-z0-9_]+)\s*\(", code_mask(converted)):
        if match.group(1) not in PURE_XMM:
            raise ValueError("Unadmitted raw guest pointer or memory helper")
    if re.search(r"\b(?:XBOX_PTR|MMX_\w+|__atomic_\w+|__sync_\w+)\s*\(", code_mask(converted)):
        raise ValueError("Unadmitted raw guest pointer or memory helper")
    return converted, stats


if __name__ == '__main__':
    import argparse
    from pathlib import Path
    parser = argparse.ArgumentParser(description='Transform the fixed source-owned memory fixture; never reads a title input.')
    parser.add_argument('output', type=Path, help='New source-owned C include; existing files are preserved')
    args = parser.parse_args()
    fixture = Path(__file__).resolve().parents[1] / 'tests/fixtures/memory_access_owned.c'
    converted, counts = instrument_memory_accesses(fixture.read_text(), 0x00100000, 0x00101000)
    with args.output.open('x', newline='\n') as output: output.write(converted)
    print(counts)
