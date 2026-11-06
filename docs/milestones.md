# Project milestones

Five verifiable outcomes cover the project. The structure follows [CTR-2-WUP’s plan](https://github.com/Jdiller-86/CTR-2-WUP/blob/9b2245595f7900d84a111ac7030bcba0bcb0a66a/plans.md#milestones): existing foundation, remaining outcome, explicit acceptance, and linked work. [The roadmap](../plans.md) retains the full requirements.

This plan replaces the old M1–M12 runtime ladder. These M1–M5 IDs name overall-project milestones; old startup/policy/repair checkpoints are supporting evidence. None of the five was accepted at the replanning baseline. Live status and review records belong to the linked GitHub issues, not a commit-count percentage.

| Milestone | Existing foundation | Remaining outcome |
| --- | --- | --- |
| [M1: Original browser execution](https://github.com/Jdiller-86/fuzion-frenzy-web-port/milestones) | Guarded translation, checked runtime components, bounded original startup, geometry and input research. | Play one original minigame through its original menus, round and results entirely in a desktop browser. |
| [M2: Complete local game and conversion](https://github.com/Jdiller-86/fuzion-frenzy-web-port/milestones) | Limited browser-local XISO/asset preview, known reference identity and reusable prototype input. | A fresh browser converts the supported player-supplied image and runs the complete original offline game. |
| [M3: Online multiplayer and recovery](https://github.com/Jdiller-86/fuzion-frenzy-web-port/milestones) | Authority, content locality and four-seat ownership requirements are recorded; online services are unimplemented. | The original game supports authoritative remote and mixed local/remote matches with recoverable tournament results. |
| [M4: Player experience](https://github.com/Jdiller-86/fuzion-frenzy-web-port/milestones) | The research app has local menus and owned keyboard/gamepad input; accounts, friends and production lobbies are unimplemented. | Players can set up their content and controls, find friends, join lobbies and play/rematch through the supported browser interface. |
| [M5: Release readiness](https://github.com/Jdiller-86/fuzion-frenzy-web-port/milestones) | Source publication guards, pinned runtime, source-only builds and Windows/Linux CI already exist. | Ship a reproducible, documented and operationally supported release for the declared browser/device matrix. |

## Acceptance rules

A milestone passes only when every criterion below has evidence tied to the same supported build, source revision and tested environment, followed by an explicit review/acceptance record. Missing, unknown or not-yet-reached requirements remain open. Task closure, compilation and standalone fixtures do not accept a milestone.

Retain the control/experiment reports and independent audit for runtime changes. Evaluate control invariance at its own declared stop; the playable candidate must demonstrate the full scenario. Keep original media, derived title code, RAM, saves and raw/private evidence local. Publish curated outcomes and evidence identities.

Work can overlap, but acceptance follows the dependencies below. The roadmap retains mobile/touch as later scope: desktop comes first, and later device support passes the same applicable input, fidelity and release checks before being advertised. No implementation task starts automatically from this plan.

## M1 — Original browser execution

**Pass when:**

- Complete original menu → minigame → results → menu three consecutive times, with original-program graphics and audible audio responding to recorded player input, and no unsupported stop or fault.
- Execute the game entirely in the browser using compatible player-local content. A privately prepared local module may support this developer gate; end-user browser conversion is required by M2.
- Retain source/build/module identities, browser/device details and the input/capture record, with the runtime control/experiment comparison and independent audit. The prototype and standalone component fixtures do not satisfy this gate.

**Acceptance depends on:** no earlier milestone.

**Work packages:** Integrate the original browser round path; Verify original browser gameplay and evidence.

## M2 — Complete local game and conversion

**Pass when:**

- From a fresh browser, select the supported image, validate/convert locally and start the original game without a developer toolchain. Exercise unsupported/corrupt input, cancel, clear/reimport and denied/evicted storage; original content never uploads.
- Complete the verified catalog/variant/character/arena inventory and original practice, Minigame Frenzy and Tournament flows, with one to four local seats and original CPU behavior. Each required entry passes matching-build reference scenarios; unknown or untested entries remain blockers.
- Verify original scoring/ties, rules, timing, movement/collisions, AI, camera/animation/audio and round/tournament progression, plus save/reload after closing and reopening the browser. Retain the full parity matrix and discrepancy record.

**Acceptance depends on:** M1.

**Work packages:** Finish browser-local conversion and save recovery; Accept the complete original offline inventory.

## M3 — Online multiplayer and recovery

**Pass when:**

- Review and implement the execution/authority/content contract: the server owns outcomes, four total seats have explicit ownership, and original code/assets/RAM/save states remain local. Client-selected winners or a relay-only authority claim fail acceptance.
- Pass four-browser and two-local-plus-two-remote cases across the required online catalog. Verify ten consecutive rounds per timing/contact/steering/orb family at 150 ms RTT, 30 ms jitter and 1% loss; music/pattern activities also pass an explicit audio/input fairness case.
- Demonstrate disconnect/refresh/reconnect, stale-input handling, lobby-owner transfer and separate match-process recovery. Persist atomic tournament checkpoints/results, restore the committed state and prevent duplicate awards or seat ownership.

**Acceptance depends on:** M2.

**Work packages:** Implement authoritative mixed-seat match services; Verify online agreement, fairness and recovery.

## M4 — Player experience

**Pass when:**

- Demonstrate fresh-account/recovery → content readiness → friend request/invite → public/private lobby or join code → ready/start → match → rematch. Verify permissions, privacy/block/report, incompatible content and rejected unauthorized API operations.
- Verify per-seat keyboard/gamepad ownership, remapping/conflicts, calibration/prompts and saved preferences on named physical devices. Focus loss, disconnect/reconnect and stale input must produce the defined offline/online behavior without stuck actions.
- Exercise keyboard-accessible navigation, visible focus, audio/fullscreen failure handling and quality/accessibility settings. Desktop support comes first; later mobile/touch support must pass these input flows and the M5 device matrix before it is advertised.

**Acceptance depends on:** M2, M3.

**Work packages:** Complete accounts, friends and lobby flows; Accept owned controls and browser usability.

## M5 — Release readiness

**Pass when:**

- Accept M1–M4 and reproduce the candidate from a clean checkout using pinned inputs/tool versions. Required CI and publication checks pass; distributed source/builds and diagnostics contain no original media, generated title code or credentials.
- Freeze and test the required desktop Chrome/Edge/Firefox/Safari and named hardware matrix. Record full-tournament soak/relaunch/save/recovery, frame times toward 60 fps with a usable documented 30 fps mode, peak memory, import time and failure behavior against budgets fixed before acceptance runs.
- Verify protocol/content/rules/converter versioning, deployment/rollback, backup/restore, capacity/admission limits, privacy/data handling and release documentation. Resolve licensing/content-release questions and authorize service costs/deployment before publication; later mobile releases pass the same applicable gates.

**Acceptance depends on:** M1, M2, M3, M4.

**Work packages:** Verify reproducible source-only release operations; Accept browser/device performance and release evidence.

[Management workflow](management.md) describes status, native parent/dependency links and the evidence required before acceptance. Scope is maintained in [profiles/project.json](../profiles/project.json); current runtime facts remain in [status](status.md).
