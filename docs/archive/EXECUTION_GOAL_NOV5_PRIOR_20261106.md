> Historical snapshot from November 6, 2026. Its automation, routing, cadence, and lane directives were retired when project automation was removed in commit `55ea550`. No scheduler or model routing is configured. See [current instructions](../../AGENTS.md), [runtime milestones](../RUNTIME_MILESTONES.md), and [current status](../PROJECT_STATUS.md).

# GOAL — Restore full-cadence original-execution progress (in effect from Nov 5)

Owner directive (Nov 5, user): recover the Nov 1–2 development tempo, fix execution structure, add a second full-time DeepSeek v4.1 Flash builder, adopt concrete milestones with success criteria, and clean up branch/PR naming. This section supersedes conflicting lane-allocation, model-pinning and hold rules in the earlier goal text. Every evidence, source-only, local-content, CI and no-false-playability guard remains binding and unamended.

## Confirmed state to build from

Current checkpoint: `docs/DEVELOPMENT_HANDOFF.md` line 1 and `docs/PROJECT_STATUS.md` — the runtime lane's boundary is a guest read fault in graphics initialization's free-framebuffer path: after the contiguous free and `CDevice_FreeFrameBuffers`, a helper loads a pointer field from the device-context object and dereferences it, and the field holds palette-ramp bytes (`0x68,0x69,0x6A,0x6B` = "hijk", from `FUN_001fdf83`) rather than a pointer, so the derived address is unmapped. Observed stop: status 10 at `0x6B6A6968`, 346 routines / 414 kernel operations (module 150 / profile 87). The faulting object is not the static device, and the ground-truth arbiter shows the register genuinely held the observed value at the read, so the capture/instrument question is settled and the open one is which executed path produces that value; no model, fix or translator edit is warranted yet. Next original task: obtain the decisive capture verdict or a blockers package with evidence. No renderer, no playable original screen.

Superseded-but-do-not-regress: FATX utility-mount (317 fn / 10,612 instr / ABI 22) and MmQueryStatistics boundaries are done, retained and never cited as "current". The earlier pending device-write decision at `0xfd009140` is superseded by lane evidence: the bounded inert aperture policy (D063/D064) resolved that computed write and both profiles advanced far past it, through the present/AV service path to the current stop. The FATX aperture and storage rules, D060 WBINVD condition and every audit format remain as-is.

## Definition of recovered speed (success metric)

- Baseline: Nov 1–2 delivered ~24 runtime checkpoint advances in ~48 h (≈12/day) on one lane.
- Target from M1 onward: a rolling-7-day average of **≥8 runtime checkpoint advances per day** across active lanes. "Advance" = a lane commit whose checkpoint records a new observed stop boundary **or** converts a reached-but-unserviced request into a serviced one with both-profile evidence.
- Orchestration budget: ≤2 non-runtime commits per lane per day unless a defect forces more.
- Measurement lives in `docs/PROGRESS_METRICS.md` (rule R7).

## Development milestones and criteria for success

Evidence-gated throughout: a milestone is closed only by the observed facts in its checkpoint and reports. If a milestone slips, the delay and reason are recorded in `DECISIONS.md` — the criteria are never loosened and no requested result is invented.

### M0 — Recovery reset (complete within day 0; ≤4 commits total)
1. `AGENTS.md` stage block and `DEVELOPMENT_HANDOFF.md` header cite only the current checkpoint with the exact numbers above (344 fn / 12,878 instr / ABI 24; stop `0x1fe085` return `0x1fe5bd`; decision `0xfd009140`). Check: no superseded boundary remains labelled current.
2. Status repair: `PROJECT_STATUS.md` truncated to current state + last milestone + metrics pointer; all superseded sections moved to `docs/archive/`. Handoff keeps only the current task block.
3. Naming scheme live (see Naming): all new branches `lane/*` or `feature/*`; every `slice-*`, `sync-*`, `check-*` branch and its worktree deleted or renamed this cycle, recorded in the metrics log.
4. Runner supports `-Lane runtime-b`: new worktree `.worktrees/opencode-runtime-b` forked from the current runtime-a head, own lock/session pin/log; the Windows task (`FuzionFrenzy-OpenCode-CycleB`, 5-min cadence offset from the primary) is created or its creation steps are handed to the user; one verified runtime-b cycle observed end-to-end.
5. `docs/PROGRESS_METRICS.md` created (R7 format).
Acceptance: all five verified by direct file/ref inspection at the next architect cycle, in one coherent commit plus the runner/task change.

