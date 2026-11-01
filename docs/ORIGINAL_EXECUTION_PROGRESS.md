# Original execution: implementation ledger and build evidence

Current full objective: implement `DEVELOPMENT_PLAN.md` with the actual original game executing entirely inside a desktop browser, plus its planned browser/online/social features. The user's exact-original requirement supersedes reconstruction/approximation allowances. None of the gates below is satisfied by the previous Twisted System remake. This ledger preserves the full objective; completing a build spike is not completion of the product.

## Product acceptance ledger

| Requirement retained from the plan and user clarification | Evidence required | Current state |
| --- | --- | --- |
| Original instructions executing entirely in browser | Actual compatible XBE executes in a normal browser with original controllable screen; no streamed/native-helper game | Not implemented. A synthetic static-recompilation probe passes in-browser; the next gate is one original initialization function. |
| Every original detail, catalog, characters, arenas and modes | Matching-build menus, instruction/practice flows, original rules/AI, animations/camera, audio/video, save behavior, round/tournament cases and parity inventory | Not established. Prior footage/static inventory is reference evidence only. |
| Player-supplied local content | Bounded worker reads, compatible-build checks, no original uploads/serving/distribution, clear/reselect and save/storage recovery | Prior importer proves one asset profile. New original-code runtime still needs local block/file devices and bounded storage. |
| Keyboard/gamepad, remapping and owned seats | Original runtime receives correct virtual controllers for 1–4 seats; focus/hotplug/calibration and physical-device tests | Prior input adapters/tests can be reused. A synthetic SDL browser keyboard event now passes; original virtual-controller integration and physical-device validation remain incomplete. |
| Offline solo/local and useful CPU fill | Complete original solo/local/practice/tournament behavior and original CPU difficulty | Not achieved by the approximate prototype. Use original execution rather than new guessed bots. |
| Online and mixed local/remote play | Four-browser and composite owned-seat cases; timing/contact/steering/orb cases under plan's latency/loss profile | Unimplemented. Requires a runtime/state/input protocol and authority decision; a relay alone would not satisfy server-owned outcomes. |
| Lobbies and recovery | Discovery/privacy, codes/invites, content readiness, owner transfer, rematch, reconnect and separate match-process recovery | Unimplemented. Do not conflate owner transfer with game authority or machine recovery. |
| Accounts and social | Recovery, friends/requests/invites, privacy/block/report, direct API authorization and isolation tests | Unimplemented; no provider/service purchase or secrets introduced. |
| Tournament persistence and authority | Atomic checkpoints/results, idempotent commits, server-validated outcomes and recovery | Unimplemented. Original execution changes how the earlier shared-TypeScript simulation assumption could be met. |
| Performance/browser/release matrix | Measured Chrome/Edge/Firefox/Safari, physical controllers, GPU/audio recovery, 60/30 fps targets, importer working-memory and long-tournament soak tests | No original runtime measurement. A compiler check or old remake's browser run cannot establish it. |
| Versioning, delivery and operations | Protocol/content/rules compatibility, source/asset boundary, deploy/rollback/backup/load/admission checks and budget-approved services | Source-only guards/CI exist; production delivery/operations remain unimplemented and separately gated. |

Material architecture issue reported before affected network implementation: running the original XBE does not automatically provide the planned authoritative shared-TypeScript server. Machine RAM/save states may contain original code/assets and must not be uploaded as a shortcut. An input-only relay would not establish server-owned scoring/collisions or crash recovery. Preserve those requirements and resolve the execution/authority/content boundary before implementing online behavior; do not silently relax them.

## Native-web route reassessment

November 1, 2026: the user confirmed no local MCPX/BIOS and asked about a native port. The [native-web checkpoint](NATIVE_WEB_FEASIBILITY.md) records the actual pinned static recompiler, declared routes for all 111 imports, and one owned x86-to-C/Wasm function passing 25 browser arithmetic/return cases twice. Routes are not semantic coverage; no original instructions or kernel calls ran. The desktop runtime and Python/compiler pipeline still need a browser port and browser-local title conversion. Publishing a precompiled title module would violate the project's local-content boundary. The next task is a bounded original initialization-function proof using the existing analysis, with title-derived output selected locally and never served/uploaded/committed. This is a route investigation, not a final architecture freeze or a smaller goal.

