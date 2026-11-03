# RAMIN aperture scan: the instance data is not a coherent command stream

November 3, 2026. A read-only scan of the observed `0xFD700000` RAMIN aperture confirms it is not the missing pushbuffer. Reconstructing the 0x5000-byte aperture from its 5,120 entries and running the NV2A header scanner yields 2,589 zero headers, only seven method kinds, 150 jumps, 8 calls, 317 unknown words and no returns. The submitted guest range remains zero at its start. The queue producer is still absent; no consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-50/startup.wasm` (local/ignored) |
| Module bytes | 526,712 |
| Module SHA-256 | `bd0acb033fcf0e9b6c768ef56a9b802225652099f6d1b797cb71e2f8cc996646` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-43.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/submission-probe.mjs`: adds `raminScan`. It reconstructs the observed `0xFD700000`–`0xFD705000` region from the aperture's dword entries (missing addresses zero) and runs the existing read-only header scanner over it. It does not read device MMIO, execute commands or advance GET.
- `tests/submission-probe.test.mjs`: covers a synthetic RAMIN header stream, entry/nonzero counts, read-only behavior and the missing-entry case (16 cases).

## Observed values

- RAMIN aperture: 5,120 entries, 559 nonzero; reconstructed 0x5000 bytes.
- Scan (4,096-word budget, truncated): **2,589** method-0 headers, method kinds **7** (methods 0, 4, 8, 12, 16, 284, and subchannel-3 method 120), **150** jumps, **8** calls, **0** returns, **317** unknown, 0 pending parameters.
- Submitted guest range GET 0 → PUT `0x03B7D000`: still zero headers for the first 4,096 words.
- PFIFO views alias-consistent (PUT `0x03B7D000`, GET 0); GET never advances.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The RAMIN content decodes as context-like data with many zeros and desynchronising words, not as a coherent NV2A command stream. Rule out "the queue is the `0xFD700000` aperture" as the explanation for the missing producer. The guest submission range is zero at its start, and no modeled CPU path writes the claim pages. Both the producer and the intended initial GET remain unestablished.

## Next task

Choose the next bounded independent startup prerequisite or producer-provenance step, preserving the fail-closed boundary. The PFIFO consumer remains blocked; do not treat zero headers or mixed RAMIN data as an empty-queue completion, and do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/submission-probe.test.mjs` and `tests/pfifo-scan.test.mjs` pass (20 combined cases). Device-register tests, the runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-43.json` (4,020,316 bytes).

## Prior milestone: zero-header submission scan

The submitted guest range's first 4,096 words decode as the zero header. Report `post-memory-browser-20261103-42.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The RAMIN scan is read-only evidence, not a producer or consumer. Next: choose an independent prerequisite.
