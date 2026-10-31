The user-supplied `Fuzion Frenzy (USA)/` folder was checked on October 31, 2026. **It is sufficient as the primary source input for the next asset-format/conversion investigation; no further disc extraction is needed for those files.** It is not a browser-ready asset library yet.

The folder contains 136 files totaling 2,083,933,828 bytes. A read-only inventory/checksum comparison against the existing local reference XISO matched all 136 regular files, with no missing, changed, or extra files. Twelve directory checksum records were excluded from the file comparison. This establishes equality with the supplied reference image, not independent authenticity or successful asset decoding.

| Content | Observation | Consequence |
| --- | --- | --- |
| Main executable | `default.xbe`, 2,834,432 bytes; SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` | Same executable previously imported/decompiled successfully; reuse the existing Ghidra project |
| Game packages | 89 `.xbp` files in `Packages/`, including localized subdirectories | Relevant packaged content is present; its internal asset structure/decoding remains unverified |
| Zone descriptions | Six `.lnd` files under `Packages/MetaLevs/`, with readable zone headers | Useful starting points for format/content inventory; not proof of all minigame rules |
| Movies | Four `.bik` files under `Packages/Video/` | Separate from the core playable asset-conversion proof |
| Demo material | `XDemos/` includes another executable, launcher images/audio and demo videos | Do not mistake its loose PNG/WAV/XPR files for the original game's character/arena asset library |
| Zero-filled files | Sixteen `.zro` files, each 64 MiB and matching a zero-filled buffer | Not useful models/textures/audio; exclude from asset-conversion workload |

The 99 regular files under `Packages/` total 673,703,453 bytes. The 89 XBP files account for the bulk of the package inventory; their presence and names do not establish which models, textures, skeletons, animation, sound or collision formats they contain. Those are the next bounded questions. Do not claim there is no parsing/unpacking left to do merely because the disc filesystem is already extracted.

Local verification outputs: `research/verification/reference-image-md5.txt` and `research/verification/provided-folder-assessment.json`. MD5 was used for per-file comparison with the local image; executable identity was additionally checked with SHA-256. No content was extracted or converted during this assessment. File/header observations came from ordinary local tools, not REA Evidence.

Development direction: read this folder directly, reuse the existing executable analysis, inspect one representative package and zone description, and prove a character/texture/animation/arena conversion before claiming the local-content pipeline works. Do not ask the user for another ROM or duplicate the extraction step. Reference gameplay captures/runtime remain a separate fidelity requirement. The developer folder does not automatically establish support for loose-folder input in the final browser importer; that remains a separately tested format choice.

The supplied folder is ignored by Git, and no files from it are tracked or uploaded. Keep raw inventory, headers, package bytes and extracted content under the existing local-only research boundary.
