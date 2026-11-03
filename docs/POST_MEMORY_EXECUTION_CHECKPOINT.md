# UMA backing verified; the observed queue region and 4 KiB before PUT are zero

November 3, 2026. The pinned xemu model confirms Xbox UMA: `nv2a_init_memory` sets `d->vram = ram` with the comment "xbox is UMA - vram *is* ram", and `nv_dma_map` returns `vram_ptr + address`. The DMA object's NVM target therefore aliases guest physical RAM; the resolver now reports PUT `0x03B7D000` and GET `0` as **verified** guest addresses. But a 4 KiB window before PUT (base `0x03B7C000`) and all five 4 KiB pages of the observed `0x03B78000`/`0x5000` claim are zero. Producer-written queue bytes and the initial GET provenance remain unestablished, so no consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-48/startup.wasm` (local/ignored) |
| Module bytes | 525,113 |
| Module SHA-256 | `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-36.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24, the sole clock import and all prior synthetic boundaries are unchanged.

## Evidence

- Pinned xemu: `nv2a_init_memory` assigns the system RAM region to `d->vram` ("xbox is UMA - vram *is* ram"); `nv_dma_map` returns `d->vram_ptr + dma.address` and length `dma.limit`. The DMA object (class 2, target NVM/VRAM, base 0, limit `0x3FFAFFF`) therefore addresses guest physical RAM.
- The submission probe now marks the PUT/GET resolution `verified: true` with backing `'xemu UMA: VRAM is system RAM (identity)'`, and captures up to 4 KiB before the cursor.
- It also emits a bounded per-4 KiB nonzero-dword summary of the observed GPU-instance claim, storing counts rather than bytes.
- Tests cover the verified resolution, target/limit/RAM failures and the page summary.

## Observed values

- `putResolved`: `0x03B7D000`, verified, backing `xemu UMA: VRAM is system RAM (identity)`.
- `getResolved`: `0`, verified, same backing.
- 4 KiB before PUT (`0x03B7C000`–`0x03B7CFFF`): **0 nonzero words**.
- Allocation claim pages `0x03B78000` + {0, 0x1000, 0x2000, 0x3000, 0x4000}: **0 nonzero words each**.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

The DMA target is backed by guest RAM, but the published queue region is empty in the proof's guest memory. Either the producer wrote the command stream elsewhere within the base-0 object, the initial GET does not identify the ring start, or the driver has not written the queue on this path. Decoding at PUT or advancing GET would therefore be unsupported.

## Next task

Locate the producer's queue writes and the initial GET provenance from static and runtime evidence: which original routines write the DMA ring, what base/offset they use, and whether GET=0 is a real ring start or an uninitialized value. A bounded write-observation or targeted static trace is the next evidence step; do not fabricate packets, a get pointer, interrupts, DMA or rendering.

## Validation

`tests/submission-probe.test.mjs` passes (11 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-36.json` (4,016,753 bytes).

## Prior milestone: unverified candidate resolver

The resolver reported candidate guest addresses with backing unverified. Report `post-memory-browser-20261103-35.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The resolver establishes the target backing, not the queue contents or a consumer. Next: establish producer writes and initial GET provenance.
