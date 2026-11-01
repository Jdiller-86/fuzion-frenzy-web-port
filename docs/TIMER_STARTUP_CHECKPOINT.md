# Browser clock and original timer-arming checkpoint

November 1, 2026: original startup now arms its real six-hour timer, returns through the original arming/helper routines and enters the next initializer in the browser. Both explicit synthetic settings profiles pass twenty checks. The next stop is untranslated helper `0x001df9c0`, reached from `0x000322ef` with return address `0x00032327`. **No playable screen, complete scheduler/runtime, original-console fidelity or custom-feature acceptance is established.** The complete original-game/browser/online/social goal and selected recompilation direction remain active.

This connects the [owned timer queue](TIMER_QUEUE_CHECKPOINT.md) to the prior [CRT startup proof](CRT_STARTUP_CHECKPOINT.md). Original files, title-derived modules/code, raw analysis, test output and screenshots remain local/ignored. No firmware, game download, repeat extraction or global tool/model/settings change occurred.

## Observed original execution

| Contract | Verified evidence | Limit |
| --- | --- | --- |
| Preserve selected original code | Six saved exports compose 16 functions / 746 instructions, 2,302 envelope bytes / 2,264 declared body bytes | Original-byte identity and independent decoder boundaries agree; translated counts are not executed-instruction coverage |
| Advance original startup | 13 distinct original routines enter; 23 trace records, including a second original SEH-prologue entry | Initializer `0x00033ccc` and the game have not completed startup |
| Arm the original timer | Ordinal 149 consumes the actual four-word NTAPI frame and queues timer `0x00319180`, DPC `0x00319160`, signed relative due time `-216000000000` in 100 ns units | Only the observed one-shot/empty-wait-list boot path is integrated; general waits/periods/interrupts remain unsupported |
| Return an actual insertion result | Previous insertion is false; guest return value/stack cleanup let original `0x0003467a` and `0x000346a7` return | This is a queue operation, not an unconditional-success stub |
| Use a real browser clock | Nineteen bounded samples in each recorded run; timer deadline equals the sampled monotonic time plus six hours | Samples are synchronous safe-point observations, not proof of timer precision, real six-hour expiry or suspend behavior |
| Preserve pending work | One active timer, valid guest links, zero pending/delivered callbacks, idle DPC state and passive IRQL at the next stop | The original deferred routine is translated/admitted but has not executed |
| Keep the next missing code explicit | Direct-target stop at `0x001df9c0`, return `0x00032327`, nine completed boot-path kernel operations | No guessed replacement or skipped helper |

The nine completed boot-path operations are thread creation/handle close, uncontended critical-section enter/leave, DPC/timer initialization, the explicit misc-setting read, exact zero-status conversion and timer arming. Other unsupported services still stop. The final private module is 54,430 bytes with fixed unshared 128 MiB Wasm memory and 64 MiB guest RAM. It has exactly one host import and no implicit start; these sizes are not peak-memory or release-device acceptance.

With synthetic misc flags 0 and 1, guest word `0x003191a8` is respectively 0 and 1 at the stop. Both request the same six-hour timer. The static deferred routine reads this state and can call ordinal 360 `HalInitiateShutdown`; no original callback/shutdown execution was observed. Matching original settings and resulting complete console behavior remain unknown. Keep guest shutdown explicit and confined to the virtual runtime; it never authorizes a host shutdown action.

## Clock capability and callback context

ABI 4 requires the sole import `env.ff_host_clock`, exactly `(i32) -> void`. The host writes sixteen bounded little-endian bytes into admitted Wasm memory: monotonic low/high and system-time low/high words. `browser-clock.mjs` uses worker-relative `performance.now()` elapsed time and UTC `Date.now()` converted to the 1601 epoch. It rejects backward/nonfinite/unsafe monotonic samples, out-of-range timestamps and invalid output ranges. It exposes no files, networking, DOM, process or upload capability. No implicit console-clock default or matching-hardware claim is made.

`inspectProofWasm` admits that signature only with explicit clock opt-in; other imports, duplicate boundary sections, implicit startup, unbounded/shared/excessive memory and malformed declarations fail before instantiation. The loader also ties the capability to ABI 4. ABI 1/2/3 retain no imports; original-code execution still requires a browser worker. `clock-library.js` is the pinned compiler declaration; its fallback throws and is not used by the standalone module's actual browser host. The first strict compiler attempt failed on the undeclared symbol and remains preserved; no undefined-symbol or import guard was disabled.

