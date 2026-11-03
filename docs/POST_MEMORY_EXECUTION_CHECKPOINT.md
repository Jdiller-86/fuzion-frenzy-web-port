# Original execution completes 0x2000CB and stops at device register 0xFD100284

November 3, 2026. The previously untranslated helper at `0x002000cb` was admitted from a retained read-only seed. It read `0xFD400700` as zero and returned without taking its optional calls. The caller `0x002010fd` continued through its device-register table and stopped at the next unsupplied register, `0xFD100284`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-22` (local/ignored) |
| Functions / instructions | 358 / 13,571 |
| Admitted helper | `0x002000cb`, 63 body bytes / 23 instructions |
| Module | `research/verification/recomp-post-memory-build-35/startup.wasm` (local/ignored) |
| Module bytes | 481,279 |
| Module SHA-256 | `3dc7bfd70e4cdffa43ee681d851339a74ab951658819d30df45cf7901173676f` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-17.json` (local/ignored) |

The retained `next-14`–`next-18` functions are combined into one private seed to stay within the 32-export admission limit. The generated header retains the source digest, the 358-function lookup and the ordinal-305 kernel binding at `0x0025346c`. The module retains ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output/TIME_FIELDS components.

The read-only export reports `0x002000cb` as a 63-byte/23-instruction established function. It reads `0xFD400700`; if zero it returns, otherwise it conditionally calls `0x001fdac0` and `0x001fd700`. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The explicit synthetic profile supplies zero for `0xFD400700` and zero-filled device registers covering `0xFD100200`–`0xFD10027C`. These are test inputs, not console defaults. Earlier supplied entries remain.

Results:

- All **83/83** initialization callbacks returned.
- **248** distinct original routines executed (up from 247).
- **402** kernel operations completed.
- `0x002000cb` read `0xFD400700` = `0` and returned without calling `0x1FDAC0`/`0x1FD700`.
- `0x002010fd` continued through its device-register table and reached the next unsupplied register, `0xFD100284`.
- Device aperture: **5,232 entries** and **5,279 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 10, guest device read at **`0xFD100284`**, ordinal 1.

## Next boundary

The next access is another device-register read in `0x002010fd`, not a new original routine. `0x2010FD` still has `0x00200f59` on its static path, and `0x2000CB` still has `0x001fdac0`/`0x001fd700` on its non-zero branch. Those are not reached.

## Prior milestone: 0x2010FD

`0x002010fd` programmed its device-register block and called `0x2000CB`. Translation `recomp-title-post-memory-21` was 357 functions / 13,548 instructions; module `recomp-post-memory-build-34` was 478,501 bytes, SHA-256 `2de725959fb619a572328bf89ef0374952bd8ef1233e81b67bb5dfae7b38a40d`. Report `post-memory-browser-20261103-16.json`.

## Prior milestone: 0x200340 and system time

`0x00200340` wrote `1` to `0xFD600100`/`0xFD600140`; `KeQuerySystemTime` and the source-owned `RtlTimeToTimeFields` returned `2026-11-03 16:02:49.719`; `0x001fe1d0` reached `0x2010FD`. Module `recomp-post-memory-build-33` was 476,718 bytes, SHA-256 `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670`. Report `post-memory-browser-20261103-15.json`.

## Prior milestone: 0x1FE1D0

`0x001fe1d0` read/modified `0xFD00184C` and called `0x200340`. Module `recomp-post-memory-build-32` was 473,896 bytes, SHA-256 `6ceba6b62364277056056fb6103ec723c7f26a8a5a53c0c2b3bcf0dbd30dc459`. Report `post-memory-browser-20261103-14.json`.

## Prior milestone: 0x20005A

`0x0020005a` programmed object/device fields and returned through `0x001fe0c6` to its caller, which reached `0x1FE1D0`. Module `recomp-post-memory-build-31` was 473,226 bytes, SHA-256 `ec7a4d9c39d7cf61413198cd8ab25b43a50a40caf7d79625362aa8bb7e92a4ca`. Report `post-memory-browser-20261103-13.json`.

## Limits and next task

`0xFD100284` is an unsupplied synthetic device-register input, not a new original routine. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device table, time conversion, port-output log and GPU instance claim are synthetic test scaffolding, not recovered console state. Next: decide a bounded explicit device-table input model for the `0xFD100200` region (or extend the current zero block), continue `0x2010FD`, and then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
