# The `0x1F65F0` batch closes the last render-state translation gap

November 5, 2026. Published head `6cbe40d` was the starting point. The render-state table had already been reached end-to-end in module 102, but a coverage audit showed that one table entry had never been executed and was still **untranslated**: index 134, `0x1F65F0` (`D3DDevice_SetRenderState_MultiSampleType`). This cycle admits that in-region handler so the table is genuinely 28/28 translated. **The post-table region (`D3DDevice_Present`) remains untouched and undecided.** There is no controllable original screen.

## Why this set

- Translation 81 covered **27 of the 28** handlers at table indices 116-143. `0x1F65F0` was the only one with neither a translated body nor a placeholder.
- The observed startup never set that render state, so it never produced a stop and was invisible to the stop-driven batch.
- It is still inside the already-authorized render-state region, so closing it does not cross the directive's post-table halt; admitting `D3DDevice_Present` would.

**Correction:** the previous checkpoint said indices 116-143 were "admitted or already translated". That was inaccurate; index 134 was missing. This checkpoint corrects the record with the 28/28 audit below.

## Establishment and seed

- The retained project had the symbol but no function. One bounded create-function at that address only (`body 0x1F65F0..0x1F6614`) reported `Save succeeded`, `TABLE_ENTRY 00202608 index=134`, `TABLE_MATCHES 1`.
- The leaf is **9 instructions / 37 bytes**: it reads the device context, stores the argument to `0x203B68`, loads two context fields (`+0x40C`, `+0x410`), and calls already-translated `0x1F5640`, then `RET 4`. No branches, no tables, no new dependency.
- Seed SHA-256 `f338d481cc457b7fb6ca0fb1e7c8e1b7cd9075cf6cebadc2b5478afae2ff97f7`; preflight `mixed False`, 0 tables, **9/9 decoded**, no normalizations; byte identity, decoder agreement and layout pass.

## Seed-export regrouping (bounded gate workaround)

The merged seed had reached the pipeline's hard **32-function-per-export** limit (`MAX_EXPANDED_EXPORTS`/`functions_per_export`), so appending the new function failed with `Each seed must contain 1-32 functions`. This was resolved by regrouping, not by raising any limit:

- Two existing single-function exports — `recomp-heap-reallocation-helper-next-01/seed.json` (`0x31973`) and `recomp-after-initializers-02/seed.json` (`0x2D540`) — were combined into one 2-function export `recomp-seeds-merged-01/seed.json`, SHA-256 `25074fa11f024de9e8de36fdc6c86c79795dd744dfed15c28eacea69493aef23`.
- The merged 32-function seed was already at the cap, so the new admission was exported as its own 1-function seed. A 33-function merged candidate (`f4dca8ea...`) was written but **not used**: translation 82 kept the 32-function merged seed plus the new export.
- Export count stays at **32**, and the original seed files are unchanged on disk.

## Translation and coverage

Translation `recomp-title-recomp-title-post-memory-82`: **457 functions / 18,626 instructions**; `0x1F65F0` has a real guarded body; 90 placeholders remain.

- `verify-seeds` **32/32/0/0**; `verify-translation` **547 functions, 0 rewrites**.
- Table coverage audit against the 28 known entries (116-143): **28/28 translated, no placeholder bodies**.

## Module and controlled pair

Module `recomp-post-memory-build-103`: **705,795 bytes**, SHA-256 `6e298d4440194fa7373cbe6b93de2014e124d04899c80b99931a67db21f592ff`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x00202150`** | 332 | 405 |

The comparator exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +53 routines, +1 operation. The experiment stop and routine counts are **unchanged from module 102**, which is the expected result for a handler this profile never sets; the pair shows no regression rather than new progress. `0x1F65F0` was not reached. Frontier: **547 functions, 90 placeholders, 89 referenced**, 354 direct call targets.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F65F0_PACKAGE_MANIFEST.json` reports 32/32 seeds, **457 admitted / 547 generated / 0 rewrites**, **45/45 digests**, pair ok (+53 routines / +1 operation), next stop `0x202150`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 33-function seed | `f4dca8ea770fde6dfd2698c99debaa376454aace0c5b0f1c3431b736f9275b7b` |
| `0x1F65F0` established-symbol seed | `f338d481cc457b7fb6ca0fb1e7c8e1b7cd9075cf6cebadc2b5478afae2ff97f7` |
| Regrouped 2-function seed export | `25074fa11f024de9e8de36fdc6c86c79795dd744dfed15c28eacea69493aef23` |
| `generation.json` | `cc8eb2011c8fd10b14bb746da01b9ce33f4ea09074665f0ccc7c3f80c8651372` |
| `title-functions.inc` | `c7679257052c38062d37e5681f91d751e248d96132e73b4bca5498d1d040f99b` |
| `title-profile.h` | `9af83b67edfa0526467373acba8f530357cf362f0e00cf68c62411bd42ee8449` |
| Module 103 | `6e298d4440194fa7373cbe6b93de2014e124d04899c80b99931a67db21f592ff` |
| Control report | `3cafd34986a787c30f24f28dc10584db09bb4b48254029067cb57b3efcf5b668` |
| Experiment report | `8e5e29337178a24d596acf18689825946295070bdcbdc8228370c57effb72fbc` |

## Next and limits

The render-state region is now fully translated (28/28). The observed stop remains the post-table direct call `0x00202150` (`D3DDevice_Present`) from `0x42C5E` in `FUN_00042be0`; it is still not admitted, and the next region remains a fresh architect decision. This cycle stops here for review.

These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. Translation coverage of a handler is not evidence that the corresponding render state is exercised or correct.