### M1 (completed by lane work, recorded) — graphics initialization executes past the device-table boundary with device services modeled
- Scope: admit + translate reached `0x1fe085` and its immediate dependencies under the batch rule (R3); decide device write `0xfd009140` as a recorded D-number policy consistent with the aperture's no-fabrication rules; run both profiles.
- Criteria (all required): the stop moves from `0x1fe085` to a new address/service in both profiles; audit JSON with zero source-identity mismatches; `npm run check`, `npm run test:runtime`, `python tests/title_seed_test.py` and owned fixtures pass; commit `M1: …` on `lane/runtime-a`; status paragraph ≤120 words; published with green CI.
- Stop condition: an unsupported register/service/resource — record the stop exactly as observed; do not supply values.

### M2 — Device access governed by one recorded policy (target day 1–2)
- Graphics-init device accesses are governed by one explicit recorded policy (read-back of known-written addresses, unknown-read faults, width/bounds rules) instead of per-address exceptions. No NV2A behavior, DMA, interrupt or rendering semantics may be inferred; the D060 condition stands.
- Criteria: ≥10 admitted device events with `beforeKnown` provenance in reports; two additional boundary advances; the policy decision recorded; checks green.

### M3 — Graphics initialization returns and hands control to the next subsystem (target day 2–5, evidence-driven)
- Original code completes the reached video-mode/graphics cluster (the cluster containing `0x001f7650`, `0x001fbb00`, `0x001fe05a`) and issues the first request of the next subsystem.
- Criteria: cluster calls return through the caller chain in both profiles; the next-system request is observed in reports; checkpoint updated; checks green. No rendering-output claim may be made.

### M4 — First presentation-target boundary (evidence-driven, no date promised)
- Original code issues a first call targeting presentation-shaped storage (backbuffer/framebuffer-class) implemented under the same no-invention storage rules.
- Criteria: the observed request and result appear in both profiles; docs describe it exactly as that and nothing more.

### M5 — Controllable original screen (first allowed "runs in browser" claim)
- Original title/menu code visibly executes in the browser and demonstrably changes state from player input.
- Criteria: browser evidence including an input sequence; only here may the claim "the original game executes in the browser" be made, scoped to what is demonstrable. Everything wider (full runtime, audio, netcode, custom features) stays explicitly unfinished.

Cadence floor: if 48 h pass with <2 runtime advances after M0, the architect must record a corrective decision and re-point lanes — not relax criteria.

## Builder lanes (replaces the three-lane distinct-model allocation)

| Lane | Branch | Model (primary → fallback) | Schedule | Owns | May not touch |
|---|---|---|---|---|---|
| runtime-a | `lane/runtime-a` (carried from `codex/opencode-source`) | `opencode-go/deepseek-v4.1-flash` → free deepseek → free GLM Flash | 5-min cycle (existing) | seed exports, admission/prep pipeline, translation batches, boundary advance, renderer-entry checkpoint, `PROJECT_STATUS.md` top | runtime service implementation and fixtures owned by runtime-b |
| runtime-b | `lane/runtime-b` (fork of runtime-a head) | `opencode-go/deepseek-v4.1-flash` → free deepseek → free GLM Flash | 5-min cycle, offset start | runtime service implementations for reached-but-unserviced requests/ordinals, service fixtures, service checkpoint, then geometry/verification backlog when idle | admission/prep pipeline, lane-a checkpoint files |
| review | (no branch) | Go GLM-5.3-Flash subagent | per batch | independent bounded review of CPU/memory/ABI/GPU diffs | — |
| integration | `lane/integration` (carried from `codex/opencode-architect`) | architect | hourly | merges, PRs, metrics, directives | — |

