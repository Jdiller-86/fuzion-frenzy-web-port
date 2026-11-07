# Research facts and evidence limits

This page consolidates useful findings after the baseline cleanup. Source, tests, active toolchain pins, and reference-build identities remain in the repository. Obsolete per-routine checkpoints, package manifests, duplicate snapshots, and automation logs have been removed; their prior versions remain in Git history.

## What the evidence establishes

| Evidence | Establishes | Limit |
| --- | --- | --- |
| Static/export findings | Bounded instruction, layout, resource, or ABI facts | Not original execution or numerical gameplay behavior |
| Source-owned fixtures | The tested component contract | Not complete title or hardware compatibility |
| Synthetic browser profiles | Execution under declared modeled inputs | Not original-console defaults or full fidelity |
| Paired browser reports/audit | Their exact observed transition and limits | Not behavior beyond the recorded scenarios |
| Reference-file metadata | Identity of the inspected files | Not authenticity, ownership, or game compatibility |
| Prototype gameplay | Independently authored rules and input behavior | Not original-game parity |

Runtime acceptance retains named control/experiment browser reports plus an independent audit locally. Original bytes, generated modules, raw exports/reports, RAM, and original-containing save states are not distributed. Missing private artifacts do not become a public test pass.

## Original startup

The accepted main-branch frontier is recorded in [current status](status.md). The repaired return-address defect is a bounded legacy runtime M6 result; no controllable original screen or full runtime has been accepted. Retired branches contained further unmerged work, which is outside this baseline.

Preparation retains original-byte identity, independent decoder agreement, complete layout, checked-memory, and resource-admission guards. A translated function count is different from an observed routine count. The Python/native developer workflow does not complete the required browser-local end-user conversion.

## Startup observation limits

The kernel capture digest covers the recorder's argument count/words and status. It is computed by the bounded runtime; some older JSON serializers omit argument tails for modeled services, so their displayed call objects cannot independently reconstruct the full digest. The current generic unsupported recorder captures four diagnostic words, while a legacy frontend may display only one; original caller/stop evidence establishes the actual ABI separately. Keep these scopes explicit when comparing capture windows; matching runtime digests, serialized fields and source-owned hash fixtures establish different facts.

The legacy ABI diagnostic getter's slots 3-5 count stack inequality among logged calls, although the browser labels them as ESI. Slots 6-8 are unused zeros. Aggregate diagnostics therefore do not identify a particular changed nonvolatile register or establish absence of ESP faults. Call/return traces and static instruction review support their own bounded claims; preserve unresolved attribution rather than treating these labels as new CPU evidence.

### Explicit trace windows

The optional `traceCaptureStart` setting selects start 0 or 8,192. Storage remains 8,192 rows; selecting the second window explicitly extends the total trace horizon to 16,384 events. Unselected legacy behavior is preserved. Each completed sample hashes kind, original address, SP, EAX, FS and exception head as six little-endian DWORDs after the NUL-terminated `ff-original-trace-v1` domain. Paired deterministic windows can independently reconstruct the prefix and full digest. Global entry/routine counters differ from the captured-only entry sequence. An omitted prefix makes initializer completion unavailable; a separate complete first window can witness it. Live omitted prefixes and legacy kernel argument tails retain their stated reconstruction limits.

Selected sampling commits a complete CPU/head sample atomically. A failed sample cannot rotate or partially hash a window. The old begun-row counter still locks pre-start settings, including after a failed first sample; it is distinct from completed-sample metadata. These observation contracts do not add guest services or establish title fidelity.

### Returning debug traps and earlier frame defects

The modeled returning `INT 2D` exposed a truncated retained debug-wrapper body: it ended at the adjacent `INT3`, omitting the original `LEAVE; RET 4`. Temporary fallthrough metadata and automatic owner derivation in a read-only Ghidra session recover the four-byte tail. Original bytes/decoder boundaries, discard logging and a reopened unchanged retained project distinguish this derivation from a saved function definition. Preparation now rejects reachable falloff after the returning trap and its eligible adjacent breakpoint slide. This guard is bounded to those paths; it is not a general proof of every trap or return.

The corrected browser trace witnesses seven restored debug-wrapper/parent frames. Reports from the earlier missing-return module after its first debug-wrapper entry are forensic evidence. Subsequent independent review identifies an earlier 24-byte deficit in `0x001F5520` and a 52-byte deficit in parent `0x00202150`: the modeled ordinal-159 and ordinal-3 handlers omitted their 24/28-byte stdcall cleanup, including return slots. Correcting those reached success paths restores both observed frames. The first 4,814 events match the immediate debug-repaired baseline; its later damaged-frame observations remain forensic. Pinned replacement-kernel declarations and original call sites establish the ABI, not original device semantics or all-register fidelity.

