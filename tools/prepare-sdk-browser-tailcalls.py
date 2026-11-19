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
LOADER_BYTES_HELPER = ROOT / "runtime/browser/sdk/guest-loader-bytes.inc"
SCENE_LINES_HELPER = ROOT / "runtime/browser/sdk/guest-scene-lines.inc"
GUEST_BINDING_HELPER = ROOT / "runtime/browser/sdk/guest-function-binding.inc"
CODE_WARMUP_HELPER = ROOT / "runtime/browser/sdk/guest-code-warmup.h"
CODE_WARMUP_PROFILE = ROOT / "profiles/browser-code-warmup.json"
CODE_WARMUP_EXTRA_PROFILE = ROOT / "profiles/browser-code-warmup-extra.json"
CODE_WARMUP_BATCH_LIMIT = 256
CODE_WARMUP_EXTRA_LIMIT = 64
REPEATED_LINE_HELPER = ROOT / "runtime/browser/sdk/guest-repeated-line.inc"
DECIMAL_PREFIX_HELPER = ROOT / "runtime/browser/sdk/guest-decimal-prefix.inc"
INTERSECTION_HELPER = ROOT / "runtime/browser/sdk/guest-intersection-spans.inc"
GRID_REJECTION_HELPER = ROOT / "runtime/browser/sdk/guest-grid-rejection.inc"
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
GRID_WEAK_WRAPPER = b"X86_WEAK_FUNC(sub_001EA530, __imp__sub_001EA530)"
GRID_DEFAULT_WRAPPER = b'''extern "C" __attribute__((weak, noinline)) void sub_001EA530(
    X86Context& __restrict ctx, uint8_t* base) {
    if (auto* stack = ff_browser_grid_batch(ctx)) {
        ff_browser_intersection::Spans spans;
        if (ff_browser_intersection::TryMappedGridSpans(stack, ctx.esp, spans)) {
            ff_browser_mapped_intersection(ctx, spans);
            return;
        }
    }
    [[clang::musttail]] return __imp__sub_001EA530(ctx, base);
}'''
LOADER_BYTE_LOOPS = (
    (b"X86_FUNC(__imp__sub_001DEFE6) {",
     "4b6878305f59d689c55829f1f65de8f07a30bd11642f1cde8370ef62cf9794b3",
     b"\nloc_001DF012:",
     b"\n\tif (ff_browser_integer::TryAccumulate(ctx)) goto loc_001DF021;"),
    (b"X86_FUNC(__imp__sub_001E70BE) {",
     "d0c503fa178bfd1612d0742e1e0f0bb9f6842b5a7a6db0ada1974353ccd285b8",
     b"\n\tX86_LOAD_FLAGS();",
     b"\n\tif (ff_browser_loader_bytes::TryUnreadStringByte(ctx)) return;"),
    (b"X86_FUNC(__imp__sub_000C3A50) {",
     "668122795a062bb108eb9111f2ba35c19f59201a7df63ba05ea62284c53f71e3",
     b"\n\t// 000C3A5B jmp 0x000C3A60",
     b"\n\t{ const auto ending = ff_browser_loader_bytes::SkipCopiedLineEnding("
     b"ctx.ecx + 8, ctx.eax, ctx.esi, ctx.esp);"
     b" if (ending.next) { ctx.edx = ending.next; ctx.eax = ending.eax; cf = 0;"
     b" goto loc_000C3A7B; } }"),
    (b"X86_FUNC(__imp__sub_001B9E00) {",
     "a6e3f41638c48a4cb94676a266a659bca446ad3d9a619389820cacf3df2c0020",
     b"\nloc_001B9E10:",
     b"\n\t{"
     b" const auto line = ff_browser_loader_bytes::CopyLine(ctx.eax, cb - ctx.eax, ctx.edx, ctx.ecx + 8u);"
     b" ctx.edx += line.copied;"
     # A matched delimiter clears CF. The unchanged cursor INC in the
     # epilogue replaces the other status flags.
     b" if (line.complete) { ctx.eax = line.delimiterEax; cf = 0; goto loc_001B9E2C; }"
     b" }"),
    (b"X86_FUNC(__imp__sub_001DF0F0) {",
     "339fa7becfb6f256dc1cc33fbce8315ab1eb40803fbad2d301a11e65dd6ca353",
     b"\n\tX86_LOAD_FLAGS();",
     b"\n\tif (const auto result = ff_browser_loader_bytes::TrySceneSubstring(ctx.esp, ctx.esi, ctx.ebx, ctx.edi);"
     b" result.complete) {"
     b" ctx.ecx = result.needleCursor; ctx.edx = (ctx.edx & 0xffff0000u) | result.pair;"
     b" ctx.eax = result.address; ctx.esp += 4; return; }"),
    (b"X86_FUNC(__imp__sub_001DF0F0) {",
     "339fa7becfb6f256dc1cc33fbce8315ab1eb40803fbad2d301a11e65dd6ca353",
     b"\nloc_001DF119:",
     b"\n\t{"
     b" const auto match = ff_browser_loader_bytes::SearchPairBytes(ctx.esi, uint16_t(ctx.edx));"
     b" ctx.esi += match.advance;"
     b" if (match.atEnd) { ctx.eax &= ~0xffu; goto loc_001DF124; }"
     b" }"),
    (b"X86_FUNC(__imp__sub_001DF0F0) {",
     "339fa7becfb6f256dc1cc33fbce8315ab1eb40803fbad2d301a11e65dd6ca353",
     b"\nloc_001E0376:",
     b"\n\tctx.edx += ff_browser_loader_bytes::SearchAsciiWordsToSkip(ctx.edx, uint8_t(ctx.ebx));"),
    (b"X86_FUNC(__imp__sub_001E0340) {",
     "c8173e63306279b86355645edbd0204e545403115d650dba4630f36f35a4ca1a",
     b"\nloc_001E0376:",
     b"\n\tctx.edx += ff_browser_loader_bytes::SearchAsciiWordsToSkip(ctx.edx, uint8_t(ctx.ebx));"),
    (b"X86_FUNC(__imp__sub_001DF984) {",
     "c638b1818fe9c23fbd41a717b30ee8cdc8c2f6096dfaecb17f782453a85ec715",
     b"\nloc_001DF99D:",
     b"\n\tctx.eax += ff_browser_loader_bytes::SearchBytesToSkip(ctx.eax, 0);"),
    (b"X86_FUNC(__imp__sub_001E0FA8) {",
     "c5d1d0cc1283b0ee476735e51d117307f0e71fc99755fa4081dcc73062166c1a",
     b"\nloc_001E0FE3:",
     b"\n\tctx.eax += ff_browser_loader_bytes::SearchBytesToSkip(ctx.eax, 0);"),
    (b"X86_FUNC(__imp__sub_0007E890) {",
     "03d98b82563c453c132a3567adea99e117c5e474e9dc20fd213501f894ca7f2f",
     b"\nloc_0007E8A0:",
     b"\n\tctx.ecx += ff_browser_loader_bytes::CopyStringPrefix(ctx.ecx, ctx.esi + ctx.ecx);"),
    (b"X86_FUNC(__imp__sub_0007E890) {",
     "03d98b82563c453c132a3567adea99e117c5e474e9dc20fd213501f894ca7f2f",
     b"\nloc_0007E8F0:",
     b"\n\t{ const auto prefix = ff_browser_loader_bytes::NormalizeTreeLinePrefix("
     b"ctx.edx, ctx.esi, ctx.esp, uint8_t(ctx.ecx));"
     b" if (prefix.consumed) { ctx.edx += prefix.consumed; ctx.esi += prefix.written;"
     b" ctx.ecx = (ctx.ecx & ~0xffu) | prefix.nextByte; } }"),
    (b"X86_FUNC(__imp__sub_001BA160) {",
     "139c04c537c741d53fabd4ac130966be65c16adeaf2f6f2f24333f1007782807",
     b"\nloc_001BA355:",
     b"\n\tctx.eax = ctx.edx + ff_browser_loader_bytes::SearchBytesToSkip(ctx.edx, 0);"),
)
SCENE_LINE_PRIMITIVES = {21: b"000C3A50", 49: b"001B9E00", 53: b"001DF0F0"}
SCENE_PARSER_SHARD = 49
SCENE_PARSER = b"001BA160"
DECIMAL_BODY = b"001E7F47"
DECIMAL_HELPERS = (b"001E7E6D", b"001E7E8E", b"001E7EEC")
DECIMAL_BODY_DIGESTS = {
    b"001E79F6": "7a2bff745924f4f5e4c0921f178c0a28a1ce78b84a7cf3c5624667a3c034150f",
    b"001E7E6D": "076a8ec275260a83da54c08cbb019ed36aa0311f6c6f853b7ac9e6aaffe5e214",
    b"001E7E8E": "5740aa5f67b3323fa0ca90cc3ed097f725157654cd9bb5dc12d4f6cfc22ebde6",
    b"001E7EEC": "746abce9877b680dce19e36999cf255e9cc9176c301f6ed925ddb1d758ba1d11",
    DECIMAL_BODY: "85caf750a084033da695fef927c7d8b74d513554982ccc3a1e333bd2ab4fd41e",
}
DECIMAL_PRODUCT_BODY = b"001E84D7"
DECIMAL_PRODUCT_DIGEST = "53b5a3cce47f8641d1f856ea9213f1852c99fd3debdd68d6d4c3544b3f511c52"
DECIMAL_PRODUCT_CALLER = b"001E86F9"
DECIMAL_PRODUCT_CALLER_DIGEST = "e85c95b6030c4acb7dc53a0b0751ee2751037e3625b4148cdb1679ad41c219cd"
DECIMAL_CONVERSION_CALLERS = {
    b"001E6678": "75bb29c2da1fd0da19c7bd7a722209bb84bc6de00ce3918db01d0db3ff65c3a0",
    b"001E6723": "ea182b36ad292211b1b22fd58889f3f8cf9d6f97955e6524c6766e92486860f7",
}
DECIMAL_CONVERSION_HELPERS = {
    b"001E65C9": ("03b09a06efb75accd2655db6b46976215d312b030f20c71803c85f56426515e9", b"CopyConversionMantissa"),
    b"001E65FD": ("3e751a40410a804cf975b758b3ee048a2e29c019d31e08766f294e90bfffd200", b"ShiftConversionMantissa"),
    b"001E6557": ("21d50b27d27c3e3877d1a29ba686f1c9e2a2a512716418ac001c6d8f276393c2", b"TruncateConversionMantissa"),
}
DECIMAL_ROUND_HELPERS = {
    b"001E64D8": "fd2b08ed933b47b05fbca4bcb43b95223822549653e8d7459fa1ac3918166baf",
    b"001E650A": "ae6334a71572133a99b44018bb7d9776f7789c8dfbe041bc3c2fdac84d0331d3",
}
REPEATED_LINE_ENTRY = b"X86_FUNC(__imp__sub_0007F08E) {"
REPEATED_LINE_DIGEST = "d0459384057379296516e727f0dd42875ed504afbc0f073fa7844052f18db17c"
SCANNER_DIGEST = "e168ac6f3c74e7d67eeda4356e26e73bdea86537d92607cea9e64be627471772"
SCANNER_GET_DIGEST = "355f8c787eb54cc135218de7ce162659c3bf40b331eacf7c0ffa08491377ea61"


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


