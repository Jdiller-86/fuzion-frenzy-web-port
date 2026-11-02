# Original wide metadata formatting and pending write

November 1, 2026. Static recompilation to WebAssembly remains the selected route for the complete original game and every planned custom feature. Original browser startup now returns from the previously missing metadata formatter and requests its actual 52-byte file write. That write stops before a guest result; the owned file remains empty. This is further original CPU startup, not a playable screen, save implementation or full runtime/fidelity acceptance.

## Observed original execution

Both explicit fresh synthetic profiles pass **74 browser checks and 8,410 owned copy/ABI cases each**. Thirteen retained exports translate **46 functions / 3,514 instructions**. The browser observes **238 trace events / 37 distinct original routines**, including six newly entered routines. **98 kernel calls complete** before the unsupported write; this is not 98 different implemented services.

The original `0x001df4f0` call receives a destination, a **142-code-unit** limit, a UTF-16 format and five variadic arguments: two wide characters and three wide strings. It returns **26 UTF-16 code units**. Captured input strings and output retain their actual encoding and termination. The independent comparator preserves the leading byte-order marker, produces the exact **52-byte little-endian payload**, and compares that with the actual write buffer without its terminating zero. A separate local audit verifies that the title argument points to and matches the title in the supplied XBE certificate. The metadata text and original strings remain in ignored local evidence.

| Newly observed routine | Entry / return pairs | Observed boundary |
| --- | --- | --- |
| `0x001df4f0` | 1 | Original outer formatter returns 26 |
| `0x001e2c35` | 1 | Formatting core returns |
| `0x001e2bd3` | 5 | Original dependency returns for each actual argument |
| `0x001e2bae` | 26 | Original dependency returns for each output code unit |
| `0x001e2bf8` | 10 | Original dependency returns |
| `0x001e6ee5` | 26 | Original output dependency returns |

Each recorded pair restores its entry stack, FS base and exception-chain head. This does not prove all formatting-library inputs, locale behavior, error paths or complete register/floating-point fidelity. Six other newly admitted functions are static immediate dependencies, not newly observed execution.

Startup next stops with state 1, **ordinal 236, `NtWriteFile`**, return **`0x00033b8c`**. Its eight actual arguments are:

| Argument | Observed value |
| --- | --- |
| File handle | `65560`, the owned regular metadata-file handle |
| Event / APC routine / APC context | All zero |
| I/O-status block | `62983920`, reused after the successful metadata query |
| Buffer / length | `62983300` / **52 bytes** |
| Byte-offset pointer | Null |

The [pinned Xbox-specific declaration](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) supports this eight-argument interpretation; it is not observed original-kernel behavior. The diagnostic validates writable status storage and readable payload/optional signed offset, copies at most 64 KiB for observation, and returns no guest success or host file access.

No write is recorded as completed. The prior I/O-status contents remain success / information 56 from the query. The owned regular node still has zero length/allocation, its handle position is zero, and both the UDATA directory and regular-file handles remain live. The second `0x000338de` invocation and `0x00033c12` caller remain pending. The older TDATA file-object reference and `T:` link remain retained. Disc readiness remains false. File writes, original saves, FATX persistence and disc reads are still unimplemented/unaccepted.

## Admission and observation contracts

The existing Ghidra project was reused read-only, with no import, extraction or broad decompilation. `recomp-metadata-next-01/seed.json` retains eight functions / 812 instructions, including the previous three-function export and immediate core dependencies. After actual browser execution reached `0x001e6ee5`, `next-02` exported that function alone and `next-03` retained it with three immediate dependencies: four functions / 211 instructions. The final thirteen-seed composition preserves the first eleven seeds unchanged.

Original-byte identity, independent decoder agreement, complete function envelopes, checked accesses for all 46 functions, 64-function / 8,192-instruction limits, sixteen-export grouping and the 64 MiB aggregate metadata bound remain enforced. Generated C and Wasm stay private and ignored. Developer compilation does not implement the required browser-local end-user converter. Original code executes only in the browser worker.

An actual 128-event trace-cap stop is retained in browser `03`. ABI **11** adds bounded read-only metadata-call snapshots and an explicit **1,024-event** trace; older 64/128-event modules still pass. Kernel records remain capped at 128. The sole bounded clock import, fixed unshared 128 MiB Wasm memory, 64 MiB guest backing and explicit fresh synthetic volume/directory/file profiles are unchanged. No new host service, original firmware, HTTP file provider or game-derived public module is introduced.

The observer captures the actual call/return before stack reuse, including five arguments, at most 128 code units per format/input snapshot and 260 for output. The measured call's limit is 142. Its comparison supports only bounded typed `%lc`/`%ls` observations and escaped percent; it is an independent check after execution, never a replacement formatter or source of guest results. Unsupported comparisons stay failed and retain a diagnostic.

