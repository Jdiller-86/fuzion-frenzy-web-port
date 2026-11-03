# PFIFO DMA object decoded: the pushbuffer is a VRAM-target offset, not a guest address

November 3, 2026. The reached PFIFO stall now has an evidence-backed DMA object. The PFIFO instance register at `0xFD00322C` holds `0x11C`, selecting the RAMIN object at `0x11C0`: flags `0xB002` (class `2`, target NVM/VRAM, adjust `0`), frame `3`, base `0`, limit `0x3FFAFFF`. DMA_PUT `0x03B7D000` and DMA_GET `0` are **offsets into that ~64 MiB VRAM-target object**, not guest addresses. Because the proof models no VRAM target buffer, the queue contents remain unavailable and no consumer is implemented. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-48/startup.wasm` (local/ignored) |
| Module bytes | 525,113 |
| Module SHA-256 | `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-34.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged; only the source-owned diagnostic and its report field changed. ABI 24 and all prior synthetic boundaries are unchanged.

## Evidence and source-owned change

- Pinned xemu model: `nv_dma_load` reads a RAMIN object (`flags`, `limit`, `frame`); `nv_dma_map` returns `vram_ptr + address` with `len = limit`, and PFIFO processes DMA_PUT/GET as offsets into that mapping. `NV_PFIFO_CACHE1_DMA_INSTANCE` is at PFIFO offset `0x122C`; the object address is `(value & 0xFFFF) << 4`. Masks: class `0xFFF`, target `0x30000`, adjust `0xFFF00000`, address `0xFFFFF000`.
- `src/runtime/submission-probe.mjs` now decodes the observed DMA object from the aperture entries: instance register at `0xFD00322C`, object fields at `0xFD700000 + objectAddress`, and reports class/target/adjust/base/limit with an explicit note that PUT/GET are offsets and the mapped buffer is not modeled.
- Tests cover a complete object, missing instance, incomplete fields, and target/adjust masking.

## Observed values

- `instanceRegister` = `0x11C` (284); `objectAddress` = `0x11C0` (4544).
- `flags` = `0xB002` (45058): class `2`, target `NVM/VRAM`, adjust `0`.
- `base` = `0`; `limit` = `0x3FFAFFF` (67,088,383); PUT = `0x03B7D000`; GET = `0`.
- The 256-byte guest-RAM windows at the raw cursor and allocation base remain zero; they are not the mapped buffer.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.

## Interpretation

The pushbuffer is a VRAM-target object covering base 0 through ~64 MiB; PUT/GET are offsets and the queue spans from GET 0 to PUT `0x03B7D000`. The proof has no VRAM target buffer, so the command words are unavailable. A consumer would require a bounded VRAM target model plus PFIFO object mapping and a command decoder; that is a device subsystem decision, not a small extension. No consumer, GET advance, command decoding, interrupt, DMA or rendering is implemented.

## Next task

Decide whether to add a bounded VRAM target model and PFIFO object mapping, then define a synchronous consumer contract (supported packets, downstream effects, work limits, fail-closed behavior). This is an open architecture decision and should receive architect escalation before implementation.

## Validation

`tests/submission-probe.test.mjs` passes (8 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-34.json` (3,996,619 bytes).

## Prior milestone: allocation-base snapshot

The probe added a second window at the observed GPU-instance allocation base; both guest windows were zero while the device aperture held 5,120 nonzero-capable entries. Report `post-memory-browser-20261103-33.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The DMA object decode is evidence only; it is not a command decoder, VRAM model or fidelity acceptance. Next: decide the bounded VRAM/consumer architecture before implementation.