def lower_loader_byte_loop(block: bytes) -> tuple[bytes, int]:
    matches = [record for record in LOADER_BYTE_LOOPS if block.startswith(record[0])]
    if not matches:
        return block, 0
    head, closing, tail = block.replace(b"\r\n", b"\n").partition(b"\n}")
    if not closing:
        return block, 0
    body_digest = sha256(head + closing)
    count = 0
    for _, digest, label, prefix in matches:
        if body_digest != digest:
            continue
        if head.count(label) != 1:
            raise ValueError("loader fast prefix entry differs from its reviewed loop")
        # Insert on entry. The original backedge stays in the scalar loop.
        head = head.replace(label, prefix + label, 1)
        count += 1
    return (head + closing + tail, count) if count else (block, 0)


def lower_repeated_line(block: bytes) -> tuple[bytes, int]:
    if not block.startswith(REPEATED_LINE_ENTRY):
        return block, 0
    head, closing, tail = block.replace(b"\r\n", b"\n").partition(b"\n}")
    if not closing or sha256(head + closing) != REPEATED_LINE_DIGEST:
        return block, 0
    head = head.replace(b"\tX86_LOAD_FLAGS();", b"\tX86_LOAD_FLAGS();\n\tuint32_t repeatedLinePrefix = 0;", 1)
    head = head.replace(
        b"\nloc_0007F046:",
        b"\n\t{ const uint32_t advance = ff_browser_loader_bytes::CopyBoundedLinePrefix(ctx.eax, ctx.esi, ctx.edx);"
        b" ctx.eax += advance; ctx.edx += advance; }\nloc_0007F046:", 1)
    head = head.replace(
        b"\nloc_0007F05A:",
        b"\nloc_0007F05A:\n\trepeatedLinePrefix = ff_browser_loader_bytes::RepeatedLinePrefix("
        b"ctx.edi, ctx.eax, ctx.esi, ctx.esp + 0x3Cu, ctx.ebx + 0x10Cu);", 1)
    head = head.replace(
        b"\nloc_0007F063:",
        b"\n\tif (repeatedLinePrefix) { ctx.eax += repeatedLinePrefix; ctx.ecx = ctx.esi; }"
        b"\nloc_0007F063:", 1)
    head = head.replace(
        b"\nloc_0007F0D0:",
        b"\n\tctx.eax += ff_browser_loader_bytes::SearchBytesToSkip(ctx.eax, 0);"
        b"\nloc_0007F0D0:", 1)
    head = head.replace(
        b"\nloc_0007F104:",
        b"\n\tctx.eax += ff_browser_loader_bytes::CopyStringPrefix(ctx.eax, ctx.edx + ctx.eax);"
        b"\nloc_0007F104:", 1)
    return head + closing + tail, 1


