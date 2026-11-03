# Original execution reaches a precise PFIFO DMA submission boundary

November 3, 2026. The USER-region wait is now identified as NV2A PFIFO DMA submission: `0x001FAED0` writes the DMA put pointer `0x03B7D000` to `0xFD800040`, and `0x001FBB00` reads the device get pointer `0xFD800044`, waiting until it catches up. A source-owned classifier and a dedicated status now report **pushbuffer consumption required** instead of spinning to the generic poll limit. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-48/startup.wasm` (local/ignored) |
| Module bytes | 525,113 |
| Module SHA-256 | `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-31.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

ABI 24, the sole `env.ff_host_clock` import, the 8,192-entry / 32,768-event aperture, the 16 MiB window, the mirror page, the 16-read poll limit and the single self-clearing flush bit are unchanged.

## Source-owned change

- `src/runtime/pfifo_dma.c` / `.h`: classifies the reached NV2A USER-region PFIFO DMA registers for channel 0 — `NV_USER_DMA_PUT` (`0xFD800040`), `NV_USER_DMA_GET` (`0xFD800044`) and `NV_USER_REF` (`0xFD800048`). Other offsets and channels return none. Evidence: the pinned xemu model maps USER at NV2A offset `0x800000`, and `nv2a_regs.h` defines those three offsets; xemu's `user.c` routes them to the PFIFO DMA put/get/reference registers.
- `tools/fixtures/thread_startup_probe.c`: on a read of DMA_GET, compares it with the aperture's DMA_PUT value; if they differ, stops with status 13. This is a fail-closed boundary, not a value model.
- `src/runtime/thread-proof-worker.mjs`: status 13 reports `pushbuffer consumption required`.
- `tests/pfifo_dma_test.c`: covers the three channel-0 registers, nearby offsets, channel 1, and out-of-window addresses.

## Observed browser execution

- All **83/83** initialization callbacks returned.
- **261** distinct original routines executed and **402** kernel operations completed. The run stops before entering the delay helper `0x001FACA0`; the previous poll-limit run reached 262 by looping.
- `0x001FAED0` wrote DMA_PUT = **`0x03B7D000`** at `0xFD800040`.
- The first DMA_GET read at `0xFD800044` returned `0` (bounded USER page), so the put/get comparison could not match and status 13 stopped the run.
- Device aperture: **5,302 entries** / **12,524 events**; memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 13 (`pushbuffer consumption required`), target `0xFD800044`.

## Next dependency

Completing this wait requires a real PFIFO consumer: parsing the submitted pushbuffer at `0x03B7D000`, executing or accounting for its commands, and advancing DMA_GET. That is a device/FIFO subsystem decision that affects multiple subsystems and must not be fabricated. The next task is to analyze `0x001FACA0`'s caller path and the queued pushbuffer contents from static and pinned-emulator evidence, then decide whether a bounded FIFO-consumer model is the right next investment (with architect escalation if it changes the route).

## Validation

`tests/pfifo_dma_test.c` passes optimized and ASan/UBSan WSL GCC runs. The runtime suite (49/49), Python memory-access 10/10 and title-seed 25/25 pass, and `git diff --check` is clean. The report saved as `post-memory-browser-20261103-31.json` (3,994,544 bytes).

## Prior milestone: 16 MiB window and USER page

Module `recomp-post-memory-build-47` was unchanged; the profile and parser admitted the 16 MiB NV2A window and a bounded USER page at `0xFD800000`, reaching the pushbuffer wait after 262 routines and 402 kernel operations. Report `post-memory-browser-20261103-30.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The mirror page, zero ranges, event capacity, poll limit, flush bit, USER page, PFIFO register map, time conversion, port-output log and GPU instance claim are synthetic proof scaffolding, not recovered console state. Next: analyze the queued pushbuffer and decide the FIFO-consumer architecture before attempting any device progress.
