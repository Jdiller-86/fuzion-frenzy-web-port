# USER and PFIFO CACHE1 DMA registers now alias; the queue is still unwritten

November 3, 2026. The synthetic aperture now supports evidence-backed address aliases, and the probe declares the three reached USER↔PFIFO CACHE1 DMA pairs (PUT, GET, REF). The observed channel state is coherent: DMA mode, channel 0, instance `0x11C`, CACHE1/USER PUT `0x03B7D000`, GET `0`. The wait still cannot complete because GET never advances, and the guest write-page map still shows no CPU writes to the published queue claim. No consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-50/startup.wasm` (local/ignored) |
| Module bytes | 526,712 |
| Module SHA-256 | `bd0acb033fcf0e9b6c768ef56a9b802225652099f6d1b797cb71e2f8cc996646` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-39.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The translation is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/device_registers.c/.h`: bounded alias pairs (up to four). Two aligned in-window addresses share one stored entry; the canonical address is the pair's first address. Events keep the address the translated block actually used. Alias addresses cannot be duplicated or declared after any access.
- `tools/fixtures/thread_startup_probe.c`: declares USER↔CACHE1 `DMA_PUT` (`0xFD800040`↔`0xFD003240`), `DMA_GET` (`0xFD800044`↔`0xFD003244`) and `REF` (`0xFD800048`↔`0xFD003248`), based on xemu's `user.c` routing to the PFIFO CACHE1 registers. Adds `ff_probe_mmio_alias_count` / `ff_probe_mmio_alias_word` exports.
- `src/runtime/device-report.mjs`: reports the declared alias pairs (bounded to four).
- `src/runtime/thread-proof-worker.mjs`: passes the reported aliases to the submission probe.
- `src/runtime/submission-probe.mjs`: `pfifoState` resolves declared aliases to the shared entry and reports consistency.
- `tests/device_registers_test.c`: covers shared storage via either address, event addresses, count, duplicate/unaligned/out-of-window/overlap rejection and declaration-after-access.
- `tests/submission-probe.test.mjs`: covers alias-aware PFIFO state (13 cases).

## Observed values

- Aliases: three pairs reported.
- Canonical entries: `0xFD800040` = `0x03B7D000` (4 writes), `0xFD800044` = `0` (3 writes); no separate CACHE1 entries.
- `pfifoState`: DMA mode, channel 0, instance `0x11C`, cachePut = userPut = `0x03B7D000`, cacheGet = userGet = `0`, `aliasConsistent = true`.
- Guest write map: 48 pages; none in the claim.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The register views now agree, but sharing storage does not create queue work: GET remains 0 and no CPU-written queue bytes exist. The missing pieces are the queue producer and the initial GET provenance, not the register alias. Advancing GET or decoding at the cursor would still be unsupported.

## Next task

Identify the expected queue producer from static and runtime evidence, or establish that the initial GET=0 is not a valid ring start. A targeted static trace of the routines that compute the PUB value and the ring base, or a bounded write-observation around the instance/queue region, is the next evidence step. Do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`device_registers_test.c` passes optimized and ASan/UBSan; `tests/submission-probe.test.mjs` passes (13 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-39.json` (4,017,913 bytes).

## Prior milestone: PFIFO state observer

DMA mode/channel 0/instance `0x11C`; USER PUT/GET `0x03B7D000`/0 vs CACHE1 0/0, alias inconsistent. Report `post-memory-browser-20261103-38.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The alias is a register-view model, not a queue producer or consumer. Next: establish producer/queue provenance.
