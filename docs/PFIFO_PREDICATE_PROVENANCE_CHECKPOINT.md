> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The PFIFO engine predicates have no producer in the retained reference either

November 4, 2026. Read-only provenance audit of the three exit predicates the reached loop waits on. The register identities are confirmed, one field name is documented, and the retained reference **does not model them**: its whole PFIFO block is an explicit stub. The engine contract therefore cannot be derived from local evidence, and this lane keeps the predicates fail-closed. **There is no engine, renderer, timing source or playable screen.**

## What the reference does with those registers

`.tools/emulation/xboxrecomp-source/src/nv2a/nv2a_core.c`:

- the PFIFO section is labelled `PFIFO - command FIFO (stub for Phase 1)` with `Full PFIFO with push buffer processing comes in Phase 2-3` (lines 612-616);
- `pfifo_read` (619-637) computes only `NV_PFIFO_INTR_0` and `NV_PFIFO_INTR_EN_0`; every other offset, including the three predicates, returns `d->pfifo.regs[addr]` verbatim;
- `pfifo_write` (640-659) likewise handles only the two interrupt registers and otherwise stores `d->pfifo.regs[addr] = val`.

So the reference preserves guest writes but never derives consumer state. There is no code that sets `CACHE1_STATUS`, sets `RUNOUT_STATUS`, or clears `CACHE1_DMA_PUSH` from engine activity.

## What the register definitions do establish

`.tools/emulation/xboxrecomp-source/src/nv2a/nv2a_regs.h`:

| register | offset | defined fields |
| --- | --- | --- |
| `NV_PFIFO_RUNOUT_STATUS` | `0x400` | `RANOUT (1 << 0)`, `LOW_MARK (1 << 4)`, `HIGH_MARK (1 << 8)` |
| `NV_PFIFO_CACHE1_STATUS` | `0x1214` | none |
| `NV_PFIFO_CACHE1_DMA_PUSH` | `0x1220` | none |

The loop tests bit `0x10` on all three. For `RUNOUT_STATUS` that bit is `LOW_MARK` by name; for the other two the header defines no fields, so the meaning of their bit `0x10` is **not** evidenced here. Field names describe a register's layout; they say nothing about which engine transition produces a value.

## Consequence

- A consumer contract ("which observable FIFO state may set `CACHE1_STATUS`/`RUNOUT_STATUS` and clear `DMA_PUSH` bit `0x10`") cannot be written from the retained reference: the block is a stub, and stubs are not evidence.
- Writing those bits to release the loop would fabricate engine progress, which the empty-ring profile deliberately excludes. The canonical profile therefore stays fail-closed.
- The already-measured predicate order remains the useful hand-off: `CACHE1_STATUS` bit `0x10` blocks first (65 unchanged reads), `DMA_PUSH` is polled (64 reads), `RUNOUT_STATUS` is never reached.

## What the owning lane needs

Either a fuller FIFO model (command consumption, cache status, runout marks) with its own evidence, or controlled original-runtime observation of the same registers while the title runs. Until one exists, this lane's profiles keep the predicates unmodelled and stop on them explicitly.

## Runner improvement in the same commit

`tools/run-native-tests.mjs` now also builds and runs every fixture under `-fsanitize=address,undefined -fno-sanitize-recover=all`, so the sanitizer claims in recent checkpoints are reproducible with the same command. All fixtures pass in both modes (**12/12 runs**).

## Validation

`npm run test:native` 12/12; runtime suite 113/113; Python memory-access 12/12; title-seed 25/25; `npm run check` 310 source files; whitespace checks pass.

## Next task

Hand this audit to the lane that owns the FIFO consumer: the predicates have no local producer, so that lane must supply either a modelled consumer with evidence or original-runtime observations. This lane keeps its canonical profile fail-closed and does not synthesise the bits.
