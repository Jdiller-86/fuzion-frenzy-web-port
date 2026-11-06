> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Frontier: `0x1FC780` read-only characterization

November 4, 2026. Bounded read-only analysis of the next reached routine from the retained executable, using the project's pinned vendored decoder and XBE parser. This is not an export, admission, translation or execution. Raw disassembly and the analysis script remain under ignored `research/verification/frontier-1fc780/`.

## Method

- Executable SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`; the entry maps into the `D3D` section.
- A direct-branch worklist started at `0x1FC780`, bounded to 512 instructions and an 8 KiB window. It records direct calls and branches, stops at returns, and reports indirect or undecodable sites instead of guessing. No external call, tail target or unresolved branch was encountered.
- The only call is a self-recursive call at `0x1FC7A6`.

## Result

| Item | Value |
| --- | --- |
| Entry | `0x1FC780` |
| Section | `D3D` |
| Reachable instructions | 17 |
| Reachable bytes | 53 |
| Return site | `0x1FC7B2` (`ret 4`) |
| Calls | one self-recursive call at `0x1FC7A6` |
| External calls / tails / indirect branches | none |
| Flag masks | `0x50000`, `0x70000`, `0x780000`, `0x80000` |
| Internal targets | `0x1FC780`, `0x1FC7AB` |

The retained control/experiment pair reaches this routine from executed `0x1F5640` (`D3DDevice_SetRenderTarget`) at return `0x1F56FD`. The stop's stack arguments are recorded in the retained report; they are not consumed here.

Static reading only: the routine reads a state word from its argument, tests the listed masks, optionally follows a linked word at `+0x14` and recurses, then sets the `0x80000` flag. That is consistent with a small state-flag propagation helper, but it does not establish its semantics, ABI, aliasing or original-run behavior.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The worklist covers direct branches inside a bounded window. It does not prove external entry points, hidden jump tables or indirect edges, and it is not a decompilation.
- `0x1FC780` remains **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- This is preparation for a future authorized admission, not progress toward one.

## Next

If a fresh architect directive authorizes the next admission, export this single recursive leaf with the existing byte/decoder/layout/memory guards and compare the same-module pair. Do not infer its semantics or a console default from this characterization.
