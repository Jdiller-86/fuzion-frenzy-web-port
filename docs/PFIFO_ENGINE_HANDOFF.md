> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# PFIFO engine/consumer handoff

November 4, 2026. The architect assigned the PFIFO engine/consumer contract to a **new engine workstream**; this lane keeps CPU memory/device-aperture and vector/memory-safety ownership. This note is the bounded handoff: seam API, measured polling trace, unresolved register semantics, the pinned reset rule and the evidence gate for activating producers. It adds no producer rules. **There is no engine, renderer, timing source or playable screen.**

## What this lane hands over

A producer-free seam in `src/runtime/pfifo_status.h/.c`, unit-tested in `tests/pfifo_status_test.c` and run by `npm run test:native`:

- `ff_pfifo_status_init` — zeroed state;
- `ff_pfifo_status_read` / `ff_pfifo_status_write` — the three reached offsets (`NV_PFIFO_CACHE1_STATUS` `0x1214`, `NV_PFIFO_RUNOUT_STATUS` `0x0400`, `NV_PFIFO_CACHE1_DMA_PUSH` `0x1220`); guest stores are preserved **verbatim**; unknown offsets rejected;
- `ff_pfifo_status_satisfied` — the reached exit predicate `cache1 & 0x10 && runout & 0x10 && !(dma_push & 0x10)`;
- `ff_pfifo_status_consumer_update` - the only consumer-side entry point; **nothing calls it** and activation is disabled until justified;
- `ff_pfifo_status_reference_reset` - the default-off pinned reset adapter: sets `LOW_MARK` on both status registers, leaves `DMA_PUSH` and `consumer_updates` untouched, and is initialization-only.

The canonical profile stays fail-closed: the three bits are never synthesised and the run stops explicitly.

## Measured polling trace (from the experiment device log)

The reached loop `0x1FE4D5` (exit `0x1FE522`) over one run:

| register | reads | writes | dominant write | site |
| --- | --- | --- | --- | --- |
| `0xFD002500` (`PFIFO + 0x500`) | 2 | 71 | `0x1` ×66 | `0x1FDD92` |
| `0xFD003250` (`NV_PFIFO_CACHE1_PULL0`) | 1 | 70 | `0x1` ×66 | `0x1FDD92` |
| `0xFD002080` (`PFIFO + 0x80`) | 64 | 0 | — | `0x1FDD9E` |
| `0xFD003214` (`NV_PFIFO_CACHE1_STATUS`) | 65 | 0 | — | `0x1FE4DB` |
| `0xFD003220` (`NV_PFIFO_CACHE1_DMA_PUSH`) | 64 | 2 | `0x0`/`0x1` | `0x1FDD1A` |
| `0xFD002400` (`NV_PFIFO_RUNOUT_STATUS`) | 0 | 0 | — | never accessed |

Measured predicate order: `CACHE1_STATUS` bit `0x10` blocks first (65 unchanged reads → status 12 in the diagnostic profile), `DMA_PUSH` is polled, `RUNOUT_STATUS` is never reached. A 64-iteration loop body performs the pump and the checks; only the toolchain-side evidence places it there, no debugger or original-runtime observation is involved.

## Unresolved register semantics

These were the unknowns when the handoff was written; the section below resolves the three polled bits from the fuller reference and leaves only the pumped offsets open.

- `PFIFO + 0x500` (`0xFD002500`) and `PFIFO + 0x80` (`0xFD002080`) have **no definition** in the retained NV2A header; their meaning is unevidenced.
- `NV_PFIFO_CACHE1_STATUS` (`0x1214`) and `NV_PFIFO_CACHE1_DMA_PUSH` (`0x1220`) define no fields, so their bit `0x10` meaning is unevidenced; only the guest's test is known.
- `NV_PFIFO_RUNOUT_STATUS` defines `RANOUT (1<<0)`, `LOW_MARK (1<<4)`, `HIGH_MARK (1<<8)`, so bit `0x10` is `LOW_MARK` by name; who sets it is unknown.
- The extracted reference in `.tools/emulation/xboxrecomp-source` models no FIFO consumer: `pfifo_read` returns raw storage except the two interrupt registers and `pfifo_write` only stores (`nv2a_core.c:612-659`).

## Guest writes to the PFIFO registers (whole admitted translation)

A scan of every admitted function, not only the reached path, settles whether the guest could produce the status bits itself:

