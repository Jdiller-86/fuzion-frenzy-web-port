# The PFIFO status seam exists; the contract owner does not

November 4, 2026. The reached loop's exit predicates now have a typed, tested seam in source, with no producer and no behaviour change. The ownership question was escalated to the architect again this cycle and **failed with a provider error**, so it remains unresolved. **There is no engine, renderer, timing source or playable screen.**

## What was added

`src/runtime/pfifo_status.h/.c` encodes exactly what has been measured and nothing more:

- the three reached offsets — `NV_PFIFO_CACHE1_STATUS` (`0x1214`), `NV_PFIFO_RUNOUT_STATUS` (`0x0400`), `NV_PFIFO_CACHE1_DMA_PUSH` (`0x1220`) — and the single polled bit `0x10`;
- guest stores are preserved verbatim, so a translated write to `DMA_PUSH` is not lost;
- `ff_pfifo_status_satisfied` evaluates the reached exit predicate (`cache1 & 0x10`, `runout & 0x10`, `!(dma_push & 0x10)`);
- `ff_pfifo_status_consumer_update` is the only way consumer-side state can change, and **nothing in the project calls it**;
- unknown offsets are rejected and a null state is tolerated.

`tests/pfifo_status_test.c` covers the default state, guest stores, distinct registers, the all-three-condition predicate, unrelated bits, unknown offsets and null handling. It is registered in `npm run test:native` and passes optimized and under ASan/UBSan (**14/14 runs**).

## What this does not do

The seam carries no rules about *when* a consumer would set or clear those bits. The retained reference models no FIFO consumer (its PFIFO block is an explicit stub), so any such rule would be invented. The canonical profile therefore still stops before the loop can exit, and no runtime path uses the seam yet.

## Ownership

This lane's documented next task was to hand the PFIFO engine/consumer contract to its owning lane, but no such lane exists: this lane owns CPU-side memory/device-aperture and vector safety; the geometry lane is on hold pending whole-chain reference review; there is no renderer or engine workstream.

The architect escalation for this cycle failed with `subscription_sharing_usage_limit_exceeded`, so the assignment remains **unresolved**. (Sol was reached successfully in an earlier cycle; this failure is the current, time-specific result.) The seam is proposed as preparation only and does not claim assignment.

## Validation

`npm run test:native` 14/14; runtime suite 113/113; Python memory-access 12/12; generator-to-compiled regression 2/2; title-seed 25/25; `npm run check` 312 source files; whitespace checks pass.

## Next task

Assign the PFIFO engine/consumer contract explicitly (architect or user). Whoever takes it must supply the producer rules and their evidence, then wire the seam's consumer entry point; this lane keeps the predicates fail-closed until then and does not synthesise them.
