# Original execution completes 0x20057A and reaches 0x200518

November 3, 2026. The large device-initialization function at `0x0020057A` now executes and the next call requests untranslated `0x00200518`. Its 3,344 device writes reused the existing `0xFD700000` entries, so the 8,192-entry aperture was not exhausted. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-29` (local/ignored) |
| Functions / instructions | 365 / 14,544 |
| Admitted helper | `0x0020057A`, 2,527 body bytes / 568 instructions, no calls |
| Module | `research/verification/recomp-post-memory-build-43/startup.wasm` (local/ignored) |
| Module bytes | 520,818 |
| Module SHA-256 | `2752487f5a25ed6d5387d0a3e9d899003ede883321f997f43a1e8ae0f1b975b6` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-26.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-19.json` (local/ignored) |

The retained `next-14`–`next-25` functions are combined in one private seed to stay within the 32-export admission limit. ABI 24, the sole `env.ff_host_clock` import, the 4,096-event trace ceiling, the 8,192-entry / 32,768-event device aperture and the source-owned GPU instance/port-output/TIME_FIELDS components are unchanged. The profile keeps its 64 explicit registers and the three zero ranges at `0xFD100200`/0x1000, `0xFD400000`/0x1000 and `0xFD003200`/0x100.

## Observed browser execution

- All **83/83** initialization callbacks returned.
- **259** distinct original routines executed and **402** kernel operations completed.
- `0x0020057A` programmed large `0xFDxxxxxx` register/context blocks with immediate constants (for example `0x10700FF9`, `0x0436086C`, `0xC001B`, `0x7FF0000`, `0x3F800000`, `0x40000000`, `0x44400000`, `0x4B7FFFFF`, `0x30303`, `0x80000`, `0x1012000`, `0x105B8`, `0x80008` and `0x1C527C`) and counted loops, then returned.
- The helper contributed **3,344** ordered device writes; the aperture retained **5,300 entries** and **12,496 events** because every write reused the existing `0xFD700000`–`0xFD704FFC` region. No capacity limit was reached.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x200518`**, return `0x1FE4A4`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x00200518` as an established **98-body-byte / 28-instruction** leaf with **no direct calls** (`recomp-post-memory-next-26/seed.json`, local/ignored). It reads a caller mode/depth word and writes four device dwords at `[EDI]`, `[EAX+0x700004]`, `[EAX+0x700008]` and `[EAX+0x70000C]`, with special cases for mode bytes `0x39`, `0x62` and `0x97`. This is static export evidence, not admitted translation or observed execution.

## Validation

`device_registers_test.c`, `port_io_test.c`, `time_fields_test.c` and `gpu_instance_test.c` pass optimized and ASan/UBSan WSL GCC runs; `recomp_memory_access_test.py` is 10/10, `title_seed_test.py` is 25/25, `npm run test:runtime` is 48/48, `npm run check` passes for 243 source files, and `git diff --check` plus LF-only checks are clean. No original module was committed or served.

## Prior milestone: 0x2004DF

Translation `recomp-title-post-memory-28` was 364 functions / 13,976 instructions; module `recomp-post-memory-build-42` was 501,401 bytes, SHA-256 `0e13a564…`. `0x002004DF` wrote 22 index/value dwords into `0xFD700010`–`0xFD700064`, caller `0x001FE43A` executed, and the run reached 258 routines with 5,300 entries / 9,152 events, stopping at `0x20057A` (return `0x1FE499`). Report `post-memory-browser-20261103-25.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes, zero ranges, time conversion, port-output log, GPU instance claim and event capacity are synthetic proof scaffolding, not recovered console state. Next: admit and translate `0x00200518` from the retained read-only export, then follow its next genuine dependency. Do not infer NV2A interrupt delivery, DMA, device behavior or rendering from the supplied synthetic reads.
