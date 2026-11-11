# Texture replacement and upscaling integration

This note records texture decode, cache, and upload seams in the retained XboxXBERecomp SDK pin `58757ab…` and Fuzion native integration pin `071db…`, used by the November 10 Windows rendering work. Original asset inventory evidence is indexed against publication commit `93ec83293921dfcf1ff4b8f0c759aa62189c330b`. SDK facts are from `runtime/gpu/texture.h`, `texture.cpp`, `ext.h`, and `d3d12.cpp`. The matching game image and texture inventory are local/private; no asset IDs or replacement mappings are included here.

The [native UI prototype](native-ui.md) now implements the three-texture seam and F1 panel. The inventory-stage pilot discussion below records the source evidence and replacement constraints; its former unimplemented status is superseded by that native implementation.

## What the renderer currently does

The guest texture descriptor is decoded by `gpu::Describe()` into a `TextureDesc`: guest address, original format, width/height/depth, mip count, pitch, cube/linear flags. `DecodeLevel()` converts a guest mip/face from Xbox swizzled, linear, palette, or compressed representation into host-ready pixels. D3D12's `BindGuestTexture()` keys the cache by palette address and guest data address, checks descriptor shape and a per-frame signature/dirty epoch, and calls `UploadTexture()` when a resource is new or its content changes. `UploadTexture()` decodes every face and mip and `UploadTextureLevel()` stages it to the GPU. Target textures are handled on a separate path and must not be treated as replaceable source textures.

The SDK extension interface already offers two observation hooks:

- `Extension::Decoded(desc, format, face, level, const pixels, pitch, flags)` runs after source decode and before upload. The pixels are const; the extension can set flags but cannot substitute pixel data through this callback.
- `Extension::Sampled(desc, target, flags, TexInfo&)` runs once a texture is selected for a draw and can annotate sampling metadata. It does not replace the GPU resource.

The native UI patch now adds `MatchUiReplacement` inside `UploadTexture()` and selects a separate immutable resource from `BindGuestTexture()`. `Extension::Decoded` remains observation-only. The three replacements leave guest RAM/descriptors unchanged; target/render-to-texture and depth resources keep their separate paths. The bounded implementation is described in [native UI](native-ui.md).

## Dimensions, UVs, and mip constraints

Guest `TextureDesc` dimensions are also used by shader constants, sampling classification, render-target identity, UV clamping, and capture metadata. The original D3D12 resource still uses those dimensions and mip count (with the existing compressed-size adjustment). The new three-entry `UiReplacement` record carries separate `physical_width`/`physical_height`; its host-only resource never changes the guest descriptor.

In principle a higher-resolution pixel image can be sampled through the same normalized UV range, but this SDK has paths where texel-space coordinates and half-texel/rect clamps are derived from the guest dimensions. Therefore physical replacement dimensions cannot be changed safely by only scaling an uploaded bitmap. Preserve the guest logical dimensions and the title's UV mapping; verify the actual shader and UV conventions for each supported asset class before introducing a separate host-resource size. The asset inventory must establish which entries are static, animated, palette-dependent, render targets, or sampled with texel-space coordinates.

Mip levels are part of the descriptor and upload/cache logic. A simple single bitmap replacement does not provide equivalent mips, cube faces, 3D slices, alpha treatment, or compression behavior. Initial replacement scope should be limited to inventory-confirmed static 2D assets, matched by stable content identity plus original descriptor metadata. If a replacement lacks a compatible mip chain, either build a documented host mip chain that preserves alpha/edge behavior or decline the match and use the guest texture.

The cache identity must include the replacement set/version and source descriptor (and palette where applicable), otherwise a changed replacement could reuse stale GPU contents. Resource admission needs explicit decoded-byte and host texture limits; the SDK's desktop resource bounds are not browser budgets. Keep all replacement files local to the player's browser and never upload them.

## Implemented first pass: bounded native pilot

The original private comparison pilot supplies three static UI textures joined to source identity and overlay UV evidence. `native-pack.py` now packages those existing files for opt-in native replacement; identity and enlarged matches were observed in the original menu. This note does not embed game assets. Art-quality approval remains separate from this functioning developer override.

