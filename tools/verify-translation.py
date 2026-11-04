#!/usr/bin/env python3
"""Verify that a prepared translation is fully instrumented by the current guard.

Reads one private `title-functions.inc`, splits it on generated function headers,
and runs `tools/recomp_memory_access.instrument_memory_accesses` over each function
with its own address range. A fully guarded translation must come back byte for
byte unchanged with zero rewritten sites; anything else means the file still holds
unguarded memory surfaces or was produced by a different emitter.

Prints counts only. A missing directory, a missing file or any rewritten site is
an error. Never reproduces original code.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import recomp_memory_access as guard  # noqa: E402

FUNCTION = re.compile(r"void ff_title_([0-9A-F]{8})\(void\)")


def split_functions(text):
    """Return (address, text) pairs for each generated function, in order."""
    starts = [(int(match.group(1), 16), match.start()) for match in FUNCTION.finditer(text)]
    parts = []
    for index, (address, start) in enumerate(starts):
        end = starts[index + 1][1] if index + 1 < len(starts) else len(text)
        parts.append((address, text[start:end]))
    return parts


def verify(text):
    functions = split_functions(text)
    if not functions:
        raise ValueError("No generated functions found")
    problems = []
    rewritten = 0
    for index, (address, body) in enumerate(functions):
        labels = [int(value, 16) for value in re.findall(r"loc_([0-9A-F]{8})", body)]
        following = max([address + 4] + [value + 4 for value in labels] +
                        ([functions[index + 1][0]] if index + 1 < len(functions) else []))
        instrumented, counts = guard.instrument_memory_accesses(body, address, following)
        if instrumented != body:
            rewritten += 1
            problems.append((address, counts))
    return {"functions": len(functions), "rewritten": rewritten, "problems": problems}


def main(argv):
    if not argv or argv[0].startswith("--"):
        print("Usage: python tools/verify-translation.py <translation-directory>", file=sys.stderr)
        return 2
    path = Path(argv[0]) / "title-functions.inc"
    if not path.is_file():
        print(f"No translation found at {path}. The translation is a private, player-local artifact.", file=sys.stderr)
        return 2
    report = verify(path.read_text(encoding="utf-8", errors="replace"))
    print(f"functions: {report['functions']}")
    print(f"functions that needed rewriting: {report['rewritten']}")
    for address, counts in report["problems"][:20]:
        print(f"  0x{address:08X} {counts}")
    if report["rewritten"]:
        print("The translation is not fully instrumented by the current guard.", file=sys.stderr)
        return 1
    print("The translation is fully instrumented by the current guard.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
