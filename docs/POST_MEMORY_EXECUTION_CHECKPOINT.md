# PFIFO provenance pass: PUT comes from context state; GET=0 is a cleared value

November 3, 2026. A bounded provenance pass following architect guidance traced the reached PUT/GET pair and found no evidence of a produced queue. PUT is derived from context state in `0x001FAED0` and written once at `0x1FAF1C`; it equals the end of the observed GPU-instance claim. GET is zero and its zero writes come from context initialization/restore sites, not a channel/ring setup. The wait compares a context field with GET. No consumer is implemented, and the fail-closed status 13 boundary is retained. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-50/startup.wasm` (local/ignored) |
| Module bytes | 526,712 |
| Module SHA-256 | `bd0acb033fcf0e9b6c768ef56a9b802225652099f6d1b797cb71e2f8cc996646` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-41.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24 and all prior synthetic boundaries are unchanged.

## Provenance table

| Item | Source / evidence | Address domain | State |
| --- | --- | --- | --- |
| PUT value `0x03B7D000` | `0x001FAED0`: `ECX = [ctx+0x400]` when `[ctx+0xC]&4`, else `[ctx]`; `PUT = ECX & 0x3FFFFFF`; store at `0x1FAF1C` | USER `0xFD800040` (alias CACHE1 `0xFD003240`) | observed |
| PUT meaning | equals `0x03B78000` + `0x5000`, the GPU-instance claim end | guest physical / claim | observed |
| Context write | `0x001FAED0`: `[ctx+0x18] = ECX` | guest context | observed |
| GET | `0`; zero writes at `0x20005A`, `0x20027C`, `0x20047F` (context init/restore) | CACHE1 `0xFD003244` / USER alias | cleared value, not a ring initialization |
| Wait | `0x001FBB00` compares `[ctx]` with GET under mask `0x0FFFFFFF` | guest context / device | observed |
| DMA object | class 2, target NVM/VRAM, adjust 0, base 0, limit `0x3FFAFFF`; xemu UMA maps VRAM to system RAM | guest physical | decoded |
| RAMIN `0xFD700000` | xemu keeps a separate 1 MiB RAMIN region; the proof has 5,120 entries / 559 nonzero context-like values | RAMIN | not the UMA target |
| Queue bytes | 4 KiB before PUT and all claim pages zero; guest write map has no claim pages | guest RAM | absent |
| Initial GET / ring base | no observed channel/ring setup; only context clears | unresolved | unresolved |
| Producer | no CPU writes to the claim; RAMIN data is context, not production | unresolved | unresolved |

## Source-owned change

- `src/runtime/submission-probe.mjs`: `submissionSummary` now lists the unique sites that write zero to the CACHE1 PUT/GET/REF registers, making the cleared-GET provenance explicit.
- `tests/submission-probe.test.mjs`: covers the zero-site lists (14 cases).

## Observed values

- USER DMA_PUT: 1 write at `0x1FAF1C`, value `0x03B7D000`; PUT = claim end.
- CACHE1/USER zero-write sites: PUT and GET `{0x20005A, 0x20027C, 0x20047F}`; REF `{0x20027C}`.
- Instance aperture 5,120 entries / 559 nonzero; guest claim pages untouched.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Decision

Per the architect recommendation, this single provenance pass is complete. The initial GET/ring base and queue producer remain unestablished, so no consumer or GET advance is authorized. Retain the fail-closed boundary and choose an independent startup prerequisite next (fallback B) rather than guessing PFIFO initialization.

## Next task

Choose the next bounded independent startup prerequisite, preserving the PFIFO boundary and all evidence. Candidates include a bounded kernel/PFIFO channel-setup model only if pinned evidence establishes the intended initial GET and ring base; otherwise pursue a different reached subsystem/service. Do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/submission-probe.test.mjs` passes (14 cases). Device-register tests, the runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-41.json` (4,018,516 bytes).

## Prior milestone: submission provenance

USER PUT write site/value, claim-end alignment and instance counts were first recorded. Report `post-memory-browser-20261103-40.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The table is evidence, not a channel contract or consumer. Next: select an independent prerequisite.
