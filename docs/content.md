# Player-supplied content

Each connected browser must supply its own compatible original game files. Local seats in one browser share that browser's content context. The project does not host game downloads, original assets, preconverted packs, or generated original-title modules.

This is a distribution/privacy contract, not a blanket permission conclusion. Possession of files does not establish rights to redistribute game content, branding, or music.

## Available research workflow

The public app and conversion proof support a bounded preview for the specific verified **uncompressed USA XISO** reference build. They use local file selection and worker processing. The Twisted System prototype uses locally converted assets and independently written rules; selecting an image does not execute the original game.

The startup research uses a separately prepared module and compatible executable selected locally. Those private developer artifacts and tools are absent from a normal clone. End-user browser-local conversion of the original title is still incomplete.

| Input/path | Current position |
| --- | --- |
| Compatible uncompressed reference XISO | Limited asset/conversion/prototype research |
| Private compatible XBE and prepared Wasm | Bounded original-startup research, not a finished public title runtime |
| Loose game folder | Retained developer analysis input; not a promised browser importer |
| ZIP/7z/compressed images, other regions, modified builds | Separate support decisions and validation required |

See [development](development.md) for the source-only app/proof commands and [research](research.md) for exact coverage. An XISO/XDVDFS reader is not ordinary ISO9660 support.

## Required content boundary

- Original files, extracted/converted assets, generated original instructions/data, raw analysis output, RAM and original-containing save states remain on the player's device.
- No upload, server/peer transfer, remote conversion, telemetry/crash attachment, external enhancement service, account sync, or CDN fallback may supply or collect that content.
- Public source/builds contain independently authored code, source-owned fixtures, and curated non-content findings. Generated title code does not become distributable merely because it is C or Wasm.
- Read selected files with bounded random-access slices in workers. Never buffer an entire large disc image, follow embedded paths beyond the selected input, or execute files as host programs, shell commands, JavaScript/eval, or page modules.
- Validate identities, headers, ranges, counts, dimensions and resource limits. Unsupported/malformed input must fail clearly without partially enabling play.
- A future original-code Wasm runtime is confined to its admitted browser capabilities, memory bounds, local inputs and approved saves. The asset preview's validation does not automatically prove that runtime safe.

## Storage and saves

The current prototype has cancel/clear and discards imported session state; it reports no persistent cache. Research RAM-backed filesystem writes and FATX components do not establish durable user saves.

The product needs optional local persistence with quota/size disclosure, versioned build/converter identities, clear/delete, and reselect/reimport after denial or eviction. Durable save compatibility and recovery need their own evidence. The 128 MiB importer and 120-second initial-import budgets from planning remain unvalidated targets.

## Multiplayer and release

Each browser must pass compatible content/converter/rules readiness before joining a match. Send only the necessary compatibility identifiers, readiness and authorized gameplay protocol data; a digest does not confer ownership or access to another player's files.

The authoritative server must not require redistribution of proprietary levels/assets or upload of original-containing machine state. Resolve the [authority/content design](risks.md) before promising online behavior. Public delivery also requires the complete [scope/fidelity](compatibility.md) and [roadmap](../plans.md) gates.

New code has no selected repository license; this project is independent of Microsoft/Blitz. Earlier policy revisions remain in Git history.
