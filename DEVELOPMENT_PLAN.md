# Project roadmap

Build the original Xbox Fuzion Frenzy into a complete browser experience through static recompilation to WebAssembly, then deliver the planned online, social, and control features. Original behavior and presentation are the baseline. Native helpers and streamed execution do not meet the browser-only requirement.

The route was selected in [decision D023](docs/DECISIONS.md). Earlier approximate-reconstruction and partial-pack assumptions are retired; they do not define completion. The offline Twisted System prototype remains research.

## Current position

Implementation is paused at the main-branch baseline pending the next user objective. The sequence below defines future acceptance; no task is currently assigned.

As of November 6, 2026, the latest recorded experiment executes bounded original startup and stops at an untranslated routine. The former return-address/stack fault has been repaired and M6 is recorded met. No controllable original screen, complete runtime, full-game parity, or online service is accepted. [Current status](docs/PROJECT_STATUS.md) owns exact counts, stops, and evidence; this roadmap owns delivery goals.

## Product requirements

| Area | Required result | Present position |
| --- | --- | --- |
| Original game | Complete verified minigame/variant catalog, characters, arenas, rules, AI, menus, camera, animation, audio, and save behavior | Startup research; parity unaccepted |
| Original modes | Practice, individual games/Minigame Frenzy, and full Tournament progression, including Frenzy rounds and original stage choices | Unaccepted |
| Players | One to four total seats; solo/local, remote, and mixed local/remote parties; original CPU behavior and difficulty | Prototype local seats only |
| Content | Each browser identifies and converts compatible player-supplied files locally; clear/cancel/reselect and storage recovery | Bounded preview and runtime proofs; end-user conversion incomplete |
| Controls | Per-seat keyboard/gamepad ownership, remapping, conflict detection, calibration, prompts, reconnect, and no stuck input | Reusable prototype input; original-runtime integration unaccepted |
| Online play | Server-owned scores, collisions, elimination, pickups, randomness, and round transitions, with responsive client presentation | Unimplemented; runtime/authority contract unresolved |
| Lobbies | Create/discover/join, privacy, codes/invites, readiness, owner handoff, rematch, and compatible-content/version checks | Unimplemented |
| Accounts/social | Recoverable accounts, friends/requests, invitations, privacy, block/report, and authorized data access | Unimplemented |
| Persistence/recovery | Saved settings and atomic tournament checkpoints/results; idempotent commits, reconnect, seat reclaim, and separately defined match-process recovery | Unimplemented |
| Browser experience | Audio/fullscreen handling, quality/accessibility settings, browser/device compatibility, and measured performance | Research only |
| Release operations | Versioned protocol/rules/converters, safe delivery/rollback, backups, load/admission controls, and approved operating costs | Source guards/CI exist; production delivery unaccepted |
| Later devices | Mobile/touch usability after desktop-browser feasibility | Later work |

There are four seats per match, even when one browser owns several. An account, browser connection, local profile, and player seat are distinct identities. An in-app friends list does not assume Xbox Live integration.

Public descriptions indicate roughly 45 minigames plus Frenzy, six characters, and six zones. The exact compatible build's selectable inventory and variants determine parity; a marketing count is not an acceptance checklist. See [scope and fidelity](docs/SCOPE_AND_FIDELITY.md) and [reference inventory](docs/REFERENCE_INVENTORY.md).

## Delivery milestones

Milestone IDs are retained so existing decisions and checkpoint links remain meaningful. They describe evidence gates, not calendar promises. The [runtime goal](docs/RUNTIME_MILESTONES.md) defines detailed criteria and dependencies.

| Milestone | Observable result | Recorded state |
| --- | --- | --- |
| M1 — Device-table startup | Reached original graphics initialization advances through the device-table boundary | Recorded complete within its bounded profile |
| M2 — Device policy | Known-value provenance and fail-closed unknown device reads, with paired evidence and additional boundary advances | Recorded met for the modeled policy |
| M3 — Graphics-cluster return | The reached graphics cluster returns and the next subsystem request is observed | Unaccepted |
| M4 — Presentation request | An original-program presentation-storage request and result are observed | Unaccepted |
| M5 — Original screen | An original title/menu screen responds visibly to player input | Unaccepted; M10 supplies the stricter acceptance gate |
| M6 — Stack/return repair | The experiment passes the former fault while the control stays invariant | Recorded met; decision D429 and commit `ad0bffa` |
| M7 — Original output write | Original commands produce a provenance-backed write to a named output surface | Unaccepted |
| M8 — Sustained frames | Eight consecutive bounded frame-equivalent cycles with output and no new stop | Unaccepted |
| M9 — Audio/control event | An original-program audio or controller event reaches its admitted device boundary | Unaccepted |
| M10 — Controllable original screen | M5 original screen, M7, M8, M9, and an input-driven transition with paired reports/audit | Unaccepted |
| M11 — Runtime coverage | Each required runtime group has an explicit evidence-backed coverage disposition | Unaccepted; unknown/not-reached items do not prove full compatibility |
| M12 — Complete product | Original browser execution, full local-content parity, and the planned feature requirements pass acceptance | Unaccepted |

