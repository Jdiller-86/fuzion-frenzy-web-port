# Compatible reference build and conversion coverage

These are retained reference facts for the current source. They identify the inspected input and bounded conversion coverage; they do not establish authenticity, broad compatibility, or full-game parity.

## Reference identity

| Item | Retained fact |
| --- | --- |
| Executable | `default.xbe`, SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Header | `XBEH`, image base `0x10000`, entry `0x00030f7f` |
| Certificate | Title ID field `0x4d530002`; title text “Fuzion Frenzy” |
| Supplied folder | 136 files, 2,083,933,828 bytes; recorded comparison matched the retained XISO |
| File identities | [reference-files.json](../profiles/reference-files.json) contains paths, sizes, and SHA-256 digests without game bytes |

The filename's USA label is not an authenticity or signature check. The public browser research workflow supports the known uncompressed reference XISO; other layouts, revisions, regions, loose folders, or compressed inputs need separate validation.

[The inventory tool](../tools/inventory-reference.mjs) can reproduce file metadata from an already supplied local folder. Do not acquire or upload media to reproduce it. Exact inputs and raw analysis stay local.

## Conversion coverage

The public source contains bounded XDVDFS/XISO and XBP readers, character/animation/geometry/texture conversion, and limited audio decoding. Recorded browser preview work demonstrated an animated Dub, representative arena geometry, and a locally decoded cue. That does not accept the full catalog, every material/actor/effect, original timing/camera, complete audio, or arbitrary package support.

The retained static character findings use serialized inverse-bind matrices, explicit palette IDs, the measured quaternion convention, and the palette's reserved register gap. Landscape/surroundings transforms and nested initial-key hierarchy also affect the preview. Those are static/converter findings, not observed original gameplay. The current source and source-owned fixtures retain the executable contracts: [character](../host/character.mjs), [animation](../host/animation.mjs), [geometry](../host/obe.mjs), and [textures](../host/texture.mjs).

Historical warm-cache preview/import snapshots are not a supported-device performance guarantee. The product still needs bounded peak memory/time measurements and end-user conversion acceptance.

## Inventory required for parity

For every actual selectable activity/variant, record the compatible build, displayed name, modes/teams/seats, rules, scoring/ties, controls, movement/collision/hazards, objectives, AI, camera/audio, assets, reference scenarios, and acceptance limits. Filenames, embedded strings, public footage, and manuals provide leads rather than verified numerical mechanics.

Public descriptions suggest roughly 45 minigames plus Frenzy, six characters, and six zones. Only the compatible shipped inventory can close [full parity](compatibility.md). No complete playable catalog is accepted.

Earlier detailed findings remain in Git history. Current evidence boundaries are consolidated in [research notes](research.md); the accepted main-branch execution stop belongs in [status](status.md).
