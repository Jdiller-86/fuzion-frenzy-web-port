#!/usr/bin/env python3
"""Count source and package workload leads without compiling or copying assets."""

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import runpy


SCALAR = re.compile(rb"\bX86_MEM(?:8|16|32|64|F32|F64)\(")
GUEST_ADDRESS = re.compile(rb"(?m)^X86_(?:FUNC|WEAK_FUNC|TRACED_FUNC)\((?:__imp__)?sub_([0-9A-Fa-f]{8})\)")
HEAP_COUNTER_LITERAL = re.compile(rb"\b0x0*(2CCEC8|2CD700)u\b", re.IGNORECASE)


def count_mixed_moves(source, memory_word):
    """Screen same-base move runs that alternate reading and writing memory."""
    run = []
    runs = accesses = 0

    def count():
        offsets = [int(move['offset'] or b'0', 16) for move in run]
        offsets = [offset - (1 << 32) if offset >= (1 << 31) else offset for offset in offsets]
        if (len({bool(move['load']) for move in run}) == 2
                and max(offsets) - min(offsets) + 4 <= (1 << 26)):
            return 1, len(run)
        return 0, 0

    for move in memory_word.finditer(source):
        if run:
            prev = run[-1]
            if (prev.end() != move.start() or prev['base'] != move['base']
                    or prev['load'] == prev['base']):
                spans, words = count()
                runs += spans
                accesses += words
                run.clear()
        run.append(move)
    spans, words = count()
    return runs + spans, accesses + words


def audit_sources(generated, manifest):
    adapter = runpy.run_path(str(Path(__file__).with_name('prepare-sdk-browser-tailcalls.py')))
    sources = manifest['generatedSources']
    shards = {name for name in sources if adapter['SHARD_NAME'].fullmatch(name)}
    if shards != {f'x86_recomp.{number}.cpp' for number in range(64)} or 'x86_func_mapping.cpp' not in sources:
        raise ValueError('Expected all 64 title shards and the dispatch map from a full browser build')
    counts = Counter()
    functions = set()
    heap_sites = Counter()
    for name in sorted(shards | {'x86_func_mapping.cpp'}):
        source = (generated / name).read_bytes()
        if hashlib.sha256(source).hexdigest() != sources[name]:
            raise ValueError(f'Generated source differs from the build manifest: {name}')
        if name not in shards:
            continue
        functions.update(address.upper() for address in GUEST_ADDRESS.findall(source))
        counts['scalarSites'] += len(SCALAR.findall(source))
        runs, words = count_mixed_moves(source, adapter['MEMORY_WORD'])
        counts['mixedMoveRuns'] += runs
        counts['mixedMoveSites'] += words
        entries = list(GUEST_ADDRESS.finditer(source))
        for index, entry in enumerate(entries):
            end = entries[index + 1].start() if index + 1 < len(entries) else len(source)
            references = HEAP_COUNTER_LITERAL.findall(source, entry.end(), end)
            if references:
                heap_sites[f'0x{entry[1].decode().upper()}'] += len(references)
    return {
        'shards': len(shards), 'guestAddresses': len(functions), **counts,
        'heapCounterLiteralsByAddress': dict(sorted(heap_sites.items())),
    }


def audit_package_census(path):
    raw = path.read_bytes()
    census = json.loads(raw)
    if census['kind'] != 'private-xbp-package-census' or census['schemaVersion'] != 1:
        raise ValueError('Expected the existing version 1 private package census')
    packages = {row['path']: row for row in census['packages']}
    counts = Counter(row['packagePath'] for row in census['resources'])
    if len(packages) != len(census['packages']) or counts != Counter({
        name: row['indexedEntries'] for name, row in packages.items()
    }):
        raise ValueError('Package index and resource counts differ')
    spans_by_package = {name: set() for name in packages}
    for row in census['resources']:
        if PurePosixPath(row['resourceName']).suffix.lower() in {'.bmp', '.gif'}:
            spans_by_package[row['packagePath']].add((row['offset'], row['size']))
    sizes = [
        {'path': name, 'spans': len(spans), 'bytes': sum(length for _, length in spans)}
        for name, spans in spans_by_package.items()
    ]
    return {
        'sha256': hashlib.sha256(raw).hexdigest(),
        'packages': len(packages),
        'resourceRows': len(census['resources']),
        'textureNamedSpans': sum(row['spans'] for row in sizes),
        'textureNamedBytes': sum(row['bytes'] for row in sizes),
        'largestByTextureNamedBytes': sorted(sizes, key=lambda row: (-row['bytes'], row['path']))[:5],
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generated', type=Path, required=True)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--package-census', type=Path, required=True, help='Existing private UI package census')
    args = parser.parse_args()
    manifest = json.loads((args.build / 'full-build-manifest.json').read_bytes())
    wasm_path = args.build / 'fuzion-sdk-full.wasm'
    if wasm_path.stat().st_size != manifest['moduleBytes']:
        raise ValueError('Wasm size differs from the build manifest')
    with wasm_path.open('rb') as wasm:
        digest = hashlib.sha256()
        for block in iter(lambda: wasm.read(1024 * 1024), b''):
            digest.update(block)
    print(json.dumps({
        'wasmSha256': digest.hexdigest(),
        'sdkCommit': manifest['sdkCommit'],
        'generatedSource': audit_sources(args.generated, manifest),
        'packageNameLeads': audit_package_census(args.package_census),
    }, indent=2))


if __name__ == '__main__':
    main()
