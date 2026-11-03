# Original execution reaches MmClaimGpuInstanceMemory from 0x1FFE27

November 3, 2026. The previously untranslated helper at `0x001ffe27` was admitted from a retained read-only seed. The browser worker executed its three device-register reads and stopped at its first kernel call, ordinal 168 `MmClaimGpuInstanceMemory`. **There is no playable original screen and no renderer/device model.**

## Selection and build

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-12` (local/ignored) |
| Functions / instructions | 348 / 13,185 |
| Admitted helper | `0x001ffe27`, 254 body bytes / 72 instructions |
| Module | `research/verification/recomp-post-memory-build-24/startup.wasm` (local/ignored) |
| Module bytes | 465,978 |
| Module SHA-256 | `67e7c9a5a560db00e8f3984e7561ed512555b5168f116dd6fb3ddcc8e390f72d` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-06.json` (local/ignored) |

The selection reuses the retained 25-seed post-memory inputs plus `recomp-post-memory-next-08/seed.json`. The generated header retains the source digest and 348-function lookup. The module retains ABI 24 and the sole `env.ff_host_clock` import.

The read-only export reports `0x001ffe27` as a 254-byte/72-instruction established function. It has one computed kernel-slot call at `0x001FFE70` through `0x00253484` (ordinal 168), one direct call at `0x001FFF15` to `0x001FDF70`, and no other direct calls. This is static export evidence; the browser observation below is separate evidence.

## Observed browser execution

The explicit synthetic profile now also supplies zero as the test value for three newly observed register reads:

- `0xFD100200`
- `0xFD100204`
- `0xFD001218`

These are synthetic test inputs, not console defaults. The earlier `0xFD000140`, `0xFD000200`, `0xFD001800`, `0xFD001804`, `0xFD001808`, `0xFD10020C`, `0xFD600140`, `0xFD680500`, `0xFD680504` and `0xFD680508` entries remain.

Results:

- All **83/83** initialization callbacks returned.
- **238** distinct original routines executed (up from 237).
- **399** kernel operations completed; the new kernel request is not yet serviced.
- `0x001ffe27` executed through its three reads and reached `0x001FFE70`.
- Device aperture: **28 events** across **19 addresses**. The new reads were `0xFD100200`, `0xFD100204` and `0xFD001218` at site `2096679`. All are synthetic-aperture observations, not NV2A behavior.
- Memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 1, ordinal **168**, return `0x1FFE76`, target `0xFE0000AC`. The request arguments are `[0x5000, 0x03C10CF0, ...]`; the second is a guest padding-output pointer.

## Reached service: MmClaimGpuInstanceMemory

The pinned public xboxrecomp bridge identifies ordinal 168 as:

`PVOID MmClaimGpuInstanceMemory(SIZE_T NumberOfBytes, SIZE_T *Padding)`

Its modeled semantics are: `MAXULONG_PTR` selects the default 128 KiB instance size; a non-null padding output receives zero; and the return value is the top of the contiguous window minus the requested bytes. The bridge uses its own 64 MiB window at `0x80000000`; that window is not this project's modeled contiguous window (`0x03c00000`–`0x03e00000` in the bounded boot runtime), so the bridge address must not be copied blindly.

No guest result was supplied. The static path after this call stores the returned base into `[ESI+0x14]`/`[ESI+0x130]`/`[ESI+0x1a0]`, programs `0xFD002210`/`0xFD002214`, reads and rewrites `0xFD100214`, then clears 0x5000 bytes at `0xFD700000` plus the padding value. That clear is static evidence only; it has not executed. The aperture currently holds 1,024 entries and 4,096 events, so a 0x5000-byte clear needs an explicit capacity or bulk-write decision before it can be observed.

## Next dependency

No new Ghidra export is needed for the immediate task: `0x001ffe27` is already translated. The next bounded task is a source-owned runtime decision/service for `MmClaimGpuInstanceMemory` and the reached 0x5000-byte clear. It must preserve the project's explicit contiguous window and memory accounting, fail closed on unsupported sizes or padding pointers, and choose a recorded capacity/bulk-write model rather than silently dropping writes. After that, the static path reaches `0x001FDF70`, which remains untranslated.

## Prior milestone: 0x001ffcc0

The earlier run translated 347 functions / 13,113 instructions in `recomp-title-post-memory-11` and built `recomp-post-memory-build-23` (464,158 bytes, SHA-256 `76eab6bd21c7656cad623f9f50fd8d702544d3755a6a74488132e50511a2c29d`). It completed 83/83 callbacks, executed 237 distinct routines and 399 kernel operations, ran `0x001ffcc0` to completion, returned to `0x001fe0c6`, and stopped at `0x1FFE27` (return `0x1FE199`). The report is `post-memory-browser-20261103-05.json`. That run also introduced the width-aware sub-dword device-read path.

## Prior milestone: 0x001fe0c6

The earlier run translated 346 functions / 12,997 instructions in `recomp-title-post-memory-10` and built `recomp-post-memory-build-22` (459,451 bytes, SHA-256 `ab28b542f277a3a1ca2b16c74dbaaba8a18a580f9e61b04c9caf3560226d832f`). It completed 83/83 callbacks, executed 236 distinct routines and 399 kernel operations, ran `0x001fe0c6` through its prologue and two stores, and stopped at `0x1FFCC0` (return `0x1FE0E7`). The report is `post-memory-browser-20261103-03.json`.

## Prior milestone: 0x001fe085 and interrupt initialization

The earlier run translated 345 functions / 12,892 instructions in `recomp-title-post-memory-09` and built `recomp-post-memory-build-21` (459,080 bytes, SHA-256 `5fc6d100977022a55beb187be233eb1b1ce663df0fde1ef38058e22d560630a9`). It completed 83/83 callbacks, executed 235 distinct routines and 399 kernel operations, returned from `0x001fe085` to `0x1FE5CE`, supplied explicit synthetic zero values for the observed reads `0xFD001800`, `0xFD001808` and `0xFD10020C`, serviced ordinals 44/109/98/47, and stopped at `0x1FE0C6` (return `0x1FE616`). That report is `post-memory-browser-20261103-02.json`.

## Instrumenter requirement

The earlier private build produced a real write value expression:

`MEM32(ebp + -3068) = MEM32(ebp + -3068) & 0`

The instrumenter evaluates any scalar write value expression that probes memory into a temporary before the staged store. This preserves the read/write ordering and event value. Python memory-access tests pass 10/10.

## Limits and next task

`0x1FFE27` reached ordinal 168 but did not receive a result. No renderer, audio, controller, disc read, playable screen or complete runtime is established. The device reads/writes and modeled interrupt services are synthetic test scaffolding, not recovered console state. Next: implement and verify the bounded `MmClaimGpuInstanceMemory` service and the reached 0x5000-byte clear under an explicit capacity/bulk-write model, then continue to `0x1FDF70`. Preserve the explicit synthetic register values and do not infer NV2A interrupt delivery, DMA, device behavior or rendering.
