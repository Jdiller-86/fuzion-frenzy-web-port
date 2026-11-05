# The Path A admission advances the stop to `0x1F6910`

November 4, 2026. A provisional hourly review proposed one Path A batch for the observed branch `0x1F70E0`. It is **not an approved design decision**; the batch uses the already-approved tooling and the frozen 32-export limit. This cycle performed one translation, one comparison and one commit, with the package verifier as the evidence step. **There is no controllable original screen.**

## Selection and grouping correction

The provisional acceptance mentioned 35 offered seeds. The frozen tooling accepts at most **32 seed exports** at `--function-limit 512`; a direct check confirms 32 seeds are accepted while 33 are rejected (`Use 1-32 bounded seed exports`). The batch therefore uses 32 exported seeds:

- Front seed: 30 → **32 functions** by adding `0x1F70E0` and `0x1FC980`; SHA-256 `b59d2fab9ae8cc285da942dc280cbd9ccaab4d8f72d65d8fd540163b145998ba`.
- Merged seed: recovered `0x002022E0` + table-aware `0x1FCB70` → **2 functions**; SHA-256 `98e1108d31db1932a7e98a6b7ae1ca20f6ee7f382532b4bf051cc0cc5c02560c`.
- New seeds: `0x1F70E0` (98 instructions / 329 bytes, SHA `ee62446f…`), `0x1FC980` (2 / 9, SHA `18744bb3…`), `0x1FCB70` table-aware (304-byte envelope, 11+2 tables, 80 data bytes, SHA `09b7da8d…`).
- Translation `recomp-title-recomp-title-post-memory-55`: **426 functions / 17,779 instructions**; `0x1F70E0`, `0x1FC980` and `0x1FCB70` all have real guarded bodies; 91 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **517 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-76`: **675,058 bytes**, SHA-256 `5d596ab2c24d022b267faf4409fcd189ee62bcd059004662fdae885062dd5518`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6910`** | 289 | 405 |

- Same module and full supplied profile; `avQueryWord` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+10 routines, +1 kernel operation**; the experiment preserves every control-reached routine and its operation count matches its successful calls.
- `0x1F70E0` and `0x1FC980` executed and are in the reached list. `0x1FCB70` was translated and guard-validated but was **not on this observed path**; its body did not execute.
- The next stop is untranslated **`0x1F6910`** (status 3, "untranslated original routine"), returning to `0x1F66F6` (call site `0x1F66F1`) from the translated caller `0x1F66A0`. The frontier's direct-call map has no entry for `0x1F6910`, so this is an observed/indirect boundary, not a direct static call.
- Frontier on translation 55: **517 functions, 91 placeholders, 90 referenced**.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_PATH_A_PACKAGE_MANIFEST.json` reports:

```
seeds: 32/32 (0 missing)
translation: 426 admitted functions; guard: 517 generated / 0 rewrites
digests: 13/13
controlled pair: ok; +10 routines / +1 operations; next stop 0x1F6910
package verified
```

## Digests

| Artifact | SHA-256 |
| --- | --- |
| 32-function front seed | `b59d2fab9ae8cc285da942dc280cbd9ccaab4d8f72d65d8fd540163b145998ba` |
| Merged recovered + `0x1FCB70` seed | `98e1108d31db1932a7e98a6b7ae1ca20f6ee7f382532b4bf051cc0cc5c02560c` |
| `0x1F70E0` seed | `ee62446fbc381b3b377d683b707af61aac5459f5f79f84e9afcb3dc0e58d92ed` |
| `0x1FC980` seed | `18744bb385b803cd8d67c0d4c2e26d02c30e005e6b0a8f8c11e7d31972e23d4b` |
| `0x1FCB70` table-aware seed | `09b7da8dbbfc15b3f32ecaab430331066ef814a04dce1e01bb0a8b51afb40305` |
| `generation.json` | `2e5ce3c6c056c7b196c73a806e2960809271a18421ef5655dd47aafe31a9fd51` |
| `title-functions.inc` | `dcdfea15bfe4e30f1a99cd0f8bedf5aa78b8619001659102642196b38f988288` |
| `title-profile.h` | `f09d8ba82fb1a23d93bafc0c6ae0a7ee43128ddc8814307f9b691151237fff1d` |
| Module 76 | `5d596ab2c24d022b267faf4409fcd189ee62bcd059004662fdae885062dd5518` |
| Control profile | `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419` |
| Experiment profile | `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af` |
| Control report | `1e6cd33f719b710720e79863d72c71e70365d2dccf70193073b2215f360366d2` |
| Experiment report | `47bd31c1ad1d965a6e353518bb4e20c35f43c90bfd24471f50e5fea0e078ccc3` |

## Next and limits

The observed stop `0x1F6910` is an ordinary untranslated routine. Per the directive, a later dependency-closure batch (≤8 functions/set) may continue only while stops remain ordinary and after the required review; this cycle stops here. `0x1F6910` was subsequently admitted in translation 56/module 77; the observed next stop is `0x1F5B70` (Path B). See [the 0x1F6910 checkpoint](FRONTIER_DEP_1F6910_CHECKPOINT.md). These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. `0x1FCB70` was translated but did not execute in this run.
