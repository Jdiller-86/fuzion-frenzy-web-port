# Full-range scan: the GET..PUT span includes the game image/heap, not a queue

**Later evidence (November 4):** [the channel-setup checkpoint](PFIFO_CHANNEL_SETUP_CHECKPOINT.md) shows the title's own context setup writes PFIFO PUT/GET to zero at `0x20005A`/`0x20027C`/`0x20047F` before the `0x1FAF1C` PUT write. GET=0 is therefore title-initialized; the command source and the empty 20 KB claim remain unresolved. Do not consume, set GET=PUT or treat the scan as a queue.

November 3, 2026. The read-only header scanner now walks the full `GET..PUT` span instead of a 4,096-word prefix. The 15,594,496-word range contains **661,203 nonzero words** up to `0x3328AC` and decodes as 1,481 method kinds, 9,138 calls, 10,994 jumps, 164 returns and 28,230 unknown words. That is the game image/heap being read as command headers, not a coherent submitted queue. `GET=0` spans the whole low memory to `PUT=0x03B7D000`, so it is not a usable ring start. The PFIFO wait cannot complete and no consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-51/startup.wasm` (local/ignored) |
| Module bytes | 526,955 |
| Module SHA-256 | `cd0bf176d65407f08bf9409b1fd45d6efafde0c7aa3cc31000d2af49c7ba26bb` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-46.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/pfifo-scan.mjs`: adds `PFIFO_SCAN_ABSOLUTE_WORDS` (16 Mi) as the absolute budget, tracks `nonZeroWords`, `firstNonZero` and `lastNonZero`, and keeps the 4,096-word default for callers that want a prefix.
- `src/runtime/submission-probe.mjs`: `pushbufferScan` scans the full `GET..PUT` range with the absolute budget.
- `tests/pfifo-scan.test.mjs`: covers the larger explicit budget, nonzero metadata and the absolute-budget rejection.

## Observed values

- Range GET 0 → PUT `0x03B7D000`: 15,594,496 words, scanned fully, not truncated.
- Nonzero words: **661,203**; first `0`, last `0x3328AC`.
- Decode: 14,918,647 method-0 headers, 1,481 method kinds, **9,138 calls**, **10,994 jumps**, **164 returns**, **28,230 unknown**.
- PFIFO views alias-consistent (PUT `0x03B7D000`, GET 0); GET never advances.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The range is not a zero-prefixed queue: it contains the loaded image and heap, which decode as many incidental headers/calls/jumps. Treating GET=0 as a ring start would make the GPU read unrelated memory as commands. This reinforces that the intended initial GET/ring base is not established and the producer is not modeled.

## Next task

Retain the fail-closed PFIFO boundary and select an independent bounded startup/feature prerequisite. Do not treat image/heap decode patterns as a queue, and do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/pfifo-scan.test.mjs` and `tests/submission-probe.test.mjs` pass (22 combined cases). Device-register/mirror/runtime-suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-46.json` (4,037,135 bytes).

## Prior milestone: written-page survey

49 pages written, 16 with command-like patterns; none is the queue claim. Report `post-memory-browser-20261103-45.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The full scan is evidence, not a producer or consumer. Next: independent prerequisite.
