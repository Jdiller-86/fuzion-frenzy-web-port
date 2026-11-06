> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# PMC zero status is an explicit experiment; the default stays fail-closed

November 4, 2026. `NV_PMC_INTR_0` (`0xFD000100`) is not an acknowledge register in the reached code, so it is no longer served by an ad-hoc zero range. Serving it as a zero status is now an opt-in experiment with a control, and the canonical profile keeps it fail-closed. **There is no controllable original screen, engine progress or fidelity result.**

## Evidence from the reached code

- `0x1FDFC3` (an initialiser) stores **zero** to `[device + 0x100]` together with `0x104`, `0x110` and `0x118`. That is the only store the original code makes to the register, and a zero store neither raises nor acknowledges anything.
- `0x1FE00B` then reads `[device + 0x100]`, adds `0x80`, shifts left 16 and adds `[device + 0x000]` into a caller-supplied output — it packs the value into a status snapshot instead of polling it.
- `0x1FE50D` is the only wait: it loops back while bit `0x1000000` (bit 24) is clear.

So the register is a **status input whose bits come from device events**, unlike `NV_PGRAPH_INTR`/`NV_PFIFO_INTR_0`, which the guest acknowledges by storing back exactly the observed bits. Applying the write-1-to-clear rule here would be unsupported, and a zero store meaning "clear everything" is the only acknowledgement-shaped use.

## Source-owned change

- `src/runtime/local-title-proof.mjs` reports `device.pmcZero` alongside `device.writeClearing`; both default to false and only a literal `true` enables them.
- `src/runtime/thread-proof-worker.mjs` sets the flag through `ff_probe_mmio_pmc_zero` before admitting the window, and fails closed if the module cannot honour it.
- `tools/fixtures/thread_startup_probe.c` registers the 4-byte zero range at `0xFD000100` **only** when that flag is set, with the evidence above recorded in place.
- `tests/mmio-profile.test.mjs` pins the parser contract: both flags default off and reject `1`, `"true"`, `{}` and `[]`.

## Three-way run over the same module

Module `recomp-post-memory-build-69` rebuilt from this source: **641,451 bytes**, SHA-256 `f46c5444b90c749f49bda9bca9c5330c815c5638376f57a598b24a2d33f1aee3`. Profiles differ only in their experimental flags.

| profile | `writeClearing` | `pmcZero` | stop | routines | kernel ops | init | trace | device events |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `settings-pfifo.json` | true | absent | status 10 on the unmodeled `0xFD000100` read | 276 | 403 | 83/83 | 3,839 | 12,535 |
| `settings-pfifo-control.json` | absent | absent | status 5 at `0x200F59` | 277 | 403 | 83/83 | 8,192 | 27,773 |
| `settings-pfifo-pmczero.json` | true | true | status 15 at `0xFD003220` | 276 | 403 | 83/83 | 7,518 | 32,768 |

All three report `timerCheck = 0`. The control reproduces the pre-model `0x200F59`/`0x1FDAC0` recurrence, so the acknowledge behaviour is attributable to `writeClearing`. The `pmcZero` run reproduces the earlier PFIFO measurement: `0x1FDC30` drives the channel (`NV_PFIFO_CACHE1_PULL0` stores and status polls) until the bounded device-event log fills. That capacity stop is **not** engine progress.

## Decision

- `NV_PMC_INTR_0` stays fail-closed by default; no zero status, no seeded bit, no interrupt delivery.
- The zero-status variant is an explicitly labelled diagnostic used only to reach the next boundary, and it cannot be mistaken for evidence about hardware, because it is off unless the selected profile says otherwise.
- No capacity was raised: the event log and trace limits are unchanged, and the `pmcZero` run is reported as a bounded-capacity stop.

## Limits

Bit 24 of `NV_PMC_INTR_0` is never raised, no interrupt is delivered, no command stream is consumed and there is no engine state. The acknowledge rule for the other two registers remains guest-implied and unverified against hardware. No rendering, audio, fidelity, compatibility or playability is established; all private profile/report evidence stays local.

## Validation

Device-register, compiled real-VM vector, 16-byte checked-pointer, kernel-timer and port tests pass optimized; Python memory-access 12/12; runtime suite **111/111** (the new profile-control test included); title-seed 25/25; `npm run check` 300 source files; whitespace checks pass.

## Next task

Decide with evidence whether any interrupt-delivery model is justified before engine expansion; until then keep `NV_PMC_INTR_0` fail-closed. Do not seed completion bits, grow trace/event capacity or treat a capacity stop as progress.
