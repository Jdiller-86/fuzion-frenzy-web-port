# The explicit AV-pack experiment passes AvSendTVEncoderOption and reaches 0x1F79A0

November 4, 2026. The ordinal-2 contract is captured, and option 6 is modeled **only** under an explicit AV-pack profile. Without a declared pack the service stays fail-closed at `0xFE000094`; with `device.avPack` declaring pack, standard and refresh, the handler writes the word to `0x203720` and the run continues to untranslated `0x1F79A0` (return `0x1F83ED`, 280 routines, 405 kernel operations). The pack is player-declared; the pinned reference's HDTV/NTSC-M/60 Hz value is not a default. **There is no renderer, timing source or playable screen.**

## Narrow admission

- Translation `recomp-title-recomp-title-post-memory-49`: **418 functions / 17,353 instructions**.
- Added `0x1FD42F` (49 instructions; calls `0x1FD410`) and `0x1FD410` (10 instructions; calls ordinal 2 through the IAT).
- One merged seed within the existing 32-export budget, recomposed with `--guard-memory`.
- `tools/verify-seeds.py`: 32 recorded / 32 offered / 0 unexpected / 0 missing.
- `tools/verify-translation.py`: 0 functions needing rewriting.
- Module `recomp-post-memory-build-70`, SHA-256 `8f567b811a728fac05ea7f36b8d01ebaff1fa421be30dc52447cffdc4659fa29`.

## Measured stop

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pcrtc` control | status 15 at `0xFD003214` | 276 | 403 | 32,768 |
| `pfifoReset` (no AV pack) | status 1 at `0xFE000094` (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |
| `pfifoReset` + `avPack` | **status 2 at `0x1F79A0`** (return `0x1F83ED`) | 280 | 405 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`. The stop record now carries `service: { ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false }`; `proof-report.mjs` requires a non-modeled descriptor for every status-1 stop and rejects one on any other stop, so the boundary cannot be reported as modeled or omitted. `src/runtime/kernel-service.mjs` is metadata only and implements nothing.

The report now also records the attempted call in the standard call list: `{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }`. These are the four observed call-frame words in argument order; they are evidence, not an interpreted signature, and no return value is supplied. The probe records the attempted call before the fail-closed stop, and the worker maps ordinal 2 to four arguments.

## Modeled option-6 experiment

- `device.avPack: { pack, standard, refresh }` is required; there is no default. `composeAvPackWord` validates the names against the pinned constants, and the worker calls `ff_probe_av_pack` before the run.
- The probe handler supports only option 6 (`AV_OPTION_QUERY_AVPACK`), validates the 4-byte writable output pointer, writes the declared word, and returns void with stdcall cleanup (pops 16 bytes).
- Without the profile, the resolver keeps the existing status-1 fail-closed stop.
- The report records `avPack: 0x00400104` and `calls[ordinal 2] = { status: 0, arguments: [0, 6, 0, 2111264], output: { packWord: 0x00400104 } }`.
- The value used in the private profile is the pinned reference's HDTV/NTSC-M/60 Hz default, explicitly declared. It is not a claim about the original console, and the canonical profile remains fail-closed.

- `proof-report.mjs` validates the observed call: ordinal 2 must carry four arguments; a status-0 call must carry `output.packWord` equal to `report.avPack`; a non-zero call must carry no output.
- `tools/original-execution-toolchain.json` records the Cxbx `585c49a` reference prefix with `retained: false`; the conflict remains unverified.
- The report carries `avPackProvenance: { primary: 'xboxrecomp-source', conflict: 'cxbx-585c49a', status: 'unverified' }` whenever `avPack` is present; `proof-report.mjs` requires it, so a modeled run cannot be read as settled provenance.

### Unconnected query fixture

`tests/av_query_fixture_test.c` models only the memory contract: a granted 4-byte write changes exactly four little-endian bytes, while read-only, unaligned, null and unmapped pointers are denied with no partial change. It is not linked to the probe, the service handler or any runtime path.

## Ordinal-2 call contract (captured, not modeled)

From the retained translation `ff_title_001FD410` and the reset report:

