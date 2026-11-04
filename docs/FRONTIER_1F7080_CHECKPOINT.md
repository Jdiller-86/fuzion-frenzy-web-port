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

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The worklist covers direct branches inside a bounded window; no indirect edge was found.
- `0x1F7080` remains **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- A one-leaf admission is plausible but is not authorized by this characterization.

## Next

If a fresh directive authorizes the next admission, export this single leaf with the existing byte/decoder/layout/memory guards and compare the same-module pair. Do not infer its semantics or a console default from this characterization.
