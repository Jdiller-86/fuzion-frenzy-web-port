# Development plan

<!-- development-state:start -->
Recorded development state: **M1 / awaiting-evidence**, profile **44**. Retained startup resume task: **SCOPE-37**.

The selected C allocation wrapper and SDK allocator return; startup reaches the existing logger provider's new-caller guard.

[Current runtime evidence](docs/status.md) owns detailed observations and limits. [Structured state](profiles/development-state.json) owns this generated pointer; [workflow](docs/development-workflow.md) explains preflight and updates. [Goal G](docs/local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) owns the SDK integration assignment; the retained startup stop is a reference boundary. Source-only work can proceed without private inputs; original execution requires the matching local baseline. No milestone is accepted by this record.
<!-- development-state:end -->

Run the original Xbox Fuzion Frenzy entirely in a browser through static recompilation to WebAssembly, then deliver the complete planned online, social and control experience. Original behavior and presentation are the baseline; all player content and derived title code stay local.

## Current work

M1's primary assignment is [Goal G](docs/local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05): use the existing Fuzion SDK, reproduce or reuse a bounded native desktop reference, then attempt the first original browser frame within an initial 150-minute local session. Desktop reference work gets at most 30 minutes; record a blocked environment and continue independent browser compilation/interface work. The session must return executable evidence or an actual compiler/browser failure and exact adapter gaps. This is a decision budget, not a completion estimate.

The generated pointer and [status](docs/status.md) retain the accepted M44 boundary separately from this experimental SDK candidate. Desktop execution helps isolate upstream/game behavior from browser adaptation; its executable is not converted to Wasm, and it does not satisfy the browser milestone. Build the same source for the browser with reviewed memory/dispatch/thread/file/GPU/audio adaptations. No controllable original browser screen or complete product is accepted.

