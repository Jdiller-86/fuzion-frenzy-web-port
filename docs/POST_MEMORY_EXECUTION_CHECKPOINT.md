# Original execution completes 0x1FDF83 and reaches 0x1FFFF4

November 3, 2026. The previously untranslated helper at `0x001fdf83` was admitted from a retained read-only seed. It initialized a 0x300-byte guest-memory table, returned to `0x001fe0c6`, and the caller continued to untranslated `0x001ffff4`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-15` (local/ignored) |
| Functions / instructions | 351 / 13,262 |
| Admitted helper | `0x001fdf83`, 64 body bytes / 22 instructions |
| Module | `research/verification/recomp-post-memory-build-28/startup.wasm` (local/ignored) |
| Module bytes | 470,384 |
| Module SHA-256 | `7882a2557f9dd38eead06643b913fc739c23f7f60d8b1855bef267ae4862e495` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-10.json` (local/ignored) |

The selection reuses the retained 28-seed post-memory inputs plus `recomp-post-memory-next-11/seed.json`. The generated header retains the source digest and 351-function lookup. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance service.

The read-only export reports `0x001fdf83` as a 64-byte/22-instruction established function with no direct calls. It computes an object-relative base (`ECX + argument * 0x300 + 0x214`) and writes three 0x100-byte tables with byte values 0–255. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **241** distinct original routines executed (up from 240).
- **400** kernel operations completed; the new helper made no kernel service call.
- `0x001fdf83` executed its guest-memory table initialization and returned to `0x001fe1a9`.
- `0x001fe0c6` continued and called **`0x001ffff4`** at `0x1FE1AB`.
- Device aperture remained at **5,148 entries / 5,176 events**; the 0x300-byte table writes went to guest RAM, not the aperture.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FFFF4`**, return `0x1FE1B0`, ordinal 0.

## Source-owned per-byte device knownness

The aperture now tracks a four-bit `known_bytes` mask per dword register. Sub-dword writes may create or extend an unknown dword entry; each write marks only the bytes it stores. Reads require every accessed byte to be known, so untouched bytes still fault rather than returning a fabricated value. Dword writes and seeded profile registers mark all four bytes known.

`tests/device_registers_test.c` covers extending an unknown register one byte at a time, rejecting a read of an unwritten byte, completing the dword, and then reading the assembled value. Optimized and ASan/UBSan runs pass.

## Next dependency

A bounded read-only export reports `0x001ffff4` as an established **54-byte / 18-instruction** function with **no direct calls** (`recomp-post-memory-next-12/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x001fff25

`0x001fff25` executed its byte/dword device-register sequence and returned to `0x001fe1a0`; `0x001fe0c6` called `0x1FDF83`. Translation `recomp-title-post-memory-14` was 350 functions / 13,240 instructions; module `recomp-post-memory-build-27` was 469,345 bytes, SHA-256 `5c3f84334ea08d5964e5bf8f2000e9d4aabe5c7c0cc61a32556d69c41f9103cf`. Report `post-memory-browser-20261103-09.json`.

## Prior milestone: 0x001fdf70

`0x001fdf70` added its stack argument to `[ECX+0x1A0]`, returned through `0x001ffe27` to `0x001fe0c6`, and reached `0x1FFF25`. Module `recomp-post-memory-build-26` was 467,100 bytes, SHA-256 `240b0bcd859b5c834b48059052a947b85e824c5633fefde93afd571db7ea0012`. Report `post-memory-browser-20261103-08.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Prior milestone: 0x001ffcc0

`0x001ffcc0` executed to completion and returned to `0x001fe0c6`, which called `0x1FFE27`. Module `recomp-post-memory-build-23` was 464,158 bytes, SHA-256 `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d`. Report `post-memory-browser-20261103-05.json`.

## Limits and next task

`0x1FFFF4` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The per-byte device model, GPU instance claim, device reads/writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FFFF4` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