| register | reads | writes | write sites |
| --- | --- | --- | --- |
| `0xFD003214` `NV_PFIFO_CACHE1_STATUS` | 5 | **0** | none |
| `0xFD002400` `NV_PFIFO_RUNOUT_STATUS` | 2 | **0** | none |
| `0xFD003220` `NV_PFIFO_CACHE1_DMA_PUSH` | 4 | 5 | `0x1FDD79`, `0x1FE853`, `0x1FE915`, `0x2004BA`, `0x20107C` |
| `0xFD003250` `NV_PFIFO_CACHE1_PULL0` | 3 | 9 | `0x1FDC72`, `0x1FDC9E`, `0x1FDD92`, `0x1FE9BE`, `0x1FE9E5`, `0x1FEA51` |
| `0xFD002500` (`PFIFO + 0x500`) | 2 | 14 | including `0x1FDD92`, `0x1FDC72`, `0x1FDC9E`, `0x2010D8` |

The two status registers are **read-only in the guest's own code**, so no admitted routine can set `CACHE1_STATUS.LOW_MARK` or `RUNOUT_STATUS.LOW_MARK`; any producer must be a modelled consumer, and the engine workstream owns that decision. The guest does write the DMA `ACCESS` bit and both pump registers, which is the input side a consumer would act on.

## Submission-to-consumption contract (producer-free)

This is the single contract the engine workstream inherits. It is assembled from the retained run's own register traffic and the pinned xemu reference, and it adds no producer code. The reference is `.tools/emulation/xemu-source/hw/xbox/nv2a/pfifo.c` with `nv2a_regs.h` line numbers as cited.

### Observed submission state

The channel is fully programmed by the guest, but the ring is empty. The channel-programming writes in the retained run (`browser-70.json`, module `recomp-post-memory-build-70`, SHA-256 `0a03de96cd248f296ba8327ddac5ded9de70d4c944e7797d62b675b1cadf1da4`) are:

| register | observed value | site |
| --- | --- | --- |
| `NV_PFIFO_MODE` (`0xFD002504`) | `0x1` (channel 0 in DMA mode) | `0x200419` |
| `NV_PFIFO_CACHE1_PUSH1` (`0xFD003204`) | `0x100` (mode DMA, channel 0) | `0x200475` |
| `NV_PFIFO_CACHE1_PUSH0` (`0xFD003200`) | `0x1` (`ACCESS`) | `0x2004C0` |
| `NV_PFIFO_CACHE1_DMA_INSTANCE` (`0xFD00322C`) | `0x11C` (instance address `0x11C0`) | `0x20047F` |
| `NV_PFIFO_CACHE1_DMA_CTL` (`0xFD003230`) | `0` | `0x20047F` |
| `NV_PFIFO_CACHE1_DMA_PUT` (`0xFD003240`) | `0` | `0x20005A`, `0x20027C`, `0x20047F` |
| `NV_PFIFO_CACHE1_DMA_GET` (`0xFD003244`) | `0` | `0x20005A`, `0x20027C`, `0x20047F` |
| `NV_PFIFO_CACHE1_DMA_STATE` (`0xFD003228`) | `0` | `0x20047F` |
| `NV_PFIFO_CACHE1_DMA_FETCH` (`0xFD003224`) | `0x86078` | `0x20047F` |
| `NV_PFIFO_CACHE1_DMA_PUSH` (`0xFD003220`) | `0x1` (`ACCESS`) | `0x2004BA` |
| `NV_PFIFO_CACHE1_PULL0` (`0xFD003250`) | `0x1` (`ACCESS`) | `0x2004C0` |
| `PFIFO + 0x500` (`0xFD002500`) | `0x1` | `0x2004C0` |

Every `DMA_PUT` write in the run is `0`, and every `DMA_GET` write is `0`. The 512 KB ring is `0x03B7D000`..`0x03BFD000` with `PUT` at the base (device-context observation), so the pushbuffer is empty; the `0` register values are the software reset values, not a populated pointer. The channel is therefore *enabled and idle*, which is exactly the state the reached wait loop expects to leave.

### First admissible method, or the exact missing input

In the reference pusher (`pfifo.c:298-375`) the first word at `DMA_GET` is a command header. A data word is dispatched to the puller only when the header set `METHOD_COUNT != 0` (`pfifo.c:329`); the puller then returns the number of words processed. `LOW_MARK` is set only when `num_proc > 0` (`pfifo.c:242-243`). The pusher's loop breaks immediately when `GET == PUT` (`pfifo.c:301`).

