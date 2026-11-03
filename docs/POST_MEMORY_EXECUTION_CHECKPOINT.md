# Original execution passes 0x1fe085 and reaches 0x1FE0C6

November 3, 2026. One retained read-only seed admitted the previously untranslated helper at `0x001fe085`. The browser worker executed it, returned to `0x1FE5CE`, serviced four reached interrupt-initialization exports and then stopped at untranslated original routine `0x1FE0C6`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-09` (local/ignored) |
| Functions / instructions | 345 / 12,892 |
| Admitted helper | `0x001fe085`, 65 body bytes |
| Module | `research/verification/recomp-post-memory-build-21/startup.wasm` (local/ignored) |
| Module bytes | 459,080 |
| Module SHA-256 | `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-02.json` (local/ignored) |

The selection reuses the retained 22-seed post-memory inputs plus `recomp-post-memory-next-05/seed.json`. The generated header retains the source digest and 345-function lookup. The module retains ABI 24 and the sole `env.ff_host_clock` import.

## Observed browser execution

One explicit synthetic console/disk/memory profile was used. It supplied zero as the explicit test value for three newly observed register reads:

- `0xFD001800`
- `0xFD001808`
- `0xFD10020C`

These are synthetic profile inputs, not console defaults. The previously supplied `0xFD001804` / `0xFD600140` entries remain.

Results:

- All **83/83** initialization callbacks returned.
- **235** distinct original routines executed.
- **399** kernel operations completed.
- `0x001fe085` executed and returned to `0x1FE5CE`.
- Device aperture: **7 events** across **6 addresses**. Sites were `2089050` for the original helper sequence and `2089093` for the three new reads. Values and directions are in the local report.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FE0C6`**, return `0x1FE616`, ordinal 0.

## Reached kernel services

| Ordinal | Export | Modeled behavior |
| --- | --- | --- |
| 44 | `HalGetInterruptVector` | Returns the supplied level (observed 3) and writes its low byte to the checked IRQL output. |
| 109 | `KeInitializeInterrupt` | Zeroes a checked 44-byte KINTERRUPT and writes routine/context/vector at +0/+4/+8. |
| 98 | `KeConnectInterrupt` | Validates the KINTERRUPT object and returns TRUE (1). |
| 47 | `HalRegisterShutdownNotification` | Validates the explicit registration pointer/flag and accepts registration without delivering a callback. |

The modeled behavior follows the pinned public xboxrecomp bridge and ordinal table. **No interrupt is delivered**, no ISR is synthesized, and no NV2A interrupt/device behavior is implied.

## Instrumenter requirement

The private build produced a real write value expression:

`MEM32(ebp + -3068) = MEM32(ebp + -3068) & 0`

The instrumenter now evaluates any scalar write value expression that probes memory into a temporary before the staged store. This preserves the read/write ordering and event value, and covers the exact observed form. Python memory-access tests pass 10/10.

## Validation

- Browser report from `http://127.0.0.1:4182/thread`; selected module, compatible local executable and explicit settings.
- `tests/interrupt_vector_test.c` optimized and ASan/UBSan native runs pass.
- `python tests/recomp_memory_access_test.py` 10/10.
- `python tests/title_seed_test.py` 25/25.
- `npm run test:runtime` 47/47.
- `npm run check` and whitespace checks pass.

No original module or content was committed. All XBE, seed, generated-code, module, settings and report files remain local and ignored.

## Limits and next task

`0x1FE0C6` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The explicit zero register values and modeled interrupt services are test scaffolding, not recovered console state. Next: admit and translate `0x1FE0C6` from a bounded read-only export, then follow its next genuine dependency.