def matched_body(source: bytes, address: bytes, digest: str) -> bytes | None:
    entry = b"X86_FUNC(__imp__sub_" + address + b") {"
    start = source.find(entry)
    if start < 0:
        return None
    end = source.find(b"\n}", start)
    if end < 0:
        return None
    body = source[start:end + 2]
    return body if sha256(body) == digest else None


def prepare_code_warmup(original: bytes, adapted: bytes, records: dict[str, str]) -> tuple[bytes, list[bytes], int]:
    original = original.replace(b"\r\n", b"\n")
    admitted = []
    declarations = []
    alias_count = 0
    for address, digest in records.items():
        if not re.fullmatch(r"[0-9A-F]{8}", address) or not re.fullmatch(r"[0-9a-f]{64}", digest):
            raise ValueError("invalid guest code warmup target")
        key = address.encode("ascii")
        if matched_body(original, key, digest) is None:
            continue
        public = b"sub_" + key
        name = b"__imp__" + public
        entry = b"X86_FUNC(" + name + b") {"
        if adapted.count(entry) != 1:
            raise ValueError("guest code warmup boundary differs from the reviewed source")
        # Keep both the real calls and the startup call on the same Wasm
        # body. The weak public entry can still be overridden by the SDK.
        declarations.append(b'extern "C" __attribute__((noinline)) void ' + name + b'(X86Context&, uint8_t*);')
        adapted = adapted.replace(entry, entry + b"\n\tif (ff_browser_code_warmup::IsWarmup(ctx)) return;", 1)
        # Untouched default forwarders can share the implementation's Wasm body.
        # Custom wrappers still do their own checks before invoking that body.
        wrapper = b"\nX86_WEAK_FUNC(" + public + b", " + name + b")\n"
        if original.count(wrapper) == 1 and adapted.count(wrapper) == 1:
            alias = b'\nX86_EXTERN_FUNC(' + public + b') __attribute__((weak, alias("' + name + b'")));\n'
            adapted = adapted.replace(wrapper, alias, 1)
            alias_count += 1
        admitted.append(name)
    if admitted:
        digest = sha256(CODE_WARMUP_HELPER.read_bytes()).encode("ascii")
        adapted = (b'// Guest code warmup helper SHA-256: ' + digest + b'\n#include "guest-code-warmup.h"\n'
                   + b"\n".join(declarations) + b"\n" + adapted)
    return adapted, admitted, alias_count


def emit_code_warmup(names: list[bytes], symbol: bytes) -> bytes:
    entry = b'\nextern "C" unsigned ' + symbol + b'() {\n'
    if not names:
        return entry + b"    return 0;\n}\n"
    declarations = b"".join(b"X86_EXTERN_FUNC(" + name + b");\n" for name in names)
    return (b'\n#include "guest-code-warmup.h"\n' + declarations + entry +
            b'    using Function = void (*)(X86Context&, uint8_t*);\n'
            b'    static constexpr Function functions[]{' + b', '.join(names) + b'};\n'
            b'    for (Function function : functions) function(ff_browser_code_warmup::context, nullptr);\n'
            b'    return sizeof(functions) / sizeof(functions[0]);\n}\n')


