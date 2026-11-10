# Fuzion Frenzy Windows builder

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
