# Architect PR and execution review

## Current cycle: direct-write fix accepted; two bounded A001 checks remain

Starting integration `a57b9780cb2e3e3214a376a58406338d2f2981c3` was clean. Requested fixed heads are source `1807769cec14f5fa015eac4b8ce61fbea0ac80e3`, geometry `220c2a80063931716280b7a101bf331eec0f9f78`, local main `ebd1ebf75173224ce266ce3ca3eda41cec7f2edb`. The reviewed two-line local-main change makes model-availability reports time-specific; it is included in this cycle's governance merge. It changes no model, permissions or title runtime. Source and geometry remain unmerged.

### A001: narrow the remaining task, do not repeat completed work

Source now emits const vector reads and adds a generator-to-compiled regression. Independent exact-head validation passes Python emitter 12/12, codegen 2/2 with no compiler skips, and device-register C optimized/ASan/UBSan. Const assignment is rejected by the compiler: the previously demonstrated direct read-as-write is **fixed**, and historical claims of a mutable read must not be repeated as current.

Two previously required checks remain, confirmed by source inspection and the independent reviewer/validator:

1. `const RecompXmm *p = &(XMM_MEM(eax));` is still admitted and compiles. An owned real-VM regression reads the next uncommitted page through the derived pointer after only one 16-byte checked access. This is an unsupported escaped-pointer form, not an original title instruction or a request for a general C parser. Return the read by value or reject address-taking structurally, and add the existing required negative address-of check alongside the passing assignment check.
2. The compiled regression executes generated reads/stores only at permitted addresses. Denied cases call pointer macros directly and compare NULL/bytes. Execute the existing generated functions under a denied access with a stopping/unwinding hook (as production does), assert the stop and unchanged state, and never dereference a returned NULL. Reuse the real VM and generator; no new fixture hierarchy, upstream type copy or CPU-wide audit is needed.

This is the full remaining A001 task. Do not repeat alignment implementation or the fixed direct-assignment reproduction. Const fixes one part; it does not satisfy the address-escape or generated-denial branches. A source merge remains withheld pending those two deltas.

### Current original-execution dependency: display timing, not a PFIFO consumer

At requested source `1807769`, the canonical run still stops on the unknown PMC read `0xFD000100` (276 routines, 83 initializer returns, 403 operations, timerCheck=0). Explicit `pmcZero` now ends at a declared status wait (65 unchanged masked reads), not event-log exhaustion. Neither status supplies an interrupt or engine progress.

Live draft #2 advanced during this review to `f402f5b7ad269be118d0d110a89cebc1ccae0e80`. Its curated provenance checkpoint identifies PMC bit 24 as **PCRTC**, not PFIFO, with pending/enabled display status and a missing event producer in the extracted reference. The architect independently checked the pin `dc10b4cea96e403d30c17ed53551fdaf023bc17f` public xemu `hw/xbox/nv2a/nv2a_regs.h`: line 67 defines `NV_PMC_INTR_0_PCRTC (1 << 24)`; lines 655–658 define PCRTC pending/enabled VBLANK bit 0. This verifies register identity, not the complete timing implementation, guest execution or fidelity. Earlier PFIFO-bit-24 labels are superseded. The newer full source head is not integration-reviewed or executed here.

After the two A001 deltas, define one bounded display-event contract from retained evidence: pending/enable/acknowledge/reset and PMC aggregation, a justified configured timing transition and budgets, and explicit stops for unknown modes or missing producer. No arbitrary 60-Hz timer, poll-count trigger, GET==PUT implication, static status bit or forced return is approved. A polled display status and CPU IRQ delivery are separate contracts. The experimental acknowledge/control and canonical fail-closed PMC boundary stay intact.

### Geometry, PRs and limits

Geometry `220c2a8` is documentation-only and correctly records the pause; no new runtime/reference evidence exists. Whole-chain and retained-reference acceptance remain required for integration. Clarification: once an admissible reference and named bounded review task exist, the architect may authorize the minimal comparison adapter **before** the comparison succeeds; do not impose a circular requirement that the comparison already pass before its adapter can be built. There is no such authorized task now; keep the lane paused without more fixtures/topology.

