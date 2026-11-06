> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Original cache-partition formatting writes

November 2, 2026. The original browser code now opens its selected cache partition, queries disk/partition information, obtains system time and writes its FATX metadata. **The formatting routine has not returned: it stops at an unsupported filesystem-dismount request. There is no playable original screen or mounted utility drive.** Static recompilation and the complete original-game/custom-feature objective remain selected.

## Observed original execution

Both explicit synthetic profiles return from the same 83 original initialization callbacks, execute 226 distinct original routines, record 3,617 entry/return events and complete 386 kernel operations. The original `0x00033ddb` formatter performs:

1. A real exclusive open of `\Device\Harddisk0\Partition5`, with sector-aligned raw I/O options, into the shared sparse disk backing.
2. Ordinal-196 `IOCTL_DISK_GET_DRIVE_GEOMETRY`, returning the configured 24-byte geometry.
3. Ordinal-196 `IOCTL_DISK_GET_PARTITION_INFO`, returning the configured 32-byte extent and descriptor.
4. Original heap-buffer allocation and ordinal-128 `KeQuerySystemTime`. The existing browser-clock capability supplies the checked eight-byte output; no new host import is introduced.
5. Seventy successful 4,096-byte writes, totaling **286,720 bytes**, at consecutive partition-relative offsets zero through 282,624. Recorded physical offsets match the explicit partition base plus each relative offset; no write crosses the partition extent.

The pending call is ordinal 200, `NtFsControlFile`, control `0x00090020` (`FSCTL_DISMOUNT_VOLUME`), returning to `0x000340d4`. Its ten arguments are `[393220, 0, 0, 0, 62983480, 589856, 0, 0, 0, 0]`. It stops before a guest result. One partition handle remains open; the earlier whole-disk handle is closed. No utility-drive link or formatter return is observed.

## Written layout and its limits

Eight bounded post-stop samples per run establish:

| Partition-relative location | Observed bytes |
| --- | --- |
| 0 | `FATX` header, 32 sectors per cluster and root-cluster field one |
| Header serial field | Matches the low DWORD of the actual returned system-time sample |
| 4,096 | FAT words `0xfffffff8`, `0xffffffff`, then zero entries |
| 8,192 and 266,240 | Zero-filled table pages |
| 270,336 through 286,719 | Four complete pages, all `0xff`, forming the 16 KiB empty-root byte pattern |

The backing has seven allocated pages: the prior disk cache record plus the six nonzero format pages. Unwritten/zero pages retain real sparse zero-fill semantics. The disk cache record remains intact. No formatter output was prefilled by the host.

For **this explicit 1 GiB partition**, the observed FAT area is 266,240 bytes and the root starts at 270,336. A naive ceiling of `partitionBytes / clusterBytes * 4` would place the root one page too early. Whether that extra page follows an always-round-up rule, an extra table entry or another boundary rule must be established from the retained original formatter before generalizing a filesystem parser. This is a material mount/fidelity concern; do not treat one geometry or public FAT descriptions as universal acceptance.

