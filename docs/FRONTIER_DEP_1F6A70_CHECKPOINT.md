> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F6A70` batch advances the stop to `0x1F6730`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F6A70`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6A70` is `D3DDevice_SetRenderState_LogicOp`, table index **130**, render state `0x82`.
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6A70` (`CREATED 001f6a70 D3DDevice_SetRenderState_LogicOp body=001f6a70..001f6abb`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **22 instructions / 76 bytes**: one call to already-translated `0x1FB3B0`, two return sites (`0x1F6A9A` and `0x1F6AB9`), no tails or unresolved branches.

## Seed and preflight

- Seed: **22 instructions / a 76-byte instruction-only envelope**, one call, no tables, SHA-256 `187d2bdbdd4a46e4016666d60e9e0819d5321ff5131790636a0186639c9b7f41`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 22 rows; **22/22 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 20-function seed â†’ **21 functions**, SHA-256 `ca4e5c987f02a3986ee7cfc7759d1f4268ab75ec2e375a06229b516d5298c704`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-72`: **445 functions / 18,426 instructions**; `0x1F6A70` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **535 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-93`: **699,510 bytes**, SHA-256 `855ce4ec17a8948a71b70547105a72f1525bc6b61f1b1bbf8130a8d0f7cbd1fe`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6730`** | 315 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +36 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6A70` executed; `0x1F6730` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **535 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6730` caller, so this is an indirect table boundary (index 131).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6A70_PACKAGE_MANIFEST.json` reports 32/32 seeds, 445 admitted / 535 generated / 0 rewrites, **32/32 digests**, pair ok (+36 routines / +1 operation), next stop `0x1F6730`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 21-function seed | `ca4e5c987f02a3986ee7cfc7759d1f4268ab75ec2e375a06229b516d5298c704` |
| `0x1F6A70` established-symbol seed | `187d2bdbdd4a46e4016666d60e9e0819d5321ff5131790636a0186639c9b7f41` |
| `generation.json` | `50fe0c5378c13958cc7d274ec6796e905e41b3b72ac6306329899b7a0024615c` |
| `title-functions.inc` | `c6d2e8c79c0789287d5854821883386a43b8ee4b4db9e18b55c13e57b3f69c5d` |
| `title-profile.h` | `c9b0c138831c9ee456033141cfa44691dd03594f183319d4e900926293fbc67e` |
| Module 93 | `855ce4ec17a8948a71b70547105a72f1525bc6b61f1b1bbf8130a8d0f7cbd1fe` |
| Control report | `33c0f758d490886589012e77e80ccae409009c2f90a2ecf9736610b8471b9f56` |
| Experiment report | `206dd144a1cd36b0e8acf33ddb09046b1ce91a739433c754e2290ea4dd318ee7` |

## Next and limits

The observed stop `0x1F6730` is the next ordinary indirect render-state entry (table index 131); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6A70` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
