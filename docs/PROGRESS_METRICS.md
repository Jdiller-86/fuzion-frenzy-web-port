# Progress metrics (R7)

One line per lane commit; the architect posts a weekly rollup. A "runtime advance"
is a lane commit recording a new observed stop boundary or a newly serviced reached
request with both-profile evidence. Baseline below; target ≥8 runtime advances
per day (rolling 7-day average) from M1 onward. <2 advances in any 48 h triggers a
mandatory corrective decision in docs/DECISIONS.md.

## Baseline

- 2026-11-01/02: ~24 runtime checkpoint advances in ~48 h (≈12/day) on one lane,
  measured from the repository reflog — pre-restructure baseline.

## Entries

- 2026-11-05 integration ops: goal installed as docs/EXECUTION_GOAL_NOV5.md
  (commit 65a4e691, subject `ops:`). Orchestration only; no runtime advance.
- 2026-11-03 lane (runtime-a): ~20 execution/service advances past `0x1fe085` into
  the device-table region (reflog subjects 0x1fe085…0x20057a) — the principal advance
  records for this goal's baseline period completed before its goal existed; the
  Nov 2-era main-docs stall masked lane progress.
- 2026-11-03 lane (runtime-a): M1 of EXECUTION_GOAL_NOV5 (translate `0x1fe085` +
  interrupt init) completed as a lane advance commit.