Live GitHub read-only verification: #1 open/non-draft/clean/mergeable at `a57b978` over main `1a67c4f`, with Linux/Windows preparation successful; #2 open draft/conflicted at `f402f5b`; #3 open draft/conflicted at `220c2a8`. Existing PRs merit retention, not duplicate openings or builder approval. No opening or merge marker is issued. The changed integration head must be published and rechecked before main authorization; no PR or credential helper was invoked by the architect.

No original module or private input was accessed or run. The independent tests used only owned synthetic RAM/source. Scout's prior-gate lookup was checked against the recorded requirements; it did not approve the new code. Full fidelity, browser-local conversion, rendering/playability and all online/social/control/authority goals remain unfinished. A011/A012 govern the narrowed priorities.

Current integration checks pass: `npm run check` (293 source files), `npm run test:runtime` (110/110), `git diff --check`, and offline parsing of the changed cycle script. No runner was invoked. These are source/governance checks, not original-game execution. The only imported executable-script change is the reviewed two-line scheduling-prompt correction; no builder device/vector/geometry implementation is included.

## Previous cycle

## Latest cycle: vector closure rejected; experiment separation improved

Starting integration `38a398a4533c63a4def196929af7de21217c8265` was clean. Fixed requested heads: source `7ee63ea333460b10481ce45ad4ab04525f8521f3`, geometry `818380b868db0dbf5b4519f7e84e270de84a4011`, local main `bb86cfd89b4da4246a5e5ce65c8b22fd6823fe1a`. Local-main governance is integrated by this cycle's merge; its sole decision-log conflict preserves all existing histories and records the revised D075 as W011, superseding W010's automatic-opening wording. Both changed PowerShell blobs passed offline parsing, and the exact marker/head gating was inspected without invoking either runner or credentials. Existing PRs require no opening marker, and this cycle requests no duplicate PR.

Live public API verification found PR #1 open/non-draft/clean/mergeable at `38a398a` over main `1a67c4f`, with successful Linux/Windows preparation jobs. Draft #2 had advanced concurrently to source `8ced8755695b9f9eaa2951d48fddb0993faadf6a`, and draft #3 to geometry `818380b`; both remain conflicted. The architect inspected the additional `8ced875` alignment-hook delta but did not approve or run the complete newer head. Source and geometry remain unmerged. Retain all three review queues; no opening marker or main-merge authorization is issued. Green checks on the starting head do not cover the new governance/review merge.

### A001 remains a reproduced permission bypass

Source `7ee63ea` adds a useful compiled real-VM fixture and mnemonic-based alignment emission, but does not change the modifiable read expression. Exact emitter input `(XMM_MEM(eax)) = xmm0;` still becomes a store through a checked pointer with read intent. The independent validator reused the owned fixture's real VM mapping in an ephemeral synthetic-RAM test: that emitted read-intent store **mutated PAGE_READONLY bytes** under both optimized and sanitizer builds, while the write-intent control was refused. No original instruction is alleged to use this adversarial input. The fixture currently hand-writes an expected emission shape instead of generating and compiling the unsafe context, so its passing result does not refute the bypass.

Do not accept the source checkpoint/title's claim that A001 is closed. Primary's single next task is a structurally non-writable/by-value generated load (or a strict admitted grammar) plus a regression that actually runs the generator, compiles its output and proves denial before mutation using the existing real-VM setup. Include a negative compile/admission check for assigning to or taking the address of a read result. Reuse the fixture; no new type-system ownership, full C parser, original-code extraction or general CPU audit is requested.

The new alignment work is separate. At `7ee63ea`, the alignment macro returned NULL immediately before generated dereference, a fault-contract problem not tested by pointer-only assertions. The newer observed `8ced875` replaces it with an explicit stopping host hook and reports regenerated movaps sites, resolving that particular design direction without fixing writable reads. Do not repeat a blanket width-based alignment guard; retain opcode-specific evidence and require an actual deterministic stop for misaligned emitted operations. Its full opcode coverage and recomposed private execution are not independently verified here.

