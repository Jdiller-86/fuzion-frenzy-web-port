# Service ordinal 3 is AvSetDisplayMode (read-only)

November 5, 2026. Read-only characterization of the new frontier. **No implementation, hook change, admission, export, seed or translation change.** The consumer and vblank-wait paths remain default-off.

## 1. Service identity and semantics

| Property | Value |
| --- | --- |
| Ordinal | **3** |
| Name | **`AvSetDisplayMode`** |
| Arguments | **6**: `addr`, `step`, `mode`, `format`, `pitch`, `fb` |
| Return | `0` on success (`g_eax = 0` in the pinned bridge) |
| Pinned implementation | `src/kernel/kernel_bridge.c:2394` (`bridge_AvSetDisplayMode`) |

The bridge records the scanout address and format, resolves the framebuffer VA (contiguous allocations live at `XBOX_CONTIG_BASE + P`), points the framebuffer window at it, calls `xbox_AvSetDisplayMode(...)`, and returns success. It is the only place the title states where it believes the picture is (`PCRTC_START` is never written directly).

## 2. Observed call

- Call site: `D3DDevice_Present` (`0x00202150..0x002022D0`), after `D3DDevice_BlockUntilVerticalBlank` returns: the six pushed arguments, `CALL [0x00253458]` (ordinal 3), return `0x00202231`; the result in `EDI` drives the submit loop (`TEST EDI,EDI; JNZ`).
- Stop: status 1 at `0xFE0000A4`, ordinal 3, `modeled: false`.
- Observed arguments: `addr 0x3C10CFC` (a stack-resident display-mode descriptor), `step 0`, `mode 0x600302C7`, `format 0`, `pitch 0xA00` (2560), `fb 0x03920000`.
- Kernel-operation count: 406 (the accepted vblank wait is the 406th completed operation); routines 340.

## 3. Pinned references on the display path

- `bridge_AvSetDisplayMode` (`kernel_bridge.c:2394-2445`) documents the framebuffer resolution, window start, and success return; the display-mode descriptor address is passed through to `xbox_AvSetDisplayMode`.
- The same file's earlier notes explain that the title never writes `PCRTC_START`; `AvSetDisplayMode` is the authoritative scanout statement.
- The resolved framebuffer is physical and must be mapped into the contiguous window; reading it directly lands in the loaded image (the bridge's own recorded failure mode).

## 4. Service decision (for the architect)

The documented path is to record `mode/format/pitch/fb`, resolve the framebuffer, start the window, and return success. A bounded model would need:

1. the display-mode descriptor layout at `addr` (or an explicit refusal to interpret it),
2. the framebuffer resolution rule and pitch/format bounds (the observed `fb 0x03920000`, pitch 2560),
3. what, if anything, the run observes as the next stop after success.

Options: model the documented `AvSetDisplayMode` state and return 0 with native tests and a pair run; or hold the display path. No modeled success without the documented fb/pitch/format state.

## 5. Limits

Static references plus the retained synthetic stop; no display timing, scanout, rendering or playability is established.
