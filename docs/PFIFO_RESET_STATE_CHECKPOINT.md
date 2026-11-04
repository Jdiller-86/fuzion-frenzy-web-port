# PFIFO LOW_MARK has a reset producer in the pinned full reference

## Current progress: source 12f4cf6, A016

The builder has implemented the default-off `pfifoReset` initial-state experiment and reports passing the LOW_MARK wait to a status2 untranslated boundary at `0x1FCB00` (caller `0x1FD4A8`, return `0x1FD4C4`). Canonical PMC and the engine-wait control retain their prior stops. This is lane-reported same-module synthetic-profile startup, not an architect-run title test or execution of the untranslated body.

Actual startup wiring initializes a fresh seam, applies the projection once and copies two initial values into the aperture. It does not read/advance GET or PUT or consume commands. GLM component review supports this scope, and the architect directly verified the probe ordering. The helper's OR semantics **must not be presented as a general reset of dirty state**: restrict its stated contract to immediately following zeroed `ff_pfifo_status_init`, and retain the separate reference RUNOUT read semantics. Before complete A015 acceptance, add the already-requested owned U8/U32 aperture consistency assertion and document all flags in a control pair differing only in `pfifoReset`. Identical call traces are not a general memory-equality proof. These are narrow contract/coverage corrections, not a request to build a reset subsystem.

Independent candidate checks pass: publication guard (328 files), runtime133/133 with no skips, native14/14 plain/sanitizer runs, evidence4/4 suites. Node24.22.1 differs from exact pin24.14.0. Full source branch is not integration-approved. Next admission is only the actually reached `0x1FCB00` and demonstrably necessary bounded dependencies from retained seeds/project; no speculative closure/frontier batch. Preserve the controls and every original-execution/local-content guard.

## Retained reference evidence and A015 task

November 4, 2026. Source review, not original execution. This corrects source `897e389`'s producer-free contract: a non-empty submission is **not** the only reference-supported route to `CACHE1_STATUS.LOW_MARK`. No adapter or runtime change is included here, and there is no controllable original screen.

## Independently verified reference path

Public xemu pin: `dc10b4cea96e403d30c17ed53551fdaf023bc17f`.

- [`hw/xbox/nv2a/nv2a.c`](https://github.com/xemu-project/xemu/blob/dc10b4cea96e403d30c17ed53551fdaf023bc17f/hw/xbox/nv2a/nv2a.c#L289-L337): `nv2a_reset` clears the PFIFO register array at line 307, then sets `NV_PFIFO_CACHE1_STATUS_LOW_MARK` at line 317. Thus the reference's post-reset cache status is **0x10, not zero**, before processing any method. The OR operation at line 317 follows the array clear; it is not evidence that unrelated pre-reset register values survive reset.
- Same file lines 387–390: `nv2a_reset_hold` calls `nv2a_reset`. Class initialization registers that callback in the reset hold phase at line 614. This establishes a reset implementation path, not a live reset observation.
- [`hw/xbox/nv2a/pfifo.c`](https://github.com/xemu-project/xemu/blob/dc10b4cea96e403d30c17ed53551fdaf023bc17f/hw/xbox/nv2a/pfifo.c#L160-L245): lines 170–173 gate the puller on access and LOW_MARK; lines 242–245 set LOW_MARK after positive method progress. That latter write is **not the only producer**.
- Same file lines 299–305: GET==PUT stops the pusher without reading a word; lines 339–341 clear LOW_MARK before dispatch. An empty queue does not erase the separately initialized reset status.
- Same file lines 51–52 returns RUNOUT_STATUS.LOW_MARK. This is reference behavior, not a measured hardware reset value. DMA_PUSH.STATE is distinct from suspended STATUS and remains stored state in the reference.

The architect fetched these exact public files and checked the reset body/callback/registration, and an independent GLM reviewer corroborated the pin and paths. No retained private artifact, title bytes or game-derived pseudocode was accessed.

## Consequence for the source contract

Source `897e389` correctly records an empty-ring profile and no demonstrated first dispatchable method, but its statements that the reference reset status is zero, that LOW_MARK only follows consumption, and that the reference must wait on this predicate with an empty ring are false. Its conclusion should distinguish **reset initialization**, **later consumption transitions**, and **unknown original hardware state**. The original title trace still does not prove an actual dispatch, and the reference reset does not establish hardware fidelity or a real display clock.

The missing reset rule is a credible, source-supported explanation of the artificial wait. It justifies a bounded **opt-in reference-reset experiment**, not forcing completion during polling. A014's prohibition on arbitrary empty-ring completion stays intact; A015 permits only the specifically evidenced initial-state rule under explicit provenance and controls.

## Authorized next task: primary lane, bounded adapter

1. Correct `PFIFO_ENGINE_HANDOFF.md`, status and decision wording, including the stale claim that an unestablished third engine lane owns the work. Primary remains the owner.
2. Reuse the existing PFIFO status seam. Add an explicit reference-reset configuration/initializer for only its owned state, with provenance to the public pin. Do not change the default zero/unsupported behavior or claim to implement full NV2A reset. Initialization happens before guest execution or at a separately evidenced reset event, never after a poll budget expires or because GET==PUT.
3. Owned fixture acceptance: after explicit initialization, cache LOW_MARK is set, DMA_PUSH.STATE is clear, the reference runout read rule is separate and explicit, consumer-update count is zero, and GET/PUT and command memory are unchanged. Verify repeat initialization and the seam's reset scope; do not carry unrelated pre-reset register bits into a claimed reference reset. Byte/word observations of the modeled register must agree. Unsupported modes/addresses and resource limits remain fail-closed.
4. Preserve the existing non-opt-in control. If retained local inputs are available to the primary lane, compare identical modules/profiles with only this reset option differing (the same declared display-event experiment, if needed, in both). Report whether the predicate releases and the next genuine stop, with transition provenance. No new original submission, method, consumer progress or GET advancement may be invented. Capacity stops are not engine progress.
5. Keep canonical PMC fail-closed. Front-loaded PCRTC pending state, causal scanout timing, CPU IRQ delivery and rendering remain separate unproved contracts. Undefined PFIFO +0x500/+0x80 accesses gain no inferred semantics from this reset finding.

No frontier batch, additional generic evidence tools, full emulator, topology expansion or changes to global settings/runners are authorized. If the narrow reset experiment fails, retain its evidence and stop at the next named dependency instead of enlarging the model speculatively.
