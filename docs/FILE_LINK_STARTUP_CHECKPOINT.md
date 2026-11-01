# Original directory-backed drive link and next metadata-file request

November 1, 2026. Static recompilation to WebAssembly remains the selected route for the complete original game and all planned custom features. Original startup now creates its `T:` link with a retained directory file object and returns from its first directory helper. It subsequently requests a regular metadata file. No playable screen, complete filesystem, original save state or full runtime/fidelity acceptance is established.

## Observed original execution

Both explicit fresh synthetic settings/volume/directory profiles pass **61 browser checks and 8,410 owned copy/ABI cases each**. The final module translates **34 functions / 2,491 instructions** from eleven retained seeds. Its **100 trace events observe 31 distinct original routines**; translation coverage is not an executed-instruction count.

After the earlier title-data directory creates/closes, the actual ordinal-67 call creates `\??\T:` targeting `\Device\Harddisk0\partition1\TDATA\4d530002`. The link references an owned namespace file object, which retains the actual directory's open-instance reference after its temporary guest handle closes. At that point volume counters are four opens / four closes / zero live handles. The retained backend object has one reference and no handle. Lowercase `\??\t:\` resolves to the same object with no remaining path. These are actual object identities; the implementation does not substitute a hardcoded `T:` path string.

Original helper `0x000338de` then returns EAX 0 with its stack, FS and exception chain restored. Its caller `0x00033c12` enters that helper a second time. Original code creates/closes `UDATA`, creates its `4d530002` child, and retains that child's real handle while preparing the next operation. The volume now contains five directory nodes, four created during startup, with six opens / five closes / one live handle. That pending handle belongs to the still-running second helper; neither it nor the caller has returned. It is not a leaked temporary link handle.

The original path buffer and counted descriptor used for the `T:` target are subsequently reused for the UDATA path. The link continues to refer to the TDATA directory's identity. The existing `D:` device link remains valid with its device reference held and disc readiness false; neither link establishes mounted disc reads.

Three newly reached original functions are admitted separately:

| Entry | Bounded translation evidence | Actual browser evidence |
| --- | --- | --- |
| `0x00034594` | 103 contiguous bytes / 41 instructions | Three calls return `0xffffffff`, the last actual XBE section descriptor, and `0xffffffff`; stack/FS/exception state is restored |
| `0x00031127` | Ten instructions | Two calls return the actual thread-local data address and write the supplied value at offset 4; restored context |
| `0x0003105c` | 101-byte envelope / 39 instructions | One complete entry/return with restored context |

The lookup invokes ordinal 279, counted ANSI string equality, 33 times with case-insensitive comparison requested: one nine-byte comparison succeeds and 32 return false. Its positive result matches the actual section-table pointer plus ten 56-byte entries; no SDK function name is inferred from that result alone.

The two lookup misses supply 1168 to `0x00031127`. The retained static export corroborates that argument at `0x000345e3`, the call at `0x000345e8`, and the accessor's argument load/store at `0x00031148` / `0x0003114c`. Browser evidence independently observes 1168 at thread-local data offset 4, with the other eight zero-fill bytes still zero and the original self-reference intact. The earlier verifier's assumption that all zero-fill bytes remain zero after later original execution was invalid. The final check requires this exact observed result; it does not ignore thread-local corruption.

## Explicit next stop

There are **96 completed kernel operations and one stopped attempt**, 97 records in total. The next call is ordinal 190 (`NtCreateFile`), return **`0x00033ac9`**, requesting:

| Field | Actual request |
| --- | --- |
| Name | `\Device\Harddisk0\partition1\UDATA\4d530002\TitleMeta.xbx` |
| Counted name | 57 bytes, maximum 58; absolute path, root handle 0 |
| Desired access | `0x40100000` |
| Object attributes | `0x40` |
| Allocation-size pointer | Null |
| File attributes / share / disposition / options | `4` / `1` / `3` / `0x22` |

