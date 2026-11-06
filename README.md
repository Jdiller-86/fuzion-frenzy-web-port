# Fuzion Frenzy Web Port

Experimental static recompilation of the original Xbox **Fuzion Frenzy to a web browser**, with planned online multiplayer, mixed local/online parties, flexible controls, friends and lobbies.

Each browser will use compatible game files supplied by its player and converted locally. Public source provides independently written tooling, runtime components and research prototypes.

**Original-game playability, complete local conversion, online play and the complete product are not available yet.** Bounded original startup is recorded in [status](docs/status.md).

[Documentation](docs/README.md) · [Development plan](plans.md) · [Milestones](docs/milestones.md) · [Project graph](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/) · [Project board](https://github.com/users/Jdiller-86/projects/3) · [Contributing](CONTRIBUTING.md)

## Project progress

[![Project progress](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/gates.svg)](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/)

Refreshed after tracking updates and hourly from [native issues](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues?q=is%3Aissue+label%3Amilestone). Open the graph for interactive task scope, acceptance criteria and dependencies. Task completion and full milestone acceptance are shown separately.

Five milestones cover original browser execution, the complete local game and converter, online multiplayer/recovery, player experience and release readiness. Each has a foundation, explicit acceptance criteria and a small work breakdown. **0/5 were accepted at the replanning baseline.**

[Native tracking issues](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues?q=is%3Aissue+label%3Amilestone) own current acceptance state. The public [GitHub Project](https://github.com/users/Jdiller-86/projects/3) organizes the five milestone parents and ten initial child tasks using CTR-2-WUP's field/view configuration. The [tracking workflow](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/workflows/project.yml) synchronizes the board and exports progress; [graph publication](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/workflows/publish-progress.yml) updates the public report. Runtime implementation is paused pending the next user-directed task.

## Use or build

Use **Node.js 24.14.0** and **npm 11.x** (pin 11.9.0):

```sh
git clone https://github.com/Jdiller-86/fuzion-frenzy-web-port.git
cd fuzion-frenzy-web-port
npm ci --ignore-scripts
npm run check
npm run dev
```

Open <http://127.0.0.1:4174/> for the retained offline Twisted System prototype. Its limited asset workflow requires the supported uncompressed USA XISO supplied locally. Selecting an image does not launch the original game.

The separate conversion preview uses `npm run dev:research` on port 4173. The source-only startup harness uses `node tools/serve-startup-proof.mjs` on port 4180 and requires privately prepared local inputs. See [development](docs/development.md) for controls, build/preview commands and check limits.

## Source map

| Path | Purpose |
| --- | --- |
| [`host/`](host/) | Bounded local file readers and asset preparation modules |
| [`translator/`](translator/) | Guarded recompilation preparation, memory instrumentation and admission verification |
| [`renderer/`](renderer/) | Source-owned geometry components and research preview rendering |
| [`runtime/core/`](runtime/core/) | Portable C memory, devices, kernel/service and scheduling components |
| [`runtime/browser/`](runtime/browser/) | Browser adapters, input, workers, admission and reports |
| [`web/`](web/) | Research app, conversion proof pages and independent prototype rules |
| [`profiles/`](profiles/) | Reference-file identities and reviewed project scope |
| [`tests/`](tests/) | Independent fixtures and verification |
| [`tools/`](tools/) | Build, delivery, analysis, validation and project tracking |
| [`docs/`](docs/README.md) and [`plans.md`](plans.md) | Current contracts, evidence, acceptance and development direction |

The organization and planning approach follow [CTR-2-WUP](https://github.com/Jdiller-86/CTR-2-WUP), adapted for browser execution. Private original content stays in ignored `research/`; downloaded/private tools stay in `.tools/`. Generated reports/builds stay in ignored output directories.

## Contribute and content notice

Start with [CONTRIBUTING.md](CONTRIBUTING.md) and the linked milestone/work issue. Keep observations, proposed behavior and limits explicit.

This independent project is unaffiliated with Microsoft or Blitz. No game downloads or original asset packs are supplied. Player possession does not establish blanket redistribution rights; see [content policy](docs/content.md). A license for newly written project code has not been selected. The package `private` flag prevents npm-package publication.
