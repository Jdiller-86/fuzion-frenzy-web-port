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
| Jump-table mapping | 8 entries -> 4 distinct cases (`0x1FCCC7`, `0x1FCCD4`, `0x1FCCE1`, `0x1FCEEE`), repeated twice |
| Case blocks | 4 x 3 instructions, each `ret 8`, inside `0x1FCCB0..0x1FCCF8` |
| Envelope | code + 32-byte table at `0x1FCCFC` + `0x90909090` padding to `0x1FCD20` (~112 bytes) |

The retained control/experiment pair reaches this routine from executed `0x1F5640` at return `0x1F576A`.

Static reading only: this is a small **switch dispatcher**. It bounds a selector, scales it by four and jumps through the table at `0x1FCCFC`. The table entries and cases are now mapped: it selects one of four three-instruction constant-return blocks (`ret 8`). A future admission needs the mixed code/table/padding envelope and the translator resync handling; the direct stream alone is not enough.

## Exported seed (read-only, not admitted)

The retained exporter reads `0x1FCCB0` as **18 instructions / a 75-byte envelope** (`0x1FCCB0..0x1FCCFA`), no calls, SHA-256 `497317554657fc3afec91122db15678b5b87218e477d6516484661cd1d18d29e`. The 32-byte table at `0x1FCCFC` is **outside** the exported body and remains guest image data; a future admission must dispatch through the image table rather than a table-in-seed envelope. The seed was not merged into the selection, and `0x1FCCB0` remains unadmitted.

## Table-aware envelope accepted (read-only, not admitted)

The retained project already defines the eight table entries at `0x1FCCFC` as separate 4-byte pointer data. A read-only in-memory body extension (`0x1FCCB0..0x1FCD1F`) plus the existing exporter produces a **112-byte envelope**: 18 instructions, 8 pointer entries and 2 undefined spans; seed SHA-256 `be1dc290a49c30dfc2b3aa108e82f4efb55f112d2579eb4ea3d235252f042f2c`. The actual translator guards accept it: `checked_function_layout` reports `mixed=True`, one table at `0x1FCCFC..0x1FCD1C` with the eight targets; `checked_dispatch_tables` accepts the computed jump at `0x1FCCC0`. The seed was not merged into the selection and no recomposition or build was performed; `0x1FCCB0` remains unadmitted pending a fresh directive.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The worklist covers direct branches inside a bounded window; the table was read separately from the retained XBE, and the four case blocks are mapped.
- `0x1FCCB0` remains **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- This is preparation for a future authorized admission, not progress toward one.

## Next

Do not export or admit this dispatcher from the direct stream alone. A future directive would need the mapped mixed envelope and an explicit resync/layout plan before recomposition; the four case blocks are three instructions each and return 8.
