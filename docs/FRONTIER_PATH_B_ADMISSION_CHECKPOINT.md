> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The Path B batch advances the stop to `0x1F6970`

November 4, 2026. Provisional continuation after the `0x1F6910` batch: the observed stop `0x1F5B70` was ordinary, so this cycle closed the Path B set. It is **not an approved design decision**; architect guidance remains `approved: false`. One translation, one comparison and one commit. **There is no controllable original screen.**

## Selection and grouping

- `0x1F5B70` seed: **118 instructions / a 347-byte envelope**, SHA-256 `aa8ee11b8e9571a62630033f6f0b03fe95be5f3fa93f67ab7ffdb54b07bd071b`. Its calls to `0x1F85E0`, `0x1FB3B0` and `0x1F6EB0` are already translated; `0x1F6300` is added here.
- `0x1F6300` seed: **113 instructions / a 392-byte envelope**, SHA-256 `f4c8d089852a19bb7c1a101ca1754e8d734e659f5bef88cfe5ab464e6438e1af`. Its calls to `0x1FB3B0` and `0x1FC980` are already translated.
- Grouping: append both to the merged recovered + `0x1FCB70` + `0x1F6910` seed → **5 functions**, SHA-256 `ab5813ec93067ffa5373f4c401694cbf0de30d5de8358d08a1a18f38020eb2b1`. The 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-57`: **429 functions / 18,038 instructions**; both new bodies real and guarded; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **519 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-78`: **687,271 bytes**, SHA-256 `d6540c1b2259c2b9025f7e8786f7cc2510480ef0c28f3cb2675a86835cc7fa92`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6970`** | 296 | 405 |

- Same module and full supplied profile; `avQueryWord` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+17 routines, +1 kernel operation**; the experiment preserves every control-reached routine and its operation count matches its successful calls.
- `0x1F5B70` and `0x1F6300` executed through their real bodies and are in the reached list. The next call target `0x1F6970` is untranslated and was **not entered**; the stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated caller `0x1F66A0`.
- Frontier on translation 57: **519 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6970` caller, so this is an observed/indirect boundary.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_PATH_B_PACKAGE_MANIFEST.json` reports:

```
seeds: 32/32 (0 missing)
translation: 429 admitted functions; guard: 519 generated / 0 rewrites
digests: 16/16
controlled pair: ok; +17 routines / +1 operations; next stop 0x1F6970
package verified
```

## Digests

| Artifact | SHA-256 |
| --- | --- |
| 32-function front seed | `b59d2fab9ae8cc285da942dc280cbd9ccaab4d8f72d65d8fd540163b145998ba` |
| Merged 5-function seed | `ab5813ec93067ffa5373f4c401694cbf0de30d5de8358d08a1a18f38020eb2b1` |
| `0x1F5B70` seed | `aa8ee11b8e9571a62630033f6f0b03fe95be5f3fa93f67ab7ffdb54b07bd071b` |
| `0x1F6300` seed | `f4c8d089852a19bb7c1a101ca1754e8d734e659f5bef88cfe5ab464e6438e1af` |
| `generation.json` | `ae6744717716f5a48ff537762985f3c1823602af0e97710055ec66d4be309373` |
| `title-functions.inc` | `958547d32d57887a06fd9d674d6e02e784dd83a65faadbf2ae82e582434c3cde` |
| `title-profile.h` | `2de71c067436fbaa59a5e6d31149cdead921a94ec55a5d2d0ab882913327fa88` |
| Module 78 | `d6540c1b2259c2b9025f7e8786f7cc2510480ef0c28f3cb2675a86835cc7fa92` |
| Control profile | `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419` |
| Experiment profile | `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af` |
| Control report | `10314bdd67e6c3df1e269e49f65490a533ced883c958e597b9d455f480ab5217` |
| Experiment report | `ef60e0e4421b2c4211ffa74b60f7eb0e02337f0327f3a4dfa691a8c8ffb2c6c1` |

## Next and limits

The observed stop `0x1F6970` is an ordinary untranslated routine. `0x1F6970` was subsequently admitted in translation 58/module 79; the observed next stop is `0x1F6620` (ordinary). Per the provisional directive, a later ≤8-function dependency batch could continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. `0x1F6970` was not entered.
