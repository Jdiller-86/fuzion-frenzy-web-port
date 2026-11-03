# Contiguous 0x80000000 window now aliases guest RAM; the queue producer still does not run

November 3, 2026. The pinned bridge documents that physical `P` is visible at `0x80000000 + P` and that D3D stores a physical address while reading the GPU position back as `GET | 0x80000000`. The proof previously modeled only one separate 4 KiB mirror page there. It now maps the full 64 MiB contiguous window directly onto modeled guest RAM. The cache-flush write at `0x80000000` now lands in guest physical 0 (one more written page, and the scanner sees its non-zero word), but no CPU write reaches the GPU-instance claim pages and the PFIFO wait still cannot complete. No consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-51/startup.wasm` (local/ignored) |
| Module bytes | 526,955 |
| Module SHA-256 | `cd0bf176d65407f08bf9409b1fd45d6efafde0c7aa3cc31000d2af49c7ba26bb` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-44.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The translation is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/guest_mirror.h/.c`: the mirror becomes a 64 MiB contiguous-window descriptor (`0x80000000`..`0x84000000`) with init, bounds checking and physical-offset calculation. It no longer owns separate storage.
- `tools/fixtures/thread_startup_probe.c`: read/write data accesses wholly inside the window return `guest_ram + offset`; writes also mark the guest write-page map. Execute access and accesses outside the window remain fail-closed.
- `tests/guest_mirror_test.c`: covers the window base/size, 64 MiB bounds, partial/crossing rejection, null/zero cases and offsets.
- The previous one-page separate storage is removed.

## Observed values

- Guest write pages: **49** (up from 48); the claim pages 15224–15228 remain **unwritten**.
- Submitted range scanner: GET 0 → PUT `0x03B7D000`; 4,096 words, **1 jump**, **4,095 method-0 headers**, 0 unknown — the cache-flush word now appears at guest address 0 through the alias.
- PFIFO views alias-consistent (PUT `0x03B7D000`, GET 0); GET never advances.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The 64 MiB contiguous alias is functional and more faithful than the separate mirror page: the cache-flush write reaches guest physical 0. It does not explain the missing pushbuffer: no CPU writes reach the claim pages, and the submitted guest range remains zero apart from the flush word. The queue producer is still unestablished.

## Next task

Choose the next bounded independent startup prerequisite or producer-provenance step, preserving the fail-closed boundary. The PFIFO consumer remains blocked; do not treat the contiguous window, zero headers or mixed RAMIN data as an empty-queue completion, and do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/guest_mirror_test.c` passes optimized and ASan/UBSan. The scanner/submission tests, device-register tests, runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-44.json` (4,020,336 bytes).

## Prior milestone: RAMIN scan

The RAMIN aperture decodes as mixed context data, not a command stream. Report `post-memory-browser-20261103-43.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The contiguous window is a memory-view model, not a producer or consumer. Next: choose an independent prerequisite.
