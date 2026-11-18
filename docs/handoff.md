# Current development handoff

## Current status

- **Windows:** builder 0.2.1 adds a clickable in-game PC menu (Settings / Exit to Desktop); rebuild the game to include it. Builder 0.2.0 was published with the Control Room launcher, native F1 settings, three player-local UI replacements and rendering fixes. Working gameplay and geometry stability are reported by the user/local agent.
- **Browser.** Newly linked shaders are saved during play. An Erodeo page reload reused all 12 live-link recipes, with zero blocking validations. The Water Gate Scramble replay with allocation-free single-object waits reached gameplay with a 50.21 ms scene-entry peak and 17.02 ms steady acknowledgement p95. [Gameplay observations](browser-gpu.md#shader-recipe-reuse-after-reload)
- **Next.** The latest peak sampled 27.84 ms in the shared guest loader. Check whether its parser can reuse classification across both passes. Measure physical presentation on modest hardware before testing 120 FPS. [Scene-entry observations](browser-gpu.md#remaining-work-toward-modest-pc-60-fps)
- **Later:** complete offline coverage/conversion, online multiplayer, accounts/friends/lobbies and release qualification. No product milestone is accepted; M44 remains the separate startup reference.

[Downloads](../README.md#available-now) · [Browser guide](sdk-browser.md) · [Windows guide](../desktop/windows/README.md) · [Remaining browser work](m1-browser-work.md).

<!-- development-state:start -->
Recorded development state: **M1 / awaiting-evidence**, profile **44**. Retained startup resume task: **SCOPE-37**.

The selected C allocation wrapper and SDK allocator return; startup reaches the existing logger provider's new-caller guard.

[Current runtime evidence](status.md) owns detailed observations and limits. [Structured state](../profiles/development-state.json) owns this generated pointer; [workflow](development-workflow.md) explains preflight and updates. [Goal G](local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) owns the SDK integration assignment; the retained startup stop is a reference boundary. Source-only work can proceed without private inputs; original execution requires the matching local baseline. No milestone is accepted by this record.
<!-- development-state:end -->

For Goal G and #49, use the aligned cold captures to isolate remaining scene-entry work. The largest Twisted System gap begins before submission and includes about 37 ms of active guest work, including floating-point scene calculations and parsing. Erodeo's last measured longest gap included its first expensive WebGL submission. The controlled original comparison with recorded input and audible audio remains open. [Goal G](local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05)

Follow [status](status.md), the [scope ledger](m1-work-scope.md) and [SDK workflow](resource-reuse.md). Keep M44 unchanged. Give the separate SDK candidate explicit resource/import/export/dispatch limits and retain basic bounds, identity and private-content checks. Detailed contract certification and independent correctness review follow the working baseline, before accepted-checkpoint promotion. M1 remains incomplete; original/generated content stays local.

The [largest-first M1 order](m1-browser-work.md#largest-first-implementation-order) begins with SDK integration and the minimum graphics connection for the first frame. SCOPE-37 below records the retained startup resume boundary and is conditional on the SDK experiment needing that exact capability. Batch supported shared-interface fixes and report a runnable result or actual compiler/browser failure plus named missing adapters; routine counts and general plans do not satisfy this assignment.

## Current boundary

The [SDK browser GPU candidate](browser-gpu.md) links 92 source objects with the full SDK/title, local ISO intake and existing input/audio/save adapters. Actual local execution reached original menus, character selection, Tank Tracks gameplay and earlier Bombstruck results. WebGL2 executes programmable vertices and pixel combiners with bounded owned command batches. The texture-cascade fix remains applied; D3D12 descriptor management stays native-only. CPU assembly/submission costs still limit dense gameplay. Keep the separate M44 checkpoint unchanged; no complete controlled-round or product milestone acceptance is claimed.

Local Goal A continuation (2026-11-09): PR #46 at `b2c4d1e` is loaded with all five pre-existing worker-cap edits preserved. The newer November8 bundle is preparation only. M44 below remains the accepted checkpoint; forecast16/28/49 is unchanged. The mutable Ghidra database has no trusted pre-incident identity, but neither unchanged replay nor the covered retained-code candidate consumes that database. Pinned original bytes, retained seeds/listings and audited source/build evidence support the exact caller and literal. Do not invent a historical database checksum or infer new facts from it.

Explicit replay preflight and separately reviewed consumed-input closure pass. Fresh M44 default43 and selected44 reports reproduce byte-for-byte; live-clock control/inert retain their bounded comparisons with recorded differences. R2's strict ABI-counter guard failed and remains retained. Fresh R3 adds exact per-helper witnesses without changing original bodies/counters:22 records account for all18 intentional diagnostic increments, with complete C/SDK return, represented FP/context and full modeled file-prefix/43-byte CRLF append checks. Independent source/build/module/harness/actual review passes; full default44 and prior3,152 CPU/385 kernel rows remain exact. Module SHA `af3eb1d6d90768bc94e9116bc4d031755e0b22220a7a17f30ffa0a82f53bfde9`; actual audit `a0f254bfd48e24e3c813b1f2a710c49d75749c3b2ad532b1ec59f3fb54c928ae`. Build50.697s; whole browser harness3.626s. This local candidate leaves the accepted M44 checkpoint unchanged. [Status](status.md#local-goal-a-readiness-2026-11-09) owns identities/timings/limits; [research](research.md#private-baseline-recovery-and-m45-preparation) distinguishes provenance from runtime proof.

M44 completes original C4C480 using the existing raw SDK allocator on the same active context. Actual allocation header6F4AA0/extent40030 backs payload6F4AD0 for40000bytes (256KiB). Allocator RET reachesESP03C10E38/RA4C4A8; original C ADD18/RET reachesESP03C10E54/RA4F68E. Whole-span/nonalias/ownership and in-module complete payload/image comparisons pass; the payload snapshot is after allocator RET. C/SDK nonvolatile/FP/SEH/flag/context checks pass. Actual selector0 yields fifth allocator wordDEFA, not an ambient pointer.

Accepted M44 stop:status2 target=ESP03C10E5C/RA4F6C2, one incoming word25C89C. The existing4B230 C-provider guard reports the stack address for the new4F6BD logger caller. Do not describe this M44 stop as a missing body or4EA30 stop. Extra words are ambient. The separate local R3 evidence above reaches the4EA30 guard; its body and outer4F5E0 return remain unexecuted/unaccepted.

Full default43 is exact except case/module identity; previous3137CPU/383kernel rows are exact prefixes. Latest3152CPU/385kernel rows,470distinct routines,897/892kernel record/raw-zero counts; wait/release7→8. Build02 module1,515,389B SHA49d9882661789b1ce92d8eaf0d0bd39f59d8fc2971beb197bc6e2ea4bd2df895;79reused9compiled. Same511/23388/32/last32/30maps/64bindings and128MiB/clock-only/186exports/no-start. Independent actual audit `725146787b3679d4ebec1e15529086bb6ff525375962e1623791898568492bdf` passes; [status](status.md) owns identities/limits. Private evidence is in `research/verification/sdk-4c480-20261108/`.

First candidate/run stopped at a missing new-bank prefix marker before wait/release models; the corrected entry/completion lifecycle passes. Preserve rejected source/build01/run01 and the summary-only timing-label erratum. Complete payload proof uses reviewed executed witnesses, not an external RAM dump. Six historical getter differences are live diagnostics; actual lock contents remain unchanged.

Forecast16/28/49, about15 completed packages and24 smaller checkpoints sinceE1 remain low confidence. Startup remains open; no extra whole package or original menu/audio/input round/M1 gate closes.

## Retained startup continuation when required

**SCOPE-37 continuation:** the exact logger caller is locally verified in R3. The candidate now stops at status2/0x4EA30, caller0x4F6C7/return0x4F6CC/ESP03C10E5C, with one explicit argument1. Assess that caller's retained byte/layout, register/format/resource and provider contract before any new admission. Its body and enclosing4F5E0 return remain unexecuted; preserve the guard, full default44 and all caps. Static availability does not admit it. Keep the accepted M44 identities and forecast unchanged unless separately promoted. For any required fact absent from retained evidence, use the recovery boundary in research; do not invent a Ghidra before-state or start another goal.
