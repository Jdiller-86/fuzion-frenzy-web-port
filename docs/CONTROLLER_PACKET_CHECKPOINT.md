# Bounded Xbox controller packet component; PFIFO boundary unchanged

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

## Next task

Continue independent runtime prerequisites or return to PFIFO only if evidence establishes the channel initialization or an actual submitted command range. A bounded host gamepad-to-state adapter can follow from the same pinned mapping rules, still without claiming original-game input fidelity.

## Prior milestone: full GET..PUT scan

The full submitted span decodes as image/heap, not a queue. Report `post-memory-browser-20261103-46.json`.

## Limits and next task

No playable game, renderer, audio, disc reads or complete runtime. The controller component is a source-owned compatibility prerequisite, not title integration or fidelity acceptance.
