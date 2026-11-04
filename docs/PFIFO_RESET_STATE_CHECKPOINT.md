# The pinned reset state passes the PFIFO low-mark wait; execution reaches 0x1FCB00

November 4, 2026. The reached loop `0x1FE4D5` waits for `NV_PFIFO_CACHE1_STATUS.LOW_MARK` and `NV_PFIFO_RUNOUT_STATUS.LOW_MARK` with `NV_PFIFO_CACHE1_DMA_PUSH.STATE` clear. The pinned reference already supplies that state for an empty, unsubmitted cache: `nv2a_reset` zeroes the PFIFO register block and then sets `CACHE1_STATUS.LOW_MARK` (`hw/xbox/nv2a/nv2a.c:307-317`), and `pfifo_read` returns `RUNOUT_STATUS.LOW_MARK` for every read (`pfifo.c:51-52`). The pusher clears `LOW_MARK` only before a dispatch (`pfifo.c:340`) and sets it after processing (`pfifo.c:242-243`). The synthetic aperture had seeded both registers as zero, so the wait could never pass. Seeding the pinned reset values lets the original wait leave and advance to untranslated `0x1FCB00`. **There is no engine, renderer, timing source or playable screen.**

## Correction to the earlier contract

The prior handoff said the reference did not model the empty-cache reset state and that only a consumer could produce `LOW_MARK`. That was incomplete: it read `pfifo.c` but missed `nv2a.c:307-317`. The reference does model it, as a reset initialization rule, not a consumer update. The corrected contract is in [the engine handoff](PFIFO_ENGINE_HANDOFF.md).

## Source-owned adapter

- `src/runtime/pfifo_status.h/.c` adds `ff_pfifo_status_reference_reset`. It ORs `LOW_MARK` into `cache1_status` and `runout_status`, leaves `dma_push` and `consumer_updates` untouched, adds no capability and is idempotent. It is initialization-only: never called from a poll or a read.
- `tools/fixtures/thread_startup_probe.c` adds the default-off `ff_probe_mmio_pfifo_reset` control. When selected, it applies the adapter once during aperture initialization and seeds `0xFD003214` and `0xFD002400` from the seam. The injection is at initialization, not from polling, and the adapter has no GET, PUT or guest-memory access.
- `src/runtime/local-title-proof.mjs` accepts the literal `device.pfifoReset: true`; `describeMmioExperiments` reports `pfifoReset`; `proof-report.mjs` accepts the name and requires `pfifoStatus` exactly when `pfifoReset` is declared, so the report cannot show the proof without the input or claim the input without the proof; the worker sets the control before the aperture is admitted. The canonical profile leaves it off and remains fail-closed.

## Proof requested by the architect

| property | evidence |
| --- | --- |
| initial `LOW_MARK` with zero consumer updates | `tests/pfifo_status_test.c` applies the adapter to an initialized seam: both status registers read `0x10`, `consumer_updates == 0`, and `satisfied` is nonzero with a clear `DMA_PUSH.STATE`. |
| unchanged GET/PUT/command memory | The adapter only changes two status words. The canonical, `engineWait` and `pfifoReset` reports contain the same `DMA_PUT`/`DMA_GET` writes (`0` at `0x20005A`, `0x20027C`, `0x20047F`); the executed trace up to and including the first enter of the wait function `0x1FE4D5` is identical in the canonical and reset runs (3,837 events), so the command-building path before the wait is unchanged. No adapter code touches guest memory. |
| repeat initialization | The native test calls the adapter twice on the same seam and asserts identical values and still zero consumer updates. |
| preserved controls | Canonical still stops at status 10 on `0xFD000100`; the `engineWait` control still stops at status 12 on `0xFD003214`. |
| no injection from polling | The adapter is called once in `ff_probe_mmio_profile`, before the guest runs; no read or status-wait path calls it. |
| end-to-end report | The reset run reports `pfifoStatus: { cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`. `proof-report.mjs` rejects any report with a consumer update, more than one reset, or a missing `LOW_MARK`, and requires `pfifoStatus` exactly when `pfifoReset` is declared, so the invariant and its declaration cannot diverge. |

## Retained profile comparison

Module `recomp-post-memory-build-70`, SHA-256 `4b8c2688519cb4338c32cf82479a909c41a4cb1d15d0bce7d1b25b54b5185874`.

| profile | stop | routines | kernel ops | device events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pfifoReset` | **status 2 at `0x1FCB00`** (return `0x1FD4C4`) | 276 | 404 | 12,584 |

The reset run reads `CACHE1_STATUS = 0x10` once and `RUNOUT_STATUS = 0x10` once, leaves the wait on the first iteration, and stops at untranslated `0x1FCB00`. The reachability filter reports `entered = 1` for `0x1FCB00` (called by `0x1FD4A8`), so the next admission is evidence-based rather than speculative.

## Limits

- The reset value is from the authorized reference, not an original-console capture.
- It does not model a consumer, a non-empty submission, rendering, timing or fidelity.
- The title still stops at the next untranslated routine, and no frontier expansion was performed this cycle.
- `pfifoReset` and `engineWait` are separate experiments and should not be combined.

## Validation

`npm run test:native` 14/14 (status seam in optimized and ASan/UBSan); `npm run test:evidence` 4/4; runtime suite 134/134; title-seed 25/25; `npm run check` and whitespace checks pass. The private module, profiles and reports remain local and ignored.

## Next task

Export `0x1FCB00` (and its closure) from the analysis project, recompose with `--guard-memory`, build and rerun with the `pfifoReset` profile; keep the canonical profile fail-closed and do not treat the reset value as a general completion rule.
