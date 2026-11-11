# Native settings and three-texture replacement prototype

The native F1 panel now uses the three confirmed header components, a bright/dim
focus list and a proposed segmented render-scale meter. It has four live SDK
settings, Apply/Save/Cancel, keyboard navigation and port-0 pad navigation. The
layout is a host addition; its development typography is not the original font.
The original guest Options menu remains available.

The integration starts from `f8e4087` on `codex/sdk-baseline-execution`, with
inventory commit `d62f99d` cherry-picked as `bddbe6b`. Native source remains pinned
to `071dbd97bdc098c4647cc24c99f9eae693fde1eb`; SDK to
`58757ab255c0a6b3929f405d7a5f1567fcd7fda3`. Retained copies have no own Git
metadata. Private build manifests identify the actual source/header/object bytes.
The original installed executable is preserved.

## Build and launch

Use the [incremental builder](../tools/ui/BUILD-NATIVE-UI.md) and existing native
release object cache. `-ApplyPatches` applies `runtime-fixes.patch`, then
`ui-replacements.patch` to the SDK, and `native-ui.patch` to native source.
The [UI patch manifest](../desktop/windows/patches/ui-manifest.json) records
normalized before/after identities. Without that switch, supply the already
patched retained source copies. The standard Windows builder **0.1.2** now applies these same patches during its
normal CMake build, generates the three-image pack directly from the selected
local ISO/folder and launches it with the chosen replacement mode. Installing
the updated builder requires a subsequent game rebuild. The separate commands
below remain the developer path.

Build the local pack from the existing inventory and pilot, without rerunning
extraction:

```powershell
python tools/ui/native-pack.py --inventory '<private>/ui-inventory/inventory.json' --pilot '<private>/ui-replacements/pilot' --out '<private>/ui-native/pack'
```

Place the matching local disc under the new executable's `assets/` directory.
Keep its own `hdd/`, `logs/` and `settings.toml` separate from the installed game.
Then launch from PowerShell:

```powershell
$env:FUZION_UI_PACK = '<absolute private path>/ui.bgra'
$env:FUZION_UI_REPLACE = 'pilot'
& '<absolute private build>/FuzionFrenzy-ui.exe'
```

F1 opens/closes; arrows select/change; Enter or pad A selects; Escape or pad B
cancels. Mouse clicks select rows and advance values. F2 retains its existing
performance panel. Use the original Start pause menu before F1 during gameplay;
the host panel does not pause simulation. Original menu idle/attract timers also
continue while F1 is open.

## Implemented behavior

| Control | Native effect |
| --- | --- |
| `fps_cap` | Existing SDK frame limiter; zero is unlimited. The live counter showed 30 and 60 fps at those caps. |
| `render_percent` | Existing target-recreation path; 90% produced 1728×1296, 100% produced 1920×1440 on the observed 2560×1440 output. |
| `aspect_ratio` | Existing shape/presentation path, corrected to honor forced 4:3 and 16:10 on a wide desktop. 4:3 visibly pillarboxed; 16:9 filled the output. Forced 16:10 is presentation shaping, not a recovered native 16:10 projection. |
| `vsync` | Existing DXGI Present interval; observed changes reached intervals 0 and 1. Driver/display behavior beyond this call was not measured. |

Apply updates live state without writing the file. Save applies and writes the
existing SDK TOML. Cancel/F1/B/Escape discard edits since the last Apply; they
do not roll back previously applied changes. Save/restart/reopen retained values.
Scale/aspect recreate targets without window reconfiguration. A transient target
recreation frame was visible before the arena settled; no persistent corruption
was seen in the subsequent paused view.

`overlay::CapturePad` receives the final pad sample after the existing physical,
keyboard and optional diagnostic adapters. While open, `PadHook` returns neutral
buttons/axes on all ports, preserving connection identity. Port 0 feeds a bounded
32-edge queue to the UI. After closing, both pad and keyboard adapters wait for
neutral/released controls before restoring guest input. Stick deadzones avoid
permanent ownership from ordinary analog drift.

Actual keyboard changes, timed pad stick/D-pad navigation and B dismissal were
observed. The guest pause remained on Continue with its timer at 1:00 after a
draft adjustment and cancellation. Normal guest navigation then opened its
sound/music/vibration Options. No physical XInput ports were connected; physical
hardware interaction remains unverified even though it uses the same capture hook.

## Bounded replacement seam

`gpu::UiAssets` admits exactly three local entries: emblem, gradient divider and
wordmark, in that slot order. The pack is capped at 8 MiB; admitted dimensions and
formats are fixed. `off` leaves guest textures unchanged, `identity` substitutes
the exact native-decoded pixels, and `pilot` uses existing 4× comparison pixels.
The pack/mode are immutable until restart. Missing/invalid packs or failed GPU
allocation retain original guest textures.

At `D3D12Backend::UploadTexture`, `MatchUiReplacement` compares every decoded BGRA
byte, including palette colors/alpha, plus format, dimensions and pitch. It
requires swizzled, uncompressed, non-depth 2D source textures with one mip.
Render targets use a separate binding path. Cube, volume, linear and multi-mip
resources are excluded. The wordmark match converts preview-rounded 5-bit colors
to the SDK's exact bit replication; the private recipe records this step.

The guest `TextureDesc`, memory, UVs, layout, texel constants and clamp behavior
remain unchanged. Physical replacement sizes are held only by separate D3D12
resources. Three immutable resources are shared by matched cache entries and
owned through the backend lifetime. Permanent ImGui heap slots 1–3 are separate
from the guest descriptor ring. Replaced cache references retire with frame
garbage; backend destruction waits for queued work. The published descriptor
rollover and texture-cascade fixes remain intact. The observation-only
`Extension::Decoded` API was not misused as a replacement callback.

All three identity and 4× pilot matches were logged in the running original menu.
Screens were inspected for placement/cropping/alpha problems. The pilot is a
functioning development override, not an approved art-quality replacement set;
no new mip generation, color key or alpha reconstruction is inferred.

## Typography, evidence and continuation

The F1 face is ImGui's embedded **ProggyClean.ttf, 20px**, measured ascent 15,
descent −6 and line height 20 before UI scaling. It is recorded in the component
configuration and isolated from F2's existing font. The bounded
[font investigation](ui-font-recovery.md) identified custom texture descriptors
inside all 30 font-named slices; original glyph mapping/metrics remain unresolved.

Private evidence is under `artifacts/ui-native/`: captures, run logs, pack recipe,
font report, launch/build commands and source/input identities. Original art and
proprietary listings are excluded from commits. The application/compiler and
publication checks were used; broad regression/synthetic suites were deferred.

The compiled browser renderer/input/audio pipeline from `797e9ae` is preserved.
Its next adapter seam is `web/sdk-browser/app.mjs` (host panel/focus),
`worker.mjs` (`parseInput`/settings messages), and
`runtime/browser/sdk/full-platform.cpp` / `full-entry.cpp` plus the SDK's
`harness/reference_renderer.cpp` decode/cache path, admitted through
`tools/prepare-sdk-browser-overlay.py` and `tools/build-sdk-browser-full.py`.
Browser capability validation, local preference persistence and replacement
admission need explicit implementations; native DXGI VSync and SRVs do not port.
This batch establishes no browser UI parity or milestone acceptance.

Next bounded native batch: exercise a connected physical controller through
open/adjust/cancel/release/return, then identify a validated guest pause-mode
lifecycle observer before adding automatic pause or a contextual original-menu
entry hint. Original font recovery separately needs the unresolved object-method
consumer joined to live atlas pixels and assembled glyph UVs.
