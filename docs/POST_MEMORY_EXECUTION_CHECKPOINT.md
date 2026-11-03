# Original execution enters 0x2010FD and reaches 0x2000CB

November 3, 2026. The previously untranslated helper at `0x002010fd` was admitted from a retained read-only seed. It programmed a block of device registers and called untranslated `0x002000cb`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-21` (local/ignored) |
| Functions / instructions | 357 / 13,548 |
| Admitted helper | `0x002010fd`, 659 body bytes / 141 instructions |
| Module | `research/verification/recomp-post-memory-build-34/startup.wasm` (local/ignored) |
| Module bytes | 478,501 |
| Module SHA-256 | `2de725959fb619a572328bf89ef0374952bd8ef1233e81b67bb5dfae7b38a40d` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-16.json` (local/ignored) |

The retained `next-14`–`next-17` functions are combined into one private seed to stay within the 32-export admission limit. The generated header retains the source digest, the 357-function lookup and the ordinal-305 kernel binding at `0x0025346c`. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output/TIME_FIELDS components.

The read-only export reports `0x002010fd` as a 659-byte/141-instruction established function. It calls `0x002000cb` and `0x00200f59`. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **247** distinct original routines executed (up from 246).
- **402** kernel operations completed; the new helper has not reached a new kernel service.
- `0x002010fd` read `0xFD000200`/`0xFD001218`, programmed its device-register block and called `0x002000cb`.
- The preceding path still completed ordinal 128 and ordinal 305; the time fields in this run were `[2026, 11, 3, 16, 5, 50, 669, 2]`.
- Device aperture: **5,189 entries** and **5,230 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x2000CB`**, return `0x201235`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x002000cb` as an established **63-byte / 23-instruction** function (`recomp-post-memory-next-18/seed.json`, local/ignored). It calls `0x001fdac0` and `0x001fd700`. This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x200340 and system time

`0x00200340` wrote `1` to `0xFD600100`/`0xFD600140`; `KeQuerySystemTime` and the source-owned `RtlTimeToTimeFields` returned `2026-11-03 16:02:49.719`; `0x001fe1d0` reached `0x2010FD`. Translation `recomp-title-post-memory-20` was 356 functions / 13,407 instructions; module `recomp-post-memory-build-33` was 476,718 bytes, SHA-256 `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670`. Report `post-memory-browser-20261103-15.json`.

## Prior milestone: 0x1FE1D0

`0x001fe1d0` read/modified `0xFD00184C` and called `0x200340`. Module `recomp-post-memory-build-32` was 473,896 bytes, SHA-256 `6ceba6b62364277056056fb6103ec723c7f26a8a5a53c0c2b3bcf0dbd30dc459`. Report `post-memory-browser-20261103-14.json`.

## Prior milestone: 0x20005A

`0x0020005a` programmed object/device fields and returned through `0x001fe0c6` to its caller, which reached `0x1FE1D0`. Module `recomp-post-memory-build-31` was 473,226 bytes, SHA-256 `ec7a4d9c39d7cf61413198cd8ab25b43a50a40caf7d79625362aa8bb7e92a4ca`. Report `post-memory-browser-20261103-13.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Limits and next task

`0x2000CB` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The time conversion, port-output log, device writes, per-byte device model and GPU instance claim are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x2000CB` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
