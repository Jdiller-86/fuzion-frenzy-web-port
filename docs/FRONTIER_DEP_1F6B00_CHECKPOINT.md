# The `0x1F6B00` batch advances the stop to `0x1F6B50`

November 4, 2026. This cycle admitted the next ordinary stop, `0x1F6B00`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6B00` is `D3DDevice_SetRenderState_BackFillMode`, the next indirect render-state entry (table `0x2025D0`, **index 120**, render state `0x78`).
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6B00` (`CREATED 001f6b00 D3DDevice_SetRenderState_BackFillMode body=001f6b00..001f6b4a`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **22 instructions / 75 bytes**: one call to already-translated `0x1FB3B0`, one internal `JNE` to `0x1F6B2F`, one `RET 4` at `0x1F6B48`, no tails or unresolved branches.

## Seed and preflight

- Seed: **22 instructions / a 75-byte instruction-only envelope**, one call, no tables, SHA-256 `b126ca7b5acd4301f8fa9b0de5a176399066a67e0ee1038bacdebaa2789951cf`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 22 rows; **22/22 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 10-function seed → **11 functions**, SHA-256 `e4c3222d2ce3334b4da987568037f2e7afb99fef3cf3dd07fb9c8615db52c133`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-63`: **435 functions / 18,158 instructions**; `0x1F6B00` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **525 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-84`: **691,173 bytes**, SHA-256 `e1f59842bf7dcd58720f54ee41e421a8a8e704dd3398dea57c3c255b8e03bfec`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6B50`** | 305 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +26 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6B00` executed; `0x1F6B50` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **525 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6B50` caller, so this is again an indirect table boundary (entry `0x2025D4`, index 121).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6B00_PACKAGE_MANIFEST.json` reports 32/32 seeds, 435 admitted / 525 generated / 0 rewrites, **22/22 digests**, pair ok (+26 routines / +1 operation), next stop `0x1F6B50`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 11-function seed | `e4c3222d2ce3334b4da987568037f2e7afb99fef3cf3dd07fb9c8615db52c133` |
| `0x1F6B00` established-symbol seed | `b126ca7b5acd4301f8fa9b0de5a176399066a67e0ee1038bacdebaa2789951cf` |
| `generation.json` | `4318ab59194ed8749a4672de06fea04e8de5cb960cb2cebdbf8430c91e48fe84` |
| `title-functions.inc` | `f5183dd0be8be51b275d5161a89b1502fcea831bd9317cd83832cdfbaa90dbf7` |
| `title-profile.h` | `dca674b8a6bb1d365633176bfca1b2771bc91b1dda9ef397d138d458cbf71df6` |
| Module 84 | `e1f59842bf7dcd58720f54ee41e421a8a8e704dd3398dea57c3c255b8e03bfec` |
| Control report | `ea5197ccf41916d2d50911d4efee78103a4ad9a0e2aa0cfc73ca5e38f9eb6851` |
| Experiment report | `2228b81546bc0dbb813eb791e0405e58e1d339930acb8d83bb3917b7bb40800b` |

## Next and limits

The observed stop `0x1F6B50` is the next ordinary indirect render-state entry (table index 121); it was not entered. A later ≤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6B00` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
