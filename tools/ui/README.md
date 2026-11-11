# Original UI investigation tools

The inventory is now also consumed by the working [native UI prototype](../../docs/native-ui.md).
Use `native-pack.py` with the existing decoded images/pilot and the
[incremental native builder](BUILD-NATIVE-UI.md); the feature batch does not need
another full extraction. Original-font recovery remains bounded and separate.

These source-only tools reuse the repository's XDVDFS, XBP, texture, XBE and
bounded OBE readers. They never execute target-provided text or modify the
supplied game files. Original inputs, resource names/details, images, captures,
generated instruction indexes and recipes stay in Git-ignored output directories.

Run from the latest Windows-rendering-fix checkout. Requirements are project-pinned
Node **24.14.0**, Python 3 with **Pillow** (the recorded pilot used 12.2.0), Git,
the compatible player-supplied loose game tree/disc, and the matching retained
native generated source. No game data or generated title code is supplied here.
The native reference is pinned to `071dbd97bdc098c4647cc24c99f9eae693fde1eb`;
SDK `58757ab255c0a6b3929f405d7a5f1567fcd7fda3` plus the rendering patches in
publication `93ec83293921dfcf1ff4b8f0c759aa62189c330b`. Current raw file hashes
are retained separately; a copied source directory is not assigned its enclosing
repository's Git revision.

## Regenerate the artifacts

```powershell
./tools/ui/Build-Inventory.ps1 `
  -SourceTree '<loose supplied game folder>' `
  -NativeSource '<matching retained fuzion-native source>' `
  -SdkSource '<matching SDK with published rendering repairs>' `
  -DiscImage '<the disc used by the native reference>' `
  -RuntimeExe '<working FuzionFrenzy.exe>' `
  -OutputRoot '<project>/artifacts' `
  -NodeExe '<Node 24.14.0>/node.exe'
```

This runs extraction → full package metadata census → original-source reference/vtable index → bounded geometry
views → font-container check → evidence join → disc/input verification → static
replacement comparisons and component preview → provenance hashes → catalog.
It preserves any existing `ui-inventory/captures/` observations. Runtime capture
is a separate manual computer-use activity; this command does not launch the
game, reproduce navigation, alter settings, or invent missing observations.

The wrapper validates input/output separation and requires Git-ignored outputs.
It writes no files under the original source tree. New output paths must be
ignored before use; it deliberately refuses a public or input-overlapping path.
To rerun an individual step, use that script's arguments; dependent indexes carry
input digests, so regenerate downstream evidence after changing the inventory.

## Open the private catalog

```powershell
node tools/ui/server.mjs --root '<project>/artifacts/ui-catalog' --port 4176
```

Open `http://127.0.0.1:4176/`. Keep the server terminal alive; stop it with Ctrl+C.
The server binds loopback and serves only generated manifest entries. It offers
no uploads, directory listing, arbitrary file access, CORS or external requests.
Do not substitute a generic repository file server.

Search/filter by package, effective confidence, type or screen; open a resource
for source offsets, hashes, alpha/mips, code references, full-size PNGs, frames
and geometry-derived rectangle overlays. Confirmed uses and inferred component
bindings are labeled separately. Contact sheets embed private PNGs and can be
opened as standalone SVGs; the printable index links each package sheet.

The catalog's HTTP routes, allowlist and data were checked. Visual browser QA
was blocked because Windows computer use could not enforce URL policy for the
installed browser; it is not reported as a passing visual check.

## Output map and scope

| Private output | Contents |
| --- | --- |
| `ui-inventory/inventory.json`, `images/` | Selected-package resource accounting and original-resolution RGBA frame previews |
| `package-census.json` | All 89 XBP indexes and identities, with name-based leads explicitly separated from decoded resources |
| `source-references.json` | Exact ASCII/resource-pointer reference index, 64 consumed shard hashes and 28 mode method pointers |
| `overlay-evidence.json` | Every attempted whole-object and isolated-leaf parse, bounds, material joins and failures |
| `font-evidence.json` | Five font-name resource slices verified against package hashes; container/glyph limitations |
| `evidence-joins.json` | Effective UI confidence, screen/component bindings and exact geometry-derived texture rectangles |
| `verification.json` | Coverage balance, PNG dimensions, index freshness and runtime disc package matches |
| `run-identities.json` | Actual XBE, native executable, disc, source/tool/reader and capture hashes |
| `captures/observations.json`, `captures/*.png` | Actual native screen observations; no runtime facts synthesized by extraction |
| `ui-catalog/` | Offline local catalog, allowlist, copied previews and contact sheets |
| `ui-replacements/pilot/` | Three static textures, 2x nearest controls and 4x premultiplied-Lanczos review images; no installation |
| `ui-replacements/settings-preview/` | Explicitly proposed read-only settings composition and deterministic recipe |

The extraction's declared scope is **14 named candidate packages, 939 resource
entries**: 548 unique decoded textures, 38 duplicate entries, and 353 unsupported
by the texture decoder. The XBP reader also excludes 12 string-table metadata
rows across the selected packages; those rows are counted separately. There are 586 unique
frame PNGs. These are accounting categories, not claims that every texture is UI.

Evidence joins classify **5 confirmed resources** (two layout resources and three
textures), **59 probable** and **875 unclassified**. There are five full-texture
geometry rectangles, not recovered font-atlas cells. The catalog includes seven
observed screen categories and 14 explicitly named package leads. Geometry
inspection attempted all 45 OBE resources in that extraction scope: 17 whole
objects parsed, 28 failed, and 123 isolated leaves parsed while two failed.
Whole-object and leaf views overlap and must not be summed as distinct assets.

The wider metadata-only census indexes all **89 XBP packages**, returning **9,660
resource entries** from 9,745 table rows (85 metadata rows omitted). All indexes
parsed; this census decodes no payloads. Its 726 resource-name leads and 42
additional candidate packages are leads, not UI confirmations. It includes 25
additional font-name rows in 14 unselected packages; their container/glyph formats
remain unexamined. Use the census to scope the next extraction rather than
presenting the 14-package decoded set as complete UI coverage.

No complete original UI inventory is claimed. Dynamic font/glyph mapping,
localization IDs, complete animation tracks/timing, global overlay transforms,
pause/result source paths, per-draw blend/clipping and a full mode registry remain
unproved. Native observations do not establish original-console or browser parity.
The full investigation and proposed implementation seams are in
[UI architecture](../../docs/ui-architecture.md),
[settings integration](../../docs/ui-settings-integration.md),
[upscaling](../../docs/ui-upscaling.md), and the source-only
[component manifest](../../profiles/ui-components.json).

## Next implementation batch

Own only native `src/common/overlay.cpp`, `input_kbm.cpp`, `game_info.cpp` and a
new owned UI asset-provider module; deliver the four existing renderer settings
through the current F1 draft/apply/save path with deliberate keyboard/controller
focus capture. Prove the three asset bindings at the decoded-pixel/native UI
boundary, retain fence-aware resource ownership, and leave the existing guest
Options route intact. The Y/F1 entry hint from original Options depends on a
validated active-mode observer and is a separate substep. Observe open/change/
cancel/save/reopen and input isolation before accepting it. Browser UI integration,
font replacement and physical texture-size changes remain separate dependencies.
