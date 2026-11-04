# Display-register traffic is now auditable in the device log

November 4, 2026. The modelled display path previously bypassed the device traffic log, so a report could show the PCRTC state but not the guest's own enable/acknowledge sequence. Accesses served outside the synthetic aperture can now be logged without creating storage, and the display registers use it. The guest-side contract is now visible per run. **There is no timing source, engine, renderer or playable screen.**

## Source-owned change

- `src/runtime/device_registers.h/.c` adds `ff_device_registers_log(device, address, direction, value, site)`: it appends one ordered event for an access served elsewhere, never creates or changes a register entry, rejects unknown directions, addresses outside the aperture and a zero site, and fails with `FF_BOOT_NO_MEMORY` when the bounded log is full.
- `tools/fixtures/thread_startup_probe.c` logs every display-path read and every committed display-path write (the write value is the guest's own stored value, applied at the shadow flush), so `0xFD600100`, `0xFD600140` and `0xFD000100` appear in `deviceRegisters.log` with their real sites.
- `tests/device_registers_test.c` adds `externally_served_accesses_can_be_logged_without_storage`: events record address/direction/value/site, the entry table is unchanged, logged addresses still read as unmodelled until written, invalid directions and out-of-window addresses are rejected without consuming a slot, and a full log returns `FF_BOOT_NO_MEMORY`.

## Observed sequence in a run

With `device.pcrtcVblanks: 8`, the report now shows the guest's path explicitly:

| address | direction | value | site |
| --- | --- | --- | --- |
| `0xFD600140` | read | 0 | `0x1FE05A` |
| `0xFD600140` | write | 0 | `0x1FE05A` |
| `0xFD600100` | read | 1 | `0x200340` |
| `0xFD600100` | write | 1 | `0x200340` |
| `0xFD600140` | read | 0 | `0x200340` |
| `0xFD600140` | write | 1 | `0x200340` |
| `0xFD000100` | read | 0 | `0x1FE50D` |

That is the acknowledge (`0xFD600100 = 1`) and enable (`0xFD600140 = 1`) already identified from the disassembly, now measured in the same report from the same run.

## Run summary

Module `recomp-post-memory-build-70`: **SHA-256 `ae51f0558f55ab31065e04d51d4d5cc56ae27b9e2c95193c39497b6c4c66949c`**.

| profile | stop | routines | trace | device events | `pcrtc` |
| --- | --- | --- | --- | --- | --- |
| `pcrtcVblanks: 8` | status 15 at `0xFD003214` | 276 | 7,517 | 32,768 (full) | `{ pending: 0, enabled: 1, events: 8 }` |

The stop is the bounded log filling while the guest services PFIFO channel work; not engine progress. The canonical profile is unchanged and remains fail-closed.

## Limits

Logging is evidence, not behaviour: it changes no read, write or timing. There is still no display timing source (the events are declared by the profile), no FIFO consumer (owner unassigned), no interrupt delivery beyond the declared input, and no rendering, fidelity or playability.

## Validation

`npm run test:native` 14/14 (device-register fixture in optimized and sanitizer modes); `npm run test:evidence` 3/3; runtime suite 113/113; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Unchanged: the PFIFO engine/consumer contract needs an assigned owner with evidenced producer rules. The new log API must not be used to imply that a logged access was modelled.
