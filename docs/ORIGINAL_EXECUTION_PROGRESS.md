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

Local report: `research/verification/original-execution-profile-20261031.json`. It stays ignored with the other analysis output. No disc extraction, executable reimport, full decompilation or firmware acquisition occurred.

Seven source-owned XBE fixture/CLI cases cover valid variants, mismatched keys, truncated/overlapping ranges, invalid import tags/termination, non-executable/BSS entry, TLS/BSS handling and library bounds. The CLI case also verifies outside-research rejection and exclusive report creation with source-owned temporary fixtures. They run within `npm run test:formats`; this is parser validation, not an execution test.

## Actual local cross-build attempt

Pinned tooling and locally measured compiler hashes are in `tools/original-execution-toolchain.json`. Sources, Linux Emscripten 3.1.50, its internal Node 16.20.0, Meson/Ninja and output are under ignored `.tools/emulation`. Project Node remains 24.14.0; no global PATH, Java, model, shell startup or system package settings changed. Only the pinned public keycodemapdb source was fetched for QEMU's subproject guard; firmware submodules were not initialized.

The repository path contains spaces. A temporary symlink did not fix Python venv's canonical path. The existing Windows short path `C:\Codex\Projects\FUZION~1` was verified to reach the same project and used through WSL, without moving files. On another machine, determine its actual short path rather than assuming this one exists.

Attempt 05 used Emscripten's actual wasm32 compiler, passed QEMU's Python setup and progressed through Meson C/C++ feature checks. It terminated at `meson.build:1084`: host-machine GLib lookup failed because the generated cross file selected nonexistent `empkg-config`. The Wasm GLib prefix was also empty. This is an observed configuration failure, not a linker/game/runtime failure. Logs: `.tools/emulation/xemu-wasm-attempt-05.log` and its `meson-logs/meson-log.txt`.

The new `tools/probe-browser-build.py` now selects the real pkg-config and restricts it to `.tools/emulation/wasm-deps`; it checks pinned source commits and refuses to overwrite attempts. It runs configure, then builds only if configure succeeds. An earlier validation run (`dependency-01`) was deliberately interrupted after identifying that its initial wrapper version inherited the same pkg-config selection; it is not reported as a completed probe. The corrected wrapper still needs a fresh run after preparing the cross libraries.

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

Next concrete work: build libffi 3.4.7 and GLib 2.84.0 (plus required cross dependencies) according to the pinned xemu Emscripten dependency recipe, using verified source archives and an isolated Wasm prefix; run a fresh pinned build probe to expose the next real failure. The [published GLib archive digest](https://download.gnome.org/sources/glib/2.84/glib-2.84.0.sha256sum) is available for verification. Then address the measured browser graphics gaps without replacing original geometry/rules. Boot inputs and playable-title correctness remain later unproved gates. Do not stop at declaring a source comparison, compiler install or metadata parser to be original-game execution.
