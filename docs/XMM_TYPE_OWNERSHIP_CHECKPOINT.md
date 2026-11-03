# XMM type ownership repaired; fail-closed helper admission and owned boundary test

November 4, 2026. The checked 16-byte XMM path no longer owns the pinned `RecompXmm` type. The memory header now exposes only the checked pointer, and the emitter casts it to `RecompXmm` at the generated use. Helper admission is an explicit allowlist of pure SSE operations; unknown vector helpers and memory-touching helpers that are not rewritten fail closed. Nested probing stores are evaluated into a temporary before the checked store. The refactored translation builds and reproduces the same execution stop. **There is no playable screen, renderer or device model.**

## Directive check

The architect directive asked to establish `0x1FB600`. Current verified evidence does not support that address: `0x1FB600` was a decimal-to-hex slip; the actual progression is `0x1FB3B0`, `0x1FB400`, `0x1FCA20`, and the current stop is `0x1FCB30`. The route is unchanged; this milestone repairs the XMM admission contract before further translation and records the address conflict.

## Source-owned changes

- `src/runtime/recomp_memory_access.h` no longer defines an `XMM` guest type or `FF_MEMORY_XMM`/`FF_MEMORY_SITE_XMM`. The header owns only the checked `FF_MEMORY_SITE_POINTER` range contract; the generated code owns the `RecompXmm` cast.
- `tools/recomp_memory_access.py`:
  - emits `*(RecompXmm*)FF_MEMORY_SITE_POINTER(address, 16u, access, site)` for `XMM_MEM`/`XMM_LOAD` and `XMM_STORE`;
  - splits an `XMM_STORE` whose value probes memory into a temporary before the checked store;
  - admits only the explicit `PURE_XMM` allowlist (`XMM_ADD`, `XMM_MUL`, `XMM_SHUFFLE`, ...);
  - rejects `XMM_MEM`/`XMM_LOAD`/`XMM_STORE` left unrewritten, `XMM_LOAD_LOW`/`XMM_STORE_LOW`, unknown `XMM_*` helpers, escaped pointers, `XBOX_PTR`, MMX/atomic helpers and malformed surfaces.
- `tests/recomp_memory_access_test.py` covers checked loads, checked stores, nested probing-store splitting, pure SSE arithmetic and the fail-closed cases.
- `tests/recomp_memory_access_xmm_test.c` includes only the source-owned memory header (no `RecompXmm`), stubs the guest probe and checks 16-byte permission, size, alignment, site and bounds.

## Rebuild and run

- Guarded recomposition with the repaired emitter: 399 functions / 16,266 instructions.
- Private module `recomp-post-memory-build-64`: 595,242 bytes, SHA-256 `e6a2bbddd13829f94cd9e56a64d45de66adc36b6449ea598caf58d5570933830`.
- Profile run: status 2 at `0x1FCB30` (return `0x1FB835`), **268** distinct routines, 83/83 initializers, 402 kernel operations.
- No-profile run: status 13 at `0xFD800044`, 261 routines, 402 operations.
- The stop is unchanged from build-63, so the repair did not change observed execution.

## Limits

No rendering, audio, fidelity or playability is established. The XMM surface admits 16-byte accesses only; it does not model general vector aliasing, half-vector helpers or device memory. The translation and modules are private; only the source-owned header, emitter and tests are published.

## Validation

`tests/recomp_memory_access_test.py` 11/11; `tests/recomp_memory_access_xmm_test.c` optimized and ASan/UBSan; runtime suite 110/110; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Establish the actual reached helper `0x1FCB30` from exact evidence (not `0x1FB600`), export its closure, recompose with `--guard-memory`, build and rerun. Do not bypass guard-memory or claim rendering/fidelity without evidence.
