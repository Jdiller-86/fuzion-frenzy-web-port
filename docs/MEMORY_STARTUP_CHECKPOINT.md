# Checked guest memory and original heap-startup checkpoint

November 1, 2026: original startup now reserves 1 MiB, commits its first 4 KiB page and initializes a critical section inside that page. Both explicit synthetic settings profiles pass **27 browser checks**, including the earlier 8,410 copy/ABI cases per run. The new stop is untranslated `0x00031da0`, return `0x00032696`. **No playable original screen, complete runtime/fidelity or planned online/social feature acceptance is established.** Static recompilation and the complete product objective remain selected.

This extends the [copy-helper proof](COPY_STARTUP_CHECKPOINT.md). Original code executes only in a browser worker. The privately compiled module and XBE are selected locally, never served/uploaded/committed; original instructions, RAM, assets and raw evidence stay local. Developer compilation is not the required browser-local end-user conversion pipeline.

## Observed original execution

| Observation | Evidence | Boundary |
| --- | --- | --- |
| Actual reservation | Ordinal 184 returns `0x00330000` and size `0x00100000` for the original null-base/reserve/read-write request | Address is chosen by this runtime; matching-console allocation order is unverified |
| Actual commitment | The next original ordinal-184 call commits 4,096 bytes at the same base | One page is committed read/write; the other 255 remain reserved and inaccessible |
| Original initialization uses that memory | Original code reaches ordinal 291 with `0x00330610`; the unsignaled synchronization event, self-links, lock count -1, zero recursion and zero owner are verified afterward | This proves initialization on this path, not contended waiting or complete locking |
| Startup advances | Twelve completed kernel operations; 649 successful checked accesses before the stop, with no original boot memory fault | Seventeen functions / 977 instructions remain translated; fourteen distinct original routines enter, with 25 trace records. These counts are not full executed-instruction coverage |
| Protection is enforced | Separate owned browser diagnostics trap before a reserved-page read and a read-only header write, recording exact address/width/access | Intentional diagnostic faults are reported separately from the fault-free original boot snapshot; no exception recovery is claimed |
| Earlier behavior remains | The 22-, 20- and 16-check modules pass through the current host, including the no-import regression | Their prior scope/fidelity limits remain |

The real six-hour timer remains pending, with no original callback delivery. Synthetic flags 0/1 still produce corresponding callback state words; original-console settings and shutdown/clock/suspend fidelity remain unknown. Missing settings, malformed Wasm and missing ABI-5 exports are rejected; Clear empties all selections/output, and recovery passes. Final browser logs contain no warnings/errors. The temporary selected tabs and verified source-only hosts were cleared/closed.

## Contracts, implementation and limits

The pinned [nxdk public ABI](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L3095) establishes the five arguments and allocation/protection flags. Pinned Cxbx [virtual-memory implementation](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/memory-manager/VMManager.cpp) supports the reserve/commit distinction, 64 KiB reservation alignment, 4 KiB page accounting, overlapping commits, decommit and partial release. Its [RTL initializer](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlRtl.cpp) and [event initializer](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlKe.cpp) corroborate the critical-section prefix. These are public compatibility references, not observations of a matching original kernel. New implementation is independently authored; no original pseudocode is published.

`virtual_memory.c` owns bounded regions and per-page ownership, commitment and access permissions. Reservation does not zero or expose pages; new commitment zeros pages while recommit preserves existing bytes. Owned cases cover unaligned page-spanning commit, implicit reservation, read-only/no-access transitions, decommit/recommit, partial release and region splitting, range/ownership/capacity errors and unchanged state on failure. Image and private boot/TLS/stack/timer/diagnostic ranges cannot be released through the guest allocation API. Protection/free are component-tested; the title proof only invokes allocation and lock initialization.

ABI 5 registers image ownership and section permissions from the compatible XBE, plus the existing private runtime arena. Sections sharing a page contribute their required permissions. This mapping policy has not been compared with the matching Xbox loader. ABI 5 retains exactly the same sole bounded clock import as ABI 4, fixed unshared 128 MiB Wasm memory and 64 MiB guest backing. Those are proof bounds, not a complete Xbox address-space model or a release memory/performance acceptance.

The source-owned `--guard-memory` adapter instruments recognized emitted C accesses without replacing original instructions. Its static census covers 287 scalar reads, 152 writes, 261 stack operations and 21 bulk ranges across all seventeen functions. Packed scalar lvalues preserve unaligned access; native bulk paths check their complete range and mode before obtaining a pointer. Function entry/exit and kernel frames also validate required code/stack ranges. Unknown raw pointers, escaping lvalues, unsupported widths/helpers or malformed surfaces fail admission. The pinned vendor checkout remains unchanged. This is a conservative adapter for the admitted emitter surfaces, not a complete C parser or all-instruction runtime.

