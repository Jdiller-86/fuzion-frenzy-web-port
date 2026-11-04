# The 0x1F79A0 admission advances the stop to 0x1F6EB0

November 4, 2026. A bounded read-only export admitted only the reached leaf `0x1F79A0` (281 body bytes / 289-byte envelope / 95 instructions, no calls) into the retained translation. `recomp-title-recomp-title-post-memory-50` now contains 419 functions / 17,448 instructions from the same 32 seed exports, with original-byte identity, decoder agreement and 419 checked memory accesses retained.

Module `recomp-post-memory-build-71`, SHA-256 `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`:

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F6EB0`** | 281 | 405 |

The frontier on the new translation has 98 placeholders (97 referenced) and 352 call targets. The control enters 0 placeholders (66 called by an executed caller, 31 static-only). The explicit run executes 281 functions and enters exactly one placeholder: **`0x1F6EB0`, called by executed original function `0x1F83C0`** (68 called-by-executed, 28 static-only). The `0x1F79A0` function executed and returned (trace enter/exit), so the next genuine stop is `0x1F6EB0`.

This is static reachability plus the retained report; it does not prove the call edge dynamically beyond the status-2 stop. `0x1F6EB0` is **not** admitted, translated, exported or treated as progress. No further admission, framework, storage expansion or fake success is added; the geometry lane stays paused.

Reproduce with `node tools/translation-frontier.mjs research/verification/recomp-title-recomp-title-post-memory-50 --run research/verification/pfifo-setup-01/browser-71-reset-avquery.json`; the text output names `ENTERED 0x001F6EB0 called by 0x001F83C0`.

An independent narrow GLM review (attempted `2026-11-04T09:15:22`; step-capped after completing the checks) found no material defect in the prequery fix or the admission evidence. It confirmed the experiment's `0x1F79A0` enter/exit trace, the `0x1F6EB0` status-2 stop, the 281/405 counts and the module digest; the control stop target `0xFE000094` was then confirmed directly from `browser-71-reset3.json`. The review treats `0x1F6EB0` as a static-reachability boundary, not a proven dynamic edge beyond the stop.

The source-owned admission guards were re-run against the new translation: `verify-seeds.py` reports **32 recorded / 32 offered / 0 unexpected / 0 missing** (the offered seeds match the translation's record exactly), and `verify-translation.py` reports **517 functions, 0 needing rewriting, fully instrumented**.

## Prior: The binding advances the frontier to translation target 0x1F79A0

November 4, 2026. The explicit synthetic ordinal-2 binding moves the frontier from the kernel service to a translation target. On module `recomp-post-memory-build-70` (`cd2fac97df95a4289b20742768555a080a995e2f16c774a36da25a856720a1a0`):

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F79A0`** | 280 | 405 |

The retained translation `recomp-title-recomp-title-post-memory-49` has 517 `ff_title_*` definitions (418 non-placeholder functions), 99 placeholders (98 referenced) and 352 call targets. With the control run, 279 functions execute, the run enters 0 placeholders, 66 placeholders are called by an executed caller and 32 have only unexecuted callers. With the explicit-word run, 280 functions execute and the run enters exactly one placeholder: **`0x1F79A0`, called by executed original function `0x1F83C0`**. The frontier partition is 69 called-by-executed and 28 static-only.

This is static reachability plus the retained run report; it does not prove the call edge was taken dynamically beyond the status-2 stop. `0x1F79A0` is **not** admitted, translated, exported or treated as progress. The next dependency is now a translation target rather than the kernel service; admitting it or its dependencies remains frozen pending the architect's decision. The canonical path still stops at ordinal 2, the geometry lane stays paused, and no default or fidelity claim is added.

Reproduce with `node tools/translation-frontier.mjs research/verification/recomp-title-recomp-title-post-memory-49 --run research/verification/pfifo-setup-01/browser-70-reset-avquery.json`; the text output names `ENTERED 0x001F79A0 called by 0x001F83C0`.

## Prior: Ordinal 2 remains the frontier; the option-6 model is withdrawn

November 4, 2026. The reset run stops at **status 1**, target `0xFE000094`, ordinal **2** (`AvSendTVEncoderOption`), return `0x1FD429`, with 279 routines and 404 kernel operations. The frontier reports 99 placeholders (98 referenced) and the run enters **0** of them: 66 are called by an executed caller and 32 have only unexecuted callers. The next dependency is the kernel service, not a translation target.

