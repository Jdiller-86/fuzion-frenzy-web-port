# Original graphics initialization requests contiguous kernel memory

November 2, 2026. Original code now runs its 640×480 video-mode setup through `0x001f7650`, enters the graphics/device cluster at `0x001fbb00`, and stops at its first genuine kernel call: ordinal 166, `MmAllocateContiguousMemoryEx`. The modeled runtime dispatches no result. **There is no renderer, no playable screen and no general kernel.** The complete original-game/custom-feature goal remains active.

## Observed execution

Both explicit fresh synthetic profiles return from all 83 initialization callbacks, execute **231** distinct original routines and complete **393** kernel operations across **3,628** events. `MmQueryStatistics` still returns `[36, 16384, 14567, 16384, 1048576, 0, 1008, 16, 789]`.

1. `0x001f7650` runs its prologue: global limits `0x80000` (at `0x00206958`) and `0x8000` (at `0x00206954`), list heads at `0x00203b90`, flag `0x00203f98 = 1`, and the mode value 0x10 folded into `0x00203b9c`.
2. It calls `0x001fbb00` with the 640×480 / format-7 state structure.
3. `0x001fbb00` loads the kernel import slot `0x00253444` (ordinal 166) into `ESI`, pushes `0x60`, `0`, `0x3ffafff`, `0`, `4`, and executes `CALL ESI` at `0x001fbb1e`.
4. Execution stops with status 1 (`unsupported Xbox service`) at target `0xfe00008c`, ordinal 166, return address `0x001fbb20` and stack arguments `[96, 0, 67088383, 0, 4, …]`. The target is the kernel function pointer held in that slot; no guest result is invented.

The pinned runtime header identifies ordinal 166 as `MmAllocateContiguousMemoryEx`, whose five arguments are `NumberOfBytes`, `LowestAcceptableAddress`, `HighestAcceptableAddress`, `Alignment` and `Protect`. This request asks for 0x60 bytes with no lower bound, a highest acceptable address of 0x3FFAFFF, zero alignment and protect 4.

## WBINVD admission (D060)

`0x001fbb00` also contains `0F 09` (WBINVD) at `0x001fbea3`. The pinned lifter cannot translate it and its checkout must stay clean, so the owned prepare pipeline now admits exactly that site as a modeled no-op:

- the seed must declare exactly one `WBINVD` row, at `0x001fbea3`, length 2, Ghidra bytes `0f09`, no call/computed flags, and the original executable bytes at that address must be `0f09`;
- the translation gate accepts exactly `{'wbinvd': [0x001FBEA3]}` and fails on any other mnemonic, address or second site;
- the emitted `RECOMP_UNIMPL` marker is rewritten to the owned sink `FF_GUEST_WBINVD(0x001FBEA3u)` (default no-op, visible in generated C and still charged as one instruction);
- `generation.json` records `modeledNoOpInstructions` with the site, bytes, semantics and decision, and `tests/title_seed_test.py` rejects every neighbouring encoding, address, duplicate and gate variant.

An independent GLM 5.3 review accepted this with those conditions: Intel documents WBINVD as cache write-back/invalidation plus external-cache signaling and serialization with no register, flag or memory effect, and none of those effects has a witness in a single-core coherent guest-RAM model with no cache, TLB, device/DMA, SMP, interleaving interrupt or cycle-fidelity claim. The record must be re-decided, not extended, if any of those is introduced or a second WBINVD site appears. **The admitted site was not reached in this run** (execution stops at the earlier kernel call), so this is a static admission only so far.

The pipeline also now accepts the observed 7-byte no-effect `LEA ESP,[ESP+0]` (`8D A4 24 00 00 00 00`) that Ghidra reports as undefined inside `0x001fb580`; a non-zero displacement and the short form remain rejected.

## Evidence and checks

Private records under `research/verification/`:

