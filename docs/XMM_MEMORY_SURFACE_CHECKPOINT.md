# Checked XMM memory surface admits the matrix helper; execution reaches 0x1FB600

November 4, 2026. The guard-memory emitter now supports checked 16-byte XMM loads and stores and allows pure SSE arithmetic. This admitted the raw matrix helper at `0x1FA460`; the recomposed translation builds and the adopted profile advances past `0x1FB5B0` to `0x1FB600`, with 263 distinct original routines. The no-profile path still stops at status 13. **There is no playable screen, renderer or device model.**

## Source-owned change

- `src/runtime/recomp_memory_access.h` adds a packed `RecompXmm` guest value and `FF_MEMORY_XMM` / `FF_MEMORY_SITE_XMM`. The accessor checks a 16-byte range through `ff_probe_guest_memory` like the other scalar surfaces.
- `tools/recomp_memory_access.py`:
  - recognizes `XMM_MEM` and `XMM_LOAD` as checked 16-byte reads;
  - rewrites `XMM_STORE(address, value)` to a checked 16-byte store with a basic-block site;
  - still rejects escaped pointers, `XBOX_PTR`, unknown vector memory helpers and malformed surfaces;
  - allows pure SSE helpers (`XMM_MUL`, `XMM_ADD`, `XMM_SHUFFLE`, etc.) because they do not touch guest memory.
- `tests/recomp_memory_access_test.py` covers checked XMM loads, checked stores, pure SSE arithmetic and the remaining rejection cases.

## Recompose and run

- `prepare-title-recomp.py --guard-memory` now succeeds: 395 functions / 16,092 instructions.
- Private module `recomp-post-memory-build-61`: 587,595 bytes, SHA-256 `6d64dd235f8506975f6dc2b9e5ac110cab8943c974087857883ca66d6942d45a`.
- With `settings-pfifo.json`: status 2 at `0x1FB600` (return `0x1FBF35`), **263** distinct routines, 83/83 initializers, 402 kernel operations.
- Without the profile: status 13 at `0xFD800044`, 261 routines, 402 operations.

## Interpretation

- The XMM surface is a checked memory admission, not a bypass: the matrix helper's vector loads and stores go through the same guest-memory probe and site accounting as scalar accesses.
- `0x1FB600` is an internal callee that Ghidra merged into `FUN_001FB580`; it is not established as a separate function, so the translation stops there. The next step is to establish/split `0x1FB600`, export it, and continue.
- The default source tree still has the prior 366-function translation; build-61 is a private generated module. Builds 60, 58 and 51 remain preserved.

## Limits

No rendering, audio, fidelity or playability is established. The translation and module are private; only the emitter/header/test changes are published. The XMM surface admits 16-byte accesses only and does not model general vector aliasing or device memory.

## Validation

`tests/recomp_memory_access_test.py` 11/11; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass. The build-61 module compiled with `--guard-memory` and ran through the existing worker.

## Next task

Establish `0x1FB600` as a separate function in the read-only project, export its closure, recompose with `--guard-memory`, build and rerun the adopted profile. Do not bypass guard-memory or claim rendering/fidelity without evidence.
