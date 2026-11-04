# The remaining exit predicates are PFIFO engine status, not display state

November 4, 2026. With the display interrupt modelled and a profile able to declare vertical-blank events, the reached wait loop still cannot complete. A read-only audit of its exit condition shows why: the three remaining predicates are PFIFO engine status bits, which require a FIFO consumer this lane does not model. **There is no timing source, engine, renderer or playable screen.**

## The exit condition

`0x1FE4D5` loops while the PMC bit-24 poll is clear, and leaves through `0x1FE522` only when all three predicates hold (`esi` is the device base, `0xFD000000`):

| access site | offset | reference register | test |
| --- | --- | --- | --- |
| `0x1FE4DB` | `0x3214` (U8) | `NV_PFIFO_CACHE1_STATUS` (`nv2a_regs.h:136`) | bit `0x10` must be **set** |
| `0x1FE4E4` | `0x2400` (U8) | `NV_PFIFO_RUNOUT_STATUS` (`nv2a_regs.h:122`) | bit `0x10` must be **set** |
| `0x1FE4ED` | `0x3220` (U8) | `NV_PFIFO_CACHE1_DMA_PUSH` (`nv2a_regs.h:139`) | bit `0x10` must be **clear** |

None of these is display state. They are the channel's cache/runout status and its DMA push control, i.e. the FIFO consumer side of the graphics engine.

## What the display component changed

`src/runtime/pcrtc_state.h/.c` (previous milestone) supplies the interrupt contract, and the profile can now declare vertical-blank events. In the experiment run the report records the component state directly:

```
pcrtc: { pending: 0, enabled: 1, events: 8 }
```

The guest enabled the PCRTC interrupt, the aggregate bit appeared, the guest acknowledged it (`pending` back to `0`), and the poll released. The canonical profile still reports no `pcrtc` state at all and stops at the unmodeled PMC read.

So the interrupt half of the wait is now modelled and observed; the loop's exit is blocked on the PFIFO engine half.

## Consequence for the lane

- Declaring more display events cannot complete the loop: after the first acknowledgement there is nothing left for the display path to do, and the remaining predicates are engine state.
- Raising PFIFO status bits to satisfy those predicates would fabricate engine progress. The project's empty-ring profile deliberately declares no FIFO consumer (`GET == PUT`, pushbuffer consumption refused), so this lane must not produce those bits.
- Therefore the canonical profile stays fail-closed, no event is delivered by default, and the boundary is recorded as an engine dependency rather than a display one.

## Report instrumentation

`ff_probe_pcrtc_word` exposes the component state (ready, pending, enabled, supplied events) and the worker reports it as `pcrtc` when present, so every run states whether the display path was exercised and what it left pending. The canonical run reports nothing, which is the fail-closed default.

## Module and runs

Module `recomp-post-memory-build-70`: SHA-256 `1e56dbbd74350e6b85a0c605c4fa8e3324bac33764bbe5353d07398a904f72c6`.

| profile | stop | routines | trace | device events | `pcrtc` |
| --- | --- | --- | --- | --- | --- |
| canonical | status 10 on the unmodeled `0xFD000100` read | 276 | 3,839 | 12,535 | absent |
| `pcrtcVblanks: 8` | status 15 on a write to `0xFD002500` | 276 | 7,886 | 32,768 | `{ pending: 0, enabled: 1, events: 8 }` |

Both report 83/83 initializers, 403 kernel operations and `timerCheck = 0`. The experiment ends at the bounded device-event log while the guest services PFIFO channel work; that is capacity, not progress.

## Limits

No vblank timing, no PFIFO consumer, no engine, no pushbuffer consumption, no interrupt delivery beyond the declared profile input. The acknowledge rule for `NV_PGRAPH_INTR`/`NV_PFIFO_INTR_0` remains guest-implied and unverified against hardware. No rendering, audio, fidelity, compatibility or playability; all private evidence stays local.

## Validation

PCRTC component test optimized and ASan/UBSan; device-register, compiled real-VM vector, 16-byte checked-pointer, kernel-timer and port tests optimized; generator-to-compiled regression 2/2; Python memory-access 12/12; runtime suite 112/112; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Hand the PFIFO engine/consumer contract to the lane that owns it: define which observable FIFO state may set `NV_PFIFO_CACHE1_STATUS`/`NV_PFIFO_RUNOUT_STATUS` and clear `NV_PFIFO_CACHE1_DMA_PUSH` bit `0x10`, with evidence. Until such a contract exists, keep this lane's profiles fail-closed and do not synthesise those bits.
