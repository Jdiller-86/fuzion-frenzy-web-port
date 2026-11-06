# Runtime execution goal and acceptance gates

This is the public runtime milestone definition, retaining milestone IDs used in the historical records. [The roadmap](../DEVELOPMENT_PLAN.md) owns the full product scope. [Current status](PROJECT_STATUS.md) owns the exact newest observation; [handoff](DEVELOPMENT_HANDOFF.md) points to it.

The selected route remains static recompilation to WebAssembly. Original instructions/data and derived output stay player-local; no native helper or streaming can satisfy browser execution. These are future acceptance gates. Implementation is paused pending the next user objective; old process-recovery, cadence, and lane directives are retired.

## Evidence and recorded acceptance

Every runtime advance retains **two named browser reports (control and experiment) plus an independent audit JSON**. Report relevant profile inputs, exact stop/status, counters, observed service/call effects, and content/module identities. Keep original bytes, generated modules, raw reports, RAM, exports and private artifacts local; public docs contain curated non-content findings.

A milestone closes only with its required evidence and an explicit decision/acceptance record. Static analysis, source fixtures, compiler output, synthetic device storage, and a later stop are not rendering or playability. Do not infer hardware semantics from a compatibility model.

M6 is recorded met in decision D429, citing commit `ad0bffa`. The source-owned `AvGetSavedDataAddress` handler consumed a missing one-dword return address, preserving the control boundary and moving the experiment beyond `0x6B6A6968`. Earlier translator/pop-order diagnoses were superseded. The raw reports and independent audit are not distributed in this clone, so this page reports recorded acceptance rather than a new independent rerun. Legacy IDs were renumbered; cite subject and commit with the ID.

## Runtime milestones

| ID | Acceptance gate | Recorded position |
| --- | --- | --- |
| M1 — Device-table startup | Pass the reached device-table boundary under the recorded device policy, with original calls/writes evidenced | Recorded complete within bounded models |
| M2 — Device policy | One explicit read-back/unknown-read/width/bounds policy, at least ten provenance-backed events, two further boundary advances, decision and passing checks | Recorded met for the synthetic policy; no NV2A semantics implied |
| M3 — Graphics return | Reached graphics cluster returns through its caller chain and the next subsystem request is observed in both profiles | Unaccepted |
| M4 — Presentation request | Original code issues a presentation-target-shaped storage request and its result is recorded in both profiles | Unaccepted; no output/rendering inference |
| M5 — Controllable screen | Original title/menu code visibly executes and changes state from input in a browser | Unaccepted; closes only under M10's stricter evidence |
| M6 — Stack/return repair | Named evidence-backed repair, control invariance at its own stop, experiment progression past the old fault, and exact next stop with required reports/audit | Recorded met by D429/`ad0bffa` |
| M7 — Original output write | Original commands produce a deterministic write to a named output-surface-shaped window, with exact address/size and before/after known provenance; matching evidence in both profiles | Unaccepted |
| M8 — Sustained frames | Eight consecutive original-program frame-equivalent cycles (or a recorded bounded equivalent), at least one M7-class write/cycle and no new stop/fault; both profiles | Unaccepted |
| M9 — Audio/control boundary | Original-program audio or controller event admitted under bounded reference-backed rules, with no invented samples/assets; both profiles | Unaccepted |
| M10 — Controllable original screen | M5 screen and M7/M8/M9, plus a bounded input sequence demonstrably changing it, with both profiles and the required audit | Unaccepted |
| M11 — Runtime coverage | Every recorded group (memory/aperture, audio, controllers, graphics pipeline, scheduling) has a reached-and-accepted or evidenced not-yet-reached disposition; both profiles and audit | Unaccepted; not-yet-reached is a coverage limit, not proof of complete compatibility |
| M12 — Product completion | Original program-defining scopes, offline play/data/conversion, full local-content parity, browser execution, and all existing online/social/custom requirements accepted | Unaccepted; adds no new scope |

M7 and M9 depend on M6; M8 depends on M7. M10 requires M7 and both M8 and M9 (the prior wording “at least two of {M8, M9}” requires both). M11 depends on M7–M10; M12 depends on M11 and the complete product requirements.

M6's control intentionally stops before the experiment's former fault. Preserve its own stop/counters and demonstrate experiment progression; requiring both to pass that fault would contradict the recorded protocol. Future gates requiring both-profile observations must name the applicable profiles and evidence without silently relaxing criteria. No currently missing screen/output gate is accepted by this clarification.

## Evidence and contribution method

Keep one current checkpoint linked from handoff line 1, and update status/handoff together when the execution boundary changes. Archive superseded observations rather than accumulating contradictory current summaries.

Work in bounded, reviewable tasks with a functional result, explicit acceptance, current exact stop, and a stop condition. Preserve the actual pipeline's byte/decoder/layout/memory and resource-admission guards. Unsupported services/registers/resources must stop explicitly. Review material CPU, memory, ABI, and GPU changes independently and use targeted checks.

Separate component progress, observed runtime advances, and product acceptance. The [contribution guide](../CONTRIBUTING.md) describes review and publication. No project agent scheduler or model routing is configured, and no implementation task is assigned.

## Analysis boundary

Use read-only retained local XBE/Ghidra evidence for a named unresolved native fact. Follow [analysis setup](ANALYSIS_SETUP.md) and the applicable REA skill/provider boundaries. Record Evidence identity, digests, provider and tool; direct Ghidra output and derived translator text are different provenance. Neither substitutes for byte/decoder/layout/memory admission checks or actual browser execution.

Unsupported provider access or missing private inputs must be recorded as limitations of the assigned task. Never acquire firmware/game downloads, invent evidence, publish original-derived output, or repeat a closed historical repair investigation.