- The geometry and verification lanes are dissolved: their retained source-owned components and tooling become backlog tasks owned by runtime-b when it has no serviced-request work. Their worktrees/branches are closed in M0 after review of retained source.
- Same-provider correlation risk is accepted by owner decision; mitigations: offset schedules, independent fallback chains, and a stated DECISIONS.md entry recording the user's direction. A provider-wide outage degrades both lanes to the fallback chain the same day rather than idling.
- Conflict rule: first conflict between runtime lanes on one file → the integrator re-partitions ownership and records it; second conflict on the same file → that file gets a single owner.

## Naming scheme (feature-based; owner directive Nov 5)

- Long-lived branches: `main`, `lane/integration`, `lane/runtime-a`, `lane/runtime-b`. Nothing else may persist.
- Feature-scoped branches are short-lived: `feature/<functional-slug>` (e.g. `feature/device-policy`, `feature/osd-input`), deleted in the cycle their PR merges or is closed.
- Commit subjects name functional results: `M<n>: <functional result>` for runtime advances (e.g. `M2: device access governed by one recorded policy`), `ops: <chore>` for orchestration. Hex addresses, register names and decision IDs stay inside commit bodies, PR Evidence sections and checkpoints — never in commit subjects, PR titles, branch names or milestone labels, except where the named original routine itself is the readable feature name.
- PR titles: review PR `[runtime-a] <functional slug>: <summary>` → `lane/integration`; integration PR `[integration] <functional slug>: <summary>` → `main`. Body template: Goal / Changes / Evidence (report IDs and the stop before→after — technical addresses allowed here) / Checks run / Limits. PRs touching CPU, memory, ABI or GPU code carry a posted review pass before integration.
- Task cards: one functional objective per card, a human-readable description of the current boundary, and one technical evidence line quoting the exact checkpoint stop (status + address) for precision. A card whose target can only be understood by looking up hex fails this rule.
- Legacy `codex/*` aliases are frozen; rename via GitHub UI only when no open PR references the branch.

## Operating rules (structure of execution)

R1 One current pointer: "current" = the checkpoint named in `DEVELOPMENT_HANDOFF.md` line 1. Stage/handoff sync happens in the same commit as the advance it describes.
R2 Status ring buffer: `PROJECT_STATUS.md` stays ≤ ~60 current lines; demoted sections move to `docs/archive/` instead of accumulating.
R3 Default work unit: one batch of ≤8 related reached functions / ≤2,000 instructions + its control/experiment comparison + both-profile run — the Nov 1–2 checkpoint rhythm, not finer slices.
R4 Max two commits per lane per cycle; no preflight/manifest/validator/recap-only commits.
R5 Review per batch, scoped to CPU/memory/ABI/GPU diffs; no blanket audit campaigns without a related change, regression or unresolved risk.
R6 Anti-drift: ≥3 consecutive non-source cycles in any lane → the next cycle must be source work or an evidence-backed HOLD in `DECISIONS.md`; every task card names the functional objective and carries one exact-stop evidence line (status + address) for precision.
R7 Metrics: one line per lane commit in `docs/PROGRESS_METRICS.md`; the architect posts a weekly rollup; <2 runtime advances/48 h triggers a mandatory corrective decision.
R8 Allowance fallback: same-cycle retry on the verified free route (as today); after 2 recognized failures per lane per day, the lane runs the rest of the day on fallback and returns to DeepSeek after a verified success.
R9 Integration: the architect evaluates the current `main` PR head before editing its branch; merges only exact approved heads through the PR gate suite (source, runtime, CI, ancestry, mergeability); one integration PR per milestone sub-step or day.
R10 No new persistent branches; short-lived `feature/*` branches are deleted in the cycle their PR merges or is closed; no force pushes; no rewriting published history.
R11 Reporting unchanged: every runtime advance ships two named browser reports and an independent audit JSON exactly as today; "current" docs cite only the newest audit.
R12 No runtime-engine redesign under this goal: changes orchestration, naming and structure; the translation pipeline's admission rules change only through their own evidence-based decisions.

