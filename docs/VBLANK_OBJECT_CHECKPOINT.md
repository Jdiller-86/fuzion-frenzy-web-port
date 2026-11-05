# The ordinal-159 wait object is a D3D device field, not a KEVENT

November 5, 2026. Read-only follow-up to the refused event gate. **No implementation, hook change, admission, export, seed or translation change.** The default-off wait model and consumer remain inert.

## Observed object

| Fact | Value |
| --- | --- |
| Wait object | `0x00206080` = D3D device `0x00203B80` + `0x2500` |
| First dword | **`0x00040000`** |
| Signal word (`+4`) | `0` |
| Caller | `D3DDevice_BlockUntilVerticalBlank` (`0x1F5520..0x1F5543`), passing it as `Object` to ordinal 159 |
| Gate result | decision 3 (`FF_VBLANK_WAIT_EVENT`) — refused, no writes |

## Static writers at device+0x2500

- `FUN_001FDC30` writes and reads `[ESI+0x2500]` and tests `byte [ESI+0x2500], 0x10` — a flag/state field, not a KEVENT type.
- `FUN_001FE928` reads and rewrites `[ESI+0x2500]`.
- `FUN_00200143` moves `[EAX+0x2500]` to `ECX` and back.
- `FUN_00200352` writes `0` to `[EAX+0x2500]` and later rewrites it.
- `FUN_0020107C` writes `[ESI+0x2500]` with `EBP`/`EBX`.
- The scanned writers use register values (pointers/flags), not a KEVENT type or signal pair; `0x00040000` appears throughout the title as a D3D resource/flag value, not as an event type.

## Interpretation

The object does not match the pinned generic event layout (`type 1` synchronization, signal state at `+4`). It is a **D3D device field** whose first dword is a flag/magic (`0x40000`) and whose `+4` word is 0; treating it as a KEVENT would be an unsupported assumption. The wait cannot be modeled through the pinned event resolver without first identifying:

1. the initializer of the `device+0x2500` field and its full layout (type/flags, signal/handled word), and
2. which side (the title's ISR or the runtime) writes the signal the wait ultimately observes.

The pinned bridge's `KeWaitForSingleObject` path is still the correct service semantics; only the object classification is unresolved.

## Halt

No model change, no writes, no new service. The next decision is whether to (a) characterize the `device+0x2500` initializer and signal word further, (b) model a D3D-specific vblank event path, or (c) hold. The consumer and wait path remain default-off; no ISR/IRQ/timing or rendering claim.