## Evidence and validation

All artifacts below remain ignored under `research/verification/`. No original strings, payload, game instructions, raw pseudocode, RAM, translated modules or screenshots are published.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-title-metadata-03/generation.json` | `64de85fa5285f9fe8391310a8cd72ffe15a4126a61c08d4eacbb45f0b24eb28e` |
| `recomp-metadata-next-01/seed.json` | `21b983cbcdfc268ab73fbeec679c7250789d164d450fbba5449599a5a4a046fc` |
| `recomp-metadata-next-03/seed.json` | `b2e28dbfcd3c5ccf8172fdbe8adff63961800ac856462181f893578b44735093` |
| `recomp-metadata-build-07/build.json` | `cd7a48f3078ec7621e2f9e931f3472efc4884021f45ab9c14b3a740f4711fef6` |
| Matching `startup.wasm` (334,842 bytes) | `8fde88b99c5a79131504f142bf6c8d7b7b6a5c14bebedfcbbc991c5d40099025` |
| `metadata-browser-20261101-11.json` (synthetic 0) | `5a5d2c31e93a490e82c2d750dc43898202ca4950a9e03c4621d013b4011c165c` |
| Matching `12.json` (synthetic 1) | `ade0e887de60f6d4452a7a2e095d085558dcec337081c212f678310a9ad0409a` |
| Matching `29.json` (recovery) | `d4d645bc4c2dde00eb8e690bbd0d82a767777a23c3f9d9535385540822ce8125` |
| Matching `29.jpg` (inspected screenshot) | `9ffe21eb916779f2e44a0aa4302b4c8bf3d5ebf17788262957793dfa3bb8fc5b` |
| `metadata-format-checkpoint-audit-02.json` | `b71aee13b17a4034206ce601131e8977b1ecb9e11cd6f605e25810ba25b5851c` |

The **30-check audit passes**: all 26 compiled owned source/declaration identities, thirteen seeds/generated output, exact browser/module identity, UTF-16 payload and original certificate-title identity, six balanced original helper paths, the pending write without mutation, retained storage metadata/clock limits, regressions/rejections/Clear/logs, unchanged supplied/reference executable and clean vendor/header pin. Four existing owned service suites retain matching identities 8/9/11/6 and twelve passing native/sanitizer/Wasm modes; those service sources did not change. Twenty-eight runtime/admission/delivery cases and nineteen Python cases pass locally. The preparation/publication guard passes for 185 source files and whitespace checks pass. CI never executes the original title or private toolchain.

Metadata-format source checkpoint `1cf4946` is published. [CI run 41377675137](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41377675137) passed Windows and Linux, including 28 runtime/admission/delivery cases, nineteen Python cases, formats, source-publication guards and the preserved prototype tests/build. Linux also passed owned regular-file create/query and prior file-target, directory/string, volume, namespace, memory, object/context, timer and triangle checks. CI executes no original game or private title toolchain; the 74-check original browser result remains separate local evidence. The preparation/publication guard passes for 185 source files and whitespace checks pass. This documentation follow-up changes no checked runtime source. The full objective stays active and unfinished; next implement the actual regular-file write using real bounded backing and checked results.

Earlier artifacts are preserved. Browser `01/02` retain newly reached untranslated dependencies, `03` the trace cap, and `04` the first actual write stop. `05` retains the incorrect narrow-string observer, `06/07` the insufficient argument comparator, and `08` the 73-check result. `09/10` retain a checker naming error corrected before the final 74-check runs. `13..23` pass the earlier 69/61/50/43/40/36/31/27/22/20/16-check modules; `24..27` reject missing file/directory/volume state and malformed Wasm; `28/30` verify Clear. The recovery warning/error log is empty. Audit `01` used Windows' default text decoding and misread the saved UTF-8 marker; `02` explicitly reads UTF-8 and passes without altering browser evidence. The temporary tab and verified source-only host were cleared/closed.

## Next concrete task

Implement and independently verify the actual synchronous regular-file write requested above: typed live handle/access, checked payload/status ranges, real bounded backing bytes, file position/size, coherent owned allocation/timestamp state and failure atomicity. Preserve the observed eight-argument ABI and null event/APC/offset request. Only then return its actual result to original code and observe the next genuine operation; do not infer a later close, link or caller return. Reuse the thirteen seeds and `recomp-title-metadata-03`; no further Ghidra export/import or disc extraction is needed for this reached runtime service.

Original save-state compatibility, persistent storage, general filesystem behavior and matching-console timing remain separate fidelity gates. Complete original execution/rendering/audio/controllers, catalog/mode/detail parity, browser-local conversion, all online/social/control features and server authority remain unfinished. Preserve the full objective, selected recompilation route and local-content boundary.
