# Original metadata-file creation and query

November 1, 2026. Static recompilation to WebAssembly remains the selected route for the complete original game and all planned custom features. Original browser startup now creates an owned regular metadata file, queries its size/attributes/times, and reaches the next untranslated original helper. No file contents, playable screen, original saves, persistence or complete runtime/fidelity acceptance is established.

## Observed original execution

Both explicit fresh synthetic settings/volume/directory/file profiles pass **69 browser checks and 8,410 owned copy/ABI cases each**. The original-function translation is unchanged: eleven seeds, **34 functions / 2,491 instructions**, and **100 trace events / 31 distinct original routines** observed. Runtime services advance execution without another translation or extraction. **98 kernel calls complete**, including repeated string comparisons; this is not 98 distinct implemented exports.

After the retained `T:` link and first directory-helper return, actual ordinal 190 creates:

| Field | Actual request/result |
| --- | --- |
| Name | `\Device\Harddisk0\partition1\UDATA\4d530002\TitleMeta.xbx` |
| Counted name | 57 bytes, maximum 58; absolute path, root handle 0 |
| Access / object attributes | `0x40100000` / `0x40` |
| Allocation-size pointer | Null |
| File attributes / share / disposition / options | `4` / `1` / `3` / `0x22` |
| Result | Success, I/O status 0, `FILE_CREATED` / 2; real guest handle `65560` |

The owned file is a distinct node under UDATA title directory 5. It has file ID 1, attributes 4, zero length/allocation and bounded empty backing. The regular-file handle has directory ID 0 and file ID 1; it is separate from the still-open UDATA directory handle. Volume counters are seven opens / five closes / two live handles, with five directory nodes and one regular file. The earlier TDATA link's referenced open instance remains a directory object with one reference and no handle. File-node, directory-node, open-instance and handle identities are separate owned tokens, not matching Xbox memory addresses.

The next actual request is **ordinal 211, `NtQueryInformationFile`**, with five arguments: the new file handle, the original I/O-status block, output `62983844`, length **56**, class **34** (`FileNetworkOpenInformation`). It returns success and information 56. The output contains the owned creation/access/write/change times, zero allocation/end-of-file, attributes 4 and zero padding. Each timestamp equals the sample supplied at creation by the existing bounded browser clock; the value lies between independently observed before/after samples. No new host import is introduced. This establishes coherent fresh-file metadata, not original-console time precision, FATX timestamp granularity or matching save state.

Original startup then reaches untranslated **`0x001df4f0`**, return **`0x00033b6e`**, and stops explicitly with state 2 / ordinal 0 before entering that function. The second `0x000338de` call and its caller `0x00033c12` remain pending. No payload write request or file contents have yet been observed or implemented; do not infer them from the filename or arguments to the next helper.

## Request lifetime and evidence correction

Original code changes the name buffer after the create call while its counted descriptor still identifies the same 57-byte buffer. Reading that buffer later as though it were the original request caused the first final-verifier attempt to report an embedded NUL. This was an observer error, not a failed original create or metadata query.

The final probe captures successful regular-file request names and descriptor fields at the create boundary, after checked admission and before returning to original code. Captures are bounded by the existing 128-call capacity and 256-byte path limit and cleared on reset. The browser verifies the captured original path, its retained file identity and the later buffer change. The 56-byte metadata result is also captured at its actual return rather than reread from reused stack storage. Capturing observations does not substitute arguments or results in original execution.

## Implemented contracts and explicit limits

`local_files.c` independently implements the admitted regular-file create/open/open-if and class-34 query. It uses the same pure guest-tree lookup as directories. Relative names require a real directory handle; intermediate missing paths, file-as-directory traversal, directory/file collisions and final-name existence remain distinct. A file cannot be created over an existing directory or treated as one. Creation admits all output/name descriptors, permissions, modes and capacity before adding nodes/handles. Unsupported inputs stop before mutation; admitted filesystem failures update the I/O-status block without creating a partial file.

Generic read/write rights are mapped for symmetric read/write sharing; handles retain the requested access. Synchronous options require synchronized access. The current regular-file profile admits read/write/append/attribute/control/synchronize rights, share bits 0–7, options within `0x62`, dispositions 1–3 and attributes 0, `0x80`, or combinations of `0x2`, `0x4` and `0x20`. Allocation hints, execute/delete rights, overwrite/supersede, unbuffered/alertable modes, deletion, ACL/concurrency/cleanup equivalence and general filesystem behavior remain unsupported or unaccepted. Accepted flags do not imply an implemented read/write operation. Original FATX attribute initialization, allocation/timestamp details and error ordering remain unverified.

There are **16 regular-file nodes**, each with **64 KiB maximum in-memory backing**, alongside the existing 32 directories, 16 live handles and 32 retained open-instance slots. New files are empty; open/open-if preserves an existing node's bytes, attributes and timestamps. Closing handles preserves nodes; referenced open instances retain their typed file identity. These are developer resource bounds, not original FATX capacity or a production-save design. Payload writes, allocation accounting, disc reads, persistent saves and eviction/recovery remain unimplemented.

The class-34 query serializes actual owned fields in little-endian order, writes exactly 56 bytes, rejects unsupported classes/invalid handles, checks output permissions and overlap, and handles short buffers without partial file mutation. Creation receives an explicit checked system-time value; it does not invent an original timestamp. The pinned public [nine-argument create ABI](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L3133), [query ABI](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L2879), [metadata layout](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L609) and [rights/options/constants](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L3169) support interpretation, not matching-original-kernel runtime fidelity.

