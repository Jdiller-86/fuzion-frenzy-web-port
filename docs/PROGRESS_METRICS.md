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
- 2026-11-05 ops (M0 item 3): all `slice-*`/`sync-*` scratch branches and their
  worktrees removed after evidence capture - 12 local refs (`slice-integrable-cc07556`,
  `slice-merge-correct`, `slice-merge-main`, `slice-rebase-b62989f-full`,
  `slice-rebase-complete`, `slice-rebase-final`, `slice-rebase-linear`,
  `sync-ae4b7ae-1fc780`, `sync-b5294fa`, `sync-becc879`, `sync-helper`,
  `sync-main-2aba638`) and 16 worktrees; no `check-*` branch ref existed. `gh` is not
  installed, so no remote ref was deleted; `origin/slice-merge-correct` remains a
  leftover. Orchestration only; no runtime advance. See D303.
- 2026-11-05 ops (M0 item 5): `docs/PROGRESS_METRICS.md` confirmed in R7 format
  (baseline, one line per lane commit, weekly rollup) and now carries this cycle's
  entries; `runtime-b` added to `tools/opencode-builder-models.json` and the dissolved
  geometry/verification lanes recorded in `docs/OPENCODE_WORKFLOW.md`. Same commit as
  M0 item 3; orchestration only.
- 2026-11-05 lane (runtime-b): first cycle returned outcome `progressed`, exit 0,
  model `opencode-go/deepseek-v4.1-flash`, commit `5ec83b8` (local only). The task card
  asked for a reached-but-unserviced service model, which is unavailable at a read-only
  diagnostic boundary (`esi` capture at 0x1FC087, stop unchanged at 10 / 0x6B6A6968 /
  346 routines / 414 kernel ops); the builder correctly refused to invent a model, so
  no runtime advance. The commit touched runtime-a-owned proof harness files, firing the
  R6 conflict rule; runtime-b was re-pointed to the source-owned line/quad
  browser-prepass backlog. See D305.
- 2026-11-05 ops: single close-out commit
  `ops: M0 close slice/sync/check scratch refs and dissolve lane worktrees; record
  decisions` carries the ref/worktree cleanup record and D302-D305 (renumbered
  D383-D386 during the Nov-5 reconciliation). Orchestration only; no runtime advance.
- 2026-11-05 ops: `ops: reconcile promoted main with orchestration reset; apply
  feature-based naming directive` (this commit) absorbs `origin/main` `97d831b` (476
  origin-only commits) onto local main `11fd2fd`: conflicted paths default to origin,
  five structural docs hand-merged, the goal doc renamed and extended for the
  feature-based naming directive, and D383-D389 appended. Orchestration only; no runtime
  advance. See D388.
- 2026-11-05 lane (runtime-b): `5ec83b8` (ESI-survival classification) is superseded by
  runtime-a's integrated equivalent `8244fa5` on `origin/codex/opencode-source`; it is
  dropped from the runtime-b branch by lane reset in the next runtime-b task, with its
  validator-hardening idea kept only as a candidate improvement note. No runtime advance.
  See D389.
- 2026-11-05 tempo: the observed stop remains unchanged (status 10 at `0x6B6A6968`)
  across the recent diagnostic series, so <2 new stop boundaries in 48 h → corrective
  decision recorded: the next runtime-a directive is to obtain the decisive capture
  verdict or a blockers package within ≤2 further cycles (R7 trigger).
- 2026-11-05 lane (runtime-b): re-pointed from the superseded `5ec83b8` (D389) to the
  promoted main head `bc5b6a5`; ops commit `c829743` replaces the current-checkpoint
  bullet with the owned NV2A line/quad browser-prepass objective and its scope boundary.
  No runtime advance. See D390.
- 2026-11-05 lane (runtime-a): `a0f4d5e` on `codex/opencode-source` replaces the stale
  A023/A022 directive block with the Nov-5 directive (decisive capture verdict or a
  blockers package within ≤2 further cycles; record unsupported path elements). Not
  pushed; the runner publishes this branch. No runtime advance. See D390.
