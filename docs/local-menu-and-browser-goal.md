# Local goal: playable browser and clickable native menus

Use the current `main` checkout, existing native build, compatible player-local ISO and retained UI inventory. Preserve local edits, saves and original assets. Do not restart integration, rebuild analysis databases or run broad/synthetic/smoke test suites. Compile the actual changed application, run it to fix observed failures, and continue until the selected goal works. Report major progress or blockers only.

## 1. Browser performance — largest remaining implementation item

The WebGL2 candidate runs the original menus and 640×480 minigames in the browser from the player-local ISO. Local Erodeo runs have reached a winner. Recent gameplay windows average about 60 FPS on the development desktop, with consumer interval p95 around 21 ms. [Build and measured results](browser-gpu.md).

Under #49 and #50, measure timer wake-up and presentation queue delays, then fix the largest observed cost while preserving guest timing and checked resource ownership. Use the existing SDK backend and software reference when comparing reached texture, vertex, shader and draw-state differences. Keep source, browser/device and scenario identities with each focused run.

Record one controlled round through results and menu return with audible audio and recorded input. Use an identified modest integrated-GPU laptop to qualify stable 60 FPS beyond the development desktop. Evaluate 120 FPS after stable 60 FPS and verify that input, physics, animation and timers retain their original behavior. Keep captures and local build artifacts private. M1 acceptance follows the separate three-cycle review.

## 2. Native original-menu mouse/keyboard and integrated settings — corrected requirement

The user wants the **actual game UI** to respond to mouse and keyboard, with **PC Settings** and **Exit to Desktop** alongside the regular main-menu entries. The floating PC menu in builder 0.2.1 does not fulfill this request. Do not deliver another toolbar, popup, launcher setting or lookalike replacement menu as the solution. Rebuilding 0.2.1 will not add these original-menu features.

### Required result

- Hover highlights the original interactive item; click activates that same item/action. Keep the original labels, layout, transitions and controller navigation.
- On menus, arrow keys navigate/change values, Enter activates and Escape returns. Coordinate these mappings with the existing gameplay Start/A/B bindings so one key cannot trigger both paths.
- Add **PC Settings** and **Exit to Desktop** as ordinary main-menu entries in the same focus/navigation sequence as the original rows. Do not replace original game options or turn an existing unrelated action into Exit.
- The settings screen must fit the original UI styling and component system. Reuse the existing SDK settings and Apply/Save/Cancel implementation behind it; F1 may remain a secondary shortcut. Settings must be reachable without knowing a hotkey.
- Exit uses normal platform shutdown after the menu action. Do not invent an autosave promise.

Start with main menu and Options. Continue through character selection, pause menus, confirmation/results dialogs and other original interactive screens. Report supported screens individually; main-menu support does not establish all-screen mouse support. Observe actual hit targets at the player's resolution, aspect and render scale; do not fake hit regions from screenshot proportions.

### Local implementation route

Use the current local native executable, matching ISO/source, private UI inventory and incremental object cache. Read `docs/ui-architecture.md` and `docs/ui-settings-integration.md`. Existing main/Options constructor, handler and presentation addresses are investigation leads. The retained source supports focus fields and hard-coded row wraps, but it does not yet establish the full live-object lifecycle, row-to-text/layout mapping or input-selector contract.

Trace the current main-menu object and its original draw/update/event path in the running game. Join each visible row to its focus state, label/layout object and activation action. Trace the existing text/font draw path rather than guessing a system-font substitute. Use the matching source/disassembly and a short actual navigation capture; do not rebuild Ghidra projects, replay M44 or demand a broad test matrix before implementing the feature.

The existing native `src/game/hooks.cpp` supports strong definitions wrapping recompiled guest functions; call their original `__imp__` implementation to preserve stock behavior where appropriate. Confirm the exact calling convention, stack/register/flag behavior and object lifetime before using a hook. Do not copy generated original bodies into public patches. Extend the real menu's row storage, count/wrap, rendering and action dispatch together; changing only a focus bound or drawing extra text is incomplete.

Collect mouse movement/clicks and keyboard edges in the host adapter, map through the actual viewport/letterbox transform, and consume commands in the owning guest menu thread. Avoid mutating live guest UI state from the renderer thread. Revalidate the active object on each relevant callback; discard queued commands when focus, screen or ownership changes. Keep keyboard and controller paths intact. Existing host settings requests should cross to the render thread through synchronized state; do not invoke ImGui drawing from a guest callback.

Own the minimum native input/menu adapters plus `desktop/windows/patches/native-ui.patch` and its normalized manifest. Preserve known-previous-patch upgrades and protection for unknown local edits. Remove the floating PC menu from the normal experience once real menu entries work; it is not an acceptable fallback completion.

Compile the actual game incrementally, run it, fix the observed interaction and continue. Do not spend this implementation pass on fixture/smoke/unit/regression suites, elaborate probe frameworks or repeated historical audits. Stop only affected work for a concrete missing contract/input; report exactly what is needed and continue independent implementation. Do not finish with another proposal when the local game/source can resolve the next step.

### Handoff result

Return the working executable and authored incremental patches, source/build identities, a short list of supported/remaining screens, and privately retained captures showing original-row mouse selection, keyboard activation, Settings entry and Exit. Keep screenshots/assets, original media, generated title code and raw traces local. Update the current UI docs and task with actual behavior and remaining gaps; do not mark this request complete for a floating host menu or a successful compile. Native UI delivery does not accept the browser milestone.
