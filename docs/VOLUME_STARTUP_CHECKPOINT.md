# Original volume lifecycle and drive-link request

November 1, 2026. This advances static recompilation toward the complete original game and planned custom features. It does not accept a playable screen, full filesystem/save fidelity or any full-product gate. Original instructions run only in a browser worker; native/Node runs below contain independently authored fixtures only.

## Implemented contract and limits

`local_volume.c` implements a bounded guest volume-root object for `\Device\Harddisk0\partition1`, with counted-name resolution, case handling, root-relative reopening, owned opaque handles, compatible read sharing, close/lifetime tracking and `FileFsSizeInformation`. Guest pointers are checked against admitted reservation/commitment/permissions before access. Unsupported devices, paths, rights, modes, information classes and overlapping output/input layouts remain explicit unsupported contracts. This does not expose host paths or native filesystem access.

The component supports sixteen simultaneous root handles in a disjoint handle range, retires closed identifiers rather than immediately reusing them, preserves the existing boot-thread handle and writes real status/information outputs. The size query emits both 64-bit allocation counts and the two geometry fields into the validated 24-byte buffer. Owned fixtures exercise counts beyond 32 bits, handle exhaustion, sharing, close/reopen/stale handles, path distinctions, short buffers, corruption, permissions and failure atomicity. A failure that deliberately reports an I/O status may update that status block; admission/unsupported failures preserve guest outputs and handle state.

ABI 6 adds mandatory explicit local volume-profile configuration and bounded volume diagnostics. It retains checked guest memory, the sole clock import and the existing fixed unshared 128 MiB Wasm / 64 MiB guest RAM proof window. Kernel observations are bounded to 32 records of 20 words, allowing a size-result snapshot before original stack reuse. Earlier proof ABIs remain compatible. None of these component capacities establishes the final product limits or a complete Xbox kernel.

`settings-volume-0.json` and `settings-volume-1.json` are source-owned combined console/volume profiles. Both require explicit synthetic provenance; allocation counts are decimal strings with checked 64-bit bounds and geometry validation. No matching original disk state or default capacity is assumed. Profiles describe a developer volume's metadata; no original disk sectors, user files, allocation map, FATX persistence or console save equivalence is established.

Public declarations corroborate the Xbox 12-byte object attributes, counted ANSI strings, syscall signatures, information class and size layout: pinned nxdk [kernel declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) and [types](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxdef.h). They are not matching-console runtime evidence or proof of every failure/status variant.

## Observed original execution

The eight saved seeds and original translation remain **23 functions / 1,364 instructions**. No extraction, new executable import, further Ghidra export or original-byte change was needed. Twenty distinct original routines enter; the trace now has 38 events, including the original volume initializer's return.

Both explicit profiles pass **36 browser checks plus 8,410 owned copy/ABI cases each**. The previous timer, heap return, stack/FS/exception-chain balance, checked reservation/commitment and protection-denial checks remain in force. The six-hour timer stays pending with no callback delivered. Fifteen kernel operations complete, extending the prior twelve with:

| Original call | Observation |
| --- | --- |
| 202, `NtOpenFile` | Original request resolves the declared hard-disk root; a live owned handle is returned with success and `FILE_OPENED` information |
| 218, `NtQueryVolumeInformationFile` | Original arguments request class 3 and 24 bytes; actual returned counts/geometry match the explicitly selected profile, with success and 24 bytes reported |
| 187, `NtClose` | The original caller closes that handle; open/close counts are 1/1, active count is zero and no volume handle remains |

Original `0x00033794` returns with its entry stack pointer, FS and exception-chain head restored. Its observed EAX is zero; no additional meaning or complete free-space algorithm is inferred from that value.

Startup then stops at **unsupported ordinal 67, `IoCreateSymbolicLink`**, return `0x00033d41`, with exactly two counted-string arguments:

| Argument | Observed name | Length / capacity |
| --- | --- | --- |
| Link | `\??\D:` | 6 / 7 bytes |
| Device | `\Device\CdRom0` | 14 / 15 bytes |

