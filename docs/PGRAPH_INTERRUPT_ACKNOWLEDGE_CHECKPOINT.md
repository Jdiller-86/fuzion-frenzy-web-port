# Acknowledged PGRAPH interrupt store clears the recurrence; the next stop is an unmodeled PMC interrupt-status read

November 4, 2026. Model-visible PGRAPH interrupt status is now acknowledged by the guest's own store instead of being latched by it. The enter-only recurrence described in [the PGRAPH recurrence checkpoint](PGRAPH_INTERRUPT_RECURRENCE_CHECKPOINT.md) is gone: `0x1FDAC0` is no longer entered at all, `0x200F59` runs and returns, and execution proceeds to the next reached boundary — an unmodeled read of `0xFD000100` (`NV_PMC_INTR_0`) from the original wait loop at `0x1FE4D5`. **There is no playable screen, renderer or device model.**

## Observed guest sequence that the model now follows

The reached translated code in `0x1FE4D5` is a drain/wait loop:

- `0x1FE4F6` calls `0x1FDC30` (context work) and then reads `[device + 0x400100]`.
- `0x1FE4FD` compares that read with zero; `0x1FE506` calls `0x1FDAC0` (the PGRAPH interrupt handler) only when it is nonzero.
- `0x1FE50D` tests `[device + 0x100]` against `0x1000000` and loops back to `0x1FE4DB` while that bit is clear, calling `0x1FD700` in between when it is set.

`0x1FDAC0` reads the same `[device + 0x400100]`, reads `0x108` and `0x704`, and stores back exactly the bit it observed (`0x1000`) before waiting for `0x400700` to read zero and calling `0x200F59`, which re-enters `0x1FDAC0` while `[device + 0x400100]` is nonzero. This is an interrupt drain loop: read the status, acknowledge the observed bit, handle it, re-read.

Under plain read-back storage the only writer of that bit was the guest's own acknowledge store, so the model raised the interrupt it then drained forever. With no pending status in this synthetic profile the register reads zero, `0x1FE4FD` skips the handler call, and `0x200F59` completes instead of recursing.

## Source-owned change

- `src/runtime/device_registers.h/.c` adds a bounded write-clearing declaration (`FF_DEVICE_WRITE_CLEARING`, capacity 4) with `ff_device_registers_add_write_clearing`. On a 4-byte store to a declared register the stored value becomes `stored & ~written`, so the store acknowledges the bits it carries and the remaining status stays set; the ordered event keeps the value the guest actually stored. A status bit can never be latched by the guest writing it. Sub-dword stores to a declared register are rejected (`FF_BOOT_UNSUPPORTED`) rather than partially applied. The declaration cannot overlap a self-clearing register.
- `tools/fixtures/thread_startup_probe.c` declares exactly one register in the reached profile, `0xFD400100` (`NV_PGRAPH_INTR`), with the observed-sequence comment recorded above.
- `tests/device_registers_test.c` seeds pending status, acknowledges one bit and asserts the other stays set, asserts a zero store and an unset-bit store change nothing, asserts a partial store is rejected, and covers duplicate/unaligned/out-of-window declarations.

## Evidence limits

The clear-on-store rule is **implied by the guest's own sequence** (read status, store back exactly the observed bit, re-read, and re-enter only while nonzero) and by the fact that the loop cannot terminate under read-back. It is not independently verified against hardware: the local xemu-derived NV2A reference in `.tools` treats PGRAPH registers as plain stored words (`pgraph_write` stores `val`), so it neither confirms nor contradicts the clear. The model is therefore limited to declared registers, applies only to 4-byte stores, and is declared explicitly in the reached profile rather than assumed globally.

## Run

- Private module `recomp-post-memory-build-68` rebuilt from this source: 639,690 bytes, SHA-256 `bd5f5c623f6b3a07dbb02eb0b02c8f62d30cdc54bab7c2520aba04826433f3b3`.
- Canonical profile run: **276** distinct routines, 83/83 initializers, 403 kernel operations, `timerCheck = 0`, DPC head `0x03E00008` self-looped.
- The trace is 3,839 events and contains **no** `0x1FDAC0` entry: `0x200F59` is entered once and returns.
- New stop: status 10 (`guest memory access stopped`) on an unmodeled **read** of `0xFD000100` (`NV_PMC_INTR_0`), stop stack `0x3C10C44`, reported return field `0x205D64`. The guest is waiting for the `0x1000000` (bit 24) PFIFO interrupt that this synthetic profile never raises.
- The 8,192-entry trace capacity is no longer reached; the earlier recurrence consumed 4,352 of its entries.

## Exploratory private run (not the canonical result)

Adding a 4-byte zero range at `0xFD000100` to a copy of the profile (`settings-pfifo-pmc.json`) serves the status read as zero and lets the loop continue; that run then stops on the next unmodeled **write** at `0xFD002100`. The supplied register list is already at its 64-entry cap, so the status had to be expressed as a zero range. This is recorded only to identify the next boundary: a zero status is not evidence of how interrupt delivery should behave, and the exploratory profile is not committed.

## Limits

No rendering, audio, fidelity, original-game compatibility or playability is established. `0xFD000100` is not modelled, no interrupt is synthesized, and no device event is invented. All private profile/report evidence stays local.

## Validation

`tests/device_registers_test.c` optimized and ASan/UBSan; kernel-timer, port and 16-byte checked-pointer tests optimized; Python memory-access 11/11; title-seed 25/25; runtime suite 110/110; `npm run check` and whitespace checks pass.

## Next task

Decide with evidence how `NV_PMC_INTR_0` should behave before this wait loop. Either model bounded interrupt delivery for the reached PFIFO path (the same contract question as the empty-ring/PUT-GET boundary) or stop explicitly at that read with the observed sequence. Do not seed a static status bit, raise a capacity, or claim rendering/fidelity without evidence.
