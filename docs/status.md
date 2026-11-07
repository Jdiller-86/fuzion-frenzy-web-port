# Project status: a second thread context is created

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. After admitting the exact published thread start `0x0004CBE0`, the existing guest-thread service creates context 2 and the original client returns handle `0x104`. That context remains **READY; its callback has not executed**. Startup next reaches **status 9, `0xC00000BB`, ordinal 192 (`NtCreateMutant`)**, return address `0x000307E3`, with 434 distinct routines and 643 completed kernel operations. The current unnamed-only mutant model refuses this named request without creating another handle or supplying a successful return. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

M1 remains unaccepted. A controllable original screen, audible original gameplay, recorded player input and three consecutive menu/minigame/results/menu sequences still require evidence. Earlier checkpoints remain in Git history; this page owns the current boundary.

## Matching inputs and preparation

The local 2,834,432-byte XBE matches [the reference identity](reference.md), SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The retained Ghidra project is read without saving analysis changes. Pinned xboxrecomp source `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6 and Emscripten 3.1.50 remain available. Compiler binary digests match the recorded toolchain; SDK revision is `e2627e265d940db5ea58dffa63e490375bfc92e5`. See [analysis setup](analysis.md).

| Admission | Seeds + checked views / instructions | Module SHA-256 prefix | Observed next stop |
| --- | --- | --- | --- |
| Prior mutant creation checkpoint | 466 + 3 / 21,945 | `9a88a183` | Untranslated `0x00030716` |
| `CreateEventA`, `0x00030716` | 467 + 3 / 21,982 | `e00dab72` | Untranslated `0x000311D1` |
| `XapiFormatObjectAttributes`, `0x000311D1` | 468 + 3 / 21,993 | `9fc1057c` | Unsupported ordinal 189 |
| Named-event model and accurate callback refusal | 468 + 3 / 21,993 | `9a4c0423` | Missing thread start `0x0004CBE0` |
| Derived `0x0004CBE0` admission | 469 + 3 / 22,004 | `8650657e` | Guarded named ordinal 192 |

The two earlier wrapper/helper admissions retain 37 instructions/97 bytes and 11 instructions/40 bytes; the newly derived callback has 11 instructions/43 bytes. Preparation verifies original-byte identity, independent decoder agreement and complete layout. Every prior admitted body, three checked direct views, timestamp guard and decoder normalization remains unchanged. Independent Sol High review checked partial-register/flag snapshots, signed status branches, checked descriptor stores, ESI preservation and stack cleanup (`RET 16` and `RET 12`). Original title execution occurs only in Chrome; private preparation does not execute it natively.

The 31 seed exports and limits of 512 functions, 32,768 instructions, 32 exports and 32 functions per export remain unchanged. Pinned-compiler rebuilds reproduce the final 1,161,612-byte ABI 25 module, SHA-256 `8650657ed598fe97a2c28301f23dcb9390038a425ae9bd79361015142683196e`. Original input, generated title code, exports and raw reports remain in ignored local research.

## Derived callback entry

The retained project had no function at or containing `0x0004CBE0`, although its listing already decoded the exact instruction there. An existing pointer publication at `0x0004CC50` in the admitted caller agrees with the actual ordinal-255 start argument. A preceding return does not fall through into the address. The 11-instruction, 43-byte body has a closed conditional loop, a single direct call to `0x0003082D`, `RET 4`, and five padding bytes before the next retained function.

Ghidra automatically derived exactly that envelope from its existing listing in a `-noanalysis -readOnly` session; no body was forced and no code cleared or disassembled. The seed records a newly established, temporary analysis function rather than pretending it was retained metadata. Headless output reports discarding changes, and reopening the retained project confirms the function definition is absent. The observed fact before admission was the thread request's pointer, not execution of this callback. See [analysis setup](analysis.md).

Original bytes, all 11 decoder boundaries and complete layout agree. The identity self-LEA preserves EBX and flags and translates as a no-op; conditional flag snapshots, checked memory, call frame and `RET 4` have independent CPU/ABI review. All 471 prior actual bodies remain exact. The sole new direct callee remains guarded. The inherited aggregate decoder-boundary flag is still false because of the unchanged FSTCW versus WAIT/FNSTCW normalization; no new normalization or mixed-layout exemption was added. All 57 kernel bindings, direct entry views, timestamp guards and admission limits remain unchanged. A callable entry is not evidence that this callback ran.

## Bounded named-event creation

`CreateEventA` receives a nonzero name pointer and follows its original named-object path. The helper calls the already supported `RtlInitAnsiString` (ordinal 289), producing an 11-byte counted name with maximum length 12. It writes the original 12-byte object-attributes layout: root `0xFFFFFFFC`, a pointer to that descriptor, and attributes `0x80`. The event call carries four words: output-handle address, object-attributes address, event type 1 and initial state 0. The original name payload stays private.

The pinned declaration and original caller establish stdcall, 16 argument bytes and NTSTATUS return. The optional handler records those four words and supplies EAX zero/ESP +20 on the observed fresh creation. Original `CreateEventA` returns handle `0x00090004`; one object and one handle remain in the selected model. Reports retain checked name/attribute addresses and before/after ABI words without exposing name payloads.

The strict default-off profile is `events: {kind: "synthetic-win32-named-events", synthetic: true, capacity: 4}`. It admits only root `0xFFFFFFFC`, attributes `0x80`, type 1 and full-word zero initial state. It copies a counted printable-ASCII name of 1–63 bytes, with length no greater than maximum length and maximum at most 64. It reads only the counted payload, without requiring a terminator. Entire output, attributes, descriptor and payload ranges are checked before mutation; output/input aliases, malformed table state and capacity exhaustion refuse atomically at the component boundary.

The model separates stable one-based objects from monotonic opaque handles. An ASCII case-insensitive existing-name open allocates another handle to the retained object and returns informational status `0x40000000`; name and initial signal state remain unchanged. Native/compiled source-owned fixtures cover this existing-name case; the current original browser observation is a fresh creation. Up to four handles and four objects survive until whole-proof reset. There is no signal, reset, wait, close, reuse, native event or complete kernel-object layout. Other roots/flags, anonymous names, other event types and nonzero initial states remain guarded. Handles occupy a separate namespace from existing storage and mutant handles.

Activation is pre-start only. Missing capability rejects before execution, including old ABI modules. Strict state and call validation checks object/handle identity and order, unsigned widths, counted bounds, successful versus informational status, final refusal and capture-window omissions. The unselected service still reports its static ABI with `modeled: false`; earlier unidentified one-word refusal reports retain narrowly checked compatibility. Static metadata alone implements no service.

Pinned Cxbx reference evidence identifies root -4 as the Win32 named-object directory, distinct from DOS-device root -3, and `0x80` as open-if-present. Its object manager distinguishes fresh creation from opening an existing same-type object. These are replacement-kernel reference facts, not original-console observations; see [research provenance](research.md#named-event-request-and-namespace-reference). Preserve the original name and namespace.

## Second guest-thread context and next named request

The admitted lookup lets the previously refused ordinal-255 request complete. Context 1 remains unchanged. Context 2 has open handle `0x104`, READY state, object `0x03C11000`, FS `0x03C11200`, stack `[0x03C12000, 0x03C22000)`, 20-byte TLS at `0x03C21FEC`, initial SP `0x03C21FC4`, start `0x0004CBE0`, zero context and the existing system routine `0x00034307`. The original thread-creation client returns the new handle. There is no trace entry for the new callback and no scheduling expansion or native thread.

The bounded arena footprint is 17 pages: one object/PCR page and 16 stack pages, with TLS inside the stack. The memory model reclassifies those 16 stack pages from its already committed pool: pool count 1,008 → 992 and stack count 16 → 32. VM regions, available physical pages, reserve and commit totals remain unchanged. This is modeled page ownership, not new VM capacity or a console allocation claim.

Original startup then reuses its named-object constructor. A successful ordinal-289 initialization records a 13-byte name with maximum length 14. The following three-argument mutant request carries output and attributes pointers plus full-word zero initial ownership. Raw attributes retain Win32 root `0xFFFFFFFC`, the counted-name descriptor and open-if `0x80`. The payload stays private. The existing unnamed-only component refuses nonnull attributes: its one unowned handle `0x00080004` survives, while its last diagnostic status becomes `0xC00000BB`. No named mutant, handle write, ownership grant or successful service return is supplied.

The earlier callback-reporting defect remains fixed: missing callbacks are marked failed and identify the actual missing argument before allocation. The present second context is accepted only after exact entry admission; lookup guards and the four-thread cap are unchanged.

## Exact comparisons and explicit clocks

The deterministic profile supplies synthetic regression inputs: one 100 ns tick per sample plus a 5,000,000-tick jump before sample 4,183, fixed system time and paired 733,333,333 Hz performance-clock services. It uses no live wait and measures neither elapsed time nor original-console timing. The former stack/return-address fault remains repaired.

The prior event model's default-off comparison remains retained. This callback admission preserves all 7,923 preceding raw CPU events, the original-entry prefix and 130 preceding call objects/outputs literally. The final refused thread request becomes successful, followed only by the 13-byte string initialization and named-mutant refusal. Final deterministic evidence has 7,927 traces and 3,986 entry events. The complete report repeats byte-for-byte using an identical rebuild. No CPU value normalization is used.

Separate live-monotonic evidence reaches the same named-mutant boundary and routine/kernel counts, with 7,923 traces and 3,984 entry events. The requested 500 ms pause before sample 4,183 measures 501.6899 ms in that run. Live calibration varies numeric values and carry branches; full live CPU identity is not claimed. Synthetic fixed system time and paired performance-clock services remain explicit in this condition.

Control and inert stops and stable trace/call projections remain unchanged. No import was added or relocated: the existing 57 bindings and ordinal-100 inert token `0xFE0000D0` are retained. Thread state, page-role reclassification and mutant diagnostic status are explicit reviewed transitions rather than discarded differences.

Final capture contains 133 suffix rows, 645 total events and 643 completions. Its first-512 digest remains `14fcb4cd259e4482e53b54e82a3bc6f08f13631a45e23f1563cc40ab4e32a785`. The full digest remains runtime-reported because other legacy serializers omit argument tails. The four-word event display and current service evidence do not reconstruct those older tails.

## Retained modeled state and limits

The existing default-off mutant profile supports only null attributes and full-word zero initial ownership, with checked writable output and up to four opaque records. Its one live unowned handle remains `0x00080004`; the current diagnostic status records refusal of the newly reached named variant. It supplies no named/owned mutant, acquisition, wait, release, close, reuse or native mutex. Activation is pre-start only; missing capability rejects before execution. The earlier full default-off comparison remains retained.

Disk/FATX, debug and aggregate ABI observations remain unchanged: EOF 265 in one 16,384-byte cluster, revision 85, opened/closed eight, no live FATX file handle, 65,517 free clusters and eight disk pages; six debug requests, last length 54. Aggregate ABI diagnostics remain 453. Legacy getter slots 3–5 count stack inequality (451), despite frontend ESI labels; unused zero slots 6–8 do not prove zero ESP faults. Individual nonvolatile-register attribution remains unresolved. Recorded CPU fields cover EAX, FS and exception-chain head plus call address/stack, not complete register snapshots.

Keep 64 timestamp samples, 64 successful counter queries, 512 kernel rows, 1,024 total kernel events, 8,192 trace rows and fixed 128 MiB Wasm memory. Its protected guest window remains 64 MiB. No resource cap was raised; the guest-thread table remains limited to four contexts.

## Validation and next dependency

Independent Sol High review covers the derived boundary/provenance, generated CPU/ABI/memory surfaces, retained bodies, second-thread state and explicit browser comparisons. Actual worker metadata confirms Luna Medium for the uncertain entry inspection and Luna Low for validation registration. The existing thread-bootstrap and memory-statistics fixtures are now included in the standard native runner. Runtime checks pass 279/279, native runs 72/72 including sanitizers, Python evidence suites 13/13, and seed verification matches all 31 exports. Native fixtures execute independently authored CPU/RAM and services, not the original title. Preparation/publication and documentation checks pass; the source-only build contains 17 allowlisted files. Local Node 24.22.1/npm 11.19.1 differ from pinned CI Node 24.14.0/npm 11.9.0; CI remains pending.

The next task is bounded, explicitly selected named-mutant creation/open-if for the reached Win32 namespace and 13-byte counted name, with zero initial ownership. Preserve name and object-type identity, checked attributes/output, current unnamed handles and unsupported wait/acquire/release/close refusals. Do not substitute anonymous creation or run the READY callback outside its admitted scheduler path. Obtain independent correctness review and retain control/inert/experiment evidence. Original graphics, audio, recorded player input and three complete original round sequences remain unverified.
