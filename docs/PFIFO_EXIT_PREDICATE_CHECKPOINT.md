> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

**Later evidence (November 4):** [the reset-state checkpoint](PFIFO_RESET_STATE_CHECKPOINT.md) shows the pinned `nv2a_reset` supplies `CACHE1_STATUS.LOW_MARK` for an empty cache, so the wait can pass with a reference reset input and no consumer. The measured predicate order below still holds for a profile that does not seed the reset state.

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

## Measured predicate order (engine-wait diagnostic)

A new literal profile flag, `device.engineWait`, declares those three registers as status inputs through the existing `ff_device_registers_add_status_wait`, so a run names the blocking predicate instead of ending at the bounded log. With `pcrtcVblanks: 1` and `engineWait: true` the run stops at **status 12 on `0xFD003214`** (read) and the report shows:

| declared input | mask | last value | reads | outcome |
| --- | --- | --- | --- | --- |
| `0xFD003214` `NV_PFIFO_CACHE1_STATUS` | `0x10` | `0` | 65 | **tripped first** |
| `0xFD002400` `NV_PFIFO_RUNOUT_STATUS` | `0x10` | `0` | 0 | never reached |
| `0xFD003220` `NV_PFIFO_CACHE1_DMA_PUSH` | `0x10` | `0` | 64 | polled, unchanged |

`pcrtc` reports `{ pending: 0, enabled: 1, events: 1 }`, i.e. the display interrupt was acknowledged and the loop moved on to the engine predicate. So `CACHE1_STATUS` bit `0x10` is the first unmet condition, `DMA_PUSH` is polled too, and `RUNOUT_STATUS` is not even examined before the first predicate fails.

## Module and runs

Module `recomp-post-memory-build-70`: SHA-256 `f3ef45b3ae556d78338ec44ea1dbf2c77e8daaf2bbcb3fc4d41a9ba5e137e164`.

| profile | stop | routines | trace | device events | `pcrtc` |
| --- | --- | --- | --- | --- | --- |
| canonical | status 10 on the unmodeled `0xFD000100` read | 276 | 3,839 | 12,535 | absent |
| `pcrtcVblanks: 1`, `engineWait` | **status 12 at `0xFD003214`** | 276 | 3,965 | 13,163 | `{ pending: 0, enabled: 1, events: 1 }` |

Both report 83/83 initializers, 403 kernel operations and `timerCheck = 0`. The engine-wait run stops on the predicate itself, well below the event-log bound, so the boundary no longer has to be inferred from capacity.

## Limits

No vblank timing, no PFIFO consumer, no engine, no pushbuffer consumption, no interrupt delivery beyond the declared profile input. The acknowledge rule for `NV_PGRAPH_INTR`/`NV_PFIFO_INTR_0` remains guest-implied and unverified against hardware. No rendering, audio, fidelity, compatibility or playability; all private evidence stays local.

## Validation

PCRTC component test optimized and ASan/UBSan; device-register, compiled real-VM vector, 16-byte checked-pointer, kernel-timer and port tests optimized; generator-to-compiled regression 2/2; Python memory-access 12/12; runtime suite 112/112; title-seed 25/25; `npm run check` and whitespace checks pass.

## Access-level evidence for the FIFO pump

The same experiment records every PFIFO access, which shows what the guest does while the exit still fails:

| address | reads | writes | dominant write value | sites |
| --- | --- | --- | --- | --- |
| `0xFD002500` (`PFIFO + 0x500`, undefined in the retained header) | 2 | 71 | `0x1`, 66 times | `0x1FDD92` x64 |
| `0xFD003250` (`NV_PFIFO_CACHE1_PULL0`) | 1 | 70 | `0x1`, 66 times | `0x1FDD92` x64 |
| `0xFD002080` (`PFIFO + 0x80`, undefined in the retained header) | 64 | 0 | none | `0x1FDD9E` |
| `0xFD003214` (`NV_PFIFO_CACHE1_STATUS`) | 65 | 0 | none | `0x1FE4DB` |
| `0xFD003220` (`NV_PFIFO_CACHE1_DMA_PUSH`) | 64 | 2 | `0x0`/`0x1` once each | `0x1FDD1A` |
| `0xFD002400` (`NV_PFIFO_RUNOUT_STATUS`) | 0 | 0 | none | never accessed |

So the guest pumps `PFIFO + 0x500` and `CACHE1_PULL0` with bit 0, reads `PFIFO + 0x80` and `DMA_PUSH`, then tests `CACHE1_STATUS` bit `0x10` — 64 iterations of one loop body at `0x1FDD92`/`0x1FDD9E`/`0x1FDD1A`. `RUNOUT_STATUS` is never touched, so its predicate is unreachable while cache status fails. The two pumped offsets without a header definition are unevidenced in meaning; the aperture preserves the writes verbatim and derives no engine state.

## Measured predicate order

A new literal profile flag, `device.engineWait`, declares those three registers as status inputs through the existing `ff_device_registers_add_status_wait`, so a run names the predicate that blocks the exit instead of ending at the bounded log. With `pcrtcVblanks: 1` and `engineWait: true` the run stops at **status 12 on `0xFD003214`** (read) and the report shows:

| declared input | mask | last value | reads | outcome |
| --- | --- | --- | --- | --- |
| `0xFD003214` `NV_PFIFO_CACHE1_STATUS` | `0x10` | `0` | 65 | **tripped first** |
| `0xFD002400` `NV_PFIFO_RUNOUT_STATUS` | `0x10` | `0` | 0 | never reached |
| `0xFD003220` `NV_PFIFO_CACHE1_DMA_PUSH` | `0x10` | `0` | 64 | polled, unchanged |

`pcrtc` reports `{ pending: 0, enabled: 1, events: 1 }`: the display interrupt was enabled, delivered and acknowledged, and the loop moved on to the engine predicate. The order is therefore measured, not inferred from the disassembly, and the boundary is reached below the event-log bound.

## Next task

Hand the PFIFO engine/consumer contract to the lane that owns it: define which observable FIFO state may set `NV_PFIFO_CACHE1_STATUS`/`NV_PFIFO_RUNOUT_STATUS` and clear `NV_PFIFO_CACHE1_DMA_PUSH` bit `0x10`, with evidence. Until such a contract exists, keep this lane's profiles fail-closed and do not synthesise those bits.
