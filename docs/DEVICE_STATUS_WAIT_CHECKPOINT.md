> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Declared device-status waits are reported instead of silently filling the event log

November 4, 2026. The reached wait on `NV_PMC_INTR_0` is now reported as a device-status wait instead of consuming the bounded device-event log until it fills. `NV_PMC_INTR_0` stays fail-closed by default; only the explicit `pmcZero` experiment declares it, and that declaration now also records which bit the guest polls. **There is no controllable original screen, engine progress or fidelity result.**

## Evidence from the experiment run

With `pmcZero` enabled, the device log for the previous build showed the reached loop behaviour precisely:

- `0xFD000100` was read **1,840 times, every read returning 0**, and was **never written**;
- the reads all come from `0x1FE50D`, which loops back while bit `0x1000000` (bit 24) is clear;
- the exit at `0x1FE522` needs `[0x3214] & 0x10` and `[0x2400] & 0x10` set with `[0x3220] & 0x10` clear, so the loop is not a PFIFO-engine wait — it is a PMC interrupt wait that no modelled device event can satisfy.

That run previously ended at status 15 with the 32,768-entry device log full, which reads like capacity pressure rather than the truth: the guest is waiting for an interrupt. A busy loop cannot be caught by a table of the last N registers, because it touches far more registers than any bounded table can hold.

## Source-owned change

- `src/runtime/device_registers.h/.c` adds `ff_device_registers_add_status_wait(device, address, mask)` (capacity 4, `FF_DEVICE_STATUS_WAITS`): a declared status input whose masked bits are compared on every read. If the masked value is unchanged for more than `FF_DEVICE_WAIT_LIMIT` (64) reads, the access fails with the bounded wait flag and `ff_device_registers_wait_address` names the register, so the probe reports status 12 (`device poll limit reached`) at that register. A store to the register restarts the count. The generic per-register table is unchanged.
- `tools/fixtures/thread_startup_probe.c` declares `0xFD000100` as a status wait on mask `0x01000000` when `pmcZero` is selected, next to the zero range, with the evidence recorded in place.
- `tests/device_registers_test.c` adds `a_declared_status_wait_trips_despite_a_busy_register_file`: the declared input is polled while more other registers than the generic wait table holds are read every pass, and the declared wait still trips and names the register; duplicate, unaligned and zero-mask declarations are rejected.

## Three-way run over the same module

Module `recomp-post-memory-build-70` rebuilt from this source: **641,451 bytes**, SHA-256 `eb42c5806e5135d6d2d1a463964183116c17151798e438f90a3d4a03590ccd73`.

| profile | stop | routines | kernel ops | init | trace | device events |
| --- | --- | --- | --- | --- | --- | --- |
| canonical (`writeClearing` only) | status 10 on the unmodeled `0xFD000100` read | 276 | 403 | 83/83 | 3,839 | 12,535 |
| control (no flags) | status 5 at `0x200F59` | 277 | 403 | 83/83 | 8,192 | 27,773 |
| `pmcZero` experiment | **status 12 at `0xFD000100`** (device poll limit) | 276 | 403 | 83/83 | 3,967 | 13,240 |

All three report `timerCheck = 0`. The experiment now stops at the wait itself, with 13,240 events instead of a full 32,768-entry log, so nothing has to be inferred from log exhaustion. The canonical profile still stops at the unmodeled read, and the control still reproduces the pre-model `0x200F59`/`0x1FDAC0` recurrence, so the acknowledge behaviour remains attributable to its own flag.

## What this establishes

- The blocking condition is a **wait for a PMC interrupt bit that no modelled device event raises**; it is not engine back-pressure and not a capacity problem.
- The 64-read bound is a wait budget, not a progress device: a run that stops here has not advanced past the wait.

## Limits

Bit 24 is never raised, no interrupt is delivered, no command stream is consumed and there is no engine state. `NV_PMC_INTR_0` stays fail-closed by default. The acknowledge rule for `NV_PGRAPH_INTR`/`NV_PFIFO_INTR_0` remains guest-implied and unverified against hardware. No rendering, audio, fidelity, compatibility or playability is established; all private profile/report evidence stays local.

## Validation

Device-register test optimized and ASan/UBSan; compiled real-VM vector test, 16-byte checked-pointer, kernel-timer and port tests optimized; generator-to-compiled regression 2/2; Python memory-access 12/12; runtime suite 111/111; title-seed 25/25; `npm run check` 302 source files; whitespace checks pass.

## Next task

Decide with evidence whether an interrupt-delivery model is justified (and on what causal event) before any engine expansion; until then keep `NV_PMC_INTR_0` fail-closed. Do not raise the wait, trace or event limits, seed completion bits, or treat a bounded stop as progress.
