> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Explicit translation budget extended to 32,768; execution reaches 0x1F78E0

November 4, 2026. The guarded translation had reached 16,283 of its 16,384-instruction cap, so the next reached function could not be admitted. The explicit aggregate budget is now selectable at 32,768 in the source-owned composer and closure exporter, with tests. `FUN_001FD200` was exported and execution advanced to `0x1F78E0`, with 270 distinct routines and 403 kernel operations. **There is no playable screen, renderer or device model.**

## Source-owned change

- `tools/prepare-title-recomp.py` accepts `--instruction-limit 32768`; `validate_instruction_count` admits `(8192, 16384, 32768)` and still rejects counts beyond the chosen limit and unsupported limits.
- `tools/ghidra/ExportRecompilationClosure.java` admits a prior generation with `instructionLimit` 32,768; the per-closure discovery budget remains capped at 8,192 and the function/export limits are unchanged.
- `tests/title_seed_test.py` adds 32,768 acceptance and `32,769` rejection cases.
- The budget remains explicit and bounded; there is no unbounded or automatic escalation.

## Export and run

- Closure for `FUN_001FD200`: 2 functions / 196 instructions.
- Guarded recomposition: 402 functions / 16,479 instructions.
- Private module `recomp-post-memory-build-66`: 606,679 bytes, SHA-256 `02086f5ee4c2c8b202a95d6e327567f0b5ace3502206a1517fe8717e4f9dd14c`.
- Profile run: status 2 at `0x1F78E0` (return `0x1FB8D6`), **270** distinct routines, 83/83 initializers, 403 kernel operations.
- No-profile run: status 13 at `0xFD800044`, 261 routines, 402 operations.

## Process note

The first closure output reused `research/verification/recomp-post-memory-next-08/seed.json`, which was already one of the 32 exports, and overwrote it. The original seed was restored from the retained main-project copy before recomposition; the final 32-export set used the restored seed plus `merge-05`. No source file or evidence digest was lost, but future closure outputs must use a new, unused directory.

## Further translation

- A closure for `0x1F78E0` added 1 function / 13 instructions; the guarded recomposition is 403 functions / 16,492 instructions.
- Private module `recomp-post-memory-build-67`: 608,098 bytes, SHA-256 `834c93937d575f8ab9af34d77a12205c9ad2cd78b0ee3e13634dd4a19a03badc`.
- Profile run: status 2 at `0x1F6240` (return `0x1FB92C`), **272** distinct routines, 83/83 initializers, 403 kernel operations.
- No-profile run: status 13 at `0xFD800044`, 261 routines, 402 operations.
- `0x1F6240` is the next export.

## Limits

No rendering, audio, fidelity or playability is established. The budget extension admits more original code but does not establish a complete runtime or device model. The translation and modules remain private; only the tool/test changes are published.

## Validation

`tests/title_seed_test.py` 25/25; `tests/recomp_memory_access_test.py` 11/11; owned XMM boundary test optimized and ASan/UBSan; runtime suite 110/110; Python memory-access 10/10; `npm run check` and whitespace checks pass.

## Next task

Export the reached helper `0x1F6240`, recompose with `--guard-memory` and the 32,768 budget, build and rerun. Do not bypass guard-memory or claim rendering/fidelity without evidence.
