> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Deterministic fixed-step host loop pacing; original execution still at the PFIFO boundary

November 3, 2026. With the controller input components complete and the original startup still fail-closed at the PFIFO DMA wait, the next independent runtime prerequisite is host-loop pacing: a pure fixed-step accumulator that decides how many whole steps to run for a supplied elapsed time and how much time carries forward. It does not read a clock, execute guest code, sample input or render, and it does not claim original-console timing or cross-runtime determinism. **There is no playable original screen and no renderer/device model.**

## Why this milestone

- The retained report `post-memory-browser-20261103-46.json` still stops at status 13 (`pushbuffer consumption required`) at `0xFD800044`; the 4 KB window before `PUT` (`0x03B7C000`..`0x03B7D000`) is entirely zero and the 20 KB claim is unwritten. There is nothing for a GPU consumer to consume, and the missing initial GET belongs to unobserved kernel/D3D channel setup, not producer output. The fail-closed boundary is retained.
- The called kernel ordinals include no GPU/display channel-setup service (`187` is handle close, `277`/`294` are critical enter/leave), so no reached import supplies that state.
- A host loop that alternates elapsed time, input sampling, runtime stepping and presentation is a prerequisite for any continuous original run; the pacing core can be built and checked without original content.

## Source-owned change

`src/runtime/host-step.mjs` exports `createHostStep({ hz = 60, maxCatchUp = 5 })`:

- Time is accumulated in units of `1/hz` microseconds, so a whole step is exactly 1,000,000 units even when `1e6` is not divisible by `hz` (for example 60 Hz). Supplied milliseconds are rounded to whole microseconds, and sub-microsecond differences are discarded explicitly rather than drifting through a floating accumulator.
- `advance(deltaMs)` returns `{ steps, pendingMs, droppedSteps, paused }`. Whole steps beyond `maxCatchUp` are dropped and reported, preventing an unbounded catch-up spiral after a long frame.
- `pause()` clears partial time and makes `advance` return zero steps until `resume()`; `reset()` clears totals, dropped count, partial time and pause. `describe()` reports `kind`, `hz`, `stepMs`, `maxCatchUp`, totals, pause state and an explicit limitation.
- `hz` is an integer from 1 to 1000, `maxCatchUp` an integer from 1 to 240; non-finite or negative deltas, values beyond the safe integer range and invalid configuration throw before changing state.

`tests/host-step.test.mjs` covers defaults and description, partial-step carry without drift, exact 1000 Hz multiples, long-frame capping and dropped counts, pause/resume, reset, and invalid configuration/deltas. It is wired into `npm run test:runtime`.

## Validation

`tests/host-step.test.mjs` passes 7/7; the runtime suite passes 94/94; Python memory-access is 10/10 and title-seed 25/25; `npm run check` and whitespace checks pass. The PFIFO boundary, module `recomp-post-memory-build-51` (526,955 bytes, SHA-256 `cd0bf176…`), ABI 24 and D060 are unchanged.

## Limits

The accumulator is pacing only. It is not wired into the worker, boot path, renderer, audio or input sampling, and it does not establish frame rate, input latency, original-console timing or determinism. No original module/content was committed.

## Next task

Integrate the accumulator only when a bounded original runtime loop is reached, or continue another independent runtime prerequisite. Do not claim timing or playability without evidence, and do not fabricate a GPU consumer, packet, get pointer, interrupt, DMA or rendering.
