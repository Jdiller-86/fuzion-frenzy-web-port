# Original execution claims GPU instance memory and reaches 0x1FDF70

November 3, 2026. The reached `MmClaimGpuInstanceMemory` request now receives a bounded source-owned result, the original 0x5000-byte device clear executes, and `0x001ffe27` continues to untranslated `0x001fdf70`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-12` (local/ignored) |
| Functions / instructions | 348 / 13,185 |
| Module | `research/verification/recomp-post-memory-build-25/startup.wasm` (local/ignored) |
| Module bytes | 466,517 |
| Module SHA-256 | `ee7c9a2e98bf4698eddb094196a0b54b9ecfbe9e3817f8945032e56dcc3a9956` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-07.json` (local/ignored) |

The translation and seed selection are unchanged from the preceding run. The module adds the source-owned `gpu_instance.c` service and expands the synthetic aperture capacity to 8,192 entries / 8,192 events so the observed 0x5000-byte clear is recorded rather than silently truncated. ABI 24 and the sole `env.ff_host_clock` import remain.

## Observed browser execution

The explicit synthetic profile now also supplies zero as the test value for `0xFD100214`; the earlier `0xFD100200`, `0xFD100204` and `0xFD001218` entries remain. These are synthetic test inputs, not console defaults.

Results:

- All **83/83** initialization callbacks returned.
- **238** distinct original routines executed.
- **400** kernel operations completed.
- Ordinal 168 `MmClaimGpuInstanceMemory` returned **status 0** with base `0x03B78000` (62,357,504) and wrote zero to the checked padding output.
- The original helper cleared 0x5000 bytes at `0xFD700000`–`0xFD704FFC` through 5,120 dword stores.
- Device aperture: **5,142 entries** and **5,152 events**; the clear's stores are attributed to site `2096891`.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 2, untranslated original routine **`0x1FDF70`**, return `0x1FFF1A`, ordinal 0.

## Source-owned GPU instance claim

`src/runtime/gpu_instance.c`/`.h` implements:

`uint32_t ff_claim_gpu_instance_memory(ff_virtual_memory *, uint32_t bytes, uint32_t padding, uint32_t lowest, uint32_t highest, uint32_t *base)`

The pinned public bridge returns the top of its 64 MiB contiguous window at `0x80000000` minus the request and writes zero to the padding output. This runtime has a different explicit window, so the service claims the topmost free span through `ff_vm_allocate_contiguous` instead of copying the bridge address. `MAXULONG_PTR` selects the public 128 KiB default. Padding is validated before allocation, so a bad output pointer cannot leave a committed region behind.

`tests/gpu_instance_test.c` covers explicit and default-size claims, page alignment, padding zeroing, read/write admission, unaligned/read-only/out-of-range padding rejection, zero/huge sizes, and atomic no-memory failure. Optimized and ASan/UBSan runs pass.

The aperture capacity change is recorded in `device_registers.h` and `device-report.mjs`; the report retains every entry and event.

## Next dependency

A bounded read-only export reports `0x001fdf70` as an established **19-byte / 6-instruction** function with **no direct calls** (`recomp-post-memory-next-09/seed.json`, local/ignored). Its body loads its stack argument, adds it to `[ECX+0x1A0]`, stores the result and returns with `RET 0x4`. This is static export evidence, not admitted translation or observed execution.

## Prior milestone: MmClaimGpuInstanceMemory request

The preceding run translated 348 functions / 13,185 instructions in `recomp-title-post-memory-12` and built `recomp-post-memory-build-24` (465,978 bytes, SHA-256 `67e7c9a5a560db00e8f3984e7561ed512555b5168f116dd6fb3ddcc8e390f72d`). It completed 83/83 callbacks, executed 238 routines and 399 kernel operations, read three explicit synthetic device values, and stopped at ordinal 168 with a 0x5000-byte request and padding pointer. Report `post-memory-browser-20261103-06.json`.

## Prior milestone: 0x001ffcc0

`0x001ffcc0` executed to completion and returned to `0x001fe0c6`, which called `0x1FFE27`. Translation `recomp-title-post-memory-11` was 347 functions / 13,113 instructions; module `recomp-post-memory-build-23` was 464,158 bytes, SHA-256 `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d`. Report `post-memory-browser-20261103-05.json`.

## Prior milestone: 0x001fe0c6

`0x001fe0c6` executed its prologue and two stores and called `0x1FFCC0`. Translation `recomp-title-post-memory-10` was 346 functions / 12,997 instructions; module `recomp-post-memory-build-22` was 459,451 bytes, SHA-256 `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f`. Report `post-memory-browser-20261103-03.json`.

## Prior milestone: 0x001fe085 and interrupt initialization

`0x001fe085` returned to `0x1FE5CE`; servers 44/109/98/47 were modeled from the pinned public bridge without delivering interrupts. Translation `recomp-title-post-memory-09` was 345 functions / 12,892 instructions; module `recomp-post-memory-build-21` was 459,080 bytes, SHA-256 `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9`. Report `post-memory-browser-20261103-02.json`.

## Limits and next task

`0x1FDF70` is untranslated. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The GPU instance claim, device reads/writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: admit and translate `0x1FDF70` from the retained read-only export, then follow its next genuine dependency. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
