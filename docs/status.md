# Project status: startup creates a named event

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. With explicitly selected synthetic named-event creation, original startup reaches **status 9 at the missing thread start routine `0x0004CBE0` during ordinal 255**, return address `0x000343DE`. It has reached 434 distinct routines and completed 641 kernel operations. `NtCreateEvent` completes the observed named, initially nonsignaled synchronization-event request and returns opaque handle `0x00090004`. The subsequent thread request refuses before allocation or a successful service return. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

M1 remains unaccepted. A controllable original screen, audible original gameplay, recorded player input and three consecutive menu/minigame/results/menu sequences still require evidence. Earlier checkpoints remain in Git history; this page owns the current boundary.

## Matching inputs and preparation

The local 2,834,432-byte XBE matches [the reference identity](reference.md), SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The retained Ghidra project is read without saving analysis changes. Pinned xboxrecomp source `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6 and Emscripten 3.1.50 remain available. Compiler binary digests match the recorded toolchain; SDK revision is `e2627e265d940db5ea58dffa63e490375bfc92e5`. See [analysis setup](analysis.md).

| Admission | Seeds + checked views / instructions | Module SHA-256 prefix | Observed next stop |
| --- | --- | --- | --- |
| Prior mutant creation checkpoint | 466 + 3 / 21,945 | `9a88a183` | Untranslated `0x00030716` |
| `CreateEventA`, `0x00030716` | 467 + 3 / 21,982 | `e00dab72` | Untranslated `0x000311D1` |
| `XapiFormatObjectAttributes`, `0x000311D1` | 468 + 3 / 21,993 | `9fc1057c` | Unsupported ordinal 189 |
| Named-event model and accurate callback refusal | 468 + 3 / 21,993 | `9a4c0423` | Missing thread start `0x0004CBE0` |

The new routines retain 37 instructions/97 bytes and 11 instructions/40 bytes respectively. Preparation verifies original-byte identity, independent decoder agreement and complete layout. Every prior admitted body, three checked direct views, timestamp guard and decoder normalization remains unchanged. Independent Sol High review checked partial-register/flag snapshots, signed status branches, checked descriptor stores, ESI preservation and stack cleanup (`RET 16` and `RET 12`). Original title execution occurs only in Chrome; private preparation does not execute it natively.

The 31 seed exports and limits of 512 functions, 32,768 instructions, 32 exports and 32 functions per export remain unchanged. Pinned-compiler rebuilds reproduce the final 1,161,355-byte ABI 25 module, SHA-256 `9a4c0423e7fb997989ecf166815751a81fa17a4ea8bd1566864ab9b69ee43aee`. Original input, generated title code, exports and raw reports remain in ignored local research.

## Bounded named-event creation

`CreateEventA` receives a nonzero name pointer and follows its original named-object path. The helper calls the already supported `RtlInitAnsiString` (ordinal 289), producing an 11-byte counted name with maximum length 12. It writes the original 12-byte object-attributes layout: root `0xFFFFFFFC`, a pointer to that descriptor, and attributes `0x80`. The event call carries four words: output-handle address, object-attributes address, event type 1 and initial state 0. The original name payload stays private.

The pinned declaration and original caller establish stdcall, 16 argument bytes and NTSTATUS return. The optional handler records those four words and supplies EAX zero/ESP +20 on the observed fresh creation. Original `CreateEventA` returns handle `0x00090004`; one object and one handle remain in the selected model. Reports retain checked name/attribute addresses and before/after ABI words without exposing name payloads.

The strict default-off profile is `events: {kind: "synthetic-win32-named-events", synthetic: true, capacity: 4}`. It admits only root `0xFFFFFFFC`, attributes `0x80`, type 1 and full-word zero initial state. It copies a counted printable-ASCII name of 1–63 bytes, with length no greater than maximum length and maximum at most 64. It reads only the counted payload, without requiring a terminator. Entire output, attributes, descriptor and payload ranges are checked before mutation; output/input aliases, malformed table state and capacity exhaustion refuse atomically at the component boundary.

The model separates stable one-based objects from monotonic opaque handles. An ASCII case-insensitive existing-name open allocates another handle to the retained object and returns informational status `0x40000000`; name and initial signal state remain unchanged. Native/compiled source-owned fixtures cover this existing-name case; the current original browser observation is a fresh creation. Up to four handles and four objects survive until whole-proof reset. There is no signal, reset, wait, close, reuse, native event or complete kernel-object layout. Other roots/flags, anonymous names, other event types and nonzero initial states remain guarded. Handles occupy a separate namespace from existing storage and mutant handles.

Activation is pre-start only. Missing capability rejects before execution, including old ABI modules. Strict state and call validation checks object/handle identity and order, unsigned widths, counted bounds, successful versus informational status, final refusal and capture-window omissions. The unselected service still reports its static ABI with `modeled: false`; earlier unidentified one-word refusal reports retain narrowly checked compatibility. Static metadata alone implements no service.

Pinned Cxbx reference evidence identifies root -4 as the Win32 named-object directory, distinct from DOS-device root -3, and `0x80` as open-if-present. Its object manager distinguishes fresh creation from opening an existing same-type object. These are replacement-kernel reference facts, not original-console observations; see [research provenance](research.md#named-event-request-and-namespace-reference). Preserve the original name and namespace.

## Accurate thread-callback refusal

The next ordinal-255 request supplies start routine `0x0004CBE0` and the already admitted system routine `0x00034307`. The existing preflight incorrectly reported the system argument whenever either lookup failed and left its capture row marked successful. Split checks now identify the first missing callback and mark that row failed before allocation. This corrects the superseded experimental target `0x00034307` and completion count 642 to the actual missing start and 641 completions. The complete CPU trace and every other observation remain unchanged by this diagnostic correction; the full capture digest changes with the corrected failed marker.

Source-owned compiled fixtures cover missing start, missing system, both missing and admitted success, including no allocator call or CPU return on refusal. Existing admitted first-thread creation, control, inert and default-off paths remain unchanged. No additional original callback, thread scheduling or native thread behavior has been supplied.

## Exact comparisons and explicit clocks

The deterministic profile supplies synthetic regression inputs: one 100 ns tick per sample plus a 5,000,000-tick jump before sample 4,183, fixed system time and paired 733,333,333 Hz performance-clock services. It uses no live wait and measures neither elapsed time nor original-console timing. The former stack/return-address fault remains repaired.

The compiled but unselected event capability reproduces the complete prior refusal report apart from module identity. Selecting it preserves all 7,919 prior recorded CPU events, original-entry prefix and 129 preceding call objects/outputs literally. It changes the final event refusal into a successful creation, then adds the refused thread request. Final deterministic evidence has 7,923 traces and 3,984 original-entry events. The complete final report repeats byte-for-byte using the rebuilt identical module. No CPU value normalization is used.

Separate live-clock evidence reaches the same callback boundary and routine/kernel counts, with 7,915 traces and 3,980 entry events. The requested 500 ms pause before sample 4,183 measures 511.3549 ms in that run. Live calibration varies numeric values and carry branches; full live CPU identity is not claimed. Reports retain the synthetic fixed system time and explicit performance-clock model even in this live-monotonic condition.

A verified import insertion adds only slot `0x00253384` → ordinal 189 at index five. All 56 previous slot/ordinal pairs remain; 51 later synthetic tokens shift by four bytes. Ordinal 192 moves from `0xFE000018` to `0xFE00001C`, and inert ordinal 100 from `0xFE0000CC` to `0xFE0000D0`. The patcher and lookup use the same table. The explicit inert comparison accepts only that target relocation; previous experiment CPU fields and service outputs remain literal, without token-value normalization. The second admission changes no binding. Fixed control and stable inert trace/call projections remain unchanged.

The final capture contains 131 suffix rows, 643 total events and 641 completions. The first-512 digest remains `14fcb4cd259e4482e53b54e82a3bc6f08f13631a45e23f1563cc40ab4e32a785`. The full capture digest remains runtime-reported because some older serializers omit other services' argument tails. The four-word event display and output ABI evidence do not make those older tails reconstructable.

## Retained modeled state and limits

The existing default-off mutant profile supports only null attributes and full-word zero initial ownership, with checked writable output and up to four opaque records. Its one live unowned handle remains `0x00080004`. It supplies no named/owned mutant, acquisition, wait, release, close, reuse or native mutex. Activation is pre-start only; missing capability rejects before execution. The earlier full default-off comparison remains retained.

Disk/FATX, debug and aggregate ABI observations remain unchanged: EOF 265 in one 16,384-byte cluster, revision 85, opened/closed eight, no live FATX file handle, 65,517 free clusters and eight disk pages; six debug requests, last length 54. Aggregate ABI diagnostics remain 453. Legacy getter slots 3–5 count stack inequality (451), despite frontend ESI labels; unused zero slots 6–8 do not prove zero ESP faults. Individual nonvolatile-register attribution remains unresolved. Recorded CPU fields cover EAX, FS and exception-chain head plus call address/stack, not complete register snapshots.

Keep 64 timestamp samples, 64 successful counter queries, 512 kernel rows, 1,024 total kernel events, 8,192 trace rows and fixed 128 MiB Wasm memory. Its protected guest window remains 64 MiB. No resource cap was raised.

## Validation and next dependency

Independent Sol High review covers the creation component, compiled bridge, strict reporting and callback-diagnostic correction, plus default-off/on, rebuild, repeat and live comparisons. Actual session metadata confirms three fresh Luna Low workers. Runtime checks pass 279/279, native component runs 68/68 including sanitizers, and Python evidence suites 13/13. Native/compiled fixtures contain independently authored CPU/RAM and services, not original title execution. Preparation/publication and documentation checks pass; the source-only build contains 17 allowlisted files. Local Node 24.22.1/npm 11.19.1 differ from pinned CI Node 24.14.0/npm 11.9.0; CI remains pending.

The next task is to inspect and establish the exact reached indirect start `0x0004CBE0` before bounded admission. It is absent from the generated function table; the retained read-only Ghidra exact-entry export reports `established: false`. Do not substitute the already admitted system routine, invent a function boundary or widen lookup admission. Establish byte/layout/decoder evidence for the actual entry, then independently review any translation and thread-memory/ABI work under the existing limits. Original graphics, audio, recorded player input and three complete original round sequences remain unverified.