The native seam compares all decoded pixels (including palette colors/alpha), format, dimensions and pitch. It admits only the three single-mip static swizzled 2D classes. Three immutable host resources have separate physical sizes, bounded dimensions/bytes and permanent UI descriptors; failed matching/admission/allocation falls back to the original. Guest UV/clamp constants remain logical. This does not admit other asset families or target/render-to-texture resources.

Use content-class gates rather than a blanket “upscale” switch:

| Asset family | First-pass treatment | Evidence/limit |
| --- | --- | --- |
| Static prompts, corner marks, logos | Candidate for deterministic local image upscaling or manual cleanup after exact identity and UV join | Static 2D status and actual UV bounds must be established per entry. |
| Panels, arrows, simple badges | Candidate for vector redraw when geometry/flat-color structure is verified | Preserve original silhouette, alpha edges, colors, and guest placement; do not infer geometry from a preview alone. |
| Fonts and glyph atlases | Block font-aware replacement pending glyph rectangles, character mapping, and advance/baseline metrics | A 512×512 atlas is observed in a runtime capture, but the captured draw UV inputs do not yield glyph bounds or metrics. Do not use an assumed grid. |
| Animated/UI sequence frames | Preserve source frames and timing until sequence linkage and timing are known | Decoded frames alone do not prove animation timing, ordering, or state transitions. |
| Mip-sensitive, palette-dependent, compressed, mesh, cube/volume, or dynamic resources | Preserve original; exclude from pilot | Requires compatible mip/face/palette/format behavior and a verified sampling contract. |

The native application was built and inspected with identity and 4x pilot replacements. This does not establish visual-quality approval or complete guest equivalence. The current branch has a compiled browser renderer/input/audio pipeline; the earlier inventory snapshot predates it. Its CPU-renderer decode/cache adapter is separate from this native D3D12 replacement seam and remains unimplemented. The image pilot does not unblock or prove browser rendering.

The local inventory currently records many decoded resources but not a complete joined presentation map. Evidence supports isolated overlay leaf views for `over_main.obe` and `over_option.obe`, yet parent/sibling transforms and semantic region labels remain incomplete. The public report must not promote candidate filenames, visual similarity, or isolated leaf geometry into confirmed in-game roles.

## Upscaling options and limits

The delivered read-only pilot is generated by
`tools/ui/replacement-pilot.py` from the three statically confirmed component
textures in `profiles/ui-components.json`. It emits 2x nearest-neighbor controls
and 4x premultiplied-alpha Lanczos comparisons, plus input/output hashes and a
recipe. It was run locally with Pillow 12.2.0. Originals, UV bounds and logical
dimensions remain unchanged; the comparison images have larger physical sizes.
Lanczos adds intermediate alpha even to an original cutout, so these are review
images, not approved drop-in replacements. No additional mip chain is generated; native admission explicitly requires one mip.
Logical/physical resource separation and representative draw observation are now
implemented. Manual edge cleanup and broader art/sampling acceptance remain follow-up work. The separate settings composition is explicitly labeled as a
proposed layout with a substitute font, and writes no runtime preferences.

The SDK's `render_percent` controls the offscreen target's render scale relative to output height; it is not a texture replacement control. Its D3D12 sampling path includes filtering and texel-correction logic for scaled targets, and `Extension::MakeTarget` can influence target scale. The compiled browser renderer needs its own bounded settings and decoded-texture adapter; native D3D12 descriptors and DXGI VSync are not portable browser interfaces. Browser application and performance remain separate work.

Recommended order:

- **Observed baseline:** guest-resolution frame rendered and presented with faithful aspect, crop, gamma, alpha, and nearest/linear behavior as evidenced per path.
- **Proposed setting:** bounded internal render scale with an explicit default matching the original guest target; cap dimensions and memory before allocation.
- **Proposed presentation filter:** choose only among filters implemented and measured in the actual browser backend; describe visual trade-offs plainly and keep the default fidelity-preserving.
- **Later optional asset replacements:** per-texture local overrides behind exact matches and opt-in controls, distinct from render scale.

Do not present a larger canvas, browser device-pixel ratio, or enlarged screenshot as proof of higher-quality original rendering. Do not promise HDR, anisotropic filtering, arbitrary texture replacements, temporal upscaling, or native desktop parity from the current browser UI/runtime.
