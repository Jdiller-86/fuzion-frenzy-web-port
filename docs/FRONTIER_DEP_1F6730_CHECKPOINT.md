> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F6730` batch advances the stop to `0x1F75B0`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F6730`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6730` is `D3DDevice_SetRenderState_EdgeAntiAlias`, table index **131**, render state `0x83`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6730` (`CREATED 001f6730 D3DDevice_SetRenderState_EdgeAntiAlias body=001f6730..001f675b`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **13 instructions / 44 bytes**: one call to already-translated `0x1FB3B0`, one `RET 4` at `0x1F6759`, no tails or unresolved branches.

## Seed and preflight

- Seed: **13 instructions / a 44-byte instruction-only envelope**, one call, no tables, SHA-256 `fe75b542333f71cf64dde7965401821625cb958e10575b770d1aaed07d0c9ed6`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 13 rows; **13/13 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 21-function seed â†’ **22 functions**, SHA-256 `61fd9a0bc91f086e77769a216175005c1586dcb40dba319988fb3db173a7f504`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-73`: **446 functions / 18,439 instructions**; `0x1F6730` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **536 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-94`: **700,022 bytes**, SHA-256 `e9b108f222addd30b65f9687a63879698d29e8ef41148adff744f696b94b9704`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F75B0`** | 316 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +37 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6730` executed; `0x1F75B0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **536 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F75B0` caller, so this is an indirect table boundary (index 132).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6730_PACKAGE_MANIFEST.json` reports 32/32 seeds, 446 admitted / 536 generated / 0 rewrites, **33/33 digests**, pair ok (+37 routines / +1 operation), next stop `0x1F75B0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 22-function seed | `61fd9a0bc91f086e77769a216175005c1586dcb40dba319988fb3db173a7f504` |
| `0x1F6730` established-symbol seed | `fe75b542333f71cf64dde7965401821625cb958e10575b770d1aaed07d0c9ed6` |
| `generation.json` | `58dbaad0eb420ecc763931648a626b89c0f2b69b806b7413e7e6a0d3871543f8` |
| `title-functions.inc` | `49e8e296fb9c0c1dde8b5d392911f11f992b2c7f960a8f4e54a4b0d40729b0a2` |
| `title-profile.h` | `b0bb4c61c7f10e82fe618f514d326122cbbb31536f17fe0d8835b6efd1627f5f` |
| Module 94 | `e9b108f222addd30b65f9687a63879698d29e8ef41148adff744f696b94b9704` |
| Control report | `dc8d415b06676591c9c663862b8dd70c282e07d323793f7835cac7dd825092b5` |
| Experiment report | `1f62a902126e5713ed5b8a91b28e545b884efba17a77421c0dbc3c99a878468c` |

## Next and limits

The observed stop `0x1F75B0` is the next ordinary indirect render-state entry (table index 132); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6730` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
