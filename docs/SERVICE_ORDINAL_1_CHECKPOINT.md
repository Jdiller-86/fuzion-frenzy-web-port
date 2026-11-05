# Service ordinal 1 is AvGetSavedDataAddress (read-only)

November 5, 2026. Read-only characterization of the new frontier after the unknown-AV-option model. **No implementation, hook change, admission, export, seed or translation change.** The consumer, vblank, display and unknown-option paths remain default-off.

## 1. Service identity

| Property | Value |
| --- | --- |
| Ordinal | **1** |
| Name | **`AvGetSavedDataAddress`** |
| Arguments | none |
| Return | ULONG in EAX (the saved-data address) |
| Pinned implementation | `kernel_hal.c:1445` — returns the static `g_av_saved_data_address` |
| Setter | `AvSetSavedDataAddress` (ordinal 4, 1 arg) — not called on this path |

The pinned HAL initializes `g_av_saved_data_address` to **0** and only `AvSetSavedDataAddress` changes it; no such call appears in the retained run, so the documented return is 0.

## 2. Observed call and caller

- Call site: `D3DDevice_Present` (`0x00202150..0x002022D0`) at `0x00202284`, immediately after the second `AvSendTVEncoderOption` call; return `0x0020228A`.
- The result is moved to `EDI` and tested. If nonzero, Present enters the saved-data path (calls through `0x00253454`, `0x00253360`, `0x0025336C`, `0x0025345C`); if zero, it skips to `0x002022B0`.
- Stop: status 1 at `0xFE000098`, ordinal 1, `modeled: false`, one recorded argument (`0x20228A`-adjacent stack value), 340 routines / 408 kernel ops.

## 3. Service decision (for the architect)

The pinned return is a static 0 with no side effects and no arguments. A bounded model would return that documented value in EAX, exact no-argument shape, default-off, with native tests and a pair run; the zero branch skips the saved-data block and reports the next genuine stop. The alternative is to hold.

No modeled success beyond the pinned static return; the saved-data path (nonzero) remains unmodeled and would be a fresh decision.

## 4. Process note

The published integration head `c8c965c` (source `5d7ebf5`) does not contain the unknown-option model `3d4cf7d`, and it is not an ancestor of the lane head; a merge is denied in this session and no fast-forward is possible. The binding "sync before any DECISIONS row" rule therefore blocks a row this cycle; this characterization is recorded in the checkpoint and status only, and the row can be added after the architect integrates `3d4cf7d`.

## 5. Limits

Static references plus the retained synthetic stop; no display, encoder or playability claim.
