# Empty-ring initialization passes the PFIFO wait; execution reaches 0x1FB5B0

**Later evidence (November 4):** [the adoption checkpoint](PFIFO_PROFILE_ADOPTION_CHECKPOINT.md) makes the model an explicit profile-driven default. Build `recomp-post-memory-build-58` reaches `0x1FB5B0` with the profile and stays fail-closed without it.

November 4, 2026. A bounded empty-ring channel model was implemented and tested. With the observed ring profile (base `0x03B7D000`, end `0x03BFD000`, PUT at the base), the model initializes GET to the base when the channel is enabled with an empty ring. In the diagnostic build this satisfied the original wait at `0xFD800044`: the startup advanced past the PFIFO boundary and stopped at an untranslated original routine at `0x1FB5B0`. This is the first observed original-code progress past the wait. **There is no playable screen, renderer or device model.**

## Source-owned change

`src/runtime/pfifo_channel.h/.c`:

- `ff_pfifo_channel_configure` validates an explicit aligned base/end within 64 MiB and starts PUT at the base.
- `ff_pfifo_channel_enable` initializes GET to PUT when the ring is empty (PUT == base).
- `ff_pfifo_channel_set_put` validates the PUT range and reinitializes GET to the base when an enabled ring is emptied.
- `ff_pfifo_channel_set_get`, `read_get` and `empty` expose the bounded state.
- It does not consume a non-empty ring, follow a jump, execute a method or read/write guest memory.

`tests/pfifo_channel_test.c` covers the observed profile, empty/non-empty enable, re-emptying, explicit GET writes and invalid null/unaligned/out-of-range inputs. It passes optimized and ASan/UBSan WSL GCC runs.

`tools/fixtures/thread_startup_probe.c` gained an opt-in `FF_PFIFO_EMPTY_RING_PROBE`: it reads the device context (`D3D_g_pDevice` at `0x00206720`), configures the channel from the observed base/end/PUT, and on a DMA_GET read serves the base when the ring is empty. The default build is unchanged.

## Diagnostic run

Private build `research/verification/recomp-post-memory-build-56` combined `FF_ALLOW_DMA_GET_READ`, `FF_CONTEXT_PROBE` and `FF_PFIFO_EMPTY_RING_PROBE`; module SHA-256 `fa9f284af966ffe64a33d5032c747a6e7945445a62054bfc0ab4db57e341761c`, 527,589 bytes.

The existing worker, retained XBE and `settings-mmio-20.json` report:

- 83/83 initialization callbacks returned.
- 262 distinct original routines executed.
- 402 kernel operations completed.
- Stop status 2 (`untranslated original routine`) at target `0x1FB5B0`, return address `0x1FB63D`.
- The PFIFO GET wait no longer stops execution.

## Interpretation

- The original wait expects an empty ring's GET to equal the base/PUT. With the observed 512 KB ring and PUT at the base, the model supplies that value and the original code proceeds.
- The command source is still the empty ring; no method was consumed and no pushbuffer command was executed. The next dependency is the untranslated routine at `0x1FB5B0`.
- The default module remains fail-closed at status 13 until the empty-ring model is reviewed and adopted in the product build.

## Limits

One diagnostic build and one explicit synthetic settings profile. The model handles only the observed empty-ring initialization; it does not establish a general consumer, the kernel D3D submission path, rendering, audio, fidelity or playability. The diagnostic defines are not the default contract.

## Validation

`tests/pfifo_channel_test.c` passes optimized and ASan/UBSan. A control build without the diagnostic defines (`recomp-post-memory-build-57`) is byte-identical to the known fail-closed module (SHA-256 `cd0bf176…`, 526,955 bytes). Runtime suite 109/109, Python memory-access 10/10, title-seed 25/25, `npm run check` and whitespace checks pass.

## Next task

Review and adopt the empty-ring model in the default build if the evidence is accepted, then translate/admit the reached original routine at `0x1FB5B0` and continue. Do not consume a non-empty ring or claim rendering/fidelity without evidence. The next genuine dependency is `0x1FB5B0`, not a GPU method.
