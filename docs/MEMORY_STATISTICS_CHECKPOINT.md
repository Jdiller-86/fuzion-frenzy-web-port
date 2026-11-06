> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Modeled memory statistics return and original code enters renderer initialization

November 2, 2026. Original browser startup now answers the reached `MmQueryStatistics` request from consistent modeled page ownership and continues through the 140-byte helper at `0x00042be0`. It stops before untranslated `0x001f7650`, which the original code calls with a 640×480 display-mode structure. **There is no playable original screen, no renderer service and no general kernel.** The complete original-game/custom-feature goal remains active.

## Observed execution

Both explicit fresh synthetic profiles (`settings-memory-0.json`, `settings-memory-1.json`) still return from all 83 initialization callbacks. Each run now executes **229** distinct original routines, records **3,626** trace events and completes **393** kernel operations (prior boundary: 227 / 3,622 / 392).

1. Ordinal 181, `MmQueryStatistics`, receives `62983784` with Length 36 and returns the nine-DWORD structure `[36, 16384, 14567, 16384, 1048576, 0, 1008, 16, 789]`: total physical pages, available pages, virtual bytes committed, virtual bytes reserved, then cache/pool/stack/image committed pages.
2. Original `0x00030ade` resumes at `0x00030af5` and returns.
3. Execution reaches `0x00042be0` (140 bytes / 40 instructions). It calls `0x002022e0` (`MOV EAX,1; RET 0x4`), clears 13 DWORDs of stack state, pushes a mode structure containing 640×480, format 7 and value 0x2a, and calls `0x001f7650`.
4. `0x001f7650` is not translated. Execution stops at target `0x001f7650` with return address `0x00042c3f` — the instruction immediately after that call — and stack arguments `[0, 1, 0, 64, 62983776, 62983772, 2954408, 3342720, 640, 480]`.

The stop is a genuine translation boundary, not an invented kernel result. No video-mode, D3D or GPU service is supplied.

## Accounting contract

[`memory_statistics.c`](../src/runtime/memory_statistics.c) derives every field from the owned guest page model instead of host/Wasm allocation size:

- Private virtual pages contribute committed bytes only when committed, and reserved bytes regardless; reserved-but-uncommitted pages do not reduce available physical pages.
- Fixed pre-mapped pages contribute only through an explicit role (pool, stack, image, cache) and only when committed. The 4 MiB supported arena at `0x03c00000` is the pool; the boot thread's 64 KiB stack is reclassified inside it; the image region is classified from its actual owned descriptor.
- The explicit `baselinePoolPages` input adds pages that the model does not itself own. Both supplied test profiles use zero, so no unmodeled pool claim is fabricated.
- `available = physical - (private committed + pool + stack + image + cache)` with a checked 64-bit sum. An inconsistent model (unknown role, unclassified committed fixed page, committed unowned page, owner/page mismatch, used > physical) fails instead of returning numbers.
- The query validates the pointer, full 36-byte write range and guest Length before writing, then writes once; every failure path leaves guest memory and statistics state unchanged.
- Admission now pins the explicit physical page count to the bounded guest RAM (`physical_pages == FF_GUEST_BYTES/FF_VM_PAGE`). A smaller synthetic value cannot understate the pages this runtime actually owns.

`MemoryAccounting.scope` in the reports is `modeled guest pages; no retail-console equivalence`. The 64 MiB guest window and 128 MiB Wasm allocation remain separate facts and are not reported as console statistics.

## Evidence and checks

Private records under `research/verification/`:

| Record | Scope |
| --- | --- |
| `recomp-title-post-memory-01/` | Current 21-seed selection (319 functions / 10,654 instructions) with exact paths and digests |
| `recomp-post-memory-build-02/` | Pinned-admission ABI-23 module (`dacbfe8b…`, 947,881 bytes) and its 36 source/declaration identities |
| `post-memory-checkpoint-20261102-01.json` | Independent identity audit: zero source/seed mismatches, generation and module digests, both report digests and their observed facts |
| `post-memory-browser-20261102-03.json`, `-04.json` | Final original runs for settings zero/one: statistics words, 229 routines, 393 operations and the `0x001f7650` stop |
| `memory-statistics-owned-03/` | Optimized and address/undefined-behavior sanitizer accounting, lifecycle, unit, budget and output-atomicity checks |
| `recomp-after-memory-next-01/seed.json`, `recomp-after-memory-next-02/seed.json` | Read-only exports of `0x00042be0` (40 instructions) and `0x002022e0` (2 instructions) |
| `memory-statistics-browser-20261102-01.json`, `-02.json` | Previous boundary: statistics returned, `0x00042be0` untranslated |

Validation executed for this checkpoint: `npm run check` (preparation contract and publication guard), `npm run test:runtime` (40/40 cases, including explicit memory-profile rejection), the owned native and sanitizer accounting suites, and both browser profiles. A negative browser case with `physicalPages: 16383` fails admission with `Cannot admit explicit modeled physical-memory accounting` and produces no report, confirming the pinned capacity. An independent GLM 5.3 review confirmed the field order/units against the pinned nxdk declaration, the absence of double counting, the invariant and atomicity behavior, the ABI/endianness wiring and the test coverage; its two remaining low findings are recorded below. No public CI result is claimed for this local checkpoint.

## Remaining boundaries

- The physical capacity is an explicit model pinned to the bounded 64 MiB guest; it is not recovered console hardware behavior.
- `baselinePoolPages` is a required explicit input, never a guessed kernel reservation.
- Thread-create reclassification reports `FF_BOOT_INVALID` rather than the underlying classify status (diagnostics only).
- General FATX file access, persistence, disc/package reads, scheduling/preemption, exceptions, renderer/D3D initialization, audio, controllers, browser-local end-user conversion and every planned online/social/control/authority feature remain unfinished.

## Next task

Admit and translate the reached `0x001f7650` video-mode initialization from a bounded read-only export, then observe its actual first kernel/device request. Preserve byte/decoder/layout/memory admission and the existing budgets; do not substitute a display-mode success stub, host canvas mode or D3D service. Reuse `recomp-title-post-memory-01`, the new build and the retained analysis project. The retained read-only export `recomp-post-memory-next-01/seed.json` already holds `0x001f7650` (147 bytes / 36 instructions): it caches two global limits, initializes the structure at `0x00203b90`, calls `0x001fbb00` and, on failure, `0x001fc010`, then returns the status. Those two callees still need their own bounded exports.
