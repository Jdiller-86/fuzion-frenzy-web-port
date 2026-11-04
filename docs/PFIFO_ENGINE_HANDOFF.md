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

- `PFIFO + 0x500` (`0xFD002500`) and `PFIFO + 0x80` (`0xFD002080`) have **no definition** in the retained NV2A header; their meaning is unevidenced.
- `NV_PFIFO_CACHE1_STATUS` (`0x1214`) and `NV_PFIFO_CACHE1_DMA_PUSH` (`0x1220`) define no fields, so their bit `0x10` meaning is unevidenced; only the guest's test is known.
- `NV_PFIFO_RUNOUT_STATUS` defines `RANOUT (1<<0)`, `LOW_MARK (1<<4)`, `HIGH_MARK (1<<8)`, so bit `0x10` is `LOW_MARK` by name; who sets it is unknown.
- The retained reference models no FIFO consumer: `pfifo_read` returns raw storage except the two interrupt registers and `pfifo_write` only stores (`nv2a_core.c:612-659`).

## Evidence gate for the engine workstream

Before any producer rule is activated:

1. **Source of truth** — original guest behaviour or an explicitly authorised reference comparison. The stub reference and "no work pending" are not evidence.
2. **Contract** — for each of the three predicates: the observed event that changes it, the enable/reset rules, and the ordering relative to `CACHE1_PULL0` / `PFIFO + 0x500` writes.
3. **Reproducible traces** — recorded transitions (register, value, site, iteration) that a test can replay.
4. **Producer-connected tests** — a bounded test that drives `ff_pfifo_status_consumer_update` from the recorded conditions and checks `ff_pfifo_status_satisfied` at the expected boundaries.
5. **Loop progress without forced bits** — the reached loop must leave because the modelled state changed, and the report must show the transition; exit alone does not establish fidelity, and no completion bit may be seeded.
6. **Fail-closed remainder** — unknown configuration, unsupported transitions or exhausted budgets must still stop explicitly.

## Constraints carried over

- Guest stores to `DMA_PUSH` remain verbatim; the seam must not discard them.
- No capacity increase (trace, device events, wait budgets) may be used to escape the loop.
- Reports already state the active experimental inputs (`experiments`) and, when the display path is used, `pcrtc`; the engine work must add its own explicit switch rather than reuse an unrelated one.

## Validation

`npm run test:native` 14/14, `npm run test:evidence` 3/3, runtime suite 120/120, title-seed 25/25, `npm run check` and whitespace checks pass.

## Next task

The engine workstream takes the contract; this lane keeps the canonical profile fail-closed and does not add producer rules. Any future activation must cite the evidence gate above.
