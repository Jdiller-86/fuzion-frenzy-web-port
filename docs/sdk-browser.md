# Local-ISO SDK browser build

[Browser build downloads and checksums](https://github.com/Jdiller-86/fuzion-frenzy-web-port/releases?q=browser)
· [Successful browser build workflow](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/43036929326)

This build compiles the original title and the full pinned native SDK into
WebAssembly. It uses the SDK's kernel services, guest threads/TLS, disc reader,
XDK overrides and CPU reference renderer. Rendering, input and audio run in the
browser; the local HTTP host only serves the application files.

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
WASD is the left stick, arrows are the D-pad, and Z/C are triggers. A standard
connected gamepad controls the first seat. Focus loss clears held keyboard
input. Save data is stored in browser-local IndexedDB when available; **Stop
game** requests a save flush and terminates the guest threads. Clearing browser
site data clears those saves. Storage failure is reported with session-only
fallback. No saves or logs are uploaded.

## Build from source

Keep the SDK and native configuration in ignored `.tools/`, using the versions
recorded below. An Emscripten 3.1.69 compiler is required for the currently built
candidate; the managed Debian fallback is documented by
`tools/setup-wasm-toolchain.py`. From the repository root:

```sh
source .tools/emscripten-env.sh
python3 tools/build-sdk-browser-full.py --opt=-O1 --jobs 2
python3 tools/package-sdk-browser.py
python3 tools/serve-sdk-browser.py --open
```

The existing Node host, `node tools/serve-sdk-browser.mjs`, serves the same page.
Both hosts bind loopback and expose only explicit source/module routes. They
never serve the game image, the repository as a directory, or arbitrary files.
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
prepares twelve pthread workers. The CPU renderer presents actual guest surfaces
and sends native mixed PCM to WebAudio. Gamma adjustment and rumble remain
unimplemented, and GPU acceleration is not yet connected.

## Build evidence and remaining boundary

The full build compiled and linked **91 source objects**, including all 64 title
shards and the function mapping. The initial optimized module is approximately
25.7 MB before ZIP compression. The bundle is approximately 3.6 MB. This is
compilation/link/package evidence, not original-game execution evidence.

The source baseline is XboxXBERecomp
`58757ab255c0a6b3929f405d7a5f1567fcd7fda3` and native game configuration
`071dbd97bdc098c4647cc24c99f9eae693fde1eb`, plus the runtime fixes published in
project commit `93ec832`. The overlay applies that patch with its recorded
before/after source hashes. Its shared fixed-function texture-cascade correction
is consumed by the CPU reference renderer through `MakePixelKey`. The D3D12
descriptor-rollover fix remains in the native backend; the browser software
renderer uses no D3D12 descriptors. The user reports smooth Windows execution
and a local agent's geometry-stability verification. Browser rendering has not
been observed here and remains separate from that native evidence.

This workspace still has no compatible original ISO/assets. No original browser
frame, game round, performance target or M1 acceptance is claimed. The CPU
reference renderer has different performance characteristics from the Windows
D3D12 backend. Next: run this build with the matching local image, fix the actual
first failing boundary, and then connect accelerated browser graphics. The accepted M44 proof remains
unchanged. Smoke, unit and regression suites remain deferred by user direction.
