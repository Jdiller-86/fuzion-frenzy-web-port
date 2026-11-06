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
- 2026-11-05 executor: lane-b's nv2a wiring verified by content, not by message - `5289983`
  registers `nv2a_line`/`nv2a_quad`/`nv2a_triangle` in `tools/run-native-tests.mjs` with
  `linkFlags: ['-ffp-contract=off', '-lm']` after the translation units, matching the Linux CI
  compile; diff is 3 lane-owned files (+38 -5) with no harness-ban path, and
  `origin/lane/runtime-b` is still at that head. No runtime advance. See D395.
- 2026-11-05 executor: the browser prepass comparison remains unimplemented in lane-b's tree -
  the commit body records the native reference and prepass inputs as absent there, and the
  read-only input check finds only triangle-side harnesses on main (`tools/compare-nv2a-geometry.mjs`
  drives `_ff_nv2a_expand_triangle` with the four-word state;
  `tools/fixtures/nv2a_geometry_check.html` is triangle-only) and no `prepass`-named file on the
  frozen `codex/opencode-geometry` branch, whose line/quad reference is "captured locally, private
  and unavailable in this worktree". The next card must locate and record those inputs before
  implementing. No runtime advance. See D395.
- 2026-11-05 executor: runtime-a tempo state unchanged - the off-by-one save-slot finding
  (site `0x1FB72E` epilogue) is one third of a blockers package with the owner-side options still
  absent, so the decisive-verdict deadline (<=2 cycles) is consumed and R7 remains 0 counted
  advances; stop still status 10 / `0x6B6A6968` / 346 routines / 414 kernel ops and the next cycle
  must land the completed package or a new boundary. No runtime advance. See D395.
- 2026-11-05 executor: PR #19 advisory against merging now - `mergeable_state: clean` with 4/4
  checks on head `18cce1d`, but that head still lacks `f9f503a`, `6ad7e9b`, `0864419`, `5d0d14c`,
  `5289983` and the `2988892`-line records and keeps the `D383`/`D392` collisions, so a merge would
  drop the M0 goal-doc, lane-cadence-revival and single-owner records; the architect must
  reconcile the head per D394 first. No runtime advance. See D395.
- 2026-11-05 executor: runtime-a's `77f060d` ("Correct the off-by-one slot claim to provisional",
  20:05:53) is the tip of `origin/codex/opencode-source` and retracts the stale
  `0x205E98`/`0x203B90` off-by-one hypothesis to provisional status - the four tail reads
  (`0x3C10E34`/`0x3C10E38`/`0x3C10E3C`/`0x3C10E40`) are unordered relative to the pop sequence,
  so an order-tagged pop capture is needed before any translator-defect report; no new stop
  boundary, no decisive verdict and the blockers package is still incomplete, with the stop at
  status 10 / `0x6B6A6968` / 346 routines / 414 kernel ops. See D396. No runtime advance.
- 2026-11-05 executor: runtime-b's only new head is the local-only card edit `6a11cf2`
  (`origin/lane/runtime-b` is still `5289983`), and PR #19 keeps head `18cce1d` and base
  `97d831b` with reported `mergeable_state: clean` and 4/4 checks (`gh` absent, so as reported);
  that head still misses `f9f503a`, `6ad7e9b`, the `2988892`-line records, the lane-b wiring and
  the `$unowned` regex hardening, so no guarded merge may target it before the 20:40 tick
  reconciles a superset head, and tempo stays 0 counted advances. See D396. No runtime advance.
- 2026-11-05 runtime-a: commit `e6bc808` ("Order-tag the pop capture and confirm the ESI slot mismatch",
  20:15:39 -0500, published to `origin/codex/opencode-source`) records `g_esi` after each pop
  (module 156): slots `0x3C10E40`=`0x3C10CFC` (ebx), `0x3C10E3C`=`0xFFFFFFFF` (ebp),
  `0x3C10E38`=`0x203B90` (esi), `0x3C10E34`=`0x205E98` (edi); ESI holds `0x203B90` through every
  pop up to `0x3C10E30` and becomes `0x205E98` at the pop consuming the saved-EDI slot
  `0x3C10E34`. That confirms the defect class as a stack-slot/register mapping mismatch in the
  `FUN_001FB640` epilogue, not guest state/profile/probe artifact. Still missing from the
  blockers package: one named writer instruction address in the translated epilogue and named
  repair options with an owner (translator-side vs `.inc` expansion). Stop unchanged at status 10 /
  `0x6B6A6968` / 346 routines / 414 kernel ops. See D397. No runtime advance.
- 2026-11-05 runtime-b: commit `55e7446` ("M2: report reached service identity at a refused stop",
  20:14:58 -0500, published to `origin/lane/runtime-b`) reports the reached kernel service by M2
  name at a refused stop, touching only `src/runtime/kernel-service.mjs`,
  `tests/kernel-service.test.mjs`, `docs/RUNTIME_B_SERVICE_CHECKPOINT.md` and
  `docs/NV2A_GEOMETRY_CHECKPOINT.md` (+194 -9, zero harness-ban paths); its own checks read
  kernel-service 3/3, test:runtime 201 pass / 1 skip, check 489 files. Auditor-verified caveat: the
  commit's "no `geom-reference-01`; no Wasm toolchain" claim holds only for the lane worktree's
  `.tools/emulation` (four stub dirs) and not for the repo root, where
  `.tools/emulation/geom-reference-01/reference.json` exists at 2,904,864 bytes with SHA-256
  `474497d8...d7b530e5` (126-input/153-output triangle reference) and `emsdk` supplies `emcc`;
  the line/quad value-set is absent in both places and needs a local capture first. See D397.
  No runtime advance.
- 2026-11-05 tempo: 0 counted advances - no new stop boundary and no newly serviced reached
  request with both-profile evidence; the stop holds at status 10 / `0x6B6A6968` / 346 routines /
  414 kernel ops across both lane cycles. See D397. No runtime advance.

