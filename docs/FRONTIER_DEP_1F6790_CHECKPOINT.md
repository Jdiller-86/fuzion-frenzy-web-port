> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F6790` batch advances the stop to `0x1F6AC0`

November 4, 2026. This cycle admitted the next ordinary stop, `0x1F6790`. It was already a Ghidra-established function, so no create-function step was needed. **There is no controllable original screen.**

## Selection and evidence

- `0x1F6790` is `D3DDevice_SetRenderState_FogColor`, the next indirect render-state entry (table `0x2025C8`, **index 118**, render state `0x76`).
- The function is already established in the retained project (no create-function this cycle).
- The leaf is **22 instructions / 71 bytes**: one call to already-translated `0x1FB3B0`, one `RET 4` at `0x1F67D4`, no jumps, tails or unresolved branches.

## Seed and preflight

- Seed: **22 instructions / a 71-byte instruction-only envelope**, one call, no tables, SHA-256 `3de7c1542a4f2935760c7fcc309a3322a61d09ee91d4789f964a970f261ddc61`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 22 rows; **22/22 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 8-function seed â†’ **9 functions**, SHA-256 `f7a2ee44815b9fa27bc57128928c7ca220c1eee98b08211a7d2bafe3dc94c930`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-61`: **433 functions / 18,118 instructions**; `0x1F6790` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **523 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-82`: **689,834 bytes**, SHA-256 `b3c354bdb3c561924f2d76c1018166a91a3136c2c63b2c704d5f0f68742f0bf0`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6AC0`** | 303 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +24 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6790` executed; `0x1F6AC0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **523 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6AC0` caller, so this is again an indirect table boundary (entry `0x2025CC`, index 119).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6790_PACKAGE_MANIFEST.json` reports 32/32 seeds, 433 admitted / 523 generated / 0 rewrites, **20/20 digests**, pair ok (+24 routines / +1 operation), next stop `0x1F6AC0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 9-function seed | `f7a2ee44815b9fa27bc57128928c7ca220c1eee98b08211a7d2bafe3dc94c930` |
| `0x1F6790` seed | `3de7c1542a4f2935760c7fcc309a3322a61d09ee91d4789f964a970f261ddc61` |
| `generation.json` | `493276f1d50e8b2d2320c346566626809d6ef008679866e5924ce1f195a1d775` |
| `title-functions.inc` | `512984d5e7d8a4b08bfd8d24190248a474458edb615993a3686a451dee207427` |
| `title-profile.h` | `b6f8e6a004864bfbdc5c622b106e78c6196872665ea30f1414a9d72a2cc75ff7` |
| Module 82 | `b3c354bdb3c561924f2d76c1018166a91a3136c2c63b2c704d5f0f68742f0bf0` |
| Control report | `e5dd752394eb9da5987cef50bd842811723b9d39bba1f50eb0619b8f0297da1e` |
| Experiment report | `8cd5a2c37de163dd299c4a78877c52a39054b2735fa9c0a1bc321eb3b0932511` |

## Next and limits

The observed stop `0x1F6AC0` is the next ordinary indirect render-state entry (table index 119); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.
