Prepared October 31, 2026. Update this file with each completed development task; do not treat an old tool check as proof of current availability after environment changes.

| Workstream | Status | Evidence / next action |
| --- | --- | --- |
| Product plan | Prepared | `DEVELOPMENT_PLAN.md`; staged release and desktop-first choices accepted |
| Model handoff | Prepared | Root instructions, Terra project defaults, bounded next task |
| Local analysis tools | Verified in setup session | REA 6.7.0, Ghidra 12.1.4, JDK 21, matching XBE loader; see `ANALYSIS_SETUP.md` |
| Original executable analysis | Partial static analysis | Time-limited Ghidra analysis completed; focused loader and animation hypotheses; runtime behavior unverified |
| User-supplied content folder | Verified against local XISO | All 136 files matched; no missing/changed/extra files; use it directly, with XBP decoding still pending; see `PROVIDED_FOLDER_ASSESSMENT.md` |
| Original build/content inventory | G0 partial | [Reference inventory](REFERENCE_INVENTORY.md): matching executable digest, certificate fields, file classes and candidate leads; revision, selectable catalog and runtime reference remain unresolved |
| Runtime gameplay evidence | Unresolved | Hardware/emulator/captures not established |
| Player-supplied content | Design selected; implementation pending | Local browser conversion, no uploads/hosted originals; see `PLAYER_SUPPLIED_CONTENT.md` |
| Scope and fidelity | Baseline prepared before gameplay work | Original rules/game feel, original-inspired/reused local assets, explicit G0-G6 gates; see `SCOPE_AND_FIDELITY.md` |
| GitHub repository | Public visibility verified | Source-only `Jdiller-86/fuzion-frenzy-web-port`; default/tracked branch `main`; initial commit `5a01264`; supplied media excluded from files/history |
| Local preparation checks | Passed | `npm ci --ignore-scripts --offline`, `npm run check`, `npm run check:environment`, PowerShell syntax and Git whitespace/ignore checks |
| GitHub Actions | Passed on public repository | Windows and Linux preparation jobs succeeded for `d7a4d77`, run `41212045874`; no billing changes |
| Browser/client/server/gameplay | Research spike only | Browser locally decodes and previews one textured animated character, stadium geometry/materials and a cue; fidelity and full G1 acceptance remain open; no runnable game or match server |
| Production auth/hosting | Not provisioned | Budget, initial regions, capacity and services remain decisions |

Latest bounded task: G0 evidence index and inventory, with a read-only identity/header and file-class check. No original files were extracted, converted, or run. G0 remains open; see [reference inventory](REFERENCE_INVENTORY.md) for evidence, limitations and capture scenarios. Next: bounded local XBP/LND format reconnaissance toward G1; no gameplay implementation. Checks for this task: `npm run check` passed the preparation contract and publication guard; `git diff --check` passed (Git reported line-ending conversion warnings). These are not gameplay tests.

Repository: https://github.com/Jdiller-86/fuzion-frenzy-web-port

Historical private-repository workflow: https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41209288599. GitHub blocked it before runner startup for account billing/spending restrictions.

After public visibility was explicitly requested and applied, the fresh run https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41212045874 completed successfully on both `windows-latest` and `ubuntu-latest` for commit `d7a4d77`. This resolves the observed CI execution blocker for this standard-runner workflow; no billing settings were changed. Checks validate preparation, not an implemented game. The result-documentation follow-up commit skips redundant CI because no source/workflow code changed after that passing run.


October 31 follow-up: local, bounded XDVDFS/XBP index readers and a browser-worker research page were added. The browser identified the supplied image and indexed one package; the actual image slices for the executable, selected package and LND matched prior SHA-256 identities. All 89 supplied XBP indexes were scanned in Node. Synthetic malformed-input checks cover the new readers. This is not a character/arena/audio conversion, a memory benchmark, or a G1 pass. See [reference inventory](REFERENCE_INVENTORY.md) for provenance, limits and candidates. A subsequent mono audio cue decoder matches FFmpeg sample for sample and runs in the browser; audible quality has not been independently observed. Next: investigate candidate OBE mesh/animation data and zone geometry, preserving local-only content. Runtime capture and the selectable catalog remain unresolved for G0.


