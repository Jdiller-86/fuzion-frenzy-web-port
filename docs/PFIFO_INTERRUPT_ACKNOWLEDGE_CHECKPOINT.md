> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# PFIFO interrupt acknowledge joins PGRAPH; the boundary moves to a PFIFO engine-state wait

November 4, 2026. `NV_PFIFO_INTR_0` (`0xFD002100`) now acknowledges the bits a store carries, exactly like `NV_PGRAPH_INTR`. The phantom PFIFO interrupt is gone and the run advances further: the reached original code now drives the PFIFO channel, writes `NV_PFIFO_CACHE1_PULL0` (`0xFD003250`) 1,846 times, polls PFIFO status 1,840 times and only stops when the bounded device-event log fills. **There is no playable screen, engine model, renderer or fidelity result.**

## Observed guest sequence for the second register

The reached routine `0x1FDC30` reads `[device + 0x2100]` once and then:

- if bit `0x100000` is set, stores `0x100000` back to `[device + 0x2100]`;
- if bit `0x1000000` is set, stores `0x1000000` back to the same register;
- re-reads `[device + 0x2100]` and acts on the low byte of the refreshed value;
- its caller `0x1FE4D5` repeats the whole block while the watched state is unchanged.

That is the same read / store-exactly-the-observed-bit / re-read shape already modelled for `NV_PGRAPH_INTR`, and under plain read-back the stored bits stay set, so the caller repeats forever. Both registers are declared write-1-to-clear, with the same limits: only the declared register, only 4-byte stores, sub-dword stores rejected. First traffic on `0xFD002100` is a `0xFFFFFFFF` store, which under the acknowledge rule clears everything — consistent with an interrupt clear at start-up.

## Source-owned changes

- `tools/fixtures/thread_startup_probe.c` declares `0xFD002100` (`NV_PFIFO_INTR_0`) as write-clearing next to `0xFD400100`, with the observed sequence recorded.
- `src/runtime/device_registers.h/.c` adds bounded per-register wait detection (`FF_DEVICE_WAITS` 8, `FF_DEVICE_WAIT_LIMIT` 64): a register re-read with the same value that many times without an intervening store is a hardware transition the aperture cannot provide, reported through `ff_device_registers_poll_limited` and named by `ff_device_registers_wait_address`. A store to that register restarts its count, so state-changing loops keep running.
- `src/runtime/thread-proof-worker.mjs` / `tools/fixtures/thread_startup_probe.c` distinguish storage exhaustion from a denied access: `ff_device_registers_capacity_reached` feeds probe status 15 (`device register capacity reached`) instead of reporting a full log as an unmodeled access.
- `tests/device_registers_test.c` covers interleaved reads of one register at different sites reaching the wait limit and being reset by a store, and capacity exhaustion being reported separately with the next access failing.

## Run

- Private module `recomp-post-memory-build-68` rebuilt from this source: 640,409 bytes, SHA-256 `a385b2d381e3d25a2f918fb8d47bc6c47ade5e2736bac600e3ab046947320007`.
- Canonical profile run: **276** distinct routines, 83/83 initializers, 403 kernel operations, `timerCheck = 0`, trace 7,518 events.
- The guest reaches `0x1FDC30` 1,841 times and drives the channel: 7,362 reads of `0xFD002100`, 1,847 stores to `0xFD002500`, 1,846 stores to `0xFD003250` (`NV_PFIFO_CACHE1_PULL0`), 1,841 reads of `0xFD400100`, 1,840 reads each of `0xFD003214`, `0xFD003220`, `0xFD002080` and `0xFD000100`.
- Stop: status 15 (`device register capacity reached`) on a read of `0xFD003220` after the ordered device log reached its 32,768-event bound (5,301 live register entries).
- The `NV_PMC_INTR_0` read that stopped the previous run is served by an explicit 4-byte zero range in the selected profile: no device event is delivered, so the status register reads zero; a store by the guest creates the entry.

## What this does and does not establish

- Established: two independent reached code paths acknowledge interrupt status by storing exactly the bits they observed, and the synthetic echo made both loops unsatisfiable. Acknowledging the stored bits removes the phantom interrupts.
- Not established: that the real registers clear on store. The rule is inferred from the guest's structure and the impossibility of the loop under echo; the local xemu-derived PGRAPH reference stores writes as a stub and cannot confirm it.
- Not established: any PFIFO/PGRAPH engine progress. The guest's remaining loop writes `NV_PFIFO_CACHE1_PULL0` and polls status ~1,840 times; nothing in the model consumes a command stream, so the loop cannot terminate. That is the same contract question as the earlier empty-ring/PUT-GET boundary, now on the reached PULL0 path.

## Limits

No rendering, audio, fidelity, original-game compatibility or playability. No engine state machine, no interrupt delivery, no pushbuffer consumption. All private profile/report evidence stays local.

## Validation

`tests/device_registers_test.c` optimized and ASan/UBSan; kernel-timer, port and 16-byte checked-pointer tests optimized; Python memory-access 11/11; title-seed 25/25; runtime suite 110/110; `npm run check` and whitespace checks pass.

## Next task

Decide the PFIFO consumption/engine-progress contract with evidence: either bound what the model does when the guest writes `NV_PFIFO_CACHE1_PULL0` and polls status, or stop explicitly at that wait with the observed sequence. Do not grow the event log to delay the boundary, invent engine completion, seed a status bit or claim rendering/fidelity without evidence.
