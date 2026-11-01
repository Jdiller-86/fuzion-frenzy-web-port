# Original startup execution checkpoint

Later November 1 progress: [original thread-start entry is now reached](THREAD_STARTUP_CHECKPOINT.md) after implemented boot-thread/handle/TLS/locking behavior. The stop-only proof below remains preserved and its six checks still pass; its old next-task paragraph is superseded by that newer checkpoint. No playable original-game screen or full runtime has been delivered.

November 1, 2026: privately recompiled original startup code executed in a browser worker, using the player-selected local executable. It entered the original entry point and thread-creation wrapper, initialized the observed TLS globals and deliberately stopped at the first unsupported kernel call. **The game has not booted; no original controllable screen, full runtime or fidelity acceptance is claimed.** The full original-game and custom-feature objective remains active.

## Observations and acceptance

| Bounded requirement | Observed evidence | Limit |
| --- | --- | --- |
| Reuse the verified build and analysis | Existing `FuzionFrenzySetup` project processed with `-noanalysis -readOnly`; original XBE identity still matches F01 | No repeat extraction/import or full decompilation |
| Mechanically translate actual original code | Four established contiguous functions, 305 bytes / 104 instructions; original bytes match Ghidra memory and every Capstone/Ghidra instruction boundary agrees | Four translated functions are not four executed functions; no whole-title coverage |
| Admit only a bounded browser module | Private Wasm is 6,397 bytes, has no imports or implicit start section, and uses one fixed unshared 128 MiB linear memory | 64 MiB guest RAM inside that memory; not a complete Xbox memory/alias/device model |
| Execute original startup in the browser | Trace enters `0x00030f7f`, then `0x0003439f`; TLS allocation becomes 20 bytes and index becomes -5 | Only these two original routines were observed; the trace does not count every executed instruction |
| Preserve unsupported behavior as a stop | Kernel ordinal 255 stops through a recorded trap, with return address `0x000343de` and all ten argument words matching the independently derived startup frame | No kernel service returned success; thread creation and child startup remain unimplemented |
| Repeat and recover | Initial run and repeat passed six checks; a further run after malformed-module rejection, incompatible-XBE rejection and Clear also passed | Engineering check in the available desktop browser, not a release-browser/performance matrix |
| Preserve local content | Both files selected through local file controls; source-only server exposes four fixed routes, rejects private paths/queries/uploads and prohibits worker network connections | Developer-prepared title module, not the required browser-local end-user conversion pipeline |

The four translated entries are `0x00030f7f` (90 bytes), `0x0003439f` (84), `0x00033c67` (101) and `0x0003417a` (30). Two untranslated direct targets remain explicit stop adapters. Three observed import slots are bound to stop handlers for ordinals 49, 187 and 255; this does not implement the executable's 111 declared imports.

The six browser checks cover the explicit unsupported-kernel stop, kernel return address, complete kernel stack frame, original function-entry trace, TLS allocation and TLS index. With proof stack top `0x03fff000`, the stopped stack pointer is 84 bytes lower. Its ten arguments are `[67104740, 0, 65536, 20, 0, 200464, 0, 0, 0, 213767]`. The pinned toolkit's import dictionary identifies ordinal 255 as `PsCreateSystemThreadEx`; that name alone does not validate a replacement implementation.

No BIOS was used. Original instructions ran only in the browser worker. Developer Ghidra/translation/compilation and Node module compilation/metadata inspection did not instantiate or execute original code. The successful browser recovery reported no warnings/errors. Clear released selections/output and the temporary tab and verified server were closed afterward.

## Local evidence identities

These files are ignored and must remain local. They are not public downloads or REA Evidence records; direct Ghidra output, generated title code and browser observations are distinguished here.

| Artifact | SHA-256 |
| --- | --- |
| Supplied `Fuzion Frenzy (USA)/default.xbe` | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| `research/verification/recomp-startup-03/seed.json` | `213a5a97cd8f204fea9f1c8818d08a0587da663f00748e5f3e0f3fbc52b3a16d` |
| `research/verification/recomp-title-startup-03/generation.json` | `612eabecb6b22c9d2e5892be30c10fa1cfd20af9cc45fdb3c2467bf8a8f71215` |
| `research/verification/recomp-title-build-02/build.json` | `02c0ce524d7de6efedad8415e3017b0da8f89488b763af31ac06e121dc28d0d2` |
| `research/verification/recomp-title-build-02/startup.wasm` | `dc3b5599257c24b799862c0d89047a355f2ce49e9f71e96e74bdfd2ac5c8df5f` |
| `research/verification/original-startup-browser-20261101-01.json` | `496fc640ea06cfc1c2356e9f0616416afea657ef03a74688a933db1d7f881e70` |
| `research/verification/original-startup-browser-20261101-02.json` (rejection/Clear/recovery) | `b0459e58de1f8413d9a4cc767bbaa11444842e40ff882372c7cbd07744824b8a` |
| Both corresponding `.jpg` screenshots (same completed view) | `8a028993396635c63b8fe3690bea543bcdc2a14719c472a2d923bdd94d6b59de` |

