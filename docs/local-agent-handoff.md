# Local agent handoff

Use this file to turn the prior process analysis into one bounded implementation task at a time. The coordinator owns sequencing, integration, acceptance, and any publication. Do not launch every goal together or treat this list as approval to implement the whole backlog.

## Master prompt

Copy this prompt into a fresh local coding-agent session, then append exactly one goal from below:

```text
Act as coordinator to implement one appended bounded goal in this repository.
Routine reversible work inside that goal is authorized; continue useful
independent work if a prerequisite is missing. Read AGENTS.md,
docs/handoff.md, docs/status.md, docs/agents.md, plans.md,
docs/m1-work-scope.md, docs/resource-reuse.md, docs/research.md, and the
relevant parts of docs/architecture.md, docs/content.md,
docs/compatibility.md, and docs/risks.md. Read docs/development-workflow.md
for the exact usage of npm run dev:status, npm run dev:preflight,
npm run dev:task, and npm run test:workflow; do not invent flags or usage.
Before original-runtime work, identify the needed evidence by claim and use
the documented purpose-specific private manifest with --require-private.
Unchanged replay uses --purpose replay plus a separately reviewed consumed-input
closure; retained-code candidates also need original-fact provenance and source/
build review. New analysis keeps the default analysis gate and read-only procedure.
Reproduce retained browser cases before candidate execution. A source-only
preflight exit or replay-input pass does not establish new original-code facts.
For M1, also read docs/m1-browser-work.md. Name the affected MB work item
alongside the SCOPE task and use its observable exit check. Report the exact
remaining connection/features and next measurement, with a structural size
revision where supported. Do not answer that all browser execution is unknown.

Detect the latest checkout, structured state, task ownership, and evidence
before acting. The values in older handoff prose (including profile M44's
addresses) are historical until confirmed against current state. Never
blindly apply them after newer work. Preserve in-flight edits, private-input
boundaries, module/admission caps, raw evidence, and focused checks. Do not
claim behavior from static availability, synthetic fixtures, counters, or
component tests alone. Do not invent SDK paths or runtime evidence.

Implement only the appended goal and its explicit owned files. Use existing
source and providers where evidence supports reuse. Keep original game inputs,
generated title code, derived proprietary content, and raw/private analysis
local and ignored. End-user execution and conversion remain browser-local:
do not add a native product helper, content upload, remote conversion, or
distributed generated title code. Existing native developer analysis/compiler
tools may prepare private M1 artifacts; they do not satisfy M2. Stop at the
goal's stop condition or any
unsupported dependency. Run only the task's focused checks and applicable
workflow checks documented by the repository. Obtain the required independent
review for CPU, memory, ABI, or GPU correctness.

Before handoff, update the current structured task/status state and timing
records using the documented workflow. Record what ran, evidence identity and
provenance, measured timing, exact limits, and next stop; distinguish a
checkpoint from a completed functional package. Keep docs/status.md and
docs/handoff.md synchronized when the runtime boundary changes, and update
docs/m1-work-scope.md only if package forecast or package closure changes.
Review the exact source diff and report files changed, behavior, focused
checks, evidence, limits, and the next concrete task. The coordinator owns
staging, commits and PR preparation under the repository's publication guards;
workers return their scoped work. Do not start another goal or schedule an
unattended coding loop from this prompt.
```

## Ordered, bounded goals

Select SCOPE-37 first. The remaining goals are separately scoped follow-ups; they may proceed only when their stated prerequisite is satisfied and the coordinator assigns them. The graphics, resource-read, and admission-scaling tasks are independent investigations after the startup continuation is brought to a coherent stop. M2 and M3 are separate later planning/implementation slices and do not close M1.

### A. Current M1 continuation: SCOPE-37 logger and startup caller

**Prerequisite:** Re-read current status and inspect the latest structured task/evidence state. Confirm the actual stop and that this caller remains unowned. The current docs describe a one-argument logger caller whose existing C-provider guard reports a stack address; these details must be revalidated against current evidence.

