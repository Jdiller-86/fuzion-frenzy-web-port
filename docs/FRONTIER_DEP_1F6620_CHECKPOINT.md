# The `0x1F6620` symbol-entry batch advances the stop to `0x1F6BB0`

November 4, 2026. Provisional continuation after Path B/`0x1F6970`: the observed stop `0x1F6620` was ordinary, but the retained exporter reported `established: false`. Read-only inspection resolved the gate: `0x1F6620` is a **named symbol** (`D3DDevice_SetRenderState_PSTextureModes`) reached through an indirect call table, not a direct call target, so Ghidra never created a function there. This cycle defined the bounded leaf in memory only and exported it with the retained exporter. It is **not an approved design decision**; architect guidance remains `approved: false`. **There is no controllable original screen.**

## Why the previous gate failed

- `0x1F6620` has symbol `D3DDevice_SetRenderState_PSTextureModes` but `FUNCTION_AT` and `FUNCTION_CONTAINING` are both null.
- The caller `0x1F66A0` (`D3DDevice_SetRenderStateNotInline`) ends in an **indirect** `CALL dword ptr [ESI*4 + 0x2023F0]` at `0x1F66EF`, returning to `0x1F66F6`.
- The table entry for `0x1F6620` is `0x2025C0`, **index 116** (`0x74`); the next entry `0x2025C4` (index 117) is `0x1F6BB0`.
- The observed experiment stack previously carried render-state value `116`; after this batch it carries `117`, matching the next table entry.

## Read-only symbol-entry export

The ignored research script `research/verification/frontier-1f6620/ExportSymbolEntry.java`:

1. Checks the retained executable SHA-256 (`45bb49e1…`).
2. Requires a primary symbol at `0x1F6620` and no existing function there.
3. Decodes exactly 8 contiguous instructions ending at `0x1F6641` (`RET 4`, three bytes) with no calls, jumps, or trailing bytes.
4. Finds exactly one reference in the bounded indirect-call table (`0x2025C0`, index 116).
5. Creates the function **in memory only** under `-readOnly` and calls the retained `ExportRecompilationSeed.java`.

The persistent Ghidra project and original executable were not modified. This is a read-only evidence technique, not a new source-owned tool. The seed exporter remains the normal admission path.

## Seed and preflight

- `0x1F6620` seed: **8 instructions / a 36-byte instruction-only envelope**, no calls/tables, SHA-256 `55977f4921d0cc94c576876fc7eefeaf7d7c834bb6f92c76d9005ed55fc66473`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 8 rows; **8/8 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 6-function seed → **7 functions**, SHA-256 `31c0a3d71bce4677e6ce2ea196425c1d03a49186e66b80f3ba1757f528eb2ee0`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-59`: **431 functions / 18,081 instructions**; `0x1F6620` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **521 functions, 0 rewrites**. The frozen 32-export limit still applies; the earlier 35-offered-seed suggestion remains superseded by the corrected 32.

## Module and controlled pair

Module `recomp-post-memory-build-80`: **688,623 bytes**, SHA-256 `cfc5786ab4fb9cbe365f4bf662be6fe62333c26e5701427264e65249fc450c90`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6BB0`** | 301 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +22 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6620` executed; `0x1F6BB0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **521 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6BB0` caller, so this is again an indirect table boundary (entry `0x2025C4`, index 117).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6620_PACKAGE_MANIFEST.json` reports 32/32 seeds, 431 admitted / 521 generated / 0 rewrites, **18/18 digests**, pair ok (+22 routines / +1 operation), next stop `0x1F6BB0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 7-function seed | `31c0a3d71bce4677e6ce2ea196425c1d03a49186e66b80f3ba1757f528eb2ee0` |
| `0x1F6620` symbol-entry seed | `55977f4921d0cc94c576876fc7eefeaf7d7c834bb6f92c76d9005ed55fc66473` |
| `generation.json` | `c0b150c49ff4ea9dd4992bc0b41a27b63df30c6c51df137e6efb993af7b8c561` |
| `title-functions.inc` | `9a0861285ae04b3d39b339f9f7978f297d22c045592f8fbc9fc5442cff9f9c43` |
| `title-profile.h` | `9922ad6702426d445619ec5361625953684b5e24eefbd788fc56dc511d5d76a2` |
| Module 80 | `cfc5786ab4fb9cbe365f4bf662be6fe62333c26e5701427264e65249fc450c90` |
| Control report | `621e76e12e2ba7eb6da5c05ab426b3d6727218204c8dbcf312de31991cd69045` |
| Experiment report | `69babf0ce18cf9a9734e2765092fd4d01468fa2fa01c9625c372ba7f17fba0c5` |

## Next and limits

The observed stop `0x1F6BB0` is an ordinary untranslated routine and the next indirect render-state entry (index 117). Per the provisional directive, a later ≤8-function dependency batch could continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. `0x1F6BB0` was not entered.

The symbol-entry definition is a bounded read-only exception for a named indirect-dispatch leaf; it does not establish general function-recovery policy, original execution, or permission to define arbitrary gaps.
