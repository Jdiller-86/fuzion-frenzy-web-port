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

The pinned public bridge models ordinal 168 as returning the top of the contiguous window minus the requested bytes and writing zero to the padding output; its 64 MiB window at `0x80000000` is not this project's window.

### Claim GPU instance memory

Using the same translation with the source-owned `gpu_instance` service and expanded aperture, module `recomp-post-memory-build-25` is 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. In the browser worker:

- all 83 initialization callbacks returned;
- 238 distinct original routines executed and 400 kernel operations completed;
- ordinal 168 returned status 0 with base `0x03B78000` and zero padding;
- the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`;
- the aperture retained 5,142 entries and 5,152 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FDF70`** (return `0x1FFF1A`, status 2). The report is `research/verification/post-memory-browser-20261103-07.json` (local/ignored). There is no playable screen.

The service claims the topmost free span through the bounded contiguous allocator rather than copying the bridge's `0x80000000` window; it validates padding before allocation and writes zero only after a successful claim. GPU-instance and device-register tests pass optimized and ASan/UBSan.

### Execute 0x1FDF70

Using the retained 26-seed selection plus `recomp-post-memory-next-09/seed.json`, translation `recomp-title-post-memory-13` contains **349 functions / 13,191 instructions**, including `ff_title_001FDF70`. Module `recomp-post-memory-build-26` is 467,100 bytes, SHA-256 `240b0bcd859b5c834b48059052a947b85e824c5633fefde93afd571db7ea0012`. In the browser worker:

- all 83 initialization callbacks returned;
- 239 distinct original routines executed and 400 kernel operations completed;
- `0x001fdf70` executed and returned to `0x001fff1a`;
- `0x001ffe27` returned to `0x001fe0c6`, which called `0x001fff25`;
- the aperture remained at 5,142 entries / 5,152 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FFF25`** (return `0x1FE1A0`, status 2). The report is `research/verification/post-memory-browser-20261103-08.json` (local/ignored). There is no playable screen.

### Execute 0x1FFF25

Using the retained 27-seed selection plus `recomp-post-memory-next-10/seed.json`, translation `recomp-title-post-memory-14` contains **350 functions / 13,240 instructions**, including `ff_title_001FFF25`. Module `recomp-post-memory-build-27` is 469,345 bytes, SHA-256 `5c3f84334ea08d5964e5bf8f2000e9d4aabe5c7c0cc61a32556d69c41f9103cf`. In the browser worker:

- all 83 initialization callbacks returned;
- 240 distinct original routines executed and 400 kernel operations completed;
- `0x001fff25` executed through its device-register byte/dword sequence and returned to `0x001fe1a0`;
- `0x001fe0c6` continued and called `0x001fdf83`;
- the aperture retained 5,148 entries and 5,176 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FDF83`** (return `0x1FE1A9`, status 2). The report is `research/verification/post-memory-browser-20261103-09.json` (local/ignored). There is no playable screen.

The aperture now stages 1/2-byte writes into known dword registers, merges the bytes, records the written sub-dword value and fails closed for unknown containing registers. Device-register tests pass optimized and ASan/UBSan.

### Execute 0x1FDF83

Using the retained 28-seed selection plus `recomp-post-memory-next-11/seed.json`, translation `recomp-title-post-memory-15` contains **351 functions / 13,262 instructions**, including `ff_title_001FDF83`. Module `recomp-post-memory-build-28` is 470,384 bytes, SHA-256 `7882a2557f9dd38eead06643b913fc739c23f7f60d8b1855bef267ae4862e495`. In the browser worker:

- all 83 initialization callbacks returned;
- 241 distinct original routines executed and 400 kernel operations completed;
- `0x001fdf83` initialized its 0x300-byte guest-memory table and returned to `0x001fe1a9`;
- `0x001fe0c6` continued and called `0x001ffff4`;
- the aperture remained at 5,148 entries / 5,176 events because the table writes went to guest RAM;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FFFF4`** (return `0x1FE1B0`, status 2). The report is `research/verification/post-memory-browser-20261103-10.json` (local/ignored). There is no playable screen.

The aperture now tracks per-byte knownness for sub-dword writes, allowing an unknown dword entry to be extended without fabricating untouched bytes; reads still require every accessed byte to be known. Device-register tests pass optimized and ASan/UBSan.

### Execute 0x1FFFF4

Using the retained 29-seed selection plus `recomp-post-memory-next-12/seed.json`, translation `recomp-title-post-memory-16` contains **352 functions / 13,280 instructions**, including `ff_title_001FFFF4`. Module `recomp-post-memory-build-29` is 471,053 bytes, SHA-256 `be6a6d13729484552f7a96ab7c6fc22584792670ed18f18c157652da1eff77b7`. In the browser worker:

- all 83 initialization callbacks returned;
- 242 distinct original routines executed and 400 kernel operations completed;
- `0x001ffff4` programmed twelve device-register dwords and returned to `0x001fe1b0`;
- `0x001fe0c6` set `[ESI+0x134]=2` and called `0x0020002a`;
- the aperture retained 5,160 entries / 5,188 events, with new writes at `0xFD008910`–`0xFD008944`;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x20002A`** (return `0x1FE1C1`, status 2). The report is `research/verification/post-memory-browser-20261103-11.json` (local/ignored). There is no playable screen.

### Execute 0x20002A

Using the retained 30-seed selection plus `recomp-post-memory-next-13/seed.json`, translation `recomp-title-post-memory-17` contains **353 functions / 13,295 instructions**, including `ff_title_0020002A`. Module `recomp-post-memory-build-30` is 471,504 bytes, SHA-256 `50985a0066a3055eb74dbc986679fa0b331701e59559653cb5a169a2e8f9516c`. In the browser worker:

- all 83 initialization callbacks returned;
- 243 distinct original routines executed and 400 kernel operations completed;
- `0x0020002a` wrote `0xFD400780` and cleared `0xFD700000`/`0xFD700004`, then returned to `0x001fe1c1`;
- `0x001fe0c6` continued and called `0x0020005a`;
- the aperture retained 5,161 entries / 5,191 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x20005A`** (return `0x1FE1C8`, status 2). The report is `research/verification/post-memory-browser-20261103-12.json` (local/ignored). There is no playable screen.

### Execute 0x20005A

Using the retained 31-seed selection plus `recomp-post-memory-next-14/seed.json`, translation `recomp-title-post-memory-18` contains **354 functions / 13,314 instructions**, including `ff_title_0020005A`. Module `recomp-post-memory-build-31` is 473,226 bytes, SHA-256 `ec7a4d9c39d7cf61413198cd8ab25b43a50a40caf7d79625362aa8bb7e92a4ca`. In the browser worker:

- all 83 initialization callbacks returned;
- 244 distinct original routines executed and 400 kernel operations completed;
- `0x0020005a` programmed object/device fields and returned to `0x001fe1c8`;
- `0x001fe0c6` returned to its caller at `0x1FE616`, which reached `0x001fe1d0`;
- the aperture retained 5,173 entries / 5,203 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x1FE1D0`** (return `0x1FE6ED`, status 2). The report is `research/verification/post-memory-browser-20261103-13.json` (local/ignored). There is no playable screen.

The link required a source-owned bounded port-output log (`src/runtime/port_io.c`), which records and ignores OUT writes; it is not a port device model. Its optimized and ASan/UBSan tests pass.

### Enter 0x1FE1D0

The retained next-14/15 exports were combined into one private seed to stay within the 32-export admission limit. Translation `recomp-title-post-memory-19` contains **355 functions / 13,401 instructions**, including `ff_title_001FE1D0`. Module `recomp-post-memory-build-32` is 473,896 bytes, SHA-256 `6ceba6b62364277056056fb6103ec723c7f26a8a5a53c0c2b3bcf0dbd30dc459`. In the browser worker:

