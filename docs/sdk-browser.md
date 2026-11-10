# SDK browser candidate

This development path compiles the retained full SDK-generated title into a
browser module. It is separate from the accepted M44 startup proof and the
Twisted System research prototype. No original rendered frame or playable
screen has been established by this build work.

The managed Linux build compiled all 64 title shards plus their function map
with the checked overlay and linked the real browser module with the runtime
bridge and SDK crypto code. The development `-O0` Wasm artifact is 175,438,485
bytes with 256 MiB initial linear memory and a 1.75 GiB maximum. No title entry
was invoked: this workspace has no matching original image or assets. The
module remains ignored local build output.

The candidate uses the existing 64 generated title shards and function map,
the pinned SDK CPU helpers with a build-time checked-memory overlay, and a
browser entry adapter. The adapter dispatches generated routines and reports
unsupported instructions, services, indirect calls and memory addresses. The
kernel bridge implements RTL memory/byte-swap operations and calls the existing
SDK SHA, HMAC and RC4 algorithms. It does not implement the full Xbox kernel.

## Prepare and build

Use Python 3.11+, Git and the project's Node version. Keep downloaded tools and
compiled title artifacts in ignored `.tools/`. Prepare the matching source:

```sh
git clone --branch fuzion-frenzy-port https://github.com/abilliontokens/XboxXBERecomp .tools/XboxXBERecomp
git -C .tools/XboxXBERecomp checkout 58757ab255c0a6b3929f405d7a5f1567fcd7fda3
git clone https://github.com/abilliontokens/fuzionfrenzy-recomp .tools/fuzion-native
git -C .tools/fuzion-native checkout 071dbd97bdc098c4647cc24c99f9eae693fde1eb
```

An existing Emscripten installation can supply `em++`. The managed Linux
environment could not download the pinned 3.1.50 release, so a separate
experimental Debian Emscripten 3.1.69/LLVM 19 setup is available for Debian 13
x86-64 with system `libllvm19` and its shared-library dependencies. It extracts
packages locally; it changes no system packages or network settings:

```sh
python3 tools/setup-wasm-toolchain.py
source .tools/emscripten-env.sh
python3 tools/prepare-sdk-host-header.py
python3 tools/build-sdk-browser.py --opt=-O0 --jobs 2
node tools/serve-sdk-browser.mjs
```

The host-header command reconstructs the upstream emitter's bindings from the
existing game configuration and its inherited XDK layout. It does not analyze
an executable or discover additional original symbols. The build keeps the
retained title sources unchanged. Compiler output is private local build
material and is excluded from the source-only site build.

Open <http://127.0.0.1:4182/sdk-browser/>. Select the matching original XBE and,
optionally, its extracted local game directory. The matching XBE is 2,834,432
bytes with SHA-256
`45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.
The worker rejects other images before invocation. Selected files stay in the
browser. The server exposes only the candidate page, its explicit source
dependencies and the two locally compiled module artifacts.

The temporary filesystem mounts the selected directory under `/game` and exposes
the selected XBE at `/game/DEFAULT.XBE`. It uses Emscripten's existing WORKERFS
backend to read local file slices on demand without copying the entire game
into memory. The read-only mount admits up to 16,384 files and 8 GiB of selected
file sizes. Kernel file APIs still need connecting to this filesystem;
mounting files alone does not make title resource loading work. Stop terminates
the worker, including an entry that does not return.

## Remaining execution connections

The current guest window is 64 MiB. Access beyond that window stops explicitly;
the SDK's high virtual system/stack ranges and physical aliases still require
translation. Kernel allocation, boot/thread/TLS initialization and file/device
services need connecting to browser implementations. The SDK software reference
renderer is available upstream for an initial original framebuffer; its graphics
backend, browser presentation, input and audio remain to integrate.

The source upload intentionally omitted the XBE and assets. This workspace can
compile the actual title but cannot observe its next original execution failure
without matching player-local inputs. Continue development against those local
inputs using this page and fix the actual reported boundary. Compilation does
not advance M44, accept M1, or establish fidelity. Smoke and regression tests
are deferred until a working baseline at the user's direction.
