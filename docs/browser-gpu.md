# Browser GPU candidate

The original title executes in WebAssembly and renders through WebGL2. This is a local prototype with measured improvements, not acceptance of M1, complete rendering fidelity, or locked 60 FPS on modest hardware. The native build and original inputs remain unchanged.

## Run and rebuild

Run `python tools/serve-sdk-browser.py --open --port 8765`, select a compatible local ISO and Start game. GPU rendering is the default; the slower software diagnostic is under Compatibility options. The game fills the window while preserving its original aspect ratio. F1 opens host controls; it does not pause the guest.

From the retained `.worktrees/browser-gpu` layout in WSL:

```sh
source ../../.tools/emulation/emsdk/emsdk_env.sh
python3 tools/build-sdk-browser-full.py \
  --sdk ../../.tools/candidates/xboxxbe-codegen-58757ab \
  --config ../../.tools/candidates/fuzionfrenzy-recomp-071dbd9/config/game.toml \
  --generated ../../research/verification/sdk-title-bulk-command-20261107/generated \
  --output .tools/artifacts/sdk-browser-wasm-eh --jobs 6 --link-opt=-O3
# Stop the game before installing a matched JS/Wasm/worker set.
for name in fuzion-sdk-full.mjs fuzion-sdk-full.wasm fuzion-sdk-full.worker.js full-build-manifest.json; do
  cp ".tools/artifacts/sdk-browser-wasm-eh/$name" ".tools/artifacts/sdk-browser-full/$name"
done
python3 tools/package-sdk-browser.py
```

The pinned SDK is `58757ab255c0a6b3929f405d7a5f1567fcd7fda3`; config checkout `071dbd97bdc098c4647cc24c99f9eae693fde1eb`, `game.toml` SHA-256 `c45bed067026b71ff2415d2b901f5bece66ad54dd94796405ce2cd0c2e1cec2f`. The measured build uses Emscripten 3.1.50 / LLVM18, O3 objects/link and native Wasm exceptions. `--exceptions js` preserves the slower development fallback and invalidates the object cache when selected. Build manifests record original title hashes; adapters modify disposable copies only. The published texture-cascade correction is consumed; native D3D12 descriptor-rollover changes remain untouched.

## Implemented optimizations

- SDK programmable and fixed-function vertex shaders, transforms, lighting and pixel combiners execute on WebGL2. Native shader generators remain the semantic source; authored translators lower their generated HLSL.
- Native vertex streams bypass CPU float4 expansion and JS transposition when admitted. The initial admission is one stream, stride aligned to four bytes and at most 252 bytes, supported SDK formats and every attribute inside its stride. Complete source spans are checked before dense views or sparse gathers; the bridge owns copies before SDK assembly buffers can be reused. SDK list expansion preserves primitive/index order, including flat-shading rotation. Incompatible layouts use converted assembly.
- Typed inputs use the SDK's normalization, integer, FLOAT2H and packed 11:11:10 reconstruction. BGRA is corrected exactly once. Raw layout groups share VBO uploads and retain state barriers; converted vertices still have their own planes and index bases.
- Exact SIMD byte equality replaces scalar comparisons for large constants, metadata and encoded images. Consecutive pixel keys reuse stable shader text before hashing. Fixed constants cache every input actually read by the native transform/light builders; per-draw fog is refreshed and captured before reuse. No draws or simulation frames are dropped as a performance shortcut.
- Admitted 2D color textures retain owned RGBA8 bytes instead of float4 texels. BGRA8 uses the existing SDK decoder and one R/B swap; BC1/2/3 reuse its block decoder through a temporary float buffer, then retain only quantized bytes. This reduces persistent decoded CPU pixels and pinned bridge payloads from 16 to 4 bytes/pixel (75%); GPU storage was already RGBA8. The software reference keeps float sampling. Faces, encoding, spans and exact owner sizes are checked; unsupported texture classes retain existing refusals. JS reuses ordinary upload staging and updates same-sized allocations with `texSubImage2D`.
- Command rows and pending merged draws use scalar offsets rather than temporary wrappers. Per-program sampler address/border snapshots reuse owned arrays and retain Float32 rounding. Uniform updates still follow source order.
- Decoded textures use complete backing/descriptor/palette keys, exact content checks, dirty epochs and same-frame write invalidation. Queued images retain immutable host owners. GPU targets have descriptor-only sampled views, stable color allocations and bounded FBO/layout caches. Lossless readback coherence avoids redundant uploads; lossy formats keep conservative handling.
- Threaded preparation and one-flight pipelining overlap C++ work and JS submission. Scratch reservations, resource barriers, queue/payload caps and acknowledgement before reuse protect ownership. Current plus in-flight command arenas stay bounded. Failure remains an explicit stop.

