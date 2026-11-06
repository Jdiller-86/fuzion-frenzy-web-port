> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# The option-6 AV model is withdrawn; ordinal 2 remains unsupported

November 4, 2026. The modeled `AvSendTVEncoderOption` option-6 handler is withdrawn. The public Cxbx-Reloaded source at commit `585c49a50af1255ab155099e06f24505f9c5a800` maps numeric option 6 to `AV_QUERY_AV_CAPABILITIES` and handles it with `AvQueryAvCapabilities()`, while the pinned `xboxrecomp-source` maps the same numeric 6 to `AV_OPTION_QUERY_AVPACK` and returns a fixed compatibility default. Both are replacement implementations with different semantics; neither establishes the original console's behavior, so no option-6 model is retained. Ordinal 2 stays unsupported and fail-closed by default: the reset run stops at **status 1**, target `0xFE000094`, ordinal 2, return `0x1FD429`, with 279 routines and 404 kernel operations. An explicit default-off synthetic raw-DWORD binding now exists only for a labeled experiment; it is not original semantics. Untranslated `0x1F79A0` is **not** admitted. **There is no renderer, timing source or playable screen.**

## Confirmed conflict

- `xboxrecomp-source/src/kernel/kernel.h:1147-1157` names option `0x06` `AV_OPTION_QUERY_AVPACK`; `kernel_hal.c:1463-1479` returns HDTV/NTSC-M/60 Hz `0x00400104`.
- Cxbx at the full cited commit defines `AV_QUERY_AV_CAPABILITIES` as `6` in `src/core/kernel/common/xbox.h` and handles it in `src/core/kernel/exports/EmuKrnlAv.cpp` by returning `AvQueryAvCapabilities()` (pack, standard, refresh and user flags derived from the SMC video mode and nonvolatile settings). It does not define `AV_OPTION_QUERY_AVPACK`; numeric 6 is not a no-op.
- The two references disagree on what numeric 6 means and how the result is composed. Neither is original-console execution, so the disagreement is not adjudicated by preferring one.
- The Cxbx source was read from the public repository at the cited commit and is not stored locally; `tools/original-execution-toolchain.json` records the full commit, `retained: false`, and the semantic conflict.

## What was withdrawn

The AV-pack model is removed from every runtime path:

- `src/runtime/av-pack.mjs` and `tests/av-pack.test.mjs` are deleted; no pack constants, composer or provenance field remain.
- `device.avPack`, the `avPack` experiment name and the `ff_probe_av_pack` control are removed from the profile parser, worker, probe and report contract.
- The probe no longer resolves ordinal 2 to a handler. The unsupported-import path records the attempted call and stops.
- `src/runtime/kernel-service.mjs` keeps only the static service facts: `name`, `callingConvention: 'stdcall'`, `argumentBytes: 16` and `returns: 'void'`. It carries no option, result bits, output pointer or permissions, and `proof-report.mjs` rejects those facts in the static descriptor.
- The canonical and experimental profiles remain fail-closed at ordinal 2.

No fixed AV-pack default, fake `NTSTATUS`/EAX success or guessed capability/mode/timing is retained.

## Ordinal-2 call contract (captured, not modeled)

From the retained translation `ff_title_001FD410` and the reset report:

| item | observed |
| --- | --- |
| target | IAT slot `0x00253450`, mapped to kernel ordinal 2 (`AvSendTVEncoderOption`) |
| call site / return | `0x1FD423` / `0x1FD429` |
| ABI | The call site pushes four arguments and a return address, and the following instruction is not an `add esp, imm`; the translator therefore emits `RECOMP_ICALL_SAFE_AT` (callee-cleans), so the call expects a stdcall/NTAPI callee that pops 16 bytes. The return is `void`-style; the caller reads the 4-byte output at `0x203720`, not an NTSTATUS in EAX. |
| arguments | `(0, 6, 0, 0x203720)`: first/register-base `0`, numeric option `6`, value `0`, output pointer `0x203720` |
| call gate | The wrapper calls only when `[0x203720] == 0`. |
| output | A 4-byte guest value at `0x203720`; the wrapper returns it in EAX, and the caller masks `0xFF`/`0xFF00` and scans a table at `0x202C38` (0xBF entries, 0xC bytes each). These are observed consumer facts; the numeric option is not assigned a meaning. |
| required inputs | A 4-byte writable guest output word at `0x203720`; no encoder handle is passed (first argument `0`). |
| other call sites | The retained caller evidence shows options `0xB`, `0xF` and `9` with a handle from `[context + 0x2308]`; the pinned header names `0xB` `AV_OPTION_FLICKER_FILTER` and `9` `AV_OPTION_QUERY_AV_CAPABILITIES`, while `0xF` is not defined there. These are different call sites and do not define option 6. |

