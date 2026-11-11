# Fuzion Frenzy Web Port

A project to run the original Xbox **Fuzion Frenzy entirely in a web browser** through static recompilation to WebAssembly. Full offline coverage, online multiplayer, friends and lobbies remain project goals.

## Available now

| Build | What it provides | Status |
| --- | --- | --- |
| [Browser preview](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases?q=browser) | Local ISO intake, full SDK runtime, software rendering, keyboard/gamepad input, audio and save adapters | Compiles and packages successfully; original browser gameplay and performance still need verification. |
| [Windows builder 0.2.0](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases/tag/windows-builder-v0.2.0-preview.1) | Builds the native game from your ISO/folder; Control Room launcher with amber progress display, settings, logs and saves access | Published installer; working native gameplay and geometry stability reported by the user/local agent. |

Supply your own compatible USA ISO/XISO. Game content stays on your device; no game assets are included or uploaded. The ISO check verifies compatibility and local access, not legal ownership.

**Browser:** extract the preview ZIP, run **Start Browser.cmd** (Python required), choose your ISO and select **Start game**. [Browser guide](docs/sdk-browser.md).

**Windows:** install `FuzionFrenzySetup.exe`, choose your ISO or extracted folder, and build. The builder installs compiler prerequisites on demand. Includes native F1 settings and three locally generated UI artwork replacements. Games already built with builder 0.1.2 or later do not need rebuilding for this launcher update. [Windows guide](desktop/windows/README.md) · [Control Room](docs/control-room-launcher.md).

## Project progress

[![Project progress](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/gates.svg?v=delivery-issues-1)](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/)

[Project board](https://github.com/users/Jdiller-86/projects/3) · [Issues](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues) · [Current evidence](docs/status.md). The chart and board refresh from reviewed scope and live issues after changes, with hourly reconciliation.

<!-- project-goals:start -->
## Goals and next work

| Milestone | Remaining outcome |
| --- | --- |
| [M1: Original browser execution](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/22) | Play one original minigame through its original menus, round and results entirely in a desktop browser. |
| [M2: Complete local game and conversion](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/25) | A fresh browser converts the supported player-supplied image and runs the complete original offline game. |
| [M3: Online multiplayer and recovery](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/28) | The original game supports authoritative remote and mixed local/remote matches with recoverable tournament results. |
| [M4: Player experience](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/31) | Players can set up their content and controls, find friends, join lobbies and play/rematch through the supported browser interface. |
| [M5: Release readiness](https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/34) | Ship a reproducible, documented and operationally supported release for the declared browser/device matrix. |

**Next:** Run the published browser candidate with the compatible player-local ISO. Fix the first actual execution or performance blocker, then reach a controllable original menu and one complete minigame. Reuse the working native reference and existing adapters; do not restart integration. Defer detailed test suites until the browser baseline works.

[M1 tasks, sizes and remaining connections](docs/m1-browser-work.md) · [Acceptance criteria](docs/milestones.md) · [Agent handoff](docs/handoff.md)

<details><summary>M1 implementation order</summary>

1. [SDK integration and execution bottlenecks](docs/m1-browser-work.md#largest-first-implementation-order) (MB04, MB05, MB06, MB07)
2. [Original graphics, including an early title frame](docs/m1-browser-work.md#largest-first-implementation-order) (MB11, MB12, MB13, MB14, MB15, MB16, MB17)
3. [Original audio](docs/m1-browser-work.md#largest-first-implementation-order) (MB18, MB19)
4. [Finish the selected startup and resource path](docs/m1-browser-work.md#largest-first-implementation-order) (MB02, MB03, MB08, MB09, MB10)
5. [Original input and controllable menu](docs/m1-browser-work.md#largest-first-implementation-order) (MB20, MB21, MB22)
6. [Complete the original round/results/return](docs/m1-browser-work.md#largest-first-implementation-order) (MB23)
7. [Bounded corrections and M1 acceptance](docs/m1-browser-work.md#largest-first-implementation-order) (MB24)

</details>
**Development:** M1 / awaiting-evidence. Accepted startup reference: **M44**; live milestone acceptance is recorded in the linked issues. The chart tracks 51 delivery issues in 10 work packages; counts are not effort estimates or a completion percentage.
<!-- project-goals:end -->

## Development

Use **Node.js 24.14.0** and **npm 11.x** (pin 11.9.0).

```sh
npm ci --ignore-scripts
npm run check
```

The original browser build uses the [SDK build guide](docs/sdk-browser.md). `npm run dev` starts the separate retained research prototype, not the original game.

[Documentation](docs/README.md) · [Roadmap](plans.md) · [Contributing](CONTRIBUTING.md) · [UI inventory and tools](tools/ui/README.md).

Update `profiles/project.json` and `profiles/m1-browser-work.json` when scope changes, then run `npm run project:docs`. Keep current evidence in `docs/status.md` and task labels/evidence in the linked issues. [Tracking rules](docs/management.md#synchronization-and-progress).

Independent project, unaffiliated with Microsoft or Blitz. No game downloads are supplied. A source license has not been selected; see [content policy](docs/content.md).
