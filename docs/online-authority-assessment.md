# Online authority assessment

## Decision status

**Unresolved; source-only assessment, not an implementation decision.** The product requires original browser execution, player-local original content and state, and server-owned match outcomes. Current evidence does not establish a route satisfying all three. Do not start accounts, lobbies, or netcode until a bounded feasibility spike produces the pass/fail evidence below and the coordinator records a decision. This assessment is not a new milestone gate and does not block unrelated M1 work.

The public architecture has no production server, database, account system, or online-match service. Existing browser startup evidence demonstrates only bounded execution with a prepared local module; it does not demonstrate online execution or a shared authoritative simulation. The pinned native SDK/title analysis in [research](research.md) supports bounded runtime and service claims, not an independently reproduced title network capability or absence thereof. Any status statement that the native title has no network support remains a claim to verify, not a finding established by this assessment.

## Requirements that must hold together

| Requirement | Necessary property | Evidence now |
| --- | --- | --- |
| Original browser execution | Each browser runs the compatible original title through the selected route. | Bounded startup only; complete browser gameplay is not accepted. |
| Original content and state stay local | Original files, extracted assets, generated title code, RAM, and original-containing saves are not uploaded, relayed, or hosted. | Required content policy; current prototypes do not implement online play. |
| Server owns outcomes | The server determines accepted scores, collisions, elimination, pickups, randomness, and round transitions. | Product requirement only; no shared authoritative simulation is proven. |

These are simultaneous requirements. Readiness identifiers and protocol metadata do not grant access to another browser's title or state, and a digest or digital signature proves neither ownership nor authority over the underlying content or result.

## Separate input sequencing from outcome authority

An input protocol can identify who supplied an input, its intended tick, and its order. That is **input sequencing**. It does not establish that the server can derive or verify the original title's consequences. An **outcome authority** must decide which game state transition and result are valid. The product cannot call a result server-authoritative solely because inputs passed through a server, were timestamped, signed, or replayed.

The original game rules and internal state fields needed to make that decision are not specified here. Do not invent them or expose guessed memory/state layouts as a protocol.

## Candidate routes

| Route | Fit with the requirements | Current assessment |
| --- | --- | --- |
| Independently authored authoritative rules simulation | Could run server-side without original assets or title state if its rules and inputs are complete and evidence-backed. It must match original outcomes for the supported catalog, including timing and randomness. | Not established. Existing Twisted System rules are explicitly independent prototype choices, not recovered original rules. No parity or complete rule/state contract supports authority today. |
| Original title execution on the server | Could centralize execution and outcomes in principle. It conflicts with the current contract that original code, assets, RAM, and original-containing saves remain on each player's device and are not sent to a server. | Incompatible with the current content boundary; do not implement or treat as a presumed option. Reconsidering the product contract requires an explicit separate decision. |
| Input relay between original browser instances | Keeps title execution and content local and can sequence or distribute inputs. Each browser still computes outcomes from its own execution. | Does not satisfy server-owned outcomes unless a separately proven verification/authority mechanism exists. None is established. |

No route is selected as feasible. A useful decision may be that the requirements conflict for some scope; do not silently weaken fidelity, locality, or authority to make an architecture appear complete.

## Minimal protocol envelope for investigation

This is a candidate envelope for the spike, not an approved wire format or complete protocol. Include only opaque identifiers or values needed to route and compare messages:

| Candidate field | Purpose and boundary |
| --- | --- |
| Protocol/rules version | Identify the reviewed protocol and any independently authored rules version; no version implies original equivalence. |
| Match and event ID | Deduplicate and order match-scoped events without containing title data. |
| Compatibility IDs | Identify claimed title build, converter/runtime build, and rules profile. A digest may compare a locally computed identity; it does not prove possession, rights, or authority. |
| Account ID | Optional service identity; distinct from a browser connection, local profile, and seat. |
| Connection ID | Identify a live transport session; replaceable on reconnect. |
| Local profile ID | Identify a player's local profile where needed; must not imply cloud synchronization of original content or saves. |
| Seat ID and ownership | Identify one of four total seats and whether it is local to a browser or remote. A browser can own multiple local seats. Validate ownership changes server-side. |
| Tick and input sequence | Candidate ordering metadata for inputs; specify duplicate, stale, late, gap, and clock handling in the spike. It does not make outcomes authoritative. |
| Input envelope | Only explicitly reviewed controls/actions and bounded protocol metadata. Do not include title memory, assets, code, or derived game-state blobs. |
| Result/acknowledgment | Carry server decision identifiers and protocol-level status only after an authority route is demonstrated. A client-declared winner is never authoritative. |

Explicitly exclude original RAM or memory snapshots, original-containing save states, game assets or media, executable/module/generated title code, extracted or converted proprietary data, and telemetry or crash attachments containing any of those. Do not send full state hashes as a substitute for local-content privacy or for a state-transition proof.

## Recovery terms are separate

- **Input replay:** Reprocessing retained protocol inputs. It is useful only if the authority implementation and its deterministic inputs/state are known; replaying inputs into separate original browser executions does not prove agreement.
- **Reconnect:** Rebinding an authenticated connection to an eligible account/profile/seat and restoring protocol position. It does not define who owns the lobby or restore a lost match process.
- **Lobby ownership:** Permission to manage a lobby, invitations, and seat assignment. Owner handoff is separate from match execution and result authority.
- **Match-process recovery:** Recovering an interrupted authoritative match process and its durable checkpoint/result. It needs atomic, idempotent persistence and a defined recovery policy; a reconnect alone is not recovery.

No original-containing state may be included in server checkpoints under the current local-content contract. Whether an independently authored, evidence-backed rules state is sufficient for recovery remains unresolved.

## Required feasibility spike and decision record

Before accounts, lobbies, or netcode, assign a source-only spike with a fixed scenario and explicit pass/fail output. Do not use private original content as a network payload or add a server. The spike should:

1. Inventory only evidence-backed rules, inputs, timing, randomness, and observable outcomes for one narrowly named original scenario. Mark every unknown; do not infer missing game fields.
2. Write two independent outcome descriptions from the same declared inputs and evidence, then identify whether a server-side rules model can decide the result without original code, assets, RAM, or saves.
3. Specify the candidate envelope's ownership, ordering, duplicate/late-input behavior, and privacy boundary. Demonstrate that the protocol carries no excluded content.
4. Define pass/fail for outcome authority, local-content preservation, and recovery independently. A pass requires a reproducible, evidence-backed server decision that agrees with original-reference outcomes for the declared scenario while keeping all excluded material local. A failure or unknown on any requirement is not a pass.
5. Report required additional original-reference evidence, uncovered cases, limits, and whether the route is feasible, infeasible under the current contract, or still unknown. Include reconnect, lobby ownership, and process-recovery as separate analyses rather than one generic recovery claim.

Record the result in the project decision/architecture/risk documentation and update the relevant structured task state. A pass authorizes only a separately scoped implementation proposal, not production networking. A fail or unknown returns the requirement conflict and options to the coordinator. The spike is an architecture decision aid, not a new milestone or a reason to suspend M1 or other source-only work.

## Related project contracts

This assessment follows [architecture](architecture.md), [player-supplied content](content.md), [compatibility and fidelity](compatibility.md), [risks](risks.md), and the [roadmap](../plans.md). In particular, account, connection, profile, and seat identities stay distinct; the total is four seats; shared code or fixed ticks do not prove determinism; and score, collision, elimination, pickup, randomness, and round authority remain requirements rather than established capabilities.