### Experiment and geometry disposition

The source now parses an explicit `device.writeClearing` flag, sets it before MMIO admission and only declares both acknowledge rules when enabled. Curated same-module control evidence is reported: absent flag reproduces the old recurrence, enabled flag stops at the unknown PMC read `0xFD000100`; the canonical PMC-zero range is removed. Accept this as the requested **experimental-policy separation**, not hardware confirmation. Keep the disabled/plain-storage control and preserve unknown-status and sub-dword rejection. The newer private report is build-69 SHA-256 `bf9e9e2d…`, 276 routines, 83 initializer returns, 403 operations, timerCheck=0 and 3,839 trace events, at the same PMC boundary; the architect ran no original module. No PMC event, engine completion, command consumer or GET advance is approved.

Geometry's delta contains tests/docs only: sentinel-tail/count checks for existing entry points. It neither changes production topology nor supplies the missing retained-reference/whole-chain review. Keep draft #3 and pause further fixture expansion until a concrete reference comparison or original renderer dependency is actionable; passing analytic bounds tests cannot create that evidence.

Independent checks on exact `7ee63ea`: new VM fixture optimized and ASan/UBSan passes, emitter Python 12/12, MMIO tests 4/4 and related runtime/report/submission tests 24/24 pass. The adversarial synthetic-RAM test returns failure because the read-only bytes mutate (a successful reproduction of the defect, not a passing safety check). Temporary worktree was removed non-force clean. Scout was asked for a narrow prior-gate lookup; no approval depends on it. No private content, original module or authenticated PR mutation was used in this review. Full fidelity, local conversion, rendering/playability and all online/social/control/authority goals remain unaccepted.

Current integration checks pass: publication/preparation for 293 source files, runtime 110/110 and whitespace verification. The independent reviewer corroborated the structural A001 concern from supplied source findings, not a complete newer-head checkout review. Live open-PR listing confirms only #1–#3; no other main PR was returned. No credentials or authenticated runner were invoked. A host alignment stop is a bounded proof stop, not full guest exception delivery or a fidelity result.

## Prior cycle record

## Reviewed snapshot

- Integration at start: `54f062e749bc528d2da165d86ac166b468baa5e9`, clean.
- Source candidate: `6759169c6a8e99e1ab6ea2f5f89bc0671148ec8a`.
- Geometry candidate: `068232865f1026c83703fbd73422717b9296bdbd`, unchanged since the previous review.
- Local main governance candidate: `111c832b37053a5e9aac3bcdd63ab5a97b799201`.

These are fixed review refs, not a claim that concurrently running builders have stopped. No original module or ignored private content was read or executed. The selected static-recompilation route, full fidelity, local-only content and all planned online/social/control/authority goals remain active and incomplete.

## Live GitHub verification (read-only API)

The public GitHub API confirms exactly three open PRs:

| PR | Head | Base | Current disposition |
| --- | --- | --- | --- |
| #1 | integration `54f062e` | main `1a67c4f` | Open, non-draft, mergeable/clean; both Linux and Windows preparation jobs successful. Retain for reviewed integration, but these checks apply only to this old head. No merge authorization is issued by this review. |
| #2 | source `6759169` | integration `54f062e` | Open draft, conflicted. Merits continued draft review, not integration approval; A001 and experimental-device gates remain open. |
| #3 | geometry `0682328` | integration `54f062e` | Open draft, conflicted. Retain for whole-chain/reference review; no new topology or reference evidence since the last review. |

No other open main PR was returned. No PR was created, edited or merged by the architect, and no shell command was used to mutate PRs. The runner alone handles publication and missing PR creation. A clean GitHub mergeability result is not a correctness review, and conflicts must be resolved in the architect worktree only after source review, never by resetting active builder work.

## Source delta: D145 is experimental, not engine progress