## Guardrails unchanged (_apply to every item above)

Static recompilation to WebAssembly; original content player-supplied and local; no original ROM/asset/pseudocode in the repo; source-only publication gates; both-profile browser evidence for runtime claims; no playability claim without a controllable original screen; exact-head PR requests; no Sol routing; the hourly architect is Go GLM-5.3-Flash with full Go GLM-5.3 only on recognized failure.

## Per-cycle architect checklist

1. Read `AGENTS.md` → `DEVELOPMENT_HANDOFF.md` line 1 (current checkpoint) → the top checkpoint → `DECISIONS.md`.
2. Compare builder outcomes with the prior directive; issue ≤3 tasks (runtime-a, runtime-b, review/integration), each with a bounded result, acceptance check and stop condition; redirect drift with evidence.
3. Evaluate the open `main` PR head on its merits; request a guarded merge only when fully reviewed; delete satisfied `tmp/*` branches.
4. Append the metrics line; verify milestone statuses against their criteria.
5. When stalled, name the exact blocker, re-point the lane, and log the decision — never broaden scope to fill a cycle.
- Integration with the PR line: when the existing main PR head does not contain the newest reviewed local-main chain, merge that chain into the integration branch first, applying the renumbering and regex rules the latest D-number records; never force-push; only then evaluate/re-request the main PR head.

## Hard completion milestones (Nov 5 owner authorization)

Every milestone below closes ONLY on evidence: (a) the evidence formats R11 names (two named browser reports + an independent audit JSON), and (b) an explicit records row citing that evidence. Lanes reach; the architect accepts and records; the translator owner decides repairs — currently the project architect, exercising the user's standing Nov-5 delegation ("decisions that genuinely improve development speed and quality"), revisable by the user at any return. A gate cannot close on documentation alone; no gate may be read as evidence the records do not name; and closer gates do not shorten the STOP address quotation rule R6 — the card rule now requires each task card to name BOTH the current target milestone ID and the current stop address.

Lane cards and hourly architect reviews from this date MUST name the current target milestone ID alongside the stop address. Owner absence never holds a lane: the lane continues per-cycle work toward its current milestone's blocked-by prerequisite, and the architect re-points cards when the chain departs from reality.

M1..M4 stand as already-worded in the `## Development milestones and criteria for success` section above; M5's gate now closes on M10's stricter evidence. The tempo ledger (`docs/PROGRESS_METRICS.md`, R7) remains independent of milestone acceptance, except where an acceptance itself IS a new stop boundary.

