# CRT startup and explicit settings checkpoint

Later component progress: [the one-shot timer queue](TIMER_QUEUE_CHECKPOINT.md) passes owned native/sanitizer/Wasm scenarios. It is not yet connected to this title proof: the sixteen-check original timer stop below remains the latest original execution boundary. Reuse the component and callback export for the pending browser-clock/CPU-context bridge.

November 1, 2026: original startup now initializes its deferred-callback and timer objects, reads an explicitly supplied test setting and reaches the original six-hour `KeSetTimer` request in the browser. Both source-owned settings inputs pass sixteen checks. **Timer arming/delivery, a playable screen, full runtime and original-console fidelity remain unaccepted.** The complete original-game/custom-feature goal and selected recompilation route are unchanged.

This advances the [thread-entry checkpoint](THREAD_STARTUP_CHECKPOINT.md). Its ten-check module still passes through the current loader without a settings input. Original files, generated code/modules, analysis and screenshots remain local/ignored; no firmware, original source or new disc extraction was acquired.

## Observations and limits

| Bounded work | Verified result | Boundary |
| --- | --- | --- |
| Compose existing analysis exports | Five checked seeds, 15 original functions, 742 instructions, 2,284 envelope bytes / 2,246 declared body bytes | Every original byte and decoder boundary agrees; translated counts do not establish executed-instruction coverage |
| Admit a real local import thunk | Original `0x00034720` is a six-byte indirect jump through declared slot `0x002533b4` (ordinal 24) | Only a single verified local jump to a declared import is admitted; external/arbitrary thunks remain rejected |
| Execute beyond original thread entry | Twelve original routines observed, including `0x00033ccc`, `0x000346a7`, `0x000309b2`, `0x00034720` and `0x0003467a` | Initializer `0x00033ccc` has not returned; later heap/file/namespace requirements remain unexecuted |
| Initialize kernel objects | Ordinals 107 (`KeInitializeDpc`) and 113 (`KeInitializeTimerEx`) populate the actual requested guest objects | No callback delivery or timer queue is supplied by initialization alone |
| Read an explicit setting | Ordinal 24 (`ExQueryNonVolatileSetting`) requests `XC_MISC` / index 17 as a four-byte `REG_DWORD`; values 0 and 1 produce the selected value, type 4 and required length 4 | Synthetic inputs, not observed original EEPROM values; no implicit fallback/default and no other settings are fabricated |
| Convert the observed status | Ordinal 301 maps the observed `STATUS_SUCCESS` to `ERROR_SUCCESS`; original wrapper returns | Nonzero statuses still stop; this is not a complete error-conversion implementation |
| Preserve the next unsupported operation | Both settings inputs request ordinal 149 (`KeSetTimer`), then trap explicitly | No fake timer success or silently skipped deadline |

The eight completed boot-path kernel operations are thread creation, handle close, uncontended critical-section enter/leave, DPC initialization, timer initialization, the explicit misc-setting read and the observed zero-status conversion. All other unsupported operations retain failure stops. The final module is 39,132 bytes, uses no host imports/implicit start, and retains fixed unshared 128 MiB linear memory with 64 MiB guest RAM. Those numbers do not establish peak memory or release-device performance.

The DPC is at `0x00319160`, deferred routine `0x00034695`, context 0. The notification timer is at `0x00319180`. The original arming frame requests that timer and DPC with signed due time `-216000000000` in 100 ns units: six hours relative. Return address is `0x00034694`. Neither `KeSetTimer` nor the deferred routine has executed. This is a real clock/queue/callback requirement, not permission to return a guessed Boolean and continue.

Sixteen browser checks cover the earlier thread/TLS/SEH/locking cases, original CRT/helper entry, DPC/timer fields, the explicit typed setting result, exact success conversion and complete six-hour request. Both final settings runs report `verified: true`, `gameBooted: false` and no browser warnings/errors. Missing settings is rejected before the original entry runs; Clear releases all three selections/output. The earlier ABI-2 module still passes ten checks with no settings profile. The temporary tab/server were cleared, closed and verified terminal afterward.

## Settings provenance and source contracts

Material fidelity gap: no matching original console EEPROM/settings profile was supplied. The value requested here is unknown for an original reference console. `tests/fixtures/settings-misc-0.json` and `settings-misc-1.json` are independently authored inputs with mandatory `synthetic: true`; both take the observed timer-request path. They are not EEPROM dumps or evidence of an original hardware default. The public proof does not claim original-console fidelity from either input.

ABI 3 requires selecting a bounded local JSON profile before execution and exposes only this explicit DWORD input to the kernel adapter. The worker rejects absent/malformed/oversized profiles, wrong kinds/provenance and invalid values. Nothing is uploaded, persisted or read from host settings. The ABI-2 proof remains compatible without a profile. Future product configuration and matching-reference validation must remain explicit; do not quietly turn these test fixtures into console defaults or manufacture keys/other settings.

