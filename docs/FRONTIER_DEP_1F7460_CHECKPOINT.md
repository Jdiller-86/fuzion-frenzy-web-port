> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F7460` batch advances the stop to `0x1F7490`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F7460`, with main ancestry retained. **There is no controllable original screen.**

## Selection and establishment

- `0x1F7460` is `D3DDevice_SetRenderState_YuvEnable`, table index **138**, render state `0x8A`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F7460` (`CREATED 001f7460 D3DDevice_SetRenderState_YuvEnable body=001f7460..001f7484`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **12 instructions / 37 bytes**: two calls to already-translated `0x1FB3B0` and `0x1F7080`, one `RET 4` at `0x1F7482`, no tails or unresolved branches.

## Seed and preflight

- Seed: **12 instructions / a 37-byte instruction-only envelope**, two calls, no tables, SHA-256 `0b8d447be837de5c1e21282c129f88cd3aec39299a4bd60eace14cbf066042ed`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 12 rows; **12/12 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 25-function seed â†’ **26 functions**, SHA-256 `dc0ab154d28e8ed813089f6861831ff42fce8a0896360eca573c120801e129d8`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-77`: **450 functions / 18,508 instructions**; `0x1F7460` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **540 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-98`: **702,324 bytes**, SHA-256 `60d2a65f73914f6ed9807516e316aa2d19bdfd2931e562a86eae44714de515e6`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7490`** | 320 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +41 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7460` executed; `0x1F7490` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **540 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F7490` caller, so this is an indirect table boundary (index 139).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7460_PACKAGE_MANIFEST.json` reports 32/32 seeds, 450 admitted / 540 generated / 0 rewrites, **37/37 digests**, pair ok (+41 routines / +1 operation), next stop `0x1F7490`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 26-function seed | `dc0ab154d28e8ed813089f6861831ff42fce8a0896360eca573c120801e129d8` |
| `0x1F7460` established-symbol seed | `0b8d447be837de5c1e21282c129f88cd3aec39299a4bd60eace14cbf066042ed` |
| `generation.json` | `a128d1fda39b0a317cb1b33b2f89a657761a63d39883cf60483033fd70e2ca50` |
| `title-functions.inc` | `c0872b17bf5c57387776f9a0a56a9b2875a057054a508771bac51dd7f0ecdf55` |
| `title-profile.h` | `055d4a009f9f8333c9cf7a723e07dac4e97533ab617d2e70cdbc014ac4dd78d9` |
| Module 98 | `60d2a65f73914f6ed9807516e316aa2d19bdfd2931e562a86eae44714de515e6` |
| Control report | `2678fdcb18fc4a97eef29f39ef49adc5a43d1b472db460b224c98f689c4e354a` |
| Experiment report | `75dd33bfe8ef7c2ccc38aead585fba238afae697240cfdee3dd43e07efaa6d02` |

## Next and limits

The observed stop `0x1F7490` is the next ordinary indirect render-state entry (table index 139); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F7460` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
