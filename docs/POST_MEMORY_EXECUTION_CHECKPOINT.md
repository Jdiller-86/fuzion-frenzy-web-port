# Guest write-page map: the published queue pages were never CPU-written

November 3, 2026. A bounded source-owned guest write-page map now records which 4 KiB guest-RAM pages the translated code writes. In this run **48 pages** were written, and **none** of the five pages covering the observed GPU-instance claim (`0x03B78000`–`0x03B7CFFF`, pages 15224–15228) was among them. The published DMA queue therefore points into guest RAM that the CPU never wrote, so producer provenance and the meaning of initial GET=0 remain unresolved. No consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-49/startup.wasm` (local/ignored) |
| Module bytes | 525,566 |
| Module SHA-256 | `05887417db7b7b06e6848e2210da8b103761ceb41fe51bc3083b117115557f97` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-37.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The translation is unchanged; only the write-observer component and its exports were added. ABI 24, the sole clock import and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/guest_write_map.c` / `.h`: a 64 MiB / 4 KiB page bitmap with init, clipped range marking, word access and popcount. It stores page bits, never guest bytes.
- `tools/fixtures/thread_startup_probe.c`: marks every translated guest write after the checked access succeeds, and exports `ff_probe_write_map_size`, `ff_probe_write_map_word` and `ff_probe_write_map_count`; reset reinitializes the map.
- `src/runtime/thread-proof-worker.mjs`: reports `guestWritePages` (page size, count, marked page indices) when the exports exist.
- `tests/guest_write_map_test.c`: covers init, single/crossing marks, clipping, duplicates, out-of-range, bounds and count.
- The private build adds the source and three exports; module ABI stays 24.

## Observed values

- UMA-backed PUT `0x03B7D000` and GET `0` remain verified guest addresses.
- Marked write pages: **48**; range examples 515–818 (image/heap) and 15359–16382 (stack).
- Claim pages 15224–15228 (`0x03B78000`–`0x03B7CFFF`): **0 marked**.
- The 4 KiB before PUT and the five claim pages remain zero.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The CPU did not write the published queue pages on this path. The queue may be produced by a different agent or mapping (for example a GPU-visible instance window, a copy engine, or a path not yet reached), or the initial GET/PUT values do not describe a CPU-produced ring. Decoding at the cursor or advancing GET would be unsupported.

## Next task

Identify the expected queue producer: inspect the original routines around the DMA submission and the `0xFD700000` instance writes, and determine whether the queue is written through the device aperture, a copy engine, or a not-yet-reached CPU path. A bounded site-level write observation for the device aperture or a targeted static trace is the next evidence step; do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/guest_write_map_test.c` passes optimized and ASan/UBSan. `tests/submission-probe.test.mjs` passes (11 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-37.json` (4,017,373 bytes).

## Prior milestone: UMA backing verified

`d->vram = ram` confirms UMA identity; the 4 KiB pre-PUT window and five claim pages were zero. Report `post-memory-browser-20261103-36.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The write map is a diagnostic, not a producer model or consumer. Next: identify the expected queue producer from static and runtime evidence.
