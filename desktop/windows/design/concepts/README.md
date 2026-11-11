# Launcher visual concepts

Four independently authored visual prototypes for choosing a launcher direction.
These are HTML/CSS explorations, **not screenshots of shipped native software**.
They change no installer, game executable, settings file or project milestone.
All displayed installation/build status is example data. No original game artwork,
fonts, screenshots or proprietary resources are included. Abstract artwork and
icons are drawn by this prototype.

Open `index.html` in a browser. Choose a concept and switch between first setup,
building and installed states. Game, Build, Settings and Controls navigation works.
Settings controls, source-file selection and build/cancel actions are local demo
interactions. File selection uses only the filename; it reads no file bytes and
uploads nothing. Play and folder/tool actions show a prototype notice. A real
implementation would connect these controls to the existing builder services.

| Concept | Direction | Layout decision |
| --- | --- | --- |
| A / Graphite | Charcoal, muted four-player colors, pale lime focus | Persistent sidebar; game card and compact source/display details. |
| B / Alloy | Silver equipment chassis, amber LCD, physical button treatment | Horizontal navigation; source/tool/output indicators and a compact build display. |
| C / Arena | Plum, warm yellow, four-player color blocks | Compact navigation rail, large primary action and broad control targets. |
| D / Paper | Warm white, forest green, fine separators | Tabular installation details with a dedicated action pane. |

Each concept includes a home, build, settings and controls layout. First-time setup
and installed/play states are distinct; compilation details are kept out of the
main play action. Platform and version remain at bottom right. There are no
navigation numbers, promotional taglines or large banner headings.

## Reference projects

Research was limited to public documentation/UI source; no project code or artwork
was copied. These are design interpretations, not claims that the projects use
our proposed colors/layouts.

- [Zelda64Recomp launcher UI](https://github.com/Zelda64Recomp/Zelda64Recomp/blob/dev/assets/launcher.rml): changes Select ROM to Start game after import; keeps controls/settings as explicit menu actions. Informed the setup-versus-play distinction and C's broad navigation targets.
- [Unleashed Recompiled installer](https://github.com/hedge-dev/UnleashedRecomp/blob/main/UnleashedRecomp/ui/installer_wizard.cpp): separate selection, installing, success and failure pages with progress presentation. Informed the separate build screen and stage list in all concepts; B's equipment aesthetic is our own exploration.
- [OpenGOAL usage guide](https://opengoal.dev/docs/usage/installation/): local ISO extraction, installation/version selection and a distinct ready-to-play state, with developer/folder controls secondary to Play. Informed A/D's installation details and secondary utilities.
- [ReCompLauncher](https://github.com/ZakyPew/ReCompLauncher): library/detail presentation, launch profiles and a controller-oriented mode. Informed A's game detail panel and C's larger action targets. We did not add its update/library/account features to this prototype.

## Rendering

The prototype needs no application dependencies, external fonts or network calls.
`render.cjs` is an optional image capture helper for a machine that already has
Playwright and Chromium. It starts an allowlisted loopback server for these three
source files, renders home/settings/build states and a comparison board, then
closes the browser and server. Example:

```sh
node desktop/windows/design/concepts/render.cjs artifacts/launcher-concepts
```

Native follow-through: preserve the existing builder/process lifecycle, local ISO
intake, game settings keys and save-preserving installation. Choose a direction
before investing in custom WinForms controls or another UI host. None of these
concepts requires changing the game renderer.

A separate [Alloy-focused second round](../alloy-studies/README.md) explores four
additional hardware directions and an animated dot-matrix build display. This
first round's four concepts and source files remain available unchanged.
