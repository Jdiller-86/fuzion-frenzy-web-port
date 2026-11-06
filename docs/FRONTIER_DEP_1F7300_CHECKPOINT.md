> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F7300` batch advances the stop to `0x1F7370`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F7300`. It was already a Ghidra-established function, and all five calls are translated. **There is no controllable original screen.**

## Selection and establishment

- `0x1F7300` is `D3DDevice_SetRenderState_ZEnable`, the next indirect render-state entry (table index **123**, render state `0x7B`).
- The function was already established (`body 001f7300..001f736b`), so no create-function was needed.
- The leaf is **34 instructions / 108 bytes**: five calls (`0x1FB3B0` twice, `0x1F85E0`, `0x1F82C0`, `0x1F7080`, all already translated), four internal conditional branches/jumps, one `RET 4` at `0x1F7369`; no tails or unresolved branches.

## Seed and preflight

- Seed: **34 instructions / a 108-byte instruction-only envelope**, five calls, no tables, SHA-256 `8552c4757db47c0e343176dc3ff7a15a9eb506178041b9736331cb49543f423a`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 34 rows; **34/34 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 13-function seed â†’ **14 functions**, SHA-256 `a48daeda50b87793c8df28f254de204abd4fabc54eb844b43aded7a8b4dbbc2a`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-66`: **438 functions / 18,229 instructions**; `0x1F7300` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **528 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-87`: **693,474 bytes**, SHA-256 `3f001885c215ace4b866ae300a1333c1fd1c803e7afd4f9a7753e07beef66ee9`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7370`** | 308 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +29 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7300` executed; `0x1F7370` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **528 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F7370` caller, so this is again an indirect table boundary (index 124).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7300_PACKAGE_MANIFEST.json` reports 32/32 seeds, 438 admitted / 528 generated / 0 rewrites, **25/25 digests**, pair ok (+29 routines / +1 operation), next stop `0x1F7370`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 14-function seed | `a48daeda50b87793c8df28f254de204abd4fabc54eb844b43aded7a8b4dbbc2a` |
| `0x1F7300` seed | `8552c4757db47c0e343176dc3ff7a15a9eb506178041b9736331cb49543f423a` |
| `generation.json` | `bbb25bd499c3ccbd3551a73d8fdd9f68f0e3c43219ad8986544009eb2fef9aea` |
| `title-functions.inc` | `c77eb5665913cf27c5950f79a2bdd8ca3282687035e92ab4e325cca731f18575` |
| `title-profile.h` | `20376ff1709ea4d4d1ace9774b8a997a6f8b0ae7e03ccf3a846bdc612cae907b` |
| Module 87 | `3f001885c215ace4b866ae300a1333c1fd1c803e7afd4f9a7753e07beef66ee9` |
| Control report | `b7970663a6c7187aa1a06ef3b41b516b919243f543fef860b463455311277069` |
| Experiment report | `4ec068cca8587fafe1810d15f085bd04d0ca18fa10266498aaade4a9c83cde7e` |

## Next and limits

The observed stop `0x1F7370` is the next ordinary indirect render-state entry (table index 124); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.