- 2026-11-05 runtime-a: card edit `a241507` ("ops: direct runtime-a to compose the epilogue-mismatch
  blockers package", parent `dcb9221`) directs the lane to publish the blockers package from the tested
  `ff_reg_order_is_reverse(push_order, pop_order, count)` rule - the two named repair-owners (translator
  callee-saved emission entry versus the `.inc` POP expansion) with exact file/site evidence, plus the
  bounded owner/user question - and records that the absence of a single named writer inside the
  translated epilogue IS the finding; the worktree card lacked the checkpoint bullet, so the directive
  was added as the first bullet with every other line untouched (+1). See D398. No runtime advance.
- 2026-11-05 runtime-b: card edit `5089305` ("ops: authorize digest-verified root-reference comparator
  for the triangle prepass", parent `7911e35`) permits the triangle reference from either the
  worktree-local or the repo-root relative path, always digest-verified, never committed or invented,
  with a recorded skip evidence line when neither resolves, and allows extending the comparator's
  allowlist to exactly those two paths if its `inside(toolRoot, ...)` guard rejects the root one; the
  same cycle's `7911e35` recorded that root `geom-reference-02` line/quad data exists, which conflicts
  with keeping line/quad as the recorded capture gap and needs an architect ruling. See D398.
  No runtime advance.
- 2026-11-05 executor: publication and integration state at ~20:30 - `origin/lane/integration` now
  exists at `1af5dce` (equal to local `main`; verified first-parent chain from `65a4e691` including
  `5c3cbb6`, which the brief omitted), while `origin/codex/opencode-architect` is still `18cce1d` and
  contains `a0f4d5e`/`8b9e061`/`d4d1207` but none of `f9f503a`, `6ad7e9b`, `5d0d14c`, `5289983`,
  `6a11cf2`, `8859dfa`, `0ffa346`, `1d53fa2`, `1af5dce`, `e6bc808`, `8b539e0`, so the two lines diverge
  and only a head claiming both may be merged; the lane-b runner copy also still lacks main's D393
  harness-ban tail. Tempo stays at 0 counted advances pending a new stop boundary. See D398.
  No runtime advance.
- 2026-11-05 ops (D399 ruling): both prepass reference sets are verified live at the repo
  root - `.tools/emulation/geom-reference-01/reference.json` (2,904,864 bytes, SHA-256
  `474497d8fb629580352390ec7705b4ed0ea47aa635b8064e9ad2f393d7b530e5`, 126-input/153-output
  triangle) and `.tools/emulation/geom-reference-02/reference.json` (12,353,921 bytes,
  SHA-256 `4fcd544b780b0d531fedc1bca8a1e1266c3b9ba0eb5daaa7900cd2f6e167fccf`, the 1,728-record
  line/quad set) - so the "line/quad capture gap" of D395/D397/D398(c) is superseded and the
  lane-b card now asks for BOTH digest-verified comparisons; card commit `d8ff7c1`
  (typo `and nothing gar less` -> `and nothing less`, ruling clause applied). No runtime advance.
- 2026-11-05 ops (tempo): 0 counted advances remains the honest count - the 20:31 lane
  commits (`dcb9221` runtime-a, `7911e35` runtime-b) are lane-progress commits, not a new
  stop boundary and not a newly serviced reached request, and the runtime-a blockers-package
  directive stands (D398(d)); the 20:40 architect tick is the expected reconciliation
  actuator for the divergent integration lines, reading the D394-D399 rows. Stop unchanged:
  status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops. No runtime advance.
- 2026-11-05 ops (component progress, not tempo): runtime-b's published `7911e35` reproduced the
  triangle browser prepass comparison against the digest-verified root reference
  `.tools/emulation/geom-reference-01/reference.json` (SHA-256
  `474497d8fb629580352390ec7705b4ed0ea47aa635b8064e9ad2f393d7b530e5`) with 960 records /
  146,880 scalars, `exact-match`, 0 forwarded / 0 slope mismatches, and module/wasm/reference
  hashes identical to the retained 2026-10-31 report; the line/quad comparison stays
  feasible-but-not-run because the comparator is triangle-hard-coded and no built module exports
  the line/quad entry points. Component progress only - no stop-boundary move and no newly
  serviced original request. No runtime advance.
- 2026-11-05 ops (tempo and handoff): 0 counted advances remains the honest count under the
  rolling-7-day >=8/day metric; runtime-a's blockers package is committed and published as
  `0f0b6c1` (row reusing D398, colliding with main's D398, to be renumbered at integration with a
  one-line note) and carries the tested `ff_reg_order_is_reverse` rule, candidates A/B and the
  bounded owner question, while `7911e35` is component progress. Stop unchanged: status 10 at
  `0x6B6A6968`, 346 routines / 414 kernel ops. No runtime advance.
- 2026-11-05 ops (D401): M6 decision recorded - the `FUN_001FB640` epilogue mismatch is ruled
  emission-side (candidate A) with candidate B (instrumentation attribution) as the registered
  fallback; the triple-gated repair protocol is (1) correct the translator's emitted callee-saved
  pop-destination order in `tools/prepare-title-recomp.py` to the observed slot table per the
  tested `ff_reg_order_is_reverse` rule, keeping the regression test rejecting the observed
  runtime order, (2) re-run both profiles and require the stop to move past `0x6B6A6968` in both
  with R11 evidence, (3) fall back to candidate B with a new dated row only if the stop does not
  move. Decision row only - no fix, no translator edit, no gate closed. No runtime advance.
- 2026-11-05 ops (tempo, D401): 0 counted advances standing; M6's first counted advance fires
  only when the stop actually moves with R11 evidence. Stop unchanged verbatim: status 10 at
  `0x6B6A6968`, 346 routines / 414 kernel ops. No runtime advance.
- 2026-11-05 ops (D402 revision): candidate (A) emission-side is refuted for `FUN_001FB640` by
  lane commit `ffacf95` (21:05:50, published to `origin/codex/opencode-source`) - module 87's
  generated `.inc` prologue pushes `ebx, ebp, esi, edi` and its epilogue pops
  `edi, esi, ebp, ebx`, an exact reverse and internally LIFO-consistent, so the generated text
  is correct and D401's step (1) emitter edit would corrupt it. Per D401 step (3) the live
  hypothesis is candidate (B), instrumentation/orchestration attribution (which probe
  instruction confirmed the register-consumer mapping when the pop consumed slot `0x3C10E34`),
  with the M6 gate unchanged (the stop must move past `0x6B6A6968` with R11 evidence) and REA
  read-only native analysis authorized for the bounded pop-attribution region if the records
  cannot decide it. The runtime-a card was replaced by lane commit `f69430e`; D400's
  `refs/pull/19/head = 44af31d` correction stands, PR #22 never existed, and the stale geometry
  draft PR #3 (head `aac82119`) is the safe close candidate. No runtime advance.
- 2026-11-05 ops (tempo): 0 counted advances standing - the 21:00-21:06 lane commits
  (`e6de8a9`, `ffacf95`, `2f2292b`) are component/evidence work, not boundary moves, so the
  stop is unchanged verbatim: status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops. No
  merge was requested or landed this hour and `origin/main` is still `97d831bd`. No runtime
  advance.
- 2026-11-05 ops (D403): PR-merge guidance for the unattended window recorded - a manual merge of
  PR #20 (head `1af5dce`) must NOT happen because its head is a strict subset of PR #19's head
  `44af31d` (`1af5dce`/`18cce1d`/`035b253`/`6ad7e9b`/`f9f503a` all verified contained), so the
  guarded merge stays on the #19 actuator route and lands only on a reconciled superset head with
  green exact-head checks (`35acdae`, `f4088e8`, `be791ef`, `c21e61b`, `0f0b6c1`, `ffacf95`,
  `d8ff7c1`/`209d60c` still absent at the ~21:40 reconciliation window); the safe owner-side action
  is closing the superseded geometry draft PR #3, and PR #20 may be closed as superseded once #19
  merges. No runtime advance.
- 2026-11-05 ops (D404): runtime-a is directed to the compiled-code/register-identity investigation - the
  module-158 sequence-tagged capture proves the pop consuming slot `0x3C10E34` (`0x205E98`) writes ESI while
  the pop consuming `0x3C10E38` (`0x203B90`) leaves ESI unchanged, so the attribution is real and the emitted
  `.inc` reverse is correct; the lane investigates only its owned `src/runtime/recomp_memory_access.h` and
  `tools/prepare-title-recomp.py`, with no `.tools`/`research`/fixture edits, and the M6 gate is unchanged
  (the stop must move past `0x6B6A6968` in BOTH profiles with R11 evidence). Tempo fact: 0 counted advances -
  the control profile's status-13 stop at `0xFD800044` is the control-condition marker, not a boundary move,
  and the stop is unchanged verbatim at `0x6B6A6968`, 346 routines / 414 kernel ops. No runtime advance.
- 2026-11-05 ops (D404): runtime-b's line/quad comparator extension is authorized - the lane's own
  960-fill-mode-record / 413,712-scalar prototype match (0 mismatches) was self-blocked on tooling-edit
  authorization, now granted for `tools/compare-nv2a-geometry.mjs` only under the bounded protocol (960-record
  triangle default untouched, skip-when-absent plus the digest check against the root
  `geom-reference-02/reference.json`, per-output rejection lines instead of invented values, outcome recorded
  in the owned checkpoint); the result stands as the lane's evidence statement, not architect-verified, and the
  card's transcribed triangle digest prefix `474ada8e2d0f...` conflicts with D399/D400's
  `474497d8...B530E5` pending a lane re-check before use. Tempo fact: 0 counted advances, stop unchanged at
  `0x6B6A6968` / 346 / 414. No runtime advance.
- 2026-11-05 ops (D405): gh CLI verified read-only (installed, authenticated as account `Jdiller-86`, scopes
  `repo`/`workflow`/`gist`/`read:org`, token redacted and never printed); the owner closed PR #3 (geometry draft,
  head `aac82119`, 21:14:08 EST) and PR #20 (head `1af5dce`, 21:26:55 EST) unmerged, so D403's #20 non-merge rule
  is moot and PR #19 is the sole main-targeting PR - `44af31d` at the 21:40 snapshot (MERGEABLE/`clean`, 4/4
  `preparation` checks), `d1cc85a` at the D405 read. Containment in `d1cc85a`: 10 of the 12 tested commits exit 0
  (`35acdae`, `f4088e8`, `be791ef`, `c21e61b`, `3aaf9fc`, `0f0b6c1`, `ffacf95`, `f69430e`, `37d94c2`, `595faf0`),
  while `9957144`/`40af26d` exit 1 (lane/runtime-b content, not on the architect branch); no `refs/pull/22` or
  `refs/pull/23` exists, so the queued "open PR" directives produced no PR and the guarded actuator route stays
  #19 only. Agent-side gh stays read-only; the guarded actuator remains the only merge path. No runtime advance.
