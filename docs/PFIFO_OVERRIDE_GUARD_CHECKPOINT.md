# Empty-ring override guarded against a changed PUT; profile path still reaches 0x1FB5B0

**Later evidence (November 4):** [the XMM checkpoint](XMM_MEMORY_SURFACE_CHECKPOINT.md) adds the checked 16-byte XMM surface, admits `0x1FA460`, and advances execution to `0x1FB600`.

November 4, 2026. The adopted empty-ring override now checks the current DMA_PUT before serving the base as DMA_GET. If the original code changes PUT, the override stops serving and the normal fail-closed read/stop path applies. The profile path still reaches `0x1FB5B0`; the no-profile path still stops at status 13. **There is no playable screen, renderer or device model.**

## Source-owned change

- `src/runtime/pfifo_channel.h/.c` adds `ff_pfifo_channel_should_serve_empty_get(channel, put)`. It returns true only when:
  - the channel is ready and enabled;
  - the supplied PUT is aligned, inside the ring and equal to the channel's current PUT;
  - that PUT equals the base.
  It does not mutate state and returns false for a changed, unaligned or out-of-range PUT.
- `tools/fixtures/thread_startup_probe.c` now reads the current USER DMA_PUT slot on a DMA_GET read and serves the base only when the guard is true. A changed PUT no longer receives the empty-ring value.
- `tests/pfifo_channel_test.c` covers the empty case, a non-base PUT, a changed PUT, an unaligned/out-of-range PUT and a null channel. Optimized and ASan/UBSan pass.

## Runs

Private build `recomp-post-memory-build-60` (527,474 bytes, SHA-256 `e6aab9451e2f7bb65031da69c202a56a7c211b3238302e806d05b8b8186dbb01`):

| Settings | Stop | Routines | Kernel operations |
| --- | --- | --- | --- |
| `settings-pfifo.json` (empty-ring profile) | status 2, `0x1FB5B0`, return `0x1FB63D` | 262 | 402 |
| `settings-mmio-20.json` (no profile) | status 13, `0xFD800044` | 261 | 402 |

83/83 initialization callbacks returned in both runs. Builds 58 and 51 remain preserved.

## Priority reconciliation

- The current reachable module is `recomp-post-memory-build-60`; the empty-ring override is now guarded against changed PUT.
- The next substantive blocker is unchanged: `0x1FB5B0` is inside `FUN_001FB580`, and the guard-memory recomposition rejects the raw matrix helper at `0x1FA460`. The override guard does not resolve that admission problem and does not translate new code.
- No host matrix substitute, raw-pointer bypass or non-empty ring consumption was introduced.

## Limits

The guard applies only to the explicit empty-ring profile. It does not turn the model into a general consumer, does not execute a non-empty ring and does not establish the kernel D3D submission path, rendering, audio, fidelity or playability.

## Validation

`tests/pfifo_channel_test.c` passes optimized and ASan/UBSan. `tests/startup-proof.test.mjs` 40/40; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Return to the guard-memory admission of `0x1FA460` (bounded modeled memory surface or a narrower closure), then recompose and rerun the adopted profile. Do not bypass guard-memory or claim rendering/fidelity without evidence.
