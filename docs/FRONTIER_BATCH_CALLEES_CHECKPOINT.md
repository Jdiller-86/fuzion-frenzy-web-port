# Frontier: callees of the post-`0x1F7080` candidates

November 4, 2026. Bounded read-only scoping of the callees named by the two static continuation candidates. This is not an export, admission, translation or execution. Raw disassembly and analysis scripts remain under ignored `research/verification/frontier-1fc980/`, `frontier-1fcb70/` and `frontier-1f6300/`.

## Result

| Callee | Shape | Return(s) | Calls / tables |
| --- | --- | --- | --- |
| `0x1FC980` | 2 instructions / 9 bytes | `0x1FC986` | no calls; single `cvttss2si` leaf |
| `0x1FCB70` | 40 instructions / 156 bytes | `0x1FCC39`, `0x1FCC4D` | no calls; two indirect jump tables at `0x1FCC50` and `0x1FCC98` |
| `0x1F6300` | 113 instructions / 392 bytes | `0x1F6485` | calls `0x1FB3B0` (admitted) and `0x1FC980`; uses `rep movsb`/`rep movsd`; no indirect edge |

- `0x1FC980` is shared by both candidate paths.
- The `0x1F70E0` path needs `0x1FC980` plus the table-aware `0x1FCB70`.
- The `0x1F5B70` path needs `0x1F6300` plus `0x1FC980`.
- The taken branch after `0x1F7080` is still unproven; this is static scoping, not an observed stop.

## Exported shared seed (read-only, not admitted)

`0x1FC980` is shared by both candidate paths. The retained exporter reads it as **2 instructions / a 9-byte instruction-only envelope**, no calls; SHA-256 `18744bb385b803cd8d67c0d4c2e26d02c30e005e6b0a8f8c11e7d31972e23d4b`. Byte identity, independent decoder agreement (2/2, no normalizations) and `checked_function_layout` pass. Re-preflighted at `2026-11-04T20:50-05:00` with the same result; with the `0x1FCB70` table-aware validation, both Path A dependencies are now preflighted. The seed was not merged into the selection, and `0x1FC980` remains unadmitted.

## 0x1FCB70 two-table shape (read-only)

The retained export is a **224-byte / 59-instruction body** ending at `0x1FCC4F`, with two computed jumps: `0x1FCB8B` through the **11-target** table at `0x1FCC50`, and `0x1FCBC6` through the **2-target** table at `0x1FCC98`. Both tables are outside the body, with a gap at `0x1FCC7C..0x1FCC97` between them. A table-aware envelope needs both tables and the gap; its read-only layout and dispatch-census validation now passes, but no seed merge or export was performed and the post-`0x1F7080` path is unproven. `0x1FCB70` remains unadmitted.

## Path B seed preflight (read-only)

- `0x1F5B70`: **347-byte / 118-instruction** envelope, 4 calls, no computed jumps; SHA-256 `aa8ee11b8e9571a62630033f6f0b03fe95be5f3fa93f67ab7ffdb54b07bd071b`.
- `0x1F6300`: **392-byte / 113-instruction** envelope, 5 calls, no computed jumps; SHA-256 `f4c8d089852a19bb7c1a101ca1754e8d734e659f5bef88cfe5ab464e6438e1af`.
- Both pass byte identity, independent decoder agreement (118/118 and 113/113, no normalizations) and `checked_function_layout` (`mixed=False`, 0 tables).
- Path B is the three-function batch `0x1F5B70` + `0x1F6300` + shared `0x1FC980`; all three seeds are preflighted and none is in the selection.

### Path A blocker

The gap `0x1FCC7C..0x1FCC97` is **28 bytes of defined 1-byte data** between the 11-entry pointer table and the 2-entry pointer table. The guard now accepts bounded defined 1-byte data: the exact byte is checked, it never becomes a table, and control flow into it is rejected. A read-only table-aware envelope of the real `0x1FCB70` now passes: mixed layout, 80 data bytes (44 pointer + 28 byte + 8 pointer), 0 padding, tables 11+2, and dispatch census `{0x1FCC50: 11, 0x1FCC98: 2}`. The byte data is not cleared or redefined. Path A was admitted in translation 55/module 76 under the frozen 32-export grouping. The observed `0x1F70E0` seed is 98 instructions / 329 bytes with four calls; only `0x1FC980` and `0x1FCB70` were untranslated (`0x1F66A0`/`0x1FB3B0` were already translated), and its preflight is 98/98. The grouping merged `0x1F70E0`+`0x1FC980` into the front seed (32 functions) and `0x1FCB70` with the recovered seed, keeping 32 exports; translation 55 has 426 functions / 17,779 instructions. `0x1F70E0`/`0x1FC980` executed; `0x1FCB70` was translated but not reached. The next stop is ordinary `0x1F6910`; see [the Path A admission](FRONTIER_PATH_A_ADMISSION_CHECKPOINT.md). The guard contracts are pinned by `tests/bounded_layout_test.py` (23 cases: instruction-only, pointer/table/padding, bounded defined bytes with a 256-byte budget, the 28-byte Path A gap, flow/envelope, two tables separated by defined bytes, and dispatch-table). A byte-data extension is a guard capability, not a seed workaround.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- Call classification reflects translation 53; a different selection changes it.
- All three callees remain **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- `0x1FCB70` would require the same mixed code/table/padding treatment as `0x1FCCB0`; `0x1F6300` would require bulk-copy coverage.

## Next

`0x1F7080` remains the immediate preflighted leaf. A future post-`0x1F7080` batch should be scoped as one of the two paths above, with the table-aware and bulk-copy needs explicit, rather than as another single-leaf export.
