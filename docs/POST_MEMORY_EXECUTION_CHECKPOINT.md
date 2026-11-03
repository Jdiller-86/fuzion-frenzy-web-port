# Original execution completes 0x200340, converts system time, and reaches 0x2010FD

November 3, 2026. The previously untranslated helper at `0x00200340` was admitted. The browser worker completed its device-register writes, `KeQuerySystemTime` returned a system time, the new source-owned `RtlTimeToTimeFields` conversion returned `2026-11-03 16:02:49.719`, and `0x001fe1d0` continued to untranslated `0x002010fd`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-20` (local/ignored) |
| Functions / instructions | 356 / 13,407 |
| Admitted helper | `0x00200340`, 18 body bytes / 6 instructions |
| Module | `research/verification/recomp-post-memory-build-33/startup.wasm` (local/ignored) |
| Module bytes | 476,718 |
| Module SHA-256 | `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-15.json` (local/ignored) |

The retained `next-14`, `next-15` and `next-16` functions are combined into one private seed to stay within the 32-export admission limit. The generated header retains the source digest, the 356-function lookup and the ordinal-305 kernel binding at `0x0025346c`. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output components.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **246** distinct original routines executed (up from 245).
- **402** kernel operations completed.
- `0x00200340` wrote `1` to `0xFD600100` and `0xFD600140`, then returned to `0x001fe210`.
- Ordinal 128 `KeQuerySystemTime` returned `134381953697190000` (100 ns units).
- Ordinal 305 `RtlTimeToTimeFields` returned `[2026, 11, 3, 16, 2, 49, 719, 2]`.
- `0x001fe1d0` continued and called **`0x002010fd`** at `0x1FE2CD`.
- Device aperture: **5,178 entries** and **5,213 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x2010FD`**, return `0x1FE2D2`, ordinal 0.

## Source-owned TIME_FIELDS conversion

`src/runtime/time_fields.c`/`.h` implements a deterministic conversion from a 100 ns interval since 1601-01-01 to the eight-field Xbox `TIME_FIELDS` layout:

- year, month, day, hour, minute, second, milliseconds and weekday;
- no host timezone, locale or wall-clock dependency;
- supports years 1601–32767 and rejects out-of-range inputs;
- rejects a null output.

`tests/time_fields_test.c` covers the 1601 and 1970 epochs, 2000-01-01 12:34:56.789, the 2024 leap day, the non-leap 1900 boundary, out-of-range input and null rejection. Optimized and ASan/UBSan runs pass.

## Next dependency

A bounded read-only export reports `0x002010fd` as an established **659-byte / 141-instruction** function (`recomp-post-memory-next-17/seed.json`, local/ignored). It calls original `0x002000cb` and `0x00200f59`. This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x1FE1D0

`0x001fe1d0` read/modified `0xFD00184C` and called `0x200340`. Translation `recomp-title-post-memory-19` was 355 functions / 13,401 instructions; module `recomp-post-memory-build-32` was 473,896 bytes, SHA-256 `6ceba6b62364277056056fb6103ec723c7f26a8a5a53c0c2b3bcf0dbd30dc459`. Report `post-memory-browser-20261103-14.json`.

## Prior milestone: 0x20005A

`0x0020005a` programmed object/device fields and returned through `0x001fe0c6` to its caller, which reached `0x1FE1D0`. Module `recomp-post-memory-build-31` was 473,226 bytes, SHA-256 `ec7a4d9c39d7cf61413198cd8ab25b43a50a40caf7d79625362aa8bb7e92a4ca`. Report `post-memory-browser-20261103-13.json`.

## Prior milestone: 0x20002A

`0x0020002a` wrote `0xFD400780` and cleared `0xFD700000`/`0xFD700004`, returned to `0x001fe0c6`, which called `0x20005A`. Module `recomp-post-memory-build-30` was 471,504 bytes, SHA-256 `50985a0066a3055eb74dbc986679fa0b331701e59559653cb5a169a2e8f9516c`. Report `post-memory-browser-20261103-12.json`.

## Prior milestone: GPU instance claim and 0x5000-byte clear

Ordinal 168 `MmClaimGpuInstanceMemory` returned a bounded source-owned base `0x03B78000`, the original 5,120 dword stores cleared `0xFD700000`–`0xFD704FFC`, and `0x001ffe27` reached `0x1FDF70`. Module `recomp-post-memory-build-25` was 466,517 bytes, SHA-256 `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956`. Report `post-memory-browser-20261103-07.json`.

## Limits and next task

`0x2010FD` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The TIME_FIELDS result reflects the synthetic browser clock, not an original console time or timezone. The port-output log, device writes, per-byte device model, GPU instance claim and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x2010FD` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
