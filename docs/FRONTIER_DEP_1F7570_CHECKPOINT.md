> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The `0x1F7570` batch completes the render-state table and stops at `D3DDevice_Present`

November 5, 2026. Published head `45c0327` (CI run `41992902803`, success) was the starting point. This cycle admitted the last two render-state handlers, `0x1F7570` and `0x1F7590`, and the experiment then left the indirect table entirely, stopping at **`0x00202150` (`D3DDevice_Present`)**. **There is no controllable original screen.** Per the directive, this is the post-table stop and the cycle halts for review; the next region is a fresh decision.

## Selection and establishment

- `0x1F7570` is `D3DDevice_SetRenderState_RopZRead`, dispatch table index **142**; `0x1F7590` is `D3DDevice_SetRenderState_DoNotCullUncompressed`, index **143**.
- The table ends after index 143: index 144 (`0x202630`) onward is non-pointer data. These two entries were the last unadmitted handlers.
- Neither address had a function. One bounded create-function was made at each address only (`body 0x1F7570..0x1F7580` and `body 0x1F7590..0x1F75A0`); both reported `Save succeeded`, `TABLE_ENTRY 00202628 index=142` / `0020262C index=143`, `TABLE_MATCHES 1`.
- Both are **4 instructions / 17 bytes**: store the argument to `0x203B88` / `0x203B8C` respectively, call the already-admitted `0x1F7230` (`D3D_CommonSetDebugRegisters`), and `RET 4`.
- No other function was created; the original executable is unchanged.

## Seeds and preflight

| entry | instructions | bytes | SHA-256 |
| --- | --- | --- | --- |
| `0x1F7570` | 4 | 17 | `8f6f04962bb9ea6035f97ac36e25a4fd9f94086d5fc2219434efd18eaec1f4ed` |
| `0x1F7590` | 4 | 17 | `024f834bf9ab9e30a3ae4a0f1c89a462b3b1ad9fa7fe3ce96fff945c3a3c34de` |

Both preflights are clean: `mixed False`, 0 tables, 4/4 decoded, no normalizations; byte identity, decoder agreement and layout pass.

- Grouping: both appended to the merged 30-function seed → **32 functions**, SHA-256 `8f1b7667f2bb6b348412280e930f9b2a85793a8dda903eac3dcd1cb4b34dea78`; the 32-function front seed and 32-export count are unchanged.
- Translation `recomp-title-recomp-title-post-memory-81`: **456 functions / 18,617 instructions**; both entries have real guarded bodies; 90 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **546 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-102`: **705,414 bytes**, SHA-256 `27ceaa9eb30f413d456692333f6d303fbdc75db780b829b88617d355b3489453`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x00202150`** | 332 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +53 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7570` and `0x1F7590` both executed; `0x202150` was not entered. Frontier: **546 functions, 90 placeholders, 89 referenced**, 354 direct call targets.

## Observed post-table stop

The experiment's stop is qualitatively different from every previous one:

- Status **2**, not 3. In the retained harness, status 2 is `ff_probe_stop_direct(target)` — a translated direct call whose target has no translated body; status 3 is the indirect-call failure path. This stop is therefore an ordinary **direct call**, not an indirect dispatch miss.
- Target `0x00202150` carries the symbol **`D3DDevice_Present`** and decodes as code (`SUB ESP,0x8` at `0x202150`); it is not a data address.
- The call site is `0x42C5E` (return `0x42C63`) inside `FUN_00042be0`, directly after a `CALL 0x001FC1B0`; static references to `0x202150` are four direct calls (`0x41F3F`, `0x42C5E`, `0x42481`, `0x4249D`).
- `0x202150` was already an established Ghidra function (`body 0x202150..0x2022D0`), but it was not admitted, so the run stops there honestly. [The `0x202150` batch](FRONTIER_DEP_202150_CHECKPOINT.md) later admitted it.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_1F7570_PACKAGE_MANIFEST.json` reports 32/32 seeds, **456 admitted / 546 generated / 0 rewrites**, **43/43 digests**, pair ok (+53 routines / +1 operation), next stop `0x202150`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Merged 32-function seed | `8f1b7667f2bb6b348412280e930f9b2a85793a8dda903eac3dcd1cb4b34dea78` |
| `0x1F7570` established-symbol seed | `8f6f04962bb9ea6035f97ac36e25a4fd9f94086d5fc2219434efd18eaec1f4ed` |
| `0x1F7590` established-symbol seed | `024f834bf9ab9e30a3ae4a0f1c89a462b3b1ad9fa7fe3ce96fff945c3a3c34de` |
| `generation.json` | `7dba7abe5994a1ae07673e347d82100e8a1f33b060ed7b217365ca6e0d029322` |
| `title-functions.inc` | `0c4aca697ab3ab8ad5046c7d7fcfd53ab8a8e4bcf563f9eb5c9337ee22afeefa` |
| `title-profile.h` | `2e9a08650987a43a8136ef3f16a8e856c01c4fed1c953da5f6218fb7636de851` |
| Module 102 | `27ceaa9eb30f413d456692333f6d303fbdc75db780b829b88617d355b3489453` |
| Control report | `0b6ddd18b025b9d55b92fc837c15d6fecaf140cdf347e068084607a76741a1cc` |
| Experiment report | `936452a468bc8f81f8d0c710d2b903b416e1f35b8e86b80ed2797cc6915e80b4` |

## Next and limits

The render-state table is **complete in the observed run**: the experiment reaches its end, and index 144 onward is non-pointer data. At this checkpoint 27 of the 28 handlers were translated: index 134 (`0x1F65F0`, `MultiSampleType`) is never set by this profile and remained untranslated. [The 0x1F65F0 batch](FRONTIER_DEP_1F65F0_CHECKPOINT.md) subsequently closed that gap (28/28). The observed next stop `0x00202150` (`D3DDevice_Present`) is a **post-table direct call**, and this checkpoint deliberately does not admit it. The next region is a fresh architect decision, not an automatic continuation.

These are synthetic startup observations, not original service semantics, fidelity, renderer, gameplay or playability. `D3DDevice_Present` being reached does not establish that presenting, rendering or a playable frame works.