Both strings are decoded through admitted read ranges, without guest writes, path following or service success. **Inference:** the next namespace operation establishes the original game-disc drive alias. **Unknown:** the link service, mounted disc device and original file-read path do not yet exist. A namespace link is not evidence that files can be read or the game can render.

## Retained evidence and checks

All artifacts below stay ignored under `research/verification/`. No original code/module, assets, RAM, raw traces or screenshots are served, uploaded or committed.

| Artifact | SHA-256 |
| --- | --- |
| `recomp-volume-build-02/build.json` | `6e42b7cb7490c15cba51111f0db35fd24a00afdbb5427daf86884e33d30746bd` |
| `recomp-volume-build-02/startup.wasm` (156,990 bytes) | `1bef6bdd57831dab6424f1a3ad90417e8502d3ad3ed7e58bd9013f15e6327ae7` |
| `local-volume-03/checks.json` | `7cca22b2e151d365fa7c61c3ff95338fe1bf6f5d0d74be6f62659693a24fc3c1` |
| `volume-startup-browser-20261101-04.json` (synthetic 0) | `02d3798eb1dae49bcc92ac02ad0805499fb267e68c54aa1193cdc7e791e0bd9a` |
| Matching `05.json` (synthetic 1) | `fcf7c35e6ca1adbb1c01cbbd12d288a534b8397ff95b89f328766438f9985128` |
| Matching `14.json` (recovery, empty warning/error log) | `23f89fee3c1dc32d9098b8f2db2808b9874c77480b93b9e483a375985d30e85b` |
| Matching `14.jpg` (inspected screenshot) | `2771de093bfc671e8feabf3ec96c4c52a9be1cd284fdc95f3495e8a2994186fa` |

`volume-checkpoint-audit-01.json` verifies eighteen compiled owned identities, six fixture identities, both generated outputs, all eight seed identities, actual browser/module identity, unchanged supplied XBE and clean pinned vendor source. Generation remains the prior `recomp-title-heap-04`; see the [heap checkpoint](HEAP_STARTUP_CHECKPOINT.md) for its identity and original-function coverage.

Owned native optimized, address/undefined-behavior sanitizer and Wasm volume fixtures all pass. Earlier fixture attempts `local-volume-01/02` preserve test-reservation alignment and permission-setup failures; they did not execute original code. Seventeen owned runtime/admission/delivery tests pass, including explicit profile bounds and standalone counted-string checks. CI adds the owned C volume test on Linux without any game files or private toolchain. Publication/whitespace/CI results are recorded in status/handoff separately.

Initial browser `01.json` records successful open and the genuine unsupported query, before enabling its observed class/length contract. `02/03.json` retain the completed lifecycle and obsolete verifier boundary; these are not relabeled passes. Final `06..10.json` preserve the passing 31/27/22/20/16-check legacy modules. `11.json` rejects missing volume settings; `12.json` rejects malformed Wasm; `13/15.json` verify cleared selections/details; `14.json` verifies recovery. Final browser warnings/errors are empty, and the temporary tab and verified source-only host were closed.

## Next concrete task

Implement the actual bounded object-namespace contract for `IoCreateSymbolicLink`, using the observed `\??\D:` to `\Device\CdRom0` request. Prove counted-name admission, object ownership, collision/lifetime/failure behavior and resolution before following further original startup. Keep the disc device tied to explicitly selected compatible local content; do not redirect paths to arbitrary host files, repository assets or an HTTP fallback. Reuse the existing folder/XISO evidence and conversion readers; do not repeat extraction or claim a link alone implements disc reads.

General file operations, mounted game-file reads, browser-local save persistence/quota/recovery, original disk/settings/clock/shutdown fidelity, full Xbox memory/exceptions, rendering/audio/input, browser-local end-user recompilation, every original detail and all planned online/social/control features remain unaccepted. The complete objective stays active.
