# Original metadata write and handle closure

November 1, 2026. Static recompilation to WebAssembly remains the selected route for the complete original game and every planned custom feature. Original browser startup now completes its actual metadata-file write and closes that file's handle. The exact bytes remain in the owned RAM volume. This is further CPU startup, not persistent saves, a playable screen or complete runtime/fidelity acceptance.

## Observed original execution

Both explicit fresh synthetic profiles pass **78 browser checks and 8,410 owned copy/ABI cases each**. The original translation is unchanged: thirteen seeds, **46 functions / 3,514 instructions**, with **238 trace events / 37 distinct original routines** observed. **100 kernel operations complete**, including the new write and close; this is not 100 different implemented services.

Actual ordinal **236**, `NtWriteFile`, receives the same eight arguments established in the [prior checkpoint](METADATA_FORMAT_STARTUP_CHECKPOINT.md): handle `65560`, null event/APC/offset, I/O status `62983920`, buffer `62983300` and length **52**. It returns success with I/O status 0 / information 52. The synchronous handle's position advances from 0 to 52 and the real file node's end-of-file becomes 52. Its retained bytes match all five captured formatter arguments and the original certificate-title input, including the UTF-16LE byte-order marker and excluding the terminating zero.

The selected geometry is 32 sectors of 512 bytes per allocation unit. The owned file charges **one 16,384-byte unit**. Available units become 65,535 in synthetic profile 0 and 98,303 in profile 1. Creation/access times remain unchanged; owned write/change times equal the existing browser-clock sample recorded at the actual write. These are coherent synthetic-volume values, not measured original FATX allocation or timestamp behavior.

Original code then successfully calls ordinal 187 to close `65560`. The file node, its bytes and allocation survive closure. Volume counters are seven opens / six closes / one live handle: the UDATA title-directory handle `65556`. The older TDATA file-object reference and `T:` link remain retained. This is retention within the worker lifetime; Clear or worker termination discards this RAM state.

Startup next stops explicitly before entering untranslated **`0x000337f7`**, return **`0x00033bc4`**, with state 2 / ordinal 0. The second `0x000338de` invocation and `0x00033c12` caller remain pending. No later link, file operation or helper return is inferred. Disc readiness remains false; no original package-file reads or playable screen are established.

## Implemented contract and fidelity limits

`ff_volume_write_file` independently implements checked, synchronous, buffered writes to typed regular-file nodes. It validates live handle/access, payload and status ranges, optional signed offset, overlap, modes and capacity before modifying backing. Ordinary sequential writes, explicit seek-and-write, end/current-position sentinels, append-only access, overwrite without truncation, zero-filled gaps and independent files have owned fixtures. Null writes preserve backing, extent, position, allocation and timestamps. Nonzero event/APC arguments and asynchronous/unbuffered modes remain unsupported rather than receiving simulated completion.

Allocation growth consumes the selected volume's available units exactly once. An admitted full-volume failure reports disk-full with zero bytes and no partial backing/position/time change. Exceeding the developer's 64 KiB per-file bound remains an explicit unsupported contract; it is not misreported as a physically full original disk. Access denial, invalid handles, checked-memory failures, overlapping outputs and offset overflow are covered. Invalid handles do not dereference or alter the output block. Exact original-kernel failure ordering remains unaccepted.