The [High Resolution Time specification](https://www.w3.org/TR/2026/WD-hr-time-3-20260901/#dom-performance-now) and [ECMAScript Date.now contract](https://tc39.es/ecma262/multipage/numbers-and-dates.html#sec-date.now) support the distinction between monotonic duration and adjustable system time. Recorded final runs observed a minimum positive sample delta of 50 ticks (5 microseconds); this is not measured clock resolution, scheduling latency or original-console fidelity. Background/suspend behavior and long-running browser scheduling remain unverified.

`dpc_context.c` and `recomp_cpu_snapshot.h` preserve all modeled integer, flags, FP/control, MMX/SSE, FS/TLS/SEH and translator return state. A dedicated admitted guest stack carries the four DPC arguments and checked return sentinel. Entry sets an empty exception chain, callback stack bounds, IRQL 2 and DPC-active state; normal completion, invalid cleanup and explicit fault recovery restore the interrupted context while retaining legitimate guest memory effects. Nested/high-IRQL entry fails or defers. The queue is popped only after address/frame admission, with no guest execution/yield between those steps.

The PCR prefix offsets are supported by pinned Cxbx [public type definitions](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/common/types.h#L2141); the four-argument ABI is also declared by pinned [nxdk](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L936). Those references do not prove full matching-kernel behavior. New code is independently authored. Owned native, sanitizer and Wasm callback fixtures pass before title integration; original deferred-callback execution is still unobserved.

Polling currently occurs at translated function entry and kernel-service boundaries. This is synchronous boot bring-up, with no general preemption, cooperative yield/resume, full exception dispatch or long-timer delivery acceptance. The proof terminates at the explicit unsupported operation; it is not a running game left waiting six hours. The no-import earlier module still passes sixteen checks. Malformed-module and missing-settings rejection, Clear of all three values/output and successful reselect/recovery also pass with no browser warnings/errors. The temporary tab and verified source-only host were cleared/closed.

## Evidence and validation

| Local-only artifact | SHA-256 |
| --- | --- |
| `research/verification/recomp-title-recomp-title-timer-01/generation.json` | `5af94d989e6d7f5a2e00381b0be6e96526e5e3bcee0764cdc86e63d4a830e99b` |
| `research/verification/recomp-timer-build-03/build.json` | `1b23d00fbabbb4d26a73322b010d63fbe6149ed1db8bfff0a843b6077e76897f` |
| Matching `startup.wasm` | `da2a37a95b9b2ed2f6aa7166a8c1b7059e6bc95bce91e284d11beab5e2fefef8` |
| `research/verification/dpc-context-03/checks.json` | `2cb49fd645be43cf300e452cafa678c7e88aa706768135841bc122a51565aa25` |
| `research/verification/timer-startup-browser-20261101-02.json` (synthetic 0) | `f04d126e1ad98e7ac3a5bfdabb840ee15a973e122890f0a2060820af49616c12` |
| Matching `03.json` (synthetic 1) | `fefe49598fed884c5cc7f25e6e24f7481cdb959f8b5fa6bfa2cffac9906249d9` |
| Matching `04.json` (legacy regression) | `23478515d6fa053458b83121edcacc58244f389e58050e6d9ea9cce1e5e89d14` |
| Matching `06.json` (rejection/recovery) | `6c6c49c56457a09fdbf16ffc7f7d0f0f15a8afd9da5af3652e0237a5bff506f4` |
| Matching `06.jpg` | `9d783718d6fac50d7e12b2b27377a014a9764a538305f7446d3ab8a42ef63fd5` |
| `research/verification/recomp-stack-01/seed.json` (next helper) | `58e5d039fc4c91da1f2c151151aa9418fcccc96c936676b92de9710944637af8` |

Earlier diagnostic `01.json`, rejection record `05.json`, final Clear record `07.json`, failed build `recomp-timer-build-01` and failed translation `recomp-title-timer-02` remain preserved. Final title build `03` corrects context member names that collided with the recompiler's register aliases. Its 54,430-byte Wasm is byte-identical to browser-verified build `02`, so the recorded browser observations apply to that exact rebuilt artifact. Owned context attempts `01/02` remain preserved; final fixture `03` also enables the register aliases during capture/restore tests.

Thirteen compiled owned C/header/compiler-declaration hashes and nine owned-fixture source/header hashes match their final reports. The alias-enabled callback fixture passes GCC C11 `-O2`, address/undefined-behavior sanitizers and Emscripten 3.1.50 Wasm; its owned code alone runs under SDK Node 16.20.0. Eleven source-owned runtime/admission/delivery tests pass. The new Linux CI step uses only owned C sources; actual original browser execution is separate local evidence. CI/publication results are recorded in status/handoff after completion.

The preparation/publication guard passes for 146 source files and whitespace checks pass. Original executable identity is unchanged; vendor source stays clean at the recorded pin. No publication guard or tool/model setting was changed to make these checks pass.

## Next task: complete the next helper's evidence

Read-only Ghidra reuse exports established helper `0x001df9c0`: 664 declared body bytes, 821-byte envelope, 231 listed instructions, 21 body ranges, 20 instruction gaps totaling 157 bytes and sixteen computed jump sites with candidate flows. No calls are listed. The observed caller supplies two guest-stack pointers and length 48. Block-copy instructions and direction changes suggest an optimized copying helper; this role is an inference, with no recovered original symbol or observed helper-body execution. Treat `recomp-stack-01` as an attempt label.

Adding that seed correctly fails the existing full-envelope coverage check. First classify the gaps as actual instructions, defined data/tables or padding in the existing project. Preserve the complete envelope and original-byte identity; do not trim gaps or force data to decode as instructions. Establish computed-branch/table semantics, then make the smallest verified exporter/translator extension with meaningful owned mixed-layout fixtures. Run the actual helper in the browser and verify its observed copy/register/stack behavior before continuing original initializer `0x000322ef`. No repeat extraction, full decompilation, fake helper or automatic route change is authorized by this failure.

Full kernel/memory/file services, rendering/audio/input, browser-local end-user conversion, online/social/server authority, supported-device performance and complete original-game fidelity remain unresolved in the [full product ledger](ORIGINAL_EXECUTION_PROGRESS.md).