- 2026-11-05 ops: deleted the stale `.worktrees/opencode-runtime-b-cycle.lock`
  (mtime 18:50:20; cycle finished 19:01:43); no other `.worktrees` file touched.
- 2026-11-05 publication: `lane/integration` and `lane/runtime-b` pushes were refused by
  the project policy `shell:git push * = deny` before any network attempt, so nothing was
  retried or forced and `git ls-remote` shows neither branch. Publication of
  `lane/integration` remains the architect/runner step, not the builder's. See D390.
- 2026-11-05 review (geometry): all eight NV2A files (`nv2a_line/quad/triangle` `.c/.h`
  and `tests/nv2a_line_test.c`, `tests/nv2a_quad_test.c`) are present on merged main
  `5c3cbb6` by `git ls-tree HEAD`; the frozen branch stays unintegrated and the remaining
  work is runtime-b's `tools/run-native-tests.mjs` wire-through plus the line/quad browser
  prepass; the stale recovery card is corrected at `0864419`. No runtime advance. See
  D391.
- 2026-11-05 review (verification): the lane remains frozen; its content is
  document/manifest/plugin-infrastructure only with no runtime-path absorption candidate,
  and it reopens only on a named runtime need. No runtime advance. See D391.
- 2026-11-05 schedule: `FuzionFrenzy-OpenCode-CycleB` is absent; the Cycle, Geometry and
  Verification triggers report `Enabled=False` while their task settings stay enabled;
  runtime-a's last cycle ran 18:52:29-18:57:04 and the idle orchestrator heartbeat plus
  all-HOLD manager cards explain the silence, not a failed task (LastTaskResult 0). No
  runtime advance. See D391.
- 2026-11-05 publication: `origin/main` is still `97d831b`; local main `5c3cbb6` is seven
  ops commits ahead and unpushed together with the lane heads, and no `lane/integration`
  ref exists yet, so the PR remains with the user/runner. No runtime advance. See D391.
- 2026-11-05 ops: annotated tag `runtime-b-5ec83b8-archive` pins superseded `5ec83b8`
  against garbage collection; no other refs were created. No runtime advance. See D391.
- 2026-11-05 executor: stall confirmed at trigger granularity - Cycle (PT5M), Geometry
  (PT1H) and Verification (PT15M) had `Triggers[0].Enabled=False` while their tasks were
  `Ready` with settings enabled, Cycle's `LastRunTime` was 18:52:28 with
  `LastTaskResult 0`, and `CycleB` did not exist; the 19:35:05 orchestrator heartbeat
  `idle` and the manager holds explain the silence without a failed run. No runtime
  advance. See D392.
- 2026-11-05 executor: `FuzionFrenzy-OpenCode-CycleB` registered from the Cycle template
  (PT5M, P1D, start `2026-11-03T00:02:30-05:00`, limit PT1H30M, `-Lane runtime-b`,
  workdir `.worktrees\opencode-runtime-b`) and its inherited disabled trigger armed with
  `-Trigger` (the documented `-Triggers` spelling is invalid on this host). No runtime
  advance. See D392.
- 2026-11-05 executor: primary `FuzionFrenzy-OpenCode-Cycle` trigger re-enabled
  (`TriggerEnabled=True`, `NextRunTime 19:45:00`); `Geometry`, `Verification` and
  `Manager` set to `State=Disabled` per D391's dissolved-lane verdicts, with
  `Architect` and both Cycle tasks left enabled. Revert: `Enable-ScheduledTask` for the
  three, `Disable-ScheduledTask` for Cycle/CycleB. No runtime advance. See D392.
