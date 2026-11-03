# Original execution completes 0x200143, 0x1FE30B and 0x200352 and reaches 0x2004DF

November 3, 2026. Three previously untranslated helpers now execute in the browser. The aperture also had to retain a second original bulk clear: the fixed 8,192-event log filled during `0x00200352`, so the bounded synthetic capacity was raised to 32,768 events. **There is no playable original screen and no renderer/device model.**

## Selection and builds

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-27` (local/ignored) |
| Functions / instructions | 363 / 13,956 |
| Admitted helpers | `0x00200143`, `0x001FE30B`, `0x00200352` |
| Final module | `research/verification/recomp-post-memory-build-41/startup.wasm` (local/ignored) |
| Final module bytes | 499,386 |
| Final module SHA-256 | `1f38b342759b5f13bda9ca2da43380056308b0a2d34fec1a86484b1f5398b861` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Reports | `post-memory-browser-20261103-21.json` … `-24.json` (local/ignored) |

Intermediate iterations: translation `recomp-title-post-memory-25` was 361 functions / 13,814 instructions (module `recomp-post-memory-build-38`, 490,040 bytes, SHA-256 `1521ea3a…`); translation `recomp-title-post-memory-26` was 362 / 13,841 (module `recomp-post-memory-build-39`, 495,674 bytes, SHA-256 `b2a77970…`); the capacity-only rebuild is `recomp-post-memory-build-41`. The retained `next-14`–`next-24` functions stay combined in one private seed to remain within the 32-export admission limit. ABI 24, the sole `env.ff_host_clock` import and the source-owned GPU instance/port-output/TIME_FIELDS components are unchanged. The event capacity change also updates `DEVICE_EVENTS` in `src/runtime/device-report.mjs` to match `FF_DEVICE_EVENTS`.

## Observed browser execution

| Run | Result |
| --- | --- |
| build-38, profile mmio-18 | Stopped at unsupplied device read `0xFD003204` (status 10, "guest memory access stopped") inside `0x00200143`. |
| build-38, profile mmio-19 | 252 routines, 402 kernel operations, 5,299 entries / 5,468 events; stop at **`0x1FE30B`** (return `0x1FE38A`). |
| build-39 | 254 routines; stop at **`0x200352`** (return `0x1FE00B`); 5,299 entries / 5,512 events. |
| build-40 | 255 routines; the fixed 8,192-event log filled during the second bulk clear at `0xFD703BA8` (status 10). |
| build-41 | 256 routines; 5,300 entries / **9,130 events**; stop at **`0x2004DF`** (return `0x1FE053`). |

Execution details:

- `0x00200143` saved `0xFD002500`, `0xFD003200` and `0xFD003250`, copied 13 PGRAPH state fields to `0xFD701000`–`0xFD701030`, wrote index `1` to `0xFD003204`, read the saved context back from `0xFD701040`–`0xFD701070`, and restored the three saved registers (34 device events).
- `0x0020107c` completed, `0x001FE1D0` returned, and `0x001FE365` called `0x001FE30B`. `0x001FE30B` (90 body bytes / 27 instructions, no calls) classified a mapping value and wrote a resolved base plus a type of 1, 2 or 3 through caller pointers.
- `0x001FE365` then wrote its packed values to the `0xFD700000` instance region and returned; the `0x001FDFC3` path called `0x00200352`.
- `0x00200352` (397 body bytes / 115 instructions, no calls) cleared **0xDFB** dwords (3,580 device writes) in the `0xFD700000` region, then programmed `0xFD002504`, `0xFD003204`, `0xFD003220`, `0xFD003224`, `0xFD003228`, `0xFD00322C`, `0xFD003240`, `0xFD003244`, `0xFD003280`, `0xFD003250`, `0xFD003200` and `0xFD002500`.
- `0x001FE026` executed and the next caller requested **`0x002004DF`**.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.

## Explicit synthetic inputs

The selected profile `settings-mmio-19.json` keeps the 64 explicit registers and adds a third zero range at `0xFD003200` (0x100 bytes), so the state reads in `0x00200143` and `0x00200352` are supplied as explicit zeros; later writes override those entries. All three zero ranges (`0xFD100200`/0x1000, `0xFD400000`/0x1000, `0xFD003200`/0x100) are synthetic test inputs, not recovered console state. The 32,768-event aperture is an ordered evidence log, not an NV2A model.

## Validation

The raised capacity passes the owned device-register test at both optimization levels, including its full-capacity atomicity loop. `device_registers_test.c`, `port_io_test.c`, `time_fields_test.c` and `gpu_instance_test.c` pass optimized and ASan/UBSan WSL GCC runs; `recomp_memory_access_test.py` is 10/10, `title_seed_test.py` is 25/25, `npm run test:runtime` is 48/48, `npm run check` passes for 243 source files, and `git diff --check` plus LF-only checks are clean.

## Next dependency

A bounded read-only export reports `0x002004DF` as an established **57-byte / 20-instruction** leaf with **no direct calls** (`recomp-post-memory-next-24/seed.json`, local/ignored). It packs caller words into `[EAX]` and `[EAX+4]` at the context's `0x130` index. This is static export evidence, not admitted translation or observed execution.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes, zero ranges, time conversion, port-output log, GPU instance claim and event capacity are synthetic proof scaffolding. Next: admit and translate `0x002004DF` from the retained read-only export, then follow its next genuine dependency. Do not infer NV2A interrupt delivery, DMA, device behavior or rendering from the supplied synthetic reads.
