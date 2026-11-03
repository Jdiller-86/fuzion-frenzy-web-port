# Original execution serves the device table through a bounded zero range and reaches 0x200F59

November 3, 2026. The device-table reads in `0x002010fd` now resolve through an explicit bounded zero range instead of exhausting individual profile entries. The run completes `0x002000cb`, continues through the table and calls untranslated `0x00200f59`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-22` (local/ignored) |
| Functions / instructions | 358 / 13,571 |
| Module | `research/verification/recomp-post-memory-build-35/startup.wasm` (local/ignored) |
| Module bytes | 481,870 |
| Module SHA-256 | `bc77a4cede489adcafe427b099f0b17d9bcd85fe2c14c3b282ccd916a6218f75` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-18.json` (local/ignored) |

The translation and seed selection are unchanged from the preceding run. The module adds the source-owned zero-range device model and the `ff_probe_mmio_zero` export. It retains ABI 24, the sole `env.ff_host_clock` import and the GPU instance/port-output/TIME_FIELDS components.

## Bounded zero-range device model

`src/runtime/device_registers.c` now accepts up to four explicit zero ranges:

- `ff_device_registers_add_zero_range(base, bytes)` validates an aligned, in-window, bounded range before mutation.
- Reads fully inside a zero range return zero and record an event with `beforeKnown = 1`, without creating an entry per address.
- A later write creates a normal entry, which takes precedence over the zero range.
- Reads outside the range and outside supplied entries still fault.
- `parseLocalMmioProfile` accepts an optional `zeroRanges` array (at most four), validates its alignment/window bounds/duplicates, and preserves it in the report's `supplied` field.
- `ff_probe_mmio_zero` seeds the ranges in the browser worker.

`tests/device_registers_test.c` covers zero-range dword/byte reads, event provenance, write override, out-of-range faults and invalid/duplicate ranges. `tests/mmio-profile.test.mjs` covers preservation and malformed ranges. Optimized and ASan/UBSan runs pass.

## Observed browser execution

The explicit synthetic profile supplies a zero range at `0xFD100200` for 0x1000 bytes and keeps the earlier explicit entries. These are test inputs, not console defaults.

Results:

- All **83/83** initialization callbacks returned.
- **248** distinct original routines executed.
- **402** kernel operations completed.
- `0x002000cb` read `0xFD400700` = `0` and returned without its optional calls.
- `0x002010fd` completed its device-table reads through the zero range and called **`0x00200f59`**.
- Device aperture: **5,275 entries** and **5,377 events**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x200F59`**, return `0x201389`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x00200f59` as an established **291-byte / 78-instruction** function (`recomp-post-memory-next-19/seed.json`, local/ignored). It calls `0x001fdac0`, `0x002000cb` (twice) and `0x0020010a`. This is static export evidence, not admitted translation or observed execution.

## Prior milestone: 0x2000CB

`0x002000cb` read `0xFD400700` = `0` and returned; `0x002010fd` stopped at unsupplied `0xFD100284`. Module `recomp-post-memory-build-35` initially used the individual-entry model and was 481,279 bytes, SHA-256 `3dc7bfd70e4cdffa43ee681d851339a74ab951658819d30df45cf7901173676f`. Report `post-memory-browser-20261103-17.json`.

## Prior milestone: 0x2010FD

`0x002010fd` programmed its device-register block and called `0x2000CB`. Module `recomp-post-memory-build-34` was 478,501 bytes, SHA-256 `2de725959fb619a572328bf89ef0374952bd8ef1233e81b67bb5dfae7b38a40d`. Report `post-memory-browser-20261103-16.json`.

## Prior milestone: 0x200340 and system time

`0x00200340` wrote `1` to `0xFD600100`/`0xFD600140`; `KeQuerySystemTime` and the source-owned `RtlTimeToTimeFields` returned `2026-11-03 16:02:49.719`; `0x001fe1d0` reached `0x2010FD`. Module `recomp-post-memory-build-33` was 476,718 bytes, SHA-256 `b1c8c1867c3caaf62007557d91a5e08db5c282567ba2d2f70b64b664a9256670`. Report `post-memory-browser-20261103-15.json`.

## Limits and next task

`0x200F59` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The zero range, time conversion, port-output log and GPU instance claim are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x200F59` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