The next implementation work should advance **M1: Original browser execution** using the [largest-first browser work order](docs/m1-browser-work.md#largest-first-implementation-order), generated from [reviewed scope data](profiles/m1-browser-work.json). Pull in the exact reached startup/resource dependencies needed for the next large proof. Existing startup, device-policy and stack-repair evidence supports that work; the recorded startup stop is not a prerequisite for independent browser interface work. None of these foundations completes a project milestone by itself.

The [M1 browser work map](docs/m1-browser-work.md) names the execution-loop, resource, graphics, audio, input and original-round connections still to integrate, with evidence, rough structural sizes and completion checks. Use it with the scope ledger to report remaining work at the capability level.

## Milestones

Following [CTR-2-WUP’s planning model](https://github.com/Jdiller-86/CTR-2-WUP/blob/9b2245595f7900d84a111ac7030bcba0bcb0a66a/plans.md#milestones), each milestone distinguishes its foundation from the remaining observable result. Full pass/fail criteria and work packages are in [project milestones](docs/milestones.md). Native [GitHub milestones](https://github.com/Jdiller-86/fuzion-frenzy-web-port/milestones) and their linked tracking issues own live status.

| Milestone | Existing foundation | Remaining outcome |
| --- | --- | --- |
| [M1: Original browser execution](docs/milestones.md#m1--original-browser-execution) | Guarded translation, checked runtime components, bounded original startup, geometry and input research. | Play one original minigame through its original menus, round and results entirely in a desktop browser. |
| [M2: Complete local game and conversion](docs/milestones.md#m2--complete-local-game-and-conversion) | Limited browser-local XISO/asset preview, known reference identity and reusable prototype input. | A fresh browser converts the supported player-supplied image and runs the complete original offline game. |
| [M3: Online multiplayer and recovery](docs/milestones.md#m3--online-multiplayer-and-recovery) | Authority, content locality and four-seat ownership requirements are recorded; online services are unimplemented. | The original game supports authoritative remote and mixed local/remote matches with recoverable tournament results. |
| [M4: Player experience](docs/milestones.md#m4--player-experience) | The research app has local menus and owned keyboard/gamepad input; accounts, friends and production lobbies are unimplemented. | Players can set up their content and controls, find friends, join lobbies and play/rematch through the supported browser interface. |
| [M5: Release readiness](docs/milestones.md#m5--release-readiness) | Source publication guards, pinned runtime, source-only builds and Windows/Linux CI already exist. | Ship a reproducible, documented and operationally supported release for the declared browser/device matrix. |

At the replanning baseline, **0/5 milestones are accepted**. The old M1–M12 runtime ladder is retired; its completed checkpoints remain research history. Work may overlap, while acceptance follows the dependencies in the milestone definitions.

## Product requirements

| Area | Required result |
| --- | --- |
| Original game | Complete verified minigame/variant catalog, characters, arenas, rules, AI, menus, camera, animation, audio and saves |
| Modes | Practice, individual games/Minigame Frenzy and full Tournament progression, including original stage choices and Frenzy rounds |
| Players | One to four total seats; solo/local, remote and mixed local/remote parties, with original CPU behavior/difficulty |
| Content | Compatible player-supplied files identified and converted in the browser; bounded memory, cancel/clear/reimport and storage recovery |
| Controls | Per-seat keyboard/gamepad ownership, remapping/conflicts, calibration/prompts, preferences and safe focus/disconnect/reconnect |
| Online | Authoritative scores, collisions, elimination, pickups, randomness and round transitions; fair input/presentation |
| Lobbies | Create/discover/join, privacy, codes/invites, compatible-content readiness, owner handoff and rematch |
| Accounts/social | Recovery, friends/requests/invitations, privacy/block/report and authorized data access |
| Persistence/recovery | Atomic tournament checkpoints/results, idempotent writes, reconnect/seat reclaim and separate match-process recovery |
| Browser experience | Audio/fullscreen, quality/accessibility, supported browsers/devices and measured performance |
| Release operations | Versioned protocols/rules/converters, safe deployment/rollback, backups, load/admission, privacy, licensing and authorized costs |
| Later devices | Mobile/touch after desktop feasibility, with the same applicable input/fidelity/release tests before support is advertised |

Accounts, browser connections, profiles and seats are distinct; multiple local seats still count toward four total. In-app friends do not assume Xbox Live integration. The compatible shipped inventory determines full parity; public descriptions of roughly 45 minigames plus Frenzy, six characters and six zones remain reference leads.

## Validation and architecture

Milestone acceptance uses repeatable matching-build reference scenarios and actual browser observations. Network checks include four remote and mixed two-local/two-remote seats at 150 ms RTT, 30 ms jitter and 1% loss, with repeated timing/contact/steering/orb and separate audio-fairness cases. Browser acceptance names Chrome/Edge/Firefox/Safari and hardware, targets 60 fps with a usable documented 30 fps mode, and measures full tournaments, memory, import and recovery. Freeze budgets before acceptance runs; earlier 128 MiB/120-second importer proposals are not measured guarantees.

The implemented source is JavaScript modules and C. TypeScript/Vite/Babylon.js, React menus, Colyseus and PostgreSQL/auth were proposals, not installed dependencies. Original recompilation does not automatically provide an authoritative shared server simulation; resolve the execution/authority/content contract before netcode. See [architecture](docs/architecture.md), [risks](docs/risks.md), [scope](docs/compatibility.md) and [content policy](docs/content.md).

Optional modernization preserves an original presentation baseline. PWA caching, daily challenges and custom rules remain follow-on enhancements; ranked play, voice/free-form chat, larger parties, spectators and Xbox social integration require separate scope.

## Tracking and maintenance

[Project management](docs/management.md) defines native issues, parent/dependency links, status and evidence acceptance. The public [GitHub Project](https://github.com/users/Jdiller-86/projects/3) organizes the milestone parents and their child work. Keep current behavior in [status](docs/status.md) and the [handoff](docs/handoff.md), consequential decisions in [decisions](docs/decisions.md), and curated facts in [research](docs/research.md). Earlier records remain in Git history. No milestone closes through documentation, task counts or source fixtures alone.