Therefore, in this run:

- **No first method exists to name.** `DMA_PUT == DMA_GET` at every programming site, so the pusher reads no word and the puller is never entered. There is no header, subchannel, method or argument word to cite.
- **The exact missing input for the consumer contract is a non-empty submission**: `DMA_PUT` advanced past `DMA_GET`, containing at least one method header with `METHOD_COUNT > 0` and its data word(s), plus a puller dispatch that returns `num_proc > 0`. The run does not contain it, and the fail-closed profile does not contain it.
- **The reference itself provides the empty-cache reset state**: `nv2a_reset` zeroes the PFIFO register block and then sets `CACHE1_STATUS.LOW_MARK` because the method cache is empty (`hw/xbox/nv2a/nv2a.c:307-317`), and `pfifo_read` returns `RUNOUT_STATUS.LOW_MARK` for every read (`pfifo.c:51-52`). The pusher clears `LOW_MARK` only before a dispatch (`pfifo.c:340`) and sets it after processing (`pfifo.c:242-243`), so an empty, unsubmitted cache keeps the reset value. The synthetic aperture had seeded both as zero, so the wait could not pass; seeding the reference reset value lets it pass (see [the reset-state checkpoint](PFIFO_RESET_STATE_CHECKPOINT.md)). This is an initialization rule, not a consumer update or a forced bit.
- The guest does own a command builder that advances `DMA_PUT` (`0x1FAF1C`, `PUT = pointer & 0x03FFFFFF`, per the ring checkpoint), but the reached run never executes it before the wait. That submission is the missing input, not a consumable queue.

### Reset / enable / error / ordering

The reference rules, and where the run observes each input, are:

| rule | reference | observed input |
| --- | --- | --- |
| Pusher runs only when `PUSH0.ACCESS`, `DMA_PUSH.ACCESS` set and `DMA_PUSH.STATUS` clear | `pfifo.c:267-269` | `PUSH0 = 1` @ `0x2004C0`; `DMA_PUSH = 1` @ `0x2004BA`; `.STATUS` clear |
| Pusher additionally requires `MODE[channel]` set and `PUSH1.MODE == DMA` | `pfifo.c:282`, `:285` | `MODE = 1` @ `0x200419`; `PUSH1 = 0x100` @ `0x200475` |
| Pusher stops when `DMA_GET == DMA_PUT` | `pfifo.c:301` | both `0` |
| Puller runs only when `PULL0.ACCESS` set **and** `LOW_MARK` clear | `pfifo.c:170-171` | `PULL0 = 1` @ `0x2004C0`; `LOW_MARK` never set |
| `LOW_MARK` is cleared before dispatch and set after `num_proc > 0` | `pfifo.c:340`, `:242-243` | never reached |
| `RUNOUT_STATUS` reads always report `LOW_MARK` | `pfifo.c:52` | the guest never reads it before the first predicate fails |
| `DMA_PUSH.STATE` has no producer; only the stored value is returned | `pfifo.c:255-274` | guest writes only `ACCESS` (`1`) or `0`; clears `.STATUS` (`0x1000`) at `0x1FDD79` |
| A DMA error suspends the pusher (`DMA_PUSH.STATUS = 1`) | `pfifo.c:444-449` | the guest's recovery branch is present but not entered |
| Protection error when `DMA_GET >= dma_len` | `pfifo.c:302-305` | not reached |

The guest's own reset/recovery sequence is part of the contract and must not be replaced by a synthesised one:

- Disable the puller (`PULL0 = 0`, `PFIFO + 0x500 = 0`), wait a bounded `0xFFFF` iterations for `PULL0` bit `12` to clear, clear `PULL0 + 8` (`0x3258`), then re-enable (`PULL0 = 1`, `PFIFO + 0x500 = 1`) at `0x1FDC72`..`0x1FDC9E`.
- On a `DMA_PUSHER` interrupt, acknowledge `PFIFO_INTR_0` bit `12` (`0x1000`), clear `DMA_STATE`, and if `PUT != GET` advance `GET` by `4` (`0x1FDCE8`..`0x1FDD0B`).
- Clear `DMA_PUSH.STATUS` (`0x1000`) to unsuspend at `0x1FDD79`, then re-enable `PULL0` and `PFIFO + 0x500` at `0x1FDD92`.

