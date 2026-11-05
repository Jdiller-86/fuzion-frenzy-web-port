# The `0x1F7550` batch advances the stop to `0x1F7570`

November 5, 2026. Published head `b576fa2` (CI run `41991304806`, success) was the starting point. This cycle admitted the ordinary stop `0x1F7550` together with the direct dependency it requires. **There is no controllable original screen.**

## Selection and establishment

- `0x1F7550` is `D3DDevice_SetRenderState_RopZCmpAlwaysRead`, dispatch table `0x2023F0` index **141** (entry `0x202624`).
- The retained project had the symbol but no function there. A bounded headless create-function at that address only (`body 0x1F7550..0x1F7560`) reported `Save succeeded` and `TABLE_ENTRY 00202624 index=141`; `TABLE_MATCHES 1`.
- The leaf is **4 instructions / 17 bytes**: it stores its argument to `0x203B84` and calls `0x1F7230`, then `RET 4`.
- `0x1F7230` is `D3D_CommonSetDebugRegisters`, **already an established function** (`body 0x1F7230..0x1F72F7`), so no create was needed or performed. It is **45 instructions / 200 bytes**, calls the already-translated `0x1FB3B0`, has internal conditional branches, and ends in a plain `RET`. It is the necessary bounded direct dependency of the new leaf.
- No other function was created or analyzed; the original executable is unchanged.

## Seeds and preflight

| entry | instructions | bytes | SHA-256 |
| --- | --- | --- | --- |
| `0x1F7550` | 4 | 17 | `610fda82e1b335135cfdd392202af9a2ad3d619c4335cce82ccf0ad097424aa9` |
| `0x1F7230` | 45 | 200 | `2038c102e223715a6aee404eb3dfdec9f917a287c399e00978913e6f7eef0267` |

Both preflights are clean: `mixed False`, 0 tables, 4/4 and 45/45 decoded, no normalizations; byte identity, decoder agreement and layout pass.

- Grouping: both appended to the merged 28-function seed → **30 functions**, SHA-256 `adce139624da473986e610087b3e8371a3e1bc59b0d9cc3b0dcff5578e5151c9`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-80`: **454 functions / 18,609 instructions**; both entries have real guarded bodies; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **544 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-101`: **705,079 bytes**, SHA-256 `cfb7b918eb44ba742a34d51371c2e762f839b47916063785da1911f60a77047e`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7570`** | 324 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +45 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7550` and `0x1F7230` both executed; `0x1F7570` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`). Frontier: **544 functions, 90 placeholders, 89 referenced**, 354 direct call targets.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7550_PACKAGE_MANIFEST.json` reports 32/32 seeds, **454 admitted / 544 generated / 0 rewrites**, **41/41 digests**, pair ok (+45 routines / +1 operation), next stop `0x1F7570`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 30-function seed | `adce139624da473986e610087b3e8371a3e1bc59b0d9cc3b0dcff5578e5151c9` |
| `0x1F7550` established-symbol seed | `610fda82e1b335135cfdd392202af9a2ad3d619c4335cce82ccf0ad097424aa9` |
| `0x1F7230` established-function seed | `2038c102e223715a6aee404eb3dfdec9f917a287c399e00978913e6f7eef0267` |
| `generation.json` | `6f0f936b776a93bea7f81f3241890542451920ef8b3888b1651bfe23aab1381c` |
| `title-functions.inc` | `7eb4da3f65ed4bbc00e5aca8b9d9d1312f66ac0666f47913340a33189d0ab4df` |
| `title-profile.h` | `aaa0e83bc5301809835f96a185e3e5cf72909d5f7015cce313aced0241648b2a` |
| Module 101 | `cfb7b918eb44ba742a34d51371c2e762f839b47916063785da1911f60a77047e` |
| Control report | `14b8920206eb0cb874e03e180420fd2338df57d8569ef1bd2b3a76a4330345bb` |
| Experiment report | `b2886711a486f43e0d8a140b0f405764665e67c0dd734e13dc551a73369e99c2` |

## Next and limits

The observed stop `0x1F7570` (`RopZRead`, table index 142) is an ordinary in-region render-state entry; it was not entered. Index 143 (`0x1F7590`, `DoNotCullUncompressed`) also remains and index 144 onward is non-pointer data, so the table is still not complete and no post-table region is declared. This cycle stops here for review; a later ≤8-function batch may continue only while stops remain ordinary. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F7550` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