- 2026-11-05 lane (runtime-a): read-only REA record at `595faf0` - XBE SHA-256
  `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`, prologue `0x1FB640..0x1FB643`
  `PUSH EBX; PUSH EBP; PUSH ESI; PUSH EDI; MOV ESI,ECX`, epilogue `0x1FB72E..0x1FB732`
  `POP EDI; POP ESI; POP EBP; POP EBX; RET`, taken under named fact (a) of the goal-doc unlock list (`11a6805`);
  the original-binary order matches the emitted `.inc` and the tested rule, so the mismatch is not in the original
  and the question is formally routed to the translator owner: "why does the compiled module bind the `POP EDI`
  slot to `esi`?" Runtime-b's line/quad comparator edit is still in flight and uncommitted (+255/-62, untracked
  test file, last commit `402fa29`), and the 21:40 tick (`integrated`, finished 21:44:06, head `d1cc85a`,
  `mergeResult` "PR is not cleanly mergeable: unstable", `origin/main` unmoved at `97d831b`) routed candidate (B)
  to the translator owner. Tempo fact: 0 counted advances, stop unchanged verbatim at status 10 `0x6B6A6968`,
  346 routines / 414 kernel ops. No runtime advance.
- 2026-11-05 ops (D406): the merge gate is green but not the superset - `refs/pull/19/head` = `d1cc85a`
  reports `mergeable: MERGEABLE`, `mergeStateStatus: CLEAN` and 4/4 `preparation` checks SUCCESS, all
  completed 21:44 EST (`preparation windows-latest` x2, `preparation ubuntu-latest` x2), with `origin/main`
  unmoved at `97d831bd` and `refs/pull/22/head` still absent, so #19 is the only guarded route (PRs #3 and
  #20 were closed earlier by the owner). The reconciled superset head per D394/D398/D403 is not yet met:
  #19's head carries runtime-a content and the D-record chain through D404 but still lacks runtime-b's
  `9957144` + `40af26d`, main's D405 record `c438e75` and post-D405 main content, so merging now would be the
  silent-drop pattern the records guard against; the 22:40 tick is the next reconciliation window and only
  then should the merge land. The tick's earlier `mergeResult` ("pending: not cleanly mergeable: unstable")
  was state-read before CI completed, so the delay is the missing-lane-content reconciliation, not the check
  gate. The owner's manual merge remains off until the reconciled superset head exists (D403 rule). Tempo
  fact: 0 counted advances, stop unchanged at status 10 `0x6B6A6968`, 346 routines / 414 kernel ops.
  No runtime advance.
