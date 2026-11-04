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

## Refused display-log entries now stop the proof

The first version discarded the return value of `ff_device_registers_log` on the display path (`(void)`), so a full event log silently dropped a PCRTC read or write and the report could claim a complete device trace while missing entries. That is evidence loss, not a tolerable diagnostic.

`tools/fixtures/thread_startup_probe.c` now checks the return value in both `pcrtc_flush` (a committed write) and `pcrtc_access` (a read) and stops with **status 15** (device capacity reached) at the display register, ordinal `2` for the write and `1` for the read. No entry is dropped and the run names its capacity boundary instead of continuing with an incomplete trace. The canonical profile and every non-display path are unchanged.

The rebuilt module is `recomp-post-memory-build-70`, SHA-256 `0a03de96cd248f296ba8327ddac5ded9de70d4c944e7797d62b675b1cadf1da4`; the pre-fix build was `ae51f055...`, so the change is present in the module. Re-run results:

| profile | stop | device events | `pcrtc` |
| --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 12,535 | absent |
| `pcrtcVblanks: 8` | status 15 at `0xFD003214` | 32,768 (full) | `{ pending: 0, enabled: 1, events: 8 }` |
| `pcrtcVblanks: 1`, `engineWait` | status 12 at `0xFD003214` | 13,233 | `{ pending: 0, enabled: 1, events: 1 }` |

The `pcrtc` profile still stops on the main aperture path when the shared log fills, before a display-log refusal can occur, so the new branch is a fail-closed guard rather than an observed path in this profile. That is the correct behaviour: the report's completeness is now guaranteed by construction, not by an unchecked assumption.

## Run summary

Module `recomp-post-memory-build-70`: **SHA-256 `0a03de96cd248f296ba8327ddac5ded9de70d4c944e7797d62b675b1cadf1da4`** (after the refused-entry fix).

| profile | stop | routines | trace | device events | `pcrtc` |
| --- | --- | --- | --- | --- | --- |
| `pcrtcVblanks: 8` | status 15 at `0xFD003214` | 276 | 7,517 | 32,768 (full) | `{ pending: 0, enabled: 1, events: 8 }` |

The stop is the bounded log filling while the guest services PFIFO channel work; not engine progress. The canonical profile is unchanged and remains fail-closed.

## Limits

Logging is evidence, not behaviour: it changes no read, write or timing. There is still no display timing source (the events are declared by the profile), no FIFO consumer (owner unassigned), no interrupt delivery beyond the declared input, and no rendering, fidelity or playability.

## Validation

`npm run test:native` 14/14 (device-register fixture in optimized and sanitizer modes); `npm run test:evidence` 4/4; runtime suite 126/126; title-seed 25/25; `npm run check` 327 source files and whitespace checks pass.

## Next task

The PFIFO engine/consumer contract is now documented producer-free in [the engine handoff](PFIFO_ENGINE_HANDOFF.md). The engine workstream must return a non-empty submission with a dispatched method, original/hardware empty-cache evidence, or an explicit decision that the stop is the correct boundary. The display log must not be used to imply that a logged access was modelled.