Raw streams, GPU vertices, threading and pipelining default on. Development fallbacks: `?rawVertices=0`, `?cpuVertices=1`, `?threadedGpu=0&pipelinedGpu=0`, or `?pipelinedGpu=0`. Restart after changing switches. Software rendering remains serial.

## Timing and presentation polish

Windows uses browser-timer pacing by default; other platforms retain the existing wait until measured. `?precisePacing=1` enables it explicitly and `?precisePacing=0` restores coarse waits. A non-main PThread schedules an absolute deadline on the runtime JS agent, then sleeps on a private futex until acknowledged. The callback rechecks the synchronized clock and refreshes its shared-memory view. Cancellation cannot release the stack signal early. No busy spin or global OS timer setting is used. The SDK virtual-blank counter, event, callback and reset order are retained.

An actual runtime-worker probe requested 16.667 ms `Atomics.wait` sleeps and measured a 31.18 ms median here. Browser `setTimeout` was better but still late at times. In five menu windows, the SDK virtual-blank rate improved from about 42-43 Hz to 57-59 Hz; this is an independent SDK counter, not a physical-display measurement. The timer remains best-effort under load and background throttling.

`prepareCompletionIntervalMs` measures render-consumer completion; `metricsCallbackIntervalMs` measures diagnostics delivery, with `frameMs` retained as its alias. Neither proves physical display cadence. FPS counts completed metrics callbacks, and JS submission overlaps CPU preparation/wait: do not add those two costs. `vblankHz` is counted independently by the SDK blank observer. All diagnostics stay local.

Startup now shows progress beside Start/Stop, locks setup inputs during boot and stops the loading indication on first frame. Per-frame status DOM rewrites were removed. Context loss returns a restart message. Full-window aspect-preserving presentation and reduced-motion handling remain; automated fullscreen previously received a browser denial, so physical fullscreen interaction remains unconfirmed.

## Actual gameplay observations

Hardware: Ryzen9 9900X, Radeon RX9070XT, ANGLE D3D11, Windows Chrome157, 1600x900 viewport and original 640x480 game target. These are sequential original-game runs, not deterministic matched replays or a modest-laptop qualification.

| Candidate | Observed workload | FPS windows | CPU prepare/wait |
| --- | --- | --- | --- |
| Published e5b1e71 | Active Erodeo, ten windows | 46.4-56.2 | 17.7-21.3 ms |
| Raw streams only | Active Erodeo, twelve windows | 46.8-59.1 | 16.3-21.3 ms |
| Raw + SIMD/key reuse | Active Erodeo, ten windows | 59.3-60.8 | 10.8-14.1 ms |
| Final cache/timer candidate | Active Erodeo, twelve windows | 59.7-60.6 | 10.9-13.3 ms |
| Final cache/timer candidate | Twisted System, ten windows | 58.7-60.5 | 6.4-9.1 ms |

Heavy Erodeo transfer volume fell from roughly 18-20 MB of expanded vertices to about 2-3 MB/frame while retaining roughly 10,000-13,000 original draws. Final Erodeo JS submission was about 5.6-9.0 ms. Consumer-completion p95 was about 19.6-21.8 ms in Erodeo and 22.3-27.4 ms in Twisted System. These tails exceed a strict 16.7 ms budget. Earlier first-use shader stalls also remain relevant. Erodeo reached results; this does not establish a calibrated complete player-controlled round.

