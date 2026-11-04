# PFIFO engine/consumer handoff

November 4, 2026. The architect assigned the PFIFO engine/consumer contract to a **new engine workstream**; this lane keeps CPU memory/device-aperture and vector/memory-safety ownership. This note is the bounded handoff: seam API, measured polling trace, unresolved register semantics, and the evidence gate for activating producers. It adds no producer rules. **There is no engine, renderer, timing source or playable screen.**

## What this lane hands over

A producer-free seam in `src/runtime/pfifo_status.h/.c`, unit-tested in `tests/pfifo_status_test.c` and run by `npm run test:native`:

- `ff_pfifo_status_init` — zeroed state;
- `ff_pfifo_status_read` / `ff_pfifo_status_write` — the three reached offsets (`NV_PFIFO_CACHE1_STATUS` `0x1214`, `NV_PFIFO_RUNOUT_STATUS` `0x0400`, `NV_PFIFO_CACHE1_DMA_PUSH` `0x1220`); guest stores are preserved **verbatim**; unknown offsets rejected;
- `ff_pfifo_status_satisfied` — the reached exit predicate `cache1 & 0x10 && runout & 0x10 && !(dma_push & 0x10)`;
- `ff_pfifo_status_consumer_update` — the only consumer-side entry point; **nothing calls it** and activation is disabled until justified.

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

## Evidence gate for the engine workstream

Before any producer rule is activated:

1. **Source of truth** — original guest behaviour or an explicitly authorised reference comparison. The stub reference and "no work pending" are not evidence.
2. **Contract** — for each of the three predicates: the observed event that changes it, the enable/reset rules, and the ordering relative to `CACHE1_PULL0` / `PFIFO + 0x500` writes.
3. **Reproducible traces** — recorded transitions (register, value, site, iteration) that a test can replay.
4. **Producer-connected tests** — a bounded test that drives `ff_pfifo_status_consumer_update` from the recorded conditions and checks `ff_pfifo_status_satisfied` at the expected boundaries.
5. **Loop progress without forced bits** — the reached loop must leave because the modelled state changed, and the report must show the transition; exit alone does not establish fidelity, and no completion bit may be seeded.
6. **Fail-closed remainder** — unknown configuration, unsupported transitions or exhausted budgets must still stop explicitly.

## Reference-derived producer contract (full xemu source)

The earlier audit used the extracted NV2A stub in `.tools/emulation/xboxrecomp-source`, which models no consumer. The fuller retained reference in `.tools/emulation/xemu-source` (main project, read-only) does implement PFIFO, and reading it resolves the meaning of all three polled bits and the producer rule that matters:

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
3. `CACHE1_STATUS.LOW_MARK` is the real gate, and the reference sets it **only after the puller has processed a method**. With an empty pushbuffer nothing is processed, so `LOW_MARK` never sets and the reference model would also leave the guest waiting.

That is decisive for the contract: the reached loop is waiting for **actual FIFO consumption of pushed work**. Satisfying it requires a consumer that processes at least one method, which is exactly what the empty-ring profile deliberately refuses to fake. No producer rule is added here; the engine workstream must decide and evidence the consumption contract.

## Constraints carried over

- Guest stores to `DMA_PUSH` remain verbatim; the seam must not discard them.
- No capacity increase (trace, device events, wait budgets) may be used to escape the loop.
- Reports already state the active experimental inputs (`experiments`) and, when the display path is used, `pcrtc`; the engine work must add its own explicit switch rather than reuse an unrelated one.

## Validation

`npm run test:native` 14/14, `npm run test:evidence` 3/3, runtime suite 120/120, title-seed 25/25, `npm run check` and whitespace checks pass.

## Next task

The engine workstream takes the contract; this lane keeps the canonical profile fail-closed and does not add producer rules. Any future activation must cite the evidence gate above.
