# Original execution completes 0x20107C and reaches 0x200143

November 3, 2026. The previously untranslated helper at `0x0020107c` was admitted. It programmed device fields, called untranslated `0x00200143`, and stopped there. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-24` (local/ignored) |
| Functions / instructions | 360 / 13,682 |
| Admitted helper | `0x0020107c`, 129 body bytes / 33 instructions |
| Module | `research/verification/recomp-post-memory-build-37/startup.wasm` (local/ignored) |
| Module bytes | 485,351 |
| Module SHA-256 | `e64070431da915dc10e9e6e9c7fb37aebd58ee88bce12620c5e545c0229886de` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-20.json` (local/ignored) |

The retained `next-14`–`next-20` functions are combined into one private seed to stay within the 32-export admission limit. The generated header retains the source digest, the 360-function lookup and the ordinal-305 kernel binding at `0x0025346c`. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output/TIME_FIELDS/zero-range components.

## Observed browser execution

The same explicit synthetic profile as the preceding run was used. No new supplied device-register value was required.

Results:

- All **83/83** initialization callbacks returned.
- **250** distinct original routines executed (up from 249).
- **402** kernel operations completed.
- `0x0020107c` programmed `0xFD002044`, `0xFD003224`, `0xFD002040`, `0xFD002500`, `0xFD003000`, `0xFD003050`, `0xFD003200`, `0xFD003250`, `0xFD003220`, `0xFD003210` and `0xFD003270`, then called **`0x00200143`**.
- Device aperture: **5,286 entries** and **5,395 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x200143`**, return `0x2010D8`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x00200143` as an established **509-byte / 132-instruction** function with **no direct calls** (`recomp-post-memory-next-21/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x200F59

`0x00200f59` read `0xFD400100` = `0`, skipped `0x1FDAC0`, called `0x2000CB` twice, and returned; the caller reached `0x20107C`. Translation `recomp-title-post-memory-23` was 359 functions / 13,649 instructions; module `recomp-post-memory-build-36` was 484,657 bytes, SHA-256 `91afe5cb2fd8a6a79196e96b2b30b9db4f1a72c7c4bed64dfca569e9fee03d66`. Report `post-memory-browser-20261103-19.json`.

## Prior milestone: zero-range device table

The device-table reads in `0x2010FD` were served through an explicit 0x1000-byte zero range at `0xFD100200`, and `0x2010FD` called `0x200F59`. Module `recomp-post-memory-build-35` was rebuilt with zero-range support and was 481,870 bytes, SHA-256 `bc77a4cede489adcafe427b099f0b17d9bcd85fe2c14c3b282ccd916a6218f75`. Report `post-memory-browser-20261103-18.json`.

## Prior milestone: 0x2000CB device boundary

`0x002000cb` read `0xFD400700` = `0` and returned; `0x002010fd` stopped at unsupplied `0xFD100284`. Module `recomp-post-memory-build-35` initially used individual entries and was 481,279 bytes, SHA-256 `3dc7bfd70e4cdffa43ee681d851339a74ab951658819d30df45cf7901173676f`. Report `post-memory-browser-20261103-17.json`.

## Prior milestone: 0x200340 and system time

`0x00200340` wrote `1` to `0xFD600100`/`0xFD600140`; `KeQuerySystemTime` and the source-owned `RtlTimeToTimeFields` returned `2026-11-03 16:02:49.719`; `0x001fe1d0` reached `0x2010FD`. Module `recomp-post-memory-build-33` was 476,718 bytes, SHA-256 `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670`. Report `post-memory-browser-20261103-15.json`.

## Limits and next task

`0x200143` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes, zero ranges, time conversion, port-output log and GPU instance claim are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x200143` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