The lower 64 MiB VA window remains a material limit. Broader virtual mappings, physical aliases/MMIO, exact fault ordering/partial string effects, recoverable exception dispatch and full Xbox memory semantics remain unimplemented. Valid but unimplemented guard/cache/copy modes, zero-bit constraints, top-down/no-zero requests and unsupported alignment/output-overlap cases stop explicitly rather than manufacturing success or an incorrect guest error. Do not treat a request outside this window as proof the title legitimately ran out of memory. Full kernel behavior must be extended before affected title paths can be accepted.

## Evidence and verification

| Local-only artifact | SHA-256 |
| --- | --- |
| `research/verification/recomp-title-memory-01/generation.json` | `cb1f8b2f4a46330de6500d649e04ac3a5ad40e750ad437ad8230d329cbde0870` |
| `research/verification/recomp-memory-build-05/build.json` | `1b6cf775b918c9674c5835a4f07815e6bd44575768ab8737eee78574bd408e3b` |
| Matching 115,163-byte `startup.wasm` | `3bcff92fd95ad306a0600e494803ed737541b2b27ea892c251a4ef510044eb74` |
| `research/verification/virtual-memory-05/checks.json` | `cb08cda743b62320c02160b5ca0a53c6a196cc72641aa83b74e38a9cc6e3974c` |
| `research/verification/memory-startup-browser-20261101-15.json` (synthetic 0) | `5da55b98f3ebe5848c1832ce2c00ea3f18d4fed766776d1b166e8537813be170` |
| Matching `16.json` (synthetic 1, empty warning/error log) | `2107243fb5c1a6511f03bff5ea7646e1bbc46f97288b2e48f30d89be17b8a273` |
| Matching `08.json` (rejection/Clear, earlier build) | `1efb85a0eb84adb41db789fa717b10736fe59e4295d4d062c52031fd89d2c74c` |
| Matching final `17.jpg` | `4a635f5270f81eca77d1d4c2325292ca085066bfc117a84e45f15d62245acfe2` |
| Matching final `17.json` (Clear) | `4adb1db82ac8db5729c5d05f9599cb7f25526dc8425ba9f80193641c4008c6c8` |
| `research/verification/recomp-heap-next-01/seed.json` | `0b6eec36963fcef0159537ea59198abbeadf515d9361da08f371e5a985d94e32` |

`memory-checkpoint-audit-02.json` verifies all sixteen compiled owned source/header/declaration hashes, ten owned fixture identities, generated-output hashes, original XBE identity and clean pinned vendor source. Ownership, transformed-access and critical-section fixtures each pass native optimized, address/undefined-behavior sanitizer and Wasm runs: nine compile/run combinations. The transformed fixture is independently authored C and contains no original instructions; only owned code executes under native/SDK Node. Fourteen existing Python admission tests plus three memory-emitter cases and twelve runtime/admission/delivery cases pass. The preparation/publication guard passes for 157 source files and whitespace checks pass. CI adds the owned generator and C memory checks without game files or private tools; publication results are recorded separately.

Earlier module builds `01/02` retain the allocation/critical-section bring-up boundaries; `03` records the first passing memory proof. Build `04` tightens unsupported protection-mode handling. Final build `05` also preserves zero-count string no-ops with unused out-of-window registers, using harmless in-array host pointers; owned native/sanitizer/Wasm tests and both original-browser profiles pass again. Earlier owned attempts, all browser records and screenshots remain preserved. The final browser screenshot is `memory-startup-browser-20261101-17.jpg`; final Clear is `17.json`.

## Next concrete task

Read-only reuse of the existing project exports established helper `0x00031da0`: 286 contiguous bytes / 107 instructions, one body range, no data/padding or computed jumps. Its calls comprise the already-declared ordinal-184 allocation slot and local routines `0x0003139a` and `0x0003185d`. This is static evidence only; the helper has not executed. Reuse `recomp-heap-next-01/seed.json` as the eighth seed, preserve all byte/decoder/guard checks, translate the actual helper and observe its real heap state/calling behavior in the browser. Stop at genuinely unsupported callees or memory variants; do not guess heap rules or bypass the memory checker.

The complete original game, rendering/audio/input, general scheduling/exceptions, original-reference fidelity, browser-local end-user conversion, all custom features and dedicated server authority remain open in the [full product ledger](ORIGINAL_EXECUTION_PROGRESS.md). No requirement or route is reduced by this checkpoint.
