# Original utility drive mounts from backed FATX bytes

November 2, 2026. Original browser startup now opens its formatted cache root, queries allocation size, closes that handle and links `Z:` to the mounted partition. Original `XMountUtilityDrive` returns one. The next original helper enters and requests memory statistics. **There is no playable original screen, general FATX file support or durable save implementation.** The complete original-game/custom-feature goal remains active.

## Observed execution

Both explicit fresh synthetic profiles return from all 83 initialization callbacks, execute 227 distinct original routines, record 3,622 events and complete 392 kernel operations. After the previously verified format/dismount/close:

1. `NtOpenFile` opens `\Device\Harddisk0\Partition5\` through the new FATX provider, returning handle `0x00070000`.
2. `NtQueryVolumeInformationFile`, class 3, returns 65,519 usable clusters, 65,518 free clusters, 32 sectors per cluster and 512 bytes per sector. These come from the written header, supplied partition extent and scanned allocation table.
3. `NtClose` releases that root handle and file instance. The mounted filesystem/device remains independently identified.
4. Original `IoCreateSymbolicLink` creates `Z:` targeting `\Device\Harddisk0\Partition5`. Its device object has two namespace references. `D:` remains an unmounted disc device; `T:` and `U:` keep their separate partition-1 file references.
5. Original `0x00030e25` returns `EAX=1`, with stack/FS/exception head restored. The next 79-byte / 25-instruction helper at `0x00030ade` enters and stops before ordinal 181, `MmQueryStatistics`, returns to `0x00030af5`.

The pending request points to `62983784`, supplies length 36 and has a writable output range. No statistics, guest status or helper return is invented. Its [pinned nxdk declaration](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) uses the nine-DWORD `MM_STATISTICS` structure, with page and byte fields that must remain distinct.

## Implementation and fidelity boundaries

[`fatx_volume.c`](../src/runtime/fatx_volume.c) mounts the actual sparse backing after RAW attachment retirement. It validates header/cluster geometry, allocation-table width and extent, root bounds/chain, and free-cluster counts. Handles and root-file instances occupy separate ranges from raw disk and partition-1 objects. Retained file instances survive handle closure until their references are released. The namespace registers a typed mounted device; it does not rewrite the cache path into the synthetic partition-1 tree.

The FAT16/FAT32 choice uses partition-size-derived entries, including the reserved entry, before subtracting metadata. In the reached 1 GiB case, selecting width from usable data clusters would incorrectly choose FAT16. The resulting four-byte entries, 266,240-byte table and 270,336-byte data offset agree with the original formatter and its retained bytes. The [first-person format research](https://aerosoul94.github.io/blog/2020/02/25/fatx-reading-and-recovery.html) and [XboxDev reader calculation](https://github.com/XboxDev/fatx/blob/master/libfatx/fatx.c) corroborate this interpretation; the latter file's inspected Git blob is `26305e3d2dca3f84b94d25c8c6935b5c01f9e907`. This is independently authored browser code; no filesystem library was installed or vendored.

Current scope is read-only root/size access over explicit fresh test partitions. It permits four mounted filesystems, sixteen file instances, a 262,144-entry scan limit and root chains up to 64 clusters. It supports the validated 512-byte-sector FAT16/FAT32 subset; other formats stop. A successful raw write advances the backing revision; stale mounts stop before using cached metadata. General directory/file access, FATX writes, mounted-filesystem dismount/remount, durable storage, original kernel failure/locking behavior and retail equivalence remain unproved. Unsupported FATX operations stop instead of being routed to the unrelated userdata provider.

ABI 22 retains fixed 128 MiB Wasm / 64 MiB guest memory, 16 MiB immutable source staging, 4,096 trace events / 512 kernel records and the sole browser-clock import. The new helper was exported read-only from the existing Ghidra project, then admitted with original-byte, independent-decoder, layout and guarded-memory checks. The selection is now 317 functions / 10,612 instructions. These counts are not whole-game coverage. Original execution occurs only in browser workers; original/derived content remains local and ignored.

## Evidence and checks

Private records under `research/verification/`:

| Record | Scope |
| --- | --- |
| `recomp-title-post-utility-01/` | Current selection and exact nineteen-seed `inputs.json`; prior exports retained |
| `recomp-post-utility-build-02/` | Current ABI-22 module; all 34 compiled source/declaration identities match |
| `post-utility-browser-20261102-03.json`, `-04.json` | Final original runs for settings zero/one, utility return, mounted device and pending memory query |
| `fatx-utility-checkpoint-20261102-01.json` | Independent source/seed/module/report identities, original returns, allocation query, handle/reference, drive-link and preserved-byte checks |
| `fatx-root-owned-02/checks.json` | Optimized and address/undefined-behavior sanitizer integration checks with owned FAT16/FAT32 structures |
| `fatx-root-disk-owned-01/checks.json` | Changed disk writer and existing range/sharing/failure-atomicity checks |
| `legacy-raw-volume-browser-20261102-01.json` | Prior ABI 21 retains its 388-operation partition-root boundary under the current browser harness |

All 39 runtime/admission/source-delivery cases pass. Owned integration checks cover allocation geometry, a root chain spanning multiple clusters, cyclic/free/out-of-range roots, separate userdata/cache identities, retained references, sharing, denied/short/overlapping outputs, stale backing and file/namespace capacity failures. Browser warning/error logs are empty and Clear succeeds. CI now includes the owned FATX fixture; no public CI result is claimed for this local checkpoint. Unchanged Python translation tests and unrelated prototype/format campaigns were not repeated.

## Next task

Implement the reached `MmQueryStatistics` contract using consistent accounting for the modeled guest environment. Reconcile physical page capacity, available pages, virtual committed/reserved bytes, and cache/pool/stack/image ownership before writing the checked 36-byte result. Represent any required unmodeled baseline explicitly; do not substitute host/Wasm allocation size, count every virtual reservation as physical use, double-count shared image pages or invent original-console defaults. Then continue original `0x00030ade` and follow its actual next dependency. Reuse the current generation and retained nineteen seeds; no new extraction/import is needed to start.

Complete scheduling/exception/floating-point behavior, disc/package reads, full filesystem/persistence, original graphics/audio/controllers, playability, end-user local conversion and every planned online/social/control/authority feature remain unfinished.
