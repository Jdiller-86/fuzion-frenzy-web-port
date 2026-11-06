> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# A bounded PCRTC interrupt component releases the PMC wait only when a profile supplies display events

November 4, 2026. The display interrupt the reached code waits on now has a modelled state component with the register contract taken from the retained reference, and a bounded, explicitly declared display-event input. With eight declared vertical-blank events the guest leaves the interrupt wait and returns to PFIFO channel work; without any declared event the run is unchanged and fail-closed. **There is no timing source, renderer, engine progress or playable screen.**

## Component

`src/runtime/pcrtc_state.h/.c` implements exactly the contract established by the provenance audit:

- `NV_PCRTC_INTR_0` (`0xFD600100`) reads the pending mask and clears the bits written to it;
- `NV_PCRTC_INTR_EN_0` (`0xFD600140`) stores the enable mask;
- pending bit 0 is the vertical-blank event (`FF_PCRTC_INTR_VBLANK`);
- `ff_pcrtc_pmc_irq` returns `FF_PCRTC_PMC_IRQ` (`0x01000000`, PMC bit 24) exactly while `pending & enabled`;
- `ff_pcrtc_vblank` is the only way a bit is raised, and nothing in the runtime timer or kernel path calls it.

Unknown offsets are rejected rather than invented, and a null state is tolerated. `tests/pcrtc_state_test.c` covers pending-without-enable, enable gating, aggregation, acknowledge, an unrelated write clearing nothing, disable-while-pending, rejection of unknown registers and re-initialisation; it passes optimized and under ASan/UBSan.

## Wiring

- `tools/fixtures/thread_startup_probe.c` serves the three reached addresses (`0xFD600100`, `0xFD600140`, `0xFD000100`) from the component **only** when the profile supplies events; otherwise the previous fail-closed path is untouched. Writes are staged through a shadow word and applied on the next access, so the guest's store value is what the contract receives. `ff_probe_pcrtc(vblanks)` admits at most 64 declared events.
- `src/runtime/local-title-proof.mjs` reports `device.pcrtcVblanks` (integer 0-64, anything else becomes 0) and `src/runtime/thread-proof-worker.mjs` calls `ff_probe_pcrtc` before admitting the window, failing closed if the module cannot honour it. `tests/mmio-profile.test.mjs` pins the bounds and the non-integer rejection.

## Runs over one module

Module `recomp-post-memory-build-70`: **642,845 bytes**, SHA-256 `bc285ea9033b5c6f564988baeda331005be7bda96d86afbf222dc04e0bd14731`.

| profile | declared input | stop | routines | trace | device events |
| --- | --- | --- | --- | --- | --- |
| canonical | none | status 10 on the unmodeled `0xFD000100` read | 276 | 3,839 | 12,535 |
| control | no flags | status 5 at `0x200F59` | 277 | 8,192 | 27,773 |
| `pmcZero` | PMC read served as zero | status 12 at `0xFD000100` | 276 | 3,967 | 13,240 |
| `pcrtcVblanks: 8` | eight declared vblank events | **status 15 at `0xFD002500` (write)** | 276 | 7,886 | 32,768 |

The fourth run is the evidence that PMC bit 24 is what releases the wait: the guest acknowledges the PCRTC bit, the aggregate reads set, and execution returns to PFIFO channel work, where it ends at the bounded device-event log — a capacity stop, not engine progress. All runs report 83/83 initializers, 403 kernel operations and `timerCheck = 0`.

## What this does not establish

- There is **no display timing source**. The events are supplied up front by the profile, not generated from a scanout model, so this validates the interrupt contract, not the timing of a real display.
- The guest's exit at `0x1FE522` additionally depends on other device state, and the loop still ends at the bounded event log rather than a modelled engine.
- The acknowledge rule for `NV_PGRAPH_INTR`/`NV_PFIFO_INTR_0` remains guest-implied and unverified against hardware.
- No rendering, audio, fidelity, compatibility or playability; original content stays local.

## Decision

The fail-closed default is preserved: no profile, no event, no delivery. A declared, bounded event count is the only way to exercise the path, which keeps the experiment separable from any fidelity claim. No capacity was raised and no completion bit is seeded.

## Validation

PCRTC component test optimized and ASan/UBSan; device-register, compiled real-VM vector, 16-byte checked-pointer, kernel-timer and port tests optimized; generator-to-compiled regression 2/2; Python memory-access 12/12; runtime suite **112/112**; title-seed 25/25; `npm run check` 307 source files; whitespace checks pass.

## Next task

Replace the declared event count with a real, bounded scanout/vblank timing source whose period and phase are evidenced, then re-verify that the wait releases on schedule rather than immediately; until then keep the canonical profile fail-closed.