The corrected display request passes MMIO register base `0xFD000000`, not a RAM descriptor, and format `0x1E`. Declaration acceptance requires the existing ready aperture at that exact base and checked containment, with no register access. The core pins base/step/mode/format/pitch and retains its bounded framebuffer range; reports pin the observed framebuffer. The old stack-pointer/format-zero tuple is rejected. Valid refusal metadata uses decision enums rather than tuple constants. Enabled but unreached AV/free profiles require zero observations; their zero decision does not claim a call succeeded. Observed-call checks preserve the startup-query router priority and include the corrected null-result tuple without changing runtime null-query semantics. The later helper now receives the original caller's `0x03100000` size, while that request remains guarded before its allocation service. Earlier successful allocation calls remain separate evidence. [Current status](status.md) owns exact module and stop evidence.

## Creation-only mutant models

The legacy explicit profile supports null-attributes, initially unowned creation with four opaque handles. The new alternative `synthetic-win32-mutants` profile retains anonymous and named creation/open-if in one four-handle/object pool. It requires the selected named-event peer, copies bounded counted ASCII names under root -4/flag `0x80`, and guards cross-type name collisions in both creation paths before mutation. Anonymous objects cannot be reopened; duplicate named opens preserve the object and return informational `0x40000000` with another handle. Guest ranges/alias checks, provider storage separation, canonical tables and pre-start peer locks are independently reviewed.

The original browser path observes fresh anonymous handle `0x80004` and named handle `0x80008`, with the 13-byte counted name retained locally and zero initial ownership. Three DWORD arguments yield NTSTATUS/EAX and ESP +16. The new profile intentionally uses `localMutantState` and richer anonymous-call metadata; common prior handle/EAX/ESP words and the entire old raw CPU prefix remain unchanged. Unselected capability reproduces the entire prior deterministic report except module identity. Source-owned fixtures establish duplicate opens, typed collisions, shared capacity and refusals; no retail-kernel equivalence, ownership/acquire/wait/release/close/native mutex or full object layout is claimed.

For this service, the old generic default-off recorder captures four diagnostic stack words and the old frontend displays one. Original call-site and pinned declaration evidence establish three actual arguments; selected handlers record those three. Keep that arity correction distinct from unreconstructable argument tails in older capture windows. Actual Wasm i32 status exports are signed JavaScript numbers; normalize only after integer/range validation, while serialized evidence remains unsigned-strict.

## Named event request and namespace reference

The reached original `CreateEventA` follows its nonzero-name path through `XapiFormatObjectAttributes`. Original bytes and checked browser execution agree on a 12-byte object-attributes layout: root, counted ANSI-name pointer and attribute word. The reached ordinal-189 request has type 1 and initial state zero. The pinned xboxrecomp declaration and original caller establish four 32-bit arguments, stdcall/16-byte cleanup and NTSTATUS return; metadata and four-word capture do not implement creation. Historical one-word diagnostic reports remain distinct from complete ABI evidence.