- 2026-11-05 executor: cadence observed live - CycleB ran at 19:42:30
  (`State=Running`, `LastTaskResult 267009`, `NextRunTime` rolled to 19:47:30) and
  `.worktrees/opencode-runtime-b-cycle-state.json` mtime advanced 19:01:43 -> 19:42:31,
  so a new runtime-b cycle began; primary Cycle still showed `LastRunTime 18:52:28` at
  the 3-minute observation boundary with `NextRunTime 19:45:00`. No runtime advance is
  claimed for this ops commit. See D392.
- 2026-11-05 executor: discarded runtime-b's duplicate ESI-capture harness set after
  confirming the dirty list was exactly five tracked modifications
  (`src/runtime/proof-report.mjs` 19:48:13/34848, `src/runtime/thread-proof-worker.mjs`
  19:47:32/115005, `tests/proof-report.test.mjs` 19:48:13/29269,
  `tools/fixtures/thread_startup_probe.c` 19:48:00/126139,
  `tools/run-native-tests.mjs` 19:46:59/7953) plus three untracked
  (`src/runtime/esi_events.c` 1352, `src/runtime/esi_events.h` 1568,
  `tests/esi_events_test.c` 2284, all 19:46:56); revert + clean left
  `git status --short` empty. No runtime advance. See D393.
- 2026-11-05 executor: `lane/runtime-b` commit `5d0d14c` hardens the single-owner boundary -
  the handoff top block and the AGENTS current-checkpoint bullet now state runtime-b's owned
  nv2a prepass objective and bar the capture harness; all later `## Prior:` sections are
  untouched (2 files, +3 -3). No runtime advance. See D393.
- 2026-11-05 executor: runtime-b publication gate in `tools/Invoke-OpenCodeCycle.ps1` extended
  to deny the harness paths (`src/runtime/proof-report.mjs`,
  `src/runtime/thread-proof-worker.mjs`, `src/runtime/esi_events.*`,
  `tests/proof-report.test.mjs`, `tests/esi_events_test.c`,
  `tools/fixtures/thread_startup_probe.c`): all seven test paths blocked, lane-owned nv2a
  source/tests, `tools/run-native-tests.mjs` and lane docs still allowed, script parses with
  zero errors. No runtime advance. See D393.
- 2026-11-05 executor: `FuzionFrenzy-OpenCode-CycleB` re-armed after the boundary hardening -
  `State Disabled -> Ready`, `Triggers[0].Enabled=True`, `NextRunTime 19:57:30` at 19:55:09
  (`LastRunTime 19:47:30`, `LastTaskResult 0`). No runtime advance. See D393.
- 2026-11-05 executor: main PR #19 verdict recorded as not ready (R9) - head `18cce1d` sits on
  base `97d831bd` but does not contain `f9f503a`, `6ad7e9b`, `0864419` or `5d0d14c`
  (merge-base `75b01d4`), so five exact gates (DECISIONS `D383`-`D386` and `D392` collisions,
  the runtime-b `$unowned` regex, the missing metrics entries, the dropped D023 stanza) block a
  guarded merge and the next cycle must first reconcile the chain. No runtime advance. See D394.
- 2026-11-05 executor: runtime-b re-armed with the hardened single-owner card and publication
  boundary - the first post-hardening cycle launched at 19:57:31 (`attemptedHead 5d0d14c`) and
  finished 20:00:04 with `outcome: progressed`, `publication: published`, head `5289983`
  (nv2a suites wired into `tools/run-native-tests.mjs`, analytic fixtures only). No runtime
  advance. See D394.
- 2026-11-05 executor: runtime-a's newest lane commit at the record was `39d111e`
  ("Instrument the FUN_001FB640 tail and classify the ESI writer"), with the next step being the
  pop-value capture to confirm the save-slot mismatch; that capture landed as `5c01c3f`
  (restore slots off by one register, site `0x1FB72E` epilogue), so the current next step is to
  report the off-by-one save-slot finding to the translator owner. Stop unchanged at status 10 /
  `0x6B6A6968` / 346 routines / 414 kernel ops. No runtime advance. See D394.