ABI 10 requires explicit `volume.fileState: "fresh"` after volume/directory admission. `settings-files-0/1.json` retain the preceding synthetic settings/capacities and add this declaration. Storage limits are checked against module exports before original execution. The sole bounded clock import, checked guest memory, fixed unshared 128 MiB Wasm / 64 MiB guest backing, 128-event/call bounds, byte/decoder/layout checks and aggregate translation/metadata budgets are unchanged. No native filesystem, network file provider, upload, firmware acquisition or game-derived public module is introduced. Original execution remains browser-worker only; native/Node fixtures contain owned code.

## Evidence and validation

All artifacts below stay ignored under `research/verification/`. Original files, translated title code/modules, raw analysis/RAM/traces, screenshots and assets are neither served nor published.

| Artifact | SHA-256 |
| --- | --- |
| Unchanged `recomp-title-file-links-04/generation.json` | `af0ea155eb2bd1b538e9ebee8d51292681eb49c530fca5a14aab95fd32a3e375` |
| `recomp-regular-files-build-04/build.json` | `967aecf3098f1a4602dab552a57c22ae23977f11c0d155a7bd180668b32cc769` |
| Matching `startup.wasm` (259,465 bytes) | `1af9935e6fdd10432d6b9f7e50a8bb495d2f2356e220ac2fd108205f6909d366` |
| `regular-files-owned-02/checks.json` | `820c6d44f05e7ad7cdb9981fe30c342971dc849873acc80ff12309fe05348226` |
| `regular-files-directories-02/checks.json` | `0b1366d560f086bda2c07ddf362ed6e86a1cf51c24d22068b03edd4c94b19454` |
| `regular-files-links-02/checks.json` | `a796e5e50b4a6cbe24f8405902869870c15df0edc948101e89a84e5d7edaaf18` |
| `regular-files-volume-02/checks.json` | `9da32421790cdf11c298ba671a4a4290cb08796ff69027d574684af77de58416` |
| `regular-files-browser-20261101-05.json` (synthetic 0) | `63639f44fa4198c1d3602b449a7d1454812aae85c4319c1cba9d33630296d3b2` |
| Matching `06.json` (synthetic 1) | `97b41802e5e3061492d9a8196b5725ca6142c4bbb8d52100ee2577bb9bccf1a4` |
| Matching `22.json` (recovery, empty warning/error log) | `5fc64bf180364655741579057ec865969073685ac0ddde87797928ef32643cd7` |
| Matching `22.jpg` (inspected screenshot) | `ab9cb99e6fa406a44b391cf1b21a9dac360a1f579e4afe5533dd49a04d651a81` |
| `recomp-regular-next-02/seed.json` (static next-helper export) | `ca04874c7969de846457ac5fb996c9fa02c5dce082b5c0d85efca82b42fa0f7f` |

`regular-file-checkpoint-audit-01.json` passes 27 checks: 26 compiled owned source/declaration identities, fixture identities 8/9/11/6, all eleven unchanged seeds/generated output, exact build/browser identity, file/directory/reference/handle ownership, captured request lifetime, metadata/clock/next-stop evidence, regressions/rejections/Clear/logs, unchanged supplied/reference XBE and clean vendor/header pin. Four fixture suites pass all twelve native optimized, address/undefined-behavior sanitizer and Wasm compile/run combinations. They cover generic-right sharing, independent files, creation/open results, close/reference/reopen lifetime, preserved bytes/times, relative/type/path handling, capacity/failure atomicity, read-only names and checked metadata output. Twenty-two runtime/admission/delivery cases and nineteen Python cases pass locally. Source checkpoint `3bb69ef` is published; [CI run 41356608921](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41356608921) passed Windows and Linux, including the new Linux create/query fixture, runtime/Python/format checks, publication guards and preserved prototype tests/build. CI executes no original game or private title toolchain. The preparation/publication guard passes for 184 source files and whitespace checks pass.

Private builds/browser `01/02` retain the actual create and subsequent query stops under the earlier verifier. Browser `03` preserves the post-use name-buffer diagnostic error; `04` preserves the passing 68-check run before explicit buffer-reuse verification. Final `05/06/22` pass 69 checks. `07..16` preserve the passing 61/50/43/40/36/31/27/22/20/16-check modules; `17..20` reject missing file/directory/volume state and malformed Wasm; `21/23` verify Clear. Recovery logs contain no warnings/errors. The temporary tab and verified source-only host were cleared/closed. Earlier owned create-only and intermediate declaration fixtures remain preserved.

## Next concrete task

Reuse the existing Ghidra project and current eleven seeds/translation. Read-only export `recomp-regular-next-01` contains only reached `0x001df4f0` (112 contiguous bytes / 46 instructions). `next-02` retains it and its two immediate direct dependencies: `0x001e2c35` (1,900 bytes / 576 instructions) and `0x001e228c` (281 bytes / 105 instructions). This is three functions / 727 static instructions, not admitted translation or observed helper-body execution. No persistent project changes, repeated import/extraction or broad decompilation occurred; direct Ghidra output is not REA Evidence.

Next admit this bounded export with the existing original-byte, independent decoder, complete-layout and checked-memory requirements; compose it with the eleven retained seeds, then run original browser startup and follow only genuinely reached missing dependencies. Do not replace the helper with guessed metadata, infer a successful payload write or execute original code natively/under Node. Complete runtime, original parity, browser-local end-user conversion, graphics/audio/input, saves, all online/social/control features and authority remain unaccepted. Preserve the complete objective and selected recompilation route.