The public [FATX layout overview](https://xboxdevwiki.net/FATX), its linked [first-person filesystem investigation](https://hackmd.io/DXo0p8VnQLy3XQ7YR1Ny_w?view=), and pinned [nxdk kernel declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) support interpretation of the structures. Their uncertainties remain separate from the actual bytes and calls observed here. They do not establish matching-retail-console behavior, full FATX support or a mounted filesystem.

## Runtime changes

[`local_disk.c`](../src/runtime/local_disk.c) now provides explicit, nonoverlapping partition views over the same backing, with 64-bit offsets, partition-specific sharing, bounded relative I/O and close/lifetime behavior. Geometry and complete partition descriptors must be explicitly configured before opening the disk. Their sizes/ranges/flags and guest output memory are checked before mutation. Unsupported controls still stop; no formatting/dismount success stub is supplied.

The tested synthetic disk remains 8 GiB with 512-byte sectors and three supplied 1 GiB cache views beginning at 4, 5 and 6 GiB. The supplied geometry is 32,768 cylinders / 16 tracks / 32 sectors. Each supplied descriptor has its matching hidden-sector count and explicit type/flag values. None of these are inferred retail defaults or original disk contents. The high offsets exercise real 64-bit aliasing rather than wrapping into the whole-disk metadata region.

Proof ABIs 18/19/20 separately admit partition views, geometry and partition information while retaining ABI 17 and older modules. Fixed 128 MiB Wasm / 64 MiB guest memory, 16 MiB local executable staging, 4,096 trace events, 512 kernel records, eight disk handles / 128 sparse backing pages and the sole clock import remain unchanged. An optional read-only write ledger and backing sampler add no guest or host capability.

The formatter loads `NtDeviceIoControlFile` into a register before calling it. The preparation tool now admits its exact checked absolute pointer-load encodings for the established ordinal 196, including `8B` register loads and `A1`. Other unknown/data imports are not silently converted into call tokens. Ordinal-40 data remains separately read-only. Original bytes, independent decoding, complete layouts and guarded memory accesses remain checked.

## Evidence and validation

All original/derived records below remain local in ignored `research/verification/`; these are browser/direct-analysis records, not new REA Evidence IDs.

| Record | Scope |
| --- | --- |
| `partition-open-browser-20261102-01.json` | Actual partition open and original register-loaded kernel pointer boundary |
| `geometry-browser-20261102-01.json` | Successful geometry query, next partition-information request |
| `partition-information-browser-20261102-01.json` | Successful descriptor query, original heap allocation and next time request |
| `format-layout-browser-20261102-05.json`, `-06.json` | Final original runs for settings zero/one, all 70 write offsets and eight backing samples each |
| `format-layout-checkpoint-20261102-03.json` | Independent call/extent/time/header/FAT/root checks, exact module/report/source identities and explicit incomplete format/mount state |
| `recomp-title-disk-geometry-01/` | Current admitted code and exact `inputs.json`; 316 functions / 10,587 instructions, including the checked register-load binding |
| `recomp-format-snapshot-build-02/` | Current ABI-20 module; all 32 compiled source/declaration identities match |
| `partition-information-owned-01/checks.json` | Optimized and address/undefined-behavior sanitizer disk/partition/control cases |
| `legacy-disk-browser-20261102-01.json` | Prior ABI-17 module retains its 83-callback / 307-operation partition-open boundary under the new browser harness |

All 27 Python admission/memory cases and 38 runtime/admission/source-delivery cases pass. The latter use the local loopback access already required by their two source-server cases. Owned fixtures cover partition/whole-device aliasing above 4 GiB, independent sharing, extent violations, malformed/overlapping layouts, geometry/descriptor sizes and fields, permissions and failure atomicity. Final browser warning/error logs are empty and Clear releases selections/results. No broad preserved-prototype or unrelated conversion campaign is repeated; no original code runs natively or under Node. No extraction, project import or additional Ghidra export occurs in this task.

## Next concrete task

Implement the actually reached filesystem-dismount and volume-attachment/lifetime contract for the backed cache partition. Establish the distinction between raw-device handles and mounted filesystem objects; preserve written bytes and handle/reference behavior. Determine the original formatting algorithm's alignment rule before using the FAT/root offsets generally. Then observe the original close/formatter return and the subsequent real utility-volume open/query/link path. Do not substitute unconditional dismount success, an invented retail failure code or a hardcoded drive alias.

Reuse the current admitted selection and module above; the formatting/mount helpers are already retained. A bounded read of the existing analysis can answer an actual remaining contract question, but repeated setup/extraction or broad decompilation is unnecessary. Formatting return, utility mount, disc/package reads, durable saves/FATX equivalence, full scheduling/exception/floating-point behavior, original graphics/audio/controllers, an original playable screen, end-user browser-local recompilation and all planned online/social/control/authority features remain unaccepted. The full goal remains active.
