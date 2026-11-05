# The `0x1F6AC0` batch advances the stop to `0x1F6B00`

November 4, 2026. This cycle admitted the next ordinary stop, `0x1F6AC0`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6AC0` is `D3DDevice_SetRenderState_FillMode`, the next indirect render-state entry (table `0x2025CC`, **index 119**, render state `0x77`).
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6AC0` (`CREATED 001f6ac0 D3DDevice_SetRenderState_FillMode body=001f6ac0..001f6afd`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **18 instructions / 62 bytes**: one call to already-translated `0x1FB3B0`, one internal `JNE` to `0x1F6AE3`, one `RET 4` at `0x1F6AFB`, no tails or unresolved branches.

## Seed and preflight

- Seed: **18 instructions / a 62-byte instruction-only envelope**, one call, no tables, SHA-256 `628ba62b1ad406e38ece04dc96994625c81e5f022e3428f38dcaa47d56fc3ec8`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 18 rows; **18/18 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 9-function seed → **10 functions**, SHA-256 `aeb158b5520c6684d8e87304ec410ec7dce42ae8484da3948b634793fed91e83`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-62`: **434 functions / 18,136 instructions**; `0x1F6AC0` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **524 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-83`: **690,455 bytes**, SHA-256 `9a84bdc1cfe8ac7e1a56871b4415d0a2f6ed2bd33475be93eb71b93cfa95cf15`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6B00`** | 304 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +25 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6AC0` executed; `0x1F6B00` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **524 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6B00` caller, so this is again an indirect table boundary (entry `0x2025D0`, index 120).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6AC0_PACKAGE_MANIFEST.json` reports 32/32 seeds, 434 admitted / 524 generated / 0 rewrites, **21/21 digests**, pair ok (+25 routines / +1 operation), next stop `0x1F6B00`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 10-function seed | `aeb158b5520c6684d8e87304ec410ec7dce42ae8484da3948b634793fed91e83` |
| `0x1F6AC0` established-symbol seed | `628ba62b1ad406e38ece04dc96994625c81e5f022e3428f38dcaa47d56fc3ec8` |
| `generation.json` | `0c815241b237dad2a6f66d56082a21cb7f2778c07718424d00fe0e5386b60a83` |
| `title-functions.inc` | `fcedd99a2fbbdfc96d5267ffa064b4821028a75d06388b028dc52fa45a41cebc` |
| `title-profile.h` | `75e7a50c1400baf45678012ec024387045bc68a39ddc40cc9010878a807208e7` |
| Module 83 | `9a84bdc1cfe8ac7e1a56871b4415d0a2f6ed2bd33475be93eb71b93cfa95cf15` |
| Control report | `780400d9863e139c61a4189282631a08413cc4c6444af0e5102804381fb1a603` |
| Experiment report | `d2e4f4bee262efb2feb53bfff1d253b4f8bfe59cd9ee195a001538a8438047ab` |

## Next and limits

The observed stop `0x1F6B00` is the next ordinary indirect render-state entry (table index 120); it was not entered. A later ≤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6AC0` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
