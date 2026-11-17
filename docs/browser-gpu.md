# Browser GPU candidate

This guide describes the local, player-content-only SDK browser candidate and its current WebGL2 renderer. It records a bounded rendering milestone; it does not accept M1 or establish complete original-game fidelity.

## Build locally

For the retained local layout, run from `.worktrees/browser-gpu` in WSL. SDK/toolchain checkouts and disc content stay local. The build reads the existing title-source package and writes adapted copies only into ignored build output:

```sh
source ../../.tools/emulation/emsdk/emsdk_env.sh
python3 tools/build-sdk-browser-full.py \
  --sdk ../../.tools/candidates/xboxxbe-codegen-58757ab \
  --config ../../.tools/candidates/fuzionfrenzy-recomp-071dbd9/config/game.toml \
  --generated ../../research/verification/sdk-title-bulk-command-20261107/generated \
  --jobs 6 --link-opt=-O3
```

The build writes the SDK overlay and module under ignored `.tools/artifacts/sdk-browser-full/`; it reads the retained generated title tree without modifying it. Run `python tools/serve-sdk-browser.py --open --port 8765`, select a compatible local ISO, and choose **GPU (WebGL2)** or **Software reference** in the launch card. The GPU option is the default. New generated copies, game images, captures and compiled artifacts stay out of source commits. A fresh checkout can use the default SDK/config paths documented in [the browser guide](sdk-browser.md).

Measured gameplay used SDK checkout `58757ab255c0a6b3929f405d7a5f1567fcd7fda3`, config checkout `071dbd97bdc098c4647cc24c99f9eae693fde1eb`, and `config/game.toml` SHA-256 `c45bed067026b71ff2415d2b901f5bece66ad54dd94796405ce2cd0c2e1cec2f`. Toolchain: Emscripten 3.1.50 / LLVM 18. The observed module used `-O3` objects and an `-O1` link; the final `-O3` relink subsequently completed. The current package uses `-O3` for both compilation and linking. The word-copy geometry optimization and invariant vertex position were then exercised in the actual Air Raid run below.

## Renderer shape

The original generated title instructions execute CPU-side in WebAssembly, and texture decoding/palette expansion remain on the CPU. The authored WebGL bridge batches render-target, draw and clear commands, copies bounded guest spans into owned command storage, and submits triangles to the browser renderer. The SDK-generated vertex-program HLSL is translated to GLSL in `web/sdk-browser/vertex-glsl.mjs`; the generated pixel combiner is translated in `web/sdk-browser/pixel-glsl.mjs`. WebGL2 executes both programmable stages with cached programs, render targets, textures and sampler state. `?cpuVertices=1` selects the development raster-only path, retaining CPU vertex evaluation while using GPU rasterization. The normal GPU path evaluates vertex programs on the GPU. Presentation uses a black full-window viewport and contains the game image at its aspect ratio; the 4:3 image was observed contained in a 1600×900 viewport without stretching. Software reference remains available for comparison and unsupported WebGL environments.

The title includes Twisted System instructions. Their observed behavior drove a replacement for the earlier broken border case using 2D border addressing and manual sampler behavior. A source-backed palette/cache-key correction restores font colors and transparency. A generated-code tail-call overflow at `0x63165` / `0x631AF` is handled in the ignored title overlay: 1,709 supported same-signature tail transfers become Clang `musttail` returns; 78 EH-bearing transfers are excluded. The retained generated files remain unchanged. The full compile succeeds, and actual execution has passed the former overflow point through original menus and results. The idle-attract path is not fully verified.

## Measured browser observations

On Ryzen 9 9900X / Radeon RX 9070 XT / Chrome 157 on Windows, the same original title measured **3.3 FPS** with software rendering (280.8 ms CPU draw, 15.1 ms clear) and **59.8 FPS** with the earlier WebGL2 raster path (6.7 ms CPU draw, 0.4 ms clear, 3.8 ms JavaScript submission, 586 draws). With programmable GPU vertex execution, the original menu/character screen measured **58.4 FPS**, 1,671 original draws in 151 GPU batches, 10 ms CPU draw and 6.6 ms JavaScript submission; nine vertex programs were observed. This establishes those menu surfaces, not gameplay performance.

