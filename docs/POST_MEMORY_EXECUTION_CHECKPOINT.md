# Original execution completes 0x1FDF70 and reaches 0x1FFF25

November 3, 2026. The previously untranslated helper at `0x001fdf70` was admitted from a retained read-only seed. The browser worker executed it, returned through `0x001ffe27` to `0x001fe0c6`, and continued to untranslated `0x001fff25`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-13` (local/ignored) |
| Functions / instructions | 349 / 13,191 |
| Admitted helper | `0x001fdf70`, 19 body bytes / 6 instructions |
| Module | `research/verification/recomp-post-memory-build-26/startup.wasm` (local/ignored) |
| Module bytes | 467,100 |
| Module SHA-256 | `240b0bcd859b5c834b48059052a947b85e824c5633fefde93afd571db7ea0012` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-08.json` (local/ignored) |

The selection reuses the retained 26-seed post-memory inputs plus `recomp-post-memory-next-09/seed.json`. The generated header retains the source digest and 349-function lookup. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance service.

The read-only export reports `0x001fdf70` as a 19-byte/6-instruction established function with no direct calls. Its body adds its stack argument to `[ECX+0x1A0]`, stores the result and returns with `RET 0x4`. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **239** distinct original routines executed (up from 238).
- **400** kernel operations completed; the new helper made no kernel service call.
- `0x001fdf70` executed and returned to `0x001fff1a`.
- `0x001ffe27` returned to `0x001fe0c6`.
- `0x001fe0c6` continued and called **`0x001fff25`** at `0x1FE19B`.
- Device aperture remained at **5,142 entries / 5,152 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FFF25`**, return `0x1FE1A0`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x001fff25` as an established **207-byte / 49-instruction** function with **no direct calls** (`recomp-post-memory-next-10/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000` for the 0x5000-byte request, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Prior milestone: MmClaimGpuInstanceMemory request

`0x001ffe27` read three explicit synthetic device values and stopped at ordinal 168 with a 0x5000-byte request and padding pointer. Module `recomp-post-memory-build-24` was 465,978 bytes, SHA-256 `67e7c9a5a560db00e8f3984e7561ed512555b5168f116dd6fb3ddcc8e390f72d`. Report `post-memory-browser-20261103-06.json`.

## Prior milestone: 0x001ffcc0

`0x001ffcc0` executed to completion and returned to `0x001fe0c6`, which called `0x1FFE27`. Translation `recomp-title-post-memory-11` was 347 functions / 13,113 instructions; module `recomp-post-memory-build-23` was 464,158 bytes, SHA-256 `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d`. Report `post-memory-browser-20261103-05.json`.

## Prior milestone: 0x001fe0c6

`0x001fe0c6` executed its prologue and two stores and called `0x1FFCC0`. Translation `recomp-title-post-memory-10` was 346 functions / 12,997 instructions; module `recomp-post-memory-build-22` was 459,451 bytes, SHA-256 `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f`. Report `post-memory-browser-20261103-03.json`.

## Prior milestone: 0x001fe085 and interrupt initialization

`0x001fe085` returned to `0x1FE5CE`; services 44/109/98/47 were modeled from the pinned public bridge without delivering interrupts. Translation `recomp-title-post-memory-09` was 345 functions / 12,892 instructions; module `recomp-post-memory-build-21` was 459,080 bytes, SHA-256 `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9`. Report `post-memory-browser-20261103-02.json`.

## Limits and next task

`0x1FFF25` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The GPU instance claim, device reads/writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FFF25` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
