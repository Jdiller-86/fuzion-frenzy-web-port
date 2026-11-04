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

`0x1FC980` is shared by both candidate paths. The retained exporter reads it as **2 instructions / a 9-byte instruction-only envelope**, no calls; SHA-256 `18744bb385b803cd8d67c0d4c2e26d02c30e005e6b0a8f8c11e7d31972e23d4b`. Byte identity, independent decoder agreement (2/2, no normalizations) and `checked_function_layout` pass. The seed was not merged into the selection, and `0x1FC980` remains unadmitted.

## Boundaries

- Static only: no original execution, browser run or runtime observation.
- Call classification reflects translation 53; a different selection changes it.
- All three callees remain **unadmitted and untranslated**. No seed, export, module, report or runtime path changed.
- `0x1FCB70` would require the same mixed code/table/padding treatment as `0x1FCCB0`; `0x1F6300` would require bulk-copy coverage.

## Next

`0x1F7080` remains the immediate preflighted leaf. A future post-`0x1F7080` batch should be scoped as one of the two paths above, with the table-aware and bulk-copy needs explicit, rather than as another single-leaf export.
