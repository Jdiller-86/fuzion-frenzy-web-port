# Project status: startup completes a local lock cycle

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. With explicit acquisition and release profiles, original startup stops with **status 2** at untranslated `0x0004E800`, return `0x0004F7DC`, SP `0x03C10E7C`, first frame word `0x002D14A8`, after **442 original routines and 741 completed kernel operations**. Helpers `0x0003080D`, `0x0004BDC0` and `0x0004C380` now return with balanced frames; the earlier wait wrappers also returned. READY callback `0x0004CBE0` remains unexecuted. No original screen or M1 acceptance exists. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

## Current build and admission

The compatible private XBE is 2,834,432 bytes, SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The ABI 25 release module is **1,186,511 bytes**, SHA-256 `21d4e978c24b760dd4c2f06b3f146049796f6b7e95e140101b9920b484a718a9`; two builds match. Profile counts are **477 seeds, 3 checked views, 22,214 instructions, 32 exports and 59 bindings**. Fixed Wasm memory is 128 MiB; the sole import is `env.ff_host_clock`.

The latest admitted original helper, `0x0003080D`, is 32 bytes / 12 instructions / `RET 4`. Its caller edge at `0x0004BE3A` and return `0x0004BE3F` match original bytes and Capstone 5.0.6. All 479 preceding generated bodies are preserved. The last export grows from two to three functions; the full caller group and all resource ceilings remain unchanged. Earlier helper `0x000311F9` takes its observed sentinel branch, returning zero without writing an optional timeout; its other arithmetic branch remains unobserved.

One exact original import pair is added relative to the acquisition checkpoint: slot `0x0025339C` contains `0x800000DD`, ordinal 221. Raw bytes, retained metadata and the independent XBE parser agree; all 58 preceding pairs remain identical. Inserting it at index 8 shifts ordinal 234's token `0xFE000020` → `0xFE000024` and inert ordinal 100 `0xFE0000D4` → `0xFE0000D8`. The prior ordinal-234 insertion remains recorded in Git/private evidence. Independent review accepts the exact binding extension and the observed inert token shift; no ceiling increases.

## Observed ownership and ABI

Both compatibility profiles are explicit and default-off. Acquisition admits only an unowned local mutant, mode 1, nonalertable, NULL timeout. NULL timeout means indefinite wait, not polling; this run only needs an immediately available object. Actual arguments `[0x0008000C, 1, 0, 0]` acquire object 3 for RUNNING thread 1: owner 0 → 1, EAX zero, SP `0x03C10E28` → `0x03C10E3C` (+20).

The reached ordinal-221 call has two arguments `[0x0008000C, 0]`. The release profile requires that same running owner and a NULL previous-count output pointer. One attempt/completion changes object 3 owner 1 → 0, EAX `0x0008000C` → zero, SP `0x03C10E54` → `0x03C10E60` (+12). All three final objects are unowned. Original helper `0x0003080D` then returns EAX 1, followed by `0x0004BDC0` and `0x0004C380`; their entry/exit SP agrees and FS/exception head stays stable. VM fault words remain zero.

Release selection requires acquisition and creation capabilities, and seals before execution. Unknown handles, wrong ownership, non-NULL previous-count pointers and unsupported wait shapes refuse without changing ownership or guest registers. Blocking, recursive ownership, abandonment, non-NULL timeouts, output-count writes and callback scheduling remain unsupported. These are observations of explicitly modeled services, not proof of retail-kernel equivalence.

The joint report validator reconstructs chronological acquire/release transitions through aliases, checks running-thread/stack bounds, exact call/last-record/count agreement and both final ownership tables, and requires all such attempts in the capture. Captured handle creations must precede use; omitted-prefix creation history is not reconstructed. It rejects duplicate publication, unwitnessed ownership and illegal transitions. Existing acquisition-only validation remains in force when release is not selected.

## Comparisons and evidence limits

With acquisition selected but release disabled, the new module still stops unsupported at ordinal 221, token `0xFE000020`, after 442 routines / 740 completed operations. Compared with the preceding helper module, only module identity and intentional ABI metadata enrichment change: ordinal 221 is named `NtReleaseMutant`, with two arguments and NTSTATUS/8-byte argument cleanup, and its call capture grows from one to two words. Raw CPU/kernel evidence and ownership remain unchanged. Legacy unidentified descriptors remain readable.

Deterministic selected execution reconstructs **9,456 rows / 4,746 entries**, using an 8,192-row initial window and 1,264-row suffix. Full trace SHA-256 is `26cb62a6172eb1290e5a90cd3d09ffe9ecb255421eb1812050d7cc3fdf0107f6`; prefix SHA-256 remains `c8a0bee78f1576f95bef8f8668132a704774172a4125a69be7e28b2c957a1f90`. The disabled 9,453-row trace is preserved, followed by three balanced original returns. Suffix/repeat reports match. Initial-window capture intentionally stops at its 8,192-row horizon before the lock cycle. The preceding acquisition audit explicitly used the retained `0x0003082D` initial window; no missing `0x000311F9` initial capture is claimed.

The selected kernel capture has 230 suffix rows / 742 events / 741 completions, SHA-256 `8a8f60fc79f1bdc03f2ebe12fe2386358c60134c5fda4998626002270f31f57d`; prefix digest remains `c76f75a83a023f01228cea60805cd130fb1e1d4259c0de6ce0a735cdaebd7ba4`. Control remains status 13 / ordinal 1 / 339 routines / 405 operations. Inert remains status 1 / ordinal 100 / 347 / 419, at the reviewed new token `0xFE0000D8`.

Live reaches the same guard/counts with **9,450 rows / 4,743 entries**, measuring **500.565 ms** for a requested 500 ms pause. Raw live rows, clocks and prefix/full digests remain separate evidence without reconstruction or blanket EAX/timing normalization.

Earlier bounded results remain qualified: the existing allocation model supplies a 49 MiB region at `0x006F4000`, and `0x001E87E0`/`0x0004BC10` return that value with balanced frames. Its service capture exposes only the first argument and status. Default debug admission remains eight completions; explicit start 8 allows a total horizon of 16, with nine observed completions and last service 1 / length 57. No original debug payload is copied, decoded or printed. Display declaration/read-back does not establish GPU execution or rendering.

## Next task and checks

Inspect only the actually reached `0x0004E800` at its observed caller/return, verify original bytes, full layout, decoder agreement and prior bodies, then replay to the next actual guard. The last export has room within its existing 32-function cap. The first frame word does not establish arity. A separate source-owned probe is evaluating the alternative native SDK's automatic C++ generation; its different register/memory/dispatch contracts require adaptation and do not bypass browser guards.

Current checks: **runtime 324**, **native 82 including sanitizers**, **Python evidence 17/17**, a **17-file source-only build**, preparation/docs and diff checks pass. Independent Sol High C/ABI and joint-history/integration reviews and the closed 12-report release audit with four rejected negative mutations pass. Checkpoint CI is pending. [Acquisition checkpoint CI](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/42387148914) passes on Ubuntu and Windows for `16c6ee5`; native fixture steps run on Linux. Fresh local checks use project-local pinned Node 24.14.0/npm 11.9.0 and retained Emscripten 3.1.50. Source CI does not run proprietary title code.

Fixed limits remain 512 functions, 32,768 instructions, 32 exports, 32 functions per export, 64 timestamp samples, 64 counter queries, 1,024 kernel events, four guest threads, 128 MiB Wasm and 64 MiB guest memory. Original rendering, audible audio, recorded player input and three complete menu/minigame/results/menu sequences remain unaccepted. M1 is active and incomplete.