Generation records the original metadata digest, clean xboxrecomp pin `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, selected functions, unsupported direct targets and generated-file digests. Capstone remains 5.0.6 and Emscripten remains 3.1.50. Exact compiler arguments and source/header identities are in the retained build report. The original executable digest was checked again after browser validation.

Retained failed attempts are tooling/harness evidence: `recomp-startup-01/02` encountered Ghidra launcher state outside the project; `recomp-title-startup-01` used an incorrect decoder helper; `recomp-title-build-01` rejected a compiler memory flag. Corrected attempts use scoped local launcher state, actual Capstone disassembly and an explicit linker memory maximum. These failures do not establish title incompatibility. Earlier seeds, generated code, build scripts and logs were preserved.

## Source and reproduction boundary

The public source adds a read-only seed exporter, bounded developer translator, independently written Wasm host harness, browser worker/resource admission, source-only proof server and source-owned tests. It contains no original instructions, generated title module, vendor runtime, game files, raw analysis or screenshots.

With the existing private tools and seed prepared, generate into a new unused attempt:

```powershell
& .tools/emulation/recomp-venv/Scripts/python.exe tools/prepare-title-recomp.py `
  --xbe 'Fuzion Frenzy (USA)/default.xbe' `
  --profile research/verification/original-execution-profile-20261031.json `
  --seed research/verification/recomp-startup-03/seed.json --attempt new-startup
```

Compile `tools/fixtures/local_startup_probe.c` using the generated private directory and the pinned toolkit's `templates/runtime` include directory. The retained build report supplies the exact exports/flags: C11, `-O2 -fwrapv -ffp-contract=off -fno-strict-aliasing`, standalone no-entry Wasm, no filesystem, 128 MiB fixed memory and no unresolved symbols. Only unused generated labels/locals are exempted from `-Wall -Wextra -Werror`. Save compiled title output exclusively under a new ignored research directory. Never instantiate it under Node or run original code as a host program.

Run `node tools/serve-startup-proof.mjs 4180` and select the private module and original XBE through its two local file controls. The HTTP server never serves those files. A fresh worker is limited to 15 seconds; Clear, error, timeout and completion terminate it. This is a developer proof, not a user-facing game launcher or a shippable conversion flow.

Five new source-owned tests cover bounded memory, import/start rejection, malformed module limits and the HTTP content boundary. Together with the existing delivery test, `npm run test:runtime` has six cases and requires no original files/tool downloads. CI also syntax-checks the new Python translator; it does not perform original-code translation/execution. Actual Ghidra export, compiler link and browser results above are separate local evidence.

Local verification passed all six runtime tests, all three Python syntax checks, dependency-free translator CLI help, `npm run check` (116 source files) and `git diff --check`. Reusing the existing title-generation attempt was rejected with exit 2 and its report remained byte-identical. The compiled harness source digest still matches the build report, both pinned source checkouts remain clean (Linux xemu checked with WSL Git), and original/private evidence remains ignored.

Source-only checkpoint `fc1e36af1be508fbc8fff77251e2bac6af3817f3` is published. [CI run 41310972664](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41310972664) completed successfully on Windows and Linux. Both jobs passed the source guard, formats, all three Python syntax checks, runtime-boundary tests, preserved prototype tests/build, environment and clean-diff checks; Linux also passed the existing native triangle test. CI executes no original game or private title toolchain. This documentation follow-up records that terminal result without changing the checked implementation.

## Inferences, unknowns and next task

Inference: the title's first startup path can be mechanically translated, mapped and executed through this isolated Wasm ABI. This is stronger than the earlier owned arithmetic fixture but does not validate every instruction, kernel contract or original behavior.

Unknowns remain: child-thread TLS/FS and scheduling, complete function/control-flow recovery, x87/SSE/flags/exceptions, memory aliases/devices, kernel/HAL services, graphics/audio/video, local files/saves, physical controllers, browser-local title conversion, release-browser performance and complete title fidelity. The full online/social/mixed-seat/recovery/server-authority requirements remain in the [product ledger](ORIGINAL_EXECUTION_PROGRESS.md); original-containing code/RAM/assets cannot move to the server as a shortcut.

Public-contract follow-up: nxdk at `f8c52e4be65bab7d6e10455db8d86d69d232bbe3` declares the ten-argument creation call and a distinct two-argument system routine receiving the game's start routine and context. Its [kernel declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L2466) and [thread startup](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/winapi/thread.c#L33) corroborate the distinction and TLS self-reference/alignment work. Inference: the original ten-word frame is consistent with that public API. This does not establish the shipped kernel's scheduling or exact TLS/FS layout, nor authorize replacing original startup with nxdk code. The toolkit's Windows convenience stub is not a browser implementation of this contract.

Next concrete task: establish and implement the actual `PsCreateSystemThreadEx` call contract, thread/TLS context and scheduling boundary before allowing original child startup to continue. Inspect only the immediate system/start routines already identified at `0x00034307` and `0x00030f10` in the existing project as needed. Verify the public contract and calling convention; do not use the toolkit's permissive success stub as semantics. Add meaningful source-owned thread/context tests, then privately translate the necessary bounded original functions and require a further observable browser result or an explicit next unsupported operation. Report material fidelity/architecture conflicts before affected implementation. No automatic route change follows a failed probe.