| item | observed |
| --- | --- |
| target | IAT slot `0x00253450`, mapped to kernel ordinal 2 (`AvSendTVEncoderOption`) |
| call site / return | `0x1FD423` / `0x1FD429` |
| ABI | The call site pushes four arguments and a return address, and the following instruction is not an `add esp, imm`; the translator therefore emits `RECOMP_ICALL_SAFE_AT` (callee-cleans), so the call expects a stdcall/NTAPI callee that pops 16 bytes. The return is `void`-style; the caller reads the 4-byte output at `0x203720`, not an NTSTATUS in EAX. |
| arguments | `(0, 6, 0, 0x203720)`: first/register-base `0`, option `6`, value `0`, output pointer `0x203720` |
| call gate | The wrapper calls only when `[0x203720] == 0`. |
| output | A 4-byte guest value at `0x203720`; the wrapper returns it in EAX, and the caller masks `0xFF`/`0xFF00` and scans a table at `0x202C38` (0xBF entries, 0xC bytes each). The pinned reference defines pack in `0xFF`, standard in `0xFF00` and refresh in `0xC00000`; the caller's observed masks cover pack and standard. The report descriptor records `output: { address: 0x203720, bytes: 4, writable: true, masks: [0xFF, 0xFF00], table: { base: 0x202C38, entries: 0xBF, stride: 0xC } }` and `resultBits: { pack: 0xFF, standard: 0xFF00, refresh: 0xC00000 }`. |
| required inputs | A 4-byte writable guest output word at `0x203720`; no encoder handle is passed (first argument `0`). |
| other call sites | The retained caller evidence shows options `0xB`, `0xF` and `9` with a handle from `[context + 0x2308]`; the pinned header names `0xB` `AV_OPTION_FLICKER_FILTER` and `9` `AV_OPTION_QUERY_AV_CAPABILITIES`, while `0xF` is not defined there. These are different call sites and do not define option 6. |
| pinned reference | `xboxrecomp-source/src/kernel/kernel.h` and `kernel_hal.c` implement the export and option layout; xemu models the SMBus encoders and the machine's `video-encoder` property, not the kernel export. |

### Pinned reference semantics

- `src/kernel/kernel.h:1100` declares `VOID __stdcall AvSendTVEncoderOption(PVOID RegisterBase, ULONG Option, ULONG Param, PULONG Result)`.
- `kernel.h:1147-1157` defines option 6 as `AV_OPTION_QUERY_AVPACK`, plus the other AV option codes.
- `kernel.h:1124-1145` defines pack (`0xFF`), standard (`0xFF00`) and refresh (`0xC00000`); standard codes are NTSC-M/J and PAL-I, refresh is 60/50 Hz.
- `kernel_hal.c:1463-1479` returns `AV_PACK_HDTV | (AV_STANDARD_NTSC_M << 8) | AV_REFRESH_60Hz` (`0x00400104`).

That value is a **reference default** for a North-American HDTV/NTSC/60 Hz console, not this player's console. It is not adopted: the service remains unsupported until the player's pack/standard/refresh is grounded. No capability or mode is guessed.

The service is unsupported by default and modeled only for option 6 when the profile declares an AV pack. The static `service` descriptor records only the service facts: `callingConvention: 'stdcall'`, `argumentBytes: 16`, `returns: 'void'` and `resultBits: { pack: 0xFF, standard: 0xFF00, refresh: 0xC00000 }`. The observed call facts — option 6, arguments `[0, 6, 0, 2111264]`, output pointer `0x203720`, masks and table — are recorded in the run's call list and in the "Ordinal-2 call contract" section, not in the static descriptor. `proof-report.mjs` rejects observed facts in the static descriptor.

## Provenance conflict

A provisional review cites a Cxbx-Reloaded pin `585c49a` conflict for option 6. That source is not retained in this worktree or the main analysis tools, so the conflict cannot be verified here. The option-6 naming in this note remains the pinned `xboxrecomp-source` fact (`kernel.h:1147-1157`), not an adjudicated cross-reference. The service stays explicit/default-off, and no default or EAX/NTSTATUS success is supplied.

## What this establishes

- The two admitted helpers execute in the original path; the PFIFO reset path is no longer the blocker for this branch.
- The next dependency is a kernel service, not an untranslated original routine.
- The service is modeled only under an explicit AV-pack profile and only for option 6; without the profile it stays fail-closed. The declared value is player input, not a claim about the original console.

## Limits

- No encoder capabilities, mode, timing or return values are guessed.
- No display timing, renderer, fidelity or playability is established.
- One helper and its direct dependency were added; no broad closure or frontier batch.

## Next task

Retain the Cxbx pin `585c49a` (or an equivalent authoritative source) to resolve the option-6 provenance conflict before any further service claims. Until then keep the service explicit/default-off and do not admit `0x1F79A0` as grounded progress. No default AV pack, no EAX/NTSTATUS success, and no broad batch.
