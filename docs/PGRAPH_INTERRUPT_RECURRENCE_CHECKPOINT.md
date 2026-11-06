> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Vector/port hardening is accepted; the next stop is a PGRAPH interrupt read-back recurrence

**Continued in [the PFIFO interrupt acknowledge checkpoint](PFIFO_INTERRUPT_ACKNOWLEDGE_CHECKPOINT.md):** `0xFD002100` (`NV_PFIFO_INTR_0`) is now acknowledged the same way, and the run advances to a PFIFO engine-state wait that fills the bounded device log.

November 4, 2026. The guarded emitter now rejects vector loads used as stores or escaped pointers and fails closed on mixed-case helper names, the port surface stops on an unmodeled port instead of fabricating zero, and the reached original code compiles and runs unchanged under those rules. With the corrected DPC head the ordinal-149 timer/clock rejection no longer occurs; the same call chain now enters a PGRAPH register path and recurses without returning until the diagnostic trace fills. **There is no playable screen, renderer, device model or fidelity result.**

## Source-owned hardening

- `tools/recomp_memory_access.py`
  - a checked vector load (`XMM_MEM` / `XMM_LOAD`) is rejected when the surrounding text shows a store target (`=`, compound assignment, `++`/`--`) or an escaped pointer (`&`); the emitter no longer silently turns a write through a load into a read-only rewrite.
  - helper admission matches any `xmm_`-cased call and requires an exact-case `PURE_XMM` entry, so mixed-case names cannot slip past the allowlist.
- `src/runtime/port_io.h/.c`: `ff_port_io_read(io, port, width, *value)` returns 0 for the modeled `0x80C0` field pin and 1 for any other port or invalid width, leaving `*value` untouched. Unknown reads are no longer fabricated as zero.
- `tools/fixtures/thread_startup_probe.c`: `xbox_PortIn` stops with status 14, the port and the width when the read is not modeled.
- `src/runtime/thread-proof-worker.mjs`: status 14 reports `unmodeled port input`.
- `tests/recomp_memory_access_test.py` adds `XMM_MEM(eax) = xmm0;`, `eax = &XMM_MEM(edi);`, `Xmm_Mul(...)`, `xmm_add(...)` and `XMM_MEM(eax)++` as rejected inputs (11 cases).
- `tests/port_io_test.c` asserts the unknown-port failure leaves the out-value untouched, an invalid width and a null out-pointer fail, and reads are not counted on failure.
- `tests/recomp_memory_access_xmm_test.c` denies a 16-byte write at the guest-memory hook and asserts the access is reported as a failed write with the requested site, then succeeds when permitted.

## Run

- Private module `recomp-post-memory-build-68` rebuilt from this source: 639,160 bytes, SHA-256 `6dc4770883b04d411cdfc7f2c96f2b8a1c40f1890dd715e151357f5265cb8181`.
- Profile run: **277** distinct routines, 83/83 initializers, 403 kernel operations, `timerCheck = 0`, DPC head `0x03E00008` self-looped, timer head `0x03E00000` → `0x319198`.
- No status-14 stop occurred: the reached path only reads the modeled `0x80C0` port, so the new fail-closed port contract is compatible with the reached code.
- The ordinal-149 timer/clock rejection at return `0x200F73` no longer occurs, and the armed six-hour timer remains valid in the queue report.
- New stop: status 5 (`diagnostic capacity reached`) at target `0x200F59`, return `0x1FDB20`.

## Corrected stop addresses

Earlier notes decoded the previous trace stop as `0x201199` / `0x1FDBE0`. Both were decimal-for-hex misreadings of the report; the verified values are `2101081 = 0x200F59` and `2087712 = 0x1FDB20`, which is exactly the instruction after `call 0x001FDAC0` at `0x1FDB1A`. The corrected values are used from here on.

## The recurrence

- From trace event 3,840 to the 8,192 capacity, all 4,352 events are `enter` with no `exit`: `0x200F59` (`eax = 0xFD000000`) alternating with `0x1FDAC0` (`eax = 0x1000`).
- The guest stack falls 24 bytes when `0x200F59` calls `0x1FDAC0` and 32 bytes when `0x1FDAC0` calls `0x200F59`, i.e. 56 bytes per cycle over roughly 2,176 cycles (121,824 bytes), so this is mutual recursion that never returns, not a polling loop.
- `0x1FDAC0` dereferences its object to the device base `0xFD000000` and then touches `0x400100`, `0x400108`, `0x400144`, `0x40014C`, `0x400700`, `0x400704`, `0x400708`, `0x400720` and `0x400764`; it also spins while `[device + 0x400700]` is nonzero. Several of those offsets match the local NV2A register header: `0x100 NV_PGRAPH_INTR`, `0x108 NV_PGRAPH_NSOURCE`, `0x140 NV_PGRAPH_INTR_EN`, `0x144 NV_PGRAPH_CTX_CONTROL`, `0x708 NV_PGRAPH_TRAPPED_DATA_LOW` and `0x720 NV_PGRAPH_FIFO`, so the chain is a PGRAPH interrupt/channel-context path. The header does not define a top-level register at `0x700`; that offset is only a data-slot read here and is already an explicit synthetic entry.
- `0x200F59` re-enters `0x1FDAC0` only while `[device + 0x400100]` is nonzero, and `0x1FDAC0` writes `0x1000` to that register before calling it back. The explicit synthetic register storage reads back the value the guest stored, so the `HI8(value) & 0x10` branch selects the recursive call every time.
- The loop is therefore driven by the register read-back, not by a translation or decode defect: the reached code would leave the recurrence if the register stopped reporting bit `0x1000` after the write.

## Not established

- Whether the real PGRAPH interrupt/status register at `0xFD400100` clears, latches or reads back is **not** established. The bounded `ff_device_registers_add_self_clearing` path clears a fixed mask after a completed write (already used for `0xFD100410`, mask `0x10000`), which is not obviously the same semantic as a write-1-to-clear status word. Registering a mask for `0xFD400100` without evidence would silently change guest control flow.
- No rendering, audio, fidelity, original-game compatibility or playability is established. All private evidence stays local.

## Validation

`tests/kernel_timers_test.c`, `tests/port_io_test.c` and `tests/recomp_memory_access_xmm_test.c` pass optimized and under ASan/UBSan; Python memory-access 11/11; runtime suite 110/110; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Establish from retained evidence how the PGRAPH interrupt/status register at `0xFD400100` behaves after the write, then model it with the bounded register mechanism only if the evidence supports it, or stop explicitly at that read with the observed sequence. Do not invent a clear, force the recursion to return, raise the trace capacity again or claim rendering/fidelity without evidence.
