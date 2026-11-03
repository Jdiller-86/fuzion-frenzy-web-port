# Original execution completes 0x1FFCC0 and reaches 0x1FFE27

November 3, 2026. The previously untranslated helper at `0x001ffcc0` was admitted from a retained read-only seed. The browser worker executed it to completion and returned to its caller `0x001fe0c6`, which then called the next untranslated original routine `0x001ffe27`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-11` (local/ignored) |
| Functions / instructions | 347 / 13,113 |
| Admitted helper | `0x001ffcc0`, 359 body bytes / 116 instructions |
| Module | `research/verification/recomp-post-memory-build-23/startup.wasm` (local/ignored) |
| Module bytes | 464,158 |
| Module SHA-256 | `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-05.json` (local/ignored) |

The selection reuses the retained 24-seed post-memory inputs plus `recomp-post-memory-next-07/seed.json`. The generated header retains the source digest and 347-function lookup. The module retains ABI 24 and the sole `env.ff_host_clock` import.

The read-only export reports `0x001ffcc0` as a 359-byte/116-instruction established function with no direct calls. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The explicit synthetic profile now also supplies zero as the test value for five newly observed register reads:

- `0xFD000200`
- `0xFD000140`
- `0xFD680500`
- `0xFD680504`
- `0xFD680508`

These are synthetic test inputs, not console defaults. The earlier `0xFD001800`, `0xFD001804`, `0xFD001808`, `0xFD10020C` and `0xFD600140` entries remain.

Results:

- All **83/83** initialization callbacks returned.
- **237** distinct original routines executed (up from 236).
- **399** kernel operations completed; the new helper made no kernel service call.
- `0x001ffcc0` executed to completion and returned to `0x001fe0c6`.
- `0x001fe0c6` continued through its first-callee return and called **`0x001ffe27`** at `0x1FE194`.
- Device aperture: **25 events** across **16 addresses**. Sites were `2089050`, `2089093`, `2089158`, `2096353`, `2096476`, `2096583` and `2089324`. The new sub-dword reads were `0xFD680505`, `0xFD680509` and `0xFD680501`; the three stores were `0xFD009200` = `0`, `0xFD009210` = `1` and `0xFD009420` = `0xFFFFFFFF`. All are synthetic-aperture observations, not NV2A behavior.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FFE27`**, return `0x1FE199`, ordinal 0.

## Sub-dword device read prerequisite

`0x001ffcc0` reads a dword and then the byte at `+1` (`MOVZX ECX, byte ptr [EAX + 0x1]`) from each of `0xFD680504`, `0xFD680508` and `0xFD680500`. The synthetic aperture previously admitted only 4-byte accesses, so the first byte read stopped the run.

The source-owned aperture now:

- admits 1-, 2- and 4-byte accesses inside the configured window, with dword-aligned 4-byte accesses and no sub-dword straddle across a dword;
- maps a 1/2-byte read into the containing dword entry and records the event at the accessed byte/halfword address with the extracted value and `beforeKnown` provenance;
- fails closed on sub-dword writes, because no original write of that width has been observed and the staged-write path records a dword value;
- passes the actual access width from `ff_probe_guest_memory` into the device accessor.

`tests/device_registers_test.c` covers byte/halfword reads, extracted values, event provenance, unknown containing registers, rejected sub-dword writes and rejected 3-byte accesses. Optimized and ASan/UBSan runs pass.

## Next dependency

A bounded read-only export of the reached callee reports `0x001ffe27` as an established **254-byte / 72-instruction** function (`recomp-post-memory-next-08/seed.json`, local/ignored). It has one kernel-import call at `0x001FFE70` through `0x00253484`, one direct call at `0x001FFF15` to `0x001FDF70`, and no other direct calls. This is static export evidence, not admitted translation or observed execution.

## Reached kernel services

| Ordinal | Export | Modeled behavior |
| --- | --- | --- |
| 44 | `HalGetInterruptVector` | Returns the supplied level (observed 3) and writes its low byte to the checked IRQL output. |
| 109 | `KeInitializeInterrupt` | Zeroes a checked 44-byte KINTERRUPT and writes routine/context/vector at +0/+4/+8. |
| 98 | `KeConnectInterrupt` | Validates the KINTERRUPT object and returns TRUE (1). |
| 47 | `HalRegisterShutdownNotification` | Validates the explicit registration pointer/flag and accepts registration without delivering a callback. |

The modeled behavior follows the pinned public xboxrecomp bridge and ordinal table. **No interrupt is delivered**, no ISR is synthesized, and no NV2A interrupt/device behavior is implied.

## Validation

- Browser report from `http://127.0.0.1:4182/thread`; selected module, compatible local executable and explicit settings.
- Browser module identity matches the built `startup.wasm`.
- `tests/device_registers_test.c` optimized and ASan/UBSan native runs pass.
- `python tests/recomp_memory_access_test.py` 10/10.
- `python tests/title_seed_test.py` 25/25.
- `npm run test:runtime` 47/47.
- `npm run check` and whitespace checks pass.
- No original module or content was committed. All XBE, seeds, generated code, modules, settings and reports remain local and ignored.

## Prior milestone: 0x001fe0c6

The earlier run translated 346 functions / 12,997 instructions in `recomp-title-post-memory-10` and built `recomp-post-memory-build-22` (459,451 bytes, SHA-256 `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f`). It completed 83/83 callbacks, executed 236 distinct routines and 399 kernel operations, ran `0x001fe0c6` through its prologue and two stores, and stopped at `0x1FFCC0` (return `0x1FE0E7`). The report is `post-memory-browser-20261103-03.json`.

## Prior milestone: 0x001fe085 and interrupt initialization

The earlier run translated 345 functions / 12,892 instructions in `recomp-title-post-memory-09` and built `recomp-post-memory-build-21` (459,080 bytes, SHA-256 `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9`). It completed 83/83 callbacks, executed 235 distinct routines and 399 kernel operations, returned from `0x001fe085` to `0x1FE5CE`, supplied explicit synthetic zero values for the observed reads `0xFD001800`, `0xFD001808` and `0xFD10020C`, serviced ordinals 44/109/98/47, and stopped at `0x1FE0C6` (return `0x1FE616`). That report is `post-memory-browser-20261103-02.json`.

## Instrumenter requirement

The earlier private build produced a real write value expression:

`MEM32(ebp + -3068) = MEM32(ebp + -3068) & 0`

The instrumenter evaluates any scalar write value expression that probes memory into a temporary before the staged store. This preserves the read/write ordering and event value. Python memory-access tests pass 10/10.

## Limits and next task

`0x1FFE27` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device reads/writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FFE27` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
