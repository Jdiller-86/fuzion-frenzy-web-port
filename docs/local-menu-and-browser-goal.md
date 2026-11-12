# Local goal: playable browser and clickable native menus

Use the current `main` checkout, existing native build, compatible player-local ISO and retained UI inventory. Preserve local edits, saves and original assets. Do not restart integration, rebuild analysis databases or run broad/synthetic/smoke test suites. Compile the actual changed application, run it to fix observed failures, and continue until the selected goal works. Report major progress or blockers only.

## 1. Browser performance — largest remaining implementation item

The user reports that the original browser game launches but is unplayably slow. The candidate already has the full SDK boot, disc, input, audio and frame pipeline. The new release build uses `-O3`, SIMD, release logging and reusable presentation buffers. Read the on-screen FPS/CPU draw/clear/copy measurements before changing architecture; record browser, CPU/GPU, build and scene. Do not claim a speedup without comparing actual execution.

The current `runtime/browser/sdk/full-platform.cpp` backend calls the SDK's **CPU** `reference::Draw/Clear/Copy`. Canvas output only displays those CPU-rendered pixels. Raising the frame cap or putting that canvas on WebGL will not accelerate the game's triangles.

Implement one accelerated backend behind the existing SDK `gpu::Backend` interface. Preserve the software backend as an explicitly selected reference/fallback. Search/reuse existing work first:

- The pinned SDK's D3D12/Vulkan backends and shader generator own the title's surface, texture, shader, vertex and render-state semantics.
- [OpenCE's browser host](https://github.com/OpenCommunityEdition/OpenCE/tree/main/port/web) supplies relevant WebGL/thread/host patterns; its game-specific renderer is not a drop-in Xbox backend.
- [web-recomp-kit](https://github.com/A1isandr/web-recomp-kit), inspected at `4a7da07447e59f02491484c17e4b35543f50f3dd`, has an MIT-licensed WebGPU device/backend and WGSL generation under `host/gpu/webgpu/` and `dx/d3d9_shadergen.cpp`. Its D3D8 shim uses different device/resource/shader interfaces. Inspect that mismatch before reuse; do not replace the SDK or imply its shader generator already accepts Xbox shader/state objects.

Keep guest address/span validation, physical aliases, resource lifetime, texture updates, render-to-texture, depth/blend state, shader/combiner semantics and CPU readback/Sync behavior intact. Do not cache guest textures indefinitely without tracking CPU writes and allocation reuse. Do not add fast-math or skip original draw calls to manufacture an FPS gain. Retain the local-content boundary.

Use the first identified original menu and one selected minigame to implement the actually exercised feature families. Group missing states by shared provider rather than opening one task per draw. Compile/run/fix the next actual failure; broad matrices, regression suites and independent acceptance review follow the playable baseline. Target a responsive menu and at least 30 FPS for the selected original 640×480 minigame on the recorded desktop, with working input/audio and results return. If a real capability blocks that target, record its exact cost/contract and continue independent work; do not stop at a plan or empty renderer scaffold.

Return the runnable package, exact source/build identities, before/after FPS and CPU/GPU costs, selected scene/device, actual visible limitations and next largest blocker. Runtime logs/captures containing game content stay local. Update #49/#50, the browser work map and current status without accepting M1 from a frame or compile alone.

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
