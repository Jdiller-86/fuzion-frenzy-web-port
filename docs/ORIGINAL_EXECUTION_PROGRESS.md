# Original execution: implementation ledger and build evidence

Current full objective: implement `DEVELOPMENT_PLAN.md` with the actual original game executing entirely inside a desktop browser, plus its planned browser/online/social features. The user's exact-original requirement supersedes reconstruction/approximation allowances. None of the gates below is satisfied by the previous Twisted System remake. This ledger preserves the full objective; completing a build spike is not completion of the product.

## Product acceptance ledger

| Requirement retained from the plan and user clarification | Evidence required | Current state |
| --- | --- | --- |
| Original instructions executing entirely in browser | Actual compatible XBE executes in a normal browser with original controllable screen; no streamed/native-helper game | Not implemented. Runtime cross-build in progress. |
| Every original detail, catalog, characters, arenas and modes | Matching-build menus, instruction/practice flows, original rules/AI, animations/camera, audio/video, save behavior, round/tournament cases and parity inventory | Not established. Prior footage/static inventory is reference evidence only. |
| Player-supplied local content | Bounded worker reads, compatible-build checks, no original uploads/serving/distribution, clear/reselect and save/storage recovery | Prior importer proves one asset profile. New original-code runtime still needs local block/file devices and bounded storage. |
| Keyboard/gamepad, remapping and owned seats | Original runtime receives correct virtual controllers for 1–4 seats; focus/hotplug/calibration and physical-device tests | Prior input adapters/tests can be reused; not integrated with an original-game runtime. |
| Offline solo/local and useful CPU fill | Complete original solo/local/practice/tournament behavior and original CPU difficulty | Not achieved by the approximate prototype. Use original execution rather than new guessed bots. |
| Online and mixed local/remote play | Four-browser and composite owned-seat cases; timing/contact/steering/orb cases under plan's latency/loss profile | Unimplemented. Requires a runtime/state/input protocol and authority decision; a relay alone would not satisfy server-owned outcomes. |
| Lobbies and recovery | Discovery/privacy, codes/invites, content readiness, owner transfer, rematch, reconnect and separate match-process recovery | Unimplemented. Do not conflate owner transfer with game authority or machine recovery. |
| Accounts and social | Recovery, friends/requests/invites, privacy/block/report, direct API authorization and isolation tests | Unimplemented; no provider/service purchase or secrets introduced. |
| Tournament persistence and authority | Atomic checkpoints/results, idempotent commits, server-validated outcomes and recovery | Unimplemented. Original execution changes how the earlier shared-TypeScript simulation assumption could be met. |
| Performance/browser/release matrix | Measured Chrome/Edge/Firefox/Safari, physical controllers, GPU/audio recovery, 60/30 fps targets, importer working-memory and long-tournament soak tests | No original runtime measurement. A compiler check or old remake's browser run cannot establish it. |
| Versioning, delivery and operations | Protocol/content/rules compatibility, source/asset boundary, deploy/rollback/backup/load/admission checks and budget-approved services | Source-only guards/CI exist; production delivery/operations remain unimplemented and separately gated. |

Material architecture issue reported before affected network implementation: running the original XBE does not automatically provide the planned authoritative shared-TypeScript server. Machine RAM/save states may contain original code/assets and must not be uploaded as a shortcut. An input-only relay would not establish server-owned scoring/collisions or crash recovery. Preserve those requirements and resolve the execution/authority/content boundary before implementing online behavior; do not silently relax them.

## Bounded source comparison

