# The `0x1F6910` dependency batch advances the stop to `0x1F5B70`

November 4, 2026. Provisional continuation after the Path A batch: the observed stop `0x1F6910` was an ordinary untranslated routine, so this cycle closed exactly that one-function dependency set. It is **not an approved design decision**; the architect guidance remains `approved: false`. One translation, one comparison and one commit. **There is no controllable original screen.**

## Selection and grouping

- `0x1F6910` seed: **28 instructions / a 95-byte instruction-only envelope**, SHA-256 `8bbc17798639ef5c445525160ffa5d1112c98574d418cc2649e206d450098520`.
- Its only direct calls are `0x1FC980` and `0x1FB3B0`, both already translated; no additional dependency seed was needed.
- Grouping: append `0x1F6910` to the 3-function merged recovered + `0x1FCB70` seed → **3 functions**, SHA-256 `69cf2ad73bfa6ea269533cc8de57d7138fdb598f16299660747f125cba77c014`. The 32-function front seed and the 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-56`: **427 functions / 17,807 instructions**; `0x1F6910` has a real guarded body; 91 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **518 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-77`: **676,813 bytes**, SHA-256 `0f41fd06b47743dc7c86cbdb6f566fa4689bcc5d32ba92155e26d30309e86635`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F5B70`** | 292 | 405 |

- Same module and full supplied profile; `avQueryWord` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+13 routines, +1 kernel operation**; the experiment preserves every control-reached routine and its operation count matches its successful calls.
- `0x1F6910` executed through its real body and is in the reached list. The next call target `0x1F5B70` is untranslated and was **not entered**; the stop returns to `0x1F5882` (call site `0x1F587D`) from executed caller `0x1F5640`.
- Frontier on translation 56: **518 functions, 91 placeholders, 90 referenced**; `0x1F5B70` is called by executed `0x1F5640`.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6910_PACKAGE_MANIFEST.json` reports:

```
seeds: 32/32 (0 missing)
translation: 427 admitted functions; guard: 518 generated / 0 rewrites
digests: 14/14
controlled pair: ok; +13 routines / +1 operations; next stop 0x1F5B70
package verified
```

## Digests

| Artifact | SHA-256 |
| --- | --- |
| 32-function front seed | `b59d2fab9ae8cc285da942dc280cbd9ccaab4d8f72d65d8fd540163b145998ba` |
| Merged 3-function seed | `69cf2ad73bfa6ea269533cc8de57d7138fdb598f16299660747f125cba77c014` |
| `0x1F70E0` seed | `ee62446fbc381b3b377d683b707af61aac5459f5f79f84e9afcb3dc0e58d92ed` |
| `0x1FC980` seed | `18744bb385b803cd8d67c0d4c2e26d02c30e005e6b0a8f8c11e7d31972e23d4b` |
| `0x1FCB70` table-aware seed | `09b7da8dbbfc15b3f32ecaab430331066ef814a04dce1e01bb0a8b51afb40305` |
| `0x1F6910` seed | `8bbc17798639ef5c445525160ffa5d1112c98574d418cc2649e206d450098520` |
| `generation.json` | `43760b5d2b59b68f8092fafb736370706fa6ebf4aac52b5e536bedd15cc2f90e` |
| `title-functions.inc` | `364f992e2669a353267a582f4daf789c49412a1008fd48e2b7fcc0324aff219f` |
| `title-profile.h` | `21320ea45ad790651dbc2d0cc9aefb5e27c3ff0d4893f01383c4d76d2631eca1` |
| Module 77 | `0f41fd06b47743dc7c86cbdb6f566fa4689bcc5d32ba92155e26d30309e86635` |
| Control profile | `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419` |
| Experiment profile | `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af` |
| Control report | `ae3ac3dcf59f74a92b6848169b740b48fd035e758d31c2b6e501dc5d2aae261a` |
| Experiment report | `7045da4f372ab85c416b33896596eefca67cb6c2d725d94cc8f96eb5cf4f23f8` |

## Next and limits

The observed stop `0x1F5B70` is an ordinary untranslated routine and the **Path B** continuation (`0x1F5B70` + `0x1F6300` + the already-translated `0x1FC980`). Path B was subsequently admitted in translation 57/module 78; the observed next stop is `0x1F6970` (ordinary). Per the provisional directive, a later ≤8-function dependency batch could continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.
