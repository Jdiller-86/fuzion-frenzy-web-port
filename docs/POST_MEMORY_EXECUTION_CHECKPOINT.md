# Original execution completes the NV_PFB_WBC flush and reaches the NV2A USER region

November 3, 2026. The reached `0x001FAED0` wait now completes: the NV2A write-back-cache register `NV_PFB_WBC` (offset `0x410`, flush bit `1 << 16`) is modeled as a single self-clearing bit, so the guest's write/poll sequence sees the flush finish. Execution then advances to a write at `0xFD800040`, the start of the NV2A USER region, which lies just outside the current 8 MiB device aperture window. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-47/startup.wasm` (local/ignored) |
| Module bytes | 524,891 |
| Module SHA-256 | `efb0fbea1c67c44c3a904b0da9294120cdf3f3439909e639e3f0866b7b76e27e` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-29.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-19.json` (local/ignored) |

The retained `next-14`–`next-26` functions are combined in one private seed to stay within the 32-export admission limit. ABI 24, the sole `env.ff_host_clock` import, the 8,192-entry / 32,768-event device aperture and the source-owned mirror-page/GPU-instance/port-output/TIME_FIELDS components are unchanged.

## Source-owned change

- `src/runtime/device_registers.c` / `.h`: `ff_device_registers_add_self_clearing` declares a register whose masked bits complete immediately. The write event retains the value the guest stored, but the entry reads back with the mask cleared; sub-dword merges apply the same rule. Up to four such registers, aligned and in-window, declared before any access.
- `tools/fixtures/thread_startup_probe.c`: after the synthetic aperture is initialized, declares `0xFD100410` with mask `0x10000`. Evidence: the pinned xemu NV2A model defines `NV_PFB_WBC` at PFB offset `0x410`, `NV_PFB_WBC_FLUSH` as `1 << 16`, and its PFB read returns `0` because a flush is not pending. Only that one bit is modeled; the rest of the register and every other PFB/NV2A register remain synthetic storage or fail closed.
- `src/runtime/device-report.mjs`: the aperture scope now states that one self-clearing flush bit is modeled, not general NV2A behavior.
- `tests/device_registers_test.c`: covers the write event value, cleared read-back, preserved unmasked bits, duplicate/unaligned/zero-mask/out-of-window rejection and the declaration-after-access guard.

The 16-read poll limit remains as a fail-closed guard for any later unmodeled wait; this run did not trigger it.

## Observed browser execution

- All **83/83** initialization callbacks returned.
- **261** distinct original routines executed and **402** kernel operations completed.
- In `0x001FAED0`: `0xFD100410` read `0`, the guest wrote `0x10000` (event value retained), and the poll at `0x1FAF10` read back `0`, so the loop exited after one iteration.
- Execution then attempted a 4-byte write at **`0xFD800040`**, which is outside the current `0xFD000000` + 8 MiB aperture window and stopped with status 10 (`guest memory access stopped`).
- Device aperture: **5,301 entries** / **12,523 events**; memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 10, target `0xFD800040`, return `0x203B90`.

## Next dependency

The pinned xemu NV2A map places the USER region at NV2A offset `0x800000` with size `0x800000` (`0xFD800000`–`0xFE000000`), just beyond the proof's 8 MiB window. The profile parser currently caps the aperture window at `0x800000` bytes. Next: decide a bounded way to reach `0xFD800040` — for example raising the explicit window ceiling and supplying the USER page — with tests and without inventing pushbuffer submission behavior or fabricating device results.

## Validation

`device_registers_test.c` passes optimized and ASan/UBSan WSL GCC runs, including the new self-clearing cases. The guest-mirror, port-output, time-field and GPU-instance fixtures also pass. The report saved as `post-memory-browser-20261103-29.json` (3,994,033 bytes).

## Prior milestone: 0x80000000 mirror page and poll limit

Module `recomp-post-memory-build-46` was 524,523 bytes, SHA-256 `cc4d7c74…`. It completed the `MOV [0x80000000], EDX; WBINVD` write through a bounded mirror page and stopped after 16 identical reads of `0xFD100410` (status 12). Report `post-memory-browser-20261103-28.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The mirror page, zero ranges, event capacity, poll limit, self-clearing flush bit, time conversion, port-output log and GPU instance claim are synthetic proof scaffolding, not recovered console state. Next: choose the bounded aperture-window extension for the NV2A USER region and follow the next genuine dependency.
