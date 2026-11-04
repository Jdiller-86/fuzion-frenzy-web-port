# Original formatter returns after a real volume dismount

November 2, 2026. The original cache formatter now returns one in both browser profiles after its actual writes, filesystem-control call and handle close. **The next root-directory open is still unsupported. No utility drive is mounted and there is no playable original screen.** Static recompilation and the complete original-game/custom-feature goal remain active.

## Observations

- All 83 original initialization callbacks return. Each run executes 226 distinct original routines, records 3,619 trace events and completes 388 kernel operations.
- Original ordinal 200 uses its ten-argument Xbox ABI and `FSCTL_DISMOUNT_VOLUME` / `0x90020`. The explicitly supplied RAW attachment moves from attached to dismounted while its real open handle retains one reference. The synchronous I/O status is success with zero transferred bytes.
- Original code then calls `NtClose` on that same handle. The final reference is released and the attachment becomes retired. Both whole-device and partition handles are closed; disk backing still holds all previously verified header/table/root bytes.
- Original `0x00033ddb` returns `EAX=1`, with its entry stack, FS and exception head restored. The original instructions do not branch on the dismount result; the formatter's result follows the preceding real write operations. No formatter-return hook supplies this result.
- The following original helper requests `NtOpenFile` for `\Device\Harddisk0\Partition5\`, access `0x00100001`, share 3, options `0x00800021`, returning to `0x000337ca`. This request stops before a guest result. It has not been aliased to the unrelated partition-1 tree.

## Implemented contract and limits

[`local_disk.c`](../src/runtime/local_disk.c) provides separately identified RAW-volume attachments over the existing sparse backing. ABI 21 requires an explicit `disk.rawVolumes` list covering the supplied cache partitions; there is no inferred retail filesystem state. The two test profiles retain the prior fresh synthetic 8 GiB disk and 1 GiB cache views at 4, 5 and 6 GiB. They explicitly begin with RAW attachments and no filesystem file objects.

The reached dismount requires a live matching attachment, one reference, an exclusive synchronous handle with the required access, no event/APC/buffers and checked writable status memory. It detaches that attachment, preserves its lifetime until close and retains disk contents. Unknown controls, shared or retained-file cases, repeat dismounts and implicit remounts remain unsupported; they do not receive invented retail error codes. Existing raw handles retain access to the backing after detachment. Retired attachments cannot be silently reused.

The pinned [nxdk declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) establish the Xbox ABI/control value. Microsoft's [dismount documentation](https://learn.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_dismount_volume) describes invalidating the filesystem view before a later mount. **The browser provider is independently implemented compatibility code, not recovered Xbox kernel code or observed retail-console behavior.** The [ReactOS RAW implementation](https://github.com/reactos/reactos/blob/master/ntoskrnl/io/iomgr/rawfs.c) has an additional locking policy; it does not establish Xbox failure/locking semantics. Broader provider/reference/failure behavior and retail equivalence remain unknown. The explicit exclusive test path is the accepted component scope.

Fixed 128 MiB Wasm / 64 MiB guest memory, 16 MiB immutable executable staging, 4,096 trace events / 512 kernel records, bounded backing and the sole clock import remain. Original execution occurs only in browser workers. Neither game files nor generated modules are served, uploaded or committed.

## Allocation-table alignment finding

The retained, already admitted formatter analysis resolves the extra-page question for the reached branch. After dividing partition bytes by cluster bytes, instruction `0x00033edc` adds one entry. The following branch selects two-byte entries below `0xfff0` and four-byte entries otherwise; `0x00033ef3` through `0x00033efc` align the table allocation.

For the observed positive 1 GiB / 16 KiB cluster case, that gives 65,537 entries, 262,148 table bytes before alignment and 266,240 after 4 KiB alignment. The root therefore starts at 270,336, matching the independently sampled backing. An extra reserved entry explains this result; an always-add-one-page rule is unnecessary. This is a static calculation corroborated by one browser geometry, not universal filesystem acceptance. A mount parser must still validate its supported cluster widths, extents, table entries, usable data area and root chain.

The local source is `recomp-utility-mount-next-01/seed.json`, SHA-256 `36d662278d2b295aa8708e758354530225962e37e1b0b86b74d0b6eaf8f1bad5`, already retained in the current generation's `inputs.json`. No further extraction, import or Ghidra export was required. Direct analysis is not a new REA Evidence ID, and its static findings are distinct from the observed return above.

## Evidence and checks

All private records remain under ignored `research/verification/`:

| Record | Evidence |
| --- | --- |
| `recomp-title-disk-geometry-01/` | Unchanged admitted selection: 316 functions / 10,587 instructions; preserved byte, decoder, layout and memory guards |
| `recomp-raw-dismount-build-01/` | ABI-21 runtime build; all 32 compiled source/declaration identities match |
| `raw-dismount-browser-20261102-03.json`, `-04.json` | Final actual browser runs for settings zero/one, successful original formatter return and the pending root open |
| `raw-dismount-checkpoint-20261102-01.json` | Independent identity, return/frame, dismount/close/lifetime, extent/time/header/table/root checks |
| `raw-volume-owned-01/checks.json` | Optimized and address/undefined-behavior sanitizer checks of source-owned disk/attachment data |
| `legacy-format-browser-20261102-01.json` | Previous ABI-20 module retains its 386-operation dismount boundary under the current browser harness |

All 39 runtime/admission/source-delivery cases pass. The targeted native fixtures cover real retained backing, final-reference cleanup, denied memory, unknown control, shared handles, stale handles and unsupported remount with failure atomicity. Browser warning/error logs are empty and Clear releases selections/results. The unchanged translation/admission Python cases and unrelated prototype/format campaigns were not repeated. Public CI has not been claimed for these local changes.

## Next task

Implement the reached partition-root open through a real FATX mount of the written backing. Derive and validate header/table/root state and usable allocation geometry; create a distinct filesystem/provider and root-object identity, with actual handle/reference lifetime. Then follow the original volume query, close and utility-drive link. Preserve the unrelated partition-1 tree and retired RAW attachment; do not use a path alias, prefilled filesystem or successful mount stub.

Reuse the current generation and retained helpers. Disc/package reads, durable saves, complete scheduling/exception/floating-point compatibility, original graphics/audio/controllers, playability, end-user local recompilation and every planned online/social/control/authority feature remain unfinished. This checkpoint advances actual startup and does not accept the full product.
