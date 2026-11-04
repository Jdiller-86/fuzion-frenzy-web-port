# The reset path reaches AvSendTVEncoderOption; no encoder service is modeled

November 4, 2026. The narrow admission of `0x1FD42F` and its direct dependency `0x1FD410` advanced the reset run to a new genuine stop: kernel ordinal 2, `AvSendTVEncoderOption`, at target `0xFE000094`, return `0x1FD429`. `0x1FD410` (`CMiniport_GetDisplayCapabilities`) calls the IAT slot `0x00253450`, which the retained profile maps to ordinal 2. The harness has no encoder model, so execution stops with status 1 (unsupported Xbox service) instead of guessing the encoder's behaviour. **There is no display model, renderer, timing source or playable screen.**

## Narrow admission

- Translation `recomp-title-recomp-title-post-memory-49`: **418 functions / 17,353 instructions**.
- Added `0x1FD42F` (49 instructions; calls `0x1FD410`) and `0x1FD410` (10 instructions; calls ordinal 2 through the IAT).
- One merged seed within the existing 32-export budget, recomposed with `--guard-memory`.
- `tools/verify-seeds.py`: 32 recorded / 32 offered / 0 unexpected / 0 missing.
- `tools/verify-translation.py`: 0 functions needing rewriting.
- Module `recomp-post-memory-build-70`, SHA-256 `6bc97c4dab8f09f2b2135dd3042ab064492fbebaec97710bca870c2dde9d0629`.

## Measured stop

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pcrtc` control | status 15 at `0xFD003214` | 276 | 403 | 32,768 |
| `pfifoReset` | **status 1 at `0xFE000094`** (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |

The reset run still reports `pfifoStatus: { cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`.

## What this establishes

- The two admitted helpers execute in the original path; the PFIFO reset path is no longer the blocker for this branch.
- The next dependency is a kernel service, not an untranslated original routine.
- The retained callers evidence names ordinal 2 as `AvSendTVEncoderOption`; the run's stack identifies the call site but the exact option value and encoder semantics are not assigned here.

## Limits

- No encoder capabilities, mode, timing or return values are guessed.
- No display timing, renderer, fidelity or playability is established.
- One helper and its direct dependency were added; no broad closure or frontier batch.

## Next task

Decide whether to model a bounded, evidence-backed `AvSendTVEncoderOption` service (arguments, return values, supported options) or take another independent prerequisite. Do not infer encoder behaviour or display timing from the call alone. Keep the canonical profile fail-closed and the geometry lane paused.