The [pinned nxdk header](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) supports the Xbox eight-argument ABI. [Microsoft's NT write documentation](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntwritefile) informs position, append and gap semantics; its nine-argument signature is not substituted for Xbox's ABI. Those public contracts support implementation choices, not observed original-console execution.

The actual handle includes write-through. Here the backing medium is the explicitly selected volatile RAM volume: completion copies real bytes to that backing immediately, with no deferred fake write. **Durable browser storage, original saves, FATX layout/granularity, timestamp update/close behavior and original error equivalence remain separate fidelity gates.** The explicit owned write/change-time policy uses the existing clock; it does not claim to reproduce FATX. No host filesystem, original-file modification, network provider, upload or firmware acquisition is introduced.

ABI **12** connects this service and records the actual call/result before original stack reuse. The verifier separately compares pre-write query captures with creation state, later backing bytes with the original formatted payload, and final capacity with actual allocation charges. It does not reinterpret reused status storage or require a closed handle to remain live. Earlier ABI modules retain their original boundaries. The sole bounded clock import, fixed unshared 128 MiB Wasm / 64 MiB guest memory, 16 files / 64 KiB backing each, 1,024-event trace, 128-call limit, byte/decoder/layout/memory guards and aggregate translation/metadata budgets remain.

## Evidence and validation

All artifacts remain ignored under `research/verification/`. Original bytes, payload, title-derived modules/code, raw exports, traces and screenshots are neither HTTP-served nor committed/uploaded.

| Artifact | SHA-256 |
| --- | --- |
| Unchanged `recomp-title-metadata-03/generation.json` | `64de85fa5285f9fe8391310a8cd72ffe15a4126a61c08d4eacbb45f0b24eb28e` |
| `recomp-file-write-build-02/build.json` | `8df458dd8f90476f3c15278147ae610d4e74f80632aac851052950a913d55d09` |
| Matching `startup.wasm` (337,279 bytes) | `b5c2f9381d8c3671ea2eb86a1dd92f8c6f99cf91c990fbf1a4f0f22b378ed87a` |
| `file-write-owned-02/checks.json` | `4fdd6517501b87d7a73ffa7efd8d2a0e4fa43d6f089bb3506c04868b0be75bd9` |
| `file-write-browser-20261101-23.json` (synthetic 0) | `058c9312f9eee43948314228ef9fa82c9aed761f3bb8fdda75f8fa53a7353677` |
| Matching `24.json` (synthetic 1) | `6e7b97ea0d42cf7523a57456d6ce23f24d51f4248f50467af690741da8059f0e` |
| Matching `30.json` (recovery) | `3b3062db966b386683203656fe6222df171b99664ab1f1e91ca36f94a306fe3e` |
| Matching `30.jpg` (inspected screenshot) | `56d66b6e566f6f2bc61c30c48ccf522118ed628da7a7c682b88c95cf5ed1fd77` |
| `recomp-file-write-next-02/seed.json` (static next-helper export) | `13277a41130bacc1ea612b458fc794a8223a594db08c424187c751db6401cea5` |
| `file-write-checkpoint-audit-01.json` | `56423ab3acc8e950de41acd8678a76f494e3441799327001b7fa67c8118cb401` |

The **26-check audit passes**, matching all 26 compiled source/declaration identities, fixture identities 8/9/11/6, all thirteen unchanged seeds/generated output and exact browser/module identity. It verifies actual write/close/backing/allocation, pre-write versus final metadata, certificate title, clock bounds, next stop, regressions/rejections/Clear/logs, unchanged supplied/reference executable and clean vendor/header pin. The four owned service suites pass all twelve native optimized, address/undefined-behavior sanitizer and Wasm modes. Twenty-eight runtime/admission/delivery cases and nineteen Python cases pass locally. The preparation/publication guard passes for 186 source files and whitespace checks pass; CI results are recorded separately in status/handoff; CI never executes the original game or private toolchain.

Browser `01` preserves the real write/close and next stop under the earlier verifier; `02/03/21` pass the initial module's 78 checks. `04..15` pass the earlier 74/69/61/50/43/40/36/31/27/22/20/16-check modules. `16..20/22` retain initial rejection/Clear coverage. Final module `02` adds guarded failed-handle observation; owned fixture `02` covers invalid output with a closed handle. Final browser `23/24/30` passes 78 checks, `25..28` rejects missing state/malformed Wasm and `29/31` verifies Clear. Recovery logs contain no warnings/errors. The two screenshots are byte-identical views of the same successful boundary. The temporary tabs and verified source-only host were cleared/closed. All original inputs and earlier evidence are preserved.

## Next concrete task

The existing Ghidra project was reused read-only to export only the actually reached helper and immediate dependencies. `recomp-file-write-next-01/seed.json` contains `0x000337f7` (231 contiguous bytes / 94 instructions). `next-02` retains it plus `0x000345fb` (33 / 13), `0x0003461c` (30 / 11) and `0x00034670` (10 / 3): **four functions / 121 static instructions**. This is not admitted translation or observed execution of those bodies. No persistent project change, repeated import/extraction or broad decompilation occurred; direct Ghidra output is not REA Evidence.

Next admit and translate this bounded export with the thirteen retained seeds, preserving original-byte identity, independent decoding, complete layouts, checked memory and aggregate limits. Reuse `recomp-title-metadata-03` and the existing project. Then run original browser startup and follow only genuinely reached missing services/dependencies, including any further helper needed by an actual path. Do not guess the next file operation from a static call or bypass it with a success stub. Original persistent saves, complete runtime/rendering/audio/controllers, browser-local end-user conversion, full catalog/mode/detail parity, every online/social/control feature and authority remain unfinished. Preserve the full objective and selected recompilation route.