The primary candidate is [xemu at `dc10b4c`](https://github.com/xemu-project/xemu/tree/dc10b4cea96e403d30c17ed53551fdaf023bc17f), because it implements the Xbox hardware and has title-compatibility evidence. Its documented native operation is not proof of browser compatibility. No native game/emulator streaming is proposed.

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

The output filename and build label must be unused. The probe's build dependencies are not installed by `npm ci`. It intentionally cannot complete until the required Wasm cross libraries are supplied.

Follow-on dependency results: libffi 3.4.7, GLib 2.84.0 and Pixman 0.44.2 now cross-build/install in the isolated prefix. GLib's archive matched the [published SHA-256](https://download.gnome.org/sources/glib/2.84/glib-2.84.0.sha256sum); its PCRE2 source and Meson patch matched the wrap hashes in that verified source. `tools/fixtures/ffi_wasm_check.c` was freshly linked and its Wasm output passed signed32, uint64 and double foreign-call checks under the SDK Node. This validates those host ABI calls, not full TCG or game execution. Pixman compiled with unsupported floating-point-exception warnings; no render/numerical acceptance is claimed, and the current xemu profile skips Pixman. Local evidence: `libffi-wasm.json`, `ffi-check/check.log`, `glib-wasm.json` and `pixman-wasm.json` under `.tools/emulation`. GLib's generated config follows the pinned recipe's host-platform fixes for unsupported POSIX spawn/pthread-name probes; the original game kernel is not being stubbed by these host-library fixes.

The corrected `runtime-02` xemu probe found pkg-config, GLib 2.84.0 and zlib 1.3.1, then failed on the missing slirp subproject. The subsequent `offline-03` probe disabled guest NAT (`--disable-slirp`) and advanced to an unconditional libcurl subproject request at `meson.build:1473`. This is offline bring-up only; it neither implements nor removes the planned browser match transport. Stock Meson requests the curl fallback whenever curl is absent, even though it exposes a curl feature option. Do not blindly repeat the build with `--disable-curl` and assume that will remove the native frontend dependency. The current helper's reports preserve both real failures, and no QEMU executable has been produced.

## Partial browser host profile

The first host-service slice is implemented through `tools/probe-browser-build.py --browser-host`. It creates a new detached worktree at the pinned xemu commit, retaining the original checkout and every earlier attempt. Three guarded Meson edits skip native curl selection on Emscripten and select two source-owned host adapters. The source-owned adapter files are copied exclusively; their hashes and the three changed file hashes are retained in the attempt report. No Xbox CPU/device/render code is edited. The key map is copied from the existing pinned public source without initializing firmware submodules. This is a partial host port, not a complete browser frontend, browser network security boundary or game runtime.

Reason for explicit exclusion: upstream `ui/xui/reporting.cc` base64-encodes all XBE headers in `SetXbeData`, includes that field in its serialized compatibility report, then exposes `http_post_json` in `Send`. The native GitHub report builder also depends on curl. These are source observations at the pinned commit, not an observed upload. No original files have been loaded into xemu. The browser profile makes native HTTP calls fail through the ordinary QEMU error contract and removes the native GitHub-report menu hook; it does not replace guest kernel/device behavior with success stubs or change the planned match transport.

Both new adapters compiled with Emscripten `-Wall -Wextra -Werror` against pinned xemu headers. QAPI headers were generated locally by that source's own generator. A fresh Wasm link of `tools/fixtures/browser_http_check.c` passed under the SDK Node: initialization/GET/POST fail, errors are reported, supplied data and progress counters remain unchanged, no transfer callback runs, and null optional arguments are accepted. Only the error allocator is replaced in this isolated fixture. This validates the host adapter contract; the entire runtime has not linked, executed or been tested in a browser. Local evidence: `.tools/emulation/browser-http-check-01/check.log` and `result.json`.

Reproduce the partial profile with a new, unused attempt label:

```powershell
wsl -d Ubuntu -- python3 /mnt/c/Codex/Projects/FUZION~1/tools/probe-browser-build.py --tool-root /mnt/c/Codex/Projects/FUZION~1/.tools/emulation --attempt new-browser-label --browser-host
```

The first isolated attempt, `browser-host-01`, passed the earlier curl fallback and stopped at the SDL3 dependency (`meson.build:1700` in the isolated source): no cross SDL3 was installed and automatic wrap downloads were disabled. This is a real subsequent configure failure, not a completed runtime build. See `.tools/emulation/xemu-wasm-browser-host-01.json`, `.log` and its Meson log. Reusing the attempt label was rejected with exit 2 and the saved report remained byte-identical. The original upstream checkout remains clean; the browser source worktree and generated artifacts stay ignored. Building more generic libraries or disabling a required rendering path is not a substitute for a controllable original-game browser screen.

Next concrete work: cross-build the SDL3 commit already pinned by xemu's wrap (`829a65d769d935c4852f8159e964312c0957260a`), preserving its real Emscripten audio/input/video paths; run a new profile attempt and address the actual next dependency failure. Do not fake an OpenGL 4 context or remove geometry-shader behavior to make configuration pass. A browser graphics port remains substantive required work. Determine the boot-input path before a title boot attempt: availability of the user's own MCPX/BIOS files was asked but not yet established; no files may be acquired as a shortcut. Boot, full compatibility/performance and every planned additional feature remain unproved.

Publication/checkpoint: `6ae0f9b` contains reviewed source/docs only. [CI run 41263236493](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41263236493) completed successfully on Windows and Linux, including the seven XBE/CLI fixture cases and Python build-probe compilation. Existing prototype tests/build remain historical engineering checks. All 136 original file identities reproduced the existing manifest byte-identically after this task. The full goal remains active and incomplete.

Dependency/ABI follow-up `01218eb` is also published source-only. [CI run 41266020534](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41266020534) passed both Windows and Linux, including formats, Python compilation and the prior prototype checks. The partial browser-host profile described above is subsequent work; these runs do not validate that profile or establish original-game execution.
