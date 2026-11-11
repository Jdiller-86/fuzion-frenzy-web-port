# Fuzion Frenzy Windows builder

Version **0.2.0** implements the selected **Control Room** interface: olive-metal
surfaces, amber dot-matrix status and a single Game / Source / Display equipment
bank. There are no navigation tabs or duplicated settings/build destinations.
The matrix contains only the current status/phase and progress, without secondary
captions or repeated configuration values.

Use **Settings** for launch preferences, **Build Options** for paths/jobs, and
**Controls** for the input reference. **Install Build Tools** is directly available
beside the source selector. Build errors open the log; platform/version remain at
bottom right. [Layout and behavior](../../docs/control-room-launcher.md).

The builder includes the [native styled F1 settings panel](../../docs/native-ui.md),
three local UI texture overrides, and the [Windows rendering repairs](patches/README.md)
into the normal build flow.

[Download the Windows installer preview](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases/download/windows-builder-v0.2.0-preview.1/FuzionFrenzySetup.exe)
· [Release and checksum](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases/tag/windows-builder-v0.2.0-preview.1)

Close the old launcher and install the update. Existing game builds, saves and
settings remain available. If you already rebuilt with 0.1.2, this visual update
does not require another game rebuild. To add the native UI features to an older
game build, choose **Build** again with your ISO or complete extracted folder.

Build now extracts only the three confirmed UI resources from your local game
packages and generates the artwork pack automatically. It does not require an
inventory, screenshots, Node, or developer commands. Python installs Pillow
12.2.0 into the private UI build cache on first use; this downloads software only.
The pack and its provenance stay beside your locally compiled executable.

**Play** uses the selected **UI artwork** mode: **4x preview** (default),
**Original pixels**, or **Replacements off**. Choose the mode in **Settings**, then **Save**; it takes effect on the next launch. The 4x artwork is a resampling preview,
not a finished HD art pack. Missing/invalid runtime packs retain original textures.

**F1** opens frame cap, render scale, aspect ratio and VSync with Apply/Save/Cancel.
**Pause gameplay through the original Start menu first**: F1 does not pause the
simulation. Typography is a development font. Physical-controller interaction
still needs hardware observation. The existing launcher retains the broader
settings controls, and **F2** retains the performance panel.

The release workflow compiles the launcher and NSIS installer on Windows Actions.
It contains the builder, not game content.

Target: Windows 10/11 x64 with a D3D12-capable GPU. The launcher includes its
.NET runtime; the separate C++ game compiler is installed on demand.

Install `FuzionFrenzySetup.exe`, open **Build and Play**, select your compatible
USA Fuzion Frenzy ISO/XISO or extracted game folder, then choose **Build**. The
first build downloads the pinned native SDK and game-port source and compiles
your game locally. No game is included, downloaded, uploaded or compiled on CI.

Use **Install Build Tools** if the required Microsoft C++/Clang toolchain, CMake,
Git or Python is missing. This is a separate user action because the development
tool downloads are large and the Microsoft installer can request elevation.
The game build can take substantial time; its log remains visible and cached
build files speed later builds.

Build-tool installation output appears in the launcher and is saved under
`%LOCALAPPDATA%\FuzionFrenzy\logs\prerequisites-*.log`. Individual installers
request elevation as needed. The builder supports standalone LLVM in
`%ProgramFiles%\LLVM` as well as Visual Studio's bundled Clang.

The supported original `default.xbe` is 2,834,432 bytes, SHA-256
`45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.
The builder rejects other revisions before recompilation. Compressed archives
need extracting first.

The launcher keeps the built game, settings, saves and logs under
`%LOCALAPPDATA%\FuzionFrenzy`, separately from the installed builder. Rebuilds
preserve saves and settings. Uninstalling the builder preserves this directory.
The built native executable is `game\FuzionFrenzy.exe`; it requires its local
`assets` directory and any staged runtime DLLs.

The launcher offers display/settings editing, play, cancellation and shortcuts
to local data folders. The existing native runtime also has **F1** settings and
**F2** frame-time overlay, keyboard-to-pad support and controllers. The launcher
writes the runtime's actual settings keys. Native execution uses D3D12 and
XAudio2 on Windows and is independent of the browser/Wasm product.

## Download and package

The repository's **Windows local-game installer** Actions workflow builds a
downloadable **FuzionFrenzy-Windows-Installer** artifact containing the installer
and SHA-256 checksum. It compiles only the launcher and installer. Builds are
unsigned development output; Windows runtime behavior has not been established
by the packaging job.

Pushing a `windows-builder-v*` version tag packages that exact source revision
and publishes the installer and checksum as a GitHub prerelease. Normal main
updates retain downloadable build artifacts without publishing a release.

To package on Windows with .NET 8 SDK and NSIS 3.11 installed:

```powershell
./desktop/windows/Package-Installer.ps1
```

On Linux with .NET 8 SDK and NSIS 3.11 installed, the same Windows application
and installer can also be cross-packaged:

```sh
./desktop/windows/Package-Installer.sh
```

`DOTNET_BIN` and `MAKENSIS_BIN` can select user-owned tool installations; an
extracted NSIS distribution also needs its normal `NSISDIR`. This is a packaging
fallback, not Windows game or UI execution evidence. No game files are needed.

Output: `artifacts/windows/FuzionFrenzySetup.exe`. Smoke and regression tests are
deferred by project direction; packaging success is not game execution evidence.

The native integration reuses:

- [fuzionfrenzy-recomp](https://github.com/abilliontokens/fuzionfrenzy-recomp),
  revision `071dbd97bdc098c4647cc24c99f9eae693fde1eb`.
- [XboxXBERecomp](https://github.com/abilliontokens/XboxXBERecomp), revision
  `58757ab255c0a6b3929f405d7a5f1567fcd7fda3`.

Upstream reports native Linux playability and a Windows cross-build, while its
Windows hardware execution and audio remain unverified. Those reports are
upstream evidence, not validation of this installer or browser milestone.

## Alternative launcher concepts

Four separate [interactive design prototypes](design/concepts/README.md) explore
graphite, silver equipment, colorful console and light desktop directions.
They are visual proposals with simulated states; the released Windows launcher
is unchanged by this design work.

The [Alloy studies](design/alloy-studies/README.md) add four hardware-oriented
alternatives with animated dot-matrix progress displays, while retaining every
original concept. These remain simulated visual prototypes.

The [B3 Studio refinements](design/studio-refinements/README.md) add an amber
version of the selected B3 design and four further directions. All earlier
concepts remain selectable in that prototype gallery.
