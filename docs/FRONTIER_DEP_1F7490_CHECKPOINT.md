# The `0x1F7490` batch advances the stop to `0x1F74F0`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F7490`, with main ancestry retained. **There is no controllable original screen.**

## Selection and establishment

- `0x1F7490` is `D3DDevice_SetRenderState_OcclusionCullEnable`, table index **139**, render state `0x8B`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F7490` (`CREATED 001f7490 D3DDevice_SetRenderState_OcclusionCullEnable body=001f7490..001f74eb`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **26 instructions / 92 bytes**: one call to already-translated `0x1FB3B0`, internal conditional branches, one `RET 4` at `0x1F74E9`, no tails or unresolved branches.

## Seed and preflight

- Seed: **26 instructions / a 92-byte instruction-only envelope**, one call, no tables, SHA-256 `7dfc9582feb1e8b4b8d6b150a97b0094f3047e40df7ded561882a4b09599db34`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 26 rows; **26/26 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 26-function seed → **27 functions**, SHA-256 `df3d4c44fb8fb2f766d11c10fd3ca7a8552b8f7f668e470b6fc5f0a603aa6db5`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-78`: **451 functions / 18,534 instructions**; `0x1F7490` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **541 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-99`: **702,974 bytes**, SHA-256 `29e937977e0c435d12dcb1d253eef090d0cb4ad88354f0b1fa5e224bc4e529b5`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F74F0`** | 321 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +42 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7490` executed; `0x1F74F0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **541 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F74F0` caller, so this is an indirect table boundary (index 140).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7490_PACKAGE_MANIFEST.json` reports 32/32 seeds, 451 admitted / 541 generated / 0 rewrites, **38/38 digests**, pair ok (+42 routines / +1 operation), next stop `0x1F74F0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 27-function seed | `df3d4c44fb8fb2f766d11c10fd3ca7a8552b8f7f668e470b6fc5f0a603aa6db5` |
| `0x1F7490` established-symbol seed | `7dfc9582feb1e8b4b8d6b150a97b0094f3047e40df7ded561882a4b09599db34` |
| `generation.json` | `682a3aa4875e53d2fe68d36b8b45bbd7616160d9a11547fc10c078a16969d836` |
| `title-functions.inc` | `8dd1bd37e2b7baba6818d82ece95d83eaed811722241d2b8f1f368726db584ac` |
| `title-profile.h` | `1cea091d4321067964e7c37a333587e21fff4d221df37a3c4b0d51f11f0cd0be` |
| Module 99 | `29e937977e0c435d12dcb1d253eef090d0cb4ad88354f0b1fa5e224bc4e529b5` |
| Control report | `ebb4448bcae4429c9b345c8ed091f1c93652f1bbaf40e648fa759eeafe49c2df` |
| Experiment report | `9b2aaea1f57d21fa2dab2514adf550866b8e5d0b684e1869f8086b035d3426fc` |

## Next and limits

The observed stop `0x1F74F0` is the next ordinary indirect render-state entry (table index 140); it was not entered. A later ≤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F7490` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