- 2026-11-05 ops (D406): runtime-b's line/quad comparator extension is in flight and uncommitted at the read
  (`M tools/compare-nv2a-geometry.mjs`, +252/-62, plus a new `?? tests/compare-nv2a-geometry.test.mjs`), and
  its own checkpoint's newest section ("### 3a. Line/quad comparison - executed and exact (D404
  authorization)") is the lane's own claim inside an in-flight record - the committed state is what counts;
  the prior 21:27-cycle instance was interrupted at the D404 pause (`0xC000013A`), an unattended-window
  scheduling fact whose state file's cycle read came from the pre-21:27 instance. runtime-a published two
  lane commits (`1f3f0f5` compiled-artifact probe exonerating the owned macro pattern, 4 files +81;
  `868d00e` pre-registering M3 and recording the reached cluster, docs, 2 files +21) - the graphics cluster
  is REACHED with the free-framebuffer fault blocking its return path, which is pre-registered M3 status and
  NOT an acceptance (the M3 gate remains unmet). No REA re-invocation this cycle; the `595faf0`
  original-binary check covers the implicated order. Tempo fact: 0 counted advances, stop unchanged at
  status 10 `0x6B6A6968`, 346 routines / 414 kernel ops (the control profile's own stop marker remains
  unrelated to the boundary). No runtime advance.

- 2026-11-05 ops (D407): runtime-b's authorized line/quad comparator work is committed as
  `1546a83` (4 files: comparator +317, new test +59, checkpoint +128, `package.json` +1 -1
  wiring the test into `test:runtime`). Committed record on the 1,728-record line/quad
  reference: 960 compared, 768 skipped with stated reasons (384 polygon-mode-1, 384
  polygon-mode-2), 0 forwarded / 0 slope mismatches, `maxSlopeUlps 0`; `node --test`
  7/7, `test:runtime` 209 pass / 1 skip, `check` 490 files. The handoff's "0 skipped" is
  corrected to the committed 768 stated-reason skips. Verified component progress only - no
  boundary move and no newly serviced original request - so it is not a counted advance.
  No runtime advance.
- 2026-11-05 ops (D407): runtime-a records the M2 device-policy core statistics at `2aa9b43`
  (`browser-157-experiment.json`: 12,637 entries, 161 reads all `beforeKnown`, 68 after write,
  12,476 writes, 0 unknown-address reads, 5,298 distinct addresses, 0 status waits), extended
  to both profiles by `1875d94` (control 12,592 entries / 137 reads / 100% `beforeKnown` / 54
  after write / 0 unknown reads). M2 still needs its two boundary advances, so this is M2
  evidence, not M2 acceptance. No runtime advance.
- 2026-11-05 ops (D407): tempo fact - still 0 counted advances. runtime-a's standalone
  compiled-artifact probe (`tests/reg_save_compiled_probe.c`, returns 1 `lifo-correct` under
  gcc 54/54 and as standalone Emscripten Wasm under Node) exonerates both the compiled artifact
  and the owned macro/expansion text, narrowing the live candidate to the lane's own in-browser
  module compilation/binding - a diagnostic narrowing, not a boundary move. The stop is
  unchanged at status 10 `0x6B6A6968`, 346 routines / 414 kernel ops; PR #19 stays the only
  guarded route with the 22:40 tick as the next reconciliation window (D403). No runtime advance.
- 2026-11-05 ops (D408): merge-gate false positive - the 22:40 tick returned `integrated`
  (finished head `39a8fa8`, was `d1cc85a`) and absorbed `c438e75`/`367febd`/`a9724a7`/
  `e74b4d6`/`1875d94`/`2aa9b43`, but omitted `MERGE_MAIN_PR` on its own unanchored marker grep
  (`^\+.*(<<<|===|>>>)`), which matched static-in-cell JavaScript in the pre-existing D217 row.
  The integrated blob is clean: anchored `^(<<<<<<<|=======$|>>>>>>>)` 0 hits,
  `git diff --check origin/main origin/codex/opencode-architect -- docs/DECISIONS.md` exit 0,
  82+/34-. `origin/main` still `97d831bd`; no merge landed. No runtime advance.