| Record | Scope |
| --- | --- |
| `recomp-title-post-memory-05/` | 343 functions / 12,869 instructions from 22 seed groups, including `modeledNoOpInstructions` |
| `recomp-post-memory-build-05/` | ABI-23 module `e1df54c3…`, 1,018,070 bytes, 36 source identities |
| `post-memory-browser-20261102-07.json`, `-08.json` | Both profiles: 231 routines, 393 operations, 3,628 events, ordinal-166 stop |
| `recomp-post-memory-front-01/seed.json` | 24-function renderer-entry front (one function retained in its original seed only) |
| `recomp-post-memory-next-01/02/03/seed.json`, `recomp-post-memory-closure-01/` | Read-only exports for the reached boundary |
| `post-memory-front-checkpoint-20261102-01.json` | Independent identity audit of the front build and the `0x1fbb00` WBINVD bytes |
| `post-memory-browser-20261102-05.json`, `-06.json` | Previous boundary: stop at `0x001fbb00` entry before the WBINVD admission |
| `post-memory-browser-20261102-09.json`, `-10.json` | Ordinal-166 service: both profiles allocate 96 then 532,480 bytes, 233 routines / 396 operations, stop `0x001fe05a` return `0x001fe5ab`, consistent statistics |
| `contiguous-memory-checkpoint-20261102-01.json` | Build-07 module/source identities, both reports' ordinal-166/107 dispatch, stop and statistics facts; zero source mismatches |

`python3 tests/title_seed_test.py` passes 25 tests (including the new WBINVD admission and rejection cases). `npm run check` and `npm run test:runtime` (40/40) pass. No public CI result is claimed.

## Remaining boundaries

- No contiguous/physical memory model and no ordinal-166 service; the graphics cluster, D3D/device state and every rendering path remain unimplemented.
- The WBINVD model has no ordering, cache, TLB, device-visibility or timing semantics and admits exactly one site.
- General FATX files/writes/persistence, disc reads, scheduling/preemption, exceptions, audio, controllers, playability, end-user conversion and every planned online/social/control/authority feature remain unfinished.

## Contiguous-memory service and the new graphics boundary

`ff_vm_allocate_contiguous` implements ordinal 166 for the reached request: page-rounded, top-down first-fit inside `[lowest, highest]`, alignment honoured, checked span/ownership, a real `FF_VM_PRIVATE` region claimed and every page committed/zero-filled, with `base` written only on success. `permissions()` accepts the documented cache modifiers `PAGE_NOCACHE (0x400)` and `PAGE_WRITECOMBINE (0x800)` as no-ops over a known base protection and still rejects every other bit.

Both profiles against `recomp-post-memory-build-07` (module `00c1b2ed…`, 1,019,182 bytes) now service ordinal 166 twice (96 bytes, then 532,480 bytes) and continue to **233** routines, **396** kernel operations and 3,631 events. `MmQueryStatistics` reports `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`, i.e. 1,948 pages used = 1,813 fixed (pool/stack/image) + 135 committed, matching the two contiguous allocations. Ordinal 107 was also dispatched with status 0 by the existing binding; its identity is not established here.

Execution then stops at **`0x001fe05a`** (status 2, untranslated original routine) with return `0x001fe5ab`, having re-entered the graphics cluster. Reports: `post-memory-browser-20261102-09.json` and `-10.json`; audit: `contiguous-memory-checkpoint-20261102-01.json` (zero source-identity mismatches, module file matches the build record, both profiles identical).

## GPU register access boundary

