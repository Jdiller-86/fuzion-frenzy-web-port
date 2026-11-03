# Original execution completes 0x1FFF25 and reaches 0x1FDF83

November 3, 2026. The previously untranslated helper at `0x001fff25` was admitted from a retained read-only seed. It executed a sequence of sub-dword device-register accesses, returned to `0x001fe0c6`, and the caller continued to untranslated `0x001fdf83`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-14` (local/ignored) |
| Functions / instructions | 350 / 13,240 |
| Admitted helper | `0x001fff25`, 207 body bytes / 49 instructions |
| Module | `research/verification/recomp-post-memory-build-27/startup.wasm` (local/ignored) |
| Module bytes | 469,345 |
| Module SHA-256 | `5c3f84334ea08d5964e5bf8f2000e9d4aabe5c7c0cc61a32556d69c41f9103cf` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-09.json` (local/ignored) |

The selection reuses the retained 27-seed post-memory inputs plus `recomp-post-memory-next-10/seed.json`. The generated header retains the source digest and 350-function lookup. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance service.

## Observed browser execution

The explicit synthetic profile supplies zero for the dword registers `0xFD6013D4`, `0xFD008088`, `0xFD00808C` and `0xFD600804`; the byte read at `0xFD6013D5` maps into the `0xFD6013D4` dword. These are test inputs, not console defaults. Earlier supplied entries remain.

Results:

- All **83/83** initialization callbacks returned.
- **240** distinct original routines executed (up from 239).
- **400** kernel operations completed; the new helper made no kernel service call.
- `0x001fff25` executed through its device-register read/write sequence and returned to `0x001fe1a0`.
- `0x001fe0c6` continued and called **`0x001fdf83`** at `0x1FE1A4`.
- Device aperture: **5,148 entries** and **5,176 events**. The new byte events include `0xFD6013D4`/`0xFD6013D5`; the dword read-modify-writes target `0xFD008088`, `0xFD00808C` and `0xFD600804`.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FDF83`**, return `0x1FE1A9`, ordinal 0.

## Source-owned sub-dword device writes

`0x001fff25` writes individual bytes (`MOV byte ptr [EAX+0x6013D4], ...`) and performs dword read-modify-writes. The aperture previously supported sub-dword reads and failed closed on sub-dword writes.

`ff_device_registers_access_width` now:

- stages 1/2-byte writes into the containing dword entry and returns the offset pointer used by the generated store;
- commits by merging the written byte/halfword into the known dword value while preserving the other bytes;
- records the written sub-dword value with its address, site and `beforeKnown` provenance;
- requires the containing dword to be known, so unknown registers still fail closed;
- supports sub-dword read-modify-write by recording the read event and seeding the staged write with the current value.

`tests/device_registers_test.c` covers byte and halfword writes, value merging, event values, unknown-containing-register rejection and the existing read path. Optimized and ASan/UBSan runs pass.

## Next dependency

A bounded read-only export reports `0x001fdf83` as an established **64-byte / 22-instruction** function with **no direct calls** (`recomp-post-memory-next-11/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x001fdf70

`0x001fdf70` added its stack argument to `[ECX+0x1A0]`, returned through `0x001ffe27` to `0x001fe0c6`, and reached `0x1FFF25`. Translation `recomp-title-post-memory-13` was 349 functions / 13,191 instructions; module `recomp-post-memory-build-26` was 467,100 bytes, SHA-256 `240b0bcd859b5c834b48059052a947b85e824c5633fefde93afd571db7ea0012`. Report `post-memory-browser-20261103-08.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Prior milestone: 0x001ffcc0

`0x001ffcc0` executed to completion and returned to `0x001fe0c6`, which called `0x1FFE27`. Translation `recomp-title-post-memory-11` was 347 functions / 13,113 instructions; module `recomp-post-memory-build-23` was 464,158 bytes, SHA-256 `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d`. Report `post-memory-browser-20261103-05.json`.

## Prior milestone: 0x001fe0c6

`0x001fe0c6` executed its prologue and two stores and called `0x1FFCC0`. Translation `recomp-title-post-memory-10` was 346 functions / 12,997 instructions; module `recomp-post-memory-build-22` was 459,451 bytes, SHA-256 `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f`. Report `post-memory-browser-20261103-03.json`.

## Limits and next task

`0x1FDF83` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The sub-dword writes, GPU instance claim, device reads/writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FDF83` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
