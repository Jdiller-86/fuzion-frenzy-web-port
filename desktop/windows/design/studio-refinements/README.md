# B3: amber refinement and four further directions

This round preserves the original green B3 and every earlier concept, adds an
amber refinement of B3, and explores four further equipment-inspired directions.
The interactive gallery contains **13 selectable designs**. No production
launcher, game source, installer version or checkpoint changes in this batch.

| Design | Treatment | Layout |
| --- | --- | --- |
| Studio Amber | B3's olive/carbon equipment, amber display text, LEDs, focus and launch accents | Refined original rack plus source/display modules; the closest continuation of the selected B3. |
| B3-A / Control Room | Olive/bronze equipment with amber phosphor | One stacked equipment bank; labelled Game, Source and Display bays; a separate launch control. |
| B3-B / Relay | Copper-brown instrument casing, charcoal workspace and amber | Self-contained control instrument beside an installation/configuration information pane. |
| B3-C / Broadcast | Warm ivory equipment, dark glass and amber/orange accents | Broad faceplate, side transport controls, source strip and a separate display configuration module. |
| B3-D / Obsidian | Smoked glass, blue-black metal, restrained amber highlights | Large integrated glass display/source/launch unit, with a quiet configuration pane beside it. |

## Behavior and evidence

These are rendered HTML/CSS/Canvas visual prototypes, not native Windows
screenshots or released features. The matrix retains the actual dot renderer
and simulated build engine from the Alloy round: Prepare → Read Game → Compile
→ Install → Ready, with percentage/progress updates, scrolling detail and cancel.
Reduced-motion mode keeps the progress readable without scrolling.

The normal sidebar/navigation, Settings, Build and Controls screens remain
interactive. The original four general concepts, all four Alloy studies, and
original green B3 remain selectable alongside the five additions. File selection
reads a filename only; there is no game upload, game execution or compilation.
All installation status, paths and logs are example data.

The amber reference keeps B3's physical rack, mounting points and olive equipment
character. The four new directions explore integrated layout, material and control
hierarchy rather than replacing the chosen concept. There are no slogans,
promotional banners or numbered application navigation tabs. Platform and version
remain at bottom right. Design labels are outside the prototype application.

All artwork, icons, dot glyphs and surfaces are independently authored prototype
work. Existing [recomp UI and hardware references](../alloy-studies/README.md#references-and-authorship)
remain the functional and material context; no proprietary product imagery,
original game assets or reference-project UI code is included.

## Capture and package

```sh
node desktop/windows/design/studio-refinements/render.cjs artifacts/studio-refinements
python desktop/windows/design/studio-refinements/package.py
```

Capture uses installed Playwright and Chromium. It serves only explicitly mapped
prototype sources over loopback, renders home/settings/build views, produces the
four-option comparison, and captures the running amber matrix demonstration.
The package helper uses installed Pillow to make the GIF and bundles standalone
HTML plus previews. No game tests or compilation are invoked.

Outputs under ignored `artifacts/studio-refinements/`:

- `studioamber-home.png`: refined B3 amber reference.
- `comparison.png`: the four further directions at the same scale.
- Per-design home/settings/build PNGs.
- `amber-matrix-demo.gif`: captured simulated progress, not real build evidence.
- `FuzionFrenzy-Studio-Refinements.html`: standalone interactive gallery, all 13 concepts.
- `FuzionFrenzy-Studio-Refinements.zip`: HTML, references, source galleries and previews.

For a production choice, connect the selected matrix to the launcher's existing
`ReadBuildStatus` phase/progress/status fields. Keep the normal build log for full
messages. This design round changes no compiler behavior or build-time forecast.