**Owned output:** The smallest source/build/harness changes needed to admit the exact reached logger caller, reuse retained formatter/file providers only where caller and format evidence support it, and assess the following supported startup continuation. Keep changes to the assigned runtime/evidence files and required synchronized status/handoff/research records.

**Acceptance:** Exact caller, argument, literal/format classification, and live provider state are source-supported; actual provider execution and return are observed through the existing guarded context; original cleanup and relevant ABI/FP/SEH/ownership checks pass. Preserve the full default comparison and prior CPU/kernel prefixes, all current function/binding/group/resource limits, and focused independent build/harness/browser review. A static body or available provider alone is not admission. Update the timing record with the measured run and evidence identity.

**Stop:** Stop at any unsupported logger format, provider, resource, branch, or failed guard. Do not assume later `0x4EA30` or outer `0x4F5E0` return is covered; use the latest state to identify the actual addresses. Do not create a broader logger or general-formatting contract from this one caller.

### B. Integrated browser graphics beyond the owned-triangle proof

**Prerequisite:** Inspect current graphics work and the existing owned-triangle vertex-assembler proof. Confirm its precise coverage, current GPU/device stop, and whether a new integration task is unowned. Existing geometry prepasses and preview are component evidence, not integrated original command execution.

**Owned output:** One narrowly selected integration seam from original guest graphics command/state handling into the existing browser renderer, with source-owned fixtures or harness support as needed. Name the exact command/state path before editing; do not undertake a renderer rewrite.

**Acceptance:** Demonstrate the selected path through the actual browser integration using independently checkable expected output/state and retain its assumptions and evidence provenance. Keep command decoding, memory ownership, state/depth/arithmetic behavior, synchronization, and unsupported cases explicit. Preserve focused existing graphics checks; arrange independent GPU correctness review. Record measured browser run timing and update structured state.

**Stop:** Stop at unknown command semantics, unsupported formats/state, required readback/synchronization not modeled, or a mismatch. Do not claim original-title rendering, NV2A fidelity, or broad GPU readiness from the triangle proof or preview.

### C. Positive original local resource read using existing XISO/Blob worker proof

**Prerequisite:** Verify the current disc-session/XISO reader and worker proof, device/provider state, and local compatible input availability under the analysis setup. The retained negative `ACCESS_DENIED` open path and zero subsequent data reads do not demonstrate a successful original content read. Do not acquire or copy game content into the repository.

**Owned output:** A single source-supported positive read path that binds the existing local `File`/`Blob`-backed XISO session to the reached original provider and reads only the selected bounded resource range. Keep proprietary inputs and raw output local/ignored; version only source-owned code and curated non-content evidence.

**Acceptance:** The actual original caller reaches the existing worker/session reader; checked offset/length, device/provider ownership, returned bytes and guest-visible destination/return are evidenced against the selected local input. Verify bounds, cancellation/cleanup, unchanged earlier evidence, focused XISO/worker and runtime checks, and record timing/evidence identity. State exactly which bytes were read; a contained executable identity check is not a full image digest or catalog proof.

**Stop:** Stop at unknown resource layout, unsupported error semantics, invalid range, absent compatible local input, or any need to synthesize bytes, use the synthetic cache, or send content off-device. No generic filesystem or successful-open semantics are implied.

### D. Proof-admission scaling at 511/512 functions and 64/64 bindings

**Prerequisite:** Read the latest admission audit and task state. Confirm whether the current module is still at 511 functions and 64 bindings and inspect the exact admission boundary. These counts are limits, not a request to raise them.

**Owned output:** Use the existing `dev:status` headroom report. Inspect the private admitted-body/provider inventory and produce a bounded scaling plan identifying necessary bodies, duplicate C/SDK representations, retained-profile dependencies and candidate slot recovery. If evidence supports one small compaction under existing caps, implement only that compaction and its retention/admission checks; otherwise return a concrete module-partition/interface proposal. Do not modify runtime evidence to manufacture capacity.