Compared with `b0ca634`, the candidate adds a second inferred write-clearing declaration for PFIFO status `0xFD002100`, an eight-slot per-register wait tracker and a separate device-capacity status. The selected private profile now supplies PMC status `0xFD000100` through an explicit zero range. Neither inferred interrupt rule is independently confirmed; declarations remain hardcoded in probe profile setup rather than selected as an explicit experimental policy. Explicitly supplying a synthetic zero does not establish that zero is the original device state.

The private report is build-68, 640,409 bytes, SHA-256 `a385b2d3…`: 276 routines, 83 initializer returns, 403 kernel operations, timerCheck=0, 7,518 trace events, then status 15 at `0xFD003220` when the 32,768-event device log fills. Repeated PULL0 writes and status polls show an unsatisfied wait under this synthetic model, not command consumption, GET advancement or hardware progress. Do not increase the log, invent engine completion or seed an interrupt bit to manufacture a later stop. The reason that a bounded wait detector did not stop this run before capacity is diagnostic behavior to explain, not permission to build a speculative engine.

The same A001 gate is still unmet: the emitter and vector fixture were not changed by D145. The earlier direct-store/helper fixes remain useful, but parenthesized read-as-write admission and generated-vector/real-VM coverage are unresolved. First close that existing bounded safety task. Then make the PGRAPH/PFIFO experiment opt-in with a retained plain-storage control and identify the original event/submission dependency from retained evidence. A006 extends to both inferred interrupt rules and the PMC-zero experiment; none is accepted as the canonical original hardware baseline.

## Local-main governance integration and checks

Local main `111c832` is integrated by the governance merge containing this record. The sole conflict, `docs/DECISIONS.md`, preserves the complete architect/runtime history and maps incoming D075 to W010; D074 already exists as W008 and was not duplicated. No configuration or active model was changed. Source and geometry remain unmerged. PR #1 is retained for the reviewed governance/documentation delta, not as approval of either draft. No exact-head merge marker is issued: GitHub checks and current PR head/base must be refreshed after publication of the new integration commit.

The independent builder and architect reviewed the PR helper's source guards: published exact head, clean source-only paths, local/remote main ancestry, local publication/runtime checks, successful Linux/Windows preparation jobs, current PR head/base and clean mergeability, then the merge API with the expected head SHA. The old direct-main push path is removed. Both changed PowerShell scripts passed offline parser checks (zero errors); neither helper nor any credential command was invoked in this audit. No token was read or printed. Nonblocking follow-ups: scheduled GCM lookup should disable interactive prompting; avoid the helper's `$args` variable shadowing; an ambiguous 25-second merge timeout needs an idempotent status refresh. These are runner-hardening tasks, not reasons to bypass merge protections.

The previous late validation at exact source `b0ca634` passed Python 11/11 and 25/25, runtime 110/110, and five owned C fixtures optimized/sanitized; geometry `0682328` passed its analytic triangle test optimized/sanitized. The current bounded validation at exact source `6759169` passes the changed device-register fixture optimized and ASan/UBSan, Python emitter 11/11 and stub vector test. It independently confirms the unchanged emitter and the real-VM coverage gap. These results do not approve branch integration or execute the original title. Scout returned the existing A001/interrupt-policy gate lookup, verified against the actual handoff/checkpoint.

Final governance-merge checks: `npm run check` passes for 293 source files, `npm run test:runtime` passes 110/110, and `git diff --check` passes. The staged path list contains only the reviewed PowerShell helpers and governing documentation; no builder runtime implementation or private input is included.

## Geometry and product limits

Geometry's prior analytic triangle test and offset assertions passed optimized and sanitizer validation, but this is not whole-chain or retained-reference validation. Keep the existing draft and topology hold; do not repeat low-impact ABI fixtures or copy private references into the cloud worktree simply to keep the lane busy. Resume for a concrete renderer dependency or an admissible locally run reference comparison yielding a curated report.

No controllable original screen, complete graphics/audio/controllers, general disc reads, durable saves, end-user browser conversion, release performance, full catalog/detail parity or online/social/server authority is established. Original-containing code/assets/RAM/saves stay local; an input relay is not server authority. No route change or approximate reconstruction is approved.