- 2026-11-05 ops (D408): runtime-b in flight and unabsorbed - lane tip `446a9c9` "geometry: match
  the pinned quad point-mode reference" with `M tools/compare-nv2a-geometry.mjs`,
  `M tests/compare-nv2a-geometry.test.mjs` (plus `M docs/RUNTIME_B_SERVICE_CHECKPOINT.md`); the
  `9957144`->`446a9c9` chain is NOT contained in `39a8fa8`, so the D403 superset gate stays open
  for the next tick. Stop unchanged at status 10 `0x6B6A6968`, 346 routines / 414 kernel ops.
  No runtime advance.
- 2026-11-05 ops (D409): blob correction withdrawn - the proposed "587 lines, max D-row 400, no
  D405-D412" reading of the integration blob is the STALE local `refs/pull/19/head` (`44af31d`,
  587 lines, 392 rows, last row D400), not the authoritative remote head `39a8fa8f`
  (`git ls-remote`), which reads 603 lines / 403 D-rows with the `D409`/`D405`/`D410`/`D411` tail
  order and a duplicated `D405` intact. Renumber-at-reconciliation (D400(c)) still applies at #19.
  No runtime advance.
- 2026-11-05 ops (D409): runtime-b tip advanced to `c4e46bd` "geometry: admit the recorded root
  reference prefix" (one commit past `446a9c9`), so the chain
  `9957144`->`40af26d`->`1546a83`->`446a9c9`->`c4e46bd` remains entirely unabsorbed
  (`git merge-base --is-ancestor` exit 1 against both `39a8fa8f` and `origin/main`); the lane cycle
  closed `no_progress` with comparator 10/10 and `test:runtime` 212 pass / 1 skip. The D403
  superset gate stays open and `origin/main` is still `97d831bd`. No runtime advance.
- 2026-11-05 ops (D409): primary card mismatch - runtime-a spent two consecutive cycles on a
  retired marker-repair card (`791f4d7`, then docs-only `c942c58`, published on
  `origin/codex/opencode-source`) although no marker fault exists at the authoritative blob
  (anchored grep 0 hits, `git diff --check` exit 0, 82+/34-); the marker card is retired and the
  D407 in-browser module-binding directive (`e74b4d6`) is still the lane's live objective, with no
  binding divergence fact produced yet. No runtime advance.
- 2026-11-05 ops (D410): R6 corrective - runtime-a landed three consecutive non-source cycles on
  the phantom marker question (`791f4d7` whitespace gate, `c942c58` package record, `5f265217`
  marker-class audit) and its own records conclude "no added marker-class line exists at HEAD and
  the gate passes, so no rewrite/amend applies"; the marker premise is closed at the authoritative
  blob `39a8fa8f` and the lane-a card (`codex/opencode-source` `87d74be`, +1/-1) now carries only
  the D407 in-browser module-binding directive. No runtime advance.
- 2026-11-05 ops (D410): tempo fact - 0 counted advances and the stop unchanged at status 10 at
  `0x6B6A6968`, 346 routines / 414 kernel ops; runtime-b tip `c4e46bd` stays unabsorbed into
  PR #19 (next window ~23:40) and no user presence is needed for that route. No runtime advance.
- 2026-11-05 ops (D411): 23:40 tick landed a new integration head `8940d3f` (was
  `39a8fa8`) in ~93 s (started 23:40:02, finished 23:41:35) with `mainHead` `8339912`
  (main through D410); containment: `39a8fa8`, `8339912` (D410) and `a9724a7` are in,
  while runtime-b `9957144`->`446a9c9`->`c4e46bd`->`3333cc7`->`bddbab1` and runtime-a
  `87d74be`, `7337147`, `677d191`, `2665586` are NOT contained. The tick's own
  "408 rows with zero duplicates" claim is contradicted by its blob (`D405` x2,
  `D376` x2, `D383` x2 at `8940d3f`), so renumbering must be applied at the integration
  head. #19 read MERGEABLE/UNSTABLE with one check IN_PROGRESS; the later re-read shows
  all four checks success and `mergeable_state` clean. runtime-a named the lifter-pop
  divergence (its lane D412): `_lift_pop` "emits `POP32(esp, r)` with the decoded
  register unchanged", runtime/compiled-artifact-only, "routed to the toolkit owner as
  'source-correct, runtime-divergent' (D412)" with no toolkit-owner decision yet.
  No runtime advance.
- 2026-11-05 ops (D411): tempo fact - 0 counted advances and the stop unchanged at
  status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops; the toolkit-owner decision
  on the D412 lifter divergence is the M6 path and the M3 acceptance stays
  `[ESI+0x47C] = 0x20400C post-repair`. No runtime advance.
- 2026-11-06 ops (D412): repair decision (owner-delegated) - the divergence is a one-dword stack-delta
  imbalance between the prologue's push-slot base and the epilogue's pop-slot base. runtime-a's lane
  commit `e8a74a8` ("Capture pop destinations and show the epilogue is one dword low", published as the
  tip of `refs/heads/codex/opencode-source`) shows destinations are the correct `edi, esi, ebp, ebx`
  but they read slots `0x3C10E30..0x3C10E3C` while the prologue saved into `0x3C10E34..0x3C10E40`, so
  every pop is one dword (4 bytes) low and ESI receives the saved-EDI value `0x205E98`. The repair is
  directed to the pipeline frame-setup/emission slot-base alignment (epilogue pop base = prologue push
  base) in runtime-a's OWN lane-owned pipeline files (`tools/prepare-title-recomp.py` emitted `POP`
  block and/or the module-preparation call-frame path; no `.tools`/`research`/fixture edits, no REA
  call, no runtime-engine re-design per R12). Acceptance is M6's gate: the stop must move past
  `0x6B6A6968` in BOTH profiles with R11 evidence (two named browser reports + independent audit JSON,
  zero source mismatches), with `[esi+0x47c]` reading `0x20400c` post-repair as the post-repair
  observable; a non-moving stop routes to the harness call-frame candidate in a new dated row.
  runtime-b's decision request (`6ad911e`, with `bddbab1` and `3333cc7`) is honored: it continues its
  owned materials/comparator work, no new lane-b directive this turn. No runtime advance.