def scene_line_batch_available(shards: list[tuple[int, Path]]) -> bool:
    # The owner, parser and all three skipped primitives must match together.
    prefix = b"X86_FUNC(__imp__sub_"
    loader_digests = {entry.removeprefix(prefix)[:8]: digest for entry, digest, _, _ in LOADER_BYTE_LOOPS}
    owner = shards[48][1].read_bytes().replace(b"\r\n", b"\n")
    if matched_body(owner, b"001B2C30", "5a8b0cfc1e120f5c92690539e544bd6119fe8cbce8b7e6a35edbfdc3e19c8c28") is None:
        return False
    for shard, address in SCENE_LINE_PRIMITIVES.items():
        source = shards[shard][1].read_bytes().replace(b"\r\n", b"\n")
        wrapper = b"X86_WEAK_FUNC(sub_" + address + b", __imp__sub_" + address + b")"
        if matched_body(source, address, loader_digests[address]) is None or source.count(wrapper) != 1:
            return False
        if shard == SCENE_PARSER_SHARD and matched_body(source, SCENE_PARSER, loader_digests[SCENE_PARSER]) is None:
            return False
    return True


def uses_default_implementation(address: bytes) -> bytes:
    return (b"ff_browser_guest_binding::UsesDefaultImplementation(sub_" + address +
            b", __imp__sub_" + address + b")")


def lower_scene_line_batches(source: bytes, shard: int) -> tuple[bytes, int]:
    address = SCENE_LINE_PRIMITIVES.get(shard)
    if address is None:
        return source, 0
    symbol = b"sub_" + address
    implementation = b"__imp__" + symbol
    wrapper = b"X86_WEAK_FUNC(" + symbol + b", " + implementation + b")"
    alias = (b'extern "C" void ' + symbol + b'(X86Context&, uint8_t*) '
             b'__attribute__((weak, alias("' + implementation + b'")));')
    source = source.replace(wrapper, alias, 1)
    if shard != SCENE_PARSER_SHARD:
        return source, 0

    source = source.replace(b"\r\n", b"\n")
    header = b'#include "x86_recomp_shared.h"\n'
    if source.count(header) != 1 or b'#include "guest-loader-bytes.inc"' not in source:
        raise ValueError("scene parser declarations differ from the reviewed build")
    defaults = list(SCENE_LINE_PRIMITIVES.values())
    declarations = b"".join(b"X86_EXTERN_FUNC(__imp__sub_" + address + b");\n" for address in defaults)
    guard = b" && ".join(uses_default_implementation(address) for address in defaults)
    digest = sha256(SCENE_LINES_HELPER.read_bytes()).encode("ascii")
    source = source.replace(header, header + b"// Scene line helper SHA-256: " + digest +
                            b'\n#include "guest-scene-lines.inc"\n' + declarations, 1)
    entry = b"X86_FUNC(__imp__sub_" + SCENE_PARSER + b") {"
    if source.count(entry) != 1:
        raise ValueError("scene parser entry differs from the reviewed build")
    source = source.replace(entry, entry + b"\n\tconst bool batchSceneLines = " + guard + b";", 1)
    for label, args in ((b"loc_001BA192:", b"false, (ctx.ebx & 255) != 0"),
                        (b"loc_001BA2A4:", b"true, false")):
        if source.count(label) != 1:
            raise ValueError("scene parser loop boundary differs from the reviewed build")
        source = source.replace(label, label + b"\n\tif (batchSceneLines)\n"
                                b"\t\tff_browser_loader_bytes::SkipQuietSceneLines(ctx.esp, ctx.esi, " + args + b");", 1)
    return source, 2


def lower_decimal_prefix(source: bytes) -> tuple[bytes, int]:
    if b"X86_FUNC(__imp__sub_" + DECIMAL_BODY + b") {" not in source:
        return source, 0
    normalized = source.replace(b"\r\n", b"\n")
    bodies = {}
    for address, digest in DECIMAL_BODY_DIGESTS.items():
        body = matched_body(normalized, address, digest)
        if body is None:
            return source, 0
        bodies[address] = body
    for address in DECIMAL_HELPERS:
        symbol = b"sub_" + address
        implementation = b"__imp__" + symbol
        wrapper = b"X86_WEAK_FUNC(" + symbol + b", " + implementation + b")"
        if normalized.count(wrapper) != 1:
            return source, 0
        # Preserve strong overrides and let the fast path identify the
        # default implementations by function address at runtime.
        declaration = (b'extern "C" void ' + symbol + b'(X86Context&, uint8_t*) '
                       b'__attribute__((weak, alias("' + implementation + b'")));')
        normalized = normalized.replace(wrapper, declaration, 1)
    body = bodies[DECIMAL_BODY]
    defaults = b" && ".join(uses_default_implementation(address) for address in DECIMAL_HELPERS)
    lowered = body.replace(b"\tX86_LOAD_FLAGS();", b"\tX86_LOAD_FLAGS();\n\tuint8_t* decimalFrame = nullptr;", 1)
    lowered = lowered.replace(
        b"\nloc_001E7F6E:",
        b"\n\tif (" + defaults + b")\n"
        b"\t\tdecimalFrame = ff_browser_decimal::AccumulateMantissa(ctx.ebp, ctx.esp, ctx.ebx, ctx.eax, ctx.df);"
        b"\n\tif (decimalFrame) {"
        b" ctx.eax = 0; ctx.ecx = ff_browser_decimal::ReadWord(decimalFrame + ff_browser_decimal::kMantissaOffset + 8);"
        b" ctx.edx = ctx.ebx + 8; ctx.esi = ctx.ebx + 12; ctx.edi = ctx.ebp - 4;"
        b" goto ff_decimal_mantissa_ready; }"
        b"\nloc_001E7F6E:", 1)
    lowered = lowered.replace(b"\n\t// 001E7FB8 xor edx, edx", b"\nff_decimal_mantissa_ready:\n\t// 001E7FB8 xor edx, edx", 1)
    lowered = lowered.replace(b"\nloc_001E7FEE:", b"\nloc_001E7FEE:\n"
                              b"\tif (decimalFrame) ff_browser_decimal::NormalizePrefix(decimalFrame);", 1)
    return normalized.replace(body, lowered, 1), 1


