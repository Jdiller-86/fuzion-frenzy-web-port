# Fuzion Frenzy Web Port

Planning and development preparation for a faithful browser-playable reconstruction of the original Xbox game, with online multiplayer, local/online mixed parties, flexible controls, friends, and lobbies.

Source repository: [Jdiller-86/fuzion-frenzy-web-port](https://github.com/Jdiller-86/fuzion-frenzy-web-port), public and source-only by user direction.

The `private` flag in `package.json` prevents npm-package publication; it does not control GitHub repository visibility.

**Status: preproduction research. There is no playable web build yet.** The accepted first release is a desktop pack of four to six polished games, followed by expansion to full original-game parity. Mobile touch support follows desktop stability.

Start with [AGENTS.md](AGENTS.md), [development handoff](docs/DEVELOPMENT_HANDOFF.md), [current status](docs/PROJECT_STATUS.md), and [project concerns](docs/PROJECT_CONCERNS.md). The full architecture, milestones, estimates, and acceptance criteria are in [DEVELOPMENT_PLAN.md](DEVELOPMENT_PLAN.md).

Players will supply their own compatible game files for local browser conversion. No ROMs or original assets are hosted, uploaded, or shared between players. An experimental local conversion proof exists; it supports only the specific verified reference build and is not a finished importer. See [player-supplied content](docs/PLAYER_SUPPLIED_CONTENT.md) and the [scope/fidelity baseline](docs/SCOPE_AND_FIDELITY.md). The visual goal is to mirror or reuse locally imported originals with optional modernization, while preserving original game feel.

## Development environment

Use Node **24.14.0** and npm **11.x**. There are no application dependencies yet.

```powershell
npm ci --ignore-scripts
npm run check
npm run check:environment
```

`check` validates preparation files and prevents common accidental source publication mistakes. It does not test gameplay. The research spike has its own commands:

```powershell
npm run test:formats
npm run dev:research
```

The latter serves only allowlisted source files on `127.0.0.1:4173`. Open that address locally and select the supported uncompressed XISO; conversion runs in the browser, and the server does not receive the image. The proof renders a selected character and stadium candidate and decodes one cue. It is experimental: visual and animation fidelity, the complete content inventory, broad format support and performance sign-off remain open. See [reference inventory](docs/REFERENCE_INVENTORY.md) for observations and limits.

Local preparation checks pass. Both public-repository Windows/Linux preparation jobs also [passed](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41212045874), resolving the earlier private-runner billing blocker for this workflow. Details are in `docs/PROJECT_STATUS.md`.

The project's Codex configuration selects `gpt-6.1-terra` with high reasoning. Trusted project settings provide defaults; an existing desktop chat's selected model can take precedence. Select **GPT-6.1 Terra** in the desktop model control, or launch `codex -m gpt-6.1-terra`. No OpenAI API key is needed by the game or the preparation scripts.

## Analysis environment

The original workstation has a tested project-local Ghidra/Java/XBE toolchain. See [analysis setup](docs/ANALYSIS_SETUP.md) for verified capabilities and commands. These large downloads and local research inputs are not included in the repository; cloning it does not install them automatically.

REA supports the configured Ghidra provider for its admitted formats. Xbox XBE analysis currently uses direct Ghidra through [the project wrapper](tools/Invoke-ProjectGhidra.ps1). The verified check covers the original executable's entry point, not complete game behavior.

## Repository contents

- `docs/`: handoff, decisions, status, risks, and local analysis documentation.
- `tools/`: source-owned preparation checks, analysis wrapper, research server, verification scripts, and pinned tool metadata.
- `src/`, `proof/`, `tests/`: experimental local content readers, browser preview, and source-owned synthetic format checks.
- `.codex/config.toml`: project model defaults; no machine paths, credentials, or permission overrides.
- `.github/workflows/prepare.yml`: preparation checks on Windows and Linux.

Future application boundaries are `apps/web`, `apps/game-server`, and shared `packages/*`; their implementation has not begun.

This repository contains no original game media or distributable original assets. It is an independent project and is not presented as an official Microsoft/Blitz release. Player-supplied files do not establish blanket permissions for branding, music, reverse engineering or redistribution. The license for new project code remains undecided; no license grant is asserted by this README.
