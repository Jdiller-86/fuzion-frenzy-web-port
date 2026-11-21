# Local-ISO SDK browser build

[Browser build downloads and checksums](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases?q=browser)
· [Successful browser build workflow](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/43036929326)

The browser builds run the original title and pinned SDK in WebAssembly with
local ISO intake. The tagged preview uses the SDK software renderer. The current
source candidate defaults to WebGL2 and offers the software reference under
Compatibility options. The loopback host serves the application files.

## Run the packaged build

Extract `FuzionFrenzyBrowser.zip` completely. On the Windows computer used for
the native builder, double-click **Start Browser.cmd**; this uses the installed
Python 3.12. On Linux/macOS run:

```sh
python3 tools/serve-sdk-browser.py --open
```

Use the URL opened by the launcher in a current 64-bit Chrome or Edge (normally
<http://127.0.0.1:4182/sdk-browser/>). The host tries port 8765 and then an
OS-selected port if a preferred port is reserved or already occupied.
Select the compatible USA ISO/XISO, then **Start game**. Opening the HTML as a
file will not work: browser threads require the supplied isolation headers.
No game compiler is needed to run the package.

If an older download fails with **WinError 10013**, run
`py -3.12 tools\serve-sdk-browser.py --open --port 8765` from the extracted
folder, or update the browser package. This error occurs before game execution:
Windows denied the local server's port. The new host handles reserved/busy ports
automatically; if every port fails, check local security software or policy.
Browser saves are scoped to the URL/port; use the same working port to retain
access to them (`Start Browser.cmd --port 8765` in the updated package).

The intake probes known XDVDFS partition offsets using the existing range
reader, extracts only `DEFAULT.XBE`, and checks its 2,834,432-byte size and
SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.
The original image stays in a lazy, read-only WORKERFS mount. The SDK mounts that
image as the original disc before boot. Nothing uploads the ISO or assets.
The check establishes build compatibility and local access, not legal ownership.

Enter is Start, Space is A, Escape is B, X/Y are the corresponding face buttons,
WASD is the left stick, arrows are the D-pad, and Z/C are triggers. The current
module supports four standard gamepads, with the keyboard sharing the first
seat. Focus loss clears guest input and controllers must return to neutral
before play resumes. Save data is stored in browser-local IndexedDB when
available. **Stop game** waits for a final local save flush before re-enabling
Start. Clearing browser site data clears those saves. See the [local beta
guide](browser-beta.md) for settings, shortcuts and tester instructions.

## Build from source

Keep the SDK and native configuration in ignored `.tools/`, using the versions
recorded below. The measured GPU candidate used Emscripten 3.1.50 with native
Wasm exceptions and O3. [Pinned build and cache details](browser-gpu.md#run-and-rebuild).
From the repository root:

```sh
source .tools/emscripten-env.sh
python3 tools/build-sdk-browser-full.py --opt=-O3 --jobs 2
python3 tools/serve-sdk-browser.py --open
```

The builder trims generated ThinLTO cache entries to 128 MiB after the linker
exits, including entries from the current job.
Successful full builds remove stale generated objects. Run the host against
the current module directly during development. Use
`python3 tools/package-sdk-browser.py` when you need a portable ZIP. Keep one
current package and reuse one scratch directory for transient browser evidence.
[Browser build storage](../AGENTS.md#browser-build-storage)

The existing Node host, `node tools/serve-sdk-browser.mjs`, serves the same page.
Both hosts bind loopback and expose only explicit source/module routes. They
never serve the game image, the repository as a directory, or arbitrary files.

Append `?sceneNodePool=1` to the local WebGL2 launch URL to try pooled
allocation for two verified scene-reader call sites. The build admits the
pool only for matching title source shards. Ordinary launches use the
original allocator. Add `&pacingTiming=1` to capture local pool timing.
[Scene-node pool evidence](browser-gpu.md#verify-scene-node-pool-fallback-and-ownership).

Append `?audioOutput=worklet` to try the bounded shared PCM queue with
AudioWorklet. The scheduled-buffer path remains available on ordinary launches
and if the worklet cannot run. [Audio worklet evidence](browser-gpu.md#play-original-pcm-through-a-shared-audio-worklet).

The packager includes only the browser files, compiled module, host, SDK notice
and build manifest, plus startup instructions. It includes compiled title code
from the source supplied to the project, but no ISO or original assets.

`tools/prepare-sdk-browser-overlay.py` creates disposable SDK source copies
with translated guest pointers and portable platform boundaries. It leaves the
SDK checkout and retained title source unchanged. `full-memory.cpp` adapts the
existing SDK allocator algorithms into four bounded 64 MiB backing windows:
user, physical, system and stack. The two console physical aliases share the
same physical window. Native process relaunch aborts explicitly in this browser
candidate. The module starts with 512 MiB linear memory, can grow to 2 GiB, and
prepares twelve pthread workers. The WebGL2 backend executes programmable vertices,
triangles and pixel combiners, and presents actual guest surfaces. Native mixed PCM
feeds WebAudio. Gamma adjustment and rumble remain unimplemented.

## Build evidence and remaining boundary

The GPU build compiles and links **92 source objects**, including all 64 title
shards, function mapping and the owned-command GPU bridge. The [GPU candidate
guide](browser-gpu.md) records actual local execution, performance, full-window
presentation, build commands and unsupported rendering features. Download links
above point to the tagged software previews. Build the GPU candidate from source using the guide.

The source baseline is XboxXBERecomp
`58757ab255c0a6b3929f405d7a5f1567fcd7fda3` and native game configuration
`071dbd97bdc098c4647cc24c99f9eae693fde1eb`, plus the runtime fixes published in
project commit `93ec832`. The overlay applies that patch with its recorded
before/after source hashes. Its shared fixed-function texture-cascade correction
is consumed by the CPU reference renderer through `MakePixelKey`. The D3D12
descriptor-rollover fix remains in the native backend; WebGL uses no D3D12
descriptors. Native evidence remains separate from browser evidence.

The local WebGL2 build navigated the original menus and character selection and reached an Erodeo winner. Four Erodeo gameplay windows measured roughly 60 callback FPS on the development desktop. Consumer interval p95 was 21.0–21.4 ms. Physical cadence, integrated-GPU performance and full audiovisual round acceptance remain open. [Measured results](browser-gpu.md#actual-gameplay-observations).

The game uses the available browser window at its original aspect ratio by
default. **Game menu** exposes fullscreen and Stop game. F1 opens Settings and
F2 toggles performance. Pause in the original game before opening Settings
during a round. Fullscreen permission remains subject to browser policy.

Next: reduce remaining CPU assembly/submission costs, verify an entire controlled
minigame and extend unsupported GPU contracts from actual failures. The
[local implementation goal](local-menu-and-browser-goal.md) records the acceptance
boundary. M44 and product milestone acceptance remain unchanged.
