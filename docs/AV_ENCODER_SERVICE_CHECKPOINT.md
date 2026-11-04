# The reset path reaches AvSendTVEncoderOption; no encoder service is modeled

November 4, 2026. The narrow admission of `0x1FD42F` and its direct dependency `0x1FD410` advanced the reset run to a new genuine stop: kernel ordinal 2, `AvSendTVEncoderOption`, at target `0xFE000094`, return `0x1FD429`. `0x1FD410` (`CMiniport_GetDisplayCapabilities`) calls the IAT slot `0x00253450`, which the retained profile maps to ordinal 2. The harness has no encoder model, so execution stops with status 1 (unsupported Xbox service) instead of guessing the encoder's behaviour. **There is no display model, renderer, timing source or playable screen.**

## Narrow admission

- Translation `recomp-title-recomp-title-post-memory-49`: **418 functions / 17,353 instructions**.
- Added `0x1FD42F` (49 instructions; calls `0x1FD410`) and `0x1FD410` (10 instructions; calls ordinal 2 through the IAT).
- One merged seed within the existing 32-export budget, recomposed with `--guard-memory`.
- `tools/verify-seeds.py`: 32 recorded / 32 offered / 0 unexpected / 0 missing.
- `tools/verify-translation.py`: 0 functions needing rewriting.
- Module `recomp-post-memory-build-70`, SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`.

## Measured stop

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pcrtc` control | status 15 at `0xFD003214` | 276 | 403 | 32,768 |
| `pfifoReset` | **status 1 at `0xFE000094`** (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`. The stop record now carries `service: { ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false }`; `proof-report.mjs` requires a non-modeled descriptor for every status-1 stop and rejects one on any other stop, so the boundary cannot be reported as modeled or omitted. `src/runtime/kernel-service.mjs` is metadata only and implements nothing.

The report now also records the attempted call in the standard call list: `{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }`. These are the four observed call-frame words in argument order; they are evidence, not an interpreted signature, and no return value is supplied. The probe records the attempted call before the fail-closed stop, and the worker maps ordinal 2 to four arguments.

## Ordinal-2 call contract (captured, not modeled)

From the retained translation `ff_title_001FD410` and the reset report:

| item | observed |
| --- | --- |
| target | IAT slot `0x00253450`, mapped to kernel ordinal 2 (`AvSendTVEncoderOption`) |
| call site / return | `0x1FD423` / `0x1FD429` |
| ABI | The call site pushes four arguments and a return address, and the following instruction is not an `add esp, imm`; the translator therefore emits `RECOMP_ICALL_SAFE_AT` (callee-cleans), so the call expects a stdcall/NTAPI callee that pops 16 bytes. The return is `void`-style; the caller reads the 4-byte output at `0x203720`, not an NTSTATUS in EAX. |
| arguments | `(0, 6, 0, 0x203720)`: first/register-base `0`, option `6`, value `0`, output pointer `0x203720` |
| call gate | The wrapper calls only when `[0x203720] == 0`. |
| output | A 4-byte guest value at `0x203720`; the wrapper returns it in EAX, and the caller masks `0xFF`/`0xFF00` and scans a table at `0x202C38` (0xBF entries, 0xC bytes each). Bits 16-31 are not consumed on this path; the exact bit meanings are not assigned. The report descriptor records `output: { address: 0x203720, bytes: 4, writable: true, masks: [0xFF, 0xFF00], table: { base: 0x202C38, entries: 0xBF, stride: 0xC } }`. |
| required inputs | A 4-byte writable guest output word at `0x203720`; no encoder handle is passed (first argument `0`). |
| other call sites | The retained caller evidence shows options `0xB`, `0xF` and `9` with a handle from `[context + 0x2308]`; those are different call sites and do not define option 6. |
| pinned reference | xemu models the SMBus encoders (`hw/xbox/smbus_cx25871.c`, `smbus_fs454.c`, `smbus_xcalibur.c`) and the machine's `video-encoder` property, not the kernel export's option/return semantics. |

The service remains unsupported: the harness records the call and stops with status 1, leaving EAX and the NTSTATUS unset. It does not return `STATUS_SUCCESS` or any NTSTATUS. The report's `service` descriptor records `callingConvention: 'stdcall'`, `argumentBytes: 16`, `returns: 'void'`, and `proof-report.mjs` requires those facts for ordinal 2.

## What this establishes

- The two admitted helpers execute in the original path; the PFIFO reset path is no longer the blocker for this branch.
- The next dependency is a kernel service, not an untranslated original routine.
- The retained callers evidence names ordinal 2 as `AvSendTVEncoderOption`; the run's stack identifies the call site but the exact option value and encoder semantics are not assigned here.

## Limits

- No encoder capabilities, mode, timing or return values are guessed.
- No display timing, renderer, fidelity or playability is established.
- One helper and its direct dependency were added; no broad closure or frontier batch.

## Next task

Decide whether to model a bounded, evidence-backed `AvSendTVEncoderOption` service (arguments, return values, supported options) or take another independent prerequisite. Do not infer encoder behaviour or display timing from the call alone. Keep the canonical profile fail-closed and the geometry lane paused.