### M6 — runtime-epilogue mismatch resolved
Gate: a DECISIONS row (the translator owner's choice, recorded under the standing Nov-5 delegation) names the chosen repair for the confirmed `FUN_001FB640` callee-saved pop-order mismatch — translator emission side vs `.inc` expansion or an equivalent evidence-shaped variant — AND the subsequent original stop moves in a browser-verified advance to an address other than `0x6B6A6968`, with the R11 evidence set. Owner: translator owner. Blocked by: runtime-a's composed blockers package (its pending record row must land and be renumbered at integration).

### M7 — first original-program output at an output-surface-shaped window (the "first pixel" gate)
Gate: under the modeled aperture rules (D063/D064), the original program's own commands — not the runtime's defaults — record a deterministic write into an output-surface-shaped device window the records identify (framebuffer/PCRTC-class), with before/after known-provenance; the R11 evidence names the window, the address and the write size; both profiles agree; no invented value. Owner: runtime-a. Blocked by: M6.

### M8 — sustained frame boundary
Gate: the modeled runtime sustains eight consecutive original-program frame-equivalent cycles (or an equivalent bounded loop the records name) in the browser reports with no new stop or fault, at least one M7-class write per cycle; both profiles. Owner: runtime-a. Blocked by: M7.

### M9 — audio/control boundary reached
Gate: a recorded audio or control device event from the original program's own commands admitted under the bounded referenced D-rules (no invented samples, no pre-fetched asset), with both-profile R11 evidence. Owner: runtime-a. Blocked by: M6 only — the original program's own ordering decides whether M8 or M9 comes first, and the records name it when it happens.

### M10 — controllable original screen (M5's stricter evidence)
Gate: the M5 screen exists AND a recorded bounded input sequence demonstrably changes it in the browser reports (input-driven screen transition), both profiles, R11 evidence. Owner: runtime-a with runtime-b's committed components available. Blocked by: M7 AND at least two of {M8, M9}.

### M11 — full runtime
Gate: the modeled runtime has an accepted answer for every runtime-lane group recorded (memory/aperture, audio, controllers, graphics pipeline, scheduling) — each group either reached-and-accepted or recorded as not-yet-reached with the evidence to know; both profiles, R11. Owner: translator owner + runtime-a. Blocked by: M7–M10.

### M12 — the product (the corrected original-game completion)
Gate: the original game's own program-defining scopes (offline play, data, conversion, the full local-content parity list from `docs/SCOPE_AND_FIDELITY.md`) satisfied in the browser with R11 evidence. Blocked by: M11 and the project's existing online/social/custom-feature plan records — this goal's ladder stops at M12 and creates NO new scope.

## REA usage for the recompilation effort (Nov 5, owner direction)

Within the recompilation effort, the `reverse-engineer-anything` (REA) skill/tooling may be used for read-only native analysis of the retained local original XBE (`research/reference/default.xbe`) or through the retained local Ghidra server/project (`research/ghidra-projects/FuzionFrenzySetup.gpr`), as described in `docs/ANALYSIS_SETUP.md`, for owner-directed questions the retained source-owned records alone cannot answer. REA invocations keep the project's local-content boundary: no original content committed, served or uploaded through REA; any use is recorded in `docs/DECISIONS.md` with its Evidence identity (Evidence IDs, digests, provider, tool); REA output is never substituted for the pipeline's byte/decoder/layout admission rules. A REA failure or provider unavailability is recorded as a factual entry, is never a reason to invent evidence, and does not block the lane from other found work.

For the runtime-a candidate (B) attribution question specifically (Nov 5 ~21:10 refinement): REA's static native-provider layer is the going oracle for ORIGINAL-BINARY facts at the implicated code, because the pipeline's own `.inc`-mirrored facts are DERIVED records of that binary, not fresh bytes read independently. Specifically, a REA call must name the fact it unlocks before calling: (a) the original XBE's actual prologue push order and epilogue pop order around `FUN_001FB640` (independently checking that the pipeline's `title-functions.inc`-derived record of prologue `ebx, ebp, esi, edi` / epilogue `edi, esi, ebp, ebx` is faithful to the original); (b) the caller-side register path into `FUN_001FC010` (whose entry `ECX`/caller, `EAX`, and `ESI` are the pop-consumption question), verified against the original code to distinguish the runtime's observed record from what the original instruction stream would have produced at those sites; and (c) `compare_functions` pairing the original function's epilogue against the retained `.inc`-translated expectation as a direct textual cross-check, recorded as retained Evidence. REA's browser-facing telemetry (page scenario capture, console/network observation) is a layer ABOVE the register-mapping question: it can surface the modeled browser page's observable behavior, but it does NOT read the modeled runtime's internal register/slot attribution — that oracle is the pipeline's own probe machinery (`siteWatch`/`guestWatch`, the ground-truth ESI arbiter). A REA call made without a named fact, or claimed as a substitute for the local-content or admission rules, is recorded as a factual REA-rejection entry and is never used as pipeline evidence.