- all 83 initialization callbacks returned;
- 245 distinct original routines executed and 400 kernel operations completed;
- `0x001fe1d0` read/modified `0xFD00184C` and called `0x00200340`;
- the aperture retained 5,174 entries / 5,208 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x200340`** (return `0x1FE210`, status 2). The report is `research/verification/post-memory-browser-20261103-14.json` (local/ignored). There is no playable screen.

The ordinal-305 kernel binding is admitted but not reached. The helper's remaining static path still contains `0x2010FD`, `0x20107C`, `KeQuerySystemTime` and `RtlTimeToTimeFields`.

### Execute 0x200340 and convert system time

The retained next-14/15/16 functions are combined into one private seed to stay within the 32-export admission limit. Translation `recomp-title-post-memory-20` contains **356 functions / 13,407 instructions**, including `ff_title_00200340`. Module `recomp-post-memory-build-33` is 476,718 bytes, SHA-256 `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670`. In the browser worker:

- all 83 initialization callbacks returned;
- 246 distinct original routines executed and 402 kernel operations completed;
- `0x00200340` wrote `1` to `0xFD600100` and `0xFD600140`;
- ordinal 128 returned system ticks `134381953697190000`;
- ordinal 305 returned `[2026, 11, 3, 16, 2, 49, 719, 2]`;
- `0x001fe1d0` continued and called `0x002010fd`;
- the aperture retained 5,178 entries / 5,213 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x2010FD`** (return `0x1FE2D2`, status 2). The report is `research/verification/post-memory-browser-20261103-15.json` (local/ignored). There is no playable screen.

The source-owned `time_fields` conversion is deterministic and uses no host timezone, locale or wall clock. Tests cover the 1601/1970 epochs, the 2024 leap day, the non-leap 1900 boundary, out-of-range input and null rejection.

### Enter 0x2010FD

The retained next-14–17 functions are combined into one private seed to stay within the 32-export admission limit. Translation `recomp-title-post-memory-21` contains **357 functions / 13,548 instructions**, including `ff_title_002010FD`. Module `recomp-post-memory-build-34` is 478,501 bytes, SHA-256 `2de725959fb619a572328bf89ef0374952bd8ef1233e81b67bb5dfae7b38a40d`. In the browser worker:

- all 83 initialization callbacks returned;
- 247 distinct original routines executed and 402 kernel operations completed;
- `0x002010fd` programmed its device-register block and called `0x002000cb`;
- the aperture retained 5,189 entries / 5,230 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x2000CB`** (return `0x201235`, status 2). The report is `research/verification/post-memory-browser-20261103-16.json` (local/ignored). There is no playable screen.

### Execute 0x2000CB and reach the device-table boundary

The retained next-14–18 functions are combined into one private seed to stay within the 32-export admission limit. Translation `recomp-title-post-memory-22` contains **358 functions / 13,571 instructions**, including `ff_title_002000CB`. Module `recomp-post-memory-build-35` is 481,279 bytes, SHA-256 `3dc7bfd70e4cdffa43ee681d851339a74ab951658819d30df45cf7901173676f`. In the browser worker:

- all 83 initialization callbacks returned;
- 248 distinct original routines executed and 402 kernel operations completed;
- `0x002000cb` read `0xFD400700` = `0` and returned without calling `0x1FDAC0`/`0x1FD700`;
- `0x002010fd` continued its device-register table and reached unsupplied `0xFD100284`;
- the aperture retained 5,232 entries / 5,279 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at device read **`0xFD100284`** (status 10, ordinal 1). The report is `research/verification/post-memory-browser-20261103-17.json` (local/ignored). There is no playable screen.

The next access is a device read in `0x2010FD`, not a new original routine. `0x200F59` on the `0x2010FD` path and `0x1FDAC0`/`0x1FD700` on the `0x2000CB` non-zero branch remain static dependencies.

### Serve the device table and reach 0x200F59

Added a bounded source-owned zero-range device model (up to four explicit ranges). Translation `recomp-title-post-memory-22` remains **358 functions / 13,571 instructions**. Module `recomp-post-memory-build-35` was rebuilt with zero-range support and is 481,870 bytes, SHA-256 `bc77a4cede489adcafe427b099f0b17d9bcd85fe2c14c3b282ccd916a6218f75`. In the browser worker:

- all 83 initialization callbacks returned;
- 248 distinct original routines executed and 402 kernel operations completed;
- `0x002000cb` read `0xFD400700` = `0` and returned;
- `0x002010fd` served its device-table reads through the explicit 0x1000-byte zero range at `0xFD100200` and called `0x00200f59`;
- the aperture retained 5,275 entries / 5,377 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x200F59`** (return `0x201389`, status 2). The report is `research/verification/post-memory-browser-20261103-18.json` (local/ignored). There is no playable screen.

