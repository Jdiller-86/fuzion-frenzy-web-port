# PFIFO completion-flag characterization (read-only)

November 5, 2026. Directed read-only characterization of the boundary after the first pushbuffer packet. **No implementation, hook change, admission, export, seed or translation change was made.** The consumer prototype is unchanged and default-off. This note halts for the responder design review.

## 1. The exact polled word

| Fact | Value |
| --- | --- |
| Device context | `0x00203B90` (loaded from `0x00206720` in `D3D_BlockOnTime`) |
| Fence pointer | `[context + 0x3F0]` = **`0x03BFF000`** |
| Polled word | dword at **`0x03BFF000`**, observed value **3** |
| Context sequence | `[context + 0x1C]` = **9** |
| Ring | base `0x03B7D000`, end `0x03BFD000`; PUT `0x03B7D7CC`; GET after the first packet `0x03B7D008` |
| Other context fields | `[+0x3E4]` = 0 (fence value copy), `[+0x2B60]` = 1 |

The word sits 8 KiB past the ring end and is the first dword of the fence buffer the context points at.

## 2. Polling routine and condition

`D3D_BlockOnTime` (`0x001FB020..0x001FB106`), loop at `0x001FB0C7..0x001FB0DC`:

```
0x1FB0C7  MOV EDX,[EDI+0x3F0]   ; fence pointer = 0x03BFF000
0x1FB0CD  MOV EDI,[EDI+0x1C]    ; current sequence
0x1FB0D0  MOV EAX,EDI
0x1FB0D2  SUB EAX,ESI           ; seq - target
0x1FB0D4  MOV ECX,[EDX]         ; read *fence   <-- the polled access
0x1FB0D6  MOV ESI,EDI
0x1FB0D8  SUB ESI,ECX           ; seq - fence
0x1FB0DA  CMP EAX,ESI
0x1FB0DC  JC  0x001FB0D4        ; loop while (seq-target) < (seq-fence)
```

The loop exits when `fence >= target` in 32-bit wrap arithmetic; the target is the routine's second argument. In the observed run the word was 3 while the sequence was 9, so the wait persisted. The **polling PC** is `0x001FB0D4`; the **routine** is `D3D_BlockOnTime`.

## 3. Writer side in the static call graph

- `D3D_SetFence` (`0x1FAF80`) builds a fence packet through `XMETAL_StartPush` (`0x1FB3B0`) and records entries at `context+0xE8/0xEC`; `[+0x1C]` advances by 2.
- `CDevice_KickOff` (`0x1FAED0`) submits USER PUT and, when `[0x00203748] != 0`, writes `seq - 2` into the fence word through `[context+0x3F0]`.
- `FUN_001D060` writes the fence pointer into `context+0x3F0`; `D3D_BlockOnTime` only reads it.
- The **value the wait needs** is produced by the GPU side after the pushbuffer is consumed. The retained executor documents this as the semaphore release: `BACK_END_WRITE_SEMAPHORE_RELEASE` (`0x1D70`) writes each release into the guest VA of the title's semaphore (for XDK D3D, the pointer at `device+0x30`) after executing everything before it — "which is what lets D3D's fence waits and ring-space checks see real progress" (`src/kernel/nv2a_backend.h`), with `nv2a_pb_set_semaphore_target()` naming the VA.

## 4. What the pinned references document

- **Kick marker and latch.** XDK D3D's KickOff ends with method `0x0310` on subchannel 5 encoding `((pushbuffer_address * 8 | (fence & 0x1F)) << 2) | (wrap & 3)`. The GPU latches it into PGRAPH `0x400B10`; D3D waits for the kick's fence bits and compares them with the semaphore. The executor notes that if the semaphore does not agree, "D3D's wait then lasts for ever" — the exact failure mode seen here (`.tools/emulation/xboxrecomp-source/src/kernel/nv2a_pb_exec.c:3820-3855`).
- **Semaphore target.** `xbox_Nv2aFenceWordVa()` resolves the registered mirror chain (`device_ptr → device+get_ptr_off`) to the guest VA of the GPU-visible fence word; the executor keeps the NV2A fence latch consistent with what the mirror writes (`src/kernel/xbox_memory_layout.c:1259-1280`).
- **GET publication.** `nv2a_pb_scan.c` documents that the walk itself publishes DMA GET, and that D3D's fence wait reads GET and sleeps on an event once the fence matches (`src/kernel/nv2a_pb_scan.c:245-255`, `:430-445`).

These are retained project references derived from the pinned public xemu/NV2A behaviour; they are not an observation of the original hardware.

## 5. What a responder must do

Consuming one packet is not enough. The responder must also **publish the completion value the title waits for**:

1. Consume the pushbuffer commands up to the submitted PUT (the prototype step already does one packet).
2. Execute the fence/semaphore release semantics for the kick: write the release counter into the guest fence word (`0x03BFF000` here) once everything before it has been consumed.
3. Keep the PGRAPH `0x400B10` latch's fence bits consistent with that semaphore, or D3D's wait never exits.
4. Do not initialize GET or invent a fence value; the release must come from the submitted stream.

This is a design requirement, not an implementation. The exact value written, the release method subset, and the latch model are the next architect decision.

## 6. Limits

The evidence is a synthetic harness run with a retained module and the pinned project mirrors. It establishes the polled address, condition, polling site and documented writer path; it does not establish original GPU timing, rendering or playability. No consumer path was enabled in any committed profile.

## 7. Bounded responder result (authorized)

One default-off responder step was authorized with binding corrections: drain GET to PUT using the existing decode/refusal rules, then, once only, write the current context sequence to the guest VA held by `[context+0x3F0]` (the fence word, never the pointer field), with the PGRAPH latch `0x400B10` explicitly unmodeled.

Module `recomp-post-memory-build-112`: **716,327 bytes**, SHA-256 `0304f2f6f8b103cf03365ab88ec62253742e9101e57857b559edcb13a6eb8d65`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no consumer) | status 13 at `0xFD800044` | 339 | 405 |
| consumer | **status 1 at `0xFE000094`**, ordinal **159** | **340** | 405 |

The consumer drained **173 packets / 499 words** to PUT (`GET == PUT == 0x03B7D7CC`), released the fence **3 -> 9** at `0x03BFF000`, and made **15** VM checks — no budget stop. `D3D_BlockOnTime` exited. The new stop is an **unsupported Xbox service**: ordinal 159, `modeled: false`, one argument, status 1, return `0x1F5543` (the `D3DDevice_BlockUntilVerticalBlank` site). The run advanced one routine beyond the control.

**Disclosures:** the release value is the current context sequence (modeled), not the stream's kick-marker fence bits; the PGRAPH latch is unmodeled; the one-shot write can pre-satisfy a later wait on an unsubmitted fence; GET is never initialized and the pointer field is unchanged. The next stop is a service, not a latched wait, so the latch and a second fence wait remain open. The consumer remains default-off.
