# Original title-data directories and next drive-link contract

November 1, 2026. Static recompilation to WebAssembly remains the selected route for the complete original game and all planned custom features. Original startup now creates its title-data directories in the explicit synthetic test volume. No playable screen, original save state, FATX persistence, complete runtime or fidelity acceptance is established.

## Observed original execution

The retained ninth seed adds actual helper `0x000338de`: 820 contiguous bytes / 271 instructions. All original-byte, independent decoder, complete-layout and checked-memory admission remains intact. Nine seeds now translate **31 functions / 2,401 instructions**; **82 trace events / 28 distinct original routines** are observed. These are different coverage measurements, not executed-instruction counts. No new Ghidra run, executable import or extraction was needed.

Both explicit synthetic settings/volume/directory profiles pass **50 browser checks and 8,410 owned copy/ABI cases each**. After the previous sixteen completed kernel operations, original code performs:

| Actual operation | Observed result |
| --- | --- |
| Ordinal 190, create/open `\Device\Harddisk0\partition1\TDATA` | Creates directory 2 under the owned volume root; real guest handle `65540`, I/O information `FILE_CREATED` / 2 |
| Ordinal 187 | Closes that handle; the directory remains available for child resolution |
| Ordinal 289, initialize counted ANSI string | Length 43, maximum 44, buffer equals the actual source pointer; the string buffer is retained rather than allocated/copied |
| Ordinal 190, create/open `\Device\Harddisk0\partition1\TDATA\4d530002` | Creates directory 3 under directory 2; distinct guest handle `65544`, information 2 |
| Ordinal 187 | Closes the second handle; both directory identities remain in the current run |

