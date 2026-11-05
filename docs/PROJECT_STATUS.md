# Project status (ring buffer)

Format: rule R2 of [docs/EXECUTION_GOAL_NOV5.md](EXECUTION_GOAL_NOV5.md). This page
holds the current state, the current milestone and the metrics pointer. Superseded
sections move to docs/archive/ instead of accumulating.

## Current checkpoint

docs/RENDERER_ENTRY_CHECKPOINT.md (Nov 2, 2026): both synthetic browser profiles
return from 83/83 initialization callbacks, execute 234 distinct original routines,
complete 396 kernel operations across 3,633 events through the admitted synthetic
device aperture `[0xFD000000, 0xFD800000)`, then stop at untranslated original
routine `0x1fe085` (return `0x1fe5bd`). Awaiting recorded decision: device write
`0xfd009140`. Selection/build: `recomp-title-post-memory-06` /
`recomp-post-memory-build-16`, 344 functions / 12,878 instructions, ABI 24, module
`d757d534…` (424,106 bytes). The earlier utility-mount (317 functions / 10,612
instructions / ABI 22) and MmQueryStatistics boundaries are retained evidence and
are never cited as current.

Established with both-profile browser evidence: original FATX cache-format writes,
RAW dismount, utility-drive mount, the MmAllocateContiguousMemoryEx and
MmQueryStatistics services, and GPU-context device access into an explicit inert
synthetic aperture (not an NV2A model). Not established: renderer, device behavior
beyond the aperture, audio, controllers, disc reads, general FATX files/writes/
persistence, scheduling preemption, a playable original screen and every planned
feature. Original content is player-supplied and stays local; the repository
contains no original ROM, assets or pseudocode.

## Current milestone

M1 of docs/EXECUTION_GOAL_NOV5.md: admit and translate reached `0x1fe085` plus its
immediate dependencies (≤8 related reached functions / ≤2,000 instructions), decide
device write `0xfd009140` as a recorded D-number policy without invented values,
run both profiles, and land the M1 commit with audit JSON (zero source-identity
mismatches), green `npm run check` / `npm run test:runtime` /
`tests/title_seed_test.py` and a green CI publish. Milestones M2–M5 and all
operating rules live in docs/EXECUTION_GOAL_NOV5.md; metrics:
docs/PROGRESS_METRICS.md.

## History pointer

Sections prior to Nov 5, 2026 moved to
docs/archive/PROJECT_STATUS_PRIOR_20261105.md; per-milepoint checkpoint history
remains in the docs/*.md checkpoint files.
