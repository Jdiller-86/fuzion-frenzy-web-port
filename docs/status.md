# Project status: startup creates an unnamed mutant

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. With explicit synthetic creation support, original startup reaches **status 2 at untranslated `0x00030716` (`CreateEventA`)**, return address `0x0004CC43`, with 432 distinct routines and 639 completed kernel operations. Ordinal 192 (`NtCreateMutant`) completes the reached unnamed, initially unowned request and returns opaque handle `0x00080004`. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

M1 remains unaccepted. A controllable original screen, audible original gameplay and three consecutive original menu/minigame/results/menu sequences still require evidence. Earlier checkpoints remain in Git history; this page owns the current boundary.

## Matching inputs and preparation

The local 2,834,432-byte XBE matches [the reference identity](reference.md), SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The retained Ghidra project is read without saving analysis changes. Pinned xboxrecomp source `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6 and Emscripten 3.1.50 remain available. Compiler binary digests matched the recorded toolchain; SDK revision is `e2627e265d940db5ea58dffa63e490375bfc92e5`. See [analysis setup](analysis.md).

Checkpoint `79c3fd0` reached the ordinal-192 refusal after these four admissions. Their unchanged title profile also underlies the new service experiment:

| Admission | Seeds + checked views / instructions | Module SHA-256 prefix | Next stop |
| --- | --- | --- | --- |
| `0x001E1587` | 463 + 3 / 21,888 | `29922b98` | `0x001E145B` |
| `0x001E145B` | 464 + 3 / 21,907 | `1ba94203` | `0x0004E6A0` |
| `0x0004E6A0` | 465 + 3 / 21,912 | `b164e4e8` | `0x000307B5` |
| `0x000307B5` | 466 + 3 / 21,945 | `ec7e1d46` | Ordinal 192 refusal |

The 70-, 19-, 5- and 33-instruction routines retain original-byte identity, independent decoder agreement and complete layout. Every prior admitted body, three checked direct views, timestamp sites and decoder normalizations remain unchanged. Independent review checked flag snapshots, partial-register writes, direction-aware checked string operations, saved registers and stack cleanup. `CreateMutexA` at `0x000307B5` uses `RET 12`; its successful path now executes after modeled creation. The nonzero-name helper at `0x000311D1` remains guarded.

Preparation retains 31 seed exports and the 512-function, 32,768-instruction, 32-export and 32-functions-per-export limits. The unchanged title profile now links the optional source-owned creation model. Pinned-compiler rebuilds reproduce the 1,150,527-byte ABI 25 module, SHA-256 `9a88a18381b0827072d4125880ea46bdc83fdd0cfb20f56aa36608b03928a70d`. Original inputs, generated title code, exports and raw reports stay in ignored local research. Original title execution occurs in Chrome.

## Explicit clock conditions

Both conditions retain synthetic UTC OS settings, fixed system time and explicitly paired performance-frequency/counter services at 733,333,333 Hz. These zero-argument services return EDX:EAX. The two timestamp sites remain default-off guards unless the selected clock model is enabled. Counter/timestamp ordering and checked scaling remain enforced; the former stack/return-address fault is repaired and is not a new repair task.

The deterministic condition supplies exact regression inputs: one 100 ns tick per sample and a 5,000,000-tick jump before sample 4,183. It requires fixed system time, rejects the measured pause, uses no live clock/wait, and refuses beyond 16,384 samples without allocating a journal. Reports identify synthetic input and validate sample/source arithmetic. It measures no elapsed time or original-console timing. The final complete report repeats exactly, including stop arguments, recorded CPU fields and all outputs, with 7,916 traces and 3,980 original-entry events.

The separate live condition retains browser monotonic sampling and a requested 500 ms one-shot wait before sample 4,183. The final run measures 506.39 ms and reaches the same untranslated stop after creation, with 7,908 traces. Live calibration changes numeric values and carry branches; full live CPU/trace identity is not claimed. Earlier read-only debugger witnesses and a bounded arithmetic oracle explain the first 15 product/carry observations under the existing binary64 model. Those observations do not establish historical RAM provenance or original extended-precision fidelity.

Keep the 64 timestamp samples, 64 successful counter queries, 512 kernel rows, 1,024 total kernel events, 8,192 trace rows and fixed 128 MiB Wasm memory limits. No resource cap was raised.

## Bounded mutant creation

The new default-off profile is `mutants: {kind: "synthetic-unnamed-unowned-mutants", synthetic: true, capacity: 4}`. Its source-owned component retains up to four opaque handle records until whole-proof reset. Creation admits only null object attributes and full-word zero initial ownership. It validates nonnull aligned four-byte writable guest output, table integrity and capacity before changing output or table state. Handles are sequential in a separate synthetic namespace, beginning `0x00080004`. Named or initially owned requests, invalid memory and capacity exhaustion stop explicitly. No host mutex, wait, acquisition, release, close, handle reuse or original kernel-object layout is supplied.

The selected handler records the exact three arguments, returns NTSTATUS zero in EAX, and advances ESP by 16 bytes for arguments plus return address. Current Chrome evidence observes the guest handle store, one retained unowned record, and continued original client execution. Unsupported operations on these handles still guard; no complete mutex semantics or retail-kernel equivalence follows. The fixed 128 MiB Wasm memory contains 64 MiB of guest RAM plus host state; output checks remain inside that guest window.

Capability/export checks reject missing support before title execution, including older ABI modules. Activation is permitted only before the probe starts. The compiled but unselected capability reproduces the entire preceding deterministic refusal report apart from module identity. The enabled condition preserves all 7,906 preceding recorded CPU events and 127 earlier call objects/outputs exactly, without normalization. The original default fallback captures four diagnostic stack words and the legacy frontend shows one for ordinal 192; the original call ABI and the new selected recorder/frontend use three. This scope distinction corrects the earlier shorthand about a one-argument recorder.

## Exact comparisons and reached side effects

Deterministic admissions preserve their preceding raw CPU trace and original-entry sequences without value normalization. Prior kernel call objects and outputs remain literal prefixes. Fixed control boundaries remain unchanged; inert comparisons retain stable sequences and call frames with the explicit token relocation below. These CPU observations contain EAX, FS and exception-chain head, plus call address/stack; they are not complete register snapshots.

Completing the formatting path adds 30 successful calls through already supported services: reopen, query EOF 210, seek, append 55 bytes and close. FATX EOF becomes 265 in the same 16,384-byte cluster, revision advances 83 to 85, opened/closed counters each advance by one, and no FATX file handle remains live. Allocation, free-cluster and disk-page counts remain unchanged. One checked debug request completes, bringing its count to six; the last request is 54 bytes and its payload remains private. Independent review accepted this explicit backend transition rather than discarding differences from the earlier state.

The new import slot `0x00253398` contains ordinal 192 in the matching XBE. Generated binding insertion at index six retains every prior slot/ordinal pair and shifts 49 later synthetic dispatch tokens by four bytes. The patcher and lookup share the same table. Ordinal 100's inert refusal moves from `0xFE0000C8` to `0xFE0000CC`; all its other stop fields remain unchanged. The new ordinal-192 token is `0xFE000018`. No preceding experiment CPU value or call output changes from this relocation; no token-value normalization is used.

The final capture contains 128 suffix rows, 640 total events and 639 completions. Its first-512 runtime digest remains `14fcb4cd259e4482e53b54e82a3bc6f08f13631a45e23f1563cc40ab4e32a785`. The full capture digest remains runtime-reported: legacy serialized rows omit argument tails for some services and cannot reconstruct that hash independently. The successful ordinal-192 row now carries the three observed inputs, output handle, creation count and before/after EAX/ESP evidence. Its full digest changes with the corrected recorded arity and successful status; the first-window digest is unchanged.

The completed formatting tail adds 21 aggregate ABI diagnostics, reaching 453. Legacy getter slots 3–5 count stack inequality among logged calls (451 total), despite the browser's ESI labels; unused slots 6–8 returning zero do not prove zero ESP faults. Reviewed traces retain the known SEH frame restorations and return through `0x0004C510`, but individual nonvolatile-register attribution remains unresolved. These diagnostic limits remain separate from the new helpers' byte, memory and ABI review.

## Validation and next dependency

Independent Sol High review passed creation-only component memory/ABI correctness, profile/report guards, exact default-off comparison, enabled CPU/call prefixes, repeat and rebuild. Worker execution metadata confirms two Luna Low workers. Runtime checks pass 267/267, Python evidence suites 11/11, and native component runs 66/66 including sanitizers. The compiled bridge fixtures contain source-owned synthetic CPU/RAM only; no original title instructions execute natively. Preparation/publication checks, documentation checks and the 17-file source-only build pass. Local Node 24.22.1/npm 11.19.1 differ from pinned CI Node 24.14.0/npm 11.9.0; CI remains pending.

The next dependency is inspection and bounded admission of reached `CreateEventA` at `0x00030716` (37 instructions, 97 bytes in the retained read-only export). Its observed name argument is nonzero; preserve that named-object path rather than substituting an anonymous event. The existing nonzero-name helper and event service remain unaccepted. Preserve the current model's refusals and obtain independent review for further CPU/memory/ABI work. Original screen, audio, recorded player input and three complete original round sequences remain unverified.
