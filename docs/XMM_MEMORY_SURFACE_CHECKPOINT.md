# Checked XMM memory surface admits the matrix helper; execution reaches 0x1FCA20

November 4, 2026. The guard-memory emitter now supports checked 16-byte XMM loads and stores and allows pure SSE arithmetic. This admitted the raw matrix helper at `0x1FA460`; the recomposed translation built and the adopted profile advanced through `0x1FB3B0` and `0x1FB400` to `0x1FCA20`, with 267 distinct original routines. The no-profile path still stops at status 13. **There is no playable screen, renderer or device model.**

**Address correction.** Earlier notes used `0x1FB5B0` and `0x1FB600`; those were decimal-to-hex slips. The actual stops were `2077616 = 0x1FB3B0` (`XMETAL_StartPush`) and `2077696 = 0x1FB400` (`FUN_001FB400`), with return `0x1FB4FD` and `0x1FBEF5` respectively. The current stop is `2083360 = 0x1FCA20` (`FUN_001FCA20`, a small switch helper) with return `0x1FB76A`.

## Source-owned change

- `src/runtime/recomp_memory_access.h` adds a packed `RecompXmm` guest value and `FF_MEMORY_XMM` / `FF_MEMORY_SITE_XMM`. The accessor checks a 16-byte range through `ff_probe_guest_memory` like the other scalar surfaces.
- `tools/recomp_memory_access.py`:
  - recognizes `XMM_MEM` and `XMM_LOAD` as checked 16-byte reads;
  - rewrites `XMM_STORE(address, value)` to a checked 16-byte store with a basic-block site;
  - still rejects escaped pointers, `XBOX_PTR`, unknown vector memory helpers and malformed surfaces;
  - allows pure SSE helpers (`XMM_MUL`, `XMM_ADD`, `XMM_SHUFFLE`, etc.) because they do not touch guest memory.
- `tests/recomp_memory_access_test.py` covers checked XMM loads, checked stores, pure SSE arithmetic and the remaining rejection cases.

## Recompose and runs

- First guarded recomposition: 395 functions / 16,092 instructions; module `recomp-post-memory-build-61` (587,595 bytes, SHA-256 `6d64dd23…`); profile run status 2 at `0x1FB400` (return `0x1FBEF5`), 263 routines.
- Closure for `0x1FB400`: 3 functions / 150 instructions; second guarded recomposition: 398 functions / 16,242 instructions.
- Private module `recomp-post-memory-build-62`: 593,096 bytes, SHA-256 `a9ce85ef6bf409de774de24d6f62d398d6d78cd819b993895dea82229d1ba78f`.
- With `settings-pfifo.json`: status 2 at `0x1FCA20` (return `0x1FB76A`), **267** distinct routines, 83/83 initializers, 402 kernel operations.
- Without the profile: status 13 at `0xFD800044`, 261 routines, 402 operations.

## Interpretation

- The XMM surface is a checked memory admission, not a bypass: the matrix helper's vector loads and stores go through the same guest-memory probe and site accounting as scalar accesses.
- `0x1FB3B0` is `XMETAL_StartPush`; `0x1FB400` is `FUN_001FB400`, which builds command words through `XMETAL_StartPush` and calls the shader-constant-mode setter. Neither is a loop body inside `FUN_001FB580`.
- The next stop, `0x1FCA20`, is an established small switch helper (`FUN_001FCA20`). It can be exported directly; no function split is needed.
- The default source tree still has the prior 366-function translation; builds 61, 62 and earlier modules are private generated modules. The source-owned XMM surface and tests are published.

## Limits

No rendering, audio, fidelity or playability is established. The translation and modules are private; only the emitter/header/test changes are published. The XMM surface admits 16-byte accesses only and does not model general vector aliasing or device memory.

## Validation

`tests/recomp_memory_access_test.py` 11/11; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass. Both modules compiled with `--guard-memory` and ran through the existing worker.

## Next task

Export the established `FUN_001FCA20` closure, recompose with `--guard-memory`, build and rerun the adopted profile. Do not bypass guard-memory or claim rendering/fidelity without evidence.
