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