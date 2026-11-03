# Original execution completes 0x1FFFF4 and reaches 0x20002A

November 3, 2026. The previously untranslated helper at `0x001ffff4` was admitted from a retained read-only seed. It programmed twelve device-register dwords, returned to `0x001fe0c6`, and the caller continued to untranslated `0x0020002a`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-16` (local/ignored) |
| Functions / instructions | 352 / 13,280 |
| Admitted helper | `0x001ffff4`, 54 body bytes / 18 instructions |
| Module | `research/verification/recomp-post-memory-build-29/startup.wasm` (local/ignored) |
| Module bytes | 471,053 |
| Module SHA-256 | `be6a6d13729484552f7a96ab7c6fc22584792670ed18f18c157652da1eff77b7` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-11.json` (local/ignored) |

The selection reuses the retained 29-seed post-memory inputs plus `recomp-post-memory-next-12/seed.json`. The generated header retains the source digest and 352-function lookup. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance service.

The read-only export reports `0x001ffff4` as a 54-byte/18-instruction established function with no direct calls. It writes two 12-dword register groups based at `[ECX] + 0x8918`: `0x1000`, `0x100000`, zero and `0xFFFFFFFF` values at the observed offsets. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **242** distinct original routines executed (up from 241).
- **400** kernel operations completed; the new helper made no kernel service call.
- `0x001ffff4` programmed the twelve dword registers and returned to `0x001fe1b0`.
- `0x001fe0c6` continued, wrote `2` to `[ESI+0x134]`, and called **`0x0020002a`** at `0x1FE1BC`.
- Device aperture: **5,160 entries** and **5,188 events**. The new writes are at `0xFD008910`–`0xFD008944`, all with explicit values `0x1000`, `0x100000`, `0` or `0xFFFFFFFF`.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x20002A`**, return `0x1FE1C1`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x0020002a` as an established **48-byte / 15-instruction** function with **no direct calls** (`recomp-post-memory-next-13/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x001fdf83

`0x001fdf83` initialized a 0x300-byte guest-memory table and returned to `0x001fe0c6`, which called `0x1FFFF4`. Translation `recomp-title-post-memory-15` was 351 functions / 13,262 instructions; module `recomp-post-memory-build-28` was 470,384 bytes, SHA-256 `7882a2557f9dd38eead06643b913fc739c23f7f60d8b1855bef267ae4862e495`. Report `post-memory-browser-20261103-10.json`.

## Prior milestone: 0x001fff25

`0x001fff25` executed its byte/dword device-register sequence and returned to `0x001fe1a0`; `0x001fe0c6` called `0x1FDF83`. Module `recomp-post-memory-build-27` was 469,345 bytes, SHA-256 `5c3f84334ea08d5964e5bf8f2000e9d4aabe5c7c0cc61a32556d69c41f9103cf`. Report `post-memory-browser-20261103-09.json`.

## Prior milestone: 0x001fdf70

`0x001fdf70` added its stack argument to `[ECX+0x1A0]`, returned through `0x001ffe27` to `0x001fe0c6`, and reached `0x1FFF25`. Module `recomp-post-memory-build-26` was 467,100 bytes, SHA-256 `240b0bcd859b5c834b48059052a947b85e824c5633fefde93afd571db7ea0012`. Report `post-memory-browser-20261103-08.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Limits and next task

`0x20002A` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes, per-byte device model, GPU instance claim and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x20002A` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
