# Ordinal 2: establish the reached call contract before emulating an encoder

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
