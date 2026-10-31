# Twisted System: G2 reference and frozen scenario set

Prepared October 31, 2026, before gameplay implementation. Revision TS-R1 freezes the implementation policies and comparison cases below. Implementation completion and G2 fidelity acceptance are separate decisions. Unverified values are configurable reconstruction parameters, not recovered original timings.

## Identity and sources

The supplied executable is inventory F01, SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The selected package is `Packages/hop-v4.xbp`, 17,053,696 bytes, SHA-256 `c3875b31803a92172a06e889351d609246aa03dc4fa99355a1c9fe9a82b013d6`. Read it in place; no extraction is needed.

Primary public observation: [Shamrock Spud gameplay recording](https://www.youtube.com/watch?v=Oi5jWuJHZUo). Its title claims hard difficulty and 1080p60; platform, executable identity, input history and source capture cadence are unverified. The player's frame-step command advanced its media clock by about 1/30 second per command; that does not prove the original simulation or recording cadence. No video was downloaded and no local game bytes were uploaded.

| Time in recording | Observed evidence | Boundary |
| --- | --- | --- |
| 0:05 | Instruction goal describes surviving the cooling rods by jumping or ducking | No numerical action windows shown |
| 0:07 | A maps to Jump, B to Duck; Start and Practice are displayed | Hold-versus-pulse duck behavior is not established by the text |
| 0:09-0:22 | Intro shows the spiral reactor, four characters across the track and three CPU seats | Camera choreography is not part of this offline browser implementation |
| 0:27 | All displayed scores are zero; low and high obstacles approach along the spiral | Approximate active-play baseline for the counts below |
| 0:37 | Four scores read 8; blue low obstacle and orange overhead obstacle are visible | One successful clearance per point is inferred, consistent with later counts |
| 0:47 | Displayed scores read 16 | Sampled progress, not a measured collision boundary |
| 0:57 | Scores 27, 25, 26, 26; characters differ in pose/position | Misses do not increase the displayed clearance score |
| 1:07 | Scores 48, 43, 46, 47; obstacle rate is visibly faster | Exact acceleration curve remains unknown |
| 1:17 | Scores 70, 66, 66, 68 | Same rate/progression limitation |
| 1:27 | Green/blue remain on track with scores 92/91; red/pink scores are 81/82 and characters are off the track | Eliminated scores stop progressing |
| 1:32 | Player 1 wins; displayed final scores 101, 81, 82, 99 | Does not exercise an original tie |

Secondary lead only: the [Twisted System wiki](https://canyouwinagain.fandom.com/wiki/Twisted_System) describes seven misses before elimination and practice continuing after only one survivor remains. This has not been independently established by a controlled original run. Use those as explicitly inferred defaults; do not call the thresholds verified original behavior.

Local static corroboration (direct Ghidra, not REA Evidence or observed execution): the module constructor at `0x0013b990` names Twisted System as Hopper's fourth variant. Resource initializer `0x00140730` references `v4/drill.obe`, `v4/drillob01.obe`, `v4/drillob02.obe`, and the corresponding package sounds. Variant dispatch `0x0013de00` selects `0x0013d9f0` for variant four. The latter positions players using their current knockback angle and handles an off-track state. These observations establish resource association and a positioning lead, not numerical hit or timing equivalence. The attempted `0x001430a0` and `0x00141bc0` leads concern water/camera presentation; do not treat them as collision rules.

Local-only reproduction/evidence is under `research/verification/`: `twisted-package-probe.mjs`, `twisted-package-leads.json`, `twisted-asset-probe.mjs`, `twisted-asset-coverage.json`, `twisted-character-probe.mjs`, `twisted-character-profile.json`, `twisted-clip-string-matches.json`, and the `twisted-*.txt` Ghidra records. Original pseudocode is not copied into application source.

## Asset coverage and material concerns

The existing readers decode the spiral, low/high obstacle resources and arena geometry. Their material hashes resolve against the local package, with any unresolved background material requiring explicit handling before rendering. The minigame-specific Dub model has 4,929 vertices and 26 bones; 2,623 vertices use two weights. The G1 one-weight restriction has been extended with source-owned blend tests rather than replacing the original model. Named clip IDs match local executable strings for run, jump, fall, land, hurt, victory and defeat; a dedicated duck clip has not been identified. Any procedural duck presentation must be labeled a visual approximation.

Known fidelity gaps reported before affected gameplay implementation: exact hazard order/RNG, acceleration, hit geometry, recovery, action timing, held-duck behavior, original simultaneous ties and original solo/practice termination are unresolved. They do not prevent a playable implementation, but they prevent verified G2 fidelity acceptance. Do not substitute a fan remake's rules or assets.

## Frozen TS-R1 implementation policy

Use a headless fixed-step simulation at 60 ticks/second, separate from DOM, renderer, devices and wall clock. It accepts semantic jump-press/duck-held commands for one to four owned local seats. Keyboard/gamepad adapters must deliver those commands independently of rendering rate. A fixed step is not a cross-runtime determinism claim.

| Behavior | TS-R1 policy | Evidence status |
| --- | --- | --- |
| Objective | Clear low rods by jumping and high rods by ducking; +1 clearance per successful obstacle | Observed/inferred from instructions and score samples |
| Misses/elimination | One miss per obstacle; seven misses eliminate a seat | Inferred default, configurable |
| Normal multiplayer end | Resolve after processing the same obstacle for every live seat, once at most one remains | Last survivor observed; atomic simultaneous resolution is an explicit reconstruction policy |
| Solo | Continue until the solo seat is eliminated; report its score | Browser solo policy; original solo lifecycle unverified |
| Practice | Continue with the last survivor until all are eliminated or practice is ended; show unranked practice results | Secondary lead plus explicit browser exit policy |
| Scoring/ties | Scores freeze on elimination. Rank by clearances; equal scores share rank. Simultaneous final elimination with equal best scores is a shared win | Original ties unverified; policy must remain disclosed |
| Countdown/results/rematch | Three-second countdown; immutable resolved results; rematch creates fresh round state with the same seats/device ownership and newly chosen seed | Browser lifecycle policy; countdown value configurable |
| Jump | Edge-triggered, no held-key auto-jump; provisional 0.40-second action, protected only in its middle window | Unverified, configurable |
| Duck | Hold-to-duck with provisional 0.06-second entry delay; no protection while airborne | Unverified, configurable; retain a pulse-mode option in rules configuration for later comparison |
| Hazard progress | Initial 0.8 obstacles/second, ramp after 25 active seconds over five seconds to 2.2/second | Approximate fit to the sampled score progression, not recovered speed logic |
| Hazard ordering | Reproducible seeded low/high sequence; same sequence for all seats | Original order/RNG unverified; this is a material fidelity deviation |
| Focus/disconnect | Clear all held/pending actions and pause; require explicit resume and released controls before rearming a lost device | Intentional browser reliability behavior |

No arbitrary round time limit, online behavior, extra minigames, friends or lobbies. One imported character with distinct seat indicators is the G2 baseline; complete roster parity remains a later requirement. Keep all uncertain numeric parameters in a named configuration and display a concise research/fidelity notice in the application.

## Scenario freeze and tolerances

These cases are frozen before simulation code is introduced. Exact expectations below test the independently written policy. They do not turn unobserved original behavior into verified fidelity.

| ID | Scenario and observable result | Comparison/tolerance |
| --- | --- | --- |
| TS01 | Instructions name low/high actions and both input mappings; compatible local content required before play | Exact meanings and no content bypass |
| TS02 | No input: miss each arriving low rod; one miss per obstacle; eliminate at configured limit; score remains zero | Exact policy result; original miss limit remains inferred |
| TS03 | Jump just before, at and after configured protected boundaries | Exact tick endpoints in fixtures; compare observed original boundary within one measured reference frame only when controllable evidence exists |
| TS04 | Hold duck for high rod; stand or jump into it; duck into low rod | Correct high/low distinction; configurable entry/pulse windows; original action-window fidelity pending |
| TS05 | Two/four seats hit or clear the same final obstacle; reverse seat iteration order | Same scores, elimination set and shared winners; no first-seat advantage |
| TS06 | Practice retains one survivor; all eliminated or explicit exit produces practice results | Exact TS-R1 transitions; original practice semantics require further primary evidence |
| TS07 | One seat completes a solo run; two and four seats complete normal rounds | Complete countdown -> active -> resolution -> results with no deadlock |
| TS08 | Rematch after a round, repeated same-seed input replay | No previous score/action/elimination leaks; identical replay snapshots for the same rules/seed/commands in the tested runtime |
| TS09 | Held keys then blur/visibility loss; regain focus and resume | Simulation pauses and commands clear; no phantom jump, duck or catch-up hazards |
| TS10 | Owned gamepad disconnects/reconnects while held; duplicate ownership attempted | Commands clear, game pauses, device cannot control two seats, reconnect requires release/reassignment as documented |
| TS11 | One to four owned keyboard layouts or mixed keyboard/gamepad seats; editable menu focus and repeated keydown | Correct seat only; no repeated jump from held key; text entry remains usable |
| TS12 | Compare roughly 10/20/30/40/50/60 active-second no-miss progress with footage samples | Provisional investigation envelope: +/-2 clearances through 30 seconds, +/-4 afterward. Passing is an approximate progression comparison, not acceptance of the original speed curve |
| TS13 | Original drill, obstacle silhouettes and imported character visible during actual browser play | Reference visual comparison; disclose camera, shading, duck-pose and effect deviations rather than claiming pixel equivalence |

The existing scope's proposed one-frame action boundaries and 5% motion tolerances remain fidelity targets where measurable. TS-R1's provisional progression envelope does not relax them into a verified gate. Record implementation browser runs, actual hardware/device availability, comparisons and deviations in a separate G2 acceptance audit.
