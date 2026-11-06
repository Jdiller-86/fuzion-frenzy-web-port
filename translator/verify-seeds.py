#!/usr/bin/env python3
"""Verify that the seeds offered for a recomposition match a prepared translation.

A translation records the SHA-256 of every seed it was built from. Before a new
admission batch is composed it is worth proving that the seeds in hand are the
same bytes, so a silent edit cannot change the admitted function set while the
recorded translation still claims the old inputs.

Usage:
  python translator/verify-seeds.py --translation <directory> --seed <file> [--seed <file> ...]

Prints counts, mismatched digests and their recorded indices only; it never
reproduces seed contents. A missing translation or seed, a duplicate seed, a
missing recorded digest or an unexpected seed is an error.
"""
import argparse
import hashlib
import json
import sys
from pathlib import Path


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main(argv):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--translation", required=True, type=Path)
    parser.add_argument("--seed", required=True, action="append", type=Path)
    arguments = parser.parse_args(argv)

    record = arguments.translation / "generation.json"
    if not record.is_file():
        print(f"No generation record at {record}.", file=sys.stderr)
        return 2
    recorded = json.loads(record.read_text(encoding="utf-8")).get("seedSha256s")
    if not isinstance(recorded, list) or not recorded or any(
            not isinstance(value, str) or len(value) != 64 for value in recorded):
        print("The generation record has no usable seed digest list.", file=sys.stderr)
        return 2
    if len(set(recorded)) != len(recorded):
        print("The generation record itself lists a duplicate seed digest.", file=sys.stderr)
        return 1

    offered = {}
    for path in arguments.seed:
        if not path.is_file():
            print(f"Missing seed: {path}", file=sys.stderr)
            return 2
        value = digest(path)
        if value in offered:
            print(f"Duplicate seed: {path} matches {offered[value]}", file=sys.stderr)
            return 1
        offered[value] = path

    expected = set(recorded)
    unexpected = sorted(value for value in offered if value not in expected)
    missing = sorted(expected - set(offered), key=recorded.index)
    index = {value: position for position, value in enumerate(recorded)}
    first = recorded[0]

    print(f"recorded seeds: {len(recorded)}")
    print(f"offered seeds: {len(offered)}")
    print(f"unexpected seeds: {len(unexpected)}")
    print(f"missing seeds: {len(missing)}")
    for value in unexpected:
        print(f"  unexpected {value} from {offered[value]}")
    for value in missing:
        marker = " [first recorded seed]" if value == first else ""
        print(f"  missing {value} (recorded index {index[value]}){marker}")
    if unexpected or missing:
        print("The offered seeds do not match this translation's record.", file=sys.stderr)
        return 1
    print("The offered seeds match this translation's record exactly.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
