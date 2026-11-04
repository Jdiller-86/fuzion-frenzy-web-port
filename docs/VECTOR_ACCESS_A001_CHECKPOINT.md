# A001 closed: generated vector ops are non-writable, and denied ops stop before mutation in compiled code

November 4, 2026. The A001 vector surface is closed on the exact evidence the review asked for: a generated read is a non-writable `const` lvalue and assigning through it fails to compile; the address-of/escape form `&XMM_MEM(...)` is rejected by the emitter and covered by a test; and **generated** denied operations are now executed through a stopping host hook and proven to halt before any guest mutation. Alignment is handled per mnemonic, the reached translation uses the guarded forms and behaves identically, and the write-1-to-clear register model remains an opt-in experiment with a control. The canonical profile keeps `NV_PMC_INTR_0` fail-closed. **There is no controllable original screen, renderer, engine progress or fidelity result.**

## Denied generated operations executed through a stopping hook

`tests/recomp_memory_access_codegen_test.py` now compiles a harness whose `ff_probe_guest_memory`/`ff_probe_guest_vector` **stop** (longjmp, standing in for the fixture's `stop()`) instead of returning a pointer the translated code would dereference. The harness executes the emitted expressions rather than inspecting them:

- a generated vector read of a mapped read-only page returns its bytes;
- a generated vector store into a writable page writes them;
- a generated vector **store** into the read-only page reaches the stop hook exactly once with the address, access `2` and the emitted site, and the bytes are verified unchanged (denial before mutation);
- a generated vector **read** of an unmapped page reaches the same stop with access `1`.

Both denial cases are compiled and executed, and the Python test asserts the harness output. The earlier `&XMM_MEM(...)`/`XMM_MEM(...) = ...` rejections remain committed and tested in `tests/recomp_memory_access_test.py`.


## Generated reads are structurally non-writable

Alignment alone did not close the permission hole: a read was emitted as the mutable lvalue `(*(RecompXmm*)FF_MEMORY_SITE_POINTER(...))`, so nothing stopped a later edit from storing through a read instruction. Reads are now emitted as `(*(const RecompXmm*)...)` for both the plain and the alignment-guarded forms; stores keep the mutable lvalue. Assigning through a generated read is therefore a compile error rather than a silent guest-memory mutation, and there is no runtime cost.

`tests/recomp_memory_access_codegen_test.py` is the generator-to-compiled regression: it runs `instrument_memory_accesses`, writes the emitted C into a temporary harness, compiles it against the real `src/runtime/virtual_memory.c`, and checks

- a store through the generated read expression **fails to compile** with a read-only diagnostic (the structural guarantee, proven by the compiler rather than by inspection);
- a permitted read returns the guest bytes and a permitted store writes them through the emitted form;
- a denied store is refused with the bytes verified unchanged (denial before mutation);
- denied reads (unmapped page, outside the window, misaligned alignment-requiring move) fail closed.

It runs under the available compiler (verified with gcc 13.3 in WSL and skips with an explicit message when no compiler is present).

The reached translation was regenerated with this emitter (`recomp-title-recomp-title-post-memory-45`, 415 functions / 17,284 instructions): **16 alignment-guarded const reads, 16 plain const reads and 4 mutable alignment-guarded stores**. The module built from it (`recomp-post-memory-build-70`) is 641,451 bytes, SHA-256 `f46c5444b90c749f49bda9bca9c5330c815c5638376f57a598b24a2d33f1aee3` — byte-identical to the previous build, which is expected because `const` is a compile-time qualifier with no codegen effect; unlike the folded ternary, nothing was removed.

## A blanket alignment guard was tried and rejected

The first attempt enforced 16-byte alignment for every 16-byte guest access in the probe hook. The real run rejected it: a legitimate 16-byte access at `0x3C10D04` (four-byte aligned) occurs in the reached code, and the run stopped at status 10 with only **233** routines and a 3,631-event trace. That access comes from a move that does not require alignment, so a width-based rule was wrong.

## The guard must live in the host, not in the macro

The second attempt put the check in the macro as `((address & 15) ? NULL : ff_probe_guest_memory(...))`. The regenerated module came out **byte-identical** to the previous one, because the generated code immediately dereferences the result: null-dereference is undefined, so the optimiser eliminated the null branch and the guard vanished.

The contract is therefore a host hook:

- `src/runtime/recomp_memory_access.h`: `void *ff_probe_guest_vector(uint32_t address, uint32_t access, uint32_t site);` and `FF_MEMORY_SITE_VECTOR(address, access, site)` expands to that call. There is no nullable value for the optimiser to fold.
- `tools/fixtures/thread_startup_probe.c`: the hook stops the proof with status 10 on a misaligned address and otherwise forwards to `ff_probe_guest_memory(address, 16u, access, site)`, so a misaligned alignment-requiring move behaves like the guest #GP instead of being served as an unaligned copy.
- `tests/recomp_memory_access_vm_test.c` implements the same hook and unwinds with `setjmp`/`longjmp`, so the test observes the fault (address, access, site, and no host call) without ever dereferencing the misaligned result.

## Emitter rules

`tools/recomp_memory_access.py` reads the translator's trailing mnemonic comment and emits `FF_MEMORY_SITE_VECTOR` only for moves that require alignment (`movaps`, `movapd`, `movdqa`, `movdqa64`, `movntps`, `movntpd`, `movntdq`); `movups`/`movupd`/`movdqu`/`lddqu` and comment-free snippets keep `FF_MEMORY_SITE_POINTER`. Both read and store paths apply, and access counts record `aligned`/`unaligned`. A vector load may still not be a store target or an escaped pointer, and helper calls must name an exact-case `PURE_XMM` entry.

## Regenerated translation

All 32 seeds were recomposed with the current guarded emitter into `recomp-title-recomp-title-post-memory-44` (`--function-limit 512 --instruction-limit 32768 --guard-memory`): **415 functions, 17,284 instructions**, matching the previous composition. The generated code now contains:

| shape | form | count |
| --- | --- | --- |
| `movaps` load | `FF_MEMORY_SITE_VECTOR`, access 1 | 16 |
| `movaps` store | `FF_MEMORY_SITE_VECTOR`, access 2 | 4 |
| `mulps` load | `FF_MEMORY_SITE_POINTER`, access 1 | 16 |

No unguarded `movaps` site remains. The module built from this translation is `recomp-post-memory-build-69`: **641,341 bytes**, SHA-256 `bf9e9e2d94738890a974aca251dd8223bfad86f9090ebb9fa5097641757a6309`; the previous translation rebuilt from the same source is `532c082bbeb7a26e770935c63bdd7d0f4f5085f880760947dfcb7de3f80881f6`, so the two modules are genuinely different builds.

Matched runs over the same source and profiles:

| module | profile | stop | routines | ops | init | trace | `timerCheck` |
| --- | --- | --- | --- | --- | --- | --- | --- |
| build-68 (translation 43) | canonical | status 10 at `0xFD000100` (read) | 276 | 403 | 83/83 | 3,839 | 0 |
| build-69 (translation 44, guarded) | canonical | status 10 at `0xFD000100` (read) | 276 | 403 | 83/83 | 3,839 | 0 |
| build-69 | control (no `writeClearing`) | status 5 at `0x200F59` | 277 | 403 | 83/83 | 8,192 | 0 |

The guarded form is behaviour-preserving on the reached path because no `movaps` there executes misaligned; a misaligned one would now stop the proof instead of quietly succeeding.

## Compiled real-VM test

`tests/recomp_memory_access_vm_test.c` compiles the exact emitted forms and resolves `ff_probe_guest_memory` through the real `src/runtime/virtual_memory.c` (`ff_vm_init` / `ff_vm_claim_fixed` / `ff_vm_map_fixed` / `ff_vm_check`):

- a 16-byte read inside a `PAGE_READONLY` region returns the region's bytes, with address, size, access and site recorded;
- a 16-byte store into that region is refused, and the bytes are verified unchanged;
- the alignment hook faults on a misaligned address without calling the host, reports address/access/site, and serves the aligned address;
- a move spanning a page boundary inside a mapped region resolves, while spanning into an unmapped page, an unmapped address, an address outside the window and an invalid access mask all fail closed;
- read-modify-write is refused where a plain read succeeds, and an unaligned-tolerant move still works at a four-byte offset.

It passes optimized and under ASan/UBSan. The Python suite pins the emission shape: 12/12, including aligned load, aligned store, `movups` and comment-free snippets.

## Experimental acknowledgement with a control

`parseLocalMmioProfile` reports `device.writeClearing`; the worker sets it through `ff_probe_mmio_write_clearing` before admitting the window; the fixture declares `0xFD400100`/`0xFD002100` write-clearing only when the profile opts in. Over the same module (640,504 bytes, SHA-256 `e1b42b34…`) the canonical profile with the flag stops at status 10 on an unmodeled read of `0xFD000100` (276 routines, 3,839 trace events), while the control profile without it reproduces the pre-model `0x200F59`/`0x1FDAC0` recurrence (277 routines, 8,192 events). The control is the required evidence that the acknowledge behaviour comes from the explicit flag, not ambient state.

## PMC stays fail-closed

The canonical profile no longer serves `NV_PMC_INTR_0`; the `0xFD000100` zero range was removed and the run stops on that unmodeled read. The earlier "PFIFO engine-state wait" observation (1,841 handler entries, `NV_PFIFO_CACHE1_PULL0` stores, the 32,768-event log filling) was obtained only while that status read was served. It is retained as conditional measurement, not as the canonical result, and capacity exhaustion is not engine progress.

## Limits

The acknowledge rule is guest-implied and unverified against hardware. `NV_PMC_INTR_0` behaviour is undecided, no interrupt is delivered, no command stream is consumed and no engine state exists. No rendering, audio, fidelity, compatibility or playability is established; all private profile/report evidence stays local.

## Validation

Device-register, compiled real-VM vector, 16-byte checked-pointer, kernel-timer and port tests pass optimized (the real-VM test also under ASan/UBSan); Python memory-access 12/12; generator-to-compiled codegen regression 2/2; runtime suite 111/111; title-seed 25/25; `npm run check` 302 source files; whitespace checks pass.

## Next task

Continue the fail-closed `NV_PMC_INTR_0` decision with evidence before any engine expansion. Do not seed completion bits, grow trace/event capacity to escape waits, or claim engine progress from a capacity stop.
