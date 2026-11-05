# The later ordinal-2 call is an unknown AV option 0x0F (read-only)

November 5, 2026. Read-only characterization of the new frontier after the display-mode declaration. **No implementation, hook change, admission, export, seed or translation change.** The display, vblank and consumer paths remain default-off.

## 1. Observed call

- Call site: `D3DDevice_Present` (`0x00202150..0x002022D0`), the second submit block at `0x00202268`; return `0x00202268` follows `CALL [0x00253450]` (ordinal 2).
- Stop: status 1 at `0xFE000094`, ordinal 2, `modeled: false`, `AvSendTVEncoderOption` (4 args, stdcall, 16 bytes, returns void).
- Observed arguments: **`0xFD000000`, `15` (option `0x0F`), `0`, `62983692` (a Result pointer on the stack)**.
- The earlier startup query `(0, 6, 0, Result)` completed through the synthetic adapter; this later call has a different shape and was denied.

## 2. Pinned semantics

- `bridge_AvSendTVEncoderOption` (`kernel_bridge.c:5429`) calls `xbox_AvSendTVEncoderOption(handle, option, param, Result)` and returns success; the arity table is 16 bytes (`:9749`).
- The pinned implementation (`kernel_hal.c:1455-1520`) handles documented options `0x01`-`0x0E` and has a **default branch for unknown options**: it logs `unknown option`, writes **0** through `Result`, and does not fail.
- `kernel.h:1148-1157` lists the documented options; **`0x0F` is not among them**, so this call is the documented unknown-option path (write 0, succeed).
- The synthetic `av_query_adapter` only accepts the observed `(0, 6, 0, Result)` startup query; every other shape is denied, which is why the run stops here.

## 3. Service decision (for the architect)

The pinned evidence supports a bounded extension of the ordinal-2 adapter: for a call whose shape is the observed late request (`handle 0xFD000000`, option `0x0F`, param 0, Result pointer) — or, more generally, the documented unknown-option default — write 0 through the fourth argument and return success, default-off, with the exact-shape guard and the ordinal-2 discipline preserved. Options:

1. Model the unknown-option default with the exact observed shape (write 0, success), native tests, a pair run to the next stop.
2. Hold the call and choose another unowned task.

No modeled success without the documented unknown-option behavior; the Result write and the handling of any other option remain refusals.

## 4. Limits

Static references plus the retained synthetic stop; no display, encoder or playability claim.
