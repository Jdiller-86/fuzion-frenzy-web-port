# The `0x1F69F0` batch advances the stop to `0x1F6A70`

November 5, 2026. This cycle admitted the next ordinary stop, `0x1F69F0`. It was already a Ghidra-established function and uses FPU operations and five calls to translated `0x1F66A0`. **There is no controllable original screen.**

## Selection and evidence

- `0x1F69F0` is `D3DDevice_SetRenderState_ZBias`, table index **129**, render state `0x81`.
- The function was already established (`body 001f69f0..001f6a63`), so no create-function was needed.
- The leaf is **41 instructions / 116 bytes**: five calls to already-translated `0x1F66A0`, FPU operations (`FILD`, `FADD`, `FCHS`, `FST`, `FMUL`, `FSTP`), one `RET 4` at `0x1F6A61`, no tails or unresolved branches.

## Seed and preflight

- Seed: **41 instructions / a 116-byte instruction-only envelope**, five calls, no tables, SHA-256 `a0e4178d55e6c00992468c19553fa8f0f4769f8c6efbf389a249e6f290cbf201`.
- Preflight with the pinned toolchain: `mixed False`, `tables 0`, 41 rows; **41/41 decoded**, no normalizations; byte identity, decoder agreement and layout pass.
- Grouping: append to the merged 19-function seed â†’ **20 functions**, SHA-256 `846cac1bd3a8518f346e2789f7e705d40f7049e891a2b24e11d9855dde4641b6`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-71`: **444 functions / 18,404 instructions**; `0x1F69F0` has a real guarded body with the FPU sequence; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **534 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-92`: **698,963 bytes**, SHA-256 `57e245de1a8ad896f57d1e67bee3fd69eb742f240561eeaacf059fac8f34e9d0`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6A70`** | 314 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +35 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F69F0` executed; `0x1F6A70` was not entered. The stop returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **534 functions, 90 placeholders, 89 referenced**; the direct map has no `0x1F6A70` caller, so this is an indirect table boundary (index 130).

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F69F0_PACKAGE_MANIFEST.json` reports 32/32 seeds, 444 admitted / 534 generated / 0 rewrites, **31/31 digests**, pair ok (+35 routines / +1 operation), next stop `0x1F6A70`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 20-function seed | `846cac1bd3a8518f346e2789f7e705d40f7049e891a2b24e11d9855dde4641b6` |
| `0x1F69F0` seed | `a0e4178d55e6c00992468c19553fa8f0f4769f8c6efbf389a249e6f290cbf201` |
| `generation.json` | `22e87b771acc6c37826ed62df68ff2fb0f3f55c2e22aa54a4b527a7c3fad51a1` |
| `title-functions.inc` | `a69caf3e77c609d7e8281f57884c5865ce7034753d7960f9450bd40b0228ad15` |
| `title-profile.h` | `fae677d3c6b2d327c22a5d4ac2a9c46e5b377c4efc0caa656530a80a8f1f644a` |
| Module 92 | `57e245de1a8ad896f57d1e67bee3fd69eb742f240561eeaacf059fac8f34e9d0` |
| Control report | `72e454c18324ebc3e3b686ec9a289089942d6b61b633ca4240afa8ddda30828d` |
| Experiment report | `acf444efa9f52f171c83d9612fac0c47c42c59eb0d4b69a9fe3d8ee3d2cd940b` |

## Next and limits

The observed stop `0x1F6A70` is the next ordinary indirect render-state entry (table index 130); it was not entered. A later â‰¤8-function batch may continue only while stops remain ordinary and after review; this cycle stops here. These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability.
