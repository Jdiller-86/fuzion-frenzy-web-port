# Original-game scope and local-content contract

## Product boundary

The product target is the original Fuzion Frenzy executing entirely in the browser through the selected static-recompilation route, with original behavior and presentation preserved. An identical replica would meet the fidelity requirement only if every minute detail were demonstrated; an approximate remake or selected minigame pack does not.

The original game files are supplied by each player. A browser imports and converts them locally. Original files and derived proprietary assets stay on that device. No game-file upload, server or peer distribution, remote conversion, telemetry attachment, or cloud sync is part of the content path. This contract governs product design; it is not a legal conclusion or blanket permission claim.

See [PLAYER_SUPPLIED_CONTENT.md](content.md) for import constraints and [plans.md](../plans.md) for the delivery roadmap.

## Fidelity inventory

Full parity means accounting for the verified shipped inventory and required original scopes, including:

- Every selectable minigame and variant, with its rules, scoring, timing, movement, collision, hazards, objectives, original AI/difficulty, and transitions.
- The original modes and progression, including Tournament and Minigame Frenzy, practice, stage counts, and Frenzy-round behavior where present in the shipped copy.
- All original characters, zones/arenas, and their gameplay-relevant layouts, camera behavior, and presentation.
- Original menus, audiovisual behavior, and other title-defined flows needed for a complete experience.
- Original save behavior and complete round/tournament state, progression, and recovery scenarios.

Public product descriptions and the manual provide reference leads, not a verified inventory for the supplied build. Establish the exact build and catalog from retained evidence before claiming completeness. Preserve unresolved items as unknowns; do not fill gaps with invented timings, mechanics, assets, or formats.

## Evidence and acceptance

Fidelity claims require evidence tied to the original program or a controlled reference, with observed conditions and limits recorded. Static analysis, public footage, synthetic profiles, component fixtures, and visually similar assets can support investigation but do not independently prove original runtime behavior or parity. A behavior is not accepted merely because it looks plausible.

Offline original behavior is the baseline. Online authority, prediction, latency handling, remapping, accessibility, and browser presentation may add behavior, but must preserve the original rules and identify user-visible differences. Do not describe network behavior as frame-identical couch play without evidence.

Full parity is accepted only when the verified inventory is complete and each required entry and mode has an explicit evidence-backed disposition. A partial research prototype or startup milestone does not count as a product release.

## Browser and distribution constraints

- Game execution and local-content conversion occur in the browser. Native helpers and remote/streamed game execution do not satisfy the requirement.
- Support compatible player-provided content; state format/build limits clearly and fail safely on unsupported input.
- Avoid whole-disc memory buffering for large images; storage may be unavailable or evicted, so users need clear local-content removal and reimport paths.
- Servers must not require proprietary level data or original assets to be redistributed. A conflict between faithful online rules and the local-content boundary is an architecture gate to resolve openly.
- The public repository and hosted application must not contain original ROMs, disc images, executable files, extracted asset packs, or game pseudocode. Keep private analysis material in ignored research locations.

## Unknowns to resolve with evidence

The exact shipped catalog and variants, complete asset-format/conversion coverage, runtime compatibility, browser conversion performance, and full fidelity remain unaccepted until demonstrated. Record each finding in the public evidence index and link it to the relevant milestone in [plans.md](../plans.md). Never turn an unresolved item into an assumed rule or permission.
