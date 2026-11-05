# The `0x1F6B50` batch advances the stop to `0x1F6890`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F6B50`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6B50` is `D3DDevice_SetRenderState_TwoSidedLighting`, the next indirect render-state entry (table `0x2025D4`, **index 121**, render state `0x79`).
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6B50` (`CREATED 001f6b50 D3DDevice_SetRenderState_TwoSidedLighting body=001f6b50..001f6ba3`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **24 instructions / 84 bytes**: one call to already-translated `0x1FB3B0`, one internal `JNE` to `0x1F6B88`, one `RET 4` at `0x1F6BA1`, no tails or unresolved branches.

## Seed and preflight

- Seed: **24 instructions / an 84-byte instruction-only envelope**, one call, no tables, SHA-256 `f520ecd54f72ba83ee5d9099baeee8caa63a0ab01b1d139917de4d9b3c7b362d`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 24 rows; **24/24 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 11-function seed â†’ **12 functions**, SHA-256 `831991b36dc80dbb566a3b582cb598d1ab328765b4b4e9344956bbc15c801b74`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-64`: **436 functions / 18,182 instructions**; `0x1F6B50` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **526 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-85`: **692,002 bytes**, SHA-256 `d136484b1c5460fe17df8db9a2217017cb5d8fbaafdb171b762a262bc9ab8b01`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6890`** | 306 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +27 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6B50` executed; `0x1F6890` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **526 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6890` caller, so this is again an indirect table boundary (entry `0x2025D8`, index 122).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6B50_PACKAGE_MANIFEST.json` reports 32/32 seeds, 436 admitted / 526 generated / 0 rewrites, **23/23 digests**, pair ok (+27 routines / +1 operation), next stop `0x1F6890`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 12-function seed | `831991b36dc80dbb566a3b582cb598d1ab328765b4b4e9344956bbc15c801b74` |
| `0x1F6B50` established-symbol seed | `f520ecd54f72ba83ee5d9099baeee8caa63a0ab01b1d139917de4d9b3c7b362d` |
| `generation.json` | `83b8b9873b12f32d5bb9cdfa75cee1239da474b3172bc6a0d642ff7aa0ce55ac` |
| `title-functions.inc` | `4a92f76472fb7b51d59dedbb701f9771e83bbe619c87b4102a135de521c414d8` |
| `title-profile.h` | `26288868aad47549709ca177f5a44e058a3a2e55534afecfa8b11af1ee12ec0f` |
| Module 85 | `d136484b1c5460fe17df8db9a2217017cb5d8fbaafdb171b762a262bc9ab8b01` |
| Control report | `8220ac955fd1ada89614530c364f75d30cc9fa05143bdb8a6d4b0f62788e87fe` |
| Experiment report | `8b5a57749264ea584ea9dfd0518eaf965f3fa0ebe2d9284697f4bf01d9fbce40` |

## Next and limits

The observed stop `0x1F6890` is the next ordinary indirect render-state entry (table index 122); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6B50` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
