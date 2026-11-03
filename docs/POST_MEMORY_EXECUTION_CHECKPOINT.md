# PFIFO state observer: DMA channel 0, but USER and CACHE1 DMA registers do not alias

November 3, 2026. A bounded source-owned PFIFO state observer now reports the reached channel state. The channel is in DMA mode with channel 0 and DMA instance `0x11C`, but the USER DMA registers and the PFIFO CACHE1 copies disagree: USER PUT/GET are `0x03B7D000`/`0`, while CACHE1 PUT/GET are `0`/`0`. The proof keeps those two apertures as separate synthetic storage, so the observed channel state is internally inconsistent; a real device mirrors USER writes into the CACHE1 DMA registers. The queue-producer question and the consumer remain unresolved. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-49/startup.wasm` (local/ignored) |
| Module bytes | 525,566 |
| Module SHA-256 | `05887417db7b7b06e6848e2210da8b103761ceb41fe51bc3083b117115557f97` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-38.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/submission-probe.mjs`: adds `pfifoState` decoded from the aperture entries — PUSH1 (`0xFD003204`: mode/channel), DMA_INSTANCE (`0xFD00322C`), CACHE1 DMA_PUT/GET (`0xFD003240`/`0xFD003244`), and the USER DMA_PUT/GET. It reports whether the USER and CACHE1 copies agree and notes that a real device aliases them.
- `tests/submission-probe.test.mjs`: covers DMA mode/channel, instance, inconsistent and consistent PUT values, and missing PUSH1 (12 cases).

## Observed values

- `push1 = 0x100`: **DMA mode, channel 0**.
- `instanceRegister = 0x11C` → object `0x11C0`.
- `cachePut = 0`, `cacheGet = 0`; `userPut = 0x03B7D000`, `userGet = 0`.
- `aliasConsistent = false`.
- Guest write map: 48 pages written; none in the claim pages 15224–15228.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The title is in a valid PFIFO DMA channel and writes the USER DMA_PUT, but the synthetic aperture does not mirror it into the PFIFO CACHE1 register that a device-side consumer would read. The proof therefore cannot establish a coherent queue state, on top of the previously observed absence of CPU-written queue bytes. A consumer would need both the USER/CACHE1 alias and a verified queue producer.

## Next task

Add the evidence-backed USER↔PFIFO CACHE1 DMA alias (PUT `0xFD800040`↔`0xFD003240`, GET `0xFD800044`↔`0xFD003244`, REF `0xFD800048`↔`0xFD003248`) to the synthetic aperture, with bounded tests, then re-observe the channel state. This is still a modeling prerequisite: no consumer, GET advance, packet decoding, interrupt, DMA or rendering may be fabricated. Separately, the queue producer remains unidentified.

## Validation

`tests/submission-probe.test.mjs` passes (12 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-38.json` (4,017,766 bytes).

## Prior milestone: guest write-page map

48 pages written; none in the published queue claim. Report `post-memory-browser-20261103-37.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The observer is a diagnostic, not an alias model or consumer. Next: add the USER/CACHE1 alias and continue producer/queue evidence.
