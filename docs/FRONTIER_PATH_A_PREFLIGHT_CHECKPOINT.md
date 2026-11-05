# Path A preflight: `0x1F70E0` and its two untranslated callees

November 4, 2026. Read-only preparation after the `0x1F7080` admission observed the taken branch at untranslated `0x1F70E0`. This is **not** a Path A admission or batch: no seed merge, translation, module, report or execution was performed. **There is no controllable original screen.**

## Observed function

- `0x1F70E0` seed: **98 instructions / a 329-byte instruction-only envelope**, no computed jumps or tables; SHA-256 `ee62446fbc381b3b377d683b707af61aac5459f5f79f84e9afcb3dc0e58d92ed`.
- Preflight at `2026-11-04T21:40-05:00`: 98 instructions / 98 decoded / no normalizations; byte identity, decoder agreement and `checked_function_layout` pass.
- Four direct calls:
  - `0x1FC980` — **untranslated** (shared 2-instruction / 9-byte leaf; preflight 2/2).
  - `0x1FCB70` — **untranslated** (40 instructions / 156-byte body; table-aware envelope 304 bytes with 11+2 pointer tables, 80 data bytes; layout and dispatch census pass).
  - `0x1F66A0` — already translated in translation 54.
  - `0x1FB3B0` — already translated in translation 54.
- No additional untranslated callees were found.

## Grouping and budgets (proposed, not applied)

Current translation 54: 32 exports / 423 functions / 17,620 instructions. Adding the three functions needs 426 functions / 17,760 instructions, within the 512-function and 32,768-instruction limits. The 32-export and 32-functions-per-export limits require a grouping change:

- Merge `0x1F70E0` and `0x1FC980` into the 30-function front seed → 32 functions (at the per-export limit).
- Merge `0x1FCB70` into an existing one-function seed (for example the recovered `0x002022E0` seed) → 2 functions.
- The export count stays 32. `0x1FCB70` must use the **table-aware** layout (both pointer tables plus the 28 defined bytes), not the body-only seed.

## Next and limits

The Path A batch was subsequently admitted in translation 55/module 76 using the frozen 32-export grouping (the provisional acceptance's 35 offered seeds were corrected to 32; 33 are rejected by the tooling). See [the Path A admission checkpoint](FRONTIER_PATH_A_ADMISSION_CHECKPOINT.md). The observed next stop is `0x1F6910`; no further batch this cycle. These are synthetic/static observations, not original service semantics, fidelity, renderer or playability.
