# Frontier: `0x1F6EB0` read-only characterization

November 4, 2026. Bounded read-only analysis of the next reached routine from the retained executable, using the project's pinned vendored decoder and XBE parser. This is not an export, admission, translation or execution, and it does not change the admission hold. Raw disassembly and the analysis script remain under ignored `research/verification/frontier-1f6eb0/`.

## Method

- Executable SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`; the entry maps into the `D3D` section.
- A direct-branch worklist started at `0x1F6EB0`, bounded to 512 instructions and an 8 KiB window. It records conditional and unconditional direct targets, stops at returns and reports indirect or undecodable sites instead of guessing. No indirect branch, tail target or undecodable site was encountered.
- A linear-only pass stopped early at an internal `jmp` that skips one FPU instruction; the direct-branch model resolves both return paths.

## Result

| Item | Value |
| --- | --- |
| Entry | `0x1F6EB0` |
| Section | `D3D` |
| Reachable instructions | 110 |
| Reachable bytes | 455 |
| Return sites | `0x1F7033`, `0x1F7076` |
| Direct calls | none |
| Tail targets | none |
| Unresolved/indirect branches | none |
| FPU-heavy mnemonics | `fadd` 8, `fmul` 10, `fstp` 14, `fld` 8, `fild` 4, `fsub` 3, `fchs` 1 |
| Memory operand addresses | `0x253C50`, `0x2025A4`, `0x203B68`, `0x203B60`, `0x2536D0`, `0x257B8C` |
| Data immediates | `0x80394`, `0x100A20`, `0x100AF0` |

The routine is a float-heavy leaf with no direct calls. The retained caller `0x1F83C0` (`D3DDevice_SetVertexShader`) initializes a buffer and passes it in; this routine reads and writes device/data addresses and returns through two paths. That is consistent with a transform/state helper, but the analysis does not establish its semantics, ABI, consumed fields or original-run behavior.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- The worklist covers direct branches inside a bounded window. It does not prove external entry points, hidden jump tables or indirect edges, and it is not a decompilation.
- `0x1F6EB0` remains **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- The architect's admission hold remains in force; this record is preparation for a future authorized admission, not progress toward one.

## Next

If the architect authorizes the next admission, export this single leaf from the retained project with the existing byte/decoder/layout/memory guards. No dependency was observed, so the selection may stay one function; a rebuild must still show the same control/experiment pair and the next genuine stop. Do not infer the helper's semantics or a console default from this characterization.