M6 is a repaired blocker, not a substitute for M3–M5. The original program determines which subsystem is reached next; numbering does not justify forcing execution past unsupported behavior. M7–M12 remain open.

## Sequence and acceptance

1. **Advance original startup.** Admit only reached, byte/decoder/layout-verified functions; implement evidence-backed services for actual requests. Compare control and experiment, preserve fixed resource limits, and stop on the next unsupported dependency.
2. **Demonstrate original output and control.** Establish output provenance, sustained execution, actual input-driven behavior, and audio/controller boundaries in a normal desktop browser.
3. **Close runtime and content gaps.** Complete graphics, audio, scheduling, memory, disc/save semantics, and the end-user browser-local conversion path. Track every original inventory item and required mode.
4. **Resolve and implement online authority.** Demonstrate server-owned outcomes without uploading proprietary RAM, assets, executable data, or save states. Prove remote and mixed-seat cases before expanding across mechanic families.
5. **Complete the surrounding product.** Deliver lobbies, social/account flows, persistence, recovery, controls, and release operations, then accept the full product against the original reference.

Source-owned prerequisites may overlap these stages. Broad content production or social UI polish must not hide an unresolved execution or authority gate.

## Validation targets

These are engineering targets to measure and calibrate, not observed original-console facts or delivered performance:

| Area | Acceptance work |
| --- | --- |
| Fidelity | Controlled matching-build scenarios for rules, score/ties, timing, movement, collisions, hazards, camera, audio, AI, and round/tournament transitions |
| Network | Four remote seats and mixed two-local/two-remote cases; identical authoritative outcomes over at least ten rounds; exercise 150 ms RTT, 30 ms jitter, 1% loss and characterize worse conditions |
| Mechanic families | Separate timing, contact/knockback, steering/vehicle, orb ownership/banking, and music/pattern fairness cases |
| Input/recovery | Physical controllers, keyboard rollover, remapping, focus loss, disconnect, stale inputs, seat reclaim, owner transfer, and process recovery |
| Browser/performance | Named Chrome/Edge/Firefox/Safari and hardware matrix; target 60 fps with a usable 30 fps quality mode; measure frame times, memory, import latency, audio, and long tournaments |
| Content processing | Bounded worker reads, malformed/unsupported inputs, cancellation, quota/eviction, clear/reselect, and no content uploads |
| Authority/data | Concurrent joins, authorization isolation, no overbooking or duplicate rewards, atomic results/checkpoints, and no client-submitted winners |
| Operations | Source-only artifacts, protocol/version compatibility, load/capacity, deployment rollback, backups, and cost review before provisioning |

The earlier proposals of a 128 MiB importer working budget and a 120-second first-activity import remain unvalidated planning budgets. Website download time and local import time are separate measurements. No reliable completion date is established; re-estimate from measured runtime, conversion, and representative gameplay work.

## Architecture and scope decisions

The public implementation is plain JavaScript modules and C components. TypeScript/Vite/Babylon.js, React menus, Colyseus match rooms, and PostgreSQL/auth persistence were proposed, not installed or proven. Original recompilation does not automatically supply a deterministic shared server simulation. Resolve that compatibility and authority question before committing to the network design; see [architecture](docs/ARCHITECTURE.md) and [risks](docs/PROJECT_CONCERNS.md).

Optional modernization must preserve an original baseline. PWA/offline caching, daily challenges, and custom rule presets are later enhancements. Ranked ladders, voice/free-form chat, larger parties, spectators, and Xbox social integration require separate scope decisions.

## Keep the plan current

Update [status](docs/PROJECT_STATUS.md) and [handoff](docs/DEVELOPMENT_HANDOFF.md) when a future task changes the runtime boundary. Record consequential decisions in [DECISIONS.md](docs/DECISIONS.md) and consolidate current findings in [research notes](docs/RESEARCH.md). Earlier logs remain in Git history. No milestone closes through documentation alone.
