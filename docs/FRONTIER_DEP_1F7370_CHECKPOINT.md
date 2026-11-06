> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F7370` batch advances the stop to `0x1F73F0`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F7370`. It was already a Ghidra-established function and its only call is translated. **There is no controllable original screen.**

## Selection and establishment

- `0x1F7370` is `D3DDevice_SetRenderState_StencilEnable`, the next indirect render-state entry (table index **124**, render state `0x7C`).
- The function was already established (`body 001f7370..001f73ef`), so no create-function was needed.
- The leaf is **37 instructions / 128 bytes**: one call to already-translated `0x1FB3B0`, internal conditional branches/jumps, one `RET 4` at `0x1F73ED`, no tails or unresolved branches.

## Seed and preflight

- Seed: **37 instructions / a 128-byte instruction-only envelope**, one call, no tables, SHA-256 `315b777603dbf13db42a0a6ac2f0ea57f444c22f381c6a5bf7374dc425f452e7`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 37 rows; **37/37 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 14-function seed â†’ **15 functions**, SHA-256 `15239924d6e18ea8f359255504c2ff0b325f548061625616f901f131bd506e13`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-67`: **439 functions / 18,266 instructions**; `0x1F7370` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **529 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-88`: **694,246 bytes**, SHA-256 `5440f6e0bb7ceb8c27eb0e5f411e66b1dd0e40891b20acdd49bb94c7d709b3ec`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F73F0`** | 309 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +30 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7370` executed; `0x1F73F0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **529 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F73F0` caller, so this is again an indirect table boundary (index 125).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7370_PACKAGE_MANIFEST.json` reports 32/32 seeds, 439 admitted / 529 generated / 0 rewrites, **26/26 digests**, pair ok (+30 routines / +1 operation), next stop `0x1F73F0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 15-function seed | `15239924d6e18ea8f359255504c2ff0b325f548061625616f901f131bd506e13` |
| `0x1F7370` seed | `315b777603dbf13db42a0a6ac2f0ea57f444c22f381c6a5bf7374dc425f452e7` |
| `generation.json` | `eb12a14d3ef67f88b1881d7927f9c053747756b4602a230a49477571fcc9b5d3` |
| `title-functions.inc` | `eb2217722ac3f8228d1505215048e1e128f121e4c465ae7c4425e4918373e28e` |
| `title-profile.h` | `e8af00c3385adde8d888c9fa9007234c0170b1d9901dc72cf2f3a3d444431739` |
| Module 88 | `5440f6e0bb7ceb8c27eb0e5f411e66b1dd0e40891b20acdd49bb94c7d709b3ec` |
| Control report | `d865ec693088c6b274687d84baf021aa9d5aa0780834f1783f34acb70c76de7b` |
| Experiment report | `e9ba4d4656e36bac5195210f1fb28b72bcf8a8b562eedb96b43a103c2f845386` |

## Next and limits

The observed stop `0x1F73F0` is the next ordinary indirect render-state entry (table index 125); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.