A subsequent byte-storage/allocation pass used eight active Erodeo windows for each fresh browser. Preserved c40f955 measured 56.7-60.7 FPS, JS submission 5.2-6.9 ms; the new candidate measured 57.0-60.2 FPS and 4.5-6.2 ms. Both median FPS values were about 60. CPU preparation ranges overlap (8.5-10.7 versus 8.8-10.8 ms), and new consumer-completion p95 remains 20.0-21.3 ms. These sequential runs have different CPU characters/camera states; they establish execution and useful estimates, not a deterministic speedup or a presentation lock. Original header assets and Erodeo textures/transparency rendered; byte-upload counters were observed. Existing intermittent text/character rendering limits remain.

The game renderer process working set was 736.0 MiB in the preserved run and 642.2 MiB in the new run. Private committed memory was 999.4 and 1003.9 MiB respectively, then fell to 172.8 and 157.4 MiB after Stop and worker termination. These are named-process Windows samples, not total browser RSS or a memory-leak proof. They supersede any interpretation of the earlier 4.05 GB experimental aggregate as physical usage. Shared backing, Wasm growth, driver allocations and idle-page costs remain separate concerns.

Private samples, screenshots, profiles, matched module sets and input/build identities are in `artifacts/browser-gpu/` and `output/playwright/`. No original images, shader captures, title listings or compiled packages belong in public commits. Older measurements remain in Git history and retained private evidence.

## Remaining work toward modest-PC 60 FPS

A true lock for all gameplay is **not established**. The measured machine is powerful. The browser rejected CPU throttling on worker targets, so page-only throttling must not be passed off as a low-end Wasm-worker benchmark. Actual integrated-GPU laptop runs, a declared device/browser matrix, longer scenes/full rounds and all minigames remain necessary.

The next changes should stay targeted to measured costs:

1. **Shader first use:** start the unique programs in an owned command batch before packing geometry. Delay blocking status/uniform queries until each program is actually used, preserving draw order and the synchronous fallback. The existing program cache is in `WebGLRenderer.program`; [Khronos parallel compilation](https://registry.khronos.org/webgl/extensions/KHR_parallel_shader_compile/) supplies a nonblocking completion query. This prepass is proposed, not implemented.
2. **CPU preparation and JS submission:** profile the warmed original worker and `WebGLRenderer.draw` again, then remove the largest remaining state copies, temporary objects and redundant constant uploads. The current frame still represents roughly 10,000-13,000 original draws. Reuse the existing adjacent-draw merger; preserve state barriers and original ordering.
3. **Compressed uploads:** optionally pass the already decoded BC1/2/3 blocks to a supported [S3TC upload](https://registry.khronos.org/webgl/extensions/WEBGL_compressed_texture_s3tc/) instead of expanding them. Keep exact alpha/block/dimension rules and the working RGBA8 fallback. This would target first-use decode cost, temporary memory and GPU storage; it is not implemented.
4. **Presentation pacing:** correlate consumer completion, SDK virtual blank and actual browser/display presentation. Current callback FPS does not prove a 16.7 ms physical cadence. Avoid changing guest simulation timing to improve a counter.
5. **Modest hardware:** measure GPU execution on an actual integrated-GPU laptop. Add internal resolution/quality controls only for confirmed GPU bottlenecks; they cannot remove CPU preparation cost. Preserve logical UI coordinates and readback behavior when introducing physical target scaling.

MB07 timing, MB14 textures and MB15 shader integration have prototype evidence; their acceptance states and M44 remain unchanged.

Known rendering limits remain: 3D/cube textures and F16/F24 depth formats are unsupported; sampling is base-level only; depth CPU initialization/readback is incomplete; EQUAL-depth bit equivalence is unproved; triangle admission is not a full Xbox drawing API. Erodeo character/corner surfaces and intermittently missing text still need native-reference comparisons. Physical gamepads and audible output were not verified in this batch. Browser settings and native UI replacement parity remain separate work.
