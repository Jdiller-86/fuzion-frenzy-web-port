# Project status (ring buffer)

Format: rule R2 of [docs/EXECUTION_GOAL_NOV5.md](EXECUTION_GOAL_NOV5.md). This page
holds the current state, the current milestone and the metrics pointer. Superseded
sections move to docs/archive/ instead of accumulating.

## Current checkpoint

Observed stop unchanged: status 10 (guest read fault) at `0x6B6A6968`, 346 routines /
414 kernel ops, in the runtime-a lane's evidence (module 150 / profile 87). The stop is
in graphics initialization's free-framebuffer path: after ordinal 171
`MmFreeContiguousMemory` and `CDevice_FreeFrameBuffers`, a helper loads a pointer field
from the device-context object (`[esi+0x47C]`) and dereferences it at site `0x1FC091`,
but the field holds palette-ramp bytes (`0x68,0x69,0x6A,0x6B` = "hijk", from
`FUN_001fdf83`'s ramp initializer), not a pointer. The faulting object is not the static
device (`D3D_g_pDevice` write-site evidence). The ground-truth arbiter compares the field
capture with the fault frame and both report the same register value, so the capture is
not misreporting it: the open question is which executed path leaves that value in the
register. No fix, model or translator edit; the investigation continues read-only.

Established with synthetic-profile browser evidence: original startup through the FATX
utility mount, the memory services (`MmAllocateContiguousMemoryEx`, `MmQueryStatistics`,
`MmFreeContiguousMemory`), the reached AV service ordinals (2, 1, 3) and the ordinal-159
vblank wait, the default-off PFIFO pushbuffer consumer, and the D3D present path into
graphics initialization - each as an explicitly labeled model where hardware is not
modeled, not as hardware fidelity. Not established: a renderer, device behavior beyond
the labeled models, audio, controllers, disc reads, general FATX files/writes/persistence,
scheduling preemption, a playable original screen and every planned feature. Original
content is player-supplied and stays local; the repository contains no original ROM,
assets or pseudocode.

## Current milestone

M1 of [docs/EXECUTION_GOAL_NOV5.md](EXECUTION_GOAL_NOV5.md) is recorded as completed by
runtime-a lane work on Nov 3: graphics initialization executes past the device-table
boundary with device services modeled. The live task is the read-only investigation of the
`0x6B6A6968` pointer fault: decide, with evidence, whether the executed value is
translated-path behavior or a capture artifact, and capture the exact failing path before
any model. Milestones M2-M5 and all operating rules live in the execution goal; lane
metrics: [docs/PROGRESS_METRICS.md](PROGRESS_METRICS.md).

## History pointer

Sections prior to Nov 5, 2026 moved to
docs/archive/PROJECT_STATUS_PRIOR_20261105.md; per-boundary checkpoint history
remains in the docs/*.md checkpoint files.
