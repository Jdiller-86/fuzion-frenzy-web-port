# Original execution completes 0x2004DF and reaches 0x20057A

November 3, 2026. The previously exported leaf at `0x002004DF` now executes, its caller `0x001FE43A` completes, and the next call requests untranslated `0x0020057A`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-28` (local/ignored) |
| Functions / instructions | 364 / 13,976 |
| Admitted helper | `0x002004DF`, 57 body bytes / 20 instructions |
| Module | `research/verification/recomp-post-memory-build-42/startup.wasm` (local/ignored) |
| Module bytes | 501,401 |
| Module SHA-256 | `0e13a564d67cd16e0c332545e8b8aba97f70ec6c1ab067e8570829425a1ff3ec` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-25.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-19.json` (local/ignored) |

The retained `next-14`–`next-24` functions are combined in one private seed to stay within the 32-export admission limit. ABI 24, the sole `env.ff_host_clock` import, the 4,096-event trace ceiling, the 32,768-event device aperture and the source-owned GPU instance/port-output/TIME_FIELDS components are unchanged. The profile keeps its 64 explicit registers and the three zero ranges at `0xFD100200`/0x1000, `0xFD400000`/0x1000 and `0xFD003200`/0x100.

## Observed browser execution

- All **83/83** initialization callbacks returned.
- **258** distinct original routines executed and **402** kernel operations completed.
- `0x002004DF` wrote 22 dwords into `0xFD700010`–`0xFD700064`, pairing an index with an encoded value (for example `0xFD700030 = 6` and `0xFD700034 = 0x8000011C`).
- `0x001FE43A` then executed without device accesses and called **`0x0020057A`**.
- Device aperture: **5,300 entries** and **9,152 events** (no new entries were needed; the writes reused the existing `0xFD700000` region).
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x20057A`**, return `0x1FE499`, ordinal 0.

## Next dependency

A bounded read-only export reports `0x0020057A` as an established **2,527-body-byte / 568-instruction** function with **no direct calls** (`recomp-post-memory-next-25/seed.json`, local/ignored). It programs large `0xFDxxxxxx` register and context blocks with immediate values and counted loops (writes only, plus guest context reads). This is static export evidence, not admitted translation or observed execution. The write volume is expected to consume thousands of device events but fits the retained 32,768-event log; entry growth may need measurement.

## Validation

`device_registers_test.c`, `port_io_test.c`, `time_fields_test.c` and `gpu_instance_test.c` pass optimized and ASan/UBSan WSL GCC runs; `recomp_memory_access_test.py` is 10/10, `title_seed_test.py` is 25/25, `npm run test:runtime` is 48/48, `npm run check` passes for 243 source files, and `git diff --check` plus LF-only checks are clean. No original module was committed or served.

## Prior milestone: 0x200143, 0x1FE30B and 0x200352

Translation `recomp-title-post-memory-27` was 363 functions / 13,956 instructions; module `recomp-post-memory-build-41` was 499,386 bytes, SHA-256 `1f38b342…`. `0x00200143` saved/restored its PGRAPH context, `0x001FE30B` classified a mapping value, and `0x00200352` cleared 0xDFB dwords before programming its device fields. That bulk clear exhausted the fixed 8,192-event log, so the bounded aperture now retains 32,768 events while entries stay at 8,192. The run reached 256 routines with 5,300 entries / 9,130 events and stopped at `0x2004DF` (return `0x1FE053`). Reports `post-memory-browser-20261103-21.json` … `-24.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes, zero ranges, time conversion, port-output log, GPU instance claim and event capacity are synthetic proof scaffolding, not recovered console state. Next: admit and translate `0x0020057A` from the retained read-only export, measure its entry/event growth, then follow its next genuine dependency. Do not infer NV2A interrupt delivery, DMA, device behavior or rendering from the supplied synthetic reads.
