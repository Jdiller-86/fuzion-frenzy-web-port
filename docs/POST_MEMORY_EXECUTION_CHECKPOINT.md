# Original execution completes 0x200F59 and reaches 0x20107C

November 3, 2026. The previously untranslated helper at `0x00200f59` was admitted. Its conditional first call was skipped by the explicit zero device state, it called `0x002000cb` twice, returned, and the caller continued to untranslated `0x0020107c`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-23` (local/ignored) |
| Functions / instructions | 359 / 13,649 |
| Admitted helper | `0x00200f59`, 291 body bytes / 78 instructions |
| Module | `research/verification/recomp-post-memory-build-36/startup.wasm` (local/ignored) |
| Module bytes | 484,657 |
| Module SHA-256 | `91afe5cb2fd8a6a79196e96b2b30b9db4f1a72c7c4bed64dfca569e9fee03d66` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-19.json` (local/ignored) |

The retained `next-14`–`next-19` functions are combined into one private seed to stay within the 32-export admission limit. The generated header retains the source digest, the 359-function lookup and the ordinal-305 kernel binding at `0x0025346c`. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output/TIME_FIELDS/zero-range components.

## Observed browser execution

The explicit synthetic profile supplies a second zero range at `0xFD400000` for 0x1000 bytes, in addition to the `0xFD100200` table range. These are test inputs, not console defaults.

Results:

- All **83/83** initialization callbacks returned.
- **249** distinct original routines executed (up from 248).
- **402** kernel operations completed.
- `0x00200f59` read `0xFD400100` = `0` and skipped its first conditional call, `0x001fdac0`.
- It called `0x002000cb` twice; each call read `0xFD400700` = `0` and returned without its optional calls.
- The `0x0020010a` branch was not taken in this run.
- The helper returned; the caller continued to `0x0020107c`.
- Device aperture: **5,277 entries** and **5,386 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x20107C`**, return `0x1FE2ED`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x0020107c` as an established **129-byte / 33-instruction** function (`recomp-post-memory-next-20/seed.json`, local/ignored). It calls `0x00200143`. This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x2000CB and zero-range table

The device-table reads in `0x2010FD` were served through an explicit 0x1000-byte zero range at `0xFD100200`, and `0x2010FD` called `0x200F59`. Translation `recomp-title-post-memory-22` was 358 functions / 13,571 instructions; module `recomp-post-memory-build-35` was 481,870 bytes, SHA-256 `bc77a4cede489adcafe427b099f0b17d9bcd85fe2c14c3b282ccd916a6218f75`. Report `post-memory-browser-20261103-18.json`.

## Prior milestone: 0x2000CB device boundary

`0x002000cb` read `0xFD400700` = `0` and returned; `0x002010fd` stopped at unsupplied `0xFD100284`. Module `recomp-post-memory-build-35` initially used individual entries and was 481,279 bytes, SHA-256 `3dc7bfd70e4cdffa43ee681d851339a74ab951658819d30df45cf7901173676f`. Report `post-memory-browser-20261103-17.json`.

## Prior milestone: 0x2010FD

`0x002010fd` programmed its device-register block and called `0x2000CB`. Module `recomp-post-memory-build-34` was 478,501 bytes, SHA-256 `2de725959fb619a572328bf89ef0374952bd8ef1233e81b67bb5dfae7b38a40d`. Report `post-memory-browser-20261103-16.json`.

## Prior milestone: 0x200340 and system time

`0x00200340` wrote `1` to `0xFD600100`/`0xFD600140`; `KeQuerySystemTime` and the source-owned `RtlTimeToTimeFields` returned `2026-11-03 16:02:49.719`; `0x001fe1d0` reached `0x2010FD`. Module `recomp-post-memory-build-33` was 476,718 bytes, SHA-256 `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670`. Report `post-memory-browser-20261103-15.json`.

## Limits and next task

`0x20107C` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The zero ranges, time conversion, port-output log and GPU instance claim are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x20107C` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
