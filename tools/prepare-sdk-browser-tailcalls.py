#!/usr/bin/env python3
"""Lower selected generated instruction sequences for the browser build.

Only exact emitter forms are changed. The retained generated package is read-only.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[1]
SHARD_NAME = re.compile(r"x86_recomp\.(\d+)\.cpp\Z")
TAIL_TRANSFER = re.compile(
    rb"(?P<prefix>\{[ \t]*X86_SAVE_FLAGS\(\);[ \t]*)"
    rb"(?P<call>sub_[0-9A-Fa-f]{8}\(ctx,[ \t]*base\));"
    rb"[ \t]*return;(?P<suffix>[ \t]*\})"
)
FUNCTION_BOUNDARY = re.compile(rb"(?m)^(?:X86_FUNC|X86_WEAK_FUNC|X86_TRACED_FUNC)\(")
EH_MARKER = re.compile(rb"\btry[ \t]*\{|\bcatch[ \t]*\(")
ADDRESS_TERM = rb"(?:0x[0-9A-Fa-f]+u|ctx\.e(?:ax|bx|cx|dx|si|di|bp|sp)(?: \* [1248]u)?)"
ADDRESS = ADDRESS_TERM + rb"(?: \+ " + ADDRESS_TERM + rb"){0,2}"
F32_LOAD = (
    rb"(?P<load>[ \t]+// [0-9A-F]{8} fld[ \t][^\r\n]*\r?\n)"
    rb"(?P<indent>[ \t]+)\{ uint32_t ea = \(uint32_t\)\((?P<left>" + ADDRESS + rb")\);"
    rb" x87_push\(ctx, \(double\)X86_MEMF32\(ea\)\); \}\r?\n"
)
F32_ARITHMETIC = (
    rb"(?P<arithmetic>[ \t]+// [0-9A-F]{8} f(?:add|sub|mul|div)[ \t][^\r\n]*\r?\n)"
    rb"[ \t]+\{ uint32_t ea = \(uint32_t\)\((?P<right>" + ADDRESS + rb")\);"
    rb" double s = \(double\)X86_MEMF32\(ea\); double d = X87_ST\(0\); X87_ST\(0\) ="
    rb" x87_prec\(ctx, \(d (?P<operation>[+*/-]) s\)\); \}\r?\n"
)
MEMORY_F32 = re.compile(
    rb"(?m)^" + F32_LOAD + F32_ARITHMETIC +
    rb"(?P<store>[ \t]+// [0-9A-F]{8} fstp[ \t][^\r\n]*\r?\n)"
    rb"[ \t]+\{ uint32_t ea = \(uint32_t\)\((?P<dst>" + ADDRESS + rb")\);"
    rb" X86_MEMF32\(ea\) = \(float\)X87_ST\(0\); x87_pop\(ctx\); \}(?=\r?$)"
)
COMPARE_F32 = re.compile(
    rb"(?m)^" + F32_LOAD + rb"(?:" + F32_ARITHMETIC + rb")?" +
    rb"(?P<compare>[ \t]+// [0-9A-F]{8} fcomp[ \t][^\r\n]*\r?\n)"
    rb"[ \t]+\{ uint32_t ea = \(uint32_t\)\((?P<limit>" + ADDRESS + rb")\);"
    rb" x87_compare\(ctx, X87_ST\(0\), \(double\)X86_MEMF32\(ea\)\); x87_pop\(ctx\); \}(?=\r?$)"
)
COMPARE_HELPER = ROOT / "runtime/browser/sdk/guest-x87-compare.inc"
MEMORY_RUN_HELPER = ROOT / "runtime/browser/sdk/guest-memory-runs.inc"
MATCHING_BYTES_HELPER = ROOT / "runtime/browser/sdk/guest-matching-bytes.inc"
INTERSECTION_HELPER = ROOT / "runtime/browser/sdk/guest-intersection-spans.inc"
GUEST_REGISTER = rb"ctx\.e(?:ax|bx|cx|dx|si|di|bp|sp)"
MEMORY_WORD = re.compile(
    rb"(?m)^(?P<comment>[ \t]+// [0-9A-F]{8} mov[ \t][^\r\n]*\r?\n)"
    rb"(?P<indent>[ \t]+)\{ uint32_t ea = \(uint32_t\)\((?P<base>" + GUEST_REGISTER
    + rb")(?: \+ 0x(?P<offset>[0-9A-Fa-f]{1,8})u)?\); "
    rb"(?:(?P<load>" + GUEST_REGISTER + rb") = \(uint32_t\)\(X86_MEM32\(ea\)\);|"
    rb"X86_MEM32\(ea\) = \(uint32_t\)\((?P<store>" + GUEST_REGISTER + rb"|0x[0-9A-Fa-f]{1,8}u)\);) \}\r?\n"
)
STACK_REGISTER = rb"ctx\.e(?:ax|bx|cx|dx|si|di|bp)"
PUSH_COMMENT = rb"[ \t]+// [0-9A-F]{8} push[ \t][^\r\n]*\r?\n"
PUSH_OPERAND = rb"(?:0x[0-9A-Fa-f]+u|" + STACK_REGISTER + rb")"
PUSH_WORD = re.compile(
    rb"(" + PUSH_COMMENT + rb")[ \t]+\{ uint32_t v = \(uint32_t\)\((" + PUSH_OPERAND
    + rb")\); X86_PUSH32\(v\); \}\r?\n"
)
CALL_RETURN = re.compile(
    rb"([ \t]+// [0-9A-F]{8} call[ \t][^\r\n]*\r?\n)([ \t]+)X86_PUSH32\((0x[0-9A-Fa-f]+u)\); "
    rb"(sub_[0-9A-Fa-f]{8}\(ctx, base\)|x86_icall\(ctx, base, ctx\.e(?:ax|bx|cx|dx|si|di|bp|sp)\));\r?\n"
)
POP_WORD = re.compile(
    rb"([ \t]+// [0-9A-F]{8} pop[ \t][^\r\n]*\r?\n)[ \t]+\{ uint32_t v = X86_MEM32\(ctx.esp\); "
    rb"ctx.esp \+= 4; (" + STACK_REGISTER + rb") = \(uint32_t\)\(v\); \}\r?\n"
)
PUSH_RUN = re.compile(rb"(?m)^(?P<words>(?:" + PUSH_WORD.pattern + rb")+)(?P<call>" + CALL_RETURN.pattern + rb")?")
POP_RUN = re.compile(rb"(?m)^(?:" + POP_WORD.pattern + rb"){2,}")

INTERSECTION_ENTRY = b"X86_FUNC(__imp__sub_001EA530) {"
INTERSECTION_DIGEST = "4e477fe9e69b75be72f0085e213f3b64d7080316084ec56dda323d3e60895e93"
INTERSECTION_ACCESS = re.compile(
    rb"(?m)^(?P<comment>[ \t]+// (?P<pc>[0-9A-F]{8}) [^\n]*\n)"
    rb"(?P<indent>[ \t]+)\{ uint32_t ea = \(uint32_t\)\((?P<address>[^)\n]+)\); (?P<body>[^\n]*) \}$"
)
INTERSECTION_MEMORY = re.compile(rb"X86_MEM(?P<type>32|F32|8)\(ea\)")
X87_SLOT = re.compile(rb"X87_ST\(([0-7])\)")
X87_PUSH = re.compile(rb"x87_push\(ctx, (.+)\);")
X87_POP = b"x87_pop(ctx);"
X86_BRANCH = re.compile(rb"\bgoto (loc_[0-9A-F]{8});")
X86_LABEL = re.compile(rb"(loc_[0-9A-F]{8}):")
GRID_ENTRY = b"X86_FUNC(__imp__sub_0019EAA0) {"
GRID_DIGEST = "9214230456b79b7634b56cc54ad7c0c5d272611e28b166ceea9fa4351d88f7a7"
GRID_PUSH = re.compile(rb"X86_PUSH32\((v|0x[0-9A-F]+u)\);")


def instruction_comment(opcode: bytes) -> bytes:
    return rb"[ \t]+// [0-9A-F]{8} " + opcode + rb"[ \t][^\r\n]*\r?\n"


def exact_instruction(opcode: bytes, statement: bytes) -> bytes:
    return instruction_comment(opcode) + rb"[ \t]+" + re.escape(statement) + rb"\r?\n"


MATCHING_BYTE_LOOP = re.compile(
    rb"(?m)^(?P<header>(?P<label>loc_[0-9A-F]{8}):\r?\n)(?P<body>"
    + exact_instruction(b"or", b"{ uint8_t a = (uint8_t)(((uint8_t)ctx.eax)); uint8_t b = (uint8_t)(((uint8_t)ctx.eax)); "
                        b"uint8_t r = (uint8_t)(a | b); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(r); "
                        b"cf = 0; of = 0; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }")
    + instruction_comment(b"jz") + rb"[ \t]+if \(zf\) \{ goto loc_[0-9A-F]{8}; \}\r?\n"
    + exact_instruction(b"mov", b"{ uint32_t ea = (uint32_t)(ctx.esi); ctx.eax = (ctx.eax & 0xFFFFFF00u) | (uint8_t)(X86_MEM8(ea)); }")
    + exact_instruction(b"inc", b"{ uint32_t a = (uint32_t)(ctx.esi);  uint32_t r = (uint32_t)(a + 1); ctx.esi = (uint32_t)(r); "
                        b"of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }")
    + exact_instruction(b"mov", b"{ uint32_t ea = (uint32_t)(ctx.edi); ctx.eax = (ctx.eax & 0xFFFF00FFu) | ((uint32_t)(uint8_t)(X86_MEM8(ea)) << 8); }")
    + exact_instruction(b"inc", b"{ uint32_t a = (uint32_t)(ctx.edi);  uint32_t r = (uint32_t)(a + 1); ctx.edi = (uint32_t)(r); "
                        b"of = r == (uint32_t)(1u << 31); zf = r == 0; sf = (uint32_t)(r >> 31) & 1; pf = X86_PARITY(r); }")
    + exact_instruction(b"cmp", b"{ uint8_t a = (uint8_t)(((uint8_t)(ctx.eax >> 8))), b = (uint8_t)(((uint8_t)ctx.eax)); "
                        b"uint8_t r = (uint8_t)(a - b); ca = a; cb = b; cf = a < b; "
                        b"of = (uint32_t)(((a ^ b) & (a ^ r)) >> 7) & 1; zf = r == 0; sf = (uint32_t)(r >> 7) & 1; pf = X86_PARITY(r); }")
    + instruction_comment(b"jz") + rb"[ \t]+if \(\(uint8_t\)ca == \(uint8_t\)cb\) \{ X86_BARRIER\(\); goto )"
    + rb"(?P=label)(?P<end>; \}\r?\n)"
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def under(path: Path, root: Path) -> bool:
    try:
        path.relative_to(root)
        return True
    except ValueError:
        return False


def overlaps(left: Path, right: Path) -> bool:
    return under(left, right) or under(right, left)


def contains_eh(block: bytes) -> bool:
    # Generated comments are not executable C++; ignore them when identifying
    # the emitter's function-wide try/catch wrapper.
    for line in block.splitlines():
        code = line.split(b"//", 1)[0]
        if EH_MARKER.search(code):
            return True
    return False


def lower_memory_f32(match: re.Match[bytes]) -> bytes:
    return (
        match["load"] + match["arithmetic"] + match["store"] + match["indent"]
        + b"ff_browser_x87::MemoryF32<'" + match["operation"] + b"'>(ctx, "
        + b", ".join(b"uint32_t(" + match[operand] + b")" for operand in ("left", "right", "dst")) + b");"
    )


def lower_compare_f32(match: re.Match[bytes]) -> bytes:
    arithmetic = match["operation"]
    operands = ["left", "limit"]
    template = b""
    if arithmetic:
        operands.append("right")
        template = b"<'" + arithmetic + b"'>"
    return (
        match["load"] + (match["arithmetic"] or b"") + match["compare"] + match["indent"]
        + b"ff_browser_x87::CompareF32" + template + b"(ctx, "
        + b", ".join(b"uint32_t(" + match[operand] + b")" for operand in operands) + b");"
    )


def lower_matching_byte_loop(match: re.Match[bytes]) -> bytes:
    end = b"\r\n" if match["header"].endswith(b"\r\n") else b"\n"
    scalar = match["label"] + b"_scalar"
    entry = end.join((
        b"\tif ((uint8_t)ctx.eax != 0) {",
        b"\t    const uint32_t skip = ff_browser_strings::MatchingBytesToSkip(ctx.esi, ctx.edi);",
        b"\t    ctx.esi += skip;",
        b"\t    ctx.edi += skip;",
        b"\t}",
        scalar + b":",
        b"",
    ))
    return match["header"] + entry + match["body"] + scalar + match["end"]


def lower_stack_runs(block: bytes) -> tuple[bytes, int, int]:
    push_count = 0

    def lower_pushes(run: re.Match[bytes]) -> bytes:
        nonlocal push_count
        words = PUSH_WORD.findall(run["words"])
        call = CALL_RETURN.fullmatch(run["call"]) if run["call"] else None
        if len(words) == 1 and not call:
            return run[0]
        push_count += 1
        comments = [comment for comment, _ in words]
        operands = [operand for _, operand in words]
        end = b"\r\n" if run[0].endswith(b"\r\n") else b"\n"
        if call:
            comments.append(call[1])
            operands.append(call[3])
        lowered = b"".join(comments) + b"\tff_browser_stack::Push(ctx, " + b", ".join(operands) + b");" + end
        if call:
            lowered += call[2] + call[4] + b";" + end
        return lowered

    def lower_pops(run: re.Match[bytes]) -> bytes:
        comments, registers = zip(*POP_WORD.findall(run[0]))
        end = b"\r\n" if run[0].endswith(b"\r\n") else b"\n"
        return b"".join(comments) + b"\tff_browser_stack::Pop(ctx, " + b", ".join(registers) + b");" + end

    block = PUSH_RUN.sub(lower_pushes, block)
    block, pop_count = POP_RUN.subn(lower_pops, block)
    return block, push_count, pop_count


def lower_memory_runs(block: bytes) -> tuple[bytes, int, int]:
    pieces = []
    run = []
    cursor = runs = words = 0

    def emit() -> None:
        nonlocal cursor, runs, words
        if len(run) < 2:
            return
        offsets = [int(move["offset"] or b"0", 16) for move in run]
        offsets = [offset - (1 << 32) if offset >= (1 << 31) else offset for offset in offsets]
        if max(offsets) - min(offsets) + 4 > (1 << 26):
            return
        first, last = run[0], run[-1]
        operation = "load" if first["load"] else "store"
        template = b", ".join(str(offset).encode("ascii") for offset in offsets)
        operands = b", ".join(move[operation] for move in run)
        newline = b"\r\n" if last[0].endswith(b"\r\n") else b"\n"
        pieces.append(block[cursor:first.start()])
        pieces.extend(move["comment"] for move in run)
        pieces.append(
            first["indent"] + b"ff_browser_memory_runs::" + operation.title().encode("ascii")
            + b"32<" + template + b">(" + first["base"] + b", " + operands + b");" + newline
        )
        cursor = last.end()
        runs += 1
        words += len(run)

    for move in MEMORY_WORD.finditer(block):
        if run:
            previous = run[-1]
            # There must be no alternate entry and no change to the base
            # before the final instruction of a run.
            if (previous.end() != move.start() or previous["base"] != move["base"]
                    or previous["load"] == previous["base"] or bool(previous["load"]) != bool(move["load"])):
                emit()
                run.clear()
        run.append(move)
    emit()
    pieces.append(block[cursor:])
    return b"".join(pieces), runs, words


def lower_mapped_x87_stack(body: bytes) -> bytes:
    # The mapped body runs without guest faults. Check the logical stack
    # depth at each branch join before deferring writes to the guest context.
    lines = body.splitlines()
    labels = {label[1]: index for index, line in enumerate(lines)
              if (label := X86_LABEL.fullmatch(line.strip()))}
    depths: dict[int, int] = {}
    pending = [(0, 0)]
    while pending:
        index, depth = pending.pop()
        if index in depths:
            if depths[index] != depth:
                raise ValueError("mapped x87 stack depths disagree at a branch join")
            continue
        if not 0 <= index < len(lines):
            raise ValueError("mapped x87 control flow leaves the function")
        depths[index] = depth
        line = lines[index].strip()
        if not line.startswith(b"//"):
            depth += line.count(X87_POP) - len(X87_PUSH.findall(line))
            if b"return;" in line:
                if depth:
                    raise ValueError("mapped x87 stack is unbalanced at return")
                continue
            if match := X86_BRANCH.search(line):
                pending.append((labels[match[1]], depth))
                if line.startswith(b"goto "):
                    continue
        pending.append((index + 1, depth))
    if len(depths) != len(lines):
        raise ValueError("mapped x87 body has unreachable statements")

    slots: set[int] = set()

    def slot(depth: int) -> bytes:
        index = depth & 7
        slots.add(index)
        return f"x87Slot{index}".encode("ascii")

    return_lines = []
    for index, line in enumerate(lines):
        if line.lstrip().startswith(b"//"):
            continue
        depth = depths[index]
        # Source operands of FLD are evaluated at the incoming depth.
        line = X87_SLOT.sub(lambda match: slot(depth + int(match[1])), line)
        line = X87_PUSH.sub(lambda match: slot(depth - 1) + b" = " + match[1] + b";", line)
        line = line.replace(X87_POP, b"")
        if b"x87_status(ctx)" in line:
            top = f"((x87Base + {depth & 7}u) & 7u)".encode("ascii")
            line = line.replace(b"x87_status(ctx)", b"uint16_t((ctx.fsw & ~0x3800u) | (" + top + b" << 11))")
        if b"return;" in line:
            return_lines.append(index)
        lines[index] = line

    if slots != {4, 5, 6, 7} or len(return_lines) != 3:
        raise ValueError("mapped x87 body differs from the reviewed stack layout")
    context_slots = [f"ctx.st[(x87Base + {index}u) & 7u]".encode("ascii") for index in sorted(slots)]
    local_slots = [slot(index) for index in sorted(slots)]
    restore = b" ".join(context + b" = " + local + b";" for context, local in zip(context_slots, local_slots))
    for index in return_lines:
        lines[index] = lines[index].replace(b"return;", restore + b" ctx.top = x87Base; return;")
    prefix = [b"\n\tconst uint32_t x87Base = ctx.top & 7u;"]
    prefix.extend(b"\tdouble " + local + b" = " + context + b";" for context, local in zip(context_slots, local_slots))
    result = b"\n".join(prefix + lines)
    if re.search(rb"\b(?:X87_ST|x87_push|x87_pop|x87_status)\(", result):
        raise ValueError("mapped x87 body contains an unlowered stack access")
    return result


def lower_intersection(block: bytes) -> tuple[bytes, int]:
    if not block.startswith(INTERSECTION_ENTRY):
        return block, 0
    head, closing, _ = block.replace(b"\r\n", b"\n").partition(b"\n}")
    if not closing or sha256(head + closing) != INTERSECTION_DIGEST:
        return block, 0

    argument_loads = {
        0x001EA533: b"leftAddress",
        0x001EA53A: b"rightAddress",
        0x001EA89B: b"resultAddress",
        0x001EA957: b"resultAddress",
    }
    types = {b"32": b"x86_u32u", b"F32": b"x86_f32u", b"8": b"uint8_t"}
    spans = {b"ecx": (b"spans.left", 16), b"edx": (b"spans.right", 16), b"eax": (b"spans.result", 24)}

    def map_access(match: re.Match[bytes]) -> bytes:
        pc = int(match["pc"], 16)
        address = match["address"]
        body = match["body"]
        prefix = match["comment"] + match["indent"]
        if pc in argument_loads:
            body = body.replace(b"X86_MEM32(ea)", b"spans." + argument_loads[pc])
            return prefix + b"{ " + body + b" }"

        base, _, displacement = address.partition(b" + ")
        read_only = base != b"ctx.esp" and base != b"ctx.eax"
        if base == b"ctx.esp":
            pointer, capacity = b"spans.stack", 48
            offset = int(displacement[:-1], 16) if displacement else 0
            # Admit ESP-32. Before the save and after the early success pop,
            # ESP is four bytes above that anchor.
            if pc < 0x001EA540 or 0x001EA910 < pc < 0x001EA92F:
                offset += 4
        elif base.startswith(b"ctx."):
            pointer, capacity = spans[base[4:]]
            offset = int(displacement[:-1], 16) if displacement else 0
        else:
            pointer, capacity = b"ff_browser_memory::backing", 1 << 26
            offset = int(base[:-1], 16)

        def map_memory(access: re.Match[bytes]) -> bytes:
            kind = access["type"]
            width = 1 if kind == b"8" else 4
            if not 0 <= offset <= capacity - width:
                raise ValueError("intersection access exceeds its admitted span")
            qualifier = b"const " if read_only else b""
            return b"(*reinterpret_cast<" + qualifier + types[kind] + b"*>(" + pointer + b" + " + str(offset).encode() + b"))"

        body, count = INTERSECTION_MEMORY.subn(map_memory, body)
        if not count:
            raise ValueError("intersection instruction has no recognized memory access")
        return prefix + b"{ " + body + b" }"

    body, count = INTERSECTION_ACCESS.subn(map_access, head[len(INTERSECTION_ENTRY):])
    body = body.replace(b"X86_PUSH32(v)", b"ctx.esp -= 4; *reinterpret_cast<x86_u32u*>(spans.stack) = v")
    body = body.replace(b"X86_MEM32(ctx.esp)", b"*reinterpret_cast<const x86_u32u*>(spans.stack)")
    if count != 198 or re.search(rb"\bX86_(?:MEM|PUSH|POP|PTR)", body):
        raise ValueError("intersection body contains unmapped memory operations")
    body = lower_mapped_x87_stack(body)

    fast = (b"[[gnu::always_inline]] static inline void ff_browser_mapped_intersection(\n"
            b"    X86Context& __restrict ctx, const ff_browser_intersection::Spans& spans) noexcept {"
            + body + closing + b"\n\n")
    entry = (INTERSECTION_ENTRY + b"\n\tff_browser_intersection::Spans spans;\n"
             b"\tif (ff_browser_intersection::TrySpans(ctx.esp, spans)) [[likely]] {\n"
             b"\t\tff_browser_mapped_intersection(ctx, spans);\n\t\treturn;\n\t}")
    return fast + entry + block[len(INTERSECTION_ENTRY):], 1


def lower_grid_arguments(block: bytes) -> tuple[bytes, int]:
    if not block.startswith(GRID_ENTRY):
        return block, 0
    head, closing, tail = block.replace(b"\r\n", b"\n").partition(b"\n}")
    if not closing or sha256(head + closing) != GRID_DIGEST:
        return block, 0

    start_label = b"\nloc_0019EB80:"
    start = head.index(start_label) + len(start_label)
    end = head.index(b"\n\t// 0019EBAE ", start)
    checked = head[start:end]
    stores = 0

    def map_store(match: re.Match[bytes]) -> bytes:
        nonlocal stores
        address = match["address"]
        if not address.startswith(b"ctx.esp + "):
            return match[0]
        offset = int(address.rsplit(b" ", 1)[1][:-1], 16) + 16
        if int(match["pc"], 16) > 0x0019EB9A:
            offset -= 4  # One argument has already been pushed.
        if not 0 <= offset <= 0x4c:
            raise ValueError("grid call store leaves the admitted stack span")
        target = f"(*reinterpret_cast<x86_u32u*>(gridStack + {offset}))".encode("ascii")
        stores += 1
        body = match["body"].replace(b"X86_MEM32(ea)", target)
        return match["comment"] + match["indent"] + b"{ " + body + b" }"

    mapped = INTERSECTION_ACCESS.sub(map_store, checked)
    cursor = 16

    def map_push(match: re.Match[bytes]) -> bytes:
        nonlocal cursor
        cursor -= 4
        pointer = f"gridStack + {cursor}".encode("ascii")
        return b"ctx.esp -= 4; *reinterpret_cast<x86_u32u*>(" + pointer + b") = " + match[1] + b";"

    mapped, pushes = GRID_PUSH.subn(map_push, mapped)
    if stores != 4 or pushes != 4:
        raise ValueError("grid call differs from the reviewed argument layout")

    setup = b"\n\tuint32_t gridStackEsp = 0;\n\tuint8_t* gridStack = nullptr;"
    guard = (b"\n\tif (gridStackEsp != ctx.esp) {\n\t\tgridStackEsp = ctx.esp;\n"
             b"\t\tgridStack = ff_browser_memory::TryWindow(gridStackEsp - 16, 0x50);\n\t}"
             b"\n\tif (gridStack) [[likely]] {")
    # The existing call and its binding remain in both paths. The cached
    # host pointer covers only argument stores, and is refreshed on ESP changes.
    mapped = mapped.replace(b"\n\t", b"\n\t\t")
    checked = checked.replace(b"\n\t", b"\n\t\t")
    return (GRID_ENTRY + setup + head[len(GRID_ENTRY):start] + guard + mapped
            + b"\n\t} else {" + checked + b"\n\t}" + head[end:] + closing + tail), 1


def adapt_source(original: bytes) -> tuple[bytes, int, int, int, int, int, int, int, int, int, int, int, int]:
    markers = list(FUNCTION_BOUNDARY.finditer(original))
    pieces = []
    cursor = 0
    transforms = 0
    excluded_eh_transfers = 0
    eh_functions = 0
    f32_transforms = 0
    f32_compares = 0
    push_runs = pop_runs = 0
    memory_runs = memory_words = 0
    matching_byte_loops = 0
    mapped_intersections = 0
    mapped_grid_calls = 0

    for index, marker in enumerate(markers):
        start = marker.start()
        end = markers[index + 1].start() if index + 1 < len(markers) else len(original)
        block = original[start:end]
        pieces.append(original[cursor:start])

        if block.startswith(b"X86_FUNC("):
            block, count = lower_intersection(block)
            mapped_intersections += count
            block, count = lower_grid_arguments(block)
            mapped_grid_calls += count
            if contains_eh(block):
                excluded_eh_transfers += len(TAIL_TRANSFER.findall(block))
                eh_functions += 1
            else:
                block, count = TAIL_TRANSFER.subn(
                    lambda match: (
                        match.group("prefix")
                        + b"[[clang::musttail]] return "
                        + match.group("call")
                        + b";"
                        + match.group("suffix")
                    ),
                    block,
                )
                transforms += count

            block, count = MEMORY_F32.subn(lower_memory_f32, block)
            f32_transforms += count
            block, count = COMPARE_F32.subn(lower_compare_f32, block)
            f32_compares += count
            block, count = MATCHING_BYTE_LOOP.subn(lower_matching_byte_loop, block)
            matching_byte_loops += count
            block, pushes, pops = lower_stack_runs(block)
            push_runs += pushes
            pop_runs += pops
            block, runs, words = lower_memory_runs(block)
            memory_runs += runs
            memory_words += words
        pieces.append(block)
        cursor = end

    pieces.append(original[cursor:])
    matched = len(TAIL_TRANSFER.findall(original))
    if matched != transforms + excluded_eh_transfers:
        raise ValueError(
            "direct-return transfers were found outside recognized generated function definitions"
        )
    adapted = b"".join(pieces)
    if f32_transforms:
        adapted = b'#include "guest-x87.h"\n' + adapted
    if push_runs or pop_runs:
        adapted = b'#include "guest-stack.h"\n' + adapted
    if f32_compares:
        helper_digest = sha256(COMPARE_HELPER.read_bytes()).encode("ascii")
        adapted = b'// x87 comparison helper SHA-256: ' + helper_digest + b'\n#include "guest-x87-compare.inc"\n' + adapted
    if memory_runs:
        helper_digest = sha256(MEMORY_RUN_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Memory run helper SHA-256: ' + helper_digest + b'\n#include "guest-memory-runs.inc"\n' + adapted
    if matching_byte_loops:
        helper_digest = sha256(MATCHING_BYTES_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Matching bytes helper SHA-256: ' + helper_digest + b'\n#include "guest-matching-bytes.inc"\n' + adapted
    if mapped_intersections:
        helper_digest = sha256(INTERSECTION_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Intersection spans helper SHA-256: ' + helper_digest + b'\n#include "guest-intersection-spans.inc"\n' + adapted
    if mapped_grid_calls:
        adapted = b'#include "full-memory.h"\n' + adapted
    return (adapted, transforms, excluded_eh_transfers, eh_functions, f32_transforms,
            push_runs, pop_runs, f32_compares, memory_runs, memory_words, matching_byte_loops,
            mapped_grid_calls, mapped_intersections)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--generated",
        type=Path,
        default=ROOT / "research/verification/sdk-title-bulk-command-20261107/generated",
        help="retained source package (read-only)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / ".tools/artifacts/sdk-browser-full/title-overlay",
        help="ignored destination for adapted title sources",
    )
    args = parser.parse_args()

    generated = args.generated.resolve()
    output = args.output.resolve()
    allowed_outputs = [
        (ROOT / ".tools/artifacts").resolve(),
        (ROOT / "artifacts").resolve(),
    ]
    if not generated.is_dir():
        parser.error(f"generated source directory does not exist: {generated}")
    if not any(output != root and under(output, root) for root in allowed_outputs):
        parser.error("--output must be a strict subdirectory of this worktree's .tools/artifacts or artifacts directory")
    if overlaps(output, generated):
        parser.error("--output must not overlap the retained generated source directory")

    shards = []
    for path in generated.iterdir():
        match = SHARD_NAME.fullmatch(path.name)
        if match and path.is_file() and not path.is_symlink():
            shards.append((int(match.group(1)), path))
    shards.sort(key=lambda item: item[0])
    if [number for number, _ in shards] != list(range(64)):
        parser.error("expected exactly x86_recomp.0.cpp through x86_recomp.63.cpp")

    package = [path for _, path in shards]
    for name in ("x86_func_mapping.cpp", "x86_recomp_shared.h"):
        path = generated / name
        if not path.is_file() or path.is_symlink():
            parser.error(f"required generated file is missing or not a regular file: {path}")
        package.append(path)

    output.mkdir(parents=True, exist_ok=True)
    records = []
    total_transforms = 0
    total_excluded_eh_transfers = 0
    total_f32_transforms = 0
    total_f32_compares = 0
    total_push_runs = total_pop_runs = 0
    total_memory_runs = total_memory_words = 0
    total_matching_byte_loops = 0
    total_mapped_intersections = 0
    total_mapped_grid_calls = 0
    for source in package:
        original = source.read_bytes()
        (adapted, count, excluded_eh_transfers, eh_functions,
         f32_transforms, push_runs, pop_runs, f32_compares, memory_runs, memory_words,
         matching_byte_loops, mapped_grid_calls, mapped_intersections) = adapt_source(original)
        destination = output / source.name
        temporary = destination.with_name(destination.name + ".tmp")
        temporary.write_bytes(adapted)
        temporary.replace(destination)
        records.append(
            {
                "name": source.name,
                "originalSha256": sha256(original),
                "transformCount": count,
                "excludedEHTransferCount": excluded_eh_transfers,
                "EHFunctionCount": eh_functions,
                "x87MemoryF32Count": f32_transforms,
                "x87CompareF32Count": f32_compares,
                "stackPushRunCount": push_runs,
                "stackPopRunCount": pop_runs,
                "memoryWordRunCount": memory_runs,
                "memoryWordsInRuns": memory_words,
                "matchingByteLoopCount": matching_byte_loops,
                "mappedIntersectionCount": mapped_intersections,
                "mappedGridCallCount": mapped_grid_calls,
                "adaptedSha256": sha256(adapted),
            }
        )
        total_transforms += count
        total_excluded_eh_transfers += excluded_eh_transfers
        total_f32_transforms += f32_transforms
        total_f32_compares += f32_compares
        total_push_runs += push_runs
        total_pop_runs += pop_runs
        total_memory_runs += memory_runs
        total_memory_words += memory_words
        total_matching_byte_loops += matching_byte_loops
        total_mapped_intersections += mapped_intersections
        total_mapped_grid_calls += mapped_grid_calls

    if not total_transforms:
        parser.error("no exact direct-return transfers matched; no adaptation was emitted")

    manifest = {
        "schemaVersion": 1,
        "transformation": "X86_SAVE_FLAGS + sub_XXXXXXXX(ctx, base); return; to clang::musttail return",
        "sourceDirectory": str(generated.relative_to(ROOT)) if under(generated, ROOT) else "external-local-path",
        "shardCount": len(shards),
        "totalTransforms": total_transforms,
        "excludedEHTransfers": total_excluded_eh_transfers,
        "x87MemoryF32Count": total_f32_transforms,
        "x87CompareF32Count": total_f32_compares,
        "stackPushRunCount": total_push_runs,
        "stackPopRunCount": total_pop_runs,
        "memoryWordRunCount": total_memory_runs,
        "memoryWordsInRuns": total_memory_words,
        "matchingByteLoopCount": total_matching_byte_loops,
        "mappedIntersectionCount": total_mapped_intersections,
        "mappedGridCallCount": total_mapped_grid_calls,
        "files": records,
    }
    manifest_path = output / "tailcall-manifest.json"
    manifest_tmp = manifest_path.with_name(manifest_path.name + ".tmp")
    manifest_tmp.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    manifest_tmp.replace(manifest_path)
    print(f"Prepared {len(shards)} title shards with {total_transforms} musttail transfers "
          f"and {total_f32_transforms} x87 memory sequences in {output}")
    print(f"Combined {total_push_runs} push runs and {total_pop_runs} pop runs")
    print(f"Combined {total_memory_words} memory moves into {total_memory_runs} runs")
    print(f"Added checked prefix scanning to {total_matching_byte_loops} byte comparison loops")
    print(f"Hoisted memory checks for {total_mapped_intersections} segment intersection bodies")
    print(f"Reused stack spans at {total_mapped_grid_calls} grid intersection calls")
    print(f"Combined {total_f32_compares} x87 comparison sequences")
    print(f"Manifest: {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
