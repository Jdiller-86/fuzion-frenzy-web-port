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

`python3 tests/title_seed_test.py` passes 25 tests (including the new WBINVD admission and rejection cases). `npm run check` and `npm run test:runtime` (40/40) pass. No public CI result is claimed.

## Remaining boundaries

- No contiguous/physical memory model and no ordinal-166 service; the graphics cluster, D3D/device state and every rendering path remain unimplemented.
- The WBINVD model has no ordering, cache, TLB, device-visibility or timing semantics and admits exactly one site.
- General FATX files/writes/persistence, disc reads, scheduling/preemption, exceptions, audio, controllers, playability, end-user conversion and every planned online/social/control/authority feature remain unfinished.

## Next task

Implement ordinal 166, `MmAllocateContiguousMemoryEx`, following the reached request: 0x60 bytes, lowest 0, highest 0x3FFAFFF, alignment 0, protect 4. Model contiguous physical page ownership consistently with `MmQueryStatistics`, return a real guest address or a checked failure, and record output/failure atomicity. Do not return the request's highest address, a Wasm pointer or an invented success. Then continue `0x001fbb00` and follow its next genuine dependency. Reuse `recomp-title-post-memory-05`, the new build and the retained analysis project.
