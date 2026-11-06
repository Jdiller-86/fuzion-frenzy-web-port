# Fuzion Frenzy Web Port

An independent research project working toward the original Xbox **Fuzion Frenzy running entirely in a web browser**, with online multiplayer, mixed local/online parties, flexible controls, friends, and lobbies.

The selected route is static recompilation to WebAssembly. Each browser will use compatible game files supplied by its player and converted locally. This repository contains independently written tools, runtime components, and research prototypes; it provides no game downloads or original assets.

**Status — November 6, 2026:** bounded original-program startup has been demonstrated in browser research. A controllable original screen, complete runtime, full fidelity, and online play have not been delivered. See [current status](docs/PROJECT_STATUS.md) for the exact observed boundary and its limits.

[Documentation](docs/README.md) · [Roadmap](DEVELOPMENT_PLAN.md) · [Contributing](CONTRIBUTING.md) · [Research index](docs/RESEARCH.md)

## What the project is building

- Original behavior and presentation across the complete verified catalog, characters, arenas, and modes.
- One to four total player seats, with solo, local, online, and mixed local/online play.
- Remappable keyboard/gamepad controls, owned devices, reconnects, and later mobile/touch support.
- Friends, invitations, lobbies, tournament progress, and authoritative online results.
- Browser-local content processing, with no original-file upload, peer transfer, or hosted asset packs.

The original game is the fidelity baseline. The retained Twisted System reconstruction is a research prototype; it does not satisfy that goal. [Scope and fidelity](docs/SCOPE_AND_FIDELITY.md) defines acceptance, and the [roadmap](DEVELOPMENT_PLAN.md) separates the runtime milestones from the remaining product work.

## Explore the public source

Use **Node.js 24.14.0** and **npm 11.x** (package-manager pin: 11.9.0).

```sh
git clone https://github.com/Jdiller-86/fuzion-frenzy-web-port.git
cd fuzion-frenzy-web-port
npm ci --ignore-scripts
npm run check
npm run dev
```

Open <http://127.0.0.1:4174/> for the offline Twisted System research prototype. Its limited local asset workflow requires the supported uncompressed USA XISO supplied by the player. Without compatible content, you can inspect the shell and run source-owned checks. Selecting an image does not launch the original game.

The separate conversion proof runs with `npm run dev:research` at <http://127.0.0.1:4173/>. Neither command builds or provides an original-title runtime module. Private developer toolchains and game-derived modules are absent from a public clone.

See the [development guide](docs/DEVELOPMENT.md) for controls, build/preview commands, prerequisites, and what each check verifies.

## Find your way around

| Path | Purpose |
| --- | --- |
| [`apps/web/`](apps/web/) | Offline research app and local asset worker |
| [`packages/`](packages/) | Prototype simulation and keyboard/gamepad input |
| [`src/`](src/) | Format readers, converters, and C/JavaScript runtime components |
| [`proof/`](proof/) | Browser conversion and bounded-runtime research harnesses |
| [`tests/`](tests/) and [`tools/`](tools/) | Source-owned fixtures, preparation, verification, and analysis tooling |
| [`docs/`](docs/README.md) | Current guides, milestone definitions, and retained evidence |

The implemented client uses plain JavaScript modules, HTML/CSS, and C runtime components. The larger client/server/social stack remains proposed; see [architecture](docs/ARCHITECTURE.md).

## Contribute

Start with [CONTRIBUTING.md](CONTRIBUTING.md), then choose a bounded task tied to the current roadmap. Reproducible source-owned fixtures, parser/runtime fixes, reference-backed findings, and documentation improvements are useful contributions. Keep runtime observations separate from hypotheses and prototype behavior.

## Project and content notice

This is an independent project, unaffiliated with Microsoft or Blitz. Players must supply compatible files; possession does not establish blanket rights to redistribute game content, branding, or music. See the [content policy](docs/PLAYER_SUPPLIED_CONTENT.md).

A license for newly written project code has not been selected. Public visibility does not itself grant reuse rights. The `private` flag in `package.json` prevents npm-package publication; it does not make this GitHub repository private.
