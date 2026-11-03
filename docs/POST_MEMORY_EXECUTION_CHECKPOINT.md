# Per-written-page scan: 16 of 49 written pages contain command-like headers, none is the queue

November 3, 2026. A read-only per-page header survey now scans each page the translated code actually wrote. Of **49 written pages**, **16** contain command-like sequences (method kinds, jumps or calls) under a bounded heuristic. The queue claim pages are still unwritten, the submitted `GET..PUT` range still starts zeroed apart from the flush word, and the PFIFO wait cannot complete. No consumer is implemented. This is the last evidence probe before retaining the boundary and selecting an independent prerequisite. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-51/startup.wasm` (local/ignored) |
| Module bytes | 526,955 |
| Module SHA-256 | `cd0bf176d65407f08bf9409b1fd45d6efafde0c7aa3cc31000d2af49c7ba26bb` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-45.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/submission-probe.mjs`: adds `writePageScan`. For up to 64 reported write pages it runs the read-only header scanner over each 4 KiB page and reports per-page method kinds, unknown words, jumps and calls, plus at most 16 "interesting" rows (`unknown < 64` and more than one method kind or any jump/call). No commands are executed and GET is not advanced.
- `src/runtime/thread-proof-worker.mjs`: computes the write-page list before the submission probe and passes it in.
- `tests/submission-probe.test.mjs`: covers the per-page survey and read-only behavior (17 cases).

## Observed values

- Written pages: **49**; interesting: **16** — pages 0, 515, 517, 686, 716, 749, 753, 770, 773, 775–778, 782, 783, 787.
- Examples: page 515 (3 calls, 6 jumps, 5 method kinds, 14 unknown); pages 770/776 (9/11 method kinds, 0 unknown); page 0 (flush word, 1 jump).
- Claim pages 15224–15228 remain unwritten; submitted range GET 0 → PUT `0x03B7D000` still zero apart from the flush word.
- PFIFO views alias-consistent (PUT `0x03B7D000`, GET 0); GET never advances.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The written pages contain code/data patterns that partially decode as command headers; none aligns with the DMA queue claim, and the queue range itself is zero. The heuristic cannot establish a producer or a ring base; a consumer remains unsupported.

## Decision

The architect's fallback applies: retain the fail-closed PFIFO boundary and stop drilling this path. Select an independent bounded startup prerequisite next rather than modeling consumption from ambiguous page patterns.

## Next task

Choose and execute one independent bounded startup/feature prerequisite, preserving the PFIFO boundary and all evidence. Do not treat command-like pages as a queue, and do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/submission-probe.test.mjs` passes (17 cases). Device-register, scanner, mirror, runtime-suite, Python memory-access 10/10 and title-seed 25/25 checks pass; `npm run check` and whitespace checks pass. The report saved as `post-memory-browser-20261103-45.json` (4,035,516 bytes).

## Prior milestone: 64 MiB contiguous alias

The contiguous window now maps to guest RAM; the cache-flush write reaches physical 0, but the queue claim is unwritten. Report `post-memory-browser-20261103-44.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The page survey is a diagnostic, not a producer or consumer. Next: independent prerequisite.
