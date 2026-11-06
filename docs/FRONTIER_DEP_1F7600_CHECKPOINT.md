> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F7600` batch advances the stop to `0x1F6760`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F7600`, on top of the restored main ancestry. **There is no controllable original screen.**

## Selection and establishment

- `0x1F7600` is `D3DDevice_SetRenderState_MultiSampleMask`, table index **133**, render state `0x85`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F7600` (`CREATED 001f7600 D3DDevice_SetRenderState_MultiSampleMask body=001f7600..001f764b`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **22 instructions / 76 bytes**: three calls to already-translated `0x1F82C0`, `0x1FB3B0` and `0x1F6EB0`, one `RET 4` at `0x1F7649`, no tails or unresolved branches.

## Seed and preflight

- Seed: **22 instructions / a 76-byte instruction-only envelope**, three calls, no tables, SHA-256 `68d64fb88f05d6f8675ee4f6a9eb6cf4cc42d0f8996845ad540327691a13d459`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 22 rows; **22/22 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 23-function seed â†’ **24 functions**, SHA-256 `b95fe13ccf9df2eab68642a28eee49cdb5baad169a53032740ba72ca11d25238`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-75`: **448 functions / 18,483 instructions**; `0x1F7600` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **538 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-96`: **701,399 bytes**, SHA-256 `8da3c4b55a2e453221ef87ca786b49c540bb5ab3c3e679aba177fa87f52c8819`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6760`** | 318 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +39 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7600` executed; `0x1F6760` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **538 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6760` caller, so this is an indirect table boundary (index 135).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7600_PACKAGE_MANIFEST.json` reports 32/32 seeds, 448 admitted / 538 generated / 0 rewrites, **35/35 digests**, pair ok (+39 routines / +1 operation), next stop `0x1F6760`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 24-function seed | `b95fe13ccf9df2eab68642a28eee49cdb5baad169a53032740ba72ca11d25238` |
| `0x1F7600` established-symbol seed | `68d64fb88f05d6f8675ee4f6a9eb6cf4cc42d0f8996845ad540327691a13d459` |
| `generation.json` | `5aba574e54374c6221a332975bccc91c90c72e0647b563fa9ce9ce0228d75440` |
| `title-functions.inc` | `101df2f2d2b131ed6ae9783f734927438d9152e2785a1e2b197927dfd02bdf4d` |
| `title-profile.h` | `bd43f8a9bd36ae5c52675de0b757a777d7ba0682df8c994019ed50d2667c0b60` |
| Module 96 | `8da3c4b55a2e453221ef87ca786b49c540bb5ab3c3e679aba177fa87f52c8819` |
| Control report | `9e9a3eb76910d86af0477df264f99922aef8cb0b73318319b44c54d7c7cd15ad` |
| Experiment report | `b72c9087175c8c001e89d5c6bd87e72a6471231cc46c949eeb4cd93122093db3` |

## Next and limits

The observed stop `0x1F6760` is the next ordinary indirect render-state entry (table index 135); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F7600` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
