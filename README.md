# Fuzion Frenzy Web Port

Experimental static recompilation of the original Xbox **Fuzion Frenzy to a web browser**, with planned online multiplayer, mixed local/online parties, flexible controls, friends and lobbies.

Each browser will use compatible game files supplied by its player and converted locally. Public source provides independently written tooling, runtime components and research prototypes.

**Original-game playability, complete local conversion, online play and the complete product are not available yet.** Bounded original startup is recorded in [status](docs/status.md).

[Documentation](docs/README.md) · [Development plan](plans.md) · [Milestones](docs/milestones.md) · [Project graph](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/) · [Project board](https://github.com/users/Jdiller-86/projects/3) · [Contributing](CONTRIBUTING.md)

## Project progress

[![Project progress](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/gates.svg?v=delivery-issues-1)](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/)

Refreshed after tracking updates and hourly from [native issues](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues?q=is%3Aissue+label%3Amilestone). Open the graph for interactive task scope, acceptance criteria and dependencies. Task completion and full milestone acceptance are shown separately.

The public [GitHub Project](https://github.com/users/Jdiller-86/projects/3) organizes milestone gates, work packages and tracked delivery issues. Open the linked chart for the full hierarchy, sizes, scope and native issue links. Its bars count delivery issues; package status and milestone acceptance remain separate. Counts describe tracked scope, not remaining effort or a completion percentage.

Reviewed source changes and issue events refresh tracking; hourly reconciliation and graph publication recover missed events. Each report shows its source revision and snapshot time. [Management](docs/management.md#synchronization-and-progress) explains which record to update, how generated views stay consistent, and how unavailable tracking is shown.

<!-- project-goals:start -->
## Goals and next work

| Milestone | Remaining product outcome | Acceptance and live status |
| --- | --- | --- |
| **M1: Original browser execution** | Play one original minigame through its original menus, round and results entirely in a desktop browser. | [Criteria](docs/milestones.md#m1--original-browser-execution) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/22) |
| **M2: Complete local game and conversion** | A fresh browser converts the supported player-supplied image and runs the complete original offline game. | [Criteria](docs/milestones.md#m2--complete-local-game-and-conversion) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/25) |
| **M3: Online multiplayer and recovery** | The original game supports authoritative remote and mixed local/remote matches with recoverable tournament results. | [Criteria](docs/milestones.md#m3--online-multiplayer-and-recovery) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/28) |
| **M4: Player experience** | Players can set up their content and controls, find friends, join lobbies and play/rematch through the supported browser interface. | [Criteria](docs/milestones.md#m4--player-experience) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/31) |
| **M5: Release readiness** | Ship a reproducible, documented and operationally supported release for the declared browser/device matrix. | [Criteria](docs/milestones.md#m5--release-readiness) · [Native issue](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/34) |

**Recorded development:** M1 / awaiting-evidence, runtime checkpoint **44**. [Runtime evidence](docs/status.md) distinguishes the accepted checkpoint from local candidates. Native issues own live milestone acceptance.

**Current implementation assignment:** Use the existing Fuzion SDK to get original execution working in the browser. Spend at most the first 30 minutes on a matching desktop reference, then build, run and fix actual browser blockers. Reach the first original frame and continue toward a controllable original menu or selected minigame. Defer detailed tests, broad matrices, per-caller audits and repeated historical comparisons until the baseline works. At 150 minutes give a brief progress update and continue productive implementation; this is not a stop or delivery promise. See Goal G under #49.

**M1 order: largest ready item first.** Select one scenario (MB01), settle the required interfaces, and pull small prerequisites forward only to enable the next large proof.

| Order | M1 work / structural size |
| --- | --- |
| **1 — SDK integration and execution bottlenecks** | [MB04](docs/m1-browser-work.md#mb04), [MB05](docs/m1-browser-work.md#mb05), [MB06](docs/m1-browser-work.md#mb06), [MB07](docs/m1-browser-work.md#mb07) · **M–L / L** |
| **2 — Original graphics, including an early title frame** | [MB11](docs/m1-browser-work.md#mb11), [MB12](docs/m1-browser-work.md#mb12), [MB13](docs/m1-browser-work.md#mb13), [MB14](docs/m1-browser-work.md#mb14), [MB15](docs/m1-browser-work.md#mb15), [MB16](docs/m1-browser-work.md#mb16), [MB17](docs/m1-browser-work.md#mb17) · **M / M–L / L** |
| **3 — Original audio** | [MB18](docs/m1-browser-work.md#mb18), [MB19](docs/m1-browser-work.md#mb19) · **L / M** |
| **4 — Finish the selected startup and resource path** | [MB02](docs/m1-browser-work.md#mb02), [MB03](docs/m1-browser-work.md#mb03), [MB08](docs/m1-browser-work.md#mb08), [MB09](docs/m1-browser-work.md#mb09), [MB10](docs/m1-browser-work.md#mb10) · **S / M–L / M** |
| **5 — Original input and controllable menu** | [MB20](docs/m1-browser-work.md#mb20), [MB21](docs/m1-browser-work.md#mb21), [MB22](docs/m1-browser-work.md#mb22) · **M / S–M** |
| **6 — Complete the original round/results/return** | [MB23](docs/m1-browser-work.md#mb23) · **M–L** |
| **7 — Bounded corrections and M1 acceptance** | [MB24](docs/m1-browser-work.md#mb24) · **S–M** |

[All 24 browser connections, evidence and exit checks](docs/m1-browser-work.md) · [Largest-first details](docs/m1-browser-work.md#largest-first-implementation-order) · [All work-package delivery briefs](docs/work-packages.md) · [Next local-agent assignment](docs/local-agent-handoff.md#bounded-goals-and-selection). These rows decompose existing scope; they are not an additional task estimate or a completion percentage.

**Tracked scope:** 51 delivery issues in 10 work packages under five milestone gates. Counts are not effort estimates.

[Tracked delivery issues](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues?q=is%3Aissue+label%3Awork%3Adelivery) are native children of the work-package issues. The README chart and Project use their live status; package closure and milestone acceptance remain separate.
<!-- project-goals:end -->

## Use or build

New development outputs: the [full-title SDK browser candidate](docs/sdk-browser.md)
links the uploaded title source with the full SDK runtime and software renderer,
with local ISO intake, keyboard/gamepad input and browser audio. The separate
[Windows installer and launcher](desktop/windows/README.md) builds the native
port from a player-supplied ISO/XISO or extracted folder and exposes display
settings, build progress and local saves/logs. These candidates do not advance
the accepted browser checkpoint; original browser playability remains unproved.

[Download the Windows builder preview](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases/tag/windows-builder-v0.1.4-preview.3).
Supply your own compatible game files; the installer contains no game content.

[Download the browser preview](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases?q=browser).
Extract the ZIP, run **Start Browser.cmd**, and select your local ISO in the
browser. This software-renderer build has compiled successfully; original
browser gameplay and performance remain unverified here.

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
