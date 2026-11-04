# Frontier: `0x1FCCB0` read-only characterization (indirect dispatch)

November 4, 2026. Bounded read-only analysis of the next reached routine from the retained executable. This is not an export, admission, translation or execution. Raw disassembly and the analysis script remain under ignored `research/verification/frontier-1fccb0/`.

## Method

- Executable SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`; the entry maps into the `D3D` section.
- A direct-branch worklist started at `0x1FCCB0`, bounded to 512 instructions and an 8 KiB window. It records direct calls and branches, stops at returns, and reports indirect or undecodable sites instead of guessing.
- The worklist stopped at an **indirect jump through a table**; it did not follow or guess the table entries.

## Result

| Item | Value |
| --- | --- |
| Entry | `0x1FCCB0` |
| Section | `D3D` |
| Reachable direct instructions | 7 |
| Reachable direct bytes | 26 |
| Return site | `0x1FCCF8` |
| Calls | none |
| Indirect jump | `0x1FCCC0`, `jmp dword ptr [eax*4 + 0x1FCCFC]` |
| Jump table | `0x1FCCFC` (entries not mapped) |
| Selector bound | `0x8` with `ja`; immediate `0x7` |
| Unresolved | table entries and case bodies |

The retained control/experiment pair reaches this routine from executed `0x1F5640` at return `0x1F576A`.

Static reading only: this is a small **switch dispatcher**. It bounds a selector, scales it by four and jumps through the table at `0x1FCCFC`. The table entries and the case bodies are not characterized, so the reachable set is incomplete by construction. A future admission needs the table/case mapping and the translator's table-resync handling, unlike the previous leaves.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The worklist covers direct branches inside a bounded window; the indirect edge is explicitly unresolved.
- `0x1FCCB0` remains **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- This is preparation for a future authorized admission, not progress toward one.

## Next

Do not export or admit this dispatcher from the direct stream alone. A future directive would need the jump-table entries, the case blocks and an explicit resync/envelope plan before recomposition.
