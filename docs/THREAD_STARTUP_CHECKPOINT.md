# Original thread-entry checkpoint

Later progress: [the CRT startup checkpoint](CRT_STARTUP_CHECKPOINT.md) passes sixteen checks through object initialization and an explicit synthetic settings read before the original timer-arming request. This page preserves the earlier ten-check module and its historical next task; that module still passes through the current loader.

November 1, 2026: the browser now runs the original loader to return, creates its child thread, executes the original system/exception/notification routines and reaches the original thread-start entry. It deliberately stops at untranslated call `0x00033ccc`. **No playable game, renderer, complete kernel, general scheduler or end-user browser converter is delivered.** The complete original-game/custom-feature objective remains active.

This advances the [earlier kernel-255 stop](ORIGINAL_STARTUP_CHECKPOINT.md). That stop-only module and all earlier artifacts remain preserved; its six checks still pass through the shared loader after this change.

## Observed result

| Bounded requirement | Evidence | Limit |
| --- | --- | --- |
| Reuse existing analysis and original files | Direct Ghidra `-noanalysis -readOnly`; in-memory discovery of only immediate startup functions; supplied XBE digest unchanged | No repeat extraction/import or full decompilation; direct Ghidra output is not REA Evidence |
| Preserve the complete selected code | Eight functions, 681 envelope bytes (669 declared body bytes), 225 instructions; original/Ghidra bytes and all Capstone/Ghidra boundaries agree | A 12-byte split-body gap is included, not trimmed; translated instructions are not an executed-instruction count |
| Implement thread creation rather than return a success stub | Separate guest stack/TLS/PCR/thread prefix, recorded handle and runnable state; real original loader closes its handle, returns, then the child runs | Four-record, bounded boot dispatcher; no preemption, waits, resume, termination or general object manager |
| Preserve original TLS and entry behavior | Original system routine and exception prologue initialize the self-reference; original notification routine enters/leaves its critical section; original thread-start entry reached | Selected normal startup path only; no exception-dispatch, registered-callback or general multithreaded fidelity acceptance |
| Fail explicitly at the next missing code | Trap at direct target `0x00033ccc`, return address `0x00030f15` | Its exact identity and dependencies are the next investigation; do not invent a success body |
| Local content and browser-only execution | Private 18,601-byte module selected with the compatible local XBE; no imports/implicit start, fixed unshared 128 MiB Wasm memory with 64 MiB guest RAM | Developer compilation remains outside the browser; end-user conversion must eventually be browser-local |
| Reject, clear and recover | Malformed module, incompatible executable and wrong proof ABI rejected; Clear empties both selections/output; reselect reaches the same thread entry and passes ten checks | Desktop engineering proof, not the supported-browser/controller/performance matrix |

The seven observed original entries, in order, are `0x00030f7f`, `0x0003439f`, `0x0003417a`, `0x00034307`, `0x001e04cc`, `0x00034198` and `0x00030f10`. The eighth translated function (`0x00033c67`) was not reached. The final trace has twelve entry/exit records; do not describe all 225 translated instructions as executed.

Ten checks passed in both the expanded run and the rejection/Clear/recovery run: thread creation with the original ten-argument frame, closed-handle lifetime, loader-return/child-start ordering, system/SEH entry, FS/thread/TLS agreement, original TLS self-reference, the two-argument system frame, explicit next stop, original thread-start entry and balanced notification critical-section state. Both runs reported no browser warnings/errors and `gameBooted: false`.

The four observed kernel calls are 255 (`PsCreateSystemThreadEx`), 187 (`NtClose`), 277 (`RtlEnterCriticalSection`) and 294 (`RtlLeaveCriticalSection`). Each completed its implemented boot-path operation. Only uncontended/recursive critical sections are supported; contention traps explicitly instead of pretending the wait completed. Other kernel calls and untranslated original targets still stop. There are no permissive guest success/no-op stubs.