def lower_decimal_product(source: bytes) -> tuple[bytes, int]:
    # Called after the shared add helper has its checked weak binding.
    body = matched_body(source, DECIMAL_PRODUCT_BODY, DECIMAL_PRODUCT_DIGEST)
    if body is None or matched_body(source, DECIMAL_PRODUCT_CALLER, DECIMAL_PRODUCT_CALLER_DIGEST) is None:
        return source, 0
    label = b"\nloc_001E8599:"
    exit_point = b"\n\t// 001E85FD "
    entry = (b"\n\tif (" + uses_default_implementation(b"001E7E6D") +
             b" && ff_browser_decimal::CompleteMantissaProduct(ctx)) goto ff_mantissa_product_ready;")
    lowered = body.replace(label, entry + label, 1).replace(
        exit_point, b"\nff_mantissa_product_ready:" + exit_point, 1)
    return source.replace(body, lowered, 1), 1


def lower_decimal_conversion_helpers(source: bytes) -> tuple[bytes, int]:
    if any(matched_body(source, address, digest) is None for address, digest in DECIMAL_CONVERSION_CALLERS.items()):
        return source, 0
    count = 0
    rounding_check = b"false"
    rounding_declaration = b""
    truncate_address = b"001E6557"
    if (matched_body(source, truncate_address, DECIMAL_CONVERSION_HELPERS[truncate_address][0]) is not None
            and all(matched_body(source, address, digest) is not None for address, digest in DECIMAL_ROUND_HELPERS.items())):
        aliases = []
        for address in DECIMAL_ROUND_HELPERS:
            symbol = b"sub_" + address
            implementation = b"__imp__" + symbol
            wrapper = b"X86_WEAK_FUNC(" + symbol + b", " + implementation + b")"
            if source.count(wrapper) != 1:
                break
            alias = (b'extern "C" void ' + symbol + b'(X86Context&, uint8_t*) '
                     b'__attribute__((weak, alias("' + implementation + b'")));')
            aliases.append((wrapper, alias))
        else:
            for wrapper, alias in aliases:
                source = source.replace(wrapper, alias, 1)
            rounding_check = b" && ".join(uses_default_implementation(address)
                                          for address in (*DECIMAL_ROUND_HELPERS, b"001E7E6D"))
            rounding_declaration = b"X86_EXTERN_FUNC(__imp__sub_001E7E6D);\n"
    for address, (digest, helper) in DECIMAL_CONVERSION_HELPERS.items():
        body = matched_body(source, address, digest)
        if body is None:
            continue
        # The entry binding remains intact. Only the verified default body
        # handles the fast case, ahead of its original faultable operations.
        head = b"X86_FUNC(__imp__sub_" + address + b") {"
        args = b"ctx"
        if helper == b"TruncateConversionMantissa":
            args += b", [] { return " + rounding_check + b"; }"
        entry = head + b"\n\tif (ff_browser_decimal::" + helper + b"(" + args + b")) return;"
        if helper == b"TruncateConversionMantissa":
            entry = rounding_declaration + entry
        source = source.replace(body, body.replace(head, entry, 1), 1)
        count += 1
    return source, count


def lower_scanner_word(source: bytes) -> tuple[bytes, int]:
    if b"X86_FUNC(__imp__sub_001E3C87) {" not in source:
        return source, 0
    normalized = source.replace(b"\r\n", b"\n")
    scanner = matched_body(normalized, b"001E3C87", SCANNER_DIGEST)
    getter = matched_body(normalized, b"001E3C71", SCANNER_GET_DIGEST)
    wrapper = b"X86_WEAK_FUNC(sub_001E3C71, __imp__sub_001E3C71)"
    entry = b"\nloc_001E4162:"
    if scanner is None or getter is None or normalized.count(wrapper) != 1 or scanner.count(entry) != 1:
        return source, 0
    alias = (b'extern "C" void sub_001E3C71(X86Context&, uint8_t*) '
             b'__attribute__((weak, alias("__imp__sub_001E3C71")));')
    # Only run on entry to a field. The scalar backedge bypasses the guard.
    lifted = scanner.replace(
        entry,
        b"\n\tif (" + uses_default_implementation(b"001E3C71") + b")"
        b"\n\t\tctx.ebx += ff_browser_loader_bytes::SkipSuppressedWord(ctx.ebp, ctx.esp);" + entry,
        1,
    )
    getter_entry = b"X86_FUNC(__imp__sub_001E3C71) {"
    read = getter.replace(getter_entry,
                          getter_entry + b"\n\tif (ff_browser_loader_bytes::TryReadStreamByte(ctx)) return;", 1)
    return normalized.replace(scanner, lifted, 1).replace(getter, read, 1).replace(wrapper, alias, 1), 2


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


