# Project status: named event creation is reached

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. With the existing explicit synthetic profiles, original startup reaches **status 1 at unsupported ordinal 189 (`NtCreateEvent`)**, dispatch token `0xFE000014`, return address `0x0003074D`. It has reached 434 distinct routines and completed 640 kernel operations. The original `CreateEventA` and named-object attribute helper now execute; no event is created or successful service return supplied. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

M1 remains unaccepted. A controllable original screen, audible original gameplay, recorded player input and three consecutive menu/minigame/results/menu sequences still require evidence. Earlier checkpoints remain in Git history; this page owns the current boundary.

## Matching inputs and preparation

The local 2,834,432-byte XBE matches [the reference identity](reference.md), SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The retained Ghidra project is read without saving analysis changes. Pinned xboxrecomp source `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6 and Emscripten 3.1.50 remain available. Compiler binary digests match the recorded toolchain; SDK revision is `e2627e265d940db5ea58dffa63e490375bfc92e5`. See [analysis setup](analysis.md).

| Admission | Seeds + checked views / instructions | Module SHA-256 prefix | Observed next stop |
| --- | --- | --- | --- |
| Prior mutant creation checkpoint | 466 + 3 / 21,945 | `9a88a183` | Untranslated `0x00030716` |
| `CreateEventA`, `0x00030716` | 467 + 3 / 21,982 | `e00dab72` | Untranslated `0x000311D1` |
| `XapiFormatObjectAttributes`, `0x000311D1` | 468 + 3 / 21,993 | `9fc1057c` | Unsupported ordinal 189 |

The new routines retain 37 instructions/97 bytes and 11 instructions/40 bytes respectively. Preparation verifies original-byte identity, independent decoder agreement and complete layout. Every prior admitted body, three checked direct views, timestamp guard and decoder normalization remains unchanged. Independent Sol High review checked partial-register/flag snapshots, signed status branches, checked descriptor stores, ESI preservation and stack cleanup (`RET 16` and `RET 12`). Original title execution occurs only in Chrome; private preparation does not execute it natively.

The 31 seed exports and limits of 512 functions, 32,768 instructions, 32 exports and 32 functions per export remain unchanged. Pinned-compiler rebuilds reproduce the final 1,157,991-byte ABI 25 module, SHA-256 `9fc1057ce06392dd061f2d045c408b5c60865ed62dca8eaac217cadeec02e9ea`. Original input, generated title code, exports and raw reports remain in ignored local research.

## Reached named request

`CreateEventA` receives a nonzero name pointer and follows its original named-object path. The helper calls the already supported `RtlInitAnsiString` (ordinal 289), producing an 11-byte counted name with maximum length 12. It writes the original 12-byte object-attributes layout: root `0xFFFFFFFC`, a pointer to that descriptor, and attributes `0x80`. The event call carries four words: output-handle address, object-attributes address, event type 1 and initial state 0. The original name payload stays private.

The pinned declaration and original caller establish stdcall, 16 argument bytes and NTSTATUS return. Static metadata now names the service while keeping `modeled: false`. New reports expose all four words already captured by the unchanged generic recorder. Earlier reports displayed one diagnostic argument; the validator retains that specific legacy unidentified-descriptor format and rejects inconsistent ABI metadata, success/output claims, repeated or nonfinal refusals, oversized words and disagreement with available stop-stack evidence.

Pinned Cxbx reference evidence identifies root -4 as the Win32 named-object directory, distinct from DOS-device root -3, and `0x80` as open-if-present. Its object manager distinguishes fresh creation from opening an existing same-type object. These are replacement-kernel reference facts, not original-console observations; see [research provenance](research.md#named-event-request-and-namespace-reference). No anonymous-event substitution, native event, name lookup, signal, wait, close or handle behavior has been supplied.

## Exact comparisons and explicit clocks

The deterministic profile supplies synthetic regression inputs: one 100 ns tick per sample plus a 5,000,000-tick jump before sample 4,183, fixed system time and paired 733,333,333 Hz performance-clock services. It uses no live wait and measures neither elapsed time nor original-console timing. The former stack/return-address fault remains repaired.

The first admission preserves all 7,916 prior recorded CPU events and all 128 previous call objects/outputs literally. The helper admission preserves that resulting 7,917-event prefix and appends only a successful string initializer followed by the refused event request. Final deterministic evidence has 7,919 traces and 3,982 original-entry events. The complete final report repeats byte-for-byte using the rebuilt identical module. The reporting change alters only the service descriptor and the displayed argument tail; raw Wasm execution and capture digest remain unchanged.

Separate live-clock evidence reaches the same service boundary and routine/kernel counts, with 7,921 traces and 3,983 entry events. The requested 500 ms pause before sample 4,183 measures 506.5101 ms in that run. Live calibration varies numeric values and carry branches; full live CPU identity is not claimed. Reports retain the synthetic fixed system time and explicit performance-clock model even in this live-monotonic condition.

A verified import insertion adds only slot `0x00253384` → ordinal 189 at index five. All 56 previous slot/ordinal pairs remain; 51 later synthetic tokens shift by four bytes. Ordinal 192 moves from `0xFE000018` to `0xFE00001C`, and inert ordinal 100 from `0xFE0000CC` to `0xFE0000D0`. The patcher and lookup use the same table. The explicit inert comparison accepts only that target relocation; previous experiment CPU fields and service outputs remain literal, without token-value normalization. The second admission changes no binding. Fixed control and stable inert trace/call projections remain unchanged.

The final capture contains 130 suffix rows, 642 total events and 640 completions. The first-512 digest remains `14fcb4cd259e4482e53b54e82a3bc6f08f13631a45e23f1563cc40ab4e32a785`. The full capture digest remains runtime-reported because some older serializers omit other services' argument tails. The new four-word event display does not make those older tails reconstructable.

## Retained modeled state and limits

The existing default-off mutant profile supports only null attributes and full-word zero initial ownership, with checked writable output and up to four opaque records. Its one live unowned handle remains `0x00080004`. It supplies no named/owned mutant, acquisition, wait, release, close, reuse or native mutex. Activation is pre-start only; missing capability rejects before execution. The earlier full default-off comparison remains retained.

Disk/FATX, debug and aggregate ABI observations remain unchanged: EOF 265 in one 16,384-byte cluster, revision 85, opened/closed eight, no live FATX file handle, 65,517 free clusters and eight disk pages; six debug requests, last length 54. Aggregate ABI diagnostics remain 453. Legacy getter slots 3–5 count stack inequality (451), despite frontend ESI labels; unused zero slots 6–8 do not prove zero ESP faults. Individual nonvolatile-register attribution remains unresolved. Recorded CPU fields cover EAX, FS and exception-chain head plus call address/stack, not complete register snapshots.

Keep 64 timestamp samples, 64 successful counter queries, 512 kernel rows, 1,024 total kernel events, 8,192 trace rows and fixed 128 MiB Wasm memory. Its protected guest window remains 64 MiB. No resource cap was raised.

## Validation and next dependency

Independent Sol High review covers both byte/CPU/ABI admissions and their exact comparisons. Actual session metadata confirms Luna Low for private preparation, reference investigation and the scoped reporting changes. Targeted runtime checks pass 269/269; preparation/publication and documentation checks pass. The source-only build contains 17 allowlisted files. The previous mutant component's 66 native/sanitizer runs and 11 Python evidence suites remain historical evidence; this checkpoint changes no portable component or native ABI implementation. Local Node 24.22.1/npm 11.19.1 differ from pinned CI Node 24.14.0/npm 11.9.0; CI remains pending.

The next implementation task is bounded, explicitly selected named synchronization-event creation for the reached Win32 namespace/open-if request, preserving its counted name, checked object/output memory and four-argument ABI. Establish a separate bounded event-object/handle table, name identity and existing-object behavior from the pinned references; retain refusals for unsupported operations. Obtain independent correctness review and compare default-off/control/inert/experiment conditions before accepting that transition. Original graphics, audio, recorded player input and the three complete original round sequences remain unverified.
