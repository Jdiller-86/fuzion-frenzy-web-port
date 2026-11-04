# PMC bit 24 is the PCRTC (display) interrupt; no host producer exists, so delivery stays fail-closed

November 4, 2026. Read-only provenance audit of the interrupt bit the reached code waits on, prompted by the architect review recorded at the end of this note. The bit is identified end to end on the guest side, the retained reference supplies the register contract, and the producing event is **absent** in that reference. Delivery therefore stays fail-closed. No runtime behaviour changed in this step. **There is no controllable original screen, engine progress or fidelity result.**

## The bit is a display-controller interrupt

From the retained local NV2A reference (`.tools/emulation/xboxrecomp-source/src/nv2a/`, xemu-derived):

- `nv2a_regs.h:66` — `NV_PMC_INTR_0_PCRTC (1 << 24)`: bit 24 of `NV_PMC_INTR_0` (`0xFD000100`) is the PCRTC (display/CRTC) aggregate.
- `nv2a_regs.h:652-655` — `NV_PCRTC_INTR_0` (`0xFD600100`) with `NV_PCRTC_INTR_0_VBLANK (1 << 0)`, and `NV_PCRTC_INTR_EN_0` (`0xFD600140`) with `NV_PCRTC_INTR_EN_0_VBLANK (1 << 0)`.
- `nv2a_core.c:101-121` — reading `NV_PMC_INTR_0` returns `d->pmc.pending_interrupts`; `nv2a_core.c:125-140` — writing it clears `pending_interrupts &= ~val` (write-1-to-clear on the aggregate).
- `nv2a_core.c:30-55` — `nv2a_update_irq` sets `PMC_INTR_0_PCRTC` exactly when `d->pcrtc.pending_interrupts & d->pcrtc.enabled_interrupts`, and clears it otherwise.
- `nv2a_core.c:312-315` / `:339-344` — `NV_PCRTC_INTR_0` reads the pending mask and clears written bits; `NV_PCRTC_INTR_EN_0` stores the enable mask.

So the causal chain is: **PCRTC pending (bit 0 = VBLANK) ∧ PCRTC enabled ⇒ PMC bit 24**.

## The guest both enables and acknowledges that path

Reached original code in the regenerated translation (`recomp-title-recomp-title-post-memory-45`):

- `0x200340` loads the device base and writes `1` to `[device + 0x600100]` **and** `[device + 0x600140]` — i.e. it acknowledges the PCRTC pending bit and enables the PCRTC VBLANK interrupt.
- `0x1FD7E0` writes `[device + 0x600100]` again (acknowledge).
- `0x1FE50D` then loops while bit 24 of `NV_PMC_INTR_0` is clear.

The guest-side contract is therefore coherent and fully evidenced: it enables the vblank interrupt, acknowledges it, and waits for the aggregate bit.

## The producer is missing in the retained reference

- Nothing in the extracted reference ever assigns to `d->pcrtc.pending_interrupts` (only reads and clears: `nv2a_core.c:313`, `:340`), so the pending bit is **never raised**.
- `nv2a_state.h:83` declares `QEMUTimer *vblank_timer`, but the extracted source contains no timer callback or scanout timing that would set the pending bit.

That is an explicit gap, not a modelled event. Raising PMC bit 24 from a host timer, from polling, from GET==PUT, or by clearing PFIFO/PGRAPH status would invent causality.

## Decision (architect-reviewed)

The architect review (2026-11-03T19:47:36-05:00, recorded here as the authoritative review for this decision) recommended keeping the read fail-closed and treating a display/timing device as the only credible candidate producer, subject to a verified contract: identified source, enable/pending/reset rules, a defined timing/state transition, and aggregation into PMC. PMC status and CPU interrupt delivery are separate contracts, so satisfying this poll would not by itself justify invoking an interrupt handler. Unknown configuration, unsupported transitions and exhausted budgets must stop explicitly rather than synthesize an event.

Accordingly: `NV_PMC_INTR_0` stays fail-closed by default, no interrupt is delivered, and the existing limits are unchanged. The `pmcZero` experiment remains the only way to observe the wait, and it is labelled as a diagnostic. This supersedes the earlier "Sol unavailable" notes in this session; Sol was reached this cycle.

## Report instrumentation added

The proof report now carries the declared status waits, so the wait is auditable without reading the raw event log: `deviceRegisters.statusWaits` lists `{ address, mask, value, count }` via `ff_probe_device_status_wait_count` / `ff_probe_device_status_wait_word`, with a bounded-capacity check in `src/runtime/device-report.mjs` and a unit test in `tests/device-report.test.mjs`. In the `pmcZero` run it reports `{ address: 0xFD000100, mask: 0x01000000, value: 0, count: 65 }`.

## Runs (unchanged behaviour)

Module `recomp-post-memory-build-70`: **641,451 bytes**, SHA-256 `273f157a47e1ca8c3fc5e21d5333a4fa0f10c6513892f65f8ce504c317b9a552`.

| profile | stop | routines | trace | device events | status waits |
| --- | --- | --- | --- | --- | --- |
| canonical | status 10 on the unmodeled `0xFD000100` read | 276 | 3,839 | 12,535 | none declared |
| control (no flags) | status 5 at `0x200F59` | 277 | 8,192 | 27,773 | none declared |
| `pmcZero` experiment | status 12 at `0xFD000100` | 276 | 3,967 | 13,240 | `0xFD000100` mask `0x01000000`, count 65 |

## Limits

No vblank or display timing exists, so bit 24 never appears; no interrupt is delivered; no command stream is consumed. The acknowledge rule for `NV_PGRAPH_INTR`/`NV_PFIFO_INTR_0` remains guest-implied and unverified against hardware. No rendering, audio, fidelity, compatibility or playability is established; all private evidence stays local.

## Validation

Device-register test optimized and ASan/UBSan; compiled real-VM vector test, 16-byte checked-pointer, kernel-timer and port tests optimized; generator-to-compiled regression 2/2; Python memory-access 12/12; runtime suite 112/112; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Build or adopt a display/vblank timing source whose pending/enable/reset rules match `NV_PCRTC_INTR_0`/`NV_PCRTC_INTR_EN_0` and whose aggregation into `NV_PMC_INTR_0` bit 24 is explicit, then re-evaluate delivery with that contract. Until then keep the read fail-closed and do not raise wait, trace or event limits.
