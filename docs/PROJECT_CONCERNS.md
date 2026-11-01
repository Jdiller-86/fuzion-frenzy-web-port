Current original-execution concerns follow the user's browser-only/exact-original correction in D016. The older reconstruction and partial-pack entries below are historical wherever they conflict with that correction.

| Current concern | Observed evidence | Required response |
| --- | --- | --- |
| Native runtime reporting crosses the local-content boundary | Pinned xemu `ui/xui/reporting.cc` base64-encodes all XBE headers in `SetXbeData`, serializes them in `GetSerializedReport`, then exposes an HTTP POST in `Send`. This is source behavior, not an observed upload; no game files have been loaded into xemu here. | Browser builds must exclude native report/upload and downloader services before loading user content. The partial browser-host profile uses explicit failure adapters and omits the native GitHub-report menu; its synthetic Wasm boundary check passes. Complete browser integration remains unimplemented. |
| Desktop GPU assumptions | Pinned xemu requests OpenGL 4 shared contexts and generates geometry shaders for common primitives; see `ORIGINAL_EXECUTION_PROGRESS.md` | A real browser graphics port is required. Do not remove required rendering behavior or claim WebGL 2 flags provide parity. |
| Authority with original execution | The plan assumes server-owned outcomes and a shared simulation, while the new candidate executes original code inside each browser | Resolve authority/state/content boundaries before online implementation. Original-containing RAM or save states must remain local; an input relay does not prove server authority. |

The following concerns were reported before gameplay development. They are planning and validation constraints, not a claim that development must wait for every question to be resolved.

| Concern | Current evidence | Development response |
| --- | --- | --- |
| Original-content distribution | User selected player-supplied files with local conversion; no broader rights arrangement established | Public source-only distribution; no hosted/imported asset packs or game uploads. Preserve the remaining branding/music/code questions; importer is not a blanket permission conclusion. |
| Reference fidelity | XBE entry import/decompilation works; gameplay runtime/captures and full rules remain unverified | Establish reference access and evidence-linked minigame specifications before claiming faithful behavior. |
| REA target boundary | Windows provider does not advertise XBE admission | Use the verified direct Ghidra wrapper; no renaming/conversion just to bypass admission. |
| Network game feel | No browser/network prototype exists | Prove timing, contact, steering, and disputed objectives under latency; music/pattern games need a separate audio fairness check. |
| Full catalog workload | Original baseline is roughly 45 minigames plus Frenzy, six arenas and characters; actual shipped inventory still pending | Deliver four to six polished games first; re-estimate from representative mechanic/content work before committing to full parity dates. |
| Staffing and ongoing cost | User has not set staffing, delivery budget, hosting budget or launch regions | Make no paid commitments; measure CPU/memory/egress before capacity and provider decisions. |
| Model continuity | Terra is available and configured as the project default; existing desktop chats retain their selected model | Select Terra for development and keep status/tasks/evidence explicit; model choice alone does not provide runtime evidence or compatibility. |
| Repository reproducibility | Original workstation has local media/tools that clones will not include | Version manifests and procedures; CI must run without game media or private tools. |
| Code licensing | No license for newly written project code has been chosen | Do not invent a license or imply original game content is licensed by this repository. |
| GitHub Actions availability | Resolved for current workflow: both public Windows/Linux preparation jobs passed in run `41212045874` | Keep standard public runners and source-only CI; no billing settings changed. Reverify when workflow/dependencies change. |
| Local ROM conversion and visual fidelity | Browser importer and proprietary asset formats remain unproved | Complete G0-G4 in `SCOPE_AND_FIDELITY.md` before broad catalog work; preserve original asset identity locally and measure import memory/time. |

Placeholder implementation and source preparation can progress independently of the original-content release decision. Public distribution, spending on services, and unsupported fidelity promises must not be implied by a successful setup check.

Reference distribution policy: [Microsoft Game Content Usage Rules](https://www.xbox.com/en-us/developers/rules), reviewed during planning; this does not establish authorization for the port.
