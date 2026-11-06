# Risks and unresolved questions

These are the material gaps between the current research and the [product goal](../DEVELOPMENT_PLAN.md). They guide bounded work; they are not reasons to stop unrelated source-owned progress. [Current status](PROJECT_STATUS.md) owns the latest execution boundary.

| Risk | What is established | Evidence needed to close it |
| --- | --- | --- |
| Browser runtime completeness | Bounded original startup advances through translated routines and explicitly modeled services | Original output, sustained frames, input-driven screen, audio, full subsystem coverage, and original-reference comparisons |
| Synthetic device state | A checked aperture can retain known-written values and stop on unknown reads | Actual device semantics where needed; storage/read-back is not GPU readiness, DMA, rendering, or hardware fidelity |
| Memory and ABI correctness | Source-owned checked memory, stack/service contracts, and fixtures exist; the earlier return-address fault is repaired | Coverage of reached mappings, aliases, permissions, call/return effects, scheduling, and failures; retain independent checks |
| Graphics fidelity | NV2A geometry prepasses have bounded native/Wasm/WebGL2 evidence | Integrated original command execution, raster/depth/state correctness, and title output; desktop GL/Vulkan assumptions do not establish browser compatibility |
| Arithmetic/timing | Measured component profiles expose host arithmetic differences | Title-level reference calibration; do not hide outcome-changing differences behind unvalidated tolerances |
| Audio and controllers | Source-owned adapters and synthetic browser/component checks exist | Original-program device events, real audio timing/quality, owned virtual controllers, and physical-device/browser tests |
| End-user conversion | Local XISO/asset and private prepared-runtime proofs exist | Browser-local title conversion for compatible builds, bounded memory/time, cancellation, validation, and clear/reselect; no native helper |
| Authority versus recompilation | The plan requires server-owned outcomes; the runtime executes original instructions in each browser | A faithful, reviewable authority/input/state contract that keeps original-containing RAM, code, assets, and save states local |
| Network fairness | No production online match implementation exists | Mixed-seat ownership/prediction, timing/contact/steering/orb scenarios, audio fairness, reconnect, and authoritative result agreement |
| Full parity inventory | Static records/manual/footage provide leads and partial conversion evidence | Matching-build catalog, variants, original AI, all modes/characters/zones, menus, audio/video, and save/tournament scenarios |
| Reference provenance | Emulator/replacement-kernel models can support investigation | Distinguish synthetic compatibility defaults from original-console observations; retain unresolved provenance and stop unsupported paths |
| Browser performance | Compiler and component checks run without establishing title performance | Named hardware/browser matrix, frame/memory/import measurements, long sessions, and failure/recovery cases |
| Local-content privacy | Public servers/builds are allowlisted; original inputs and derived artifacts are excluded | Preserve no-upload behavior through future diagnostics, caches, reporting, online state, and production delivery |
| Reproducibility | Public fixtures/CI do not require original media | Document private tool/input prerequisites separately; never report missing private-input execution as a public test pass |
| Rights and licensing | Player-local content and source-only publication are project boundaries | Resolve remaining code/upstream/content/branding/music release questions; no project code license has been selected |
| Delivery and operating cost | No reliable full-runtime schedule or production service plan is established | Re-estimate from measured work and capacity; obtain authorization before service spending or a separate public deployment |

These risks guide future work; no implementation task is assigned at the baseline. Use [research notes](RESEARCH.md) for consolidated facts and source/fixture limits. Earlier investigations and resolved defects remain in Git history.