Ordering that a consumer must preserve: `LOW_MARK` clear → method dispatch → `LOW_MARK` set; the pusher advances `DMA_GET` only over words it actually dispatched; the guest's pump writes (`PULL0`, `PFIFO + 0x500`) follow the dispatch and may be freely repeated while `LOW_MARK` is set. No consumer may set `LOW_MARK` before a dispatch, set `RUNOUT_STATUS`, or produce `DMA_PUSH.STATE`.

### Unresolved pumped accesses

These two are written or read by the pump on every iteration but have no definition in the retained header, so no engine meaning is assigned:

- `PFIFO + 0x500` (`0xFD002500`): 13 writes across the translation, toggled `0`/`1` in lockstep with `PULL0.ACCESS`; the pump polls its bit `4` at `0x1FDD70` after `LOW_MARK`. It is not `NV_PFIFO_MODE` (`0x504`) or `NV_PFIFO_DMA` (`0x508`).
- `PFIFO + 0x80` (`0xFD002080`): read-only in the guest; the pump masks it with `1` and ORs it into its return value at `0x1FDD9E`; 64 reads per pump iteration.

Secondary undefined accesses carried by the same context restore, also unevidenced and preserved verbatim: `PULL0 + 8` (`0x3258`), `PFIFO + 0x4C` (`0x204C`, written `0x1FFFFF` at `0x200317`), and `0x1260`..`0x126C`. The aperture stores them; it derives no engine state.

## Evidence gate for the engine workstream

Before any producer rule is activated:

1. **Source of truth** — original guest behaviour or an explicitly authorised reference comparison. The stub reference and "no work pending" are not evidence.
2. **Contract** — for each of the three predicates: the observed event that changes it, the enable/reset rules, and the ordering relative to `CACHE1_PULL0` / `PFIFO + 0x500` writes.
3. **Reproducible traces** — recorded transitions (register, value, site, iteration) that a test can replay.
4. **Producer-connected tests** — a bounded test that drives `ff_pfifo_status_consumer_update` from the recorded conditions and checks `ff_pfifo_status_satisfied` at the expected boundaries.
5. **Loop progress without forced bits** — the reached loop must leave because the modelled state changed, and the report must show the transition; exit alone does not establish fidelity, and no completion bit may be seeded.
6. **Fail-closed remainder** — unknown configuration, unsupported transitions or exhausted budgets must still stop explicitly.

## Reference citations for the contract (full xemu source)

The earlier audit used the extracted NV2A stub in `.tools/emulation/xboxrecomp-source`, which models no consumer. The fuller retained reference in `.tools/emulation/xemu-source` (main project, read-only) does implement PFIFO; the bullets below are the citation appendix for the contract above.

- `hw/xbox/nv2a/nv2a_regs.h:137-139` — `NV_PFIFO_CACHE1_STATUS` (`0x1214`) bit `0x10` is `NV_PFIFO_CACHE1_STATUS_LOW_MARK` (`1 << 4`), with `HIGH_MARK` at `1 << 8`.
- `nv2a_regs.h:123-126` — `NV_PFIFO_RUNOUT_STATUS` (`0x400`) bit `0x10` is `NV_PFIFO_RUNOUT_STATUS_LOW_MARK`.
- `nv2a_regs.h:140-145` — `NV_PFIFO_CACHE1_DMA_PUSH` (`0x1220`): bit `0x00` is `ACCESS`, bit `0x10` is **`STATE`**, bit `0x1000` is `STATUS` (suspended). **This corrects the earlier reading** that called bit `0x10` "STATUS".
- `hw/xbox/nv2a/pfifo.c:51-52` — reads of `NV_PFIFO_RUNOUT_STATUS` return `LOW_MARK` unconditionally ("low mark empty"), so the guest's `runout & 0x10` requirement is always satisfied by the reference.
- `pfifo.c:165-173` — the puller refuses to run when `PULL0.ACCESS` is clear **or** `CACHE1_STATUS.LOW_MARK` is set: `LOW_MARK` means "cache1 at low mark / nothing to pull".
- `pfifo.c:242-245` — `LOW_MARK` is **set after the puller processes a method** (`num_proc > 0`).
- `pfifo.c:339-341` — `LOW_MARK` is **cleared before** the puller dispatches the next method.
- `pfifo.c:261-274` — the pusher stalls while `DMA_PUSH.ACCESS` is clear or `DMA_PUSH.STATUS` (suspended) is set; the comment there notes `DMA_PUSH_STATE` as the busy bit but the model never writes it.
- `pfifo.c:443-451` — a DMA error sets `DMA_PUSH.STATUS` (suspended).

