# Control Room Windows launcher

The selected Control Room concept is implemented in the native Windows builder
starting with **0.2.0**. The earlier 13 design prototypes remain available under
`desktop/windows/design/`; they are not deleted or silently restyled.

## Information layout

The main window has no navigation tabs, large heading/banner or slogans. It uses
one equipment bank with three bays, followed by utility buttons and the optional
build-log panel. Platform/version remain at bottom right.

| Location | Information/action | Why it is here |
| --- | --- | --- |
| Game | Amber dot-matrix status; Play/Build/Cancel primary action; Rebuild when an executable exists | The primary action follows the actual install/operation state. Play needs only the installed executable; rebuilding also needs local source files. |
| Source | Editable selected ISO/folder path; one chooser for either input type; Build Options; Install Build Tools | Setup actions have one visible home. Tool installation is directly accessible when a build reports a missing prerequisite. |
| Display | Window/aspect/frame-cap/render-scale/artwork summary; one Settings button | Current launch preferences are visible; the full editor has a single entry point. |
| Utilities | Game Folder, Saves, Build Log, Controls | Distinct destinations, without duplicating the main configuration actions. |
| Build Options dialog | Workspace, installed-game folder and parallel jobs | Advanced build configuration stays out of everyday play. Save/Cancel are explicit. |
| Settings dialog | All existing native renderer settings and artwork mode | Save applies on next launch. Cancel restores unapplied control values. Settings can be prepared before the first build. Reopening reloads the runtime TOML so in-game changes are reflected. |
| Controls dialog | Existing keyboard mapping and controller guidance | Reference information does not need a permanent application tab. |

The dot matrix has **one status line**. It contains no executable filename,
renderer, platform, FPS/aspect values, bottom captions or decorative secondary
text. Those values are already represented by the source/display bays or footer.
During a build, the matrix shows the real phase and reported percentage with a
dotted progress rail. Unknown-progress operations use a moving indicator until
a measured value arrives; idle status does not animate.

## Operation and failure behavior

`ReadBuildStatus` consumes the existing `build-status.json` fields; no simulated
18-second build engine or sample phase percentages are included in the released
launcher. A previous run's status is ignored until the current run writes its
first status. Null progress remains indeterminate. Completion still waits for
the child process to exit, rather than enabling Play on an intermediate file.

Source/configuration changes and another build/tool install are blocked during
an operation. A game launched by this builder is tracked until it exits: the
matrix shows Running, duplicate launches and rebuilds are disabled, and closing
the builder leaves the game running. The main primary button becomes Cancel; cancellation stops the
child process tree and is distinguished from failure. Existing installed games
remain available after a failed/cancelled rebuild. A failed operation retains its
failure headline, shows the actual error outside the matrix, and opens the log.

The on-screen log retains a bounded tail. The full launcher build/tool output is
written under `%LOCALAPPDATA%/FuzionFrenzy/logs/launcher-*.log`. The log panel has
one folder menu for builder and game logs. Prerequisite installation also retains
its existing transcript. Save files, existing TOML keys/comments and artwork
launch modes remain supported. New UI preferences retain compatibility with older
launcher settings; parallel-job selection is now saved too.

This change affects the builder/launcher, not the game executable, graphics
runtime, browser implementation or milestone acceptance. A game already built
with 0.1.2 or later does not need recompiling for this launcher update.

## Development evidence

The native capture workflow compiles the actual Windows controls and can render
setup, example-ready, example-progress and the three dialogs without ISO/assets.
Ready/progress captures are presentation examples, not game/build execution
proof. The normal installer workflow compiles the launcher and NSIS installer.
Existing publication CI is retained; no new smoke/unit/regression suite is added.

Builder 0.2.0 [installer publication](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/43098718168) and [native control captures](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/43097742369) completed successfully. Captures use sample UI states without game content; they do not verify interactive gameplay.