The current create service admits directories only. The regular-file request stops internally with state 9 / `0xc00000bb` before returning to the guest. **This is an unsupported runtime contract, not an accepted original-kernel error result.** No regular file, payload write, file-size effect, save or success response is fabricated. The second directory helper remains active.

Next implement and independently verify real regular-file create/open-if identity, handle/access/share/options, checked outputs and bounded local backing for this requested contract. Then connect it to the existing translation and observe the next actual operation, including any payload write; do not assume its buffer, length or success. Reuse the eleven seeds and `recomp-title-file-links-04`. More Ghidra work is not a substitute for this already-reached service. No directory-as-file adapter, hardcoded title filename success, native/HTTP path fallback or implicit original disk state is accepted.

## Implemented contracts and limits

`file_namespace.c` binds the owned namespace to the admitted guest volume. Opening a link target uses an actual temporary directory handle, references its file object, then closes the handle. The namespace owns that reference until the final link is deleted. Additional aliases can share the same target; a suffix is resolved relative to the target's directory identity. Failed creation releases temporary references/objects. Opaque file IDs are owned runtime tokens, not a claim to reproduce the Xbox file-object memory layout.

`local_volume.c` separates directory nodes, handles and referenced open instances. Referenced file objects are materialized when needed and can outlive handle closure. Ordinary handle closure still releases its ownership. The existing directory walker is shared by guest directory creation and internal target opening; no fabricated guest frames or guest-memory writes are used for the internal open. Sharing checks cover live handles under the admitted rights; complete original cleanup/sharing/ACL/concurrency behavior remains unproved. The link's internal open requests access 0, share 7 and directory option 1, so it introduces no read/write access conflict.

