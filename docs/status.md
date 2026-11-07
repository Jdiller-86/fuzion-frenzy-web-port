# Project status: named-mutant creation reaches the next original routine

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. The explicitly selected, bounded Win32 mutant model completes the reached named `NtCreateMutant` request (ordinal 192). The original client returns handle `0x00080008`, preserving the counted name, root -4, open-if flag and zero initial ownership. Startup next stops at **status 2, untranslated `0x0004C380`**, return address `0x0004F7D0`, with **434 distinct routines and 644 completed kernel operations**. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

M1 remains unaccepted. A controllable original screen, audible original gameplay, recorded player input and three consecutive menu/minigame/results/menu sequences still require evidence. Earlier checkpoints remain in Git history; this page owns the current boundary.

## Matching inputs and preparation

The local 2,834,432-byte XBE matches [the reference identity](reference.md), SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. Pinned xboxrecomp source `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b`, Capstone 5.0.6 and Emscripten 3.1.50 remain available. Compiler binary digests and SDK revision `e2627e265d940db5ea58dffa63e490375bfc92e5` match the recorded toolchain. See [analysis setup](analysis.md).

This service change admits no additional title code. The profile retains 469 seeds, three checked direct views, 22,004 instructions, 31 seed exports and 57 bindings. Original bytes, prior generated bodies, timestamp guards, decoder normalizations and resource limits are unchanged. The inherited false aggregate decoder-boundary flag reflects one unchanged FSTCW versus WAIT/FNSTCW normalization; it is not a new exemption. The derived callback's 11 instructions/43 bytes have exact independent boundary agreement and read-only provenance.

Pinned-compiler rebuilds reproduce the 1,165,595-byte ABI 25 module, SHA-256 `16ef10c3c48a386bbc5c69ac706868f8c32460d5e7a54fed2274e56a1af55d2c`. Its sole host import remains `env.ff_host_clock`. The prior module (`8650657e`, 1,161,612 bytes) remains the named-request refusal baseline. Original input, generated code, modules, exports and raw reports remain ignored and local. Original title execution occurs only in Chrome; native fixtures use independently authored CPU/RAM.

## Bounded Win32 mutant creation

The new alternative profile is `mutants: {kind: "synthetic-win32-mutants", synthetic: true, capacity: 4}`. It requires the explicitly selected named-event profile. It replaces the legacy unnamed-only mutant pool; it does not add capacity. One pool retains at most four mutant objects and four handles total across anonymous creation and named open-if, with no reuse until reset.

Anonymous creation requires null attributes and full-word zero ownership; each creation makes a distinct unowned object. Named creation accepts only Win32 root `0xFFFFFFFC`, attributes `0x80`, and a printable counted ASCII name of 1–63 bytes, length no greater than maximum length, maximum at most 64. The eight-byte ANSI descriptor is DWORD aligned. Full output, attributes, descriptor and counted payload spans are checked before mutation; output/input aliases, invalid tables and exhausted capacity refuse atomically.