Consequences for the reached predicate (`CACHE1_STATUS & 0x10` set, `RUNOUT_STATUS & 0x10` set, `DMA_PUSH & 0x10` clear):

1. The runout condition is free — the reference always reports `LOW_MARK`.
2. `DMA_PUSH.STATE` (`0x10`) has **no producer** in the reference; it reads back whatever is stored, and the guest writes only `ACCESS` (`0x1`) or `0` to that register, so the clear requirement holds without modelling.
3. `CACHE1_STATUS.LOW_MARK` is the real gate. The reference sets it at reset (empty cache) and after the puller processes a method, and clears it only before the next dispatch. The run programs a valid DMA channel but leaves `DMA_PUT == DMA_GET == 0` on the empty 512 KB ring, so no dispatch occurs; the reset value therefore remains set. The pinned reset rule is an initialization input, not a consumer.

That is decisive for the contract: the reached loop is waiting for a cache at low mark. The pinned reset rule already supplies that state for an empty, unsubmitted cache; no consumer is needed to satisfy this wait, and no producer rule is added. A future consumer still owns any non-empty submission and the transitions that follow it.

## Explicit reference-reset experiment

The reset rule is wired through the existing `ff_pfifo_status` seam as a separate, default-off input:

- `ff_pfifo_status_reference_reset` sets `CACHE1_STATUS.LOW_MARK` and `RUNOUT_STATUS.LOW_MARK` and touches nothing else. It does **not** call `ff_pfifo_status_consumer_update`, leaves `DMA_PUSH` and `consumer_updates` unchanged, and is idempotent. Call it once before the guest runs, never from a poll or a read.
- `device.pfifoReset: true` selects it. The probe applies the adapter once during aperture initialization and seeds the two status registers from the seam; the canonical profile leaves it off and stays fail-closed.
- The adapter has no GET, PUT or guest-memory access: it changes two status words only. The retained runs confirm the `DMA_PUT`/`DMA_GET` writes are byte-for-byte identical across the canonical, `engineWait` and `pfifoReset` profiles.

Measured with module `recomp-post-memory-build-70` (SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`):

| profile | stop | routines | kernel ops | device events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pfifoReset` | status 1 at `0xFE000094` (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |

The reset run reads `CACHE1_STATUS = 0x10` once and `RUNOUT_STATUS = 0x10` once, exits the wait on the first iteration, executes the admitted `0x1FCB00`, `0x1FD42F` and `0x1FD410`, and then stops at unsupported kernel ordinal 2 (`AvSendTVEncoderOption`) at `0xFE000094`. It reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`, and the report validator rejects any consumer update, more than one reset, or a missing `LOW_MARK`. The validator also requires `pfifoResetState` exactly when `pfifoReset` is declared. The canonical profile still stops on the unmodeled PMC read, and the `engineWait` control still stops on the status predicate.

No frontier expansion was performed this cycle; `0x1FCB00` is now an evidence-based next target.

## Constraints carried over

- Guest stores to `DMA_PUSH` remain verbatim; the seam must not discard them.
- No capacity increase (trace, device events, wait budgets) may be used to escape the loop.
- Reports already state the active experimental inputs (`experiments`) and, when the display path is used, `pcrtc`; the engine work must add its own explicit switch rather than reuse an unrelated one.
- The two undefined pumped offsets (`PFIFO + 0x500`, `PFIFO + 0x80`) stay value-preserving: no read is reinterpreted as status and no write is translated into engine progress.

## Validation

`npm run test:native` 14/14 (the status seam now covers the reference reset, zero consumer updates, repeat initialization and preserved stores), `npm run test:evidence` 4/4, runtime suite 134/134, title-seed 25/25, `npm run check` and whitespace checks pass. The adapter is default-off; the canonical profile remains fail-closed.

## Next task

The next genuine stop is unsupported kernel ordinal 2 (`AvSendTVEncoderOption`) at `0xFE000094`; option 6 cannot be modeled from the conflicting references, so the run stays fail-closed. `0x1FCB00`, `0x1FD42F` and `0x1FD410` are admitted; the narrow batches added only those helpers and no broad closure.

The engine workstream still owns any non-empty submission and the transitions after it. This lane does not inject consumer state from a poll or a read, does not treat the reset value as a general completion rule, and does not infer display timing. Any future activation must cite the evidence gate above.
