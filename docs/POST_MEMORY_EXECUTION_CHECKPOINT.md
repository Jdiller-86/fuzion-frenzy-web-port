# Bounded submission snapshot at the PFIFO boundary; consumer deferred

November 3, 2026. An architect escalation reviewed the PFIFO DMA stall and approved **observation only**: no consumer, no DMA_GET advance, no method execution. A bounded, read-only snapshot now records at most 256 bytes of checked guest memory at the submitted cursor, with explicit availability reasons. The first snapshot at `0x03B7D000` covers `0x03B7CF00`–`0x03B7CFFF` and is entirely zero, which confirms the cursor alone does not identify queued commands. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-48/startup.wasm` (local/ignored) |
| Module bytes | 525,113 |
| Module SHA-256 | `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-32.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged; the milestone adds a source-owned report diagnostic. ABI 24 and every prior synthetic boundary are unchanged.

## Architect escalation

The project architect reviewed the evidence (`DMA_PUT`/`GET` at `0xFD800040`/`0xFD800044`, the `0x190` delay helper, xemu's PFIFO routing) and recommended **A: bounded read-only submission diagnostics**, not a consumer. Key cautions:

- `DMA_PUT` is a producer cursor, not automatically the first command address.
- Its position at the top of the `0x03B78000 + 0x5000` claim makes forward decoding especially unsafe.
- Address translation, buffer bounds and initial GET provenance must be established before identifying commands.
- A synthetic GET of zero is not evidence of a valid command-stream start.
- Parsing packets alone would not justify advancing GET past unperformed effects.

This approves observation only; the consumer remains an open architecture decision.

## Source-owned changes

- `src/runtime/submission-probe.mjs`: bounded builder that returns unavailable with an explicit reason unless the run stopped at status 13 and an observed DMA_PUT exists. It captures at most 256 bytes from checked guest memory ending at the cursor, plus channel, PUT, GET, base and a note that the bytes are a memory snapshot, not decoded commands. It never mutates memory or state.
- `src/runtime/thread-proof-worker.mjs`: adds the `submissionProbe` report field from the guest-memory view and the observed aperture entries.
- `tools/serve-startup-proof.mjs`: serves the new module on the source-only proof host.
- `tests/submission-probe.test.mjs`: covers the exact little-endian window, read-only behavior, missing PUT/GET, unaligned/out-of-range cursors, invalid window sizes and the low-cursor clamp. Wired into `npm run test:runtime`.

## Observed browser execution

- All **83/83** initialization callbacks returned; **261** distinct original routines and **402** kernel operations.
- Stop remains status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- `submissionProbe`: available, channel 0, PUT `0x03B7D000`, GET `0`, base `0x03B7CF00`, 256 bytes, **0 nonzero words**.
- Device aperture: **5,302 entries** / **12,524 events**; memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

The zero window demonstrates that the submitted cursor is not immediately preceded by identifiable command data; the buffer base, cursor units and initial GET must be established before any consumer work.

## Next dependency

Establish the PFIFO DMA object/base/limit, cursor units, address mapping, initial GET, packet/control-flow forms and reached method dependencies from pinned evidence. Then decide a bounded synchronous PFIFO contract (supported operations, downstream side effects, work limits and fail-closed behavior) before implementing any consumer. No GET advancement or method execution is approved.

## Validation

`tests/submission-probe.test.mjs` passes. `npm run test:runtime`, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-32.json` (3,995,391 bytes).

## Prior milestone: PFIFO DMA map

Module `recomp-post-memory-build-48` classified channel-0 `NV_USER_DMA_PUT`/`GET`/`REF` and added status 13; the run stopped at the first GET read with PUT = `0x03B7D000`. Report `post-memory-browser-20261103-31.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The snapshot is evidence only; it is not a command decoder, a FIFO consumer or fidelity acceptance. Next: establish the DMA cursor/base/units and initial GET from pinned evidence before proposing a bounded consumer.
