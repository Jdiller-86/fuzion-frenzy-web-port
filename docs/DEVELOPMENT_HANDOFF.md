Terra 6.1 is the primary development executor. This file provides a bounded start point; `DEVELOPMENT_PLAN.md` provides the broader roadmap. Read the root `AGENTS.md` and `PROJECT_STATUS.md` before working.

**Accepted decisions.** Use `gpt-6.1-terra`; desktop keyboard/gamepad first; first public pack four to six games; full original-game expansion later; touch/mobile later. The proposed client is TypeScript/Vite/Babylon.js, with shared headless rules and dedicated Node/Colyseus servers. Hosting and auth vendors are not purchased/provisioned.

The user also selected required player-supplied files, local-only asset conversion, public source-only hosting, and modernized visuals mirroring or reusing original assets. Read `PLAYER_SUPPLIED_CONTENT.md` and `SCOPE_AND_FIDELITY.md`: complete the reference/import/offline/network gates before full-force content production. The asset importer does not establish blanket legal permission and does not execute an Xbox binary in the browser.

**Current evidence.** REA 6.7.0 and a Ghidra provider are configured. Direct Ghidra successfully imported the actual XBE and decompiled its entry; this is an analysis toolchain test. It establishes no particular minigame timing, score rule, art format, or browser performance claim. The original planning-pass limitations are historical; use `ANALYSIS_SETUP.md` for the newer setup facts.

The user supplied `Fuzion Frenzy (USA)/`, now verified against all 136 regular files in the existing XISO. Use it directly instead of extracting another copy. It includes 89 XBP packages and six LND zone-description files, but internal models/textures/animation/audio conversion remains unproved. Read `PROVIDED_FOLDER_ASSESSMENT.md` before requesting more input or running extraction.

**Model/environment.** `.codex/config.toml` defaults to Terra with high reasoning for trusted project sessions. It does not replace the model in an existing desktop chat. Select Terra in that chat before execution. The local catalog listed the exact model ID as available during preparation; verify if the account/client changes. Do not substitute another model silently. No model API call, API key, or paid service is part of application setup.

**Repository.** Public source-only repository: https://github.com/Jdiller-86/fuzion-frenzy-web-port, branch `main`, as requested by the user. Local preparation checks pass. The first private-repository CI attempt was blocked before execution; read current status for public workflow verification instead of assuming visibility alone fixed it.

**G0 inventory progress.** [Reference inventory](REFERENCE_INVENTORY.md) records executable identity and certificate fields, file categories, local evidence, capture scenarios, and unresolved catalog and fidelity questions. G0 remains open; the previously proposed first-pack activities are not confirmed from this build. The browser conversion experiment and its limits are described below. Do not repeat extraction or reimport the XBE.

**Remaining phase 0 reference work.** Continue the following as evidence becomes available, before building affected game systems:

1. Inspect current local research without reimporting/decompiling everything. Check target digests and persistent Ghidra project availability.
2. Record executable build metadata and an inventory of disc paths/container candidates. Treat filenames as leads, not format or behavior proof.
3. Create a curated, source-safe findings index and content inventory schema. Keep raw target content and pseudocode local. Link each established fact to an evidence record/digest and label inferences/unknowns.
4. Record what reference runtime/captures are available. If none, identify the missing input; do not claim play-tested rules or acquire game/firmware downloads.
5. Choose a first minigame candidate only from established inventory/reference information. Write its questions and acceptance criteria, including the timing values still unknown.

Acceptance: a reproducible inventory procedure, verified executable identity, evidence-linked findings, a filled unknowns list, and one bounded next task. No unsupported claims about recovered original source, complete content parity, or original-content distribution. Update status/decisions and run preparation checks. This task does not include writing 45 minigames or setting up production infrastructure.

**Then prove the foundation.** Approve the route through measured spikes before broad content work: renderer with a placeholder scene; headless fixed-step authority; one/multiple local seats per online connection; input/prediction/acknowledgment behavior; and one representative placeholder/content conversion path. A source scaffold is useful only when it supports these proofs. Introduce exact dependency versions and lockfile updates with these tasks.

The first gameplay sequence is timing -> contact -> steering -> orb objectives, then broader pack production. Add the audio/pattern-family gate before shipping those games. Offline/local behavior and server rules share an interface, not a claim of bitwise determinism. Keep game frame state out of the database.

**Handoff after every task.** Record the task, evidence/changed files, exact checks and outcomes, unfinished work, decisions, and next task. Avoid replacing uncertainty with optimistic completion labels. Keep the master plan's release scope separate from small task acceptance.

OpenAI references used for preparation: [project configuration](https://learn.chatgpt.com/docs/config-file/config-basic), [AGENTS.md discovery](https://learn.chatgpt.com/docs/agent-configuration/agents-md), [model selection](https://learn.chatgpt.com/docs/models).


**Current research spike.** Run `npm run test:formats` for synthetic input checks and `npm run dev:research` to start the loopback-only, allowlisted inspection page. The local browser has identified the supplied XISO and renders a partial scene and character. See [reference inventory](REFERENCE_INVENTORY.md) for exact observations, external format lead and limits. One mono audio cue decodes in the browser and matches a local FFmpeg PCM comparison, though audible quality has not been independently observed. Both G0 and G1 remain open. Do not treat index timing as total import time or memory evidence.


The existing Ghidra project now has a completed time-limited automatic analysis and a focused OBE relocation lead; static pseudocode and outputs remain local. See the inventory's static lead and scripts before further analysis. The initial sandboxed invocation could not write the normal Ghidra user directory; setting `APPDATA` only for the PowerShell process to `research/ghidra-user` allowed reuse without changing global settings. Follow the type-2 node and geometry path; do not claim decoded rendering from relocation alone.


**Latest browser experiment:** the loopback-only page now displays textured stadium geometry and a textured character animated from locally sampled tracks, and decodes one mono cue. The narrowly accepted executable and four packages are hash checked. Its timing/memory snapshot and synthetic failure results are in [reference inventory](REFERENCE_INVENTORY.md); visual fidelity, actual audio audition, original timing and peak memory remain open. Continue from these artifacts; do not treat G0 or G1 as passed. Matching original gameplay/reference captures and a verified selectable catalog are still missing.


Read [G0/G1 checkpoint](G0_G1_CHECKPOINT.md) for the current requirement-by-requirement evidence and missing reference/hardware inputs. The identity helper has negative synthetic tests; browser cancellation and altered-package rejection were verified. The local synthetic image used for the latter includes the supplied executable and stays under ignored `research/verification/`.


The default stadium preview includes landscape and wall only. A separate surrounding-geometry diagnostic can omit two triangles with extreme UVs, but its current view hides the arena; it is not included in the proof. Identity rejection and cancellation/reimport were checked in the browser. Continue against the checkpoint, including a real reference and peak-memory profiling; do not call the partial scene complete.


The local proof supports Repeat import and optional paused-stage memory samples; see the inventory for reproducible timing, cache and coverage distinctions. Profiling deliberately pauses the worker and its elapsed time is not a speed benchmark. Neither diagnostic proves peak memory or the target-machine matrix.


The user confirmed neither original Xbox capture nor configured emulator is currently available. Do not repeatedly ask the same setup question; continue independent work and identify the missing matching reference at the gate. [CI run 41223835447](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41223835447) passed `npm run test:formats` and preparation checks on Windows and Linux for `e85dda5`. Next independently investigate the stadium surroundings' transforms and extreme UVs using bounded local evidence; full-scene fidelity still requires a matching reference.
