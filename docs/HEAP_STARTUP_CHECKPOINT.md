# Original heap return and first file request

November 1, 2026. This advances the selected static-recompilation route; it does not accept a playable game or any full-product feature gate. Reuse the supplied folder, existing Ghidra project and saved exports. No disc extraction, persistent-project import, firmware acquisition or native original-game execution occurred.

## Established static and build evidence

Eight retained seeds compose **23 functions / 1,364 instructions**, preserving original-byte identity, independent decoder agreement, complete envelopes, mixed code/data layout and checked scalar/stack/bulk accesses. Direct Ghidra exports are static evidence, not REA Evidence or observed execution. The eighth seed, `research/verification/recomp-heap-children-03/seed.json`, contains only these dependencies:

| Original entry | Complete bytes | Instructions |
| --- | --- | --- |
| `0x00031da0` | 286 | 107 |
| `0x0003139a` | 147 | 63 |
| `0x0003185d` | 250 | 87 |
| `0x00031379` | 33 | 10 |
| `0x00031259` | 288 | 111 |
| `0x001e0505` | 17 | 9 |

The final private module is 149,904 bytes, retains ABI 5, fixed unshared 128 MiB Wasm memory / 64 MiB guest RAM and exactly one bounded clock import. An optional `ff_probe_trace_cpu(index, field)` diagnostic records EAX, FS and the exception-chain head alongside existing entry/exit events. It changes no original routine or mandatory earlier ABI. The newer heap-return checks require those observations; an earlier module reaching this stop without them correctly fails two checks.

## Observed browser execution

Both explicit synthetic settings profiles pass **31 checks and 8,410 owned copy/ABI cases each**. Thirty-seven trace events observe twenty distinct original routines; translation coverage is larger than execution coverage. In particular, translated `0x00031379` is not observed on this path.

The original helper chain and original exception epilogue return. Initializer `0x000322ef` returns EAX `0x00330000`, matching its actual reservation base, with its entry stack pointer, FS and preceding exception-chain head restored. The inner exception frame was installed and removed by original routines. This establishes normal-return balance on the observed path, not recoverable exception dispatch or every heap operation.

The previous twelve completed kernel operations remain: thread creation/close, notification locking, DPC/timer initialization, explicit settings/status conversion, real timer arming, reservation/commitment and heap-lock initialization. The 1 MiB reservation still has only its first 4 KiB page committed. No boot memory fault occurred; separate reserved-page-read and read-only-image-write diagnostics still trap. The six-hour timer remains pending with no callback delivered.

Original routine `0x00033794` then reaches **unsupported ordinal 202, `NtOpenFile`**, return `0x000337ca`. Its synthetic binding is checked against the actual declared slot, not computed from the ordinal. The counted request is:

