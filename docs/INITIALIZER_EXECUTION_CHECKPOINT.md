> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Original initialization and disk-cache checkpoint

November 2, 2026. Selected route: static recompilation of the supplied original executable to WebAssembly, entirely inside a desktop browser. **All 83 original initialization callbacks return and the original application-entry path executes. There is still no playable original screen.** The complete original-game and planned custom-feature objective remains active.

## Observations

Two explicit fresh synthetic profiles reach the same boundary. Each browser worker executes 223 distinct original routines, records 3,583 entry/return events and completes 307 kernel operations. The original dispatcher finishes the 83 nonzero callbacks from its 84-slot table at `0x00281750..0x002818a0`. Returns are counted in original table order at the dispatcher's callback stack frame, with the original FS and exception-chain state restored. This is coverage of one initialization table, not 100% startup or whole-game coverage.

Execution continues through original `0x0002d540`, its callback wrapper `0x0004e690` and application routine `0x0002cd80`. The original utility-drive mount then:

1. Opens `\Device\Harddisk0\partition0` with a real owned block-device handle.
2. Reads 512 zero bytes at byte offset 2,048 from the explicitly fresh synthetic disk.
3. Constructs and writes its own 512-byte title/cache record, including the actual certificate title ID, version and selected cache index.
4. Closes the handle; the written bytes remain in the local sparse backing.
5. Selects cache partition 5 and enters its original formatting routine. Its next `NtOpenFile` remains unsupported and stops **before a guest result**.

The two resulting cache records are byte-identical, SHA-256 `14b8a36146690dbb751218db8b7a24eb24283eca4265bd4060d7526aab666b9f`. Both report one allocated backing page, one disk handle opened/closed, no open disk handles and no observed guest-memory fault. This does not establish existing-console cache contents or FATX fidelity.

The pending ordinal-202 request opens `\Device\Harddisk0\Partition5`, access `0x00100003`, share zero and options `0x18`, returning to `0x00033e0e`. Partition formatting, its helper's return and utility-drive linking have **not** succeeded.

## Implementation and static evidence

The current generated and compiled selection contains 316 original functions / 10,587 decoded instructions. Reuse `recomp-title-disk-device-01` and `recomp-build-disk-device-01`. Original-byte, independent decoder, complete-envelope and checked-memory admission remain enabled; the recorded x87 grouping normalization remains explicit. Translation is not proof that every selected function executes.

The previous prepared 305-function selection now runs in the browser: all 83 callbacks return, followed by the actual application-entry boundary. Three application-entry functions and eight immediate utility-mount dependencies were then admitted. A nine-byte undefined gap inside the application's established envelope was decoded read-only and retained as instructions; no bytes or function ranges were trimmed. An initial broad closure stopped at its discovery budget; the smaller exact exports are retained separately. No new disc extraction, XBE import, whole-program analysis or native/Node execution of original code occurred.

[`local_disk.c`](../src/runtime/local_disk.c) implements an independently authored sparse RAM block device with real handles, sharing, generic access mapping, synchronous read/write offsets and positions, zero-filled unwritten regions, close/reopen lifetime and bounded failure-atomic writes. It models the whole-disk device only. It does not expose a host path or give a cache partition a successful open without backing and an established contract.

ABI 17 retains ABI 16's 4,096 trace events / 512 operation records, fixed 128 MiB Wasm / 64 MiB guest memory, 16 MiB immutable local executable staging and sole bounded clock import. The disk has eight handles and at most 128 backing pages of 4,096 bytes; individual transfers are bounded at 1 MiB. The tested logical size is an explicitly selected synthetic 8 GiB, with three explicitly selected cache partitions and fresh zero state. These values are developer inputs, not retail defaults or observed original geometry. Backing remains volatile.

