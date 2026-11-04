# The reset run enters one frontier placeholder: 0x1FCB00

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
