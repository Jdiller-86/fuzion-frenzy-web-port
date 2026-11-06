> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F6890` batch advances the stop to `0x1F7300`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F6890`. It was a named symbol without a Ghidra function, so one bounded create-function was required. **There is no controllable original screen.**

## Selection and establishment

- `0x1F6890` is `D3DDevice_SetRenderState_NormalizeNormals`, the next indirect render-state entry (table `0x2025D8`, **index 122**, render state `0x7A`).
- The retained project initially had the symbol but no function there.
- A bounded headless run created exactly one function at `0x1F6890` (`CREATED 001f6890 D3DDevice_SetRenderState_NormalizeNormals body=001f6890..001f68bc`) and reported `Save succeeded`.
- No other function was created or analyzed; the original executable is unchanged.
- The leaf is **13 instructions / 45 bytes**: one call to already-translated `0x1FB3B0`, one `RET 4` at `0x1F68BA`, no jumps, tails or unresolved branches.

## Seed and preflight

- Seed: **13 instructions / a 45-byte instruction-only envelope**, one call, no tables, SHA-256 `67887947840b7bc7e87f7b16cb6f746686e93cab91f6bc940edecfc77ca37150`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 13 rows; **13/13 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 12-function seed â†’ **13 functions**, SHA-256 `a41d2643124928a2137d73dde0a6e5f6b0d2d453204f5f2405eaecb5b47f49d1`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-65`: **437 functions / 18,195 instructions**; `0x1F6890` has a real guarded body; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **527 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-86`: **692,563 bytes**, SHA-256 `2a0ff87ba332b1dda66589dc4270b05dc7a5cda7fdcd77a469f558402d2117ed`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7300`** | 307 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +28 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6890` executed; `0x1F7300` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **527 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F7300` caller, so this is again an indirect table boundary (table entry for index 123).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F6890_PACKAGE_MANIFEST.json` reports 32/32 seeds, 437 admitted / 527 generated / 0 rewrites, **24/24 digests**, pair ok (+28 routines / +1 operation), next stop `0x1F7300`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 13-function seed | `a41d2643124928a2137d73dde0a6e5f6b0d2d453204f5f2405eaecb5b47f49d1` |
| `0x1F6890` established-symbol seed | `67887947840b7bc7e87f7b16cb6f746686e93cab91f6bc940edecfc77ca37150` |
| `generation.json` | `8f4adc4d68e72cb63006b1fe4abbaf3639c3c8a28944c6e6ea29143658230460` |
| `title-functions.inc` | `5955e59c237ec66ca11fbeb75c50a8947797d12a5c7241c8cae3b1a8586f1e5c` |
| `title-profile.h` | `ccc30cfd0c4f2c9ed688efe68484a6f3285723b34c3345de35b485e7fe9b31bc` |
| Module 86 | `2a0ff87ba332b1dda66589dc4270b05dc7a5cda7fdcd77a469f558402d2117ed` |
| Control report | `fad79436f6c79cf4c572946fe186509ba0c16d5554c7bfac5692c4df5dcf4495` |
| Experiment report | `74e28e10c8b393ce883265033dfc3487e233e21f830740eeb2f594558ad2b6d0` |

## Next and limits

The observed stop `0x1F7300` is the next ordinary indirect render-state entry (table index 123); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.

The persistent `0x1F6890` function is a bounded, evidence-based definition for one named indirect-dispatch leaf; it does not establish general function-recovery policy or permission to define arbitrary gaps.