`guest_strings.c` adds counted equality with separate admission status and guest Boolean result. Unequal lengths and empty strings do not dereference payload buffers; equal nonempty lengths require checked reads, preserve embedded NUL bytes and never mutate guest memory. The explicit byte-case profile follows the pinned public comparator: ASCII and admitted Latin-1 lowercase folding, excluding `0xf7`, with `0xff` mapped to `?`. This is bounded compatibility evidence, not matching-original-kernel fault, alignment, locale or complete string-library acceptance. See the pinned [ordinal-279 implementation](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlRtl.cpp#L811) and [byte uppercase profile](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlRtl.cpp#L2124). The [public link-target resolver](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlOb.cpp#L446) corroborates opening/referencing a directory and closing the temporary handle; it is not original-kernel runtime evidence.

ABI 9 adds retained file-target observations and admits an explicitly declared **128 kernel-record capacity**. Earlier modules retain 32; arbitrary capacities are rejected. The trace remains capped at 128 events. All original-byte, independent decoder, layout, checked-memory, 64-function / 8,192-instruction and metadata budgets remain. Fixed unshared 128 MiB Wasm / 64 MiB guest backing and exactly one bounded clock import are unchanged. ABI 9 retains ABI 8's explicit fresh synthetic directory state. There is no host filesystem/network capability, original upload/serving, persistent disk state or disc-file provider. Original execution remains browser-worker only; native/Node fixtures contain independently authored code.

## Evidence and verification

Artifacts below stay ignored under `research/verification/`. No original instructions/XBE, generated title C/Wasm, raw analysis/RAM/traces, screenshots or assets are served, uploaded or committed.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-file-links-next-02/seed.json` | `a9dd78399b787627efe12e5b1c066fb888fdfe15a1e6fb2d2f8bb8b59ad890d7` |
| `recomp-file-links-next-04/seed.json` | `bd4173691a8ea8e0524b5fb9578e3303575547c71bd43495be3748c1814f449e` |
| `recomp-title-file-links-04/generation.json` | `af0ea155eb2bd1b538e9ebee8d51292681eb49c530fca5a14aab95fd32a3e375` |
| `recomp-file-links-build-07/build.json` | `27e1e010fadc5bddb5962ffbb132f7da26d8c13ec44dc6bfa7f72b4003cb0110` |
| Matching `startup.wasm` (250,642 bytes) | `a528eb41d8b0b7647a72a4217685450f6eb2b4fb719e0e8436ace3cd1487d8e1` |
| `file-links-owned-02/checks.json` | `cb23c6a65196477d9e52f8623746927c819611a6d0629030ee4c421df8962050` |
| `file-links-directories-02/checks.json` | `8f4fba7925c61324151f34a3b26114ce8c49914bb05a4857b66d51589abc0299` |
| `file-links-volume-01/checks.json` | `0abe760a7859d3656791b4fecdd8853fd6c674f1dcba89c1bb418d0045c0489d` |
| `file-links-namespace-01/checks.json` | `23592affc76241ab05da390ab8c6368d1ae2fffdafa8fd24d6a90e71b84a310f` |
| `file-links-browser-20261101-09.json` (synthetic 0) | `80c89b10adc47ae9f53fe996be856fa05f5c651513ccea335600cc840e7cc5f1` |
| Matching `10.json` (synthetic 1) | `447c207d28e7cf35478adf7ae10107f4350fa82c46af26789671451e50f39e86` |
| Matching `24.json` (recovery, empty warning/error log) | `979bd86b2a139a3779c81aa9f334252858db57c66a3abd011ba7012d52c1c526` |
| Matching `24.jpg` (inspected screenshot) | `2534998528d41880d21b9e0f9f350bf670346ce98010613a2dd5561e71591325` |

`file-link-checkpoint-audit-01.json` passes 27 checks: 25 compiled owned source/declaration identities, fixture identities 11/9/6/6, all eleven seeds, generated/build/browser identity, retained target/close/handle state, actual helper/TLS/string/file-request evidence, regressions/rejections/Clear/logs, unchanged supplied/reference XBE and clean vendor/header pin. The first nine seeds remain those of the directory checkpoint; append `recomp-file-links-next-02` and `recomp-file-links-next-04` only.

Four owned fixture suites pass all twelve native optimized, address/undefined-behavior sanitizer and Wasm compile/run combinations. Coverage includes retained references, extra references/aliases, target deletion, relative child opens, collision/capacity cleanup, read-only names, unsupported paths, preserved directory/volume contracts and string comparison boundaries/failures. Twenty-one source-owned runtime/admission/delivery cases and nineteen Python cases pass locally. The first restricted runtime-test attempt could not connect to loopback (`EACCES`); its permitted loopback rerun passes all 21. Source checkpoint `6b062f3` is published; [CI run 41353212749](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41353212749) passed Windows and Linux, including the new Linux file-target fixture, runtime/Python/format checks, publication guards and preserved prototype tests/build. CI executes no original game or private title toolchain. The preparation/publication guard passes for 181 source files and whitespace checks pass.

Preserved attempts are not relabeled passes: owned `file-links-owned-01` lacked a standard header before correction; private build `03` reached the genuine 32-record observation limit. Seed `next-03` failed complete-byte admission; the read-only gap decoder then accounted for 13 previously undefined bytes without modifying the persistent Ghidra project. Build `06` was mistakenly started after that failed translation, lacked generated input, failed compilation and executed no original code. Final `next-04` / translation `file-links-04` / build `07` pass admission/compilation. Browser records `01..06` retain genuine dependency/budget stops and the old verifier; `07/08` retain its incorrect post-use zero-fill assumption and diagnostic. Final `09/10/24` pass 61 checks; `11..19` preserve the passing 50/43/40/36/31/27/22/20/16-check modules. `20..22` retain the three rejected inputs; a local assertion initially expected the word Wasm instead of the actual WebAssembly rejection text, then Clear was verified in `23/25`. `file-links-browser-inputs-01` reconstructs explicit synthetic profiles from the retained accepted metadata; no original-console settings are inferred. The temporary tab/verified host were cleared and closed.

Original saves, FATX persistence, regular-file payloads and disc reads, full memory/exceptions/settings/clock/shutdown/scheduling, graphics/audio/input, browser-local end-user conversion, every original detail and all planned online/social/control features remain unaccepted. No failed probe authorizes a route change or reduced product scope.
