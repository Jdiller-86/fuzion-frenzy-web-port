# The `0x1F6BB0` symbol-entry batch advances the stop to `0x1F6790`

November 4, 2026. After the approved `0x1F6620` work was already complete and the architect integration gate remained blocked by a verification-test CI failure, this cycle chose the next unowned primary task: the ordinary stop `0x1F6BB0`. It is **not** a new route decision. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6BB0` is the next indirect render-state entry: table `0x2025C4`, **index 117** (`0x75`), symbol `D3DDevice_SetRenderState_VertexBlend`.
- The retained project initially had the symbol but no function there (`FUNCTION_AT` and `FUNCTION_CONTAINING` were null).
- A bounded headless run created exactly one function at `0x1F6BB0` (`CREATED 001f6bb0 D3DDevice_SetRenderState_VertexBlend body=001f6bb0..001f6be1`) and reported `Save succeeded`; a later read-only re-open reports `FUNCTION_AT 001f6bb0`.
- No other function was created or analyzed. The original executable is unchanged.
- The leaf is **15 instructions / 50 bytes**: one call to already-translated `0x1FB3B0`, one `RET 4` at `0x1F6BDF`, no jumps, tails or unresolved branches.

## Seed and preflight

- Seed: **15 instructions / a 50-byte instruction-only envelope**, one call, no tables, SHA-256 `70add3a6e6e1d595e83e805eb4231e89414ed41f7c3594a9da9661a58b5324e9`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 15 rows; **15/15 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 7-function seed â†’ **8 functions**, SHA-256 `4fcdcd022b61125afd09a7b58102dfd77d7049a72fed7b127d34671bb639d55f`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-60`: **432 functions / 18,096 instructions**; `0x1F6BB0` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **522 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-81`: **689,176 bytes**, SHA-256 `d795973beea54c70344e363f587206f1268c98dbdac86c951c647f412cdc3d67`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6790`** | 302 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +23 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6BB0` executed; `0x1F6790` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **522 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6790` caller, so this is again an indirect table boundary (entry `0x2025C8`, index 118).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6BB0_PACKAGE_MANIFEST.json` reports 32/32 seeds, 432 admitted / 522 generated / 0 rewrites, **19/19 digests**, pair ok (+23 routines / +1 operation), next stop `0x1F6790`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 8-function seed | `4fcdcd022b61125afd09a7b58102dfd77d7049a72fed7b127d34671bb639d55f` |
| `0x1F6BB0` established-symbol seed | `70add3a6e6e1d595e83e805eb4231e89414ed41f7c3594a9da9661a58b5324e9` |
| `generation.json` | `38a911265371e3177aa573251526e3cdf6b6665fbb5e55751af4e86e43fd8ad1` |
| `title-functions.inc` | `0b65b120cb63cb2d3faa270c4b5f3508135edc3aba31f8ef953c23323c757b7a` |
| `title-profile.h` | `3e8455a72d42a3d4a8a1350f1ff8bad4b6d513ef5a6e530ea4349d45b82eac26` |
| Module 81 | `d795973beea54c70344e363f587206f1268c98dbdac86c951c647f412cdc3d67` |
| Control report | `78e5c64cc267b75fed50ca207bae3ce1c12b6b904e602f2deef81ee5e2895180` |
| Experiment report | `a1d2d24a23d950c73fc00ff7b9bb29f506013458f8d105cda1a7640b4ed2b72c` |

## Next and limits

The observed stop `0x1F6790` is an ordinary untranslated routine, already a Ghidra-established function (`D3DDevice_SetRenderState_FogColor`), so the next batch does not need a create-function step for it. Per the standing â‰¤8-function policy, a later batch may continue only after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. `0x1F6790` was not entered.

The persistent `0x1F6BB0` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
