# The `0x1F73F0` batch advances the stop to `0x1F6850`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F73F0`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F73F0` is `D3DDevice_SetRenderState_StencilFail`, the next indirect render-state entry (table index **125**, render state `0x7D`).
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F73F0` (`CREATED 001f73f0 D3DDevice_SetRenderState_StencilFail body=001f73f0..001f7458`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **29 instructions / 105 bytes**: one call to already-translated `0x1FB3B0`, internal conditional branches, one `RET 4` at `0x1F7456`, no tails or unresolved branches.

## Seed and preflight

- Seed: **29 instructions / a 105-byte instruction-only envelope**, one call, no tables, SHA-256 `856b5a100771821e71ed4b17c2b4dbdfad801f4e92fe0dd03714a5b3f2792811`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 29 rows; **29/29 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 15-function seed â†’ **16 functions**, SHA-256 `14cc691346b7d4cfd6748c6ca7a306d3a8bae3f929b935772fd86584e8769e68`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-68`: **440 functions / 18,295 instructions**; `0x1F73F0` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **530 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-89`: **694,956 bytes**, SHA-256 `4f64855e0f6e39583dd9887da58ffa240ab4b74cefb46d9cbc94c397b4ea36d9`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6850`** | 310 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +31 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F73F0` executed; `0x1F6850` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **530 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6850` caller, so this is again an indirect table boundary (index 126).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F73F0_PACKAGE_MANIFEST.json` reports 32/32 seeds, 440 admitted / 530 generated / 0 rewrites, **27/27 digests**, pair ok (+31 routines / +1 operation), next stop `0x1F6850`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 16-function seed | `14cc691346b7d4cfd6748c6ca7a306d3a8bae3f929b935772fd86584e8769e68` |
| `0x1F73F0` established-symbol seed | `856b5a100771821e71ed4b17c2b4dbdfad801f4e92fe0dd03714a5b3f2792811` |
| `generation.json` | `685c40c29fccb3d5bcf5cbe9ba59787681aedfa2abbebbf9e8c20330b10a6c76` |
| `title-functions.inc` | `260eca5ecabadbdc41390a0e56d863c34b302c16dabb6743ba0162826256ba37` |
| `title-profile.h` | `cb5c18c155e91d751ff4921c767a8d684d3ac258a2c08c74fc02b516f34ab7d6` |
| Module 89 | `4f64855e0f6e39583dd9887da58ffa240ab4b74cefb46d9cbc94c397b4ea36d9` |
| Control report | `75b954f876f0d586d494b32abb6eefe7c34e9aedaf70534dca3eff5c47b84bef` |
| Experiment report | `4b883e65e27800553b952dda230c6239c1ff65f0afc010e44466d2b070e1726b` |

## Next and limits

The observed stop `0x1F6850` is the next ordinary indirect render-state entry (table index 126); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F73F0` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
