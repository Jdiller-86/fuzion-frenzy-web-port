# Documentation

<!-- development-state:start -->
Recorded development state: **M1 / awaiting-evidence**, profile **44**. Retained startup resume task: **SCOPE-37**.

The selected C allocation wrapper and SDK allocator return; startup reaches the existing logger provider's new-caller guard.

[Current runtime evidence](status.md) owns detailed observations and limits. [Structured state](../profiles/development-state.json) owns this generated pointer; [workflow](development-workflow.md) explains preflight and updates. [Goal G](local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) owns the SDK integration assignment; the retained startup stop is a reference boundary. Source-only work can proceed without private inputs; original execution requires the matching local baseline. No milestone is accepted by this record.
<!-- development-state:end -->

Start with [the project overview](../README.md) and [development plan](../plans.md). The source and tracking follow CTR-2-WUP's domain layout and evidence-based planning, adapted to browser execution. Follow the assigned user objective and [current status](status.md); later roadmap work is not implicitly assigned.

<!-- project-goals:start -->
## Goals and next work

| Milestone | Remaining product outcome | Acceptance and live status |
| --- | --- | --- |
| **M1: Original browser execution** | Play one original minigame through its original menus, round and results entirely in a desktop browser. | [Criteria](milestones.md#m1--original-browser-execution) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/22) |
| **M2: Complete local game and conversion** | A fresh browser converts the supported player-supplied image and runs the complete original offline game. | [Criteria](milestones.md#m2--complete-local-game-and-conversion) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/25) |
| **M3: Online multiplayer and recovery** | The original game supports authoritative remote and mixed local/remote matches with recoverable tournament results. | [Criteria](milestones.md#m3--online-multiplayer-and-recovery) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/28) |
| **M4: Player experience** | Players can set up their content and controls, find friends, join lobbies and play/rematch through the supported browser interface. | [Criteria](milestones.md#m4--player-experience) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/31) |
| **M5: Release readiness** | Ship a reproducible, documented and operationally supported release for the declared browser/device matrix. | [Criteria](milestones.md#m5--release-readiness) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/34) |

**Recorded development:** M1 / awaiting-evidence, runtime checkpoint **44**. [Runtime evidence](status.md) distinguishes the accepted checkpoint from local candidates. Native issues own live milestone acceptance.

**Current implementation assignment:** Use the existing Fuzion SDK to get original execution working in the browser. Spend at most the first 30 minutes on a matching desktop reference, then build, run and fix actual browser blockers. Reach the first original frame and continue toward a controllable original menu or selected minigame. Defer detailed tests, broad matrices, per-caller audits and repeated historical comparisons until the baseline works. At 150 minutes give a brief progress update and continue productive implementation; this is not a stop or delivery promise. See Goal G under #49.

**M1 order: largest ready item first.** Select one scenario (MB01), settle the required interfaces, and pull small prerequisites forward only to enable the next large proof.

| Order | M1 work / structural size |
| --- | --- |
| **1 — SDK integration and execution bottlenecks** | [MB04](m1-browser-work.md#mb04), [MB05](m1-browser-work.md#mb05), [MB06](m1-browser-work.md#mb06), [MB07](m1-browser-work.md#mb07) · **M–L / L** |
| **2 — Original graphics, including an early title frame** | [MB11](m1-browser-work.md#mb11), [MB12](m1-browser-work.md#mb12), [MB13](m1-browser-work.md#mb13), [MB14](m1-browser-work.md#mb14), [MB15](m1-browser-work.md#mb15), [MB16](m1-browser-work.md#mb16), [MB17](m1-browser-work.md#mb17) · **M / M–L / L** |
| **3 — Original audio** | [MB18](m1-browser-work.md#mb18), [MB19](m1-browser-work.md#mb19) · **L / M** |
| **4 — Finish the selected startup and resource path** | [MB02](m1-browser-work.md#mb02), [MB03](m1-browser-work.md#mb03), [MB08](m1-browser-work.md#mb08), [MB09](m1-browser-work.md#mb09), [MB10](m1-browser-work.md#mb10) · **S / M–L / M** |
| **5 — Original input and controllable menu** | [MB20](m1-browser-work.md#mb20), [MB21](m1-browser-work.md#mb21), [MB22](m1-browser-work.md#mb22) · **M / S–M** |
| **6 — Complete the original round/results/return** | [MB23](m1-browser-work.md#mb23) · **M–L** |
| **7 — Bounded corrections and M1 acceptance** | [MB24](m1-browser-work.md#mb24) · **S–M** |

[All 24 browser connections, evidence and exit checks](m1-browser-work.md) · [Largest-first details](m1-browser-work.md#largest-first-implementation-order) · [All work-package delivery briefs](work-packages.md) · [Next local-agent assignment](local-agent-handoff.md#bounded-goals-and-selection). These rows decompose existing scope; they are not an additional task estimate or a completion percentage.

**Tracked scope:** 51 delivery issues in 10 work packages under five milestone gates. Counts are not effort estimates.

[Tracked delivery issues](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues?q=is%3Aissue+label%3Awork%3Adelivery) are native children of the work-package issues. The README chart and Project use their live status; package closure and milestone acceptance remain separate.
<!-- project-goals:end -->

| Purpose | Guide |
| --- | --- |
| Five overall-project outcomes and pass/fail criteria | [Milestones](milestones.md) |
| Concrete child-issue deliverables, first assignments, prerequisites and evidence | [Work-package delivery briefs](work-packages.md) |
| Native issues, dependencies, status and progress reports | [Project management](management.md) |
| Current main-branch evidence and baseline handoff | [Status](status.md) and [handoff](handoff.md) |
| Setup, commands, prototype controls and check limits | [Development](development.md) |
| Checked current-state pointers, private-baseline integrity and measured task iteration | [Iteration workflow](development-workflow.md) |
| Copy-paste prompt and separately bounded local implementation goals | [Local agent handoff](local-agent-handoff.md) |
| Local Sol High coordinator, Luna workers and execution verification | [Agent setup](agents.md) |
| Source boundaries and proposed architecture | [Architecture](architecture.md) |
| Source-only authority/content assessment and required decision experiment | [Online authority](online-authority-assessment.md) |
| Player files, conversion and content locality | [Content policy](content.md) |
| Original-game fidelity and full parity | [Scope and compatibility](compatibility.md) |
| Current technical/release questions | [Risks](risks.md) |
| Reference identities and conversion coverage | [Reference inventory](reference.md) |
| Original UI resource/code investigation and private catalog workflow | [UI architecture](ui-architecture.md), [settings integration](ui-settings-integration.md), [upscaling](ui-upscaling.md), and [tools](../tools/ui/README.md) |
| Runnable native F1 settings and bounded texture replacements | [Native UI](native-ui.md), [font recovery](ui-font-recovery.md), and [incremental build](../tools/ui/BUILD-NATIVE-UI.md) |
| Curated facts, static/fixture/browser evidence distinctions | [Research](research.md) |
| Optional private toolchain and analysis prerequisites | [Analysis](analysis.md) |
| Resource reuse, automated preparation and fast M1 iteration | [Development workflow](resource-reuse.md) |
| Measured M1 scope, existing providers, grouped gaps and next-task ledger | [M1 work scope](m1-work-scope.md) |
| Named remaining browser connections, rough sizes, dependencies and observable completion checks | [M1 browser work map](m1-browser-work.md) |
| Consequential choices | [Decisions](decisions.md) |
| Contributions and repository instructions | [Contributing](../CONTRIBUTING.md) and [AGENTS](../AGENTS.md) |

Keep one current milestone definition and native tracking status. Earlier checkpoints and automation transcripts remain in Git history. Publish curated evidence identities/outcomes rather than original media or private raw artifacts.