Bombstruck was observed rendering a tournament round and completing CPU opponents through the results screen. In Twisted System before programmable GPU vertex execution, the raster-only development path measured **13.7 FPS**, 9,041 original draws in 173 GPU batches, 64.7 ms CPU draw and 16.6 ms JavaScript submission. Earlier gameplay/results measurements on the previous renderer were about 9 FPS during gameplay and 31.2 FPS on results; those are historical observations and do not represent the current GPU vertex path.

With programmable GPU vertex execution, a Tank Tracks tournament run (P1 Dub / CPU 3) rendered the original 3D scene without error and observed 11 vertex programs. Measured windows ranged **15.0–15.7 FPS**, 6,197–7,123 SDK draws, 529–625 GPU batches, 56.1–60.4 ms CPU draw, and 22.3–23.2 ms JavaScript submission. CPU racers made race progress. The P1 green tank visibly changed position after a 1.8-second W input and a shoot/Escape pulse; autonomous drift and input effects could not be separated, so this records input issued and position changed, not proven input causation or calibrated control. These observations do not support a 30 FPS gameplay claim.

The final `-O3` linked module ran Air Raid with 8 observed vertex programs and no reported GPU error. Measured gameplay windows were **18.1-20.7 FPS**, 6,845-7,390 SDK draws, 582-742 GPU batches, 42.6-48.1 ms CPU draw/submit and 17.4-18.4 ms JavaScript submission. JavaScript submission is part of the synchronous draw/submit path, not an additional independent frame cost. A separate early camera window measured 17.9 FPS. Current-module screenshots are `output/playwright/gpu-o3-airraid-live.png` and `output/playwright/gpu-o3-airraid-controlled.png`. The round subsequently reached the original play-again prompt with CPU Jet credited one win (private `output/playwright/gpu-o3-input-before.png`); P1 remained at zero. This proves round progression, not a successfully controlled complete round. Flat gray background areas remain a visual comparison dependency; no complete rendering-fidelity claim is made.

Local screenshots are retained at `output/playwright/gpu-vertex-tank-live.png` and `output/playwright/gpu-vertex-tank-play.png`; they are private runtime evidence and remain out of source commits.

The browser UI has an explicit fullscreen button, but the observed `requestFullscreen` call was denied with “not granted.” Fullscreen interaction therefore remains unconfirmed; the browser's native F11 shortcut is left available.

## Known limits and next evidence

- Programmable vertex and pixel stages execute on WebGL2. CPU-side title instruction execution and texture decoding remain in the pipeline; `?cpuVertices=1` is a development fallback, not the normal GPU path.
- Cubemap and 3D texture admission and F16/F24 depth encodings are unsupported. Sampling is base-level only; mip/LOD behavior is not established. The retained vertex/reference admission path handles triangle topologies, not a complete Xbox drawing API.
- GLSL invariant position is requested, but EQUAL-depth bit equivalence to native `precise` arithmetic is unverified.
- Initial guest CPU depth contents are not uploaded: WebGL depth targets are initialized with null storage. Depth rendering observed so far does not prove equivalent initial-depth semantics. Guest depth readback is also explicitly unsupported.
- Do not infer original renderer fidelity from a passed compile, a menu, a synthetic case, or a successful tournament result. Preserve per-surface visual and behavior comparisons.
- Repeat the Tank Tracks input/position observation with a controlled idle comparison, profile remaining CPU assembly/submission work and verify the idle-attract path before expanding the performance claim.

Private captures, Playwright output, generated title code and compiled `.tools/artifacts` stay ignored and local. This GPU/presentation work does not complete the broader M1 goals: a responsive controllable original menu, a complete playable minigame, and the required repeatable audiovisual/input evidence remain separate acceptance work.

## Gameplay optimization toward 60 FPS

The next candidate preserves the original draw order and moves converted fixed-function vertices, including world transforms and lighting, onto WebGL2 alongside programmable vertices. It uses the retained SDK's `FixedVertexHlsl` and extracts its existing constant builders into ignored build output using `tools/prepare-sdk-browser-fixed.py`. Original SDK and generated title inputs remain unchanged.

Decoded CPU textures now have a complete descriptor/backing/palette cache with exact per-frame byte checks, dirty epochs and same-frame `Written` invalidation. Queued images retain immutable host owners. GPU-owned render targets use descriptor-only sampled views; color uploads of unchanged shape preserve texture/FBO identity. Readback upload elision is restricted to lossless A8R8G8B8 coherence; lossy/forced-alpha formats keep conservative handling.

