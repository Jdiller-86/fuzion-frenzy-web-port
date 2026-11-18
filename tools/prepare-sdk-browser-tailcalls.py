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
MEMORY_F32 = re.compile(
    rb"(?m)^(?P<load>[ \t]+// [0-9A-F]{8} fld[ \t][^\r\n]*\r?\n)"
    rb"(?P<indent>[ \t]+)\{ uint32_t ea = \(uint32_t\)\((?P<left>" + ADDRESS + rb")\);"
    rb" x87_push\(ctx, \(double\)X86_MEMF32\(ea\)\); \}\r?\n"
    rb"(?P<arithmetic>[ \t]+// [0-9A-F]{8} f(?:add|sub|mul|div)[ \t][^\r\n]*\r?\n)"
    rb"[ \t]+\{ uint32_t ea = \(uint32_t\)\((?P<right>" + ADDRESS + rb")\);"
    rb" double s = \(double\)X86_MEMF32\(ea\); double d = X87_ST\(0\); X87_ST\(0\) ="
    rb" x87_prec\(ctx, \(d (?P<operation>[+*/-]) s\)\); \}\r?\n"
    rb"(?P<store>[ \t]+// [0-9A-F]{8} fstp[ \t][^\r\n]*\r?\n)"
    rb"[ \t]+\{ uint32_t ea = \(uint32_t\)\((?P<dst>" + ADDRESS + rb")\);"
    rb" X86_MEMF32\(ea\) = \(float\)X87_ST\(0\); x87_pop\(ctx\); \}(?=\r?$)"
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


def adapt_source(original: bytes) -> tuple[bytes, int, int, int, int]:
    markers = list(FUNCTION_BOUNDARY.finditer(original))
    pieces = []
    cursor = 0
    transforms = 0
    excluded_eh_transfers = 0
    eh_functions = 0
    f32_transforms = 0

    for index, marker in enumerate(markers):
        start = marker.start()
        end = markers[index + 1].start() if index + 1 < len(markers) else len(original)
        block = original[start:end]
        pieces.append(original[cursor:start])

        if block.startswith(b"X86_FUNC("):
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
    return adapted, transforms, excluded_eh_transfers, eh_functions, f32_transforms


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
    for source in package:
        original = source.read_bytes()
        adapted, count, excluded_eh_transfers, eh_functions, f32_transforms = adapt_source(original)
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
                "adaptedSha256": sha256(adapted),
            }
        )
        total_transforms += count
        total_excluded_eh_transfers += excluded_eh_transfers
        total_f32_transforms += f32_transforms

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
        "files": records,
    }
    manifest_path = output / "tailcall-manifest.json"
    manifest_tmp = manifest_path.with_name(manifest_path.name + ".tmp")
    manifest_tmp.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    manifest_tmp.replace(manifest_path)
    print(f"Prepared {len(shards)} title shards with {total_transforms} musttail transfers "
          f"and {total_f32_transforms} x87 memory sequences in {output}")
    print(f"Manifest: {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
