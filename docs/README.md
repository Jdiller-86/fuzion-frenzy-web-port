# Documentation

Start with the [README downloads and status](../README.md), [current evidence](status.md) and [next assignment](handoff.md). The Windows builder is published; the browser build is published but still needs original gameplay verification.

<!-- development-state:start -->
Recorded development state: **M1 / awaiting-evidence**, profile **44**. Retained startup resume task: **SCOPE-37**.

The selected C allocation wrapper and SDK allocator return; startup reaches the existing logger provider's new-caller guard.

[Current runtime evidence](status.md) owns detailed observations and limits. [Structured state](../profiles/development-state.json) owns this generated pointer; [workflow](development-workflow.md) explains preflight and updates. [Goal G](local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) owns the SDK integration assignment; the retained startup stop is a reference boundary. Source-only work can proceed without private inputs; original execution requires the matching local baseline. No milestone is accepted by this record.
<!-- development-state:end -->

Start with [the project overview](../README.md) and [development plan](../plans.md). The source and tracking follow CTR-2-WUP's domain layout and evidence-based planning, adapted to browser execution. Follow the assigned user objective and [current status](status.md); later roadmap work is not implicitly assigned.

<!-- project-goals:start -->
## Goals and next work

| Milestone | Remaining outcome |
| --- | --- |
| [M1: Original browser execution](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/22) | Play one original minigame through its original menus, round and results entirely in a desktop browser. |
| [M2: Complete local game and conversion](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/25) | A fresh browser converts the supported player-supplied image and runs the complete original offline game. |
| [M3: Online multiplayer and recovery](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/28) | The original game supports authoritative remote and mixed local/remote matches with recoverable tournament results. |
| [M4: Player experience](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/31) | Players can set up their content and controls, find friends, join lobbies and play/rematch through the supported browser interface. |
| [M5: Release readiness](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/34) | Ship a reproducible, documented and operationally supported release for the declared browser/device matrix. |

**Next:** Use the release-optimized browser build and its FPS/CPU-draw readout to identify actual costs. Connect an accelerated browser renderer using existing SDK/upstream components, then reach a responsive original menu and complete minigame. Reuse the working native reference; do not restart integration or run broad test suites before the playable baseline.

[M1 tasks, sizes and remaining connections](m1-browser-work.md) · [Acceptance criteria](milestones.md) · [Agent handoff](handoff.md)

<details><summary>M1 implementation order</summary>

1. [SDK integration and execution bottlenecks](m1-browser-work.md#largest-first-implementation-order) (MB04, MB05, MB06, MB07)
2. [Original graphics, including an early title frame](m1-browser-work.md#largest-first-implementation-order) (MB11, MB12, MB13, MB14, MB15, MB16, MB17)
3. [Original audio](m1-browser-work.md#largest-first-implementation-order) (MB18, MB19)
4. [Finish the selected startup and resource path](m1-browser-work.md#largest-first-implementation-order) (MB02, MB03, MB08, MB09, MB10)
5. [Original input and controllable menu](m1-browser-work.md#largest-first-implementation-order) (MB20, MB21, MB22)
6. [Complete the original round/results/return](m1-browser-work.md#largest-first-implementation-order) (MB23)
7. [Bounded corrections and M1 acceptance](m1-browser-work.md#largest-first-implementation-order) (MB24)

</details>
**Development:** M1 / awaiting-evidence. Accepted startup reference: **M44**; live milestone acceptance is recorded in the linked issues. The chart tracks 51 delivery issues in 10 work packages; counts are not effort estimates or a completion percentage.
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
| Windows Control Room builder layout, progress and failure handling | [Control Room launcher](control-room-launcher.md), [Windows installer](../desktop/windows/README.md) |
| Current GPU-performance and original-menu mouse assignment | [Local implementation goal](local-menu-and-browser-goal.md) |
| Curated facts, static/fixture/browser evidence distinctions | [Research](research.md) |
| Optional private toolchain and analysis prerequisites | [Analysis](analysis.md) |
| Resource reuse, automated preparation and fast M1 iteration | [Development workflow](resource-reuse.md) |
| Measured M1 scope, existing providers, grouped gaps and next-task ledger | [M1 work scope](m1-work-scope.md) |
| Named remaining browser connections, rough sizes, dependencies and observable completion checks | [M1 browser work map](m1-browser-work.md) |
| Consequential choices | [Decisions](decisions.md) |
| Contributions and repository instructions | [Contributing](../CONTRIBUTING.md) and [AGENTS](../AGENTS.md) |

Keep one current milestone definition and native tracking status. Earlier checkpoints and automation transcripts remain in Git history. Publish curated evidence identities/outcomes rather than original media or private raw artifacts.
