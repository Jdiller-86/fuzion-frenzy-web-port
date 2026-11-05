# The `0x1F75B0` batch advances the stop to `0x1F7600`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F75B0`, on top of the restored main ancestry. **There is no controllable original screen.**

## Integration prerequisite

- PR #7 merged to main as `42287210`.
- The source branch was two commits behind main; `origin/main` was merged locally at `a0be99b`, restoring ancestry.
- Main's `D327` row was kept; the seven newer primary rows were renumbered to `D328`–`D334`, and this batch uses `D335`.
- A merge-introduced EOF blank line was removed at `8c49a22`; the publication whitespace gate is clean.

## Selection and establishment

- `0x1F75B0` is `D3DDevice_SetRenderState_MultiSampleAntiAlias`, table index **132**, render state `0x84`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F75B0` (`CREATED 001f75b0 D3DDevice_SetRenderState_MultiSampleAntiAlias body=001f75b0..001f75fb`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **22 instructions / 76 bytes**: three calls to already-translated `0x1F82C0`, `0x1FB3B0` and `0x1F6EB0`, one `RET 4` at `0x1F75F9`, no tails or unresolved branches.

## Seed and preflight

- Seed: **22 instructions / a 76-byte instruction-only envelope**, three calls, no tables, SHA-256 `d75806bfff6f17ec89b54d5babc53f57c190365773c83c83bb3257d8afd7832c`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 22 rows; **22/22 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 22-function seed → **23 functions**, SHA-256 `d74b0d28de532c51ca42614108b1ef2ed36aaec2e37be18af534716b8ff60810`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-74`: **447 functions / 18,461 instructions**; `0x1F75B0` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **537 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-95`: **700,698 bytes**, SHA-256 `286daba75b816b3fb7bf933fc614bc63d5c289c0eca01633d27f87093b77f182`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7600`** | 317 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +38 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F75B0` executed; `0x1F7600` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **537 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F7600` caller, so this is an indirect table boundary (index 133).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F75B0_PACKAGE_MANIFEST.json` reports 32/32 seeds, 447 admitted / 537 generated / 0 rewrites, **34/34 digests**, pair ok (+38 routines / +1 operation), next stop `0x1F7600`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 23-function seed | `d74b0d28de532c51ca42614108b1ef2ed36aaec2e37be18af534716b8ff60810` |
| `0x1F75B0` established-symbol seed | `d75806bfff6f17ec89b54d5babc53f57c190365773c83c83bb3257d8afd7832c` |
| `generation.json` | `2f1eae8b75cfcd17c5d99d476e4b56834c3994f179a9c8b21b173bc16b8e93f3` |
| `title-functions.inc` | `1c1c05f561e84d0f407f7da58d0e18ec88fd71b4c6eb28270fe81042cefe908d` |
| `title-profile.h` | `020aba328bf19b116ef8cd6f406572017f0d87fee528f19eff39515575aba28d` |
| Module 95 | `286daba75b816b3fb7bf933fc614bc63d5c289c0eca01633d27f87093b77f182` |
| Control report | `ff2c7ac104e9253b031c5c22acec97f4b792ce2a2be1bb64ba61f568274f3a0f` |
| Experiment report | `f98ebcbc0e50aa334bfc510538964dc91de7460fb65eaf4941115babb83ee83d` |

## Next and limits

The observed stop `0x1F7600` is the next ordinary indirect render-state entry (table index 133); it was not entered. A later ≤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F75B0` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
