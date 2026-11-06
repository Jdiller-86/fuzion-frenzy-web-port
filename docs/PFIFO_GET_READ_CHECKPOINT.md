# Serving the observed DMA_GET makes the original wait poll to its limit

November 4, 2026. A bounded diagnostic tested the hypothesis that the status-13 PFIFO boundary was an over-eager probe gate. With an opt-in flag, the probe serves the observed `DMA_GET` value (`0`) instead of stopping, letting the original comparison decide. The original code then spins on the same read until the device poll limit (status 12). This confirms the wait expects a different GET value and rules out the "false boundary" explanation. The default build remains fail-closed at status 13 and is byte-identical to the known module. **There is no playable screen, renderer or device model.**

## Method

- `tools/fixtures/thread_startup_probe.c` gained an opt-in `FF_ALLOW_DMA_GET_READ` diagnostic: when defined, the existing status-13 stop is skipped and the normal device-register accessor serves the observed GET. Without the define, the source is unchanged.
- Private build `research/verification/recomp-post-memory-build-52/` compiled the existing translation `recomp-title-post-memory-30` with the probe and `-DFF_ALLOW_DMA_GET_READ=1`; module SHA-256 `f8a517ad60699703e7b0a20579355bb61926329e0d2777c259937a5e7360ebb0`, 526,793 bytes.
- The existing `thread-proof-worker.mjs` ran the module with the retained XBE and the explicit `settings-mmio-20.json` profile. Output is ignored at `research/verification/pfifo-setup-01/browser-52.json`.
- Private control build `recomp-post-memory-build-53` compiled the same source without the define; it is byte-identical to `recomp-post-memory-build-51` (SHA-256 `cd0bf176d65407f08bf9409b1fd45d6efafde0c7aa3cc31000d2af49c7ba26bb`).

## Observation

The diagnostic run reports `phase: original-initializer-bringup`, `verified: false`, and:

- 83/83 initialization callbacks returned.
- 262 distinct original routines executed (one more than the fail-closed run).
- 402 kernel operations completed.
- Stop status 12 (`device poll limit reached`) at target `0xFD800044`, the same DMA_GET read.
- The original wait therefore runs with the observed GET=0 and never matches.

## Interpretation

- The status-13 boundary was not the only thing preventing progress: serving the real GET value leaves the original code spinning. The wait expects GET to become a different value (the ring base/put candidate), which no modeled GPU/consumer produces.
- This strengthens the ring-base and command-source blocker: the issue is not the probe gate or the software GET reset alone, but the missing hardware/consumer state that would make GET match.
- The default fail-closed contract is retained. The opt-in diagnostic is for evidence gathering only and is not part of the runtime product path.

## Limits

One diagnostic build and one explicit synthetic settings profile. It does not establish the expected GET value, the ring base/end, a populated command stream, a GPU consumer or any rendering. A status-12 poll limit is not original behavior.

## Validation

Private diagnostic run completed; control build reproduces the known fail-closed module byte-for-byte. The source-owned runtime suite (109/109), Python memory-access 10/10, title-seed 25/25, `npm run check` and whitespace checks pass. ABI 24 and D060 are unchanged.

## Next task

Establish the expected GET value and the ring base/end from the device context and the command source. If the evidence shows an empty-ring initialization or a populated buffer, return to a bounded consumer model with that explicit evidence. Otherwise continue another independent prerequisite; do not set GET=PUT, consume packets or treat the poll limit as progress.
