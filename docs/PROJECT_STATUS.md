# Project status (ring buffer)

Format: rule R2 of [docs/EXECUTION_GOAL_NOV5.md](EXECUTION_GOAL_NOV5.md). This page
holds the current state, the current milestone and the metrics pointer. Superseded
sections move to docs/archive/ instead of accumulating.

## Current checkpoint

Observed stop (status 10, guest read fault) at `0x6B6A6968`, 346 routines / 414 kernel
ops, module 130 / profile 87 in the runtime-a lane's evidence. The stop is behind
ordinal 171 `MmFreeContiguousMemory`: the read at site `0x1FC091` (`MOV EDX,[EAX]`) in
`FUN_001FC010` takes `EAX` from `[esi+0x47C]` at `0x1FC087`; the faulting object base is
`0x205D58`, and field `0x2061D4` holds ASCII palette-ramp bytes
(`0x68,0x69,0x6A,0x6B` = "hijk") written by `FUN_001fdf83`'s 3-lane × 0x100 palette
initializer — a bogus pointer read of palette data, not a pointer. The faulting object
is not the static device (`D3D_g_pDevice` write-site evidence), and the pointer's writer
is not identified, so this is a read-only diagnostic boundary with no model warranted
yet. The Nov 2 renderer-entry checkpoint is superseded as a description of the current
boundary.

Established with both-profile browser evidence: original FATX cache-format writes,
RAW dismount, utility-drive mount, the MmAllocateContiguousMemoryEx and
MmQueryStatistics services, and GPU-context device access into an explicit inert
synthetic aperture (not an NV2A model). Not established: renderer, device behavior
beyond the aperture, audio, controllers, disc reads, general FATX files/writes/
persistence, scheduling preemption, a playable original screen and every planned
feature. Original content is player-supplied and stays local; the repository
contains no original ROM, assets or pseudocode.

## Current milestone

M1 of docs/EXECUTION_GOAL_NOV5.md (admit and translate reached `0x1fe085` plus its
immediate dependencies) is recorded as completed by runtime-a lane work on Nov 3 —
"Execute 0x1fe085 and model reached interrupt initialization" (`10fa58f`, lane branch,
pending promotion). The live lane task is the read-only `esi`/caller capture at the
`0x6B6A6968` investigation boundary: record `esi` at `0x1FC087` and `FUN_001FC010`'s
entry `ECX`/caller, then decide with evidence. Milestones M2–M5 and all operating
rules live in docs/EXECUTION_GOAL_NOV5.md; metrics:
docs/PROGRESS_METRICS.md.

## History pointer

Sections prior to Nov 5, 2026 moved to
docs/archive/PROJECT_STATUS_PRIOR_20261105.md; per-milepoint checkpoint history
remains in the docs/*.md checkpoint files.
