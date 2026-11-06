# Continue from the runtime-a fault investigation at 0x6B6A6968

Runtime lane records are appended in `.worktrees/opencode-source/docs/`; this page reflects the last promotion-facing state.

OpenCode model routing and source-only worktree instructions are in [the OpenCode workflow](OPENCODE_WORKFLOW.md); the [continuous development goal](OPENCODE_GOAL.md) defines the bounded advancement loop. This changes development coordination, not the runtime checkpoint below.

Observed stop (status 10, guest read fault) at `0x6B6A6968`, 346 routines / 414 kernel ops, module 130 / profile 87 in the lane's evidence; the stop is behind ordinal 171 `MmFreeContiguousMemory`: read at site `0x1FC091` (`MOV EDX,[EAX]`) in `FUN_001FC010`, `EAX` from `[esi+0x47C]` at `0x1FC087`; the faulting object base is `0x205D58` and field `0x2061D4` holds ASCII palette-ramp bytes (`0x68,0x69,0x6A,0x6B` = "hijk") written by `FUN_001fdf83`'s 3-lane × 0x100 palette initializer — a bogus pointer read of palette data, not a pointer. The faulting object is not the static device (`D3D_g_pDevice` write-site evidence); the pointer's writer is not identified; this is a read-only diagnostic boundary with no model warranted yet.

Since Nov 3 the runtime-a lane executed and advanced through `0x1fe085` (plus reached interrupt initialization), `0x1fe0c6`→`0x1FFCC0`, sub-dword device reads/writes (`0x1ffcc0`, `0x1fff25`), `MmClaimGpuInstanceMemory` modeled with the 0x5000-byte clear (`0x1ffe27`), `0x1fdf70`→`0x1FFF25`, `0x1fdf83` per-byte device knownness, `0x1ffff4`→`0x20002A`, `0x20002a`/`0x20005a` with a bounded port-output log, `0x1fe1d0`→`0x200340`, converted system time→`0x2010FD`, bounded device-table services, `0x200F59`, `0x20107C`, `0x200143`/`0x200352`, `0x2004df`→`0x20057A`. These are lane-branch records pending promotion, so M1 of the execution goal is recorded as completed by lane work, not blocked.

**Next task:** the lane's live handoff in `.worktrees/opencode-source/docs/DEVELOPMENT_HANDOFF.md` governs detail until its promotion: record `esi` at `0x1FC087` and `FUN_001FC010`'s entry `ECX`/caller (read-only), then decide with evidence. Main docs re-point at promotion.

Checks per lane handoff: `test:native` 48/48, proof-report 23/23, `test:runtime` pass, `check` 478 files. No controllable original screen.
Prior checkpoint handoff sections are archived at
docs/archive/DEVELOPMENT_HANDOFF_PRIOR_20261105.md.
