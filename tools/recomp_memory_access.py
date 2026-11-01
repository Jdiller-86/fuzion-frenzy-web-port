"""Conservative memory-access instrumentation for the clean pinned C emitter.

This observes/validates accesses; it does not replace guest instructions. Only
recognized lvalue, stack and bulk-pointer surfaces are admitted. Unknown pointer
escapes/helpers fail instead of silently bypassing guest memory permissions.
"""
import re

ACCESSORS = {"MEM8": "U8", "MEM16": "U16", "MEM32": "U32", "MEM64": "U64",
             "SMEM8": "I8", "SMEM16": "I16", "SMEM32": "I32", "SMEM64": "I64",
             "MEMF": "F32", "MEMD": "F64"}


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


def instrument_memory_accesses(text):
    stats = {"reads": 0, "writes": 0, "readWrites": 0, "stack": 0, "bulkRanges": 0}
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

    def rewrite(source):
        mask = code_mask(source); pieces, cursor, scan = [], 0, 0
        while match := re.search(r"\b(?:S?MEM\w*|PUSH32|POP32)\b", mask[scan:]):
            begin, end = scan + match.start(), scan + match.end()
            name = source[begin:end]
            opening = end
            while opening < len(mask) and mask[opening].isspace(): opening += 1
            if opening >= len(mask) or mask[opening] != '(':
                raise ValueError("Unexpected generated memory token")
            if name in ("PUSH32", "POP32"):
                pieces += [source[cursor:begin], "FF_MEMORY_PUSH" if name == "PUSH32" else "FF_MEMORY_POP"]
                cursor = scan = end; stats["stack"] += 1; continue
            if name not in ACCESSORS: raise ValueError("Unsupported generated memory width")
            depth, closing = 1, opening + 1
            while closing < len(mask) and depth:
                depth += (mask[closing] == '(') - (mask[closing] == ')'); closing += 1
            if depth: raise ValueError("Unclosed generated memory expression")
            before, after = mask[:begin].rstrip(), mask[closing:].lstrip()
            if before.endswith('&'): raise ValueError("Generated guest pointer may escape its checked access")
            if after.startswith(('+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<=', '>>=', '++', '--')) or before.endswith(('++', '--')):
                access, key = 3, "readWrites"
            elif after.startswith('=') and not after.startswith('=='):
                access, key = 2, "writes"
            else: access, key = 1, "reads"
            argument = rewrite(source[opening + 1:closing - 1])
            if not argument.strip(): raise ValueError("Empty generated memory address")
            pieces += [source[cursor:begin], f"FF_MEMORY_{ACCESSORS[name]}(({argument}), {access}u)"]
            stats[key] += 1; cursor = scan = closing
        pieces.append(source[cursor:]); return "".join(pieces)

    converted = rewrite(text)
    if re.search(r"\b(?:XBOX_PTR|XMM_\w+|MMX_\w+|__atomic_\w+|__sync_\w+)\s*\(", code_mask(converted)):
        raise ValueError("Unadmitted raw guest pointer or memory helper")
    return converted, stats


if __name__ == '__main__':
    import argparse
    from pathlib import Path
    parser = argparse.ArgumentParser(description='Transform the fixed source-owned memory fixture; never reads a title input.')
    parser.add_argument('output', type=Path, help='New source-owned C include; existing files are preserved')
    args = parser.parse_args()
    fixture = Path(__file__).resolve().parents[1] / 'tests/fixtures/memory_access_owned.c'
    converted, counts = instrument_memory_accesses(fixture.read_text())
    with args.output.open('x', newline='\n') as output: output.write(converted)
    print(counts)
