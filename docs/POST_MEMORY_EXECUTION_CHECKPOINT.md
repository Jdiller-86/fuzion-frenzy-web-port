# PFIFO header scan: the submitted range starts with 4,096 zero command headers

November 3, 2026. A source-owned read-only NV2A pushbuffer scanner now inventories the submitted `DMA_GET..DMA_PUT` range without executing anything or advancing GET. From GET 0 to PUT `0x03B7D000` (15,594,496 words), the first 4,096 words all decode as the zero header (`method 0`, `subchannel 0`, `count 0`) with no jumps, calls, returns or unknown words. The scan is budget-limited and reports truncation; it confirms the submitted range's prefix is zero-filled, not a command stream. No consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-50/startup.wasm` (local/ignored) |
| Module bytes | 526,712 |
| Module SHA-256 | `bd0acb033fcf0e9b6c768ef56a9b802225652099f6d1b797cb71e2f8cc996646` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-42.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Source-owned change

- `src/runtime/pfifo-scan.mjs`: read-only NV2A/NV20 header scanner. It decodes increasing and non-increasing method headers, jump/call/return words and unknown words, tracks parameter words so payload is not mistaken for headers, caps the scan at 4,096 words, and fails closed outside aligned, bounded memory. It executes no method, writes no memory and never advances GET.
- `src/runtime/submission-probe.mjs`: reports `pushbufferScan` for `GET..PUT`.
- `tools/serve-startup-proof.mjs`: serves the new module on the source-only proof host.
- `tests/pfifo-scan.test.mjs`: covers header inventory, parameter words, flow words, truncation and invalid ranges/budgets; wired into `npm run test:runtime`.
- `tests/submission-probe.test.mjs`: covers the read-only scan field (15 cases).

## Observed values

- Scan range: GET `0` → PUT `0x03B7D000`; total **15,594,496** words.
- Scanned: **4,096** words, truncated; **0** pending parameters, **0** jumps, **0** calls, **0** returns, **0** unknown.
- Inventory: `methodKinds = 1`, `{subchannel 0, method 0, count 4096}` — the zero header repeated.
- PFIFO views alias-consistent (PUT `0x03B7D000`, GET `0`); GET never advances.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The submitted range's prefix is zero-filled; it does not contain recognizable commands. This agrees with the zero guest claim/pre-PUT windows and the unproduced-queue evidence. It does not establish what the range would contain if the producer ran, nor whether GET=0 is the intended ring start. Advancing GET or executing the zero headers would be unsupported.

## Next task

Choose the next bounded independent startup prerequisite or a provenance step for the producer, preserving the fail-closed boundary. Do not treat the zero header inventory as an empty-queue completion or fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/pfifo-scan.test.mjs` and `tests/submission-probe.test.mjs` pass (19 combined cases). Device-register tests, the runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-42.json` (4,019,026 bytes).

## Prior milestone: provenance pass

PUT derives from context state and equals the claim end; GET=0 comes from context clears. Report `post-memory-browser-20261103-41.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The scanner is an inventory, not an executor or consumer. Next: choose an independent prerequisite or producer-provenance step.
