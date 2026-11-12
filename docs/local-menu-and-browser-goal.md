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

## 2. Native original-menu mouse support and integrated actions

Builder 0.2.1 adds a clickable **PC menu → Settings / Exit to Desktop** host overlay. It opens the existing mouse-capable F1 panel and uses the normal Windows close event. Install it and rebuild the game to include the patch. F1 remains available. This does **not** add mouse hit targets or new rows to original guest menus.

Use `docs/ui-architecture.md`, `docs/ui-settings-integration.md`, the private inventory and retained native source to trace the active original main-menu/Options object, input handler and presentation transform. Existing constructor/vtable addresses and isolated quad coordinates are static leads, not a complete active-state or hit-target contract.

Implement mouse hover/selection/click for the original main menu and Options first. Derive hit areas from the actual rendered layout, including letterboxing, aspect changes and render scale. Route actions through the same selection/activation path as the original pad. Clear held/queued actions on focus loss and transitions; clicks must not leak into gameplay or the host panel. Preserve keyboard/controller navigation.

Add **PC Settings** and **Exit to Desktop** to the main-menu presentation only once its active-state, row-count/layout, ownership and action lifecycle are established. PC Settings opens the existing host draft/settings path; Exit uses the platform's normal shutdown path, with no invented autosave promise. If an original row cannot be safely extended, use an explicit, correctly positioned host addition gated to the verified main-menu state. Do not guess screen coordinates or overwrite unrelated original options. Retain the general PC menu as a fallback.

Own `desktop/windows/patches/native-ui.patch` and its normalized manifest, plus the minimum original input/UI adapter changes. Preserve the known-previous-patch upgrade path in `scripts/Ui-Patches.ps1`; unknown local source edits must remain protected. Use the existing incremental native build and retained object cache. Observe actual click/navigation/shutdown behavior in the original game; do not substitute fixture tests for that implementation loop.

Return the working executable/incremental patch, privately retained before/after menu captures and a short map of supported menus and remaining screens. Publish only authored adapters/patches, normalized manifests and curated findings; never publish original media, extracted UI assets, generated title code or private raw traces. Keep this native work separate from browser milestone acceptance.