Reads inside the zero range return zero without creating an entry, writes override the range, and out-of-range reads still fault. C, parser and report tests cover the model.

### Execute 0x200F59

Translation `recomp-title-post-memory-23` contains **359 functions / 13,649 instructions**, including `ff_title_00200F59`. Module `recomp-post-memory-build-36` is 484,657 bytes, SHA-256 `91afe5cb2fd8a6a79196e96b2b30b9db4f1a72c7c4bed64dfca569e9fee03d66`. In the browser worker:

- all 83 initialization callbacks returned;
- 249 distinct original routines executed and 402 kernel operations completed;
- `0x00200f59` read `0xFD400100` = `0` and skipped `0x1FDAC0`;
- it called `0x2000CB` twice; each call read `0xFD400700` = `0` and returned;
- the `0x20010A` branch was not taken;
- the aperture retained 5,277 entries / 5,386 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x20107C`** (return `0x1FE2ED`, status 2). The report is `research/verification/post-memory-browser-20261103-19.json` (local/ignored). There is no playable screen.

### Execute 0x20107C

Translation `recomp-title-post-memory-24` contains **360 functions / 13,682 instructions**, including `ff_title_0020107C`. Module `recomp-post-memory-build-37` is 485,351 bytes, SHA-256 `e64070431da915dc10e9e6e9c7fb37aebd58ee88bce12620c5e545c0229886de`. In the browser worker:

- all 83 initialization callbacks returned;
- 250 distinct original routines executed and 402 kernel operations completed;
- `0x0020107c` programmed its device fields and called `0x00200143`;
- the aperture retained 5,286 entries / 5,395 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x200143`** (return `0x2010D8`, status 2). The report is `research/verification/post-memory-browser-20261103-20.json` (local/ignored). There is no playable screen.

### Execute 0x200143, 0x1FE30B and 0x200352

Translation `recomp-title-post-memory-27` contains **363 functions / 13,956 instructions**. Module `recomp-post-memory-build-41` is 499,386 bytes, SHA-256 `1f38b342759b5f13bda9ca2da43380056308b0a2d34fec1a86484b1f5398b861`. In the browser worker:

- all 83 initialization callbacks returned;
- 256 distinct original routines executed and 402 kernel operations completed;
- `0x00200143` saved/restored its PGRAPH context (34 device events), `0x001FE30B` classified a mapping value, and `0x00200352` cleared 0xDFB dwords (3,580 writes) before programming its device fields;
- `0x001FE026` then executed and the caller requested `0x002004DF`;
- the aperture retained 5,300 entries / 9,130 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

In the first `0x200143` attempt the aperture stopped at unsupplied `0xFD003204`; the selected synthetic profile now declares a third zero range at `0xFD003200` (0x100 bytes). The second bulk clear in `0x00200352` then filled the fixed 8,192-event log, so `FF_DEVICE_EVENTS` and `DEVICE_EVENTS` were raised to 32,768 ordered events. Reports `post-memory-browser-20261103-21.json` … `-24.json` (local/ignored). There is no playable screen.

### Execute 0x2004DF