- 2026-11-06 ops (D412): integration state and tempo - #19's head remains `8940d3f` (contained
  `8339912`; NOT contained `87d74be`/`7337147`/`677d191`/`2665586`/`e8a74a8` (runtime-a) and
  `9957144`->`6ad911e` (runtime-b), re-verified with `git merge-base --is-ancestor`); `origin/main`
  unmoved at `97d831bd`; the 00:40 tick must integrate the lane chains and apply the D-rows renumber
  rule before the guarded merge per D394/D398/D403 - `8940d3f`'s blob still shows `D405` twice and
  already carries its own `D412` row, so this row renumbers at integration. Tempo: 0 counted advances
  and the stop unchanged verbatim at status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops.
  No runtime advance.
- 2026-11-06 ops (D413): PR-head blob fact refreshed - after `git fetch origin --prune`,
  `git show refs/pull/19/head:docs/DECISIONS.md` reads **392 D-rows, max row D400, duplicate
  numbers `D376`, `D383`, `D392`** and carries none of rows D401-D412, so merging #19 at its
  current head would regress `docs/DECISIONS.md` from local main's 402 rows / max D412 to
  392 rows / max D400 with 3 duplicate numbers - the exact loss pattern the D403/D407/D411
  rules guard. The earlier "603 lines / 400+ row tails / D405 x2" readings were local-cache
  artifacts and are retracted as such. The repair site is narrowed (D412 refined): runtime-a
  lane commit `7feb88d` "Localize the one-dword deficit to the leftover icall return address"
  shows the indirect-call return word at `0x1FB64C` is not consumed by the resolved callee (or
  the safe path), so the frame is one dword taller than the epilogue's pops assume; destinations
  stay correct but every pop lands one dword low, and the cited safe-path macro file
  `src/runtime/recomp_types.h` lives in the ignored prepared translation (not tracked in the
  branch). runtime-a is directed to implement the icall return-word consumption repair in its
  lane-owned pipeline files (`tools/prepare-title-recomp.py`-driven), with D412's R11 acceptance
  unchanged (stop past `0x6B6A6968` in both profiles with R11 evidence; `[esi+0x47c] = 0x20400c`
  post-repair; the harness call-frame/`ret N` fallback row stays reserved). runtime-b's a/b/c
  trio (`6ad911e`) is answered by the M-milestone order: option (a), record the committed
  comparator work and continue to the next owned step. Tempo: 0 counted advances and the stop
  unchanged verbatim at status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops.
  No runtime advance.
- 2026-11-06 ops (D413): next-tick integration checklist - before any merge the next architect
  tick (00:40 or later) must (i) integrate BOTH divergent chains into the integration head,
  runtime-a `87d74be`->`7feb88d` and runtime-b `9957144`->`6ad911e` (both verified uncontained by
  local main at this record), resolving the D-rows renumber at the integrated blob (local main's
  D401-D412 rows into numeric order, the 3 duplicate numbers removed), (ii) only then execute the
  guarded merge with exact-head green gates, and (iii) record `origin/main`'s move past `97d831bd`
  in its own row. The current tick's directive ("MERGE_MAIN_PR withheld this cycle; PR #19 remains
  open with re-checks on the next integration head") is recorded as deliberately withheld, and this
  D-row authorizes nothing earlier. Tempo: 0 counted advances and the stop unchanged verbatim at
  status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops. No runtime advance.
- 2026-11-06 ops (D414): systemic ABI facts and the runtime-b trio decision - runtime-a's restored ABI
  logger (`c2c60b1`, `8924710`) counts **136 ABI violations / 131 ESI** (first `0x1E04CC`, last
  `0x202150`) with **0 ESP-delta**, so the one-dword deficit (D412) is a systemic symptom and the
  consumed-vs-unconsumed icall return-word contract (D413/B, `0x1FB64C`) stays the repair site; the
  flagged publication gap is re-verified CLOSED (`origin/codex/opencode-source` reads `8924710`).
  Runtime-b trio option (b) is decided by the D393 single-owner rule: `thread_proof-worker.mjs` /
  `proof-report.mjs` are runtime-a-owned, so runtime-b records ABI facts only in its own owned
  checkpoints and routes fact requests through DECISIONS rows; options (a) new-ordinal evidence and
  (c) quad-strip recapture remain valid lane-b work. No runtime advance.
- 2026-11-06 ops (D414): tempo and directive - 0 counted advances, stop unchanged verbatim at
  status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops; the stale tick directive tail (the
  D404-era `FUN_001F6240` register-identity investigation) is superseded by the D412/D413 owner
  decisions, so the next card points runtime-a at the icall return-word repair implementation.
  No runtime advance.
