# Submission snapshot finds the command data in the device aperture, not guest RAM

November 3, 2026. The bounded submission probe now captures two read-only windows: the 256 bytes before DMA_PUT and the 256 bytes at the observed GPU-instance allocation base. Both guest-RAM windows are entirely zero. The same run's aperture retains **5,120 entries** at `0xFD700000`–`0xFD704FFC`, **559 of them nonzero**, so the instance/command data is represented in the separate synthetic device backing, not at the guest physical addresses the DMA cursor names. Address translation and buffer ownership remain unestablished, and no consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-48/startup.wasm` (local/ignored) |
| Module bytes | 525,113 |
| Module SHA-256 | `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-33.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged; only the source-owned snapshot diagnostic and its report field changed. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/submission-probe.mjs`: the status-13 snapshot now also accepts the observed GPU-instance allocation (`base`, claim bytes) and captures a second bounded read-only window at that base. Each window is independently available/unavailable with an explicit reason; both are labelled memory snapshots, not decoded commands. The before-PUT window remains the primary `base`/`words` fields.
- `src/runtime/thread-proof-worker.mjs`: extracts the successful ordinal-168 claim (`baseAddress`, requested bytes) from the reported calls and passes it to the probe.
- `tests/submission-probe.test.mjs`: covers the allocation window, short claim capture, out-of-range base, read-only behavior and the missing-allocation reason.

## Observed browser execution

- All **83/83** initialization callbacks returned; **261** distinct original routines and **402** kernel operations.
- Stop remains status 13 (`pushbuffer consumption required`) at `0xFD800044`; the aperture retains 5,302 entries / 12,524 events.
- `submissionProbe`: available, channel 0, PUT `0x03B7D000`, GET `0`, before-window base `0x03B7CF00` / 256 bytes / **0 nonzero words**.
- Allocation window: available at base `0x03B78000` / 256 bytes / **0 nonzero words**.
- The device aperture's `0xFD700000`–`0xFD704FFC` region contains **5,120 entries, 559 nonzero** (for example `0xFD700014 = 0x80000118`, `0xFD70001C = 0x80000112`), matching the instance/context writes observed earlier.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The DMA_PUT/DMA_GET pair names guest physical addresses, but the proof's device aperture is separate storage: the GPU-instance claim was zeroed at the guest base while the corresponding instance writes landed in the `0xFD700000` aperture. The command/context bytes therefore are not visible at the cursor in guest RAM. Before any consumer, the proof must establish the DMA object base/limit, the guest-physical-to-device-aperture mapping, cursor units, initial GET and packet/control-flow forms. Zero windows at the cursor and allocation base must not be read as an empty queue.

## Next task

Establish the DMA object/base/limit and the physical↔device-aperture translation from pinned evidence, then decide a bounded synchronous PFIFO contract. This remains an open architecture decision; do not fabricate the get pointer, command decoding, interrupt delivery, DMA or rendering.

## Validation

`tests/submission-probe.test.mjs` passes (5 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-33.json` (3,996,235 bytes).

## Prior milestone: first submission snapshot

Module `recomp-post-memory-build-48` added a single 256-byte pre-PUT snapshot; the run showed that window entirely zero and deferred the consumer. Report `post-memory-browser-20261103-32.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The snapshots are evidence only; they are not a command decoder, FIFO consumer or fidelity acceptance. Next: establish the DMA/device translation and buffer ownership before proposing a bounded consumer.