| Request field | Observed value |
| --- | --- |
| Object name | `\Device\Harddisk0\partition1\` |
| Name length / capacity | 29 / 30 bytes |
| Root directory handle | 0 |
| Object attributes | `0x40` |
| Desired access | `0x00100001` |
| Share access | `0x3` |
| Open options | `0x00800021` |

Only six arguments belong to this syscall; following captured stack words are not extra arguments. The independently written decoder validates the 12-byte Xbox object-attributes structure, 8-byte counted ANSI descriptor, admitted read/write ranges and a 1,024-byte diagnostic name limit. It preserves byte values, rejects embedded NUL and performs no path resolution, host file access, guest writes or service completion. Public declarations corroborate layout/signatures, not matching-console runtime behavior: pinned nxdk [kernel declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) and [string/types](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxdef.h).

**Inference:** startup depends on a hard-disk volume namespace, not just the supplied game-file package reader. **Unknown:** matching original disk contents, volume metadata and saved state were not supplied. Do not fabricate them or acquire an HDD/firmware download. An explicitly synthetic fresh-volume developer profile can support independent service testing; it cannot establish original-console save/filesystem fidelity.

## Retained local evidence and validation

All paths below are under ignored `research/verification/`; no title-containing module, export, trace, RAM or screenshot is published.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-heap-children-03/seed.json` | `5198165864198d04d67e2d38e550f550576c4bb7f839d870d2c76f33562b1dcf` |
| `recomp-title-heap-04/generation.json` | `378425157e653f2aa51b00724127500711f775fa6cfc1d821044549fb1f1dac7` |
| `recomp-heap-build-05/build.json` | `bb59086702674cee645f54b5738468f2927b7341f151e5cf332d17575abf1236` |
| `recomp-heap-build-05/startup.wasm` | `c369853a5bc314def97c9be8287ba7599c475a3b31eceaec9e1a4d4b107bbc7e` |
| `heap-startup-browser-20261101-05.json` (synthetic 0) | `db144c277c94238ab234e5b4771a98fe1f971c16015fff88860cc9ed594d2941` |
| Matching `06.json` (synthetic 1) | `3de9eb189aecd4a28e213529b3e0c75ddd965cb295350223a1e6b583767f637f` |
| Matching `14.json` (recovery, empty warning/error log) | `d29325f82447fc23a2022eb539ceb04b9c22acbf1cc9c1eecb0c96757f76cf9a` |
| Matching `14.jpg` (inspected screenshot) | `2475e74f704c94316f0f659d96efe862164fee61764db873ace622e2d1decdf7` |

Earlier `01..04.json` retain successive original helper/epilogue stops and the obsolete verifier boundary, without relabeling those runs as passes. Final `07..10.json` preserve the passing twenty-seven-, twenty-two-, twenty- and sixteen-check modules through the updated worker. `11.json` rejects missing CPU-return observations; `12.json` rejects a malformed owned module; `13.json` rejects missing explicit settings; `14.json` verifies recovery. `15.json` records empty file selections/details and no warnings/errors after Clear. The temporary tab and verified source-only server were closed.

`heap-checkpoint-audit-01.json` verifies all sixteen compiled owned identities, both generated-output identities, all eight seed identities, browser/module identity, unchanged original executable and clean vendor source at `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`. Fifteen owned runtime/admission/delivery cases pass, including three new malformed/permission/counted-name cases. Fourteen seed tests and three memory-emitter tests pass. These owned checks run without original files; actual original execution occurs only in the browser worker. Publication/whitespace and CI results are recorded in status/handoff separately.

## Next concrete task and acceptance

The retained `recomp-crt-02/seed.json` export of `0x00033794` statically declares three successive call sites: `0x000337c4` through slot `0x002533d0` (202, open), `0x000337df` through `0x002533cc` (218, `NtQueryVolumeInformationFile`) and `0x000337ea` through `0x00253378` (187, close). Local `heap-next-dependencies-01.json` preserves source/translation identities and this metadata without raw code. Only the first call has been observed; the later query's actual class, buffer length, results and close behavior remain unknown. Public declarations linked above establish the query signature, not its execution. Follow actual browser arguments after implementing open rather than guessing volume metadata from those declarations.

Implement and independently test the **actual volume-root/open-handle contract** requested by ordinal 202, using an explicit local synthetic fresh-volume profile while original disk state remains unknown. Establish namespace resolution, bounded directory/volume handles, requested access/share/options validation, status/output behavior, close/lifetime behavior and failure atomicity before connecting it to original startup. Then run both explicit browser profiles and follow the next genuine unsupported dependency. Do not return success with an invented handle or redirect the device name to arbitrary host paths.

Keep game-file reads tied to explicitly selected compatible local content; persistent save/storage must stay browser-local with clear/reselect/quota/failure behavior. General filesystem semantics, full Xbox VA/physical memory, fault recovery, original settings/clock/shutdown, rendering/audio/input, browser-local end-user recompilation, complete original parity and every planned online/social/control feature remain unaccepted. The complete objective stays active.