Module `recomp-post-memory-build-70`, SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`.

## Withdrawn option-6 model

The explicit AV-pack profile, its handler and its report fields are removed. Cxbx-Reloaded at commit `585c49a50af1255ab155099e06f24505f9c5a800` maps numeric option 6 to `AV_QUERY_AV_CAPABILITIES` and handles it, while the pinned `xboxrecomp-source` maps the same numeric 6 to `AV_OPTION_QUERY_AVPACK`. The replacement semantics differ, so option 6 is neutral and ordinal 2 is unsupported. Untranslated `0x1F79A0` is **not** reached, and no frontier expansion was performed. See [the AV encoder checkpoint](AV_ENCODER_SERVICE_CHECKPOINT.md).

## Next task

Obtain original-console or otherwise authoritative evidence for ordinal-2 option-6 semantics, or a definitive adjudication of the reference conflict, before any service model. Do not export `0x1F79A0` as grounded progress, do not invent a default pack word, and do not take a broad batch.

## Prior: the 0x1FD42F admission reaches AvSendTVEncoderOption (ordinal 2)

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-49` admits the next reached helper `0x1FD42F` (49 instructions) and its direct dependency `0x1FD410` (10 instructions) for **418 functions / 17,353 instructions**. The reset run executes both and stops at **status 1** (unsupported Xbox service), target `0xFE000094`, ordinal **2**, return `0x1FD429`, with 279 routines and 404 kernel operations. The retained callers evidence names ordinal 2 as `AvSendTVEncoderOption`; no encoder behaviour is modeled.

The frontier now has 99 placeholders (98 referenced), but the run enters **0** of them: the next blocker is the kernel service, not a translation target. 66 placeholders are called by an executed caller and 32 have only unexecuted callers; none can advance this run while ordinal 2 is unsupported.

## Narrow admission