The existing SDK render thread and pipelined submission are enabled by default. Their copied adapter adds scratch-ring reservations and fences, bounded queued batches/payloads, alias-safe resource waits and depth-write tracking. One current and one in-flight owned arena overlap C++ preparation with JavaScript submission; a shared acknowledgement permits reuse only after JavaScript consumes the packet. Sync, copy, invalidation and flush drain earlier work. Errors remain explicit stops. Use `?threadedGpu=0&pipelinedGpu=0` for the serial development fallback, or `?pipelinedGpu=0` for synchronous submission. Restart the game after changing these switches. Software reference remains serial.

On the same recorded desktop, full fixed-function GPU execution plus the threaded consumer measured about 37 FPS in an Air Raid window using an O1 development link. Successive O3 texture, metadata and uniform improvements reached 53.2-60.6 FPS in eight measured Air Raid windows using JavaScript exceptions. Native WebAssembly exceptions then measured **56.8-60.9 FPS** across ten active Air Raid windows and **59.3-60.8 FPS** across ten Twisted System windows. CPU preparation/wait in Air Raid fell to about 11.6-13.1 ms and JavaScript submission to 5.8-9.0 ms. These are sequential original-game observations, not deterministic matched replays. Hardware: Ryzen 9 9900X, Radeon RX 9070 XT, Windows Chrome 157, ANGLE D3D11, original 640×480 render size in a 1600×900 viewport.

This is **not locked 60 FPS**, and it does not cover every minigame. Submission/metrics callback intervals still reached 57 ms in Air Raid and 34 ms in Twisted System, and first-use shader compilation can stall. An Erodeo attempt exposed palette-cache exhaustion because temporary resource headers were treated as persistent identities; the bounded cache now keys canonical palette backing/span and recycles old entries. The subsequent Erodeo run reached results: twelve countdown-through-results windows measured 39.8-60.9 FPS, up to about 12,500 original draws/frame, with a 226 ms maximum callback interval. This mixed sequence is not a sustained gameplay-only benchmark; heavy windows remain below target. Keep stopped attempts separate from successful gameplay measurements. Private per-window samples and screenshots are under `artifacts/browser-gpu/` and `output/playwright/`.

GPU diagnostics now report CPU preparation/wait elapsed time for pipelined mode, including backpressure. JavaScript submission overlaps that interval; do not add those numbers. `frameMs` percentiles describe completed submission/metrics callback intervals, not hardware GPU timing or physical display refresh. Raw diagnostics remain player-local in `globalThis.fuzionPerformance` and ignored artifacts.

The default build uses `-fwasm-exceptions` for every object and the link, after observing original startup, menus and the gameplay windows above. [Emscripten documents the native exception backend](https://emscripten.org/docs/porting/exceptions.html). `--exceptions js` retains the slower development fallback; changing the flag invalidates object signatures. The initial native-EH module was 19.55 MB, versus about 24.46 MB with JS exceptions. These observations do not qualify arbitrary JS exceptions crossing C++ or all original continuations.

The remaining performance batch is a direct validated raw-vertex bridge: current `AssembleConverted` expands every register to float4, the bridge owns those expanded bytes, and JavaScript repacks them for attributes. Reusing the SDK raw representation requires checked stream spans, format/decode handling, index rebasing and a preserved converted software path; flipping `XBE_RAW_VERTICES` alone is unsafe. Profile actual slow gameplay before implementing that seam. Frame pacing, shader warm-up and broader per-minigame evidence remain separate work toward the 60 FPS target.

The final incremental build also removes non-indexed sequence allocation and skips unused transform/light constant calculation for fixed pretransformed draws. A subsequent ten-window **active Erodeo** run measured **46.4-56.2 FPS**, about 9,900-12,800 original draws/frame, with 17.7-21.3 ms CPU preparation/wait, 7.9-11.2 ms JS submission and a 44.7 ms maximum callback interval. No dropped-draw or frame-skipping shortcut was introduced. Erodeo character/corner-surface appearance and intermittently missing text need a native reference comparison; a targeted CPU-vertex capture also showed visual limitations and is not a fidelity oracle.
