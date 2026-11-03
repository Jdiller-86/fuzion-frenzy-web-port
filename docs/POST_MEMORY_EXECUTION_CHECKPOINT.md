# Original execution completes 0x20005A and reaches 0x1FE1D0

November 3, 2026. The previously untranslated helper at `0x0020005a` was admitted from a retained read-only seed. It programmed object and device-register fields, returned through `0x001fe0c6` to its caller, and execution continued to untranslated `0x001fe1d0`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-18` (local/ignored) |
| Functions / instructions | 354 / 13,314 |
| Admitted helper | `0x0020005a`, 113 body bytes / 19 instructions |
| Module | `research/verification/recomp-post-memory-build-31/startup.wasm` (local/ignored) |
| Module bytes | 473,226 |
| Module SHA-256 | `ec7a4d9c39d7cf61413198cd8ab25b43a50a40caf7d79625362aa8bb7e92a4ca` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-13.json` (local/ignored) |

The selection reuses the retained 31-seed post-memory inputs plus `recomp-post-memory-next-14/seed.json`. The generated header retains the source digest and 354-function lookup. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output components.

The read-only export reports `0x0020005a` as a 113-byte/19-instruction established function with no direct calls. It writes object fields `0x120`, `0x124` and `0x11C`, then writes device dwords at `0xFD003210`, `0xFD003270`, `0xFD003240`, `0xFD003244`, `0xFD003058`, `0xFD003258`, `0xFD002504`, `0xFD002508`, `0xFD00250C`, `0xFD003228`, `0xFD002410` and `0xFD002420`. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **244** distinct original routines executed (up from 243).
- **400** kernel operations completed; the new helper made no kernel service call.
- `0x0020005a` programmed its object/device fields and returned to `0x001fe1c8`.
- `0x001fe0c6` returned to its caller at `0x1FE616`; execution continued and reached **`0x001fe1d0`**.
- Device aperture: **5,173 entries** and **5,203 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FE1D0`**, return `0x1FE6ED`, ordinal 0.

## Source-owned port-output log

The translated renderer-init path already contained an `OUT DX, AL` site, but the module link failed on the undefined `xbox_PortOut` symbol. The new source-owned `port_io.c`/`.h` records a bounded 1/2/4-byte port-output event and ignores the write. It does not model a port device, input value, timing or hardware effect. `tests/port_io_test.c` covers masking, width rejection, ordered records and capacity. Optimized and ASan/UBSan runs pass.

The port component is a link/build prerequisite for the reached translation; no port write is claimed as observed original behavior in this run.

## Next dependency

A bounded read-only export reports `0x001fe1d0` as an established **315-byte / 87-instruction** function (`recomp-post-memory-next-15/seed.json`, local/ignored). It calls original `0x00200340`, `0x002010fd` and `0x0020107c`, plus kernel slots `0x002533b0` (ordinal 128) and `0x0025346c` (not yet in the current binding list). This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x20002A

`0x0020002a` wrote `0xFD400780` and cleared `0xFD700000`/`0xFD700004`, returned to `0x001fe0c6`, which called `0x20005A`. Translation `recomp-title-post-memory-17` was 353 functions / 13,295 instructions; module `recomp-post-memory-build-30` was 471,504 bytes, SHA-256 `50985a0066a3055eb74dbc986679fa0b331701e59559653cb5a169a2e8f9516c`. Report `post-memory-browser-20261103-12.json`.

## Prior milestone: 0x1FFFF4

`0x001ffff4` programmed twelve device-register dwords at `0xFD008910`–`0xFD008944`, returned to `0x001fe0c6`, which set `[ESI+0x134]=2` and called `0x20002A`. Module `recomp-post-memory-build-29` was 471,053 bytes, SHA-256 `be6a6d13729484552f7a96ab7c6fc22584792670ed18f18c157652da1eff77b7`. Report `post-memory-browser-20261103-11.json`.

## Prior milestone: 0x1FDF83

`0x001fdf83` initialized a 0x300-byte guest-memory table and returned to `0x001fe0c6`, which called `0x1FFFF4`. Module `recomp-post-memory-build-28` was 470,384 bytes, SHA-256 `7882a2557f9dd38eead06643b913fc739c23f7f60d8b1855bef267ae4862e495`. Report `post-memory-browser-20261103-10.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Limits and next task

`0x1FE1D0` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The port-output log, device writes, per-byte device model, GPU instance claim and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FE1D0` from the retained read-only export, resolving its new kernel-slot binding, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
