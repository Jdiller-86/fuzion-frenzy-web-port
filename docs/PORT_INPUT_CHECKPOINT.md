# Bounded port input for 0x80C0; execution reaches a new register read at 0xFD002080

November 4, 2026. The reached D3D path reads x86 port `0x80C0`, which the build did not define. The source-owned port surface now models the pinned bridge's field-pin read: bit 5 toggles per read. The `xbox_PortIn` bridge is defined in the probe. The rebuilt module advances to 276 distinct routines and stops on an undeclared device-register read at `0xFD002080`. **There is no playable screen, renderer or device model.**

## Evidence and source change

- The pinned public bridge documents `0x80C0` as the ACPI GPIO field pin for the video encoder: bit 5 follows vblank parity; before a vblank count it toggles per read. It returns zero for every other port.
- `src/runtime/port_io.h/.c` adds `ff_port_io_read`: valid 1/2/4-byte reads only; `0x80C0` returns `(reads & 1) << 5`; other ports return zero; invalid widths return zero without counting.
- `tools/fixtures/thread_startup_probe.c` defines `xbox_PortIn` as `ff_port_io_read` alongside the existing bounded `xbox_PortOut` log.
- `tests/port_io_test.c` covers the output log, `0x80C0` toggling, other-port zero, invalid width and reset; optimized and ASan/UBSan pass.
- This is a synthetic input model derived from the pinned bridge, not observed original-console hardware or vblank timing.

## Build and run

- Translation with the `0x1F6240` closure: 415 functions / 17,284 instructions.
- Private module `recomp-post-memory-build-68`: 637,321 bytes, SHA-256 `e83b898660598969d62dc322083117f3e79b9522591cd60db8712ed28bbc5827`.
- Profile run: status 10 (`guest memory access stopped`) at target `0xFD002080`, read intent, return `0x205E98`; **276** distinct routines, 83/83 initializers, 403 kernel operations.
- The `0x80C0` reads are no longer a build or runtime blocker.

## Next blocker

`0xFD002080` is a PFIFO-aperture address not covered by the 64 explicit register entries or the four explicit zero ranges (the nearest zero range starts at `0xFD003200`). It is a read, not a write; no value can be invented. The next step is to add an explicit synthetic register or zero-range entry for `0xFD002080` (within the bounded profile), then rerun. Extending the zero-range bound or replacing an unused explicit entry is a source/profile decision that needs the same explicit-input discipline.

## Limits

No rendering, audio, fidelity or playability is established. The `0x80C0` model is a pinned-bridge field pin, not original timing; other ports read zero. The translation and modules are private; only the port source and test are published.

## Validation

`tests/port_io_test.c` optimized and ASan/UBSan; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Add the explicit `0xFD002080` synthetic register/zero-range input, rebuild and rerun; then follow the next reached dependency. Do not invent the register's original value or claim rendering/fidelity without evidence.
