Terra 6.1 is the primary development executor. This file provides a bounded start point; `DEVELOPMENT_PLAN.md` provides the broader roadmap. Read the root `AGENTS.md` and `PROJECT_STATUS.md` before working.

Current audited result: G0's bounded identity/inventory deliverable and the G1 browser-local conversion proof are verified for the supplied profile. Read [the requirement-by-requirement audit](G0_G1_CHECKPOINT.md) first; older entries below are research history and do not override it. The proof renders animated Dub and a representative stadium with three conveyor/stair instances, and consumes one decoded cue. The expanded run measured 0.15 seconds and a 29.7-30.0 MiB page/worker snapshot. Originals remain local and unchanged; source publication guards and format checks passed. Exact gameplay/animation timing, complete scene rendering, audible-quality judgment, peak working memory, other builds and the release browser matrix are not accepted.

The latest corrections replace the assumed animation-zero bind pose and linear bone mapping with serialized inverse binds and the original split palette convention, apply previously ignored constant arena rotations, and add bounded initial-key hierarchy support for the three visible stairs. The [file manifest](reference-files.json) contains metadata only for all 136 supplied files and reproduced byte-identically. A Clear local preview control releases imported state and playback; browser rejection/clear/reimport checks passed. See the inventory for local evidence paths and public frame timestamps.

Next task: the bounded G2 Twisted System reference specification, before gameplay implementation. Read `PROJECT_CONCERNS.md`; establish the instruction/control/state/scoring scenarios and measurable action/hazard cases from available references. Keep unobserved timing unknown. No broad content production, engine freeze or production services follow automatically from G1.


**Accepted decisions.** Use `gpt-6.1-terra`; desktop keyboard/gamepad first; first public pack four to six games; full original-game expansion later; touch/mobile later. The proposed client is TypeScript/Vite/Babylon.js, with shared headless rules and dedicated Node/Colyseus servers. Hosting and auth vendors are not purchased/provisioned.

Latest public-reference evidence: read `PUBLIC_REFERENCE_OBSERVATIONS.md`. The browser inspection yielded a 45-entry displayed catalog, including the three proposed named minigames, and a timestamped stadium scene. Exact-build identity and controlled timing remain unknown. Next compare the local stadium camera/material presentation with the 31:54 view, and inspect the recording's character-selection animation. Preserve these public observations separately from local binary findings.

The camera comparison now has an opt-in orthographic axis/pitch view; it is diagnostic and not fidelity acceptance. `REFERENCE_INVENTORY.md` records both tested directions and a texture/vertex-alpha lead. Next use the existing Ghidra project to inspect the renderer's material/vertex setup; avoid guessing opacity from appearance. The public reference and local partial proof are still available investigation paths.

Motion-start regression found during browser recheck: a same-frame rAF timestamp could yield negative elapsed time. The proof clamps elapsed time to zero; actual motion start/continued poses/pause were verified after reimport. The browser also returned a 27.0 MiB memory snapshot. These are scoped proof observations, not original timing or peak-memory sign-off.

Read-only renderer investigation now has bounded symbol and assembly scripts and retained local findings. See the inventory for addresses and output paths. Packed diffuse color and separate passes are corroborated, but alpha normalization is not: the deferred texture-state slots differ from current Xbox reference definitions. Continue by identifying the build's SDK state layout and flag/pass mapping. Do not implement guessed half/double alpha from decompiler snippets.

The executable declares D3D8 build 3925 (inventory F07). Use that version for deferred-state-layout research. Current Cxbx header values are not automatically valid for this older SDK. The local library-version probe and renderer evidence already exist; do not repeat setup or import.

Cxbx's version-aware texture-state converter resolved the old slot mapping. The proof's opt-in material profile now applies double modulation to decoded vertex colors and experiments with a flag-1 blended pass. Browser rendering and synthetic color/flag tests succeeded. Treat pass mapping/sorting/alpha testing/lighting as provisional; preserve the default until those differences have reference evidence. The inventory links the exact external source and records implementation limits.

Correction: transparency now follows the serialized third material pass, not primitive flag 1. The flag-only rule failed the broader scene-resource probe. Read the latest inventory findings for pass counts, static addresses and browser verification. The expanded scene still has lighting/framing differences; the default remains separate and no fidelity gate was accepted.

Character clip metadata is now read by `src/clips.mjs` and selectable in the proof. The earliest imported range is 1..61; a separate range 2080..2090 was also played in the browser. Do not restore the arbitrary 0..60 loop. Original speed is still unknown (raw step field 17); the UI deliberately labels its diagnostic rate. Continue with time integration and public motion comparison, reusing the existing static outputs listed in the inventory.

Timing path is now located: `0x00036e70` integrates a queued step and delta; `0x0002cc90` produces a display-field-based delta normalized to 60 updates, with speed factors. Original clip-start calls can override the serialized step. Do not equate raw 17 with fps. Continue at the actual selected reference clip's start call instead of retracing the integrator or clock producer; evidence paths are in the inventory.

The latest hash probe supplies a candidate `select` clip at keys 728..881, alongside walk/run/jump candidates. These are independently hash-matched leads, not runtime-confirmed labels; the UI intentionally keeps numeric names. The generic menu loader's `in` default is not proof of the character-selection action. Continue with the selection candidate and its actual call-site multiplier; preserve rate uncertainty until compared with observed motion.

The specific character frontend setup now corroborates `idle_charselect`, imported keys 667..727, with an explicit 1024 step. This is the proof's default static 60-unit idle profile; other clips remain diagnostic at 30. Clock tests and actual browser playback passed. Continue with Dub's public selection idle and deformation comparison, not another search for the generic menu's `in` action. See inventory for exact addresses and the fixed-point loop convention.

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

The subsequent surroundings diagnostic is described in the [inventory](REFERENCE_INVENTORY.md): the preview's reversed depth orientation exposes the arena and background, but two triangles are omitted for invalid UVs and the original rendering is unverified. The default scene remains unchanged. Continue with the LND-referenced sky and material/depth interpretation; use the existing local probe and project, not a new extraction. Matching reference and continuous peak-memory measurements remain gate inputs.

The resumed resource audit found 21 distinct OBE references in the selected LND, including three shared-package resources. The opt-in diagnostic now includes the locally decoded sky at the documented translation and was imported in the browser. Eleven stadium resources exceed the flat-node reader's support, and another fails texture support; full-level coverage remains unverified. Reuse the ignored resource audit and its JSON report for the next targeted node/material investigation. Tests and checks for this follow-up are recorded in current status; no new CI result is implied.

Completion audit now identifies the matching-reference input as the blocker to G0/G1 acceptance; see the checkpoint's exact capture checklist. Do not keep extending speculative level decoding as if it verifies fidelity. The goal can resume meaningfully when matching local captures or a user-supplied reference runtime are available. Keep peak-memory and broader converter coverage as explicit independent follow-ups, without adding full-level or release-matrix requirements to the one-set G1 proof. All current local source/evidence changes are preserved; no new push was performed.

Correction after user steering: inspect the public NTSC Xbox longplay linked in the checkpoint before requesting local captures. Its chapter metadata supplies arena and catalog leads. This route has not yet been visually or audibly inspected; preserve revision/timing uncertainty and distinguish uploader claims from observed footage. Public-reference investigation can proceed without a user-owned capture setup.
