# Original execution executes 0x1FE0C6 and reaches 0x1FFCC0

November 3, 2026. The previously untranslated helper at `0x001fe0c6` was admitted from a new retained read-only seed. The browser worker executed its first nine instructions, performed the two device writes in its body, and called the next untranslated original routine `0x001ffcc0`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-10` (local/ignored) |
| Functions / instructions | 346 / 12,997 |
| Admitted helper | `0x001fe0c6`, 266 body bytes / 105 instructions |
| Module | `research/verification/recomp-post-memory-build-22/startup.wasm` (local/ignored) |
| Module bytes | 459,451 |
| Module SHA-256 | `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-03.json` (local/ignored) |

The selection reuses the retained 23-seed post-memory inputs plus `recomp-post-memory-next-06/seed.json`. The generated header retains the source digest and 346-function lookup. The module retains ABI 24 and the sole `env.ff_host_clock` import.

The read-only export reports `0x001fe0c6` as a 266-byte/105-instruction established function. It has seven direct calls: `0x001ffcc0`, `0x001ffe27`, `0x001fff25`, `0x001fdf83`, `0x001ffff4`, `0x0020002a` and `0x0020005a`. This is static export evidence; only the first nine instructions were observed executing.

## Observed browser execution

The same explicit synthetic console/disk/memory profile and the previously supplied device-register values were used. No new register read was supplied for this run.

Results:

- All **83/83** initialization callbacks returned.
- **236** distinct original routines executed (up from 235).
- **399** kernel operations completed; `0x001fe0c6` stopped at its first callee before reaching a kernel service.
- `0x001fe0c6` executed its prologue and two stores, then called **`0x001ffcc0`**.
- Device aperture: **9 events** across **8 addresses**. Sites were `2089050` for the earlier helper sequence, `2089093` for the three supplied reads, and `2089158` (`0x1FE0C6`) for the two new writes. The new writes were `0xFD001830` = `0` and `0xFD00180C` = `0xF800`; both had `beforeKnown` false. They are synthetic-aperture storage observations, not NV2A behavior.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FFCC0`**, return `0x1FE0E7`, ordinal 0.

## Next dependency

A bounded read-only export of the reached callee reports `0x001ffcc0` as an established **359-byte / 116-instruction** function with **no direct calls** (`recomp-post-memory-next-07/seed.json`, local/ignored). This is static export evidence, not admitted translation or observed execution.

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
- `python tests/recomp_memory_access_test.py` 10/10.
- `python tests/title_seed_test.py` 25/25.
- `npm run test:runtime` 47/47.
- `npm run check` and whitespace checks pass.
- No original module or content was committed. All XBE, seeds, generated code, modules, settings and reports remain local and ignored.

## Prior milestone: 0x001fe085 and interrupt initialization

The earlier run translated 345 functions / 12,892 instructions in `recomp-title-post-memory-09` and built `recomp-post-memory-build-21` (459,080 bytes, SHA-256 `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9`). It completed 83/83 callbacks, executed 235 distinct routines and 399 kernel operations, returned from `0x001fe085` to `0x1FE5CE`, supplied explicit synthetic zero values for the observed reads `0xFD001800`, `0xFD001808` and `0xFD10020C`, serviced ordinals 44/109/98/47, and stopped at `0x1FE0C6` (return `0x1FE616`). That report is `post-memory-browser-20261103-02.json`.

## Instrumenter requirement

The earlier private build produced a real write value expression:

`MEM32(ebp + -3068) = MEM32(ebp + -3068) & 0`

The instrumenter evaluates any scalar write value expression that probes memory into a temporary before the staged store. This preserves the read/write ordering and event value. Python memory-access tests pass 10/10.

## Limits and next task

`0x1FFCC0` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FFCC0` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