The pinned [Cxbx object-manager header](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/common/ob.h) identifies -4 as the Win32 named-object directory, -3 as the DOS-device directory and `0x80` as `OBJ_OPENIF`. Its [object insertion implementation](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlOb.cpp#L786) distinguishes a newly inserted object from a same-type object opened by name, returning a name-exists informational status for the latter. These are replacement-kernel reference semantics, not original-console observations. The retained xboxrecomp bridge ignores object attributes and documents only root -3, so it cannot establish the reached root -4 behavior by itself.

Preserve the original counted name and namespace; do not substitute anonymous creation or discard flags. The explicit synthetic creation model now keeps a separate four-object/four-handle table with copied ASCII counted names and case-insensitive identity. It distinguishes fresh zero status from existing-name informational `0x40000000`, checks full memory/alias ranges before mutation and supplies no signal, reset, wait, close or native event. Those later operations remain guarded. Original browser evidence currently observes fresh creation; existing-name behavior is covered by independently authored component/bridge fixtures. Original name bytes and raw reports remain private. Static bytes at the observed buffer address are not a historical browser RAM witness.

## Indirect thread-callback admission

Thread creation must refuse an unavailable start or system callback before allocation. The previous wrapper reported the system argument for either missing lookup and left that refusal row marked successful. Split checks now record the actual first missing callback and a failed capture marker, without changing guest return registers or allocating a thread. A compiled source-owned fixture independently covers missing start, missing system, both missing and admitted success. The corrected boundary distinguishes callback admission from kernel creation failure; it does not expand scheduling or establish a function boundary at an unadmitted indirect target.

## AV services and provenance

The source-owned service descriptor records ordinal 2 as `AvSendTVEncoderOption`, stdcall, 16 argument bytes, and void return. It is metadata rather than an implementation or service result. See [the descriptor](../runtime/browser/kernel-service.mjs).

A retained startup call used `(0, 6, 0, 0x203720)`: four arguments, a four-byte writable output, and a caller consuming that output rather than an NTSTATUS from EAX. These are call-specific facts, not a universal option-6 semantic contract.

Pinned replacement-kernel references disagree about numeric option 6. Cxbx treats it as a capabilities query; the retained xboxrecomp source labels a pack query and supplies a synthetic compatibility default. Neither is an original-console observation. The explicit, default-off raw-DWORD model must remain labeled as synthetic; do not infer original capabilities, timing, or mode from it. Pins and provenance limits remain in [original-execution-toolchain.json](../tools/original-execution-toolchain.json).

The current component/adapter, call records, and validators define supported modeled cases. Older “next task” or “unsupported stop” paragraphs do not supersede current source or [status](status.md).

## Derived indirect entries and guest-thread evidence

An observed thread request may publish an exact executable address without a retained Ghidra function. For `0x0004CBE0`, the existing decoded listing, an exact pointer reference in an admitted caller, non-fallthrough predecessor return, closed 43-byte flow and padding support automatic temporary derivation. The seed records newly established analysis; headless read-only discard output and reopening the retained project distinguish it from a persisted function. All 11 new instruction boundaries agree. The older aggregate decoder-boundary flag remains false for the explicitly retained FSTCW versus WAIT/FNSTCW normalization, with no new exception.

Admission permits the existing bounded service to create a second READY context; it does not prove the callback ran. Its 17-page arena footprint includes one object/PCR page and 16 stack pages with TLS inside. Pool-to-stack role reclassification changes modeled ownership, not VM reservation, commitment or free capacity. Existing source-owned multi-thread/TLS/capacity and page-accounting fixtures are now part of the standard native runner. Preserve pointer, context metadata, actual execution trace and scheduling acceptance as separate evidence.

The following named-mutant request is now served only by the explicit alternative creation model. Its Win32 root/open-if attributes, 13-byte counted name and zero initial owner remain preserved. Startup reaches the next untranslated routine; later synchronization and callback scheduling remain unaccepted. See [current status](status.md).

## Native recompilation reference and browser reuse

The public [Fuzion Frenzy native port](https://github.com/abilliontokens/fuzionfrenzy-recomp/tree/071dbd97bdc098c4647cc24c99f9eae693fde1eb) uses the separate XboxXBERecomp SDK. Its reported executable size, entry and SHA-1 `c6fb6082900532cde5fa8dbb7de8dfa202f0fdad` match the retained local input. Its maintainer records Linux menus and minigames, game-specific XDK 3925 findings and rendering fixes, while audio remains unverified and a rendering discrepancy remains open. These observations have not been independently reproduced here; older unreached summaries coexist with newer observations in its documentation.

Prioritize a bounded comparison of those curated function/layout findings and reusable runtime components before further bespoke startup services. The native project uses Vulkan/SDL3 or Direct3D 12/XAudio2; neither its native gameplay report nor its compiler output proves browser compatibility. Evaluate a small source-owned browser slice with explicit host, memory and backend limits before considering integration. Preserve static original execution, player-local content, current admission guards and browser-only runtime evidence; no route change or M1 acceptance follows from this reference.

## Conversion and geometry

[Reference inventory](reference.md) records the compatible executable and limited conversion coverage. Bounded character/arena/audio preview and NV2A native/Wasm/WebGL2 component comparisons support engineering work. They do not accept integrated original rendering, all primitives/state, depth/arithmetic fidelity, title performance, or full content conversion.

Host arithmetic profiles matter; a successful component comparison must retain its actual evaluation assumptions. Do not hide outcome-changing differences behind an unvalidated tolerance.

## Offline prototype evidence

The retained Twisted System app uses independently written rules and locally converted assets. Its source-owned tests cover scoring, ties, elimination, input ownership, focus/device safety, and delivery. See [rules](../web/prototype/twisted-system.mjs), [input](../runtime/browser/input), and [tests](../tests/twisted-system.test.mjs).

Public footage and static resource association support the jump/duck cooling-rod concept. Exact original hazard order/RNG, acceleration, hit geometry, recovery, action windows, held-duck behavior, simultaneous ties, and solo/practice termination remain unaccepted. Configurable simulation values, the chosen seven-miss threshold, bot policy, and procedural presentation are prototype choices or inferences rather than recovered original rules.

## Reproduce or extend evidence

Use [development](development.md) for public checks and [analysis setup](analysis.md) for optional private prerequisites. Reuse retained local projects where available, publish only curated findings, and report evidence type, source/commit, observed conditions, and limits. Resume implementation only when the next user objective is assigned.
