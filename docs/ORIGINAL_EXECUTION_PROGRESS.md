# Product acceptance ledger

This ledger maps the complete project goal to evidence. Exact startup details belong in [current status](PROJECT_STATUS.md); the [roadmap](../DEVELOPMENT_PLAN.md) and [runtime gates](RUNTIME_MILESTONES.md) define acceptance. The Twisted System prototype does not accept original-game requirements.

| Requirement | Evidence required | Recorded state |
| --- | --- | --- |
| Browser-only original execution | Original instructions, output, and a controllable title/menu screen in a normal browser; no native or streamed execution | Bounded startup observed; no controllable screen accepted |
| Full runtime | Explicit coverage of memory/aperture, scheduling, graphics, audio, controllers, filesystem/disc/save behavior, with stated limits | Partial source-owned components and startup models; M11 unaccepted |
| Full original parity | Compatible-build inventory and reference scenarios for every minigame/variant, mode, character/zone, AI, menus, camera/animation/audio, saves and tournaments | Unaccepted |
| Local player content | Supported-build validation and bounded browser-local conversion; cancel/clear/reselect, storage recovery, no upload/peer distribution | Limited local asset/disc/runtime proofs; finished end-user converter unaccepted |
| Original solo/local play | One to four owned seats, practice, rounds/tournaments, original CPU behavior/difficulty | Prototype research only; original integration unaccepted |
| Controls | Original runtime receives correct keyboard/gamepad actions; remapping, focus, hotplug, calibration, physical devices | Adapters/fixtures reusable; original behavior unaccepted |
| Online and mixed parties | Four remote seats and multi-seat browsers under representative latency/loss; authoritative outcomes | Unimplemented; authority/runtime/content contract unresolved |
| Lobbies/recovery | Discovery/privacy, invitations/codes, readiness, owner handoff, reconnect, rematch, and match-process recovery | Unimplemented |
| Accounts/social | Account recovery, friends/requests, privacy/block/report, authorization/isolation | Unimplemented |
| Persistence/authority | Atomic tournament checkpoints/results, idempotent commits, no client-selected winners | Unimplemented |
| Browser/release matrix | Named browsers/hardware/controllers, audio/GPU recovery, frame/memory/import measurements and soak tests | No accepted original-title matrix |
| Delivery/operations | Source-only builds, compatibility/versioning, deploy/rollback, backup/load/admission and costs | Public guards and CI exist; production product delivery unaccepted |

The repaired stack/return-address defect is a bounded M6 result; it does not close the product items above. Read [risks](PROJECT_CONCERNS.md) before affected work, particularly the conflict between browser original execution and the proposed authoritative shared simulation.

The [prior implementation ledger](archive/ORIGINAL_EXECUTION_PROGRESS_PRIOR_20261106.md) retains dated native cross-build, SDL/browser-host, compiler, and early-startup observations. Follow [the research index](RESEARCH.md) to their checkpoint records instead of treating those old counts or next-task paragraphs as current.
