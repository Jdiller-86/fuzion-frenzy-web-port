# Frontier: `0x1F7080` read-only characterization

November 4, 2026. Bounded read-only analysis of the next reached routine from the retained executable. This is not an export, admission, translation or execution. Raw disassembly and the analysis script remain under ignored `research/verification/frontier-1f7080/`.

## Method

- Executable SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`; the entry maps into the `D3D` section.
- A direct-branch worklist started at `0x1F7080`, bounded to 512 instructions and an 8 KiB window. It records direct calls and branches, stops at returns, and reports indirect or undecodable sites instead of guessing.
- No external call, tail target, indirect branch or unresolved site was encountered.

## Result

| Item | Value |
| --- | --- |
| Entry | `0x1F7080` |
| Section | `D3D` |
| Reachable instructions | 27 |
| Reachable bytes | 89 |
| Return site | `0x1F70D8` |
| Direct calls | none |
| Tails / indirect branches | none |
| Memory operand addresses | `0x203B78`, `0x203B3C` |
| Internal branch targets | `0x1F7094`, `0x1F70A3`, `0x1F70C5`, `0x1F70CB` |

The retained experiment reaches this routine from executed `0x1F5640` at return `0x1F57C9` (call site `0x1F57C4`). Static reading only: a small leaf that reads the two listed words, tests flags and returns; it does not establish semantics, ABI or original-run behavior.

## Exported seed (read-only, not admitted)

The retained exporter reads `0x1F7080` as **27 instructions / an 89-byte envelope**, instruction-only layout, no calls or tables; SHA-256 `4968b4371b592b7269c20cf5e9804b474d3595a34b23fb231e189b121fb1d431`. Byte identity, independent decoder agreement (27/27, no normalizations) and `checked_function_layout` pass. The seed was not merged into the selection, and `0x1F7080` remains unadmitted.

## Review-ready preflight revalidation

Re-ran the retained seed preflight at `2026-11-04T20:26-05:00` with the pinned toolchain: seed SHA `4968b4371b592b7269c20cf5e9804b474d3595a34b23fb231e189b121fb1d431`, `mixed False`, `tables 0`, 27 rows; 27 instructions / 27 decoded / no normalizations; byte identity, decoder agreement and layout pass. This confirms the seed is admission-ready but does not admit it.

## Review-ready admission manifest (not authorized)

- Function `0x1F7080`, 27 instructions / an 89-byte instruction-only envelope; no calls, tails, indirect branches or tables.
- Seed SHA `4968b437…`, source `45bb49e1…`.
- Merge: append to the 29-function front seed → 30 functions in one export group; the 32-export / 512-function / 32,768-instruction budgets remain satisfied (expected 423 functions / 17,620 instructions).
- Acceptance: byte/decoder/layout identity; `verify-seeds` 32/32 (including the recovered seed); no `UNIMPLEMENTED` at `0x1F7080`; same-module control/experiment pair differing only in `avQueryWord` with comparator exit 0; record the next genuine stop.
- Stop: report the next genuine stop and stop; do not translate dependencies, infer semantics or claim original execution/fidelity.
- No seed merge, translation, module, report or execution was performed.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The worklist covers direct branches inside a bounded window; no indirect edge was found.
- `0x1F7080` remains **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- A one-leaf admission is plausible but is not authorized by this characterization.

## Next

If a fresh directive authorizes the next admission, export this single leaf with the existing byte/decoder/layout/memory guards and compare the same-module pair. Do not infer its semantics or a console default from this characterization.