def grid_rejection_source(leaf: bytes) -> bytes | None:
    leaf = leaf.replace(b"\r\n", b"\n")
    start = leaf.find(INTERSECTION_ENTRY)
    if start < 0 or leaf.count(GRID_WEAK_WRAPPER) != 1:
        return None
    head, closing, _ = leaf[start:].partition(b"\n}")
    if not closing or sha256(head + closing) != INTERSECTION_DIGEST:
        return None
    instructions = {
        int(match[1], 16): match[2]
        for match in re.finditer(rb"(?m)^[ \t]+// ([0-9A-F]{8}) [^\n]+\n[ \t]+([^\n]+)", head)
    }

    def arithmetic(ranges: tuple[tuple[int, int], ...], memory: dict[int, bytes], point_base: int,
                   swap_segments: bool = False) -> None:
        # Lift the arithmetic from the checksum-matched emitter. Stores in
        # these straight-line ranges are private float conversions only.
        fpu: list[bytes] = []

        def read(operand: re.Match[bytes]) -> bytes:
            register, displacement = operand.groups()
            offset = int(displacement, 16) if displacement else 0
            if register == b"esp":
                return b"double(" + memory[offset] + b")"
            if register == b"edx":
                offset -= point_base
            if offset not in ((0, 4) if register == b"edx" else (0, 4, 8, 12)):
                raise ValueError("grid predicate coordinate is outside its admitted span")
            base = b"query" if register == b"ecx" else b"point"
            return b"double(ff_browser_grid::ReadFloat(" + base + b" + " + str(offset).encode() + b"))"

        for pc, instruction in instructions.items():
            if not any(low <= pc <= high for low, high in ranges):
                continue
            if swap_segments:
                instruction = re.sub(rb"ctx\.(ecx|edx)\b",
                                     lambda m: b"ctx.edx" if m[1] == b"ecx" else b"ctx.ecx", instruction)
            operand = re.search(rb"ea = \(uint32_t\)\(ctx\.(ecx|edx|esp)(?: \+ 0x([0-9A-Fa-f]+)u)?\);", instruction)
            if operand and b"x87_push(ctx, (double)X86_MEMF32(ea))" in instruction:
                fpu.append(read(operand))
            elif operation := re.search(rb"x87_prec\(ctx, \(d ([+*\-]) s\)\)", instruction):
                if operand:
                    right = read(operand)
                elif X87_POP in instruction and b"X87_ST(1) =" in instruction:
                    right = fpu.pop()
                else:
                    raise ValueError("unexpected x87 arithmetic in the admitted grid prefix")
                left = fpu.pop()
                if operation[1] in (b"+", b"*"):
                    # The endpoint and swapped-segment paths commute these operands.
                    # Only ordered, finite side values may admit a skip.
                    left, right = sorted((left, right))
                fpu.append(b"x87_prec(ctx, (" + left + b" " + operation[1] + b" " + right + b"))")
            elif operand and operand[1] == b"esp" and b"X86_MEMF32(ea) = (float)X87_ST(0)" in instruction:
                memory[int(operand[2], 16)] = b"float(" + fpu[-1] + b")"
                if X87_POP in instruction:
                    fpu.pop()
            else:
                raise ValueError("unexpected instruction in the admitted grid prefix")
        if fpu:
            raise ValueError("grid predicate has unbalanced x87 arithmetic")

    line: dict[int, bytes] = {}
    opposite_line: dict[int, bytes] = {}
    arithmetic(((0x001EA545, 0x001EA54B), (0x001EA562, 0x001EA578)), line, 0)
    arithmetic(((0x001EA558, 0x001EA55E), (0x001EA621, 0x001EA637)), opposite_line, 0, True)
    offsets = (4, 0x14, 0x28)
    opposite_offsets = (0x28, 0x1c, 8)
    if (set(line) != set(offsets) or set(opposite_line) != set(opposite_offsets) or
            tuple(line[key] for key in offsets) != tuple(opposite_line[key] for key in opposite_offsets)):
        raise ValueError("grid line terms differ when swapping the two segments")
    initial = {offset: f"terms[{index}]".encode() for index, offset in enumerate(offsets)}
    first, second = initial.copy(), initial.copy()
    arithmetic(((0x001EA57C, 0x001EA593),), first, 0)
    arithmetic(((0x001EA59F, 0x001EA5B7),), second, 8)
    opposite = {offset: f"terms[{index}]".encode() for index, offset in enumerate(opposite_offsets)}
    opposite_first, opposite_second = opposite.copy(), opposite.copy()
    arithmetic(((0x001EA63B, 0x001EA652),), opposite_first, 0, True)
    arithmetic(((0x001EA65E, 0x001EA676),), opposite_second, 8, True)
    if not first[0x0c] == second[8] == opposite_first[0x0c] == opposite_second[8]:
        raise ValueError("grid endpoint paths differ beyond commuted arithmetic")
    # Scalar and vector operands use the selected SDK precision. Float stores
    # in the line expressions still round at the original scratch boundaries.
    side = first[0x0c]
    vector_side = side.removeprefix(b"float(").removesuffix(b")")
    vector_side = re.sub(rb"double\(ff_browser_grid::ReadFloat\(point \+ ([04])\)\)",
                         lambda match: b"x" if match[1] == b"0" else b"y", vector_side)
    vector_side = re.sub(rb"double\(terms\[([012])\]\)",
                         lambda match: (b"a", b"b", b"c")[int(match[1])], vector_side)
    vector_side = vector_side.replace(b"x87_prec(ctx, ", b"(")
    if vector_side != b"((c + ((((x * b)) + ((y * a))))))":
        raise ValueError("grid endpoint expression differs from the checked contour proof")
    typed_line = [line[key].replace(b"double(", b"Real(").replace(b"x87_prec(ctx, ", b"(") for key in offsets]
    constants = []
    for comparisons in ((0x001EA5C7, 0x001EA5D8, 0x001EA686, 0x001EA697),
                        (0x001EA5E9, 0x001EA603, 0x001EA6A8, 0x001EA6C2)):
        addresses = [re.search(rb"ea = \(uint32_t\)\((0x[0-9A-F]+u)\);", instructions[pc]) for pc in comparisons]
        if not all(addresses) or len({match[1] for match in addresses}) != 1:
            raise ValueError("grid endpoint comparisons use different constants")
        constants.append(addresses[0][1])
    return (b"static uint8_t* ff_browser_grid_batch(X86Context& ctx) noexcept {\n"
            b"    const auto lineTerms = []<typename Real>(const uint8_t* query) {\n"
            b"        return std::array<float, 3>{" + b", ".join(typed_line) + b"};\n    };\n"
            b"    const auto vectorSide = [](auto x, auto y, auto a, auto b, auto c) { return " + vector_side + b"; };\n"
            b"    return ff_browser_grid::BatchRejectedEdges(ctx, lineTerms, vectorSide, 0x0019EBAEu, "
            + b", ".join(constants) + b");\n}\n")


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

    setup = (b"\n\tff_browser_grid::ContourScope contourScope(ctx);"
             b"\n\tuint32_t gridStackEsp = 0;\n\tuint8_t* gridStack = nullptr;")
    guard = (b"\n\tif (gridStackEsp != ctx.esp) {\n\t\tgridStackEsp = ctx.esp;\n"
             b"\t\tgridStack = ff_browser_memory::TryWindow(gridStackEsp - 16, 0x50);\n\t}"
             b"\n\tif (gridStack) [[likely]] {")
    # The existing call and its binding remain in both paths. The cached
    # host pointer covers only argument stores, and is refreshed on ESP changes.
    mapped = mapped.replace(b"\n\t", b"\n\t\t")
    checked = checked.replace(b"\n\t", b"\n\t\t")
    return (GRID_ENTRY + setup + head[len(GRID_ENTRY):start] + guard + mapped
            + b"\n\t} else {" + checked + b"\n\t}" + head[end:] + closing + tail), 1


