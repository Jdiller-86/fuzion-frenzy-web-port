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

## Head-placement correction

The first diagnostic (`timerCheck = 15`) showed the chosen DPC head, `thread->fs + 0x50`, held `0x205E98` — a code/return address, not a DPC object; its first word was `0xFD000000`, not type `0x13`. That address overlaps a live TLS field the reached code writes. The model-owned heads were moved to the reserved synthetic queue area: timer head `0x03E00000`, DPC head `0x03E00008`. The rebuilt module reports `timerCheck = 0`, `dpcHead = 0x03E00008`, `dpcHeadValue = 0x03E00008` (self-loop) and a valid timer head.

## Further evidence: trace capacity

- Private module `recomp-post-memory-build-68`: 639,037 bytes, SHA-256 `583bc2f6545c348065687851938d3ae2ee3b6e8efc6e6ac16b76d30309104eea`.
- Profile run: 277 distinct routines, 83/83 initializers, 403 kernel operations; the timer diagnostic passes (`timerCheck = 0`), then the run stops at status 5 (`diagnostic capacity reached`) at target `0x201199`, return `0x1FDBE0`.
- The 8,192-entry initializer trace is now full; the next step is an explicit trace-capacity or tracing-scope decision.

## Limits

No rendering, audio, fidelity or playability is established. The timer/DPC model still covers one-shot timers and bounded DPC delivery only. The list heads are model-owned synthetic queue storage; the `0xFD002080` zero range is explicit synthetic storage; no original value is invented. The module is private.

## Validation

`tests/kernel_timers_test.c` optimized and ASan/UBSan; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Decide whether to extend the explicit initializer trace capacity beyond 8,192 or narrow tracing, then rerun. Do not force the DPC list empty, invent the timer/DPC state or claim rendering/fidelity without evidence.
