# Original execution clears the 0x80000000 mirror write and stops at a device poll

November 3, 2026. `0x00200518` executed, the run advanced into `0x001FAED0`, and the reached cache-flush write to `0x80000000` now completes through a bounded source-owned physical-mirror page. `0x001FAED0` then polls `0xFD100410` for a hardware transition the synthetic aperture cannot supply, so a new bounded poll limit stops the proof at `0x1FAF10` instead of exhausting the event log. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Admitted helper | `0x00200518`, 98 body bytes / 28 instructions |
| Module | `research/verification/recomp-post-memory-build-46/startup.wasm` (local/ignored) |
| Module bytes | 524,523 |
| Module SHA-256 | `cc4d7c74358a354ca3b44641aa2bc8731eda96fe9ce6c82cb942f9c9dc4d318b` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-28.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-19.json` (local/ignored) |

The retained `next-14`–`next-26` functions are combined in one private seed to stay within the 32-export admission limit. ABI 24, the sole `env.ff_host_clock` import, the 8,192-entry / 32,768-event device aperture and the source-owned GPU instance/port-output/TIME_FIELDS components are unchanged.

## Source-owned changes

- `src/runtime/guest_mirror.c` / `.h`: one bounded 0x1000-byte guest physical-mirror page at `0x80000000`. The pinned public bridge documents that physical page `P` is visible at `0x80000000 + P` on hardware and keeps that window as separate storage because it does not model physical aliasing; this proof does the same for one page. Accesses wholly inside the page round-trip through its storage; execute access, partial overlap and any address outside the page still fall through to the identity-window check and fail closed. This is not a true physical alias or a general `0x80000000` window.
- `tools/fixtures/thread_startup_probe.c`: initializes the mirror on reset and routes only read/write data accesses inside the page to it, before the identity-window check.
- `src/runtime/device_registers.c` / `.h`: `FF_DEVICE_POLL_LIMIT` (16) stops a run of identical reads of the same address and value at one site. The read is recorded first, then the limit fails closed; `ff_device_registers_poll_limited` distinguishes it. Validated reads and failed reads keep their previous atomicity (failed accesses do not mutate the aperture).
- `src/runtime/thread-proof-worker.mjs`: status 12 now reports `device poll limit reached`.
- `tests/guest_mirror_test.c` and an extended `tests/device_registers_test.c` cover mirror bounds/reset/round-trip and the poll limit (including site/value reset and failure atomicity).

## Observed browser execution

- All **83/83** initialization callbacks returned.
- **261** distinct original routines executed and **402** kernel operations completed.
- `0x00200518` executed; execution entered **`0x001FAED0`** (new) and then block `0x1FAF10`.
- The original `0x001FBE9D MOV dword ptr [0x80000000], EDX` followed by `WBINVD` now completes against the bounded mirror page; the virtual-memory fault slot is clear.
- `0x001FAED0` repeatedly reads `0xFD100410`, which holds `0x10000`, waiting for a hardware transition. After **16** identical reads at `0x1FAF10` the bounded poll limit stops the run.
- Device aperture: **5,301 entries** / **12,539 events**; **74,065** checked guest accesses.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 12, `device poll limit reached`, device address `0xFD100410`, return `0x203B90`.

A prior build-45 run without the poll limit produced **20,246** identical reads and exhausted the 32,768-event log; its ~10 MB report exceeded the browser's local save path. The bounded limit keeps the ordered evidence small enough to retain.

## Validation

`tests/guest_mirror_test.c` and the extended `tests/device_registers_test.c` pass optimized and ASan/UBSan WSL GCC runs. The device-register test covers the poll limit, site/value reset and unchanged-state failures.

## Next dependency

`0x001FAED0` is an established translated function (2,527-byte entry region ends at `0x1FAF10`'s caller chain) that waits on `0xFD100410`. The next task is to identify what condition the loop waits for from static evidence and decide whether a narrow, evidence-based modeled completion is justified; the synthetic aperture must not fabricate a device transition. No renderer, playable screen or complete runtime is established.

## Prior milestone: 0x20057A

Translation `recomp-title-post-memory-29` was 365 functions / 14,544 instructions; module `recomp-post-memory-build-43` was 520,818 bytes, SHA-256 `2752487f…`. `0x0020057A` programmed large device register/context blocks (3,344 writes reusing existing entries) and the run reached 259 routines with 5,300 entries / 12,496 events, stopping at `0x200518` (return `0x1FE4A4`). Report `post-memory-browser-20261103-26.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes, zero ranges, mirror page, time conversion, port-output log, GPU instance claim, event capacity and poll limit are synthetic proof scaffolding, not recovered console state. Next: analyze the `0xFD100410` wait in `0x001FAED0` from static evidence, then choose the next bounded step without inventing the awaited device value.