def adapt_source(original: bytes, grid_predicate: bytes | None = None
                 ) -> tuple[bytes, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int]:
    original, decimal_prefixes = lower_decimal_prefix(original)
    decimal_products = 0
    decimal_conversions = 0
    if decimal_prefixes:
        original, decimal_products = lower_decimal_product(original)
        original, decimal_conversions = lower_decimal_conversion_helpers(original)
    original, loader_byte_loops = lower_scanner_word(original)
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
    repeated_lines = 0
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
            block, count = lower_loader_byte_loop(block)
            loader_byte_loops += count
            block, count = lower_repeated_line(block)
            repeated_lines += count
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
    if loader_byte_loops and not repeated_lines:
        helper_digest = sha256(LOADER_BYTES_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Loader byte loops helper SHA-256: ' + helper_digest + b'\n#include "guest-loader-bytes.inc"\n' + adapted
    if repeated_lines:
        helper_digest = sha256(LOADER_BYTES_HELPER.read_bytes() + REPEATED_LINE_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Repeated line helper SHA-256: ' + helper_digest + b'\n#include "guest-repeated-line.inc"\n' + adapted
    if b"ff_browser_integer::TryAccumulate(ctx)" in adapted:
        adapted = b'#include "guest-integer.h"\n' + adapted
    if decimal_prefixes:
        helper_digest = sha256(DECIMAL_PREFIX_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Decimal prefix helper SHA-256: ' + helper_digest + b'\n#include "guest-decimal-prefix.inc"\n' + adapted
    if mapped_intersections:
        helper_digest = sha256(INTERSECTION_HELPER.read_bytes()).encode("ascii")
        adapted = b'// Intersection spans helper SHA-256: ' + helper_digest + b'\n#include "guest-intersection-spans.inc"\n' + adapted
    if mapped_grid_calls:
        adapted = b'#include "guest-grid-contours.h"\n' + adapted
    if mapped_intersections and grid_predicate is not None:
        digest = sha256(GRID_REJECTION_HELPER.read_bytes()).encode("ascii")
        adapted = (b'// Grid rejection helper SHA-256: ' + digest + b'\n#include "guest-grid-rejection.inc"\n'
                   + grid_predicate + adapted.replace(GRID_WEAK_WRAPPER, GRID_DEFAULT_WRAPPER))
    return (adapted, transforms, excluded_eh_transfers, eh_functions, f32_transforms,
            push_runs, pop_runs, f32_compares, memory_runs, memory_words, matching_byte_loops,
            loader_byte_loops, repeated_lines, decimal_prefixes, decimal_products, decimal_conversions,
            mapped_grid_calls, mapped_intersections)


def instrument_scene_timing(original: bytes, source: bytes, shard: int) -> bytes:
    identities = {
        14: '3eb3b65c203bd8d50870ec7c599612a17f0bccd7072b3cd1e734e82bd230534f',
        48: '4cf909c61b1465dc6ac0f6fa17f62061da558d69d18c9c0d189cb83f10668004',
    }
    if shard not in identities:
        return source
    if sha256(original) != identities[shard]:
        raise ValueError(f'Scene timing requires the verified source for title shard {shard}')

    callers = {
        14: [('0007F2F0', '0007EFE0', 'Reader', 1)],
        48: [('001B6170', '001B43C0', 'Loader', 1),
             ('001B6170', '0019F0F0', 'Grid', 1),
             ('001B6170', '0019EC50', 'Grid', 1),
             ('001B43C0', '001B4310', 'Tree', 1),
             ('001B43C0', '001B2C30', 'TwoPass', 1),
             ('001B2C30', '001BA160', 'Parser', 13)],
    }
    for caller, callee, phase, expected in callers[shard]:
        entry = f'X86_FUNC(__imp__sub_{caller}) {{'.encode()
        if source.count(entry) != 1:
            raise ValueError(f'Scene timing caller differs: {caller}')
        start = source.index(entry) + len(entry)
        end = source.index(b'\n}', start)
        call = rb'(?m)^\t(X86_PUSH32\(0x[0-9A-F]+u\); sub_' + callee.encode() + rb'\(ctx, base\);)\r?$'
        scope = f'ff_browser_scene_timing::Scope sceneTiming{{ff_browser_scene_timing::{phase}}};'.encode()
        timed, matches = re.subn(call, lambda match: b'\t{\n\t\t' + scope + b'\n\t\t' + match[1] + b'\n\t}',
                                 source[start:end])
        if matches != expected:
            raise ValueError(f'Scene timing edge differs: {caller} to {callee} ({matches} calls)')
        source = source[:start] + timed + source[end:]

    if shard == 48:
        entry = b'X86_FUNC(__imp__sub_001B6170) {'
        source = source.replace(entry, b'''X86_FUNC(ff_browser_timed_scene_original);
X86_FUNC(__imp__sub_001B6170) {
\tff_browser_scene_timing::Scope sceneTiming{ff_browser_scene_timing::Scene};
\tff_browser_timed_scene_original(ctx, base);
}
X86_FUNC(ff_browser_timed_scene_original) {''', 1)
    helper = ROOT / 'runtime/browser/sdk/guest-scene-timing.h'
    return b'// Scene timing helper SHA-256: ' + sha256(helper.read_bytes()).encode() + b'\n#include "guest-scene-timing.h"\n' + source


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
    parser.add_argument('--scene-timing', action='store_true', help='Instrument verified scene setup calls')
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
    total_loader_byte_loops = 0
    total_scene_line_batches = 0
    total_repeated_lines = 0
    total_decimal_prefixes = 0
    total_decimal_products = 0
    total_decimal_conversions = 0
    total_mapped_intersections = 0
    total_mapped_grid_calls = 0
    total_grid_reject_batches = 0
    grid_predicate = None
    grid = shards[46][1].read_bytes()
    grid_index = grid.find(GRID_ENTRY)
    if grid_index >= 0 and lower_grid_arguments(grid[grid_index:])[1] == 1:
        grid_predicate = grid_rejection_source(shards[54][1].read_bytes())
    scene_lines = scene_line_batch_available(shards)
    shard_numbers = {path: number for number, path in shards}
    warmup_batches: list[list[bytes]] = [[], []]
    warmup_profiles = []
    warmup_addresses = set()
    for profile_path, limit in ((CODE_WARMUP_PROFILE, CODE_WARMUP_BATCH_LIMIT),
                                (CODE_WARMUP_EXTRA_PROFILE, CODE_WARMUP_EXTRA_LIMIT)):
        profile = json.loads(profile_path.read_text(encoding="utf-8"))
        addresses = [address for records in profile.values() for address in records]
        if len(addresses) > limit:
            raise ValueError("guest code warmup target limit exceeded")
        if len(set(addresses)) != len(addresses) or warmup_addresses.intersection(addresses):
            raise ValueError("duplicate guest code warmup target")
        warmup_addresses.update(addresses)
        warmup_profiles.append(profile)
    warmup_alias_count = 0
    for source in package:
        original = source.read_bytes()
        (adapted, count, excluded_eh_transfers, eh_functions,
         f32_transforms, push_runs, pop_runs, f32_compares, memory_runs, memory_words,
         matching_byte_loops, loader_byte_loops, repeated_lines, decimal_prefixes, decimal_products, decimal_conversions,
         mapped_grid_calls, mapped_intersections) = adapt_source(original, grid_predicate)
        scene_line_batches = 0
        if scene_lines:
            adapted, scene_line_batches = lower_scene_line_batches(adapted, shard_numbers.get(source, -1))
        for profile, names in zip(warmup_profiles, warmup_batches):
            warmup_records = profile.get(str(shard_numbers.get(source)))
            if warmup_records:
                adapted, admitted, alias_count = prepare_code_warmup(original, adapted, warmup_records)
                names.extend(admitted)
                warmup_alias_count += alias_count
        if b"ff_browser_guest_binding::UsesDefaultImplementation(" in adapted:
            digest = sha256(GUEST_BINDING_HELPER.read_bytes()).encode("ascii")
            adapted = b'// Guest binding helper SHA-256: ' + digest + b'\n#include "guest-function-binding.inc"\n' + adapted
        if source.name == "x86_func_mapping.cpp":
            adapted += emit_code_warmup(warmup_batches[0], b"ff_browser_warm_guest_code")
            adapted += emit_code_warmup(warmup_batches[1], b"ff_browser_warm_guest_code_extra")
        if args.scene_timing:
            adapted = instrument_scene_timing(original, adapted, shard_numbers.get(source, -1))
        grid_reject_batches = mapped_intersections if grid_predicate is not None else 0
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
                "loaderByteLoopCount": loader_byte_loops,
                "sceneLineBatchCount": scene_line_batches,
                "repeatedLineCount": repeated_lines,
                "decimalPrefixCount": decimal_prefixes,
                "decimalProductCount": decimal_products,
                "decimalConversionHelperCount": decimal_conversions,
                "mappedIntersectionCount": mapped_intersections,
                "mappedGridCallCount": mapped_grid_calls,
                "gridRejectBatchCount": grid_reject_batches,
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
        total_loader_byte_loops += loader_byte_loops
        total_scene_line_batches += scene_line_batches
        total_repeated_lines += repeated_lines
        total_decimal_prefixes += decimal_prefixes
        total_decimal_products += decimal_products
        total_decimal_conversions += decimal_conversions
        total_mapped_intersections += mapped_intersections
        total_mapped_grid_calls += mapped_grid_calls
        total_grid_reject_batches += grid_reject_batches

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
        "loaderByteLoopCount": total_loader_byte_loops,
        "sceneLineBatchCount": total_scene_line_batches,
        "repeatedLineCount": total_repeated_lines,
        "decimalPrefixCount": total_decimal_prefixes,
        "decimalProductCount": total_decimal_products,
        "decimalConversionHelperCount": total_decimal_conversions,
        "mappedIntersectionCount": total_mapped_intersections,
        "mappedGridCallCount": total_mapped_grid_calls,
        "gridRejectBatchCount": total_grid_reject_batches,
        "guestCodeWarmupCount": sum(map(len, warmup_batches)),
        "guestCodeWarmupExtraCount": len(warmup_batches[1]),
        "guestCodeWarmupAliasCount": warmup_alias_count,
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
    print(f"Added checked byte scanning to {total_loader_byte_loops} loader loops")
    print(f"Batched unneeded iterations in {total_scene_line_batches} scene line loops")
    print(f"Reused line scans in {total_repeated_lines} loader routines")
    print(f"Added checked decimal arithmetic to {total_decimal_prefixes} loader routines")
    print(f"Grouped {total_decimal_products} truncated decimal products")
    print(f"Added mapped mantissa operations to {total_decimal_conversions} conversion helpers")
    print(f"Hoisted memory checks for {total_mapped_intersections} segment intersection bodies")
    print(f"Reused stack spans at {total_mapped_grid_calls} grid intersection calls")
    print(f"Added proven early-rejection batches to {total_grid_reject_batches} grid calls")
    print(f"Prepared {sum(map(len, warmup_batches))} guest code warmup targets with {warmup_alias_count} public aliases")
    print(f"Combined {total_f32_compares} x87 comparison sequences")
    print(f"Manifest: {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
