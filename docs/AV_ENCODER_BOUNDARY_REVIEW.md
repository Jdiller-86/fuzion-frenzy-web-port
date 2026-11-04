# Ordinal 2: establish the reached call contract before emulating an encoder

## Current review: numeric option 6 captured; reference semantics conflict (A018)

Source `ebb29e0` records the attempted call as `(RegisterBase, Option, Param, Result) = (0,6,0,0x203720)` at return `0x1FD429`, while keeping ordinal2 unsupported. Its retained caller evidence supports a four-argument stdcall/void-style call and a 4-byte output consumed with masks `0xFF` and `0xFF00`. This is useful lane-reported evidence, not an architect replay. The PFIFO fields are now correctly named `pfifoResetState` with `scope:'seam'`; that snapshot is still not live aperture state.

The extracted `xboxrecomp-source` checkpoint names option6 `QUERY_AVPACK` and option9 `QUERY_AV_CAPABILITIES`, and describes a simplified fixed HDTV/NTSC/60-Hz result. It does not provide a reproducible public commit pin for that extracted source. **Do not treat this convenience implementation as an authoritative original kernel specification.** A second, fuller public implementation conflicts:

- [Cxbx-Reloaded pin `585c49a50af1255ab155099e06f24505f9c5a800`, `src/core/kernel/common/xbox.h:20–75`](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/common/xbox.h#L20-L75) defines numeric option6 as `AV_QUERY_AV_CAPABILITIES`, option9 as `AV_OPTION_BLANK_SCREEN`, and standard mask as `0x0F00` (not the caller-tested `0xFF00`). Pack mask is `0xFF`, refresh mask `0xC00000`; additional HDTV/aspect flags exist. Caller masking is not proof of a semantic field width.
- [Same pin, `src/core/kernel/exports/EmuKrnlAv.cpp:118–153,168–206`](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlAv.cpp#L118-L206) derives the option6 DWORD from SMC pack selection, factory AV region and user video settings, and writes it through Result. NTSC/60Hz is a failed-settings-read fallback, not a measured/default console value. This corroborates a void four-argument query path, not every option or hardware behavior.

The architect fetched these exact public files; independent GLM review confirmed the option/mask/composition discrepancy. Neither third-party reference is original kernel truth. Keep **numeric option6** neutral while reconciling names and provenance; do not relabel option9 or discard capability flags on the basis of the extracted stub. No result value is adopted.

### Bounded next deliverable: corrected contract and an unconnected query fixture

Primary may prepare **one standalone, unconnected option6 component** using explicitly supplied synthetic DWORD inputs, after correcting the contract/provenance. It is not permission to satisfy the title's unsupported kernel call yet. Requiring a matching physical console is unnecessary for an honestly labeled owned fixture; it remains necessary evidence for any later exact-original settings/fidelity claim.

- Separate observed call words, expected caller/static facts, public-reference semantics and proposed synthetic inputs. `describeKernelService(ordinal)` currently hardcodes option6, Result address and `writable:true`; those are not observations for every ordinal2 call. Keep four actual arguments separate, select option-specific metadata from those arguments or label a known-call-site expectation, and obtain output permissions from real VM admission rather than a constant descriptor. Schema checks must not require a false permission/option claim.
- Support only the evidenced query shape `(0,6,0,Result)` in the standalone fixture. An explicit caller-supplied synthetic DWORD is test input, never an implicit HDTV/NTSC/60Hz fallback or a value derived from the browser display. No non-query options, mode switch, register writes, encoder state or clock.
- Acceptance against the existing real-VM fixture: absent configuration and unknown option/parameter stop unchanged; deny null/out-of-range/read-only output and unsafe spans before mutation; a valid configured output writes exactly four bytes, with neighboring memory unchanged. Use two explicit input values to show no fixed fallback. Any alignment policy must follow supported contract evidence, not a guessed Xbox fault. Do not claim full original ABI merely from this host-side component test.
- Keep the kernel binding unsupported and the guest CPU/stack unchanged. Do not synthesize EAX/NTSTATUS success for a VOID service. Submit the corrected option/input contract, component and denial/unchanged-state evidence for review **before** a future opt-in browser wiring task.
- No new generic report framework, frontier export batch, settings subsystem, complete encoder, consumer or renderer. Retain original-content locality, all memory/resource guards and canonical/reset/display controls.

This is the narrow source-owned prerequisite while option provenance and original settings are unresolved; it does not claim the original query has executed.

## Retained A017 boundary task

November 4, 2026. Architect review of source `ccb53cb84496bb1a3b8633335a5b75e26ee66b1c`. This is curated source/checkpoint review; no original game, private module or raw report was accessed or executed here.

## Established and reported boundary

- The builder reports that narrowly admitting `0x1FCB00`, then reached `0x1FD42F` and its direct dependency `0x1FD410`, advances the reset-profile browser run to unsupported kernel ordinal **2**, `AvSendTVEncoderOption`, target `0xFE000094`, return `0x1FD429`. The original helpers are reported executed; the unsupported kernel service is **not** implemented or completed.
- Curated translation: 418 functions / 17,353 instructions; module SHA-256 `6bc97c4dab8f09f2b2135dd3042ab064492fbebaec97710bca870c2dde9d0629`; reset-profile boundary 279 routines / 404 kernel operations. These are lane-reported synthetic-profile startup facts, not independently replayed title execution, rendering or hardware fidelity.
- The checkpoint leaves the actual numeric option and encoder semantics unknown. A caller labeled display-capability lookup does not establish an option's meaning, output or lack of side effects.

## Public declaration checked by the architect

[`XboxDev/nxdk`, pin `004fefbfe8fd93a5ea8ee9a95ff187b0f58ec8ed`, `lib/xboxkrnl/xboxkrnl.h:4462–4468`](https://github.com/XboxDev/nxdk/blob/004fefbfe8fd93a5ea8ee9a95ff187b0f58ec8ed/lib/xboxkrnl/xboxkrnl.h#L4462-L4468) declares `AvSendTVEncoderOption` as **VOID NTAPI**, with four parameters: `RegisterBase`, `Option`, `Param`, `Result` (output ULONG pointer). This is a pinned public declaration, not original source, proof of the title's exact kernel ABI, or option semantics. Do not fabricate an NTSTATUS/EAX success return for a void declaration; corroborate the actual call/cleanup convention with retained caller evidence before implementation.

## Next primary task: one read-only call contract

1. Reuse existing stopped-call stack arguments, records and retained caller analysis at `0x1FD429`; do not extract the title again or expand unrelated translation. Record the four actual argument words and numeric option/parameter, with caller/return/module/profile identity and the full experiment list. Prefer existing bounded observations over a new generic reporting framework.
2. Corroborate ordinal/ABI/argument order and option identity against a pinned public reference compatible with the observed call. Distinguish the modern declaration from the original build; identify any remaining mismatch instead of guessing.
3. Establish Result nullability/width/alignment/writable range, allowed output bits and the caller's consumption, and whether Param is a scalar or carries option-specific addressing. Do not dereference unvalidated guest/device values as host pointers. Capture is read-only and must leave registers, stack, guest memory and device state unchanged except existing diagnostic accounting.
4. State exact supported semantics, hardware/settings/profile inputs and side effects, or name the missing evidence/input. Host browser display properties, a helper name, an arbitrary AV pack/capability value or an assumed 60-Hz mode are not original-console evidence.
5. Keep ordinal 2 explicitly unsupported while semantics are unknown. Return the curated contract and owned observation/permission checks for bounded review; **no encoder implementation, output fabrication, success stub, timing source or mode switch is authorized by this step**. Preserve canonical/reset/display controls and every byte/decoder/layout/ABI/memory/local-content guard.

A later narrowly evidenced read-only query may be a feasible service milestone, but only after this option-specific contract exists. No full encoder, renderer, scanout clock, IRQ model or frontier batch follows automatically.

## Separate evidence correction: PFIFO report snapshot

`ff_probe_pfifo_status_word` exposes the initialization seam, which is copied into the device aperture once and is not updated by subsequent guest writes. Its `dmaPush:0` and `consumerUpdates:0` fields are **initialization snapshot values**, not live PFIFO state or end-to-end consumption evidence. Guest DMA_PUSH writes live in `device_registers` instead. Relabel the snapshot consistently in the worker, validator/tests and docs (or explicitly separate it from any actual live aperture values); do not enforce initial LOW_MARK as a universal final-state invariant. Tests over synthetic report objects check schema, not that the original probe observed the claimed state. This correction is bounded to the existing fields and requires no general evidence subsystem.