The implemented interfaces/layouts were checked against pinned nxdk [kernel declarations/types](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h), including `XC_MISC`, the DPC/timer structures and the query signature. Pinned Cxbx [initializers](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlKe.cpp#L931) and [settings-query behavior](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlEx.cpp#L394) support interpretation of the ABI, not matching-console runtime fidelity. New C implementations and test fixtures are independently written; vendor sources and original routines are not redistributed.

An early registration-role inference for `0x00034720` was wrong. Its actual import target is the nonvolatile-settings query. The first diagnostics and old-reference mismatches remain preserved; no result is relabeled as an original-console observation. The expanded verifier separately checks the newly established boundary and does not weaken full-game acceptance.

## Reproducible source and retained evidence

`prepare-title-recomp.py` accepts repeated `--seed` arguments: at most eight exports of eight functions each, retaining the 8,192-instruction aggregate budget, source identity, full-envelope coverage, overlap rejection, original-byte comparisons and independent decoder checks. Duplicate functions or mixed builds fail instead of replacing previous code. Generation records all seed digests and recognized kernel thunks. This remains a bounded developer tool, not a browser-local end-user compiler or full architecture freeze.

The final bundle uses `recomp-thread-05/seed.json` and `recomp-crt-01..04/seed.json`, all under `research/verification/`. Their identities are recorded in generation metadata. The first has the eight prior routines; subsequent exports add only the observed initializer's immediate dependencies. Read-only Ghidra reuse, scoped launcher state and all older projects/artifacts are preserved. No original instructions execute during analysis/translation/compilation; original execution remains browser-worker-only.

| Local-only artifact | SHA-256 |
| --- | --- |
| Supplied `Fuzion Frenzy (USA)/default.xbe` | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| `research/verification/recomp-title-crt-03/generation.json` | `7067a12335d3ca664ef75afdc9fe89107c01d01329a75201f4b974ecb87dc9e5` |
| `research/verification/recomp-crt-build-04/build.json` | `603ac8cbd184526df0be21c396f5790ea98f046cc8d6aff08f2d277698a8c0ad` |
| `research/verification/recomp-crt-build-04/startup.wasm` | `75260f7532b9fc32bd959e4fecf6e5e910387ef45b64b51e38495e6b9a52a80e` |
| `research/verification/crt-startup-browser-20261101-06.json` (synthetic 0) | `9d244fba0ef44587902b2a8ac6a50489a110a6a5b7779eed70b4478024dcdb51` |
| `research/verification/crt-startup-browser-20261101-07.json` (synthetic 1) | `10da4e9a420a6012d916a57eb31e4872e5fa2926b0d383945f9ab02f2702ecab` |
| Matching final `.jpg` | `56a6fe9f45c4704cf4959cfecfa310f3202cdfcef299e7321125c1a678933057` |
| `research/verification/thread-startup-browser-20261101-04.json` (legacy regression/Clear) | `ff0354accf9d80d27a374c5df4b0f02ab8e8cc297f20fc2297d1eb8b5b71d7f5` |

Earlier CRT browser diagnostics `01..05`, builds `01..03`, seeds and logs stay ignored. Exact compiler arguments, owned C/header hashes and generation identity are in the final build report. Current compiled sources match that report; original XBE identity is unchanged. Pins remain xboxrecomp `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6, Ghidra 12.1.4 and Emscripten 3.1.50; no global tool/model/settings changed.

Owned tests cover both timer types, DPC fields, preservation of unspecified fields, alignment/range and failure atomicity; typed misc values/type/length, short buffers and unsupported keys; JSON profile admission; and composed-seed/thunk bounds. Local C11 `-O2 -Wall -Wextra -Werror` fixture runs, eight Python cases and eight runtime tests pass. CI adds the two new owned C checks on Linux. None uses original game files or private tools; actual compilation/browser execution is separate local evidence.

Final source review also passes Python syntax, the preparation/publication guard for 135 source files and whitespace checks. The first restricted runtime rerun failed only at loopback connections (`EACCES`); the rerun with loopback access passes all eight cases. All seven compiled owned C/header hashes and every artifact digest above match current files. CI publication results are recorded separately in the current status/handoff.

To reproduce, combine the five saved seeds with repeated `--seed` arguments and a new attempt label. Compile `thread_startup_probe.c`, `thread_bootstrap.c`, `kernel_objects.c` and `local_settings.c` with the pinned private type header/generated includes, exporting the existing ABI plus `ff_probe_misc_flags`; the retained build report has exact flags. Select the private module, compatible local XBE and one source-owned settings JSON at `/thread` on the source-only host. Do not serve the module or run it under Node/native tools.

## Next task and acceptance still open

Implement and verify the clock-backed `KeSetTimer`/timer-queue/DPC contract before letting the original arming call return. Establish relative/absolute time, previous-insertion return value, object/queue state, rearm/cancel, callback argument/context behavior and safe dispatch; use controlled-clock owned fixtures instead of waiting six hours or ignoring the deadline. Inspect/translate the original deferred routine at `0x00034695` only as required, preserving all CPU/TLS/SEH state. If host clock access is introduced, define and test that narrow worker capability explicitly; the current no-import proof is not permission for arbitrary host access.

Then continue original CRT bring-up. Virtual memory, remaining settings/statuses, file/namespace services, general scheduling/waits/interrupts/exceptions, rendering/audio/input, browser-local compilation, online/social/server authority and full original fidelity remain unresolved in the [full product ledger](ORIGINAL_EXECUTION_PROGRESS.md). No automatic switch to emulation, approximate mechanics or a smaller product follows a failed probe.