Admitting `0x001fe05a` (43 bytes / 9 instructions, no calls) advanced execution to the first real device access. `recomp-title-post-memory-06` (344 functions / 12,878 instructions) and `recomp-post-memory-build-08` (module `f97390b4…`, 1,021,385 bytes) run both profiles to **234** routines / **396** kernel operations / 3,632 events, then stop with status 10 (checked guest-memory access violation) reading **4 bytes at `0xfd001804`** — the NV2A graphics register window, outside the modeled 64 MiB RAM. `ordinal` is 1 (the read access type) and the return address is `0x001fe5ab`. Memory statistics remain `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Reports: `post-memory-browser-20261102-11.json`, `-12.json`; audit: `gpu-boundary-checkpoint-20261102-01.json` (zero source-identity mismatches, matching module digest, identical profiles).

## Synthetic register storage and the bounded aperture (D063/D064)

Helper `0x001fe05a` performs a read-modify-write of `0xfd001804` and writes zero to `0xfd600140`; the follow-on code writes further offsets derived from the device base. A fixed per-address list could not scale, so the proof now admits a bounded **aperture** [`0xFD000000`, `0xFD800000`) whose base and size are explicit profile inputs (D064). Storage is sparse (1,024 addresses) with a 4,096-event ordered log; only aligned 4-byte accesses inside the window are admitted, **unknown reads fault instead of returning a fabricated zero**, writes establish storage and record whether the previous value was known, and capacity exhaustion fails before the access takes effect. Every other address, width or direction still stops.

Both profiles ran `recomp-post-memory-build-16` (module `d757d534…`, 424,106 bytes): four device events across three addresses (`0xfd001804` read 0, then a write whose resulting entry value is 4; `0xfd600140` written 0; `0xfd009140` first write, `beforeKnown = false`), 234 routines / 396 kernel operations / 3,633 events, memory statistics unchanged `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`. Execution then advanced past the device window and stopped at **untranslated original routine `0x1fe085`** (return `0x1fe5bd`).

Reports: `post-memory-browser-20261102-15.json`, `-16.json`; audit `aperture-checkpoint-20261102-01.json` (zero source mismatches, matching module digest, identical profiles).

## Basic-block provenance for checked memory accesses

`ff_probe_guest_memory` now takes the guest address of the translated basic block that performed the access. The emitter instrumentation (`tools/recomp_memory_access.py`) emits `FF_MEMORY_SITE_*` with the nearest preceding `loc_%08X:` label, using the function entry for a leading prologue access. The label convention was checked against the pinned public emitter source (`sp00nznet/xboxrecomp` commit `f0f8a46`, `tools/recomp/translator.py`): labels are emitted for the entry block and every block that is a successor or jump target, and blocks are emitted in address order. The instrumenter fails closed on a label below the function, descending labels, an access after a label at or beyond the function end, and a checked function with scalar accesses but no block labels.

`ff_probe_guest_memory` forwards the site into `ff_device_registers_touch`; `ff_probe_mmio_event(..., 4)` exposes it, and the browser worker records `site` on each device event. A device access with site zero now stops instead of being attributed to a fabricated routine. Zero remains the marker for stack and bulk helpers, which the aperture cannot route because it admits only aligned four-byte scalar accesses. The ABI remains 24; the site field is additive, and an older module returns `UINT32_MAX` for the new field, which the worker reports as `null`.

Validation: `python tests/recomp_memory_access_test.py` 8/8, `python tests/title_seed_test.py` 25/25, `npm run test:runtime` 40/40, `npm run check` (221 files) and whitespace checks pass. The owned transformed fixture passes optimized and ASan/UBSan native GCC in WSL, covering three labelled blocks with integer and float widths, a device read/write site, and rejection of a zero-site device access. **No original module was rebuilt or executed; this is not a new original-runtime observation.** The reached original boundary remains `0x1fe085`.

### Staged device writes

The aperture no longer records a write before the generated store. `ff_device_registers_stage_write` returns the lvalue that the store fills; `ff_device_registers_commit` records one write event with the stored value. `ff_device_registers_touch` and `ff_device_registers_access` commit a pending write first, so a later read observes it and event order is preserved. `ff_device_registers_access` maps READ, WRITE and READWRITE; the read half of a read-modify-write is recorded immediately and its write is staged. `ff_probe_guest_memory` commits before any later access, and the helper-return stop plus the event readers commit before reading counts.

`tests/device_registers_test.c` covers staged writes, consecutive writes, auto-commit before a following read, read-modify-write, unknown read halves, zero-site/invalid-mask rejection, and capacity atomicity. `tests/memory_access_test.c` exercises the same path through transformed guest accessors, including the split `movsd` form. `tools/recomp_memory_access.py` splits the pinned lifter's exact `movs` mem-to-mem forms and evaluates any other scalar write whose value expression probes memory into a temporary before the staged store; the Python memory-access suite is 10/10. The transform covers the observed `MEM32(ebp-3068) = MEM32(ebp-3068) & 0` form. Optimized and ASan/UBSan WSL runs pass.

No original module was rebuilt or run, so build-16's recorded write values remain the old probe's pre-store placeholders; no new browser observation is claimed. ABI 24 and D060 are unchanged.

### Complete device-aperture reporting

The proof worker previously copied only 16 aperture entries and 8 ordered events into its report, which could hide the next original register sequence. `src/runtime/device-report.mjs` now validates the module's counts against the 1,024-entry / 4,096-event capacities, copies every recorded entry and event, and maps the additive site field (`UINT32_MAX` to `null` for older modules). Node tests cover 17 entries / 9 events (beyond the old caps), site/null mapping, capacity rejection and missing accessors; `npm run test:runtime` is 44/44. This is an observer capability only: no original module was rebuilt or run, and ABI 24/D060 are unchanged.

### Explicit multi-register initial profiles

The aperture profile now accepts 2–64 distinct explicit initial entries, not only the two reached addresses. `ff_device_registers_init_many` validates base/width, alignment, window bounds and distinct addresses before mutation. `ff_device_registers_add_initial` seeds further values only while no access or staged write has been recorded. The probe exports `ff_probe_mmio_add`; the worker seeds entries beyond `0xfd001804`/`0xfd600140` and fails explicitly if the private module lacks that export. The parser still requires the two reached addresses and rejects duplicates, unaligned/out-of-window values and more than 64 entries. `device-report.mjs` now records the normalized supplied window/registers separately from observed entries/log, so a private run can distinguish profile inputs from reads/writes. Owned C and Node tests pass; `npm run test:runtime` is 47/47. Older ABI-24 modules remain compatible for exactly two entries; no module was rebuilt or run, and no register value is inferred.

### Post-memory execution: 0x1fe085 and interrupt initialization

Using the retained private seeds, the selected `0x001fe085` seed was admitted with the existing 22-seed selection. Translation `recomp-title-post-memory-09` contains **345 functions / 12,892 instructions**, including `ff_title_001FE085`. Module `recomp-post-memory-build-21` is 459,080 bytes, SHA-256 `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9`.

One explicit synthetic profile supplied zero as the test value for three newly observed reads: `0xFD001800`, `0xFD001808` and `0xFD10020C`. These are explicit test inputs, not console defaults. In the browser worker:

- all 83 initialization callbacks returned;
- 235 distinct original routines executed and 399 kernel operations completed;
- `0x001fe085` executed and returned to `0x1FE5CE`;
- the aperture logged seven events across six addresses, with sites `2089050` and `2089093`;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

The next reached services were modeled from the pinned public xboxrecomp bridge/ordinal table: ordinal 44 `HalGetInterruptVector`, ordinal 109 `KeInitializeInterrupt`, ordinal 98 `KeConnectInterrupt` and ordinal 47 `HalRegisterShutdownNotification`. The first returned vector 3 and wrote the IRQL byte; the second wrote a zeroed 44-byte KINTERRUPT with routine/context/vector at offsets 0/4/8; the third returned TRUE; the fourth accepted registration and dropped it. **No interrupt is delivered and no NV2A interrupt behavior is modeled.**

Execution then stopped at untranslated original routine **`0x1FE0C6`** (return `0x1FE616`, status 2) after 399 kernel operations. The report is `research/verification/post-memory-browser-20261103-02.json` (local/ignored).

### Execute 0x1FE0C6

Using the retained 23-seed selection plus `recomp-post-memory-next-06/seed.json`, translation `recomp-title-post-memory-10` contains **346 functions / 12,997 instructions**, including `ff_title_001FE0C6`. Module `recomp-post-memory-build-22` is 459,451 bytes, SHA-256 `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f`.

In the browser worker:

- all 83 initialization callbacks returned;
- 236 distinct original routines executed and 399 kernel operations completed;
- `0x001fe0c6` executed its prologue and two stores, then called untranslated `0x001ffcc0`;
- the aperture logged nine events across eight addresses, with sites `2089050`, `2089093` and `2089158`; the two new writes were `0xFD001830` = `0` and `0xFD00180C` = `0xF800`, both unknown-before;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FFCC0`** (return `0x1FE0E7`, status 2). The report is `research/verification/post-memory-browser-20261103-03.json` (local/ignored). There is no playable screen.