The original routine also reads kernel **data** import ordinal 40 (`HalDiskCachePartitionCount`). The preparation tool admits its reached `A1` pointer-load encoding only when its exact encoded slot matches the declared data import; normal byte/decoder/layout checks still apply. ABI 17 binds it to a separately read-only guest page containing the supplied profile value. Both browser runs observe value three and deny write permission in the page model. Function-call tokens are not used as data pointers. Public [pinned nxdk kernel declarations](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h) corroborate the kernel declarations; they do not establish matching-console runtime behavior.

## Local evidence and validation

All original/derived content below stays in ignored `research/verification/`. Direct Ghidra records and browser observations are distinct evidence types; this checkpoint has no new REA Evidence ID.

| Record | What it establishes |
| --- | --- |
| `initializers-browser-20261102-01.json` | Actual 83/83 callback returns with the first 305-function compiled selection; 216 routines execute |
| `recomp-title-main-entry-02/` | Three further application-entry functions admitted, preserving the complete nine-byte gap |
| `recomp-utility-mount-next-01/seed.json` | Eight bounded static mount/format dependencies, not observations that they all execute |
| `utility-mount-browser-20261102-01.json` | Actual whole-disk open request before the disk service existed |
| `recomp-title-disk-device-01/generation.json` | Byte/decoder/layout/memory admission and explicit data binding for the 316-function selection |
| `recomp-build-disk-device-01/build.json` | Successful build; SHA-256 identities of all 32 compiled source/declaration inputs |
| `disk-cache-browser-20261102-02.json`, `disk-cache-browser-20261102-03.json` | Final original browser runs for explicit settings zero and one, including corrected disk-transfer annotations and the next partition request |
| `disk-cache-checkpoint-20261102-01.json` | Report/module/source identities, identical cache payloads, handle closure, read-only kernel data and the common pending boundary |
| `local-disk-owned-03/checks.json` | Optimized and address/undefined-behavior sanitizer fixtures pass for source-owned disk cases |

The owned disk fixtures cover sparse round trips above 4 GiB, partial reads/end-of-file, close/reopen identity and backing lifetime, sharing and write permissions, rejected callbacks and guest memory, zero-page reclamation, capacity failure and unchanged disk/position on rejected writes. Earlier failed fixture attempts remain local; the corrected fixtures respect the existing fixed-page mapping contract. Targeted disk-profile/data-import checks pass. All 26 Python admission/memory cases pass. Of 35 runtime/admission/delivery cases, 33 pass under the normal runner; the two local-server cases initially encounter Windows loopback `EACCES` and pass on their exact scoped repeat with loopback access. These are infrastructure checks, not gameplay acceptance. The actual original execution above is separate browser evidence.

## Inferences and unknowns

The observed path has progressed beyond the CRT constructor table into original application setup. The fresh-disk cache-selection behavior explains why partition 5 is requested. Neither finding proves the rendering path, a controllable menu, a minigame or exact original timing.

Cache partition views, geometry/formatting, utility-drive mount completion, mounted disc/package reads, durable saves, scheduling/waits/interrupts/general exception dispatch, complete floating-point behavior, graphics/audio/controller integration, browser-local end-user recompilation and all planned online/social/control/authority features remain unfinished. Original gameplay/catalog/presentation fidelity and target-browser performance remain unaccepted. Earlier G0/G1 proofs and the approximate G2 prototype do not substitute for these requirements.

## Next concrete task

Implement the actually requested cache-partition view using an explicit synthetic partition layout and real bounded backing. Preserve separate partition identity, access/sharing, sector alignment, offsets and lifetime. Then follow the original formatting routine's actual next request; supply geometry, information or block I/O only under its established contract. Verify its genuine return and the original utility-drive link before claiming a mount. Do not infer a partition layout from the test count, return fake formatting success, prefill the original cache record or acquire console firmware/disk images.

Reuse the current translation, existing exported `0x00033ddb` routine and analysis project; no additional extraction/import/export is needed to begin this runtime task. Keep every original-game/custom-feature requirement and the local-content boundary intact.