Names are copied locally and matched with ASCII case folding. Fresh creation returns zero; opening an existing named mutant returns informational `0x40000000` and a new handle to the retained object. Both mutant and event creation refuse a name belonging to the other object type with `0xC0000024`, without changing the peer. These namespace semantics follow pinned replacement-kernel references, not an original-console observation. See [research provenance](research.md#named-event-request-and-namespace-reference).

The observed original path first creates anonymous object 1/handle `0x00080004`, then named object 2/handle `0x00080008`, both unowned. The named request has a 13-byte name, maximum 14, and returns EAX zero with ESP +16 for three DWORD arguments. The original name bytes remain private. Duplicate opens and cross-type collisions are established only by synthetic component/compiled-bridge fixtures in this checkpoint. Ownership, acquisition, waits, release, close, native mutexes and a complete kernel-object ABI remain unsupported.

Activation is pre-start only. All required exports are checked before setters; the event peer initializes first and is locked while the new mutant model is active. Legacy and new mutant selection are exclusive. Reports use `localMutantState` for the new profile and retain `mutantState` for the legacy profile. Typed Wasm adapters normalize signed i32 status words only after integer/range checks; strict serialized state validation remains unsigned.

## Preserved event, thread and storage state

The prior named synchronization event remains object 1/handle `0x00090004`, with its copied 11-byte name, type 1 and unsignaled state. Event creation/open-if remains bounded to four objects/handles, root -4, flag `0x80` and zero initial state. It supplies no signal/reset/wait/close operation or native event.

Context 1 is unchanged. Context 2 remains **READY; its callback has not executed**. Its open handle is `0x104`, object/FS `0x03C11000`/`0x03C11200`, stack `[0x03C12000, 0x03C22000)`, TLS at `0x03C21FEC` (20 bytes), initial SP `0x03C21FC4`, start `0x0004CBE0`, context zero and system routine `0x00034307`. No scheduling expansion or native thread is added.

The prior 17-page arena footprint and pool-to-stack reclassification remain unchanged: pool 992, stack 32; VM regions, available physical pages, reserve and commit are unchanged. Disk/FATX retains EOF 265 in one 16,384-byte cluster, revision 85, opened/closed eight, no live file handle, 65,517 free clusters and eight disk pages. Six debug requests remain, last length 54.

## Exact comparisons and explicit clocks

With the capability compiled but the old profile selected, the entire deterministic report matches the prior checkpoint except module identity. Selecting the new profile preserves all **7,927 preceding raw CPU events** and the original-entry prefix. Of 133 captured calls, 131 objects are unchanged. The earlier anonymous-192 output intentionally gains object/name metadata while preserving every common handle/EAX/ESP value; the final named-192 refusal becomes a fresh success. No raw CPU normalization is used.

Five subsequent raw events and one existing helper entry reach the untranslated routine. Final deterministic reports repeat byte-for-byte with identical rebuilt modules: **7,932 traces / 3,987 entries**. The synthetic input remains one 100 ns tick per sample, a 5,000,000-tick jump before sample 4,183, fixed system time and paired 733,333,333 Hz performance services. It measures no elapsed time or original-console timing.

Separate live-monotonic evidence reaches the same stop and routine/kernel counts, with **7,926 traces / 3,984 entries**. The requested 500 ms pause measures **508.535 ms**. Full live CPU identity is not claimed. Control/inert stops, trace kind/address/stack projections and captured argument/status frames remain unchanged; live-clock outputs vary. The 57 bindings and ordinal-100 inert token `0xFE0000D0` remain fixed.

The closed audit pins all changed fields: one clock sample, VM checked accesses 126,919 → 126,951, PFIFO diagnostic snapshot checks 28,931 → 28,946, rolling bounded notes/pop records and ABI aggregate 453 → 454. PFIFO work/output and VM regions/fault remain unchanged. Legacy getter slots 3–5 count stack inequality (451 → 452), despite frontend ESI labels; unused zero slots 6–8 do not prove absence of ESP faults. Individual register attribution remains unresolved; raw CPU reports cover EAX, FS and exception head, not all registers.

Capture retains 133 suffix rows and 645 total events, now with 644 completions. The first-512 digest remains `14fcb4cd259e4482e53b54e82a3bc6f08f13631a45e23f1563cc40ab4e32a785`. The full digest remains runtime-reported because legacy JSON serializers omit some argument tails. Eight negative audit mutations are rejected.

## Validation and next dependency

Independent Sol High review covers C memory/ABI, typed namespace identity, profile locks, strict reporting and the browser transition. Actual metadata confirms Luna Medium core/report work and Luna Low compiled-fixture work. Runtime checks pass 293/293, native runs 74/74 including sanitizers, and Python evidence suites 14/14. The existing event fixture now extracts its complete nested conditional; the new bridge fixture covers both namespace directions, shared capacity, atomic refusals and default-off compilation. Local Node 24.22.1/npm 11.19.1 differ from pinned CI Node 24.14.0/npm 11.9.0; CI remains pending.

Keep limits of 512 functions, 32,768 instructions, 32 exports, 32 functions per export, 64 timestamp samples, 64 counter queries, 512 capture rows, 1,024 kernel events, 8,192 trace rows, four guest threads, four mutant handles and fixed 128 MiB Wasm/64 MiB guest memory.

The next task is exact admission of the reached `0x0004C380` under the unchanged guards. A new read-only retained-project export identifies an established 48-instruction/156-byte function; it has not yet passed independent decoder/profile admission or executed. Validate its caller/arguments, bytes, layout, generated CPU/memory/ABI and retained bodies before replay; stop at unsupported callees. Keep the READY callback on its existing scheduler path. Original graphics, audio, recorded player input and three complete round sequences remain unverified.
