# A001 closed with a compiled real-VM vector test; acknowledgement stays experimental with a control

November 4, 2026. Generated vector operations are now structurally checked by the emitter and exercised by a compiled test against the real virtual-memory component, and the write-1-to-clear register model is opt-in experiment with a control run. The canonical profile keeps `NV_PMC_INTR_0` fail-closed. **There is no controllable original screen, renderer, engine progress or fidelity result.**

## A blanket alignment guard was tried and rejected

The first attempt enforced 16-byte alignment for every 16-byte guest access in the probe hook. The real run rejected it: a legitimate 16-byte access at `0x3C10D04` (four-byte aligned) occurs in the reached code, and the run stopped at status 10 with only **233** routines and a 3,631-event trace. That access comes from a move that does not require alignment, so a width-based rule was wrong.

## What the emitter and header now do

- `src/runtime/recomp_memory_access.h` adds `FF_MEMORY_SITE_VECTOR(address, access, site)`: the checked 16-byte access plus a 16-byte alignment guard. The plain `FF_MEMORY_SITE_POINTER` form keeps serving unaligned-tolerant moves.
- `tools/recomp_memory_access.py` reads the translator's trailing mnemonic comment and emits the alignment-guarded form only for moves that require it (`movaps`, `movapd`, `movdqa`, `movdqa64`, `movntps`, `movntpd`, `movntdq`); `movups`/`movupd`/`movdqu`/`lddqu` and comment-free snippets keep the plain form. Read and store paths both apply, and the access counts now record `aligned` and `unaligned` separately.
- Existing structural rejections are unchanged: a vector load may not be a store target or an escaped pointer, and helper calls must name an exact-case `PURE_XMM` entry.
- The change takes effect for the reached code only on recomposition. The current `title-functions.inc` predates it: its 32 vector memory sites carry `movaps` (16) and `mulps` (16) comments, so a regenerated translation would emit 16 alignment-guarded reads.

## Compiled real-VM test (new)

`tests/recomp_memory_access_vm_test.c` compiles the exact emitted form
`(*(RecompXmm*)FF_MEMORY_SITE_POINTER((address), 16u, access, site))` and resolves `ff_probe_guest_memory` through the real `src/runtime/virtual_memory.c` (`ff_vm_init` / `ff_vm_claim_fixed` / `ff_vm_map_fixed` / `ff_vm_check`) rather than a stub:

- a 16-byte read inside a `PAGE_READONLY` region returns the region's bytes, with address, size, access and site recorded;
- a 16-byte store into that region is refused, and the bytes are verified unchanged;
- `FF_MEMORY_SITE_VECTOR` refuses a misaligned address **without calling the host**, serves the aligned address, and refuses the store form on a read-only region;
- a move spanning a page boundary inside a mapped region resolves, while spanning into an unmapped page, an unmapped address, an address outside the window and an invalid access mask all fail closed;
- read-modify-write is refused where a plain read succeeds.

It passes optimized and under ASan/UBSan (`gcc -O2/-O1 -std=c11 -Wall -Wextra -Werror` with `tests/recomp_memory_access_vm_test.c src/runtime/virtual_memory.c src/runtime/thread_bootstrap.c`, the ASan run adding `-fsanitize=address,undefined`). The Python suite also pins the emission shape: 12/12, including aligned load, aligned store, `movups` and comment-free snippets.

## Experimental acknowledgement with a control

`parseLocalMmioProfile` now reports `device.writeClearing`, the worker sets it through `ff_probe_mmio_write_clearing` before admitting the window, and the fixture declares `0xFD400100`/`0xFD002100` write-clearing only when the profile opts in. The private build exports the new function and rebuilt to **640,504 bytes**, SHA-256 `e1b42b3431ef179d870b555334179e8227ec452fe6950d38c4510cd69dce30e5`.

Two runs over the same module, differing only in that flag:

| profile | `writeClearing` | stop | routines | trace |
| --- | --- | --- | --- | --- |
| `settings-pfifo.json` (canonical) | `true` | status 10 (`guest memory access stopped`) on an unmodeled read of `0xFD000100` | 276 | 3,839 |
| `settings-pfifo-control.json` (control) | absent | status 5 (`diagnostic capacity reached`) at `0x200F59`, return `0x1FDB20` | 277 | 8,192 |

Both report 83/83 initializers, 403 kernel operations and `timerCheck = 0`. The control reproduces the pre-model enter-only `0x200F59`/`0x1FDAC0` recurrence, which is the required control evidence: the acknowledge behaviour comes from the explicit experiment flag, not from ambient state.

## PMC stays fail-closed

The canonical profile no longer serves `NV_PMC_INTR_0`; the `0xFD000100` zero range was removed and the run stops on that unmodeled read. The earlier "PFIFO engine-state wait" observation (1,841 handler entries, `NV_PFIFO_CACHE1_PULL0` stores, the 32,768-event log filling) was obtained only while that status read was served. It is retained as conditional measurement, not as the canonical result, and capacity exhaustion is not engine progress.

## Limits

The acknowledge rule is guest-implied and unverified against hardware. `NV_PMC_INTR_0` behaviour is undecided, no interrupt is delivered, no command stream is consumed and no engine state exists. The emitter change is not yet present in the reached translation. No rendering, audio, fidelity, compatibility or playability is established; all private profile/report evidence stays local.

## Validation

Compiled real-VM vector test and device-register test optimized and ASan/UBSan; kernel-timer, port and 16-byte checked-pointer tests optimized; Python memory-access 12/12; title-seed 25/25; runtime suite 110/110; `npm run check` 299 source files; whitespace checks pass.

## Next task

Regenerate the reached translation so the alignment-guarded vector form reaches the original code, then continue the fail-closed `NV_PMC_INTR_0` decision with evidence. Do not seed completion bits, grow trace/event capacity to escape waits, or claim engine progress from a capacity stop.
