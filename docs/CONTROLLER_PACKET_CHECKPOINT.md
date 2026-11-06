> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Bounded Xbox controller packet, adapter, port, browser source and four-port device; PFIFO boundary unchanged

November 3, 2026. Following architect guidance, the next bounded milestone is an independent runtime prerequisite rather than a speculative PFIFO consumer: a source-owned Xbox controller state and guest-facing packet encoder based on the pinned compatibility layout. It covers connection state, digital buttons, eight analog buttons and signed stick axes, with explicit packet-change accounting and checked 24-byte encoding. Rumble, host gamepad mapping, async requests, multiplayer remapping and boot-path integration are excluded. The PFIFO fail-closed boundary and all prior evidence are unchanged. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-51/startup.wasm` (local/ignored) |
| Module bytes | 526,955 |
| Module SHA-256 | `cd0bf176d65407f08bf9409b1fd45d6efafde0c7aa3cc31000d2af49c7ba26bb` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Last startup report | `research/verification/post-memory-browser-20261103-46.json` (local/ignored) |

No module rebuild is required; the component is not yet integrated into the boot path.

## Evidence and source-owned change

- Pinned compatibility header `xboxrecomp-source/src/input/xinput_xbox.h` defines `XBOX_GAMEPAD` as `WORD wButtons`, `BYTE bAnalogButtons[8]`, and four `SHORT` thumb axes, plus `XBOX_INPUT_STATE` with a leading `dwPacketNumber`. `input_map.c/h` provide the host-to-Xbox button/axis mapping rules.
- `src/runtime/controller_packet.h/.c`: bounded state (`packet`, `connected`, buttons, analog[8], four signed axes) and a checked encoder. Guest layout is explicit at 24 bytes: `dwPacketNumber` at 0, `wButtons` at 4, analog at 6–13, `sThumbLX/LY/RX/RY` at 14/16/18/20, two trailing alignment bytes zeroed. Packet number increments only when a guest-visible field changes. Encoding a disconnected state, a null state/buffer, or a short buffer fails before writing.
- `tests/controller_packet_test.c`: neutral state, button/analog/axis extremes, signed little-endian axes, unchanged-state packet accounting, disconnect behavior, reserved-byte zeroing and invalid-argument rejection.

## Validation

`tests/controller_packet_test.c` passes optimized and ASan/UBSan WSL GCC runs. The PFIFO boundary remains fail-closed with status 13 at `0xFD800044`; no consumer, GET advance, interrupt, DMA or rendering is added. The controller component is not connected to the browser/worker and does not establish host input mapping or latency.

## Host gamepad adapter

`src/runtime/gamepad-adapter.mjs` adds a pure, bounded adapter from an explicit standard-mapping snapshot to the controller state above. It follows the pinned `input_map.c` rules: 15% radial stick deadzone with rescale, 5% trigger threshold, 50% digital-button threshold, analog face/shoulder buttons at 255, and Y positive upwards. It requires checked 16-button/4-axis snapshots, maps D-pad/Start/Back/thumb bits, keeps the two extra analog slots at zero, and fails closed on malformed input. It does not read the browser Gamepad API, host devices or a clock, and is not integrated into the worker or boot path. `tests/gamepad-adapter.test.mjs` covers neutral, buttons, triggers, deadzone, Y inversion, disconnection and malformed snapshots; it is wired into `npm run test:runtime`. This is adapter-component progress, not input or title fidelity.

## Controller port query

`src/runtime/controller_port.h/.c` wraps one bounded controller port around the packet state and a guest-facing query. The pinned `xinput_device.c` `xbox_InputGetState` returns `result` from `XInputGetState`, which is `ERROR_DEVICE_NOT_CONNECTED` (`1167`) for an absent pad, and does not fill the guest state on that path; on success it zeroes the state and fills `dwPacketNumber`, `wButtons`, the eight analog buttons and the four thumb axes. The port query mirrors the disconnected part of that contract: `FF_CONTROLLER_NOT_CONNECTED` is `1167`, a disconnected port returns it, and the output buffer is left untouched. On success it returns 0 and writes the checked 24-byte packet. Null port/output or a short buffer returns `FF_BOOT_INVALID` before writing; this is a source-owned programming-error code, matching the project's other bounded components rather than the pinned layer's use of `1167` for a null state pointer. Port indices, enumeration, keyboard/mouse merge, rumble and asynchronous requests are excluded.

`tests/controller_port_test.c` covers the disconnected query with a byte-identical untouched buffer, connect, packet-number advance only on change, port independence and null/short rejection. It passes optimized and ASan/UBSan WSL GCC runs. The port remains unintegrated with the worker/boot path.

## Browser gamepad source

`src/runtime/controller-source.mjs` adds the browser-side link above the adapter. It consumes an explicit list shaped like `navigator.getGamepads()` and assigns four positional seats: a missing entry or `connected: false` is a disconnected port; a present pad must declare `mapping === 'standard'` and an integer `index` equal to its seat, otherwise that port fails closed with a reason. Each accepted pad is adapted through `adaptStandardGamepad`, and ports stay independent. The module never calls a browser API, reads a device or uses a clock; the caller supplies the list. `tests/controller-source.test.mjs` covers four disconnected seats, positional assignment, per-port button isolation, non-standard mapping, index mismatch, malformed pad data, explicit disconnection, ports beyond the requested count and invalid list/port-count rejection; it is wired into `npm run test:runtime`. This is source-component progress, not host input or title fidelity.

## Four-port controller device

`src/runtime/controller_device.h/.c` composes four controller ports, matching the pinned `XBOX_MAX_CONTROLLERS` (`4`). Each seat keeps its own connection state and packet number; `ff_controller_device_set_port` and `ff_controller_device_query` reject a port at or beyond four with `FF_CONTROLLER_NOT_CONNECTED` (`1167`), matching the pinned layer's treatment of an out-of-range controller, and return `FF_BOOT_INVALID` for null devices/arguments. A disconnected seat still returns `1167` and leaves the output untouched. Port enumeration, keyboard/mouse merge, rumble and asynchronous requests are excluded. `tests/controller_device_test.c` covers four initial seats, connection, per-seat packet independence, out-of-range rejection, disconnection and null/short rejection; it passes optimized and ASan/UBSan WSL GCC runs. The device remains unintegrated with the worker/boot path.

## Next task

Integrate the source, adapter, packet encoder, port query and four-port device only when a bounded original controller path is reached, or continue another independent runtime prerequisite. Do not claim host input fidelity, latency or original-game compatibility without evidence.

## Prior milestone: full GET..PUT scan

The full submitted span decodes as image/heap, not a queue. Report `post-memory-browser-20261103-46.json`.

## Limits and next task

No playable game, renderer, audio, disc reads or complete runtime. The controller component is a source-owned compatibility prerequisite, not title integration or fidelity acceptance.
