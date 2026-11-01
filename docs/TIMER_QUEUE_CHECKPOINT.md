# One-shot timer queue component checkpoint

Later integration: [the timer startup checkpoint](TIMER_STARTUP_CHECKPOINT.md) connects this component and an owned CPU-context bridge to original browser startup. This page preserves the independent component checkpoint and its limits before that integration.

November 1, 2026 component checkpoint: an independently authored timer/DPC queue component passed controlled-clock scenarios under native GCC, address/undefined-behavior sanitizers and WebAssembly. This implemented part of the contract required by the original six-hour timer request. At this checkpoint it was not wired into the title runtime, and original `KeSetTimer` still stopped. The selected recompilation route and complete original-game/custom-feature objective remain active.

## Established behavior

`src/runtime/kernel_timers.c` maintains bounded guest timer objects and a deferred-callback FIFO. All clock samples are explicit 64-bit values in 100 ns units. It has no wall-clock, browser, filesystem, network, firmware or original-code dependency. `kernel_timers.h` documents the caller contract.

| Contract | Owned evidence | Limit |
| --- | --- | --- |
| Relative one-shot expiry | Both timer types stay queued at six hours minus one tick and signal exactly at the controlled deadline, across 32-bit boundaries | Not a six-hour real-time/browser measurement |
| Absolute deadlines | System-time rollback extends the absolute deadline; a forward adjustment expires it; relative timers retain their monotonic deadline | A real browser/virtual-console clock source is not connected |
| Rearm/cancel | Set/cancel return the actual prior insertion state; rearming replaces the old deadline/DPC; cancellation prevents future expiry | Cancelling a signalled timer does not revoke an already queued DPC |
| Object state and queues | Header flags, signal state, due time, DPC pointer, guest forward/back links and ordered expiry agree with the component's state | Owned sorted queue, not a reproduction of original kernel timer buckets or arbitrary kernel-internal layout |
| Deferred arguments | The callback record exposes DPC, current routine/context and the low/high system-time words sampled at expiry | Peek/pop do not execute callbacks or establish CPU-context preservation |
| Deduplication and dispatch admission | A pending DPC is queued once without overwriting its original arguments; peek precedes pop; dequeue defers at IRQL 2 or above | Caller still must admit executable addresses, raise/restore IRQL and preserve CPU/TLS/SEH state |
| Bounded failure | 32 active timers and 32 pending DPCs; full queues fail without dropping work; admission/advance failures preserve state | General waits, periodic timers, arbitrary external queue mutation and broader scheduling remain unsupported |

The fixture also exercises malformed ranges/alignment, overlapping reserved heads, unknown inserted objects, null callback routines, nonempty wait lists, nonzero periods, corrupted links, backward monotonic samples and deadline overflow. `INT64_MIN` relative duration is handled without signed negation overflow. Empty callback queues do not manufacture a callback. The component has no default clock or success fallback.

These are source-owned inputs/assertions, not observations of the original Xbox kernel. Interfaces/layouts and semantics were checked against pinned nxdk [kernel declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h), Cxbx [timer set/cancel](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlKe.cpp#L2055) and [due-time/expiry/DPC handling](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlKi.cpp#L456). Those implementations support the contract interpretation, not matching-build kernel fidelity. No vendor implementation body was copied into the component or fixture.

## Original callback evidence

The existing Ghidra project was reused with scoped launcher state and `-noanalysis -readOnly`, selecting only original callback `0x00034695`. Private export `research/verification/recomp-timer-01/seed.json` identifies an established 18-byte / four-instruction function, a read of guest address `0x003191a8` and a conditional call through declared slot `0x00253438`, ordinal 360. The pinned reference identifies that ordinal as [HalInitiateShutdown](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlHal.cpp#L796).

Observation: this is static callback/import evidence, not executed shutdown behavior. Inference: the six-hour callback can participate in virtual-console shutdown, depending on game state. Unknown: that flag's runtime state and complete matching-console behavior. The callback was exported but not translated, linked or executed in this task. No host shutdown operation is part of the browser contract. Keep unsupported guest shutdown explicit until its virtual-runtime behavior is implemented; do not turn it into success or a host action.

No disc extraction, XBE reimport, original-code execution, firmware acquisition, tool installation or global configuration change occurred. Original game files, raw callback analysis, generated test output and all prior artifacts remain local/ignored. Direct Ghidra output is not REA Evidence.

## Validation and retained identities

`tests/kernel_timers_test.c` passes native C11 `-O2 -Wall -Wextra -Werror`, native `-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer`, and Emscripten 3.1.50 `-O2` WebAssembly. Only the independently authored fixture/component and existing owned object initializer are compiled. The Wasm fixture runs under the SDK's Node 16.20.0 because it contains no original instructions; the browser-only rule for original title execution is unchanged. This is not browser-clock or original-title execution evidence.

Commands, source/header digests and all four sanitizer/Wasm compile/run exit codes (zero) are retained in `research/verification/kernel-timers-01/checks.json` and `checks.log`. The optimized native fixture is retained there as `check`. CI now compiles/runs the same owned scenarios with GCC on Linux; no private SDK or game input is required. Publication results are recorded in status/handoff after the run finishes.

The preparation/publication guard passes for 139 source files and whitespace checks pass. All six compiled owned source/header digests match the report, and the original callback export and supplied XBE match the established executable identity. No publication guard was relaxed.

| Artifact | SHA-256 |
| --- | --- |
| `src/runtime/kernel_timers.c` | `81307cb8ee0355f4dad2a54a6a99d4305c84ba23fbb0028cd50bcf230e66b0c6` |
| `src/runtime/kernel_timers.h` | `4a212df5255727e9a698fe09d551d192422b736cad34cbfed703bd1cc91dd1c6` |
| `tests/kernel_timers_test.c` | `aa52d1ed2e0355200411c9261958302c2d4c38f2fdd59196d5c0aa23fe6e8f00` |
| Local `kernel-timers-01/checks.json` | `bb801bd472f321a4f5fa65cec0d81ca6a55df4e6484115418006011a222fcdf3` |
| Local `recomp-timer-01/seed.json` | `4a8d476c32fe25991fcbf4d4a8f5692d81aaee8696e140c7233a07ba422c2fe1` |

## Next task

Integrate a narrowly defined real browser clock with this queue and a CPU-context-preserving DPC bridge. Keep monotonic durations separate from adjustable system time; the [High Resolution Time specification](https://www.w3.org/TR/2026/WD-hr-time-3-20260901/#dom-performance-now) and [ECMAScript Date.now contract](https://tc39.es/ecma262/multipage/numbers-and-dates.html#sec-date.now) explain those distinct sources. Measure/record browser precision and suspend behavior instead of claiming original clock fidelity. Any new Wasm host capability needs an explicit allowlist/signature and negative admission checks; existing title modules still admit no imports.

Verify callback frame/return cleanup, IRQL and complete integer/FP/SIMD/TLS/SEH state preservation with owned fixtures before executing the original callback. Keep unknown settings and unsupported shutdown visible. Only then connect ordinal 149, allow the real arming call to return, and observe the next original startup dependency in the browser. Do not skip the six-hour deadline, execute original code under Node/native tools, or substitute a guessed Boolean. Full kernel, rendering/audio/input, browser-local conversion, online/social/authority and original-game fidelity remain unaccepted in the full product ledger.
