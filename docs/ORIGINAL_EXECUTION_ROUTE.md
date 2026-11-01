# Corrected target: original game execution inside the browser

User clarification, October 31, 2026: run the actual original Fuzion Frenzy entirely inside the browser. A fallback replica is acceptable only if identical in every minute detail. Native helpers, local-emulator streaming and remote streaming do not meet the clarified requirement. The approximate Twisted System reconstruction is preserved engineering research, not the requested product or evidence of parity.

This supersedes the prior reconstruction-first route, partial-pack delivery assumptions and permitted approximation baseline. Preserve the local-only/player-supplied-content and source-publication boundaries. Do not continue approximating additional mechanics or quietly fall back to reconstruction if an execution experiment fails. Optional modernization cannot replace the original baseline under this requirement.

Current build/source evidence and the full uncompleted feature ledger: [original execution progress](ORIGINAL_EXECUTION_PROGRESS.md). This supersedes metadata-only investigation status below.

November 1, 2026 update: the user confirmed no local MCPX boot ROM or BIOS and asked about native web execution. The [original-startup checkpoint](ORIGINAL_STARTUP_CHECKPOINT.md) now verifies original entry/thread-wrapper execution and TLS initialization in a browser worker, stopping explicitly at unsupported kernel 255. No BIOS or success stub was used; all generated title code was selected locally and never served/uploaded/committed. Next is the thread-creation/TLS/scheduling contract. The game has not booted, and complete runtime acceptance/browser-local end-user conversion remain unresolved. The [earlier native feasibility](NATIVE_WEB_FEASIBILITY.md) and xemu/renderer work are preserved.

The user explicitly confirmed that the plan must pursue recompilation. Static recompilation to WebAssembly is the selected implementation direction (D023; earlier feature comparison D022). The checkpoint compares social/control wrappers, mixed-seat online play, title hooks, recovery and authority. Keep emulator work as supporting research and report material blockers before proposing a route change; do not automatically fall back after a failed probe. Native title compatibility and the server/content boundary remain unproved.

## Bounded route investigation

Primary sources checked October 31, 2026; external documentation is evidence about those projects, not a successful run of this title here.

| Route/source | Observation | What remains unproved |
| --- | --- | --- |
| [xemu](https://xemu.app/) and [source](https://github.com/xemu-project/xemu) | Original Xbox full-system emulator; documented native Windows/macOS/Linux support | A browser build, browser CPU/graphics/audio integration, title performance and exact output here. No ready browser implementation was verified. |
| [xemu compatibility testing list](https://xemu.app/testing_priority) | Lists Fuzion Frenzy as Playable, with last evaluation March 11, 2021 and priority retesting indicated | Playable is not perfect fidelity, current compatibility or a match to our supplied executable. It is not browser evidence. |
| [xemu required files](https://xemu.app/docs/required-files/) | Full-system route requires MCPX boot ROM, compatible BIOS and HDD image in addition to the game | The supplied game folder does not itself supply a verified execution environment. No firmware was acquired or downloaded. Browser feasibility research can proceed independently of obtaining those inputs. |
| [QEMU Wasm backend sample](https://github.com/ktock/qemu-wasm-sample) and [author's patch explanation](https://lists.gnu.org/archive/html/qemu-arm/2025-04/msg00153.html) | Documents an Emscripten/QEMU system-emulation path with a Wasm translation backend and browser workers | This is not an Xbox hardware implementation or a Fuzion Frenzy demonstration. Reusing a CPU backend alone would not establish NV2A graphics, audio, devices or title compatibility. |
| [xboxrecomp](https://github.com/sp00nznet/xboxrecomp) | Describes native Windows recompilation and mentions WebAssembly as a portability possibility; documents Windows memory mapping and HLSL translation | No Fuzion Frenzy match was found in the inspected README. Neither generated C nor the WebAssembly mention establishes a working browser runtime, title correctness or performance. |
| [ImLunaHey/emulators](https://github.com/ImLunaHey/emulators) | Browser/Wasm project labels its Xbox core Foundation, reporting nxdk homebrew and triangle geometry | This is an actual browser-oriented candidate worth inspecting, but its own stated scope does not establish retail Fuzion Frenzy compatibility. No local build/run was performed. |

Search ambiguity: the [other Xemu Emscripten demos](https://github.lgb.hu/xemu/emscripten/index.html) emulate MEGA65/C65/PRIMO machines. They are not a browser port of the original-Xbox xemu. Running an emulator in an Xbox console's browser also does not establish emulation of Xbox games. Do not count either as a successful route.

## Acceptance before claiming success

The first execution milestone must run original game instructions from the compatible player-supplied image in a normal desktop browser, render an original controllable screen and demonstrate input. A homebrew triangle, parsed XBE, converted model, recompilation output or streamed native window cannot substitute for this title proof.

Then verify original menus, full catalog/modes, characters, arenas, rules, AI, animations, camera, audio, save behavior and round/tournament flows, with repeatable original-reference comparisons. An emulator's general compatibility rating does not prove every detail identical. Browser input/audio scheduling and rendering differences must be measured, not hidden under a fidelity claim. Keep unresolved cases explicitly unaccepted.

The browser receives no original content from our server. Files remain player supplied and local. Any generated code containing original game instructions or data must remain local and ignored; do not publish a recompiled original binary disguised as independently written source. Keep independently authored runtime/emulator code and its upstream license requirements distinct from game-derived output.

## Next bounded investigation

Continue the selected recompilation route from the recorded kernel-255 stop. Establish and test thread creation, TLS/FS and scheduling, then privately translate only the immediate original child-startup functions needed for the next browser result. Preserve the existing Ghidra project, supplied files and failed attempts. Record actual execution or a concrete failure; do not start another approximation, full decompilation or emulator rewrite.

Current result: the corrected target and recompilation direction are recorded; partial original CPU startup executes in the browser. **No playable original-game screen, complete runtime or identical replica has been delivered.**