The two create requests have nine Xbox arguments: null allocation-size pointer, attributes `0x80`, share 3, disposition 3 and options `0x4021`. Requested access is respectively `0x100001` and `0x120117`. These values are runtime observations. The [pinned nxdk declaration/constants](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L2927) establish the nine-argument contract and create/open information values; the [pinned Cxbx export](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlNt.cpp#L312) corroborates ordinal 190. These public declarations are not original-kernel execution evidence.

There are **21 completed kernel operations and one further stopped attempt**. Volume counters are three opens / three closes / zero live handles, with three directory nodes including root. Handle numbers and directory IDs are chosen by this runtime, not matching-console observations. The earlier original format return, heap/volume return, memory protections, pending timer and disc-device reference checks remain intact.

## Explicit next stop

The next original ordinal-67 request is `\??\T:` targeting `\Device\Harddisk0\partition1\TDATA\4d530002`, return **`0x00033c05`**. The existing namespace only admits direct disc-device targets and lacks this hard-disk/file-object path. Its internal admission failure stops execution with state 9 / `0xc000000d`; **that value is not returned to the guest or accepted as the original kernel's response**. No `T:` link is inserted; the five earlier namespace objects remain unchanged. Neither `0x000338de` nor its caller `0x00033c12` has returned.

The [pinned public target resolver](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlOb.cpp#L413) follows object parsing and can open/reference a directory file object while closing its temporary handle. This explains a concrete gap in the direct-device-only model; it does not establish exact original-kernel fidelity. Next implement owned volume/file-object resolution and retained target references, including remaining path handling, rather than hardcoding a `T:` string substitution or pretending the missing contract succeeds.

## Implemented contracts and limits

`local_directories.c` owns a bounded directory tree inside the explicitly synthetic volume. Create, open and open-if distinguish missing parents, missing final names and collisions. Handles refer to directory identity; closing handles preserves nodes. Sharing is symmetric for admitted read/write rights and applies to the same directory, not every handle on the volume. All output, descriptor, counted-name and memory permissions are admitted before mutation; handle/node capacity failures do not create leaked directories. Root-volume opening retains its existing behavior and now compares the correct directory identity.

The profile supports 32 directories, 16 live handles, ASCII paths shorter than 256 bytes and components shorter than 64 bytes. Unsupported paths, modes, rights, allocation hints, non-directory operations, aliases and overwrite/delete behavior remain explicit. ABI 8 requires `volume.directoryState: "fresh"` in the explicitly synthetic input; the earlier capacity-only profiles are rejected before original execution. `settings-directory-0.json` and `settings-directory-1.json` supply this declaration while preserving the prior settings/capacity choices. The tree resets for each proof; no original disk sectors, FATX allocation accounting, ACL/concurrency equivalence, file payloads, persistent saves or general filesystem are established. Synthetic capacity metadata is not measured original free space.

`guest_strings.c` implements the admitted counted-string initializer with checked source reads, an eight-byte descriptor write and the retained source pointer. Null and empty sources differ correctly; disjoint strings up to 65,534 bytes are admitted. Longer, aliased, unmapped or unsupported output cases stop before partial writes. The guest ABI returns void; the component's internal admission status is not a guest return value. The [public nxdk ABI](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L2005) and [pinned Cxbx implementation](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlRtl.cpp#L1089) corroborate this bounded case, not all fault/overflow behavior.

ABI 8 adds explicit owned-directory observations and the actual ordinal-190/289 services. It retains checked memory, explicit synthetic settings/volume metadata, the declared 128-event trace and exactly one bounded clock import. Earlier modules remain compatible. No host filesystem/network capability, original-content service or uploaded state is introduced. Original code executes only in the browser worker; native/Node fixture execution contains independently authored code only.

## Evidence and checks

All artifacts below remain ignored under `research/verification/`. No original XBE/instructions, translated title C/Wasm, raw analysis/RAM/traces, screenshots or assets are served, uploaded or committed.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-title-init-next-01/generation.json` | `740422aaaf2beb1609aa19b939aa97667a65efaaa66d67361409b6392bc4fc58` |
| `recomp-directory-build-03/build.json` | `f5ba0b3b95e7e4e59addb8c496ab430195c9d59afd5c126ca1f4f34c510864e1` |
| Matching `startup.wasm` (230,206 bytes) | `6856cadfe021fc3569eef372662227a121bfd2896644d9c73ca78d2316d040a5` |
| `local-directories-04/checks.json` | `cc19768523c2cac03e5736f7675ba451a028b8e6aaaa2ff8e134dd87a28d2dec` |
| `local-volume-05/checks.json` | `e026919fe8c7044743625c44dc1c2a7200a542d7c8f7576fab23c5f109623cb4` |
| `directory-startup-browser-20261101-20.json` (synthetic 0) | `c1a9ac4fd7452146adfe56c6431c9c3e0cedd5e9e911af6cc2ad18c3c81bee86` |
| Matching `21.json` (synthetic 1) | `ad995e4fc16be2c74a232e379aa6ee04c4bdfbe9e570d5f10d430564d1130573` |
| Matching `34.json` (recovery, empty warning/error log) | `e8268b6af19927f6c9fa657b721ef697376cea82d39385c397312f90afdd9d30` |
| Matching `34.jpg` (inspected screenshot) | `e5a5565a864c011afc7f9f4b7a153fcecf57805f216a4b7c5e1e3b14fb6eaea7` |

`directory-checkpoint-audit-02.json` passes twenty-one checks covering explicit fresh state, 23 compiled owned identities, nine directory/string and six volume fixture identities, all nine seeds, generated/build/browser identity, actual directory/string/stop evidence, regressions/Clear/logs, unchanged supplied XBE and clean vendor/header pin. The ninth seed remains `recomp-library-next-05/seed.json` at `ec1720f5d56dfc2d22f45ce410a2f0409ed726b5df829fc412c4baa78efe08b8`. Translation retains the 64-function / 8,192-instruction and metadata budgets. The module retains fixed unshared 128 MiB Wasm / 64 MiB guest backing; this is not peak-memory or release-performance acceptance.

Directory/string and prior volume fixtures pass native optimized, address/undefined-behavior sanitizer and Wasm runs. They cover identity/lifetime, relative/absolute lookup, create/open outcomes, per-directory read/write sharing, capacity/failure atomicity, descriptor/memory admission, counted-string null/empty/high-byte/boundary/alias cases and preserved source buffers. `local-directories-02` preserves an invalid fixture mapping below the admitted guest window; `03/04` retain the corrected runs. Earlier directory-only `01` and volume `04` remain preserved.

Twenty source-owned runtime/admission/delivery cases and nineteen Python cases pass locally. CI adds the owned directory/string check on Linux; publication/CI results are recorded in status/handoff. Private init-next/browser `01/02` and directory/browser `01..04` retain the genuine create/string/access/link stops and earlier verifier state, not relabeled passes. Browser `05..19` and audit `01` preserve the 49-check result before fresh-state admission became explicit. Final `22..29` preserve the passing 43/40/36/31/27/22/20/16-check modules; `30..32` reject missing fresh state, missing volume metadata and malformed Wasm; `33/35` verify Clear; `34` verifies recovery. Warning/error logs are empty. The temporary tabs and verified source-only hosts were cleared and closed.

Next reuse the current seeds/translation and implement the reached directory-backed drive-link contract with real target lookup, lifetime and suffix resolution. Original save state, general filesystem/persistence, mounted disc reads, full memory/exceptions/settings/clock/shutdown/scheduling, graphics/audio/input, browser-local end-user conversion, every original detail and all planned online/social/control features remain unaccepted. No failed probe authorizes a route change or reduced product scope.