The child has ID 1 and handle 256 (closed while the child remains runnable/running), a 65,536-byte stack, a 20-byte TLS allocation and a self-pointer aligned to 16 bytes. Its guest object starts at `0x03c00000`, PCR prefix at `0x03c00200`, stack top at `0x03c11000`, TLS at `0x03c10fec`, and original initialization writes `0x03c10ff0` into the TLS self-reference. Allocation zeroes the block, so zero bytes alone do not prove the original zero-fill instructions ran; the changed self-reference and original execution trace are the stronger observations.

## Public contract and implementation boundary

The independently written `thread_bootstrap.c/.h` implements only the required boot path and explicit failure cases. It reserves a checked arena separate from the loaded image/loader stack. It checks output ranges and options, creates distinct contexts/frames, preserves runnable ownership after closing a handle, honors suspension without dispatching it, and refuses unsupported/resource-exhausted operations. The proof harness records kernel calls, exposes only metadata and stops on every unsupported path. It dispatches the child after the loader returns; this is not evidence that the original kernel always schedules in that order.

Contract sources were read at pinned commits: nxdk `f8c52e4be65bab7d6e10455db8d86d69d232bbe3` [thread/system routine declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L2466), [TLS startup](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/winapi/thread.c#L33) and [critical-section layout](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxdef.h#L179); Cxbx `585c49a50af1255ab155099e06f24505f9c5a800` [PCR/PRCB layout](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/common/types.h#L2141). These support ABI interpretation, not a controlled run of the matching shipped kernel. No external source bodies replace original startup, and no vendor sources are published here.

`local-title-proof.mjs` shares admission/loading between the old and new workers and rejects execution outside a browser worker before compilation/instantiation. The source-only server has seven fixed GET/HEAD routes; `/thread` selects this proof, `/` retains the earlier one. Private modules/XBE, uploads, queries and traversal remain rejected. Worker network connections are prohibited. Clear/error/timeout/completion terminate the worker, and stale generations cannot replace newer output. Game-derived Wasm is never HTTP-served, uploaded, committed or executed under Node/native tools.

## Retained private evidence

| Local artifact | SHA-256 |
| --- | --- |
| Supplied `Fuzion Frenzy (USA)/default.xbe` | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| `research/verification/recomp-thread-05/seed.json` | `abdc24f82569976353bcb5d43e202b59795375c06ecfefd429f0607d4e4650e4` |
| `research/verification/recomp-title-thread-03/generation.json` | `02793e24eff406461fd3975638d9ea165cc2fb8e5cd4d73584d3f921433a178a` |
| `research/verification/recomp-thread-build-02/build.json` | `cdbc599c628300555d4875d93fb1930e33e005f2eaccaefa8edeea15c2b164e8` |
| `research/verification/recomp-thread-build-02/startup.wasm` | `c813f2cc18ae20a7cd4c5d604c93d94f52788fc67a27e56833ce16e85825a2c4` |
| `research/verification/thread-startup-browser-20261101-02.json` | `afca7493dbca1c6e703bb9448ed7b80a2fe800f5a54040a1fb850272bdb392b2` |
| `research/verification/thread-startup-browser-20261101-03.json` (rejection/recovery) | `6162fc094d632598c288d12913e37a69b5117835186716e81500f0247a61d527` |
| Matching recovery `.jpg` | `4da4ef37e677f6860ea13e22bb32e78e34641a838dedbc4a767a1b34746f685c` |
| `research/verification/original-startup-browser-20261101-03.json` (old-proof regression) | `e4c5a6e09db07610ed56c419cdcd224f9502d071fb15595dc104a7bf591e0d86` |
| `research/verification/recomp-crt-01/seed.json` (next initializer, static only) | `74334cf9309b4838bb4cb0b4a902cac203e55cc3d39389186257103d48d73c2c` |

Earlier `recomp-thread-01..04` and `recomp-title-thread-02`/`recomp-thread-build-01` preserve discovery and the intermediate five-routine/TLS proof. The first expanded seed exposed the split-body gap; generation rejected it rather than silently omitting bytes. `DecodeBoundedFunctionGaps.java` subsequently decoded only the selected undefined 12-byte gap in read-only analysis memory, with a checked 256-byte gap budget and no defined-data overwrite. The final exporter/translator verify the complete envelope and reject overlaps, missing instructions and decoder/byte disagreements.

Pinned tools remain xboxrecomp `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6, Ghidra 12.1.4 and Emscripten 3.1.50. Exact compile arguments, generated/header digests and owned C source identities are retained in the private build report; current compiled C identities still match it. No original code executed during analysis/translation/compilation or Node's compile-only module inspection. No BIOS, disc extraction or global tool/model/settings change occurred.

## Validation and next task

Local checks: source-owned C context/critical-section tests passed with GCC C11 `-O2 -Wall -Wextra -Werror`; four Python function-envelope cases and syntax checks passed; seven runtime delivery/resource/browser-worker-boundary tests passed. Actual Ghidra export, title compilation, ten-check browser runs and the earlier six-check regression are separate local evidence. CI adds the Python cases on both platforms and the owned native C tests on Linux; it executes no original game or private title toolchain.

`npm run check` passed for 126 source files, and `git diff --check` passed. Path-named private artifact digests in this document were compared to the actual files, the supplied executable identity is unchanged, and current compiled C sources match the private build report. Title/evidence paths remain ignored. The verified temporary server reached terminal state after both tabs were cleared and closed.

Source-only checkpoint `b221b5ad1930ccc2ee4e118caf9e1b79315a2193` is published. [CI run 41315595912](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41315595912) completed successfully on Windows and Linux. Both jobs passed source/format checks, Python syntax and four envelope cases, seven runtime-boundary tests, preserved prototype tests/build, environment and clean-diff checks. Linux additionally passed the owned boot-context/locking fixture and existing triangle test. CI executes no original game or private title toolchain. This documentation follow-up records the terminal result without changing the checked implementation.

Reproduce the owned C check with `gcc -std=c11 -O2 -Wall -Wextra -Werror tests/thread_bootstrap_test.c src/runtime/thread_bootstrap.c -o <new-private-output>` and run that owned fixture. Generate title code with `prepare-title-recomp.py` using the final seed, compatible XBE and existing profile into a new attempt. Compile `thread_startup_probe.c` together with `thread_bootstrap.c`, the private generated include directory and pinned runtime types, following the retained build report. Select that module locally at `http://127.0.0.1:4180/thread`. Never run the title module outside the browser or serve it as a fixture asset.

Static follow-up while CI ran: a new read-only discovery/export for `0x00033ccc` establishes a contiguous 271-byte / 97-instruction function. Its first direct call is `0x000346a7`; there are seven distinct direct targets and two declared calls through import slot `0x002533e0`, mapped by the existing profile/pinned dictionary to ordinal 67 (`IoCreateSymbolicLink`). The selected function has no direct FS operations or floating-point instruction family in the exported listing. This is static evidence only; no new original instructions or file/namespace operations executed. Its exact identity and the behavior of its callees remain unresolved. Raw decompilation, seed and logs stay in `research/verification/recomp-crt-01/`.

Next concrete task: use that saved initializer evidence, inspect first dependency `0x000346a7` and other immediate CRT/startup calls as needed, establish their actual kernel/memory/FS requirements, extend the bounded checked function bundle explicitly beyond its current eight-function limit, and advance original startup in the browser. Implement any required symbolic-link/local-file namespace through a real browser-local contract; do not return success without the represented operation. Preserve TLS/SEH/handle/critical-section behavior and all explicit unsupported stops; do not replace the initializer with guessed success. General scheduling, contention/waits, exception dispatch, full memory/kernel/render/audio/files/saves, browser-local end-user compilation, input, online/social/server authority and full fidelity remain unresolved in the [product ledger](ORIGINAL_EXECUTION_PROGRESS.md). Keep the full goal and selected recompilation route unchanged.