Before this steering, the actual generator also produced a new native line/quad reference: 72 variants, 24 owned streams, 1,728 records and 746,640 outputs, with primitive-count and untouched-tail checks. Its source/data identities are in the native-web checkpoint. No corresponding browser prepass has been implemented; the triangle component and previous outputs remain preserved.

## Prior bounded source comparison

The earlier primary candidate was [xemu at `dc10b4c`](https://github.com/xemu-project/xemu/tree/dc10b4cea96e403d30c17ed53551fdaf023bc17f), because it implements the Xbox hardware and has title-compatibility evidence. Its documented native operation is not proof of browser compatibility. No native game/emulator streaming is proposed. That work remains preserved while the native route is evaluated.

Source observations at that pinned commit:

- `meson.build:955` requires the TCG interpreter for wasm32. `configs/meson/emscripten.txt` and `os-wasm.c` establish some inherited browser-platform support, not a working Xbox browser port.
- `meson.build:1885` requires epoxy; the OpenGL platform branch at 1889–1897 admits Darwin/Windows/Linux and otherwise errors. No Emscripten graphics branch exists there.
- `hw/xbox/nv2a/pgraph/thirdparty/gloffscreen/sdl.c` requests a shared desktop OpenGL 4.0 core context. The geometry generator's `pgraph_glsl_need_geom` returns true for ordinary triangles, lines and quads. Geometry shaders, desktop shader versions and shared-context behavior need a browser rendering solution; merely selecting WebGL 2 is not a port. [Emscripten's supported GL subset](https://emscripten.org/docs/porting/multimedia_and_graphics/OpenGL-support.html) does not establish compatibility with those desktop operations.

The [browser-oriented Rust core at `d548ff0`](https://github.com/ImLunaHey/emulators/tree/d548ff0a24a128e0aab2b534302e09f87d9f4a7d/packages/xbox) is not a substitute for a mature retail runtime. Its source documents starter CPU coverage, empty audio output and success/no-op kernel stubs; its mesh-demo commit explicitly reports a CPU floating-point failure. A symbol or handler being present is not behavioral coverage. No local game execution through that core is claimed.

## Executable dependency profile

`src/xbe.mjs` is an independently written bounded metadata reader, using the public [XBE layout](https://xboxdevwiki.net/Xbe). It validates consistent retail/debug/beta entry/import keys, executable file-backed entry, section and pointer ranges, TLS and import termination/count bounds. It never executes or emits game instructions. `tools/profile-execution.mjs` writes a new metadata-only report with an exclusive-create guard.

Reading the supplied folder directly produced:

| Metadata observation | Result |
| --- | --- |
| Executable identity | 2,834,432 bytes; SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`, unchanged after the work |
| Entry/profile | Retail key pair; entry `0x00030f7f`, matching existing Ghidra entry evidence |
| Sections | 11 declared sections; Ghidra's earlier 12 memory blocks are a different measurement; the exact block/section correspondence was not re-audited here |
| Image/stack/TLS | 3,239,712-byte declared image, 65,536-byte stack, TLS present |
| Kernel imports | 111 thunk slots / 111 unique ordinals; declarations, not observed calls or proof that any runtime handles them correctly |
| Libraries | Six declarations: XAPILIB/LIBC/XBOXKRNL/XGRAPHC 3911, D3D8/DSOUND 3925; consistent with prior static evidence |

Local report: `research/verification/original-execution-profile-20261031.json`. It stays ignored with the other analysis output. No disc extraction, executable reimport, full decompilation or separate Xbox firmware acquisition occurred.

Seven source-owned XBE fixture/CLI cases cover valid variants, mismatched keys, truncated/overlapping ranges, invalid import tags/termination, non-executable/BSS entry, TLS/BSS handling and library bounds. The CLI case also verifies outside-research rejection and exclusive report creation with source-owned temporary fixtures. They run within `npm run test:formats`; this is parser validation, not an execution test.

## Actual local cross-build attempt

Pinned tooling and locally measured compiler hashes are in `tools/original-execution-toolchain.json`. Sources, Linux Emscripten 3.1.50, its internal Node 16.20.0, Meson/Ninja and output are under ignored `.tools/emulation`. Project Node remains 24.14.0; no global PATH, Java, model, shell startup or system package settings changed. Only the pinned public keycodemapdb source was fetched for QEMU's subproject guard; firmware submodules were not initialized and no separate Xbox boot firmware was acquired.

The repository path contains spaces. A temporary symlink did not fix Python venv's canonical path. The existing Windows short path `C:\Codex\Projects\FUZION~1` was verified to reach the same project and used through WSL, without moving files. On another machine, determine its actual short path rather than assuming this one exists.

Attempt 05 used Emscripten's actual wasm32 compiler, passed QEMU's Python setup and progressed through Meson C/C++ feature checks. It terminated at `meson.build:1084`: host-machine GLib lookup failed because the generated cross file selected nonexistent `empkg-config`. The Wasm GLib prefix was also empty. This is an observed configuration failure, not a linker/game/runtime failure. Logs: `.tools/emulation/xemu-wasm-attempt-05.log` and its `meson-logs/meson-log.txt`.

The new `tools/probe-browser-build.py` now selects the real pkg-config and restricts it to `.tools/emulation/wasm-deps`; it checks pinned source commits and refuses to overwrite attempts. It runs configure, then builds only if configure succeeds. An earlier validation run (`dependency-01`) was deliberately interrupted after identifying that its initial wrapper version inherited the same pkg-config selection; it is not reported as a completed probe. The corrected wrapper was subsequently run with the prepared cross libraries, as recorded below.

zlib 1.3.1 from its [official source archive](https://zlib.net/fossils/zlib-1.3.1.tar.gz) built/installed into that Wasm prefix. Locally recorded archive SHA-256: `9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23` (not independently checked against a published digest). Although make warned about clock skew, a separate fresh link of `tools/fixtures/zlib_wasm_check.c` executed its Wasm output successfully under the SDK's Node: version check, golden CRC vector and compression roundtrip passed. Generated CommonJS glue needs an isolated `{"type":"commonjs"}` in its ignored probe directory because the application uses ESM. No application module setting was changed. Evidence: `.tools/emulation/zlib-wasm.json`, `zlib-wasm.log`, and `zlib-check/rerun.log`. This is a cross-library ABI check, not a browser or game run.

## Reproduce and continue

With the pinned local sources/SDK/Python build packages already prepared:

```powershell
node tools/profile-execution.mjs "Fuzion Frenzy (USA)/default.xbe" "research/verification/new-execution-profile.json"
npm run test:formats
npm run check
git diff --check
wsl -d Ubuntu -- python3 /mnt/c/Codex/Projects/FUZION~1/tools/probe-browser-build.py --tool-root /mnt/c/Codex/Projects/FUZION~1/.tools/emulation --attempt new-label
```

The output filename and build label must be unused. The probe's build dependencies are not installed by `npm ci`. Supplying the required Wasm cross libraries still leaves the browser frontend/GPU port and original-title execution proof; dependency installation alone cannot complete this route.

Follow-on dependency results: libffi 3.4.7, GLib 2.84.0 and Pixman 0.44.2 now cross-build/install in the isolated prefix. GLib's archive matched the [published SHA-256](https://download.gnome.org/sources/glib/2.84/glib-2.84.0.sha256sum); its PCRE2 source and Meson patch matched the wrap hashes in that verified source. `tools/fixtures/ffi_wasm_check.c` was freshly linked and its Wasm output passed signed32, uint64 and double foreign-call checks under the SDK Node. This validates those host ABI calls, not full TCG or game execution. Pixman compiled with unsupported floating-point-exception warnings; no render/numerical acceptance is claimed, and the current xemu profile skips Pixman. Local evidence: `libffi-wasm.json`, `ffi-check/check.log`, `glib-wasm.json` and `pixman-wasm.json` under `.tools/emulation`. GLib's generated config follows the pinned recipe's host-platform fixes for unsupported POSIX spawn/pthread-name probes; the original game kernel is not being stubbed by these host-library fixes.

The corrected `runtime-02` xemu probe found pkg-config, GLib 2.84.0 and zlib 1.3.1, then failed on the missing slirp subproject. The subsequent `offline-03` probe disabled guest NAT (`--disable-slirp`) and advanced to an unconditional libcurl subproject request at `meson.build:1473`. This is offline bring-up only; it neither implements nor removes the planned browser match transport. Stock Meson requests the curl fallback whenever curl is absent, even though it exposes a curl feature option. Do not blindly repeat the build with `--disable-curl` and assume that will remove the native frontend dependency. The current helper's reports preserve both real failures, and no QEMU executable has been produced.

## Partial browser host profile

The first host-service slice is implemented through `tools/probe-browser-build.py --browser-host`. It creates a new detached worktree at the pinned xemu commit, retaining the original checkout and every earlier attempt. Guarded Meson edits skip native curl selection on Emscripten and select source-owned host adapters. The adapter files are copied exclusively and changed file hashes are retained in the attempt report. The follow-up below adds the callback adapter and compiler-machine-file flags. No Xbox CPU/device/render code is edited. The key map is copied from the existing pinned public source without initializing firmware submodules. This is a partial host port, not a complete browser frontend, browser network security boundary or game runtime.

Reason for explicit exclusion: upstream [reporting.cc](https://github.com/xemu-project/xemu/blob/dc10b4cea96e403d30c17ed53551fdaf023bc17f/ui/xui/reporting.cc) base64-encodes all XBE headers in `SetXbeData`, includes that field in its serialized compatibility report, then exposes `http_post_json` in `Send`. The native GitHub report builder also depends on curl. These are source observations at the pinned commit, not an observed upload. No original files have been loaded into xemu. The browser profile makes native HTTP calls fail through the ordinary QEMU error contract and removes the native GitHub-report menu hook; it does not replace guest kernel/device behavior with success stubs or change the planned match transport.

Both new adapters compiled with Emscripten `-Wall -Wextra -Werror` against pinned xemu headers. QAPI headers were generated locally by that source's own generator. A fresh Wasm link of `tools/fixtures/browser_http_check.c` passed under the SDK Node: initialization/GET/POST fail, errors are reported, supplied data and progress counters remain unchanged, no transfer callback runs, and null optional arguments are accepted. Only the error allocator is replaced in this isolated fixture. This validates the host adapter contract; the entire runtime has not linked, executed or been tested in a browser. Local evidence: `.tools/emulation/browser-http-check-01/check.log` and `result.json`.

Reproduce the partial profile with a new, unused attempt label:

```powershell
wsl -d Ubuntu -- python3 /mnt/c/Codex/Projects/FUZION~1/tools/probe-browser-build.py --tool-root /mnt/c/Codex/Projects/FUZION~1/.tools/emulation --attempt new-browser-label --browser-host
```

The first isolated attempt, `browser-host-01`, passed the earlier curl fallback and stopped at the SDL3 dependency (`meson.build:1700` in the isolated source): no cross SDL3 was installed and automatic wrap downloads were disabled. This is a real subsequent configure failure, not a completed runtime build. See `.tools/emulation/xemu-wasm-browser-host-01.json`, `.log` and its Meson log. Reusing the attempt label was rejected with exit 2 and the saved report remained byte-identical. The original upstream checkout remains clean; the browser source worktree and generated artifacts stay ignored. Building more generic libraries or disabling a required rendering path is not a substitute for a controllable original-game browser screen.

## SDL3 browser backend and callback repair

SDL3 at xemu's wrap pin `829a65d769d935c4852f8159e964312c0957260a` (version 3.4.18) now cross-builds and installs in the isolated prefix. Its source remains unmodified. The actual generated config enables Emscripten audio, video and joystick backends plus pthreads; the setup retains xemu's disabled optional subsystems. Source acquisition and CMake setup/build/install commands all exited 0. Local evidence: `.tools/emulation/sdl3-acquire-01.json` and `sdl3-wasm-01.json/.log`. Locally measured archive digest for the installed `libSDL3.a`: `1834dd5e18aec45ac20fd443dc010f6f63039779d27fb3a0d9517531473d2d2f`.

`tools/fixtures/sdl_wasm_check.c` freshly linked and ran under the SDK Node. It checks header/library version agreement, a real SDL worker with mutex/semaphore/join and known signed-16 stereo to float mono PCM conversion. This is an ABI/data check, not audible output or original DSOUND behavior. Evidence: `.tools/emulation/sdl3-check-01/check.log`.

The actual browser test then exposed a failure that compilation and the Node SDL test missed: the audio callback threw in the SDK's legacy dynamic-call path. Explicit `-sDYNCALLS=1` alone still failed. Pinned SDK `src/library.js` normalizes pointer signature aliases for memory64 but leaves them unresolved in this wasm32 legacy path; SDL uses such signatures. `tools/xemu/wasm32-dyncall.js` supplies the wasm32 pointer-to-integer signature mapping through the SDK's supported library mechanism. It requires dynamic-call exports and rejects memory64. No guest instructions, kernel functions or device behavior are replaced.

`tools/fixtures/dyncall_wasm_check.c` reproduces the baseline failure (Node exit 7) and passes with the adapter (exit 0), covering pointer argument/return, void callback, uint64 and double cases. The prior libffi signed32/uint64/double check also passes with this adapter. Evidence: `.tools/emulation/dyncall-check-01/result.json` and `check.log`; the baseline failure is expected evidence, not a passing unmodified runtime.

The browser fixture (`tools/fixtures/sdl_browser_check.c/.html`) uses SDL's browser main callbacks, requests an actual ES3/WebGL2 context and produces only synthetic pixels and silence. Its first two failed builds are preserved. Build `sdl-browser-03` passes real browser readback of the expected RGBA color, a Space event, and advancing silent audio callbacks. The saved screenshot shows 54 frames / 928 audio callbacks / one Space press; a later DOM snapshot records 122 / 1901 / one. These are separate observations, not a frame-rate or target-device benchmark. The current-build console had no warnings/errors; Stop reached `Stopped` and reported clean shutdown without new errors. No physical gamepad was connected (reported count 0); audible quality, four virtual controllers and full xemu threading/context integration are unverified.

Local browser evidence: `research/verification/sdl-browser-check-20261031-03.jpg` (SHA-256 `28e700cbc60e98e8ee8a2bb0e172dee9dd169ed10e4d61d0c8be2d5be90f856c`) and matching `.json`; source identities are in `.tools/emulation/sdl-browser-03/source-sha256.json`. None contains original game data, and all stay ignored. The temporary check servers were stopped and the temporary tab closed after validation.

`tools/serve-sdl-check.mjs` serves only four named generated fixture outputs on loopback with the headers required for browser threads. `npm run test:runtime` checks those routes, content types, HEAD, rejection of other files/queries/traversal/uploads, and unchanged private synthetic data. It initially hit the local connection restriction (`EACCES`); the same test passed with loopback access. CI runs this source-owned delivery check without downloading an emulator or supplying game files. This command is not an emulator execution test.

After preparing a new output directory in a shell configured for the pinned Emscripten SDK and installed SDL prefix, the actual browser fixture link flags are:

```bash
emcc -O2 -pthread -Wall -Wextra -Werror -I.tools/emulation/wasm-deps/include \
  tools/fixtures/sdl_browser_check.c -L.tools/emulation/wasm-deps/lib -lSDL3 -lm \
  -sPTHREAD_POOL_SIZE=2 -sEXIT_RUNTIME=0 -sINITIAL_MEMORY=64MB \
  -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sWASM_BIGINT -sASYNCIFY=1 \
  -sALLOW_TABLE_GROWTH -sDYNCALLS=1 --js-library tools/xemu/wasm32-dyncall.js \
  -sEXPORTED_RUNTIME_METHODS=callMain -o .tools/emulation/sdl-browser-new/check.js
```

Use a new unused output directory, copy the source-owned fixture HTML there as `index.html`, then serve it with `node tools/serve-sdl-check.mjs --attempt new --port 4177`. The compiler command does not acquire/build SDL or game files. Exact executed commands for the saved run are in `.tools/emulation/sdl-browser-03/build.log`; older outputs must be preserved.

The actual xemu `browser-sdl-02` attempt found SDL3 3.4.18 and stopped at epoxy (`meson.build:1888` in the isolated source). The subsequent `browser-abi-03` attempt accepted the adapter in the C/C++ Emscripten machine flags and reached the same dependency failure. Both are terminal configuration failures; no xemu executable has linked and no original instructions ran. Source inspection also shows [the Vulkan backend calling the same geometry generator](https://github.com/xemu-project/xemu/blob/dc10b4cea96e403d30c17ed53551fdaf023bc17f/hw/xbox/nv2a/pgraph/vk/shaders.c#L267). Neither installing a desktop GL dispatcher nor selecting Vulkan supplies the missing browser GPU behavior.

The next task's native-reference environment was checked independently: existing WSL EGL 1.5 / OpenGL 4.5 core / GLSL 4.50 successfully compiled and drew a source-owned GLSL-400 vertex/geometry/fragment program. Its center pixel was exactly `[64,128,191,255]`, with GL error 0. This uses llvmpipe software rendering; the runtime warned that DRI3 acceleration was unavailable. It is a correctness-reference capability check, not a speed benchmark or an NV2A shader result. No installation, global graphics change, game or firmware input occurred. The cache/evidence remain under `.tools/emulation/egl-reference-01-cache`, `probe-egl-reference-01.py` and `egl-reference-01.json` (report SHA-256 `ac0aa03e55142dce7a335e712d6a6ac338384a21b1a744531bcbacdcebf42eaf`); preserve these rather than rerunning the same exclusive-output script.

The subsequent [filled-triangle component checkpoint](NV2A_GEOMETRY_CHECKPOINT.md) now executes the actual pinned generator and compares its native shader output with a Wasm CPU prepass and WebGL2 feedback. All 960 cases / 146,880 values match exactly under the measured host arithmetic profile. The fused profile is independently checked against native libm. The initial 11-ULP discrepancy, diagnostic probes and all earlier outputs are retained; no tolerance hides it. This covers a geometry-stage candidate only, with no original game execution or complete renderer integration.

At that checkpoint, the next task was line/quad output and assembly. Its native reference is now captured; browser expansion is pending after the native-route steering above. Other primitives, original vertex/pixel shaders, the GL/thread host port and performance remain unproved. Keep the full original execution/online/social target; no Xbox firmware may be acquired as a shortcut and no partial synthetic drawing is a substitute for title proof.

Publication/checkpoint: `6ae0f9b` contains reviewed source/docs only. [CI run 41263236493](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41263236493) completed successfully on Windows and Linux, including the seven XBE/CLI fixture cases and Python build-probe compilation. Existing prototype tests/build remain historical engineering checks. All 136 original file identities reproduced the existing manifest byte-identically after this task. The full goal remains active and incomplete.

Dependency/ABI follow-up `01218eb` is also published source-only. [CI run 41266020534](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41266020534) passed both Windows and Linux, including formats, Python compilation and the prior prototype checks. The partial browser-host profile described above is subsequent work; these runs do not validate that profile or establish original-game execution.

Browser-host source checkpoint `f718ded` is published source-only. [CI run 41266781332](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41266781332) passed Windows and Linux publication, format, Python compilation and prior prototype checks. The local Wasm adapter checks and terminal configure failure are recorded above; CI does not build/download the local emulator toolchain or execute the original game. No source changed after that run; this follow-up records its result. The full product goal remains active and incomplete.

SDL/ABI checkpoint `fac9615` is published source-only. [CI run 41269459896](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41269459896) passed Windows and Linux, including the new synthetic runtime-delivery check, formats, Python compilation and preserved prototype checks. The actual SDL/browser/callback/native-reference checks are local evidence, outside that CI scope. This documentation follow-up adds the terminal CI/reference result without changing checked implementation. Original-game execution and the complete product remain unachieved.

Triangle geometry checkpoint `402fa29` is published source-only. [CI run 41273163025](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41273163025) passed Windows/Linux checks and the new Linux native geometry compile/run. The 960-case actual-shader/Wasm/WebGL2 comparison and fused-profile checks are retained local evidence, not CI title execution. Source publication guards pass for 104 files; original media and generated/vendor artifacts remain excluded. The supplied XBE digest is unchanged. This follow-up records the completed CI result without changing checked implementation; the full goal remains active and incomplete.

Native-web feasibility checkpoint `684a665` is published source-only. [CI run 41304728962](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41304728962) passed Windows/Linux checks, including syntax compilation of the new probe, formats, runtime delivery and preserved prototype tests/build; the Linux triangle check also passed. Actual owned-function translation/native/Wasm/browser comparisons remain separate local evidence. No-profile generation reproduces the tested C without title metadata. Original game execution, browser-local title conversion and all full-product gates remain incomplete. This follow-up records the result without changing checked implementation.
