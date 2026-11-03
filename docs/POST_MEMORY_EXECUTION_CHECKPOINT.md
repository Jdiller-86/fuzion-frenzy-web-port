# DMA offsets resolve to candidate guest addresses, but target backing stays unverified

November 3, 2026. Following architect escalation, the project did **not** allocate a separate VRAM buffer or implement a consumer. It added an observation-only DMA resolver: the decoded object maps PUT `0x03B7D000` and GET `0` to candidate guest-physical addresses under an identity assumption, but the mapping is explicitly unverified and the guest bytes at those addresses remain zero. The PFIFO boundary stays fail-closed. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-48/startup.wasm` (local/ignored) |
| Module bytes | 525,113 |
| Module SHA-256 | `19ff32755d85e8db53ed6cb1829b82fb3f2b56f0061655f9146d92ce61156b2b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-35.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged. ABI 24, the sole clock import and all prior synthetic boundaries are unchanged.

## Architect escalation

The architect reviewed the decoded object (class 2, target NVM/VRAM, base 0, limit `0x3FFAFFF`; PUT/GET offsets) and recommended **D: prove target backing and queue provenance first**. Key points:

- Xbox uses unified memory; the NVM/VRAM target designation does not itself establish separate 64 MiB storage.
- The missing piece may be GPU-visible mapping of existing RAM, not more capacity.
- Verify aliasing against pinned sources and the physical-memory contract; do not assume identity from the label.
- Trace the initial GET origin and where original code writes the submitted bytes.
- Expose only checked bounded snapshots of established backing; keep status 13 and no consumer/GET advance.
- If backing cannot be established, report the unresolved mapping rather than synthesizing storage or bytes.

## Source-owned change

- `src/runtime/submission-probe.mjs`: adds `resolveDmaOffset`, which returns a **candidate** guest-physical address only when the object is available, the target is NVM/VRAM, the offset is aligned and within the object limit, and base+offset stays inside modeled guest RAM. It always reports `verified: false`, `backing: 'guest-physical identity (unverified)'` and `reason: 'target backing is not established'`; it never returns bytes, mutates memory or advances GET.
- The probe now reports `putResolved` and `getResolved` alongside the decoded object and snapshots.
- `tests/submission-probe.test.mjs`: covers candidate resolution, AGP/non-NVM targets, object-limit rejection, guest-RAM bounds and unchanged read-only behavior (10 cases).

## Observed values

- Object: instance `0x11C` → RAMIN `0x11C0`; flags `0xB002` (class 2, target NVM/VRAM, adjust 0); base 0; limit `0x3FFAFFF`.
- `putResolved`: candidate `0x03B7D000`, verified false, backing unverified.
- `getResolved`: candidate `0`, verified false, backing unverified.
- Guest-RAM windows at the raw cursor and allocation base: 0 nonzero words each.
- Run: 83/83 initializers, 261 routines, 402 kernel operations; stop status 13 (`pushbuffer consumption required`) at `0xFD800044`.
- Memory statistics unchanged: `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Interpretation

A candidate identity mapping places PUT/GET inside the modeled 64 MiB guest RAM, but the bytes there are zero, so producer-written command data and the initial GET provenance are not established. The NVM target may alias unified RAM, but neither the pinned emulator model nor the proof establishes that alias. Allocating a separate VRAM buffer or decoding at the candidate addresses would be unsupported.

## Next task

Verify the NVM/VRAM target backing and initial GET provenance against pinned sources and the physical-memory contract: whether the target aliases guest physical RAM, where the producer wrote the queue, and what the initial GET means. If backing cannot be established, record the unresolved mapping and continue only with an explicit evidence-based re-plan. No consumer, GET advance, packet execution, interrupts, DMA or rendering is approved.

## Validation

`tests/submission-probe.test.mjs` passes (10 cases). The runtime suite, Python memory-access 10/10 and title-seed 25/25 pass, and `npm run check`/whitespace checks pass. The report saved as `post-memory-browser-20261103-35.json` (3,997,109 bytes).

## Prior milestone: DMA object decode

The instance/object decode established class 2, NVM/VRAM target, base 0 and limit `0x3FFAFFF`; PUT/GET are offsets. Report `post-memory-browser-20261103-34.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime. The resolver is a fail-closed observation, not a mapping proof or command decoder. Next: establish target backing and GET/producer provenance from pinned evidence.
