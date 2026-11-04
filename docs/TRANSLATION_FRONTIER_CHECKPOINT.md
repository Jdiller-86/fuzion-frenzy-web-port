# The admitted translation has 102 known-untranslated callees; 101 are referenced

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
