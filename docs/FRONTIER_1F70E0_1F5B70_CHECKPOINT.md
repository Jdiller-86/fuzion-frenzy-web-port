> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Frontier: `0x1F70E0` and `0x1F5B70` static continuation candidates

November 4, 2026. Bounded read-only analysis of the two static continuation candidates after the `0x1FCCB0` caller path. This is not an export, admission, translation or execution. Raw disassembly and analysis scripts remain under ignored `research/verification/frontier-1f70e0/` and `frontier-1f5b70/`.

## Method

- Each entry was analyzed with a direct-branch worklist in the retained `D3D` section, bounded to 512 instructions and an 8 KiB window.
- No indirect branch or unresolved branch was encountered. Direct call targets were classified against translation 53's admitted functions and `untranslatedDirectTargets`.

## Result

| Candidate | Shape | Return | Direct calls and status |
| --- | --- | --- | --- |
| `0x1F70E0` | 98 instructions / 329 bytes, FPU-heavy | `0x1F7228` | `0x1F66A0` admitted; `0x1FB3B0` admitted; `0x1FC980` untranslated; `0x1FCB70` untranslated |
| `0x1F5B70` | 118 instructions / 347 bytes | `0x1F5CC8` | `0x1F6300` unclassified direct target; `0x1F6EB0` admitted; `0x1F85E0` admitted; `0x1FB3B0` admitted |

- `0x1F70E0` was a static candidate from caller `0x1F5846`; `0x1F5B70` from caller `0x1F587D`.
- The taken branch after `0x1F7080` is unproven; the actual next stop will be observed only when the `0x1F7080` admission executes.
- Neither candidate is a single-leaf admission. `0x1F70E0` needs the two untranslated callees; `0x1F5B70` has an unclassified direct target at `0x1F6300`.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The call classification reflects translation 53 only; a different selection changes it.
- Both candidates remain **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.

## Next

`0x1F7080` remains the immediate preflighted leaf. A future admission after it should be scoped as a small batch with the candidates' callees, not as a single-leaf export.
