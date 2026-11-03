# Submission provenance: PUT equals the GPU-instance claim end; the instance aperture holds nonzero data

November 3, 2026. A bounded submission-provenance summary now records where the USER DMA_PUT write came from and how it relates to the observed GPU-instance claim. The single USER PUT write occurs at site `0x1FAF1C` with value `0x03B7D000`; the claim is `0x03B78000` + `0x5000`, so **PUT equals the claim end**. The device aperture's `0xFD700000`–`0xFD704FFC` region holds 5,120 entries, 559 nonzero, but the guest write map still shows no CPU writes to the claim pages. The queue producer and initial GET provenance remain unresolved, and no consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-50/startup.wasm` (local/ignored) |
| Module bytes | 526,712 |
| Module SHA-256 | `bd0acb033fcf0e9b6c768ef56a9b802225652099f6d1b797cb71e2f8cc996646` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-40.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/submission-probe.mjs`: adds `submissionSummary` — count and last site/value of USER DMA_PUT writes, the observed claim base/bytes, whether PUT equals the claim end, and the count/nonzero count of the `0xFD700000`–`0xFD705000` aperture entries. It is explicitly observational and does not establish backing.
- `src/runtime/thread-proof-worker.mjs`: passes the device event log so the summary can identify the write site.
- `tests/submission-probe.test.mjs`: covers write-site/value selection, claim-end alignment, instance counts and the observational note (14 cases).

## Observed values

- USER DMA_PUT writes: **1**, at site `0x1FAF1C` (2076444), value `0x03B7D000`.
- Claim: base `0x03B78000`, bytes `0x5000`; **PUT = claim end**.
- Instance-aperture entries `0xFD700000`–`0xFD705000`: **5,120**, **559 nonzero**.
- Guest write map: 48 pages; none in the claim.
- PFIFO state: DMA mode, channel 0, instance `0x11C`; alias-consistent PUT `0x03B7D000`, GET `0`; GET never advances.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

PUT is the physical end of the claimed GPU-instance allocation, and the corresponding device-aperture region contains 559 nonzero entries, while the guest claim pages are untouched. This is consistent with the instance/queue data living in the separate synthetic device aperture rather than guest RAM, but the translation between the claim/aperture and the DMA object's NVM target is still unverified. Decoding at the guest cursor or advancing GET remains unsupported.

## Next task

Determine whether the `0xFD700000` instance-aperture content is the queue that the DMA object addresses: establish the aperture↔physical translation, inspect the method-like entries, and verify the initial GET. This is evidence work only; no consumer, GET advance, packet decoding, interrupt, DMA or rendering may be fabricated.

## Validation

`tests/submission-probe.test.mjs` passes (14 cases). The device-register tests, runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-40.json` (4,018,276 bytes).

## Prior milestone: USER/CACHE1 alias

The three DMA register pairs now share storage; channel state is alias-consistent but GET stays 0. Report `post-memory-browser-20261103-39.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The summary is provenance evidence, not a producer, translation or consumer. Next: establish the aperture↔physical translation and inspect the method-like entries.