- 2026-11-06 ~00:50 runtime-a (M6 counted advance #1): `ad0bffa` Fix the ordinal-1 return-address
  pop and advance past the fault - the modeled ordinal-1 handler now consumes the call's return
  address, the stop moves from status-10-at-`0x6B6A6968` to status-1-at-`0xFE0000C8` (ordinal 100),
  347/419; control invariant; reports + checkpoint on record; COUNTED. See D429.
- 2026-11-06 ~00:50 ops (D429): 00:40 tick integrated - new integration head `10cae52` (was
  `8940d3f`), `sourceHead ad0bffa` absorbed, runtime-b chain (`9957144`->`854958e`, 10 commits)
  verified unabsorbed, `origin/main` unmoved (`97d831bd`), integration blob `418 rows` / max `D428` /
  duplicate `D370`, `D376`, `D383`, `D405`, `D410` (renumber still pending), markers naming
  `10cae5297…` instead of the real head `10cae5256…`, and PR #19 `OPEN`/`CLEAN`/`MERGEABLE` at
  `10cae52`. Local records only; the tick's own work is orchestration. No runtime advance.
- 2026-11-06 ~01:00 ops (D430): merge-marker-topology defect - the 00:40 tick's `OPEN_MAIN_PR:` /
  `QUEUE_MAIN_PR_MERGE:` markers name `10cae52978585938ac4beee11b4680d1a4ac83c`, which
  `git cat-file -t` rejects ("Not a valid object name"), while the real integration head is
  `10cae52567e2076d36f537b9f7deed6ad1a668f8` (= `refs/pull/19/head`, contains the runtime-a chain);
  the state file's 00:44:34 write carried the bogus hash with `prManagement:
  no_architect_open_request`, so no merge request was ever issued - the runner must re-issue
  against `10cae5256`. Orchestration only. No runtime advance.
- 2026-11-06 ~01:00 ops (D430 tempo): the second counted advance is NOT due - the count stays 1
  (`ad0bffa`); runtime-a's tip is unchanged at `ad0bffa` locally and on
  `origin/codex/opencode-source`, no lane commit has moved the stop past `0xFE0000C8` (advance
  record: status 1 at `0xFE0000C8`, ordinal 100, 347/419), the pre-advance shape remains status 10
  at `0x6B6A6968` / 346 / 414, main's `docs/PROJECT_STATUS.md` still carries the pre-advance text,
  and runtime-b's tip `854958e` shows no new boundary (its in-flight set is 6 interrupt-vector /
  kernel-service files, not the already-committed geometry refactor). No runtime advance.
- 2026-11-06 ~01:05 ops (D431 repeat capture): the module-161 pair re-run is a reproducibility
  capture, not a second counted advance - runtime-a's own record quotes "control unchanged at
  status 13 `0xFD800044` (339/405); experiment advances past the fault to status 1 at
  `0xFE0000C8`, ordinal 100 (`KeDisconnectInterrupt`, argument `0x206030`), 347 routines / 419
  kernel ops", i.e. the SAME numbers D429 counted, from the SAME `browser-161-*` reports
  (mtimes 00:17:17 / 00:29:32, both earlier than the D429 record); the live lane worktree's
  uncommitted `browser-162-*` pair (00:57:23 / 00:59:53) prints the identical stop. The count
  stays 1; no boundary move past `0xFE0000C8` is recorded. No runtime advance.
- 2026-11-06 ~01:05 ops (D431 chain): the merge marker is still mis-issued - the 00:40 tick's
  `lastSummary` carries `OPEN_MAIN_PR:` / `QUEUE_MAIN_PR_MERGE: 10cae52978...`
  (`git cat-file -t` -> "Not a valid object name") while the real `refs/pull/19/head` is
  `10cae52567...` (state `prManagement: no_architect_open_request`, no #24/#25 opened), so the
  D430 re-issue directive stands; the runtime-b chain `9957144`->`854958e` is still unabsorbed
  (containment exit 1 for all nine at `10cae52567`, `ad0bffa` exit 0), the lane-b cycle reads
  `outcome: resume` / `finishedHead 854958e` / `publication: retry` ("Worktree has unfinished
  edits"), the 6-file kernel/service set is still in flight, the comparator accounting is inside
  `854958e`, and runtime-a is at `ad0bffa` with its 00:40:01 cycle in flight. No runtime advance.
- 2026-11-06 ~01:10 ops (D432 lane): runtime-b named and modeled the reached ordinal-100 service in
  `553a0f6` ("service: name and model the reached ordinal-100 KeDisconnectInterrupt", 01:05:07
  -0500), 7 files +236 -3 (`docs/RUNTIME_B_SERVICE_CHECKPOINT.md`, `src/runtime/interrupt_vector.c/.h`,
  `src/runtime/kernel-service.mjs`, 2 tests, `tools/run-native-tests.mjs`); the checkpoint quotes
  `kernel-service+comparator 16/16`, `test:native 58/58`, `test:runtime` 214 pass / 1 skip / 0 fail,
  `check` 490 files, whitespace clean, and the restraint line "No original code was run, no
  ordinal-100 call was served". It is the SAME boundary runtime-a's first advance reached
  (`ad0bffa`): runtime-b quoted that stop rather than re-observing it, is identity-only in the
  runtime-owned report contract, and its handler wiring is runtime-a-owned, so no run advance is
  claimed. Live corrections to the brief: the commit is now published (remote `lane/runtime-b` =
  `553a0f6`, was `854958e`) and the lane worktree is clean, so the erstwhile 6-file in-flight set is
  that commit's content. No runtime advance.
- 2026-11-06 ~01:10 ops (D432 tempo): the count stays **1** - no stop move past `0xFE0000C8`; the
  recorded stop is unchanged at status 1, `0xFE0000C8`, ordinal 100 (the first-advance record),
  347 routines / 419 kernel ops. `origin/main` remains `97d831bd` (local main 37 ahead), the 00:40
  tick's markers still name the bogus `10cae52978...` ("Not a valid object name") so the D430/D431
  re-issue directive stands, PR #19 reads `OPEN`/`MERGEABLE`/`CLEAN` at `10cae52567`, and `553a0f6`
  is unabsorbed. No runtime advance.
- 2026-11-06 ~01:15 ops (D433, three negative checks): the 00:40 tick's `OPEN_MAIN_PR` /
  `QUEUE_MAIN_PR_MERGE` markers still name `10cae52978...` (`git cat-file -t` ->
  "Not a valid object name") while `origin/main` stays `97d831b` and `refs/pull/19/head` reads
  `10cae5256`, so the corrected marker stands at `10cae5256`; the PR head lacks the runtime-b
  chain (`9957144` / `854958e` / `553a0f6` / `eb33cd9`, all containment exit 1) but DOES contain
  the runtime-a tail (`87d74be`...`8924710`, exit 0 - correcting the audit's Check (2)); the
  PR-head `docs/DECISIONS.md` blob is 418 rows / max D428 with `D405 x2` against local main's
  408 rows / max D432 with `D405 x1`. No runtime advance.
- 2026-11-06 ~01:15 ops (D433 divergence and tempo): the divergence is bidirectional - local main
  (`37551c9`) contains neither `ad0bffa` nor `10cae5256` (exit 1 each), merge-base(main, PR19 head)
  = `1275473`, main-only = 4 commits (D429-D432 doc rows), PR19-only = 57 commits (source chain),
  so a reconciled superset needs BOTH lines with the D-row renumber applied at the integrated blob.
  Tempo: count stays 1 - stop unchanged at status 1, `0xFE0000C8`, ordinal 100 (the first-advance
  record), 347 routines / 419 kernel ops; the runtime-b "stop move" quote is runtime-a's D429-era
  record transcribed verbatim. No runtime advance.
- 2026-11-06 ~01:30 runtime-a counted advances #2 AND #3: d721550 (0xFE0000C8 → 0x004F7A0 placeholder,
  modeled ordinal-100) and a2a83de (0x004F7A0 → 0x004C2E0 at 348/421, FUN_0004f7a0 admitted;
  module 163; 3 browser-163 reports as R11 evidence); both-profile evidence; COUNTED.
- 2026-11-06 ~01:35 ops (D434 integration-state gap): the integration head `10cae5256` absorbs the
  runtime-a chain only up to `8924710` - `d721550` and `a2a83de` exit 1 on `git merge-base
  --is-ancestor` - and the runtime-b chain `9957144`→`eb33cd9` is likewise unabsorbed; the D-rows
  renumber is unapplied at that blob (`D405 x2`) and the tick's bogus marker
  `10cae52978585938ac4beee11b4680d1a4ac83c` is still uncorrected at the tick, so the machine's own
  ticks still absorb nothing of the lane tails and those two items are what remains outstanding at
  the integration side. No runtime advance.
- 2026-11-06 ~01:50 runtime-a counted advance #4: 91e5e0b (0x004C2E0 → 0x004E480, 350/422, FUN_0004c2e0 admitted). COUNTED.
  The 01:40 tick's two D433 checklist items landed (D-rows renumber at `9158e70`: 419 rows / max D434 /
  `D405` x1 / zero duplicates, bogus marker spellings 0 hits; runtime-a absorption `7feb88d` / `d721550` /
  `a2a83de` / `ad0bffa` / `b26840c` all exit 0), while its merge-withhold reason does NOT reproduce
  (`git diff --check origin/main 9158e70` exits 0 with empty output; `origin/main` still `97d831bd`) and its
  own `MERGE_MAIN_PR` was withheld (`prManagement no_architect_open_request`). `91e5e0b` is one commit past
  the tick head (`git merge-base --is-ancestor 91e5e0b 9158e70` exits 1).
- 2026-11-06 ~01:50 ops (runtime-b absorption, the hung item): `git merge-base --is-ancestor <c> 9158e70`
  exits 1 for all eleven commits `9957144` → `eb33cd9` (lane tip `eb33cd9`), so the D403 reconciled-superset
  gate stays open and the machine's next tick is the named window for that absorption. No runtime advance.
- 2026-11-06 ~02:05 ops (D436, the EOF-withhold is a stale premise, not a gate failure): the 01:43:38 tick's own
  `lastSummary` withheld `MERGE_MAIN_PR` and every PR marker on "`git diff --check origin/main HEAD` still flags
  'new blank line at EOF' on `docs/DECISIONS.md`", but that check reads CLEAN on the head it named:
  `git diff --check origin/main 9158e70` -> exit 0 (empty output), `git diff --check 97d831bd 9158e70` -> exit 0, and
  `git show 9158e70:docs/DECISIONS.md` (419 rows / max D434) ends with a single trailing newline after the last D-row
  (no blank line at EOF). The withhold is a stale machine-side premise; the next tick must recompute the check in its
  own integration worktree, land `MERGE_MAIN_PR` at the live head if it exits 0, re-issue the marker against that
  head's exact hash (not the bogus `10cae52978…` / `10cae52967…` spellings) and absorb the runtime-b chain
  `9957144` -> `baf0365` (13 commits; all twelve ancestor reads exit 1 against `9158e70`). No runtime advance.
- 2026-11-06 ~02:05 tempo: 4 counted advances - runtime-a's newest counted boundary stays `91e5e0b` (advance #4:
  status 2 at `0x004E480`, 350 routines / 422 kernel ops; no new lane commit since); runtime-b's published lane tip
  `baf0365` ("service: name the reached interrupt field contract; pin the Irql gap") is service/contract work and not a
  runtime-boundary move. The integration head `9158e70` still carries neither `91e5e0b` nor the runtime-b chain.
  No inflation. No runtime advance.
- 2026-11-06 ~02:15 verify (D437, counted advance #5 at `6fdcd98`): this pass is record verification, not execution.
  The runtime-a lane's *committed* record carries the both-profile pair verbatim - "control 339/405 at `0xFD800044`;
  inert 347/419 at `0xFE0000C8` ordinal 100; experiment executes the initializer and stops at status 2, `0x004B160`
  (352 routines / 422 kernel ops)" - so the batch rule (<=8 evidenced related reached functions, applied at 7:
  `FUN_0004e480` + six direct callees; profile 90: 472 functions / 19,329 instructions; module 165: 1,013,193 bytes)
  produced a NEW observed stop boundary with both-profile evidence. The count moves 4 -> 5. No runtime advance.
- 2026-11-06 ~02:15 tempo: 5 counted advances; the stop is status 2 / `0x004B160` / 352 routines / 422 kernel ops
  (runtime-a's committed record). Runtime-b's published tail `3f6c3ea` ("service: serve the reached interrupt
  initialization's Irql argument") is service/contract work - its own line: "No run advance is claimed" - and is not
  counted. The integration head `9158e70` still carries neither `6fdcd98` nor `3f6c3ea` (both ancestor reads exit 1),
  `origin/main` is unmoved at `97d831bd`, and the D436 fresh-check directive is the named next step.
  No runtime advance.
