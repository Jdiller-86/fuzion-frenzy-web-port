# The `0x1F6850` tail-call batch advances the stop to `0x1F68C0`

November 5, 2026. This cycle admitted `0x1F6850` and its already-established tail target `0x1F67E0` as one two-function batch. **There is no controllable original screen.**

## Selection and tail call

- `0x1F6850` is `D3DDevice_SetRenderState_FrontFace`, table index **126**, and was a named symbol without a Ghidra function.
- It ends in a tail `JMP 0x1F67E0` at `0x1F687F`, with no return of its own.
- A bounded headless run created exactly one function at `0x1F6850` (`body 0x1F6850..0x1F6883`) and preserved the tail exit; `Save succeeded`.
- `0x1F67E0` is `D3DDevice_SetRenderState_CullMode`, table index **127**, and was already established (`body 0x1F67E0..0x1F6841`).
- No other function was created or analyzed; the executable is unchanged.

## Seed and preflight

- `0x1F6850` seed: **14 instructions / 52 bytes**, one call to translated `0x1FB3B0`, one established tail to `0x1F67E0`, SHA-256 `80bdcbee9c1fde5fad4da056f5dff191ee1baace87ee64e7687eae5e673eb13e`; preflight 14/14, no tables or normalizations.
- `0x1F67E0` seed: **29 instructions / 98 bytes**, one call to translated `0x1FB3B0`, two return sites, SHA-256 `2d5f7bf52d0bdf71a189dff150dbc4900cf099feed05726724353785e4eeebf5`; preflight 29/29, no tables or normalizations.
- Grouping: both appended to the merged 16-function seed â†’ **18 functions**, SHA-256 `3bbc774af2a6e024e193f2a5913bf8054ffa27442aa70d1a9df2ee73e435dc0f`; 32 exports and the front seed are unchanged.
- Translation `recomp-title-recomp-title-post-memory-69`: **442 functions / 18,338 instructions**; both new bodies are real guarded code; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **532 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-90`: **696,254 bytes**, SHA-256 `f74a2161de4d8d3761b1ac601bef65913b564b57eaf94d783df4b35cef466a88`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F68C0`** | 312 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +33 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6850` executed, tail-called `0x1F67E0`, and both are reached; `0x1F68C0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **532 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F68C0` caller, so this is an indirect table boundary (index 128).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6850_PACKAGE_MANIFEST.json` reports 32/32 seeds, 442 admitted / 532 generated / 0 rewrites, **29/29 digests**, pair ok (+33 routines / +1 operation), next stop `0x1F68C0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 18-function seed | `3bbc774af2a6e024e193f2a5913bf8054ffa27442aa70d1a9df2ee73e435dc0f` |
| `0x1F6850` tail-call seed | `80bdcbee9c1fde5fad4da056f5dff191ee1baace87ee64e7687eae5e673eb13e` |
| `0x1F67E0` seed | `2d5f7bf52d0bdf71a189dff150dbc4900cf099feed05726724353785e4eeebf5` |
| `generation.json` | `fd765b0ba636460f8f9a956160e9860c9e1d4e349e4032ef59d86fc31140461b` |
| `title-functions.inc` | `63245f543752c408c445eb90319717d766ab992c00283c2c1146dcd03730743f` |
| `title-profile.h` | `232b470453df49105c84f465fb80c0b95aee707e57ae2dd7b6c440bfa4bff178` |
| Module 90 | `f74a2161de4d8d3761b1ac601bef65913b564b57eaf94d783df4b35cef466a88` |
| Control report | `48cd5477c5f1a9acae961986a4fc98429a314fe0ad7ce0213b7371db28088f2c` |
| Experiment report | `6320d603812733d7406ec32b99f0193947628dc7c1b7017428eae4ed48da4321` |

## Next and limits

The observed stop `0x1F68C0` is the next ordinary indirect render-state entry (table index 128); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The tail-preserving create-function is limited to an established local tail target and does not establish general function-recovery policy or permission to define arbitrary gaps.