The reset report records the attempted call in the standard call list: `{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }`. These are the four observed call-frame words in argument order; they are evidence, not an interpreted signature. No output word or return value is supplied. `proof-report.mjs` requires exactly four observed arguments, rejects any output on an unsupported ordinal-2 call, and rejects a status-0 call so the unsupported service cannot be reported as successful.

### Consumer-domain evidence

Read-only analysis of the retained executable's table at `0x202C38` (0xBF entries, 0xC bytes) records the accepted domain: NTSC-M, NTSC-J and PAL-I standards with SCART, HDTV, VGA or wildcard packs; the last entry is an `0xFF` sentinel and **no row has standard `0x00`**. A zero result therefore matches no row. Cxbx's option 6 is a capabilities query, not a no-op, and the synthetic `xboxrecomp-source` value `0x00400104` matches 33 candidate rows. Both are replacement implementations, not original behavior. The aggregate evidence and limits are in [the mode-domain checkpoint](AV_MODE_DOMAIN_CHECKPOINT.md).

### Unconnected numeric-6 component

`src/runtime/av_query.h/.c` is one reusable, unconnected component for the observed `(0, 6, 0, Result)` shape. It checks `RegisterBase == 0`, numeric option `6` and `Param == 0`, takes a nullable explicit `ff_av_query_config` pointer (a null pointer means no synthetic DWORD was supplied and the query is denied; there is no default), and writes the configured word only when `ff_vm_check` grants the whole four-byte span on a real `ff_virtual_memory` window. The store is bytewise little-endian, so an unaligned guest pointer is safe, and a denial leaves every byte unchanged. It supplies no guest return value and mutates no CPU or stack state.

`tests/av_query_fixture_test.c` calls that component directly, not a duplicate writer. Two distinct DWORDs, zero and all-ones must appear verbatim in little-endian order at aligned and unaligned pointers; a null config is denied; wrong option/base/param, read-only, null, unmapped, one-page-crossing and VM-end-crossing spans are denied; a committed two-page span succeeds; and 32-bit wrap addresses are denied. Each success changes exactly four bytes and leaves four bytes on both sides unchanged; each denial leaves the whole window unchanged. It is compiled in the native component suite but is not linked to the probe, a service handler or any runtime path.

## Measured result

Module `recomp-post-memory-build-70`, SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`.

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pcrtc` control | status 15 at `0xFD003214` | 276 | 403 | 32,768 |
| `pfifoReset` | **status 1 at `0xFE000094`** (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`, and the stop record carries `service: { ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false }`.

## Reference facts (not adopted)

- `src/kernel/kernel.h:1100` declares `VOID __stdcall AvSendTVEncoderOption(PVOID RegisterBase, ULONG Option, ULONG Param, PULONG Result)`.
- `kernel.h:1147-1157` defines option 6 as `AV_OPTION_QUERY_AVPACK` in that reference.
- `kernel.h:1124-1145` defines pack (`0xFF`), standard (`0xFF00`) and refresh (`0xC00000`).
- `kernel_hal.c:1463-1479` returns `AV_PACK_HDTV | (AV_STANDARD_NTSC_M << 8) | AV_REFRESH_60Hz` (`0x00400104`).

These remain a labeled reference implementation, not an original-runtime observation. No value or capability is adopted.

## What this establishes

- The two admitted helpers execute in the original path; the PFIFO reset path is no longer the blocker for this branch.
- The next dependency is a kernel service, not an untranslated original routine.
- The numeric-6 memory contract is captured in one reusable, unconnected component; the kernel semantics remain unresolved.
- The consumer's accepted AV mode domain is captured, and a zero result cannot match it; the original console's mode is still unknown. Cxbx's option 6 is a capabilities query, not a no-op.
- The service cannot be modeled from the current evidence: the replacement references have different option-6 semantics, and no original-console observation exists.

## Limits

- No encoder capabilities, mode, timing, refresh or return value are known.
- No display timing, renderer, fidelity or playability is established.
- The service stays unsupported by default; the explicit synthetic raw-DWORD binding is not original semantics and `0x1F79A0` is not grounded progress.
- No frontier expansion was performed this cycle.

## Next task

Keep ordinal 2 unsupported by default and numeric option 6 neutral. The accepted component and its default-off adapter are documented in [the binding checkpoint](AV_QUERY_BINDING_CHECKPOINT.md); the raw word is explicit, never composed or defaulted. Do not export or admit `0x1F79A0`, supply a default pack word or fake EAX/NTSTATUS success. Disc/FATX expansion and guest wiring stay frozen pending a reached dependency or review.
