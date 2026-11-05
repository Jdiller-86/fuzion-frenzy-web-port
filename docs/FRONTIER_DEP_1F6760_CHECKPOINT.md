# The `0x1F6760` batch advances the stop to `0x1F7460`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F6760`, with main ancestry retained. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6760` is `D3DDevice_SetRenderState_ShadowFunc`, table index **135**, render state `0x87`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6760` (`CREATED 001f6760 D3DDevice_SetRenderState_ShadowFunc body=001f6760..001f678e`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **13 instructions / 47 bytes**: one call to already-translated `0x1FB3B0`, one `RET 4` at `0x1F678C`, no tails or unresolved branches.

## Seed and preflight

- Seed: **13 instructions / a 47-byte instruction-only envelope**, one call, no tables, SHA-256 `3a4e953de511eafa97cc099b92d940e3a208ae107ead751f1f8185b94d6f822e`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 13 rows; **13/13 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 24-function seed → **25 functions**, SHA-256 `9cb1f5c6bf425bd09ae6aae3f07f02e0b3a7e151f5794a125cea3f16c878dd25`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-76`: **449 functions / 18,496 instructions**; `0x1F6760` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **539 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-97`: **701,883 bytes**, SHA-256 `c7eb34ab8fd1160cf6211d762f9bf6c0eec505d8cc5e24ab784a310fe407c17e`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7460`** | 319 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +40 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6760` executed; `0x1F7460` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **539 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F7460` caller, so this is an indirect table boundary (index 138).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6760_PACKAGE_MANIFEST.json` reports 32/32 seeds, 449 admitted / 539 generated / 0 rewrites, **36/36 digests**, pair ok (+40 routines / +1 operation), next stop `0x1F7460`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 25-function seed | `9cb1f5c6bf425bd09ae6aae3f07f02e0b3a7e151f5794a125cea3f16c878dd25` |
| `0x1F6760` established-symbol seed | `3a4e953de511eafa97cc099b92d940e3a208ae107ead751f1f8185b94d6f822e` |
| `generation.json` | `ff48803dae0d00592d4e50c2482f5ddef52ce438ef0e6318a38f52a4e83f343b` |
| `title-functions.inc` | `fdc50f0aec5781b7a183db6a2cdb86ca6e35baf6965e87097b414f15f03fecd9` |
| `title-profile.h` | `6ec09bea72c7493ca17e6e40326286aa18a66011bfa92e695e04780b18d8d185` |
| Module 97 | `c7eb34ab8fd1160cf6211d762f9bf6c0eec505d8cc5e24ab784a310fe407c17e` |
| Control report | `c6bf02e107a4986b244c03936098e87906ead197d6bec16180d0baf3699150ca` |
| Experiment report | `7480fdc964b48a9d52680def91fb3e7ef40eca763856eafb559dbbb633f5a181` |

## Next and limits

The observed stop `0x1F7460` is the next ordinary indirect render-state entry (table index 138); it was not entered. A later ≤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6760` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
