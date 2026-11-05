# The `0x1F6970` dependency batch advances the stop to `0x1F6620`

November 4, 2026. Provisional continuation after Path B: the observed stop `0x1F6970` was ordinary, so this cycle closed that one-function set. It is **not an approved design decision**; architect guidance remains `approved: false`. **There is no controllable original screen.**

## Selection and grouping

- `0x1F6970` seed: **35 instructions / a 119-byte instruction-only envelope**, SHA-256 `5d519e103331b673e0b93a5c912156b73eb86bdbbf28f6eda2d35a2c6c5d57e6`.
- Its only call is `0x1FB3B0`, already translated; no additional dependency seed was needed.
- Grouping: append to the merged 5-function seed → **6 functions**, SHA-256 `87f3ccab6c7d08e42a2b80e2f586c68358e5a34de457cebb7dc663c42c27e3ad`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-58`: **430 functions / 18,073 instructions**; `0x1F6970` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **520 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-79`: **688,315 bytes**, SHA-256 `8baa111d9e5a1505f719a2086ccdd5952eb9a7d833194d6118208791c3467e39`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6620`** | 300 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +21 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6970` executed; `0x1F6620` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **520 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6620` caller, so this is an observed/indirect boundary.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6970_PACKAGE_MANIFEST.json` reports 32/32 seeds, 430 admitted / 520 generated / 0 rewrites, 17/17 digests, pair ok (+21 routines / +1 operation), next stop `0x1F6620`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 6-function seed | `87f3ccab6c7d08e42a2b80e2f586c68358e5a34de457cebb7dc663c42c27e3ad` |
| `0x1F6970` seed | `5d519e103331b673e0b93a5c912156b73eb86bdbbf28f6eda2d35a2c6c5d57e6` |
| `generation.json` | `cb5da6e858ce7f14dd63c3cf0ee14458c989df37058a73ba56c86c0b6bee0b20` |
| `title-functions.inc` | `2007ef33c42cdb91c0fc17c960bc1fccafccf8e3943b7121e3cab6435177b158` |
| `title-profile.h` | `881113447b2c123a0614c781e8779977a34a8a8e7e101a0490f5d035390a61e0` |
| Module 79 | `8baa111d9e5a1505f719a2086ccdd5952eb9a7d833194d6118208791c3467e39` |
| Control report | `7830642017d8c5c90e9837344ce0545073e819bdbe8d641f2fad19a2a9104e0e` |
| Experiment report | `4061ef6c786881e818fa2fbbfb712c313f814f121abdf0d6b6395adc0a932360` |

## Next and limits

A read-only direct-branch worklist then characterized `0x1F6620` as an **8-instruction / 36-byte D3D leaf** with one return at `0x1F6641`, no calls, jumps, tails or unresolved branches, and memory references `0x206720`/`0x203B20`. It is a **named symbol** (`D3DDevice_SetRenderState_PSTextureModes`) reached indirectly through table `0x2025C0` (index 116), so the retained project had no function there. The next cycle defined that bounded leaf in memory only for the initial export, then established it persistently in the retained project, exported a clean seed and admitted it in translation 59/module 80; the observed next stop is `0x1F6BB0` (table `0x2025C4`, index 117). See [the 0x1F6620 checkpoint](FRONTIER_DEP_1F6620_CHECKPOINT.md). These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. `0x1F6620` was not entered in this batch; it executed in the next.
