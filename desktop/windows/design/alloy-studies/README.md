# Alloy: four hardware-inspired launcher studies

A second design round focused on the physical-control and illuminated-display
philosophy of original option B. The first four concepts remain unchanged in
`../concepts/`. This gallery also includes those four as selectable original
options, alongside the four new studies. No design has been removed or selected
for production, and the released Windows launcher is unchanged.

| Study | Palette | Layout and reference direction |
| --- | --- | --- |
| B1 / Receiver | Brushed silver and warm amber | A receiver-style faceplate; one round launch key beside the readout, separate source strip and setting modules. Closest to original B. |
| B2 / Portable | Pearl shell and ice-blue dots | A portable MiniDisc-inspired body, recessed display and tactile transport controls, with installation details beside it. |
| B3 / Studio | Carbon/olive metal and phosphor green | A rack-style status module, compact controls, distinct source/configuration modules. |
| B4 / Workbench | Blue-grey titanium and white/cyan dots | A compact desktop tool: narrow matrix readout, component table and utility sidebar. |

Each new study has home, settings, build and controls views. Preferences use the
existing game settings model. Platform and version remain at bottom right; there
are no promotional taglines or numbered navigation tabs.

## Working dot-matrix component

The matrix is a real canvas renderer using an independently authored 5×7 pixel
font, rather than a regular font styled to resemble a display. It draws discrete
lit/unlit cells with palette-specific glow. A simulated build emits progress
through Prepare, Read Game, Compile and Install, then switches to Ready. The
matrix displays the active stage, percentage, progress line and scrolling detail
when needed. The stage list and standard progress bar follow the same model.
Reduced-motion preferences disable scrolling while retaining readable progress.

Open `index.html`; select a study and choose **Building** in the outer gallery
controls, or choose Build/Rebuild and start the demonstration. The demonstration
runs for about 18 seconds. Cancel returns to the uninstalled state. File selection
reads only the chosen filename; no game files are read, compiled or uploaded.
Installed state, filenames, configuration and log messages are sample data.

The original four styles are available in the concept selector. Original B keeps
its original static display so the new matrix can be compared against it.

### Production connection

The existing `desktop/windows/scripts/Build-Game.ps1` writes phase, progress,
message, status and error to `build-status.json`. The native launcher already
reads that file in `ReadBuildStatus`. A production matrix would consume those
existing values, without changing the compiler or pretending the demonstration's
18-second duration forecasts real build time. Text should be restricted to short
stage names and percentages, with the normal log retained for full messages.

Before a real build starts, the display can show selected input and tool status;
during compilation it shows measured progress; on completion it shows ready;
on failure it should show the actual failed phase and offer the existing log.
The prototype implements the simulated setup/build/completion path, not real
hardware discovery, compilation, game launching, or the production error path.

## References and authorship

The first round's [recompilation UI references](../concepts/README.md#reference-projects)
remain relevant: Zelda64Recomp's import/start distinction, OpenGOAL's installation
and everyday play separation, and Unleashed Recompiled's staged installer. Those
functional patterns remain common to all four studies.

Additional hardware directions were considered using the official
[Sony MZ-N1 manual index](https://www.sony.com/electronics/support/product/mz-n1/manuals)
and [Yamaha RX-V1000 manual library](https://usa.yamaha.com/support/manuals/index.html?c=&k=rx-v1000&l=en).
They are broad design references for portable transports, source selection,
displays and physical control hierarchy. No hardware logo, product photograph,
font, manual artwork, game asset or third-party UI code was copied. The palettes,
layouts, CSS surfaces, icons and matrix renderer are prototype work for this repo.

## Capture and delivery

```sh
node desktop/windows/design/alloy-studies/render.cjs artifacts/alloy-studies
```

The capture helper reuses installed Playwright/Chromium, serves only the explicitly
allowlisted prototype sources on loopback, and closes its own browser/server.
It exports home/settings/build views for all four new studies, a comparison board,
and frames of the actual running matrix demo. `?freeze` holds progress at a sample
61 percent for still captures; it is not a game-performance measurement.

Source is in this directory. Shareable standalone HTML, image previews and the
animated GIF are generated under ignored `artifacts/alloy-studies/`. They contain
no original game data. The first round's generated artifacts are retained too.

After capture, `python desktop/windows/design/alloy-studies/package.py` uses
installed Pillow to assemble the matrix GIF and a ZIP. The ZIP includes both
standalone HTML galleries, all eight selectable concepts and full-size previews.
The standalone Alloy gallery includes the original four in its selector as well.

A [third round focused on B3](../studio-refinements/README.md) adds an amber
refinement and four further directions. Every earlier concept, including green
B3, remains available; this round's source and previews are retained.
