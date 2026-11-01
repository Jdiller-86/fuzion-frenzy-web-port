# Original drive link and next startup helper

November 1, 2026. This advances the selected static-recompilation route toward the complete original-game/custom-feature goal. It does not accept a playable screen, mounted disc/file reads, full object-manager behavior or fidelity. Original execution remains browser-worker-only; native/Node fixture runs contain independently authored code only.

## Implemented contract and evidence boundary

`object_namespace.c` implements bounded guest directory/device/link objects with owned identifiers and references. The original `IoCreateSymbolicLink` request resolves an existing device object before inserting a permanent named link; duplicate names fail without leaking the target reference. Case-insensitive resolution reaches the held device object, and deletion releases the link's reference. Guest counted strings are read only after length/range/permission admission; unsupported inputs do not mutate guest RAM or namespace state.

The Xbox-specific comparator mattered: pinned Cxbx [I/O wrapper](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlIo.cpp) uses a permanent case-insensitive object, [creation implementation](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlNt.cpp) retains its resolved target, and [target resolver](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlOb.cpp) looks up an actual object. These are public implementation observations, not a controlled run of the original Xbox kernel. The independently authored component uses its own storage/lookup representation; raw game/kernel pseudocode is not published.

The guest disc device has **no mounted medium or file provider**. Its existence supplies a namespace target, not game-file bytes or successful reads. No host path, HTTP fallback, file upload or native filesystem is introduced. Readiness remains false after link creation. The selected compatible XBE proves CPU/build identity; it does not itself mount the remaining game packages.

The admitted profile has 32 objects, ASCII paths shorter than 256 bytes, components shorter than 64 bytes and links to exact device objects. Directory/file targets, general parse callbacks, guest object layouts/handle APIs, full concurrency and further device kinds remain unsupported. These are proof limits, not reduced product requirements. ABI 7 adds namespace admission/diagnostics while retaining ABI 6's explicit synthetic volume profile, checked memory and exactly one clock import. Earlier ABIs remain compatible.

## Observed original execution

Both explicit settings/volume profiles pass **40 browser checks and 8,410 owned copy/ABI cases each**. Sixteen kernel operations complete. The new ordinal-67 operation returns success only after creating the actual owned link and holding its device reference:

| Observation | Result |
| --- | --- |
| Directories | Root, `\??` and `\Device` |
| Disc device | `CdRom0`, local-disc kind, references 2, mounted-content readiness 0 |
| Permanent drive link | `D:` under `\??`, references 1, targeting that device and retaining `\Device\CdRom0` |
| Namespace population | Five distinct owned objects |
| Case-insensitive `\??\d:\` resolution | Success, same device, no remaining filename |

Original startup then enters `0x00033c12`. Its complete 85-byte / 29-instruction envelope is mechanically translated, preserving byte identity, independent decoder agreement, layout and memory guards. The final bundle is **24 functions / 1,393 instructions**; 39 trace events observe 21 distinct original routines. The previous heap/volume returns, stack/FS/exception balance, pending six-hour timer, reservation/commitment, protection-denial and local-content checks remain intact.

The helper **has not returned**. It stops at untranslated `0x001dee6e`, return `0x00033c29`. No algorithm name, format semantics or successful callee execution is inferred from that stop. Full original game behavior and every product gate remain unaccepted.

## Retained local evidence and checks

All artifacts below remain ignored under `research/verification/`. No title-containing module/export, original instructions, RAM, raw traces, screenshots or Ghidra output is served, uploaded or committed.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-namespace-next-01/seed.json` (seven-function eighth seed) | `d56a11bba319e38afb38a1239b2bad2102619b99caf28fe4df857f8d9f51d142` |
| `recomp-title-namespace-01/generation.json` | `d445d45d4a78d0f2d8ee30e99e317547566ef21c1d817c0a3ce43b9691382610` |
| `recomp-namespace-build-02/build.json` | `928a4b96f4a2ef34001e15ccf61e65a2005f9f4cc90eb2c444c2b7cb03e5ceb7` |
| `recomp-namespace-build-02/startup.wasm` (162,309 bytes) | `688fc0579aed4b96973b0cdd691e65512591796f9eaafff3da835617eca23b24` |
| `object-namespace-03/checks.json` | `03a5282000aa73582e035650591099cbdcfd1a349267fdcb806f7561ee3c90bb` |
| `namespace-startup-browser-20261101-03.json` (synthetic 0) | `037e4fac11c551d6fa75d63ead6e221f80ccbdb270499c5abdbc3f8d6d68973c` |
| Matching `04.json` (synthetic 1) | `f6163649ea4e3d4a8b48ecafe5a7bedb1aa6dcc9bd00bb7580454d762a299914` |
| Matching `14.json` (recovery, empty warning/error log) | `b0fd7e924f5c09460c5c4e5e20402ca1eaa8edd566740d708562d60bb9d6ff29` |
| Matching `14.jpg` (inspected screenshot) | `60f9920a75f02a3e651f76492964990fe5953adbbbb617819e9ad60cb5364136` |

`namespace-checkpoint-audit-01.json` verifies twenty compiled owned identities, six fixture identities, both generated outputs, all eight seed identities, exact browser/module identity, unchanged supplied executable and clean vendor source at the existing pin. The final module retains fixed unshared 128 MiB Wasm / 64 MiB guest RAM, which is not a peak-memory or release-performance measurement.

Owned namespace scenarios pass native optimized, address/undefined-behavior sanitizer and Wasm runs, including target/reference lifetime, collision, missing target, counted-string corruption, read-only/unmapped memory, path boundaries and capacity. Earlier `object-namespace-01/02` preserve portable test-literal compiler failures, not original-runtime failures. Seventeen runtime/admission/delivery cases and fourteen seed tests pass locally. CI adds the owned namespace C test on Linux; publication/whitespace/CI results are recorded in status/handoff separately.

Browser `01.json` retains real link creation and the old unsupported-helper/verifier boundary; `02.json` retains the newly entered helper before final verifier update. These are not relabeled passes. Final `05..10.json` preserve the passing 36/31/27/22/20/16-check modules. `11/12.json` reject missing volume settings/malformed Wasm; `13/15.json` verify cleared selections/details; `14.json` verifies recovery. Final warnings/errors are empty; the temporary tab and verified source-only host were cleared and closed.

## Next concrete task

Reuse read-only export `recomp-namespace-next-02/seed.json` as the eighth seed, retaining the seven previous seeds. It contains the seven current dependencies plus established `0x001dee6e`: 88 contiguous bytes / 36 instructions, with direct calls to `0x001e2433` and `0x001e228c` and no computed branches. SHA-256: `8c6cabefae787751ab8d241dcd1b343d924be96aaaa6e48fa06c4f0c1c0efb0f`. This is static Ghidra evidence, not REA Evidence or callee execution. It was exported from the existing project read-only with scoped launcher state; no extraction or new XBE import occurred.

Translate that actual next callee with complete byte/decoder/memory checks, run both explicit browser profiles and follow only genuinely reached dependencies. Do not replace it with an invented library behavior or relax the object-reference/local-content boundary. Mounted game-file reads, general filesystem/save persistence, original disk/settings/clock/shutdown fidelity, full memory/exceptions, rendering/audio/input, browser-local end-user recompilation, every original detail and all planned online/social/control features remain unaccepted. The complete objective stays active.