Translation `recomp-title-post-memory-28` contains **364 functions / 13,976 instructions**, including `ff_title_002004DF`. Module `recomp-post-memory-build-42` is 501,401 bytes, SHA-256 `0e13a564d67cd16e0c332545e8b8aba97f70ec6c1ab067e8570829425a1ff3ec`. In the browser worker:

- all 83 initialization callbacks returned;
- 258 distinct original routines executed and 402 kernel operations completed;
- `0x002004DF` wrote 22 index/value dwords into `0xFD700010`–`0xFD700064`;
- `0x001FE43A` then executed without device accesses and called `0x0020057A`;
- the aperture retained 5,300 entries / 9,152 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x20057A`** (return `0x1FE499`, status 2). The report is `research/verification/post-memory-browser-20261103-25.json` (local/ignored). There is no playable screen.

### Execute 0x20057A

Translation `recomp-title-post-memory-29` contains **365 functions / 14,544 instructions**, including `ff_title_0020057A`. Module `recomp-post-memory-build-43` is 520,818 bytes, SHA-256 `2752487f5a25ed6d5387d0a3e9d899003ede883321f997f43a1e8ae0f1b975b6`. In the browser worker:

- all 83 initialization callbacks returned;
- 259 distinct original routines executed and 402 kernel operations completed;
- `0x0020057A` programmed large `0xFDxxxxxx` register/context blocks with immediate constants and counted loops, contributing 3,344 device writes (138 events in its entry block, the rest across its block sites);
- every write reused the existing `0xFD700000`–`0xFD704FFC` entries, so the entry count stayed at 5,300;
- the aperture retained 5,300 entries / 12,496 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0x200518`** (return `0x1FE4A4`, status 2). The report is `research/verification/post-memory-browser-20261103-26.json` (local/ignored). There is no playable screen.

### Execute 0x200518, model the 0x80000000 mirror write, and bound the 0xFD100410 poll

Translation `recomp-title-post-memory-30` contains **366 functions / 14,572 instructions**, including `ff_title_00200518`. Module `recomp-post-memory-build-46` is 524,523 bytes, SHA-256 `cc4d7c74358a354ca3b44641aa2bc8731eda96fe9ce6c82cb942f9c9dc4d318b`. In the browser worker:

- all 83 initialization callbacks returned;
- 261 distinct original routines executed and 402 kernel operations completed;
- `0x00200518` wrote its four device dwords, then execution entered `0x001FAED0` and block `0x1FAF10`;
- the original `MOV dword ptr [0x80000000], EDX` followed by `WBINVD` completed through the bounded 0x1000-byte mirror page; the virtual-memory fault slot is clear;
- `0x001FAED0` read `0xFD100410` (value `0x10000`) 16 times, then the new poll limit stopped the run;
- the aperture retained 5,301 entries / 12,539 events and 74,065 checked guest accesses;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at device address **`0xFD100410`** with status 12 (`device poll limit reached`), return `0x203B90`. The report is `research/verification/post-memory-browser-20261103-28.json` (local/ignored). A prior run without the limit produced 20,246 identical reads, exhausted the 32,768-event log and exceeded the browser report save path. There is no playable screen.

### Model NV_PFB_WBC and reach the NV2A USER region

Translation `recomp-title-post-memory-30` contains **366 functions / 14,572 instructions**. Module `recomp-post-memory-build-47` is 524,891 bytes, SHA-256 `efb0fbea1c67c44c3a904b0da9294120cdf3f3439909e639e3f0866b7b76e27e`. In the browser worker:

- all 83 initialization callbacks returned;
- 261 distinct original routines executed and 402 kernel operations completed;
- `0x001FAED0` read `0xFD100410` = `0`, wrote `0x10000` (`NV_PFB_WBC_FLUSH`), and its poll read back `0`, so the flush completed after one iteration;
- the aperture retained 5,301 entries / 12,523 events;
- execution then wrote `0xFD800040`, outside the current 8 MiB window;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0xFD800040`** (status 10, `guest memory access stopped`, return `0x203B90`). The report is `research/verification/post-memory-browser-20261103-29.json` (local/ignored). The self-clearing model follows the pinned xemu `NV_PFB_WBC` read/write behavior and covers only the flush bit. There is no playable screen.

### Extend the aperture window to the NV2A USER region

`local-title-proof.mjs` now admits an explicit synthetic window of at most `0x1000000` bytes, and profile `settings-mmio-20.json` selects `0xFD000000` / `0x1000000` with the same 64 registers and a new bounded USER page at `0xFD800000` (0x1000). Module `recomp-post-memory-build-47` is unchanged (524,891 bytes, SHA-256 `efb0fbea1c67c44c3a904b0da9294120cdf3f3439909e639e3f0866b7b76e27e`). In the browser worker:

- all 83 initialization callbacks returned;
- 262 distinct original routines executed and 402 kernel operations completed;
- execution wrote `0xFD800040` and entered `0x001FACA0` (new);
- loop `0x1FBECD` compared the context value with `0xFD800044` (USER page reads zero) under mask `0x0FFFFFFF`;
- the aperture retained 5,302 entries / 12,542 events;
- after 16 identical reads of `0xFD800044` the poll limit stopped the run;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0xFD800044`** with status 12 (`device poll limit reached`). The report is `research/verification/post-memory-browser-20261103-30.json` (local/ignored). There is no playable screen.

### Identify the PFIFO DMA submission boundary

`src/runtime/pfifo_dma.c`/`.h` classify the reached channel-0 USER-region PFIFO registers from the pinned xemu model: `NV_USER_DMA_PUT` (`0xFD800040`), `NV_USER_DMA_GET` (`0xFD800044`) and `NV_USER_REF` (`0xFD800048`). The probe compares a GET read with the aperture's PUT and stops with a dedicated status instead of waiting for a FIFO consumer.

Translation `recomp-title-post-memory-30` contains **366 functions / 14,572 instructions**. Module `recomp-post-memory-build-48` is 525,113 bytes, SHA-256 `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b`. In the browser worker:

- all 83 initialization callbacks returned;
- 261 distinct original routines executed and 402 kernel operations completed;
- `0x001FAED0` wrote DMA_PUT = `0x03B7D000` at `0xFD800040`;
- the first DMA_GET read at `0xFD800044` returned `0`, so the put/get comparison could not match;
- the aperture retained 5,302 entries / 12,524 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution stopped at **`0xFD800044`** with status 13 (`pushbuffer consumption required`). The report is `research/verification/post-memory-browser-20261103-31.json` (local/ignored). No get pointer, FIFO progress, interrupt, DMA or rendering is fabricated. There is no playable screen.

### Add a bounded PFIFO submission snapshot

An architect escalation approved observation only. `src/runtime/submission-probe.mjs` builds a bounded read-only snapshot at the status-13 stop: channel, PUT/GET, base and at most 256 bytes of checked guest memory ending at the cursor, with explicit unavailable reasons. The worker adds `submissionProbe`; the source-only proof host serves the module; `tests/submission-probe.test.mjs` covers the window, read-only behavior and bounds.

Module `recomp-post-memory-build-48` is unchanged (525,113 bytes, SHA-256 `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b`). In the browser worker:

- all 83 initialization callbacks returned;
- 261 distinct original routines executed and 402 kernel operations completed;
- `submissionProbe` is available for channel 0 with PUT `0x03B7D000`, GET `0`, base `0x03B7CF00` and 256 bytes;
- all 64 captured words are zero, so the cursor alone does not identify queued commands;
- the aperture retained 5,302 entries / 12,524 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution still stops at **`0xFD800044`** with status 13. The report is `research/verification/post-memory-browser-20261103-32.json` (local/ignored). The snapshot is a memory view, not decoded commands, and does not advance DMA_GET.

### Add the observed allocation-base window

