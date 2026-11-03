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

## Next task

Admit and translate the reached `0x001fe05a` from a bounded read-only export, then observe its next genuine kernel or translation dependency. Preserve byte/decoder/layout/memory admission and the existing budgets; do not substitute a graphics/D3D result. The ordinal-166 service is bounded to this identity-mapped window and still has no physical-address space separate from guest VA; record any new address/permission semantics explicitly.