### Execute 0x1FFCC0

Using the retained 24-seed selection plus `recomp-post-memory-next-07/seed.json`, translation `recomp-title-post-memory-11` contains **347 functions / 13,113 instructions**, including `ff_title_001FFCC0`. Module `recomp-post-memory-build-23` is 464,158 bytes, SHA-256 `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d`.

The explicit synthetic profile also supplied zero for `0xFD000200`, `0xFD000140`, `0xFD680500`, `0xFD680504` and `0xFD680508`; these are test inputs, not console defaults. In the browser worker:

- all 83 initialization callbacks returned;
- 237 distinct original routines executed and 399 kernel operations completed;
- `0x001ffcc0` executed to completion and returned to `0x001fe0c6`;
- `0x001fe0c6` continued and called untranslated `0x001ffe27`;
- the aperture logged 25 events across 16 addresses, including sub-dword reads at `0xFD680505`/`0xFD680509`/`0xFD680501` and stores `0xFD009200` = `0`, `0xFD009210` = `1`, `0xFD009420` = `0xFFFFFFFF`;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FFE27`** (return `0x1FE199`, status 2). The report is `research/verification/post-memory-browser-20261103-05.json` (local/ignored). There is no playable screen.

The byte reads required a source-owned aperture extension: 1/2/4-byte reads inside the window now map into the containing dword, while sub-dword writes fail closed. Device-register tests pass optimized and ASan/UBSan.

### Execute 0x1FFE27

Using the retained 25-seed selection plus `recomp-post-memory-next-08/seed.json`, translation `recomp-title-post-memory-12` contains **348 functions / 13,185 instructions**, including `ff_title_001FFE27`. Module `recomp-post-memory-build-24` is 465,978 bytes, SHA-256 `67e7c9a5a560db00e8f3984e7561ed512555b5168f116dd6fb3ddcc8e390f72d`.

The explicit synthetic profile also supplied zero for `0xFD100200`, `0xFD100204` and `0xFD001218`; these are test inputs, not console defaults. In the browser worker:

- all 83 initialization callbacks returned;
- 238 distinct original routines executed and 399 kernel operations completed;
- `0x001ffe27` executed through its three device reads and reached its first kernel call;
- the aperture logged 28 events across 19 addresses; the new reads were `0xFD100200`, `0xFD100204` and `0xFD001218` at site `2096679`;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at ordinal **168 `MmClaimGpuInstanceMemory`** (status 1, return `0x1FFE76`, request `[0x5000, padding pointer]`). The report is `research/verification/post-memory-browser-20261103-06.json` (local/ignored). There is no playable screen.

The pinned public bridge models ordinal 168 as returning the top of the contiguous window minus the requested bytes and writing zero to the padding output; its 64 MiB window at `0x80000000` is not this project's window. The static path after the call clears 0x5000 bytes at `0xFD700000` plus the padding value; that clear has not executed, and the aperture's 1,024-entry / 4,096-event capacity needs an explicit decision.

## Next task

Implement and verify the bounded `MmClaimGpuInstanceMemory` service and the reached 0x5000-byte clear under an explicit capacity/bulk-write model, preserving the project's contiguous window and memory accounting. Then continue to `0x1FDF70`. Preserve the explicit synthetic register values and the modeled interrupt initialization limits; do not infer NV2A interrupt delivery, DMA, device behavior or rendering from the synthetic writes. The captured line/quad reference comparison and browser prepass also remain private.
