# The `0x1F74F0` batch advances the stop to `0x1F7550`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F74F0`, after merging architect `d4e0107` and renumbering the local decision rows (`D339` stays the architect merge row; the `0x1F7490` admission is `D340`). **There is no controllable original screen.**

## Selection and establishment

- `0x1F74F0` is `D3DDevice_SetRenderState_StencilCullEnable`, dispatch table `0x2023F0` index **140** (entry `0x202620`), reached from the previous experiment.
- The table does **not** end at this entry: indices 141-143 (`RopZCmpAlwaysRead` `0x1F7550`, `RopZRead` `0x1F7570`, `DoNotCullUncompressed` `0x1F7590`) still point to handlers, and index 144 onward is non-pointer data. The stop is therefore ordinary and in-region.
- The retained project had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F74F0` (`CREATED 001f74f0 D3DDevice_SetRenderState_StencilCullEnable body=001f74f0..001f754b`, `TABLE_ENTRY 00202620 index=140`, `TABLE_MATCHES 1`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **26 instructions / 92 bytes**: one call to already-translated `0x1FB3B0`, internal conditional branches (`JZ` ×3, `JNZ` ×1), one `RET 4` at `0x1F7549`, no tables, no tails, no unresolved branches.

## Seed and preflight

- Seed: 26 instructions / a 92-byte instruction-only envelope, one call, no tables, SHA-256 `8b53384bdbfb9cf9678dc459e43df74afea68b524a622205e6edc1b4b6a12844`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 26 rows; **26/26 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 27-function seed → **28 functions**, SHA-256 `877657f289222c2756af57e333941ca3065e76cc6905c734c4a22d08e20c8138`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-79`: **452 functions / 18,560 instructions**; `0x1F74F0` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **542 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-100`: **703,605 bytes**, SHA-256 `c03009873b3f46c7a248da4abcad6b74a34af8d5862305108afd8aaf88adcf96`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7550`** | 322 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +43 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F74F0` executed; `0x1F7550` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`). Frontier: **542 functions, 90 placeholders, 89 referenced**, 353 direct call targets.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F74F0_PACKAGE_MANIFEST.json` reports 32/32 seeds, **452 admitted / 542 generated / 0 rewrites**, **39/39 digests**, pair ok (+43 routines / +1 operation), next stop `0x1F7550`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 28-function seed | `877657f289222c2756af57e333941ca3065e76cc6905c734c4a22d08e20c8138` |
| `0x1F74F0` established-symbol seed | `8b53384bdbfb9cf9678dc459e43df74afea68b524a622205e6edc1b4b6a12844` |
| `generation.json` | `fe0d9880cf9f9514f762dc61f239487fd2f199a24098ba3165980a81091ba9f2` |
| `title-functions.inc` | `b6e04cbb71afd2e555c44a2345acf35230b5a427783fac813b452258af3c9300` |
| `title-profile.h` | `f3a1cb3e6ed824dc285e6aa9857b15c674e3213e95a28169feaa5b94173d5c31` |
| Module 100 | `c03009873b3f46c7a248da4abcad6b74a34af8d5862305108afd8aaf88adcf96` |
| Control report | `25d6161dd3b2433630eb946a3c7b7983517108de23d8d85f32ec558798ee46af` |
| Experiment report | `003c2196f8216fa7a6d1951b05b81219b1ac868e3cbfeaee0b47533805d326a1` |

## Next and limits

The observed stop `0x1F7550` (`RopZCmpAlwaysRead`, table index 141) is an ordinary in-region render-state entry; it was not entered. Indices 142-143 also remain, so the table is not complete and this checkpoint does **not** declare a post-table region. This cycle stops here for review; a later ≤8-function batch may continue only while stops remain ordinary. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F74F0` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
