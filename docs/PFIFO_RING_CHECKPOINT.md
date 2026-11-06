# Pushbuffer ring pointer model; ring base/end and command source still unresolved

November 4, 2026. The command-source pass continued from the corrected channel setup. The title's D3D device code builds commands at a current push pointer, writes a wrap jump when it needs space, and writes `DMA_PUT` from that pointer. The ring base and end are fields of the device context and are not established by the retained runtime report. This milestone adds a read-only pointer model that describes explicit values and encodes the observed jump/mask arithmetic; it does **not** infer empty/wrap semantics or authorize consumption. **There is no playable screen, renderer or device model.**

## Static findings

| Routine | Finding |
| --- | --- |
| `D3D8::CDevice_MakeSpace` at `0x1FB110` | When space is short, writes a wrap jump at the current push pointer using `(base & 0x0FFFFFFF) + 1`, waits for `(DMA_GET \| 0x80000000)` to reach the current pointer, then restarts at the base. Reads GET through `context+0x8c1` and falls back to `DMA_SUBROUTINE` at `+0x324C`. |
| `FUN_001FACC0` at `0x1FACC0` | Initializes the current push pointer to `param_1[4]` and derives a bounded span from `DAT_00206958`; the base and end are context fields. |
| `D3D8::XMETAL_StartPush` at `0x1FB3B0` | Ensures space and exposes the current push pointer to the command builder. |
| `D3D8::D3D_SetFence` at `0x1FAF80` | Writes a small fence command sequence at the current pointer and advances it. |
| `D3D8::CDevice_KickOff` at `0x1FAED0` | Writes `DMA_PUT` as `pointer & 0x03FFFFFF`; the retained write is `0x03B7D000` at site `0x1FAF1C`. |

The observed `PUT` equals the end of the 20 KB GPU-instance claim `0x03B78000`..`0x03B7D000`, which is consistent with the pushbuffer allocation beginning at `0x03B7D000`, but the context base/end values and the command-buffer ownership are not observed. `GET=0` is outside that range.

## Source-owned change

`src/runtime/pfifo-ring.mjs` exports:

- `encodeRingJump(base)` — the observed wrap word `(base & 0x0FFFFFFF) | 1`, rejecting values outside a 32-bit unsigned integer.
- `describePfifoRing({ base, end, put, get, ramBytes })` — validates integer, aligned, bounded values and reports `ringBytes`, `jumpWord`, `putPhysical`/`getPhysical` (`& 0x03FFFFFF`), `putInsideRing`, `getInsideRing`, `zeroGet` and `samePointer`.
- The model deliberately does not decide whether the ring is empty or wrapped, does not read or write guest memory, and does not advance `DMA_GET`.

`tests/pfifo-ring.test.mjs` covers the observed example, the low-28-bit jump/26-bit address masks, both pointers inside, a put at the base, and invalid sizes/alignment/bounds/pointers. It is wired into `npm run test:runtime`.

## Limits

- The context base and end are not established. The model describes explicit values only; it does not claim the title's ring geometry.
- `GET=0` remains outside the observed candidate range; the command source and buffer ownership are unresolved.
- A zero claim is not a submitted queue. No consumer, GET advance, jump follow, packet, get pointer, interrupt, DMA or rendering is added.

## Validation

`tests/pfifo-ring.test.mjs` passes 5/5; the runtime suite passes 109/109; Python memory-access is 10/10 and title-seed 25/25; `npm run check` and whitespace checks pass. Module `recomp-post-memory-build-51` (526,955 bytes, SHA-256 `cd0bf176…`), ABI 24 and D060 are unchanged.

## Next task

Establish the device-context ring base/end and the command-buffer ownership before any consumer work. Determine why `GET=0` is outside the candidate ring and whether the kernel D3D submission path populates it. If the evidence gives a bounded base/end and a populated range, return to the consumer; otherwise continue another independent prerequisite. Do not infer emptiness from pointers or treat the zero claim as a queue.
