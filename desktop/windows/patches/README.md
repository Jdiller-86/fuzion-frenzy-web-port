# Windows rendering fixes

The optional [native UI prototype](../../../docs/native-ui.md) additionally uses
`ui-replacements.patch` after these rendering fixes and `native-ui.patch` against
the pinned native host. `ui-manifest.json` records their normalized identities.
They contain source only; the three-image pack must be created locally from the
player's private inventory. `tools/ui/Build-NativeUi.ps1 -ApplyPatches` owns this
opt-in build lifecycle; the standard installer still applies only the rendering
fixes below.

`runtime-fixes.patch` modifies the public XboxXBERecomp SDK at commit
`58757ab255c0a6b3929f405d7a5f1567fcd7fda3`. It contains no original game code
or assets. The SDK is MIT-licensed; retain its upstream license when building.

- `runtime/gpu/shaders.cpp`: terminate the fixed-function texture cascade when
  COLORARG1 selects an unbound texture. SELECTARG2 remains exempt because it
  does not consume COLORARG1. This prevents a later white fallback texture from
  covering the correctly textured geometry.
- `runtime/gpu/d3d12.cpp`: allocate additional shader-visible descriptor heaps
  when a frame fills its 32,768-entry SRV region. Keep each page until that
  frame's fence completes. Wrapping the old cursor could overwrite descriptors
  referenced by pending draws, causing textures and background geometry to flicker.
  All callers rebind descriptor tables after switching heaps; allocation failure
  skips the affected operation instead of using an invalid descriptor.

The builder applies these changes only around compilation and restores the
SDK checkout afterwards. `manifest.json` records complete before/after SHA-256
identities after normalizing CRLF to LF. An interrupted build's exact known
patch can be restored on the next run; unknown source edits are rejected.

## Evidence and limits

Local Windows D3D12 verification on November 10–11, 2026 showed the formerly
white Twisted System geometry textured correctly. The descriptor diagnostic
first observed 576 unsafe wraps. With the fix it recorded 609 overflow-page
allocations/reuses and no heap-allocation failures; a complete Twisted System
round retained stable background geometry. A separate normal run sampled about
60 FPS (16.35 ms). These observations cover that local configuration and scene,
not every arena, GPU, or the browser runtime.

The shader change passed seven synthetic cases using the production shader
generator. The descriptor fixture executes the extracted production allocator
with a modeled device, checking page retention, reuse and allocation failures;
it also checks fence/reset ordering in production source. Both GPU changes
received independent source review. Synthetic checks do not establish GPU or
original-console fidelity.

To repeat the component checks, apply the patch to a clean pinned SDK checkout,
open an x64 Visual Studio developer shell with Clang available, and run:

```powershell
./desktop/windows/tests/Run-ShaderRegression.ps1 -SdkRoot <SDK> -OutputDirectory <ignored-output>
./desktop/windows/tests/Run-DescriptorRegression.ps1 -SdkRoot <SDK> -OutputDirectory <ignored-output>
```

No game input is needed. Keep generated test executables and includes outside
tracked source paths. The existing downloadable 0.1.1 preview predates these
patches; a newly packaged builder is required to apply them on future builds.
