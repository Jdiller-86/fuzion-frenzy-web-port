# Original execution enters 0x1FE1D0 and reaches 0x200340

November 3, 2026. The previously untranslated helper at `0x001fe1d0` was admitted from a retained read-only seed. It read/modified device register `0xFD00184C` and called untranslated `0x00200340`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-19` (local/ignored) |
| Functions / instructions | 355 / 13,401 |
| Admitted helper | `0x001fe1d0`, 315 body bytes / 87 instructions |
| Module | `research/verification/recomp-post-memory-build-32/startup.wasm` (local/ignored) |
| Module bytes | 473,896 |
| Module SHA-256 | `6ceba6b62364277056056fb6103ec723c7f26a8a5a53c0c2b3bcf0dbd30dc459` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-14.json` (local/ignored) |

The retained `next-14` and `next-15` function exports were combined into one private seed to stay within the 32-export admission limit. The generated header retains the source digest, the 355-function lookup and the new ordinal-305 kernel binding at `0x0025346c`. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output components.

The read-only export reports `0x001fe1d0` as a 315-byte/87-instruction established function. It calls `0x00200340`, `0x002010fd`, `0x0020107c`, kernel slot `0x002533b0` (ordinal 128) and kernel slot `0x0025346c` (ordinal 305, `RtlTimeToTimeFields`). This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The explicit synthetic profile now also supplies zero for device register `0xFD00184C`; this is a test input, not a console default. Earlier supplied entries remain.

Results:

- All **83/83** initialization callbacks returned.
- **245** distinct original routines executed (up from 244).
- **400** kernel operations completed; the new helper has not reached a kernel service.
- `0x001fe1d0` read/modified `0xFD00184C` and called untranslated `0x00200340`.
- Device aperture: **5,174 entries** and **5,208 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x200340`**, return `0x1FE210`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x00200340` as an established **18-byte / 6-instruction** function with **no direct calls** (`recomp-post-memory-next-16/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

The helper's remaining static path still contains `0x002010FD`, `0x0020107C`, ordinal 128 `KeQuerySystemTime` and ordinal 305 `RtlTimeToTimeFields`. Those are not yet reached and no result is supplied.

## Prior milestone: 0x20005A

`0x0020005a` programmed object/device fields and returned through `0x001fe0c6` to its caller, which reached `0x1FE1D0`. Translation `recomp-title-post-memory-18` was 354 functions / 13,314 instructions; module `recomp-post-memory-build-31` was 473,226 bytes, SHA-256 `ec7a4d9c39d7cf61413198cd8ab25b43a50a40caf7d79625362aa8bb7e92a4ca`. Report `post-memory-browser-20261103-13.json`.

## Prior milestone: 0x20002A

`0x0020002a` wrote `0xFD400780` and cleared `0xFD700000`/`0xFD700004`, returned to `0x001fe0c6`, which called `0x20005A`. Module `recomp-post-memory-build-30` was 471,504 bytes, SHA-256 `50985a0066a3055eb74dbc986679fa0b331701e59559653cb5a169a2e8f9516c`. Report `post-memory-browser-20261103-12.json`.

## Prior milestone: 0x1FFFF4

`0x001ffff4` programmed twelve device-register dwords at `0xFD008910`–`0xFD008944`, returned to `0x001fe0c6`, which set `[ESI+0x134]=2` and called `0x20002A`. Module `recomp-post-memory-build-29` was 471,053 bytes, SHA-256 `be6a6d13729484552f7a96ab7c6fc22584792670ed18f18c157652da1eff77b7`. Report `post-memory-browser-20261103-11.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Limits and next task

`0x200340` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The port-output log, device writes, per-byte device model, GPU instance claim and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x200340` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