- One merged seed within the existing 32-export budget, recomposed with `--guard-memory`.
- `0x1FD42F` calls `0x1FD410`; `0x1FD410` calls ordinal 2 through IAT `0x00253450`. No other dependency was added.
- `tools/verify-seeds.py`: 32 recorded / 32 offered / 0 unexpected / 0 missing.
- `tools/verify-translation.py`: 0 functions needing rewriting.
- Module `recomp-post-memory-build-70`, SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`.

## Measured result

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pcrtc` control | status 15 at `0xFD003214` | 276 | 403 | 32,768 |
| `pfifoReset` | **status 1 at `0xFE000094`** (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`.

## Limits

- No encoder capabilities, mode, timing or return values are guessed.
- One helper and its direct dependency were added; no broad closure or frontier batch.
- No display, renderer, fidelity or playability.

## Next task

Do not export more frontier targets for this run: the next dependency is the kernel service. Decide whether to model a bounded, evidence-backed `AvSendTVEncoderOption` service or take another independent prerequisite. Keep the canonical profile fail-closed and the geometry lane paused.

## Prior: The 0x1FCB00 admission advances the stop to 0x1FD42F

November 4, 2026. The first evidence-based admission after the reset breakthrough is complete. Translation `recomp-title-recomp-title-post-memory-48` admits exactly the reached helper `0x1FCB00` (10 instructions, a leaf that returns `0xF` for inputs `3`/`0x1C`, otherwise reads `[EAX + 0x2024F0]` and masks `0x3C`) and contains **416 functions / 17,294 instructions**. The reset run now executes it and stops at the next reached helper, untranslated `0x1FD42F` (status 2, return `0x1FD4C9`, 277 routines, 404 kernel operations). The frontier filter reports **entered = 1** for `0x1FD42F` (called by `0x1FD4A8`), so the next admission is evidence-based.

## Narrow admission

- One seed function merged into an existing export; 32 exports total. No broad closure, frontier batch, consumer or guessed timing.
- The helper has no calls, so no dependencies were needed.
- `tools/verify-seeds.py` reports 32 recorded / 32 offered / 0 unexpected / 0 missing; `tools/verify-translation.py` reports 0 functions needing rewriting.
- The `--guard-memory` recomposition preserves the byte/decoder/layout/memory/ABI guards.
- The canonical, `engineWait` and `pcrtc` controls are unchanged.

## Measured result

Module `recomp-post-memory-build-70`, SHA-256 `2c0b69dfbf55d0de2a183fd05c951283a8cc7f847fab3d9235f89ea0434dfb22`.

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pfifoReset` | **status 2 at `0x1FD42F`** (return `0x1FD4C9`) | 277 | 404 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`.

## Limits

- This is one helper, not a closure or a playable path.
- The admitted helper's byte read at `[EAX + 0x2024F0]` is exercised only as reached; no broader table ownership is claimed.
- No engine, renderer, timing source, fidelity or playability.

## Next task

Export `0x1FD42F` (and only demonstrably necessary dependencies) from the retained project, recompose with the guard, build and rerun. Keep the canonical profile fail-closed.

## Prior: the reset run enters one frontier placeholder: 0x1FCB00

November 4, 2026. With the default-off `pfifoReset` experiment the run leaves the PFIFO wait and stops at untranslated `0x1FCB00` (status 2, return `0x1FD4C4`). The reachability filter reports **entered = 1** (`0x1FCB00` called by `0x1FD4A8`), 68 called by an executed caller and 32 with only unexecuted callers. This is the first evidence-based next admission target; **no frontier expansion was performed this cycle**.

## Source-owned tool fix

A status-2 stop target is the untranslated routine itself and does not appear in `originalFunctionsReached`. `parseRunEvidence` now adds it to the entered set, and `tests/translation-frontier.test.mjs` covers a stop target absent from the reached set.

## Measured result

| item | pre-reset | reset |
| --- | --- | --- |
| executed functions | 276 | 276 |
| placeholders entered | 0 | **1 (`0x1FCB00`)** |
| called by an executed caller | 66 | 68 |
| only unexecuted callers | 35 | 32 |

The reset run executes `0x1FD4A8`, which statically calls `0x1FCB00`; the run enters the placeholder and stops with status 2. The canonical profile still reports 0 entered.

## Limits

- "Called by an executed caller" remains a static upper bound.
- The entered target is one run's evidence; a different profile or a later run changes it.
- The filter does not translate, build, execute, or prove fidelity.

## Next task

Export `0x1FCB00` and its closure, recompose with the guard, build, and rerun with the `pfifoReset` profile. Do not export the remaining untaken-branch targets.

## Prior: pre-reset static-only run

November 4, 2026. The static frontier is now intersected with a retained run report, so the next translation batch can be scoped by executed callers instead of every static target. Against `browser-70.json` (276 executed functions), **0 of 101 referenced placeholders were entered**, 66 have at least one statically-calling function that executed, and 35 have only unexecuted callers. The 66 are untaken branches, not reachable calls: the run stops at the PFIFO wait, not at an untranslated routine. **There is no engine, renderer, timing source or playable screen.**

## Source-owned change

`tools/translation-frontier.mjs` gains `--run <report.json>` and the exported `parseRunEvidence` / `analyzeReachability`:

- accepts `originalFunctionsReached` and/or a `trace`; validates the report object, address range and `enter`/`exit` kinds;
- uses only entered addresses from the trace, because the retained traces contain unmatched exits from tail calls and returns; it deliberately does not reconstruct a call stack or claim dynamic call edges;
- partitions each referenced placeholder into `entered`, `calledByExecutedCaller` (a static upper bound) and `staticOnly`;
- reports the executed-function count and warns when the trace reached its declared capacity;
- `--json` includes `reachability`; the human output prints counts and `ENTERED` / `EXECUTED-CALLER` addresses only.

`tests/translation-frontier.test.mjs` adds five cases (9 total): the three-way partition, malformed addresses and event kinds, tolerance of an unmatched exit, a trace-only report with truncation, and a CLI test that proves `--run` is not mistaken for the translation path. No original code is reproduced.

## Measured result

Report `browser-70.json`, translation `recomp-title-recomp-title-post-memory-46`:

| item | count |
| --- | --- |
| translation functions (including 102 placeholders) | 517 |
| referenced placeholders | 101 |
| executed functions | 276 |
| placeholders entered | **0** |
| called by an executed caller (untaken branch) | 66 |
| only unexecuted callers | 35 |

The 66 cluster on `0x2CD80` (62), with singletons from `0x34307` (`0x341D0`), `0x1E0DAB` (`0x1E0E52`), `0x200F59` (`0x20010A`) and `0x42BE0` (`0x202150`). Because the run enters none of them, admitting any of these placeholders cannot advance the current execution; it would only prepare untaken branches. The current blocker remains PFIFO consumption, owned by the engine workstream.

## Limits

- "Called by an executed caller" is a static upper bound: the caller's branch may not be taken.
- The executed set is one profile's run; a different profile or a later run changes it.
- The filter does not prove reachability from the current stop, decode a call stack, translate, build or execute anything.
- No engine, renderer, timing source, rendering, fidelity or playability.

## Validation

`tests/translation-frontier.test.mjs` 9/9; runtime suite 131/131; `npm run test:evidence` 4/4; `npm run test:native` 14/14; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Wait for a run that actually enters a placeholder (then `entered` is non-empty) before exporting that address, or for the engine workstream to unblock PFIFO consumption. Do not export the 66 untaken-branch targets as if they were reachable.

## Static inventory

November 4, 2026. An inventory of the boundary placeholders in the current translation quantifies the remaining admission work. It is a planning datum only: nothing here is a fidelity or progress claim, and no new routine was translated. **There is no engine, renderer, timing source or playable screen.**

## What was measured

The regenerated translation (`recomp-title-recomp-title-post-memory-46`, 415 functions / 17,284 instructions) emits one placeholder per known boundary: `void ff_title_XXXXXXXX(void) { ff_probe_stop_direct(0xXXXXXXXXu); }`. Counting those against the call graph of the admitted functions gives:

| item | count |
| --- | --- |
| boundary placeholders (known-untranslated routines) | **102** |
| placeholders that are direct call targets of admitted code | **101** |
| distinct call targets across admitted code | 352 |

Nearly every known boundary is already referenced by admitted code, so these are the next translation frontier rather than hypothetical gaps. A run that reached one would stop with status 2 (untranslated original routine); the current runs do not, because the 276 reached routines stay inside the admitted set.

## Why this matters for the blocked path

The reached loop's exit needs PFIFO engine state (unassigned owner), but when that unblocks, execution moves into code whose callees include 101 known-untranslated routines. This list lets the next translation batch be scoped from evidence instead of being rediscovered at the stop.

## Inventory

Placeholder addresses from the private translation — addresses only; no original code, bytes or disassembly is reproduced:

```
  0x00013A10, 0x00018350, 0x00026C60, 0x00027540, 0x0002B470, 0x0002B4B0
  0x0002B520, 0x0002B640, 0x0002B790, 0x0002B870, 0x0002B8A0, 0x0002C020
  0x0002C270, 0x0002C400, 0x0002C460, 0x0002C520, 0x0002C5B0, 0x0002C680
  0x0002C8E0, 0x0002CC90, 0x0002FA20, 0x00030603, 0x0003080D, 0x00030943
  0x0003097D, 0x00031227, 0x000341D0, 0x00036E70, 0x000393E0, 0x00041EE0
  0x00042530, 0x00042570, 0x00042590, 0x00043440, 0x000438B0, 0x00043B70
  0x00043D20, 0x00047070, 0x00047CB0, 0x00047DE0, 0x00048D60, 0x0004AE80
  0x0004B110, 0x0004B160, 0x0004B210, 0x0004B230, 0x0004B540, 0x0004B620
  0x0004B7A0, 0x0004BB30, 0x0004BE40, 0x0004C480, 0x0004C5F0, 0x0004D4F0
  0x0004D620, 0x0004EED0, 0x0004F710, 0x0004F7A0, 0x0004F860, 0x0004F890
  0x0004F8B0, 0x0004F8C0, 0x0004FF30, 0x00050150, 0x00050250, 0x000502A0
  0x00055240, 0x000559A0, 0x00071B10, 0x000762E0, 0x00082860, 0x00082870
  0x00082A00, 0x000A6FA0, 0x001DF03E, 0x001E056D, 0x001E0E52, 0x001E2038
  0x001E50E3, 0x001E55A3, 0x001E6DA9, 0x001E6DED, 0x001F54D0, 0x001F5B70
  0x001F6EB0, 0x001F7080, 0x001F70E0, 0x001F79A0, 0x001F8090, 0x001F80F0
  0x001FB320, 0x001FC580, 0x001FC780, 0x001FC910, 0x001FC980, 0x001FCB00
  0x001FCB70, 0x001FCCB0, 0x001FD410, 0x001FD42F, 0x0020010A, 0x00202150

## Limits

- The list reflects the current seed set; a different set changes it.
- Being referenced is not the same as being reachable on the current runtime path: static call targets include conditional branches today's run never takes.
- No placeholder was translated, no seed added, and no module rebuilt for this audit.

## Validation

Documentation-only change; the standing suites still pass (`npm run test:evidence` 3/3, `npm run test:native` 14/14, runtime suite 114/114, title-seed 25/25, `npm run check`, whitespace).

## Next task

Scope the next translation batch from this inventory — largest referenced clusters first — and keep the PFIFO engine/consumer assignment separate, since neither substitutes for the other.