**Acceptance:** The plan identifies exact inventories, recoverable slots and tradeoffs from retained evidence. Any implemented compaction reproduces the retained browser cases and all required body/provider/dispatch comparisons under unchanged caps. One-over-boundary candidates fail clearly before unsafe/partial admission, with no hidden truncation or silent cap increase. Focused boundary checks pass and the status/task record identifies exact counts, behavior, timing, and evidence provenance. A proposal without a browser replay remains a proposal.

**Stop:** Stop if safe admission requires expanding a cap, remapping, new exports/imports, or a changed module contract. Submit that as a separate coordinator decision; do not silently raise limits or recompile a broader title.

### E. M2 browser-local title preparation and compile slice

**Prerequisite:** Coordinator assigns an M2 slice after reviewing M1/current translator state and the local-content contract. Confirm that the chosen compatible player-supplied build and required private tooling are locally available; no private input is presumed available from a public clone.

**Owned output:** Establish the complete proposed preparation-to-Wasm route for one bounded title slice: selected original bytes, browser-local preparation/code generation/compilation, and guarded instantiation. Reuse the existing translator/worker flow where applicable. Name which native pipeline components can move into the browser and which need adaptation before implementation. Include resource bounds, cancellation/error handling and local artifact handling. Generated title code remains local to that browser/device. Do not add a native product helper, upload, remote conversion, or distributed generated title code.

**Acceptance:** A fresh browser produces and instantiates the bounded slice from compatible locally selected bytes without a prebuilt private module or native helper in that tested path. Keep original and derived title content local. Demonstrate bounded reads/memory/time and cleanup/reselect behavior, with source-owned fixtures and focused workflow checks. Record exact tested build, browser, conditions, timing, limits, and local evidence identity. If a needed compiler stage cannot yet run in the browser, return a precise dependency/prototype plan and do not claim this goal complete.

**Stop:** Stop at unsupported image/build/format, missing private prerequisite, resource budget breach, or any conversion path requiring a native helper or remote handling. A bounded preparation/compile slice does not establish complete conversion, offline play, or M2 acceptance.

### F. M3 server authority and local-content architecture decision

**Prerequisite:** Coordinator assigns a design task after reviewing current runtime capabilities, content contract, and milestone dependencies. Treat server authority versus original execution in each browser as unresolved; do not presume networking code or an authoritative simulation already exists.

**Owned output:** A decision record and bounded interface/data-flow proposal for the selected authority contract. Address authoritative score/collision/elimination/pickup/randomness/round outcomes, browser-local original code/assets/RAM/saves, the minimal information peers/server may exchange, mixed/local seat ownership (four seats total), reconnect, lobby ownership, and match-process recovery as distinct concerns. No network implementation is included unless separately assigned.

**Acceptance:** The decision names what can be authoritative with available evidence, what cannot be reconciled with local-content/fidelity constraints, required experiments, threat/failure cases, and explicit unresolved questions. Update the structured task state and relevant architecture/risk/decision records; do not claim simulation equivalence or product readiness. Record review/timing metadata as instructed by the workflow.

**Stop:** Stop if the contract requires uploading or distributing original content/title code/state, inventing original rules, or claiming shared authority without a demonstrated basis. Escalate the architectural conflict to the coordinator with options and evidence; do not add a server or network code by assumption.

## State and timing handoff

The coordinator is adding the `dev:status`, `dev:preflight`, `dev:task`, and `test:workflow` npm workflows. Use their exact documented commands and fields from `docs/development-workflow.md`; this handoff intentionally does not invent flags or schemas. Before work, detect and preserve the newest state rather than treating prose here or in an older profile as current. After work, update the workflow-owned task state and timing record, then synchronize narrative status/handoff if the accepted runtime frontier changed. Keep evidence local when it contains original or derived content, and report the distinction between a completed bounded task, an execution checkpoint, and milestone acceptance.
