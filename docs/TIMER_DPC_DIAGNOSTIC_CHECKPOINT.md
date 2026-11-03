# Timer diagnostic identifies the DPC-list head mismatch at the ordinal-149 stop

November 4, 2026. The ordinal-149 stop at `0xC00000BB` (`FF_BOOT_UNSUPPORTED`) is now diagnosed. `ff_timers_check` reports the first structural/timer/DPC/link failure without mutating the queue; the probe records it before stopping and the worker reports `timerCheck`. The result is **15**: the guest DPC list head is not the expected empty/self value while the model's DPC count is zero. The timer entry itself remains valid and active. **There is no playable screen, renderer or device model.**

## Source-owned change

- `src/runtime/kernel_timers.h/.c` adds `ff_timers_check(const ff_timer_queue *)`, returning 0 when `validate` would pass and a small reason code otherwise:
  - 1 structure/head bounds; 2-3 timer object validity; 4 timer flags/absolute/deadline/order; 5-6 timer DPC validity; 7 timer links; 8 duplicate timer;
  - 9-10 queued DPC validity; 11 DPC links; 12 duplicate DPC;
  - 13-16 timer head, timer tail, DPC head and DPC tail mismatches.
- `tools/fixtures/thread_startup_probe.c` records the check code before the ordinal-149 stop and exports `ff_probe_timer_check`.
- `src/runtime/thread-proof-worker.mjs` reports `timerCheck` when the export exists.
- `tests/kernel_timers_test.c` asserts a valid queue returns 0 and a corrupted DPC head returns 15; optimized and ASan/UBSan pass.

## Run

- Private module `recomp-post-memory-build-68`: 638,768 bytes, SHA-256 `05e26e139e0e22c9c813e87695ff594e9b2a89247ade82c3a52a60b2a25a2496`.
- Profile run: **277** distinct routines, 83/83 initializers, 403 kernel operations; stop status 9 at `0xC00000BB`, ordinal 149, return `0x200F73`, `timerCheck = 15`.
- The timer queue still has one active timer (`3248512`, DPC `3248480`, deadline `216000308224`).

## Interpretation

- The failure is not a timer flag, deadline/order or DPC-object validity error: those codes were not returned. The guest DPC list head points at a non-empty list while the modeled `dpc_count` is zero. The reached code has enqueued a DPC through a path the model does not observe, or wrote the guest list directly.
- Do not synthesize the DPC or force the head empty. The next step is to inspect the guest DPC head/object and identify the enqueue path/service with evidence, then model it within the bounded timer/DPC contract.

## Limits

The timer/DPC model still covers one-shot timers and bounded DPC delivery only. No rendering, audio, fidelity or playability is established; no non-empty ring, raw-pointer bypass, host matrix substitute or original content is fabricated or committed. The module is private.

## Validation

`tests/kernel_timers_test.c` optimized and ASan/UBSan; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Inspect the guest DPC list head/object at the ordinal-149 stop, identify the enqueue path and model it with explicit evidence. Then rebuild and rerun. Do not force the list empty or invent the DPC state.
