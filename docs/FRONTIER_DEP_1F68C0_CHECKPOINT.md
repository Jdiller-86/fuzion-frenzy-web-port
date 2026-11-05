# The `0x1F68C0` batch advances the stop to `0x1F69F0`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F68C0`, including its `rep stosd` texture-factor copy. It was already a Ghidra-established function. **There is no controllable original screen.**

## Selection and evidence

- `0x1F68C0` is `D3DDevice_SetRenderState_TextureFactor`, table index **128**, render state `0x80`.
- The function was already established (`body 001f68c0..001f690b`), so no create-function was needed.
- The leaf is **25 instructions / 76 bytes**: one call to already-translated `0x1FB3B0`, one `rep stosd` copying 16 dwords, two return sites (`0x1F68FC` and `0x1F6909`), no tails or unresolved branches.

## Seed and preflight

- Seed: **25 instructions / a 76-byte instruction-only envelope**, one call, no tables, SHA-256 `bc293f7745bc533e5940ff6441edad4e1431211ba42ea91568e5efe92fb55b05`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 25 rows; **25/25 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 18-function seed â†’ **19 functions**, SHA-256 `347da10020008e5b6e2d5565ed12498b4468cdc3d4d0b39da18845a35b6d4338`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-70`: **443 functions / 18,363 instructions**; `0x1F68C0` has a real guarded body with the `rep stosd`; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **533 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-91`: **697,055 bytes**, SHA-256 `21ef36559c62ea4cce52c9b6719cd3d0ffc69c484bb2ccb5c480b8d6261e45ae`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F69F0`** | 313 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +34 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F68C0` executed; `0x1F69F0` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **533 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F69F0` caller, so this is an indirect table boundary (index 129).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F68C0_PACKAGE_MANIFEST.json` reports 32/32 seeds, 443 admitted / 533 generated / 0 rewrites, **30/30 digests**, pair ok (+34 routines / +1 operation), next stop `0x1F69F0`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 19-function seed | `347da10020008e5b6e2d5565ed12498b4468cdc3d4d0b39da18845a35b6d4338` |
| `0x1F68C0` seed | `bc293f7745bc533e5940ff6441edad4e1431211ba42ea91568e5efe92fb55b05` |
| `generation.json` | `e639d65caecfddf85b13c5f1727c9b4b3b093491b7cc4a3da42fd7e714a4b4d4` |
| `title-functions.inc` | `bdc0df6a68621a0db17b4b5927cfabd7b509203e384b27b3773593cf3b6ca2ba` |
| `title-profile.h` | `66d75960492661723c8bdbdeb12ab56cdff8bf6c7c2a62f274c67f295180b902` |
| Module 91 | `21ef36559c62ea4cce52c9b6719cd3d0ffc69c484bb2ccb5c480b8d6261e45ae` |
| Control report | `09f6dd2263a7c4f406eee053d237326cef9f86d6d19b5f8d169e58a17d933aea` |
| Experiment report | `2762eface613dc92e8db1a7851e28ea042216cc688da6ef7322033cc1d478526` |

## Next and limits

The observed stop `0x1F69F0` is the next ordinary indirect render-state entry (table index 129); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.