Direct Ghidra analysis of the existing project completed within the configured timeout; targeted static lookup found an OBE relocation lead and one LND-to-package arena candidate. These remain hypotheses, not recovered source, visual conversion or runtime observation. See [reference inventory](REFERENCE_INVENTORY.md) for addresses, evidence and limits. Next follow the type-2 node path and validate geometry and animation interpretation against multiple resources.


Latest browser spike: the supplied XISO produced a textured stadium geometry preview (6,746 vertices, 3,682 triangles), a textured 26-node character with sampled original key tracks, and a mono cue in a worker. One local browser reported 0.09 seconds from file selection to render and a 27.1 MiB page-and-worker snapshot before the subsequent package-hash checks; a package-checked run reported 0.15 seconds. These are single measurements, not peak or full G1 sign-off. A synthetic truncated image was rejected. The original XISO SHA-256 matched before and after a browser import. See [reference inventory](REFERENCE_INVENTORY.md) for all assumptions, test scope, current limitations and missing reference. Next: verify the current package-checked flow, compare visuals/motion/audio with the matching original runtime, obtain a meaningful peak-memory measurement, and complete G0 selectable inventory. No gameplay implementation is implied.


Package-checked browser follow-up: 0.12 seconds to initial render and a 27.1 MiB page-and-worker snapshot on the documented Ryzen 9 Windows machine; GPU usage, peak memory, cold runs and the proposed integrated-GPU baseline were not established. Web Audio reported playback started; listening comparison is still missing. The XISO digest remained unchanged after the package-checked run. G0/G1 acceptance and all fidelity claims remain pending as detailed in the [inventory](REFERENCE_INVENTORY.md).


The supplied attract video provides a limited local visual reference: a sampled frame shows the imported character's recognizable colors and features. It is edited footage, not a reproducible interactive reference; the selected stadium, timing and catalog still lack matching captures. The source and derived frame identities and limits are in [reference inventory](REFERENCE_INVENTORY.md).


Latest checkpoint: exact selected-content identity is covered by source-owned positive/negative tests. In the browser, an image with the correct executable and an altered package was rejected; cancel cleared the preview and a subsequent valid reimport succeeded. See [G0/G1 checkpoint](G0_G1_CHECKPOINT.md) for requirement-by-requirement status and outside input versus engineering work. Neither gate has passed.


The experimental stadium preview now includes the LND-referenced wall in addition to the landscape (9,382 positions and 5,000 candidate triangles). One surrounding asset was rejected by current format checks and remains excluded; visual fidelity remains unverified. The local browser reimported the expanded scene in 0.12 seconds and reported a 27.1 MiB page-and-worker snapshot, still not a peak or target-machine measurement. See [reference inventory](REFERENCE_INVENTORY.md) and [checkpoint](G0_G1_CHECKPOINT.md).


The latest browser reimport after the identity and cancellation changes completed in 0.17 seconds to initial partial-scene rendering. All format/identity tests and the publication guard passed. A diagnostic surroundings pass identified three extreme UV values affecting two triangles and was excluded from the default view because it obscured the stadium. A representative full scene and matching reference remain unresolved. See [G0/G1 checkpoint](G0_G1_CHECKPOINT.md).


A repeat-import control now records bounded in-page timing history; five runs and two paused-stage browser memory diagnostic runs are documented in the [inventory](REFERENCE_INVENTORY.md). The first cache state is unknown, repeated runs are likely warm, and snapshots do not measure peak or GPU memory. Prior preview GPU resources are explicitly released on reimport. G0/G1 remain open.


Checkpoint validation: `npm run test:formats`, `npm run check`, and `git diff --check` passed; Git reported line-ending conversion warnings. Browser tests covered package mismatch, cancellation/reimport, five timing runs and paused-stage samples. Original media, converted data, raw pseudocode and local logs remain ignored.


The user confirmed that neither an original Xbox capture setup nor an already configured emulator is currently available. Matching interactive reference remains an external input for G0/G1 fidelity verification; do not acquire firmware or game downloads. The XBP reader now caps individual and aggregate names, and XDVDFS validates numeric ranges and overlapping directory entries. All 89 local package indexes, five selected image paths, synthetic format tests and a browser import passed after these changes. [CI run 41223835447](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41223835447) passed on Windows and Linux for commit `e85dda5`, including `npm run test:formats`. This validates the source-owned checks, not gameplay or gate acceptance. Next: investigate the stadium surroundings' transforms and extreme UVs with bounded local format evidence; compare the complete scene only when a matching reference is available.
