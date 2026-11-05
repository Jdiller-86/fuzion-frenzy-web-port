# Service ordinal 159 characterization (read-only)

November 5, 2026. Read-only characterization of the new frontier service. **No implementation, hook change, admission, export, seed or translation change.** The consumer path is unchanged and default-off. This halts for the service decision; the ordinal-2 discipline applies — no modeled return values without documented semantics.

The branch was first fast-forwarded to the published architect integration head `1e35f8a`, restoring `origin/main` ancestry; the next free decision number is used.

## 1. Service identity

| Property | Value |
| --- | --- |
| Ordinal | **159** (`0x9F`) |
| Name | **`KeWaitForSingleObject`** |
| Arguments | **5** (20 bytes): `Object`, `WaitReason`, `WaitMode`, `Alertable`, `Timeout` |
| Return | **NTSTATUS** (`0` = success) |
| Pinned mapping | `src/kernel/kernel_bridge.c:9827` (`case 159: return 20`), `:10283` (`bridge_KeWaitForSingleObject`), `src/kernel/kernel_thunks.c:198` |

## 2. Observed call and caller

- Call site: `D3DDevice_BlockUntilVerticalBlank` (`0x001F5520..0x001F5543`). It loads `ESI = [device + 0x2500]` (the wait object), pushes `0, 0, 1, 6, ESI`, calls through `[0x00253448] = 0x8000009F` (ordinal 159), and loops while `EAX != 0`.
- Stop: status 1 at `0xFE000094`, ordinal 159, return `0x1F5543`, `modeled: false`.
- Observed stack arguments: **Object `0x00206080`** (device+0x2500), **WaitReason 6**, **WaitMode 1**, **Alertable 0**, **Timeout NULL** (infinite).
- Expected result: `STATUS_SUCCESS` (0) once the vblank event is signaled; the caller retries on any nonzero status.

## 3. Pinned references on the wait and vblank delivery

- `kernel_bridge.c:2037-2072` — the bridge resolves a guest event through `ke_guest_event_prepare`; the event's SignalState in guest memory is the truth, and a synchronization-type event consumes the signal by writing `0` at `object+4`. A signaled wait returns 0.
- `kernel_bridge.c:2780-2825` — the vblank is **delivered, not merely enabled**: the D3D8 library installs an ISR and waits on it; the registers must say vblank before the routine runs — **PCRTC_INTR_0 bit 0** for the vblank and **PMC_INTR_0 bit 24** for the PCRTC source. The pinned schedule is a fixed 60 Hz off the host performance counter, with no field/interlace handling.
- The current synthetic profile carries no `pcrtcVblanks` input, so no vblank event is delivered and the wait object never signals — hence the unsupported-service stop.

## 4. Service decision (for the architect)

The documented path is: deliver a vblank interrupt (PCRTC_INTR_0 bit 0 + PMC_INTR_0 bit 24), let the title's ISR signal the wait object, and return `STATUS_SUCCESS` only when the event is actually signaled. A modeled success without that delivery would be exactly the ordinal-2 failure mode the project forbids.

Options for the next bounded decision:
1. Add a synthetic vblank delivery plus a bounded wait-object model, returning success only after delivery, with native tests and a pair run to the next stop.
2. Hold the service and choose another unowned task.

## 5. Limits

The evidence is static plus the retained synthetic stop. It does not establish real display timing, interrupt delivery on original hardware, or playability.