`submission-probe.mjs` now also accepts the observed ordinal-168 GPU-instance claim and captures a second bounded read-only window at that base. `thread-proof-worker.mjs` extracts the claim; tests cover both windows, short captures, out-of-range bases and read-only behavior.

Module `recomp-post-memory-build-48` is unchanged (525,113 bytes, SHA-256 `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b`). In the run:

- all 83 initialization callbacks returned; 261 routines and 402 kernel operations;
- `submissionProbe` reports PUT `0x03B7D000`, GET `0`, before-window base `0x03B7CF00` and allocation base `0x03B78000`, each 256 bytes with **0 nonzero words**;
- the same run's device aperture retains 5,120 entries at `0xFD700000`–`0xFD704FFC`, **559 nonzero**;
- the aperture overall logged 5,302 entries / 12,524 events;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

Execution still stops at **`0xFD800044`** with status 13. The report is `research/verification/post-memory-browser-20261103-33.json` (local/ignored). The command/context data is in the separate device-aperture backing, not at the guest physical addresses the cursor names; no consumer or translation is implemented.

### Decode the PFIFO DMA object

`submission-probe.mjs` now decodes the DMA object from the observed aperture entries using the pinned xemu semantics: `NV_PFIFO_CACHE1_DMA_INSTANCE` at `0xFD00322C`, object address `(value & 0xFFFF) << 4`, and the RAMIN object's flags/limit/frame at `0xFD700000 + objectAddress`. Tests cover complete, missing, incomplete and masked objects.

Module `recomp-post-memory-build-48` is unchanged (525,113 bytes, SHA-256 `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b`). In the run:

- instance register `0x11C` → object address `0x11C0`;
- flags `0xB002` (class 2, target NVM/VRAM, adjust 0), base `0`, limit `0x3FFAFFF`;
- DMA_PUT `0x03B7D000` and DMA_GET `0` are offsets into that ~64 MiB VRAM-target object;
- the 256-byte guest-RAM windows at the raw cursor and allocation base remain zero and are not the mapped buffer;
- the run still has 261 routines / 402 kernel operations, stop status 13 at `0xFD800044`;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

The report is `research/verification/post-memory-browser-20261103-34.json` (local/ignored). The proof has no VRAM target buffer, so no queue bytes are decoded and no consumer is implemented.

### Resolve DMA offsets without claiming backing

Architect escalation recommended proving target backing and queue provenance before allocating VRAM or consuming. `submission-probe.mjs` now resolves PUT/GET to candidate guest-physical addresses only when the object is NVM/VRAM, the offset is aligned and within the object limit, and base+offset stays inside modeled guest RAM. Every candidate is reported with `verified:false` and backing unverified; no bytes are returned and GET never advances.

Module `recomp-post-memory-build-48` is unchanged (525,113 bytes, SHA-256 `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b`). In the run:

- `putResolved` candidate `0x03B7D000`; `getResolved` candidate `0`, both unverified;
- the guest-RAM windows at the raw cursor and allocation base remain zero, so producer-written queue data is not established;
- the run still has 261 routines / 402 kernel operations and stops at status 13 (`pushbuffer consumption required`) at `0xFD800044`;
- memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

The report is `research/verification/post-memory-browser-20261103-35.json` (local/ignored). No VRAM was allocated, no consumer was added and no get pointer, packet, interrupt, DMA or rendering is fabricated.

## Next task

Verify the NVM/VRAM target backing and initial GET provenance against pinned sources and the physical-memory contract: whether the target aliases guest physical RAM, where the producer wrote the queue, and what initial GET means. If backing cannot be established, record the unresolved mapping and re-plan only on explicit evidence. Preserve the explicit synthetic registers, the 16 MiB window with its four zero ranges, the 8,192-entry / 32,768-event aperture, the mirror page, the 16-read poll limit, the single self-clearing flush bit, the PFIFO register/object map, the bounded submission windows and the fail-closed resolver; do not fabricate the get pointer, command decoding, interrupt delivery, DMA or rendering. The captured line/quad reference comparison and browser prepass also remain private.
