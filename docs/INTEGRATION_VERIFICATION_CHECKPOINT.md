# Integration verification checkpoint

Independent verification lane. Owned paths: `tools/verify-*`, `tools/check-*`,
`tests/integration-*`, `tests/verification-*` and this file. The lane does not
touch the runtime, the general status files, the model/scheduler scripts or
builder-owned tests, does not read private research, does not delegate, and does
not merge or push. It records what is mechanically checkable and leaves every
decision to the architect.

## PR #7 exact-head evidence and merge outcome (November 5, 2026)

### Pre-merge head/base SHA equality

| Item | Value |
| --- | --- |
| PR #7 | `codex/opencode-architect@2d6f770ea680b74c28bd96e8eb5e93a1384b4702` -> `main@c543720d5cf838e80f645de15c85789f2bcbd44c` |
| Head equals the architect branch head | yes: `codex/opencode-architect` and `origin/codex/opencode-architect` are both `2d6f770ea680b74c28bd96e8eb5e93a1384b4702` |
| Base equals the published main at PR creation | yes: published `main` was `c543720d5cf838e80f645de15c85789f2bcbd44c` |
| PR size | 74 commits, 65 changed files, +5377/-111 |
| Opened / closed | created 2026-11-05T04:47:53Z, updated 2026-11-05T06:41:12Z |

### Green preparation check-runs at the exact head (read-only, GitHub API)

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `41975739180` | `push`, branch `codex/opencode-architect` | `completed` / **success** (05:42:54Z -> 05:43:42Z) | `127074034541` "preparation (ubuntu-latest)" success; `127074035031` "preparation (windows-latest)" success |
| `41975745063` | `pull_request` | `completed` / **success** (05:42:57Z -> 05:43:47Z) | `127074054986` "preparation (windows-latest)" success; `127074055144` "preparation (ubuntu-latest)" success |

Both runs report head `2d6f770ea680b74c28bd96e8eb5e93a1384b4702`.

### `verify-exact-head` pass at `2d6f770`

`node tools/verify-exact-head.mjs 2d6f770` -> **exit 0**: the candidate is the
architect head; the published base (now the merge commit `4228721`, whose two
sides are both in the head's history) is **parents-contained**, head ahead 0 /
behind 1, **0 missing base paths**; 413 tracked paths at the head. Running the
tool against the merge commit itself (`4228721`) exits 1 with "not the
architect head", which is structural: a merge commit lives on `main` while the
architect branch head stays at `2d6f770`.

### Merge outcome

| Item | Value |
| --- | --- |
| PR #7 | **merged** at 2026-11-05T06:41:12Z |
| Merge commit | `42287210e508575e82951fbd8dd2f9b413721bce` "Merge reviewed architect integration", parents `c5437200…` and `2d6f770e…` |
| Published main | `main` -> `42287210e508575e82951fbd8dd2f9b413721bce` |
| Merge content delta | `git diff 2d6f770 4228721` changes **no** path |
| CI at the merge commit | run `41982077791` "Preparation checks", event `push`, branch `main`, `completed` / **success** (06:41:14Z -> 06:42:04Z); jobs `127094245244` "preparation (ubuntu-latest)" success, `127094244913` "preparation (windows-latest)" success |

### The verification record is now published

The lane head `92b84bb` is an ancestor of both `2d6f770` and the merged main
`4228721`: the architect integrated this lane's work, and the published line
now carries `docs/INTEGRATION_VERIFICATION_CHECKPOINT.md` (blob `e1cf2149`),
`tools/verify-exact-head.mjs` (blob `64f4f86a`) and
`tests/verification-exact-head.test.mjs` (blob `a1acb07f`). The lane-publication
gap recorded in the previous cycle is therefore closed through the architect's
integration. The lane's own published ref `origin/codex/opencode-verification`
is still the pre-rebase `077ee14`, so the runner's plain lane push still cannot
fast-forward it; the content reached the published line by the integration
route instead.

### Lane resync and one owned fix in this cycle

- The lane branch was fast-forwarded onto the published integration head
  `2d6f770` (a plain fast-forward, no merge commit, no history rewrite; the
  lane's earlier commits stay in that history). This picks up the architect's
  CI identity fix (`d0c38c2`) to `tests/integration-verification.test.mjs`,
  which the lane would otherwise have replayed unchanged.
- The lane's own standalone `tests/verification-exact-head.test.mjs` created its
  temporary merge commits **without an explicit identity**, the same ambient
  git-identity dependency the architect removed from the CI-wired sibling file.
  The lane now passes `-c user.email=… -c user.name=…` on those two merge
  calls as well. Both test files pass with `GIT_CONFIG_GLOBAL` and
  `GIT_CONFIG_SYSTEM` pointed at non-existent files, so neither depends on a
  machine-level git identity. A local reproduction of an identity-less merge
  succeeded here through git's hostname fallback, so the original CI failure was
  environment-dependent rather than universal; the dependency is removed
  regardless.
- The standalone file is not listed in `package.json`'s `test:runtime`, and
  that file is outside this lane's ownership, so CI exercises only the wired
  sibling file. The lane has not changed `package.json`.

### Evidence limits

- The merge, CI and pull-request facts are a read-only observation of the public
  GitHub API without credentials on 2026-11-05.
- The lane resync and the identity fix are local, verified changes. The
  architect's integration of them is its own action.
- The `0x1FCCB0` and `0x1F6730` admission work is the primary lane's reviewed
  result. This lane checked head identity, ancestry and CI only.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs 2d6f770` -> exit 0.
- `node tools/verify-exact-head.mjs 4228721` -> exit 1 (merge commit, not the
  architect head; expected).
- `git merge-base --is-ancestor 92b84bb 4228721` -> 0 (the lane record is in the
  published main).
- `git diff --name-only 2d6f770 4228721` -> no path.
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass, also with
  `GIT_CONFIG_GLOBAL`/`GIT_CONFIG_SYSTEM` suppressed.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass, also with
  the global/system git config suppressed.
- `npm run test:runtime` -> 196 tests, 195 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 413 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request; the same command checks
that head against the then-current architect head and the published merge
target. No further tooling work is authorized until the architect asks for it.

## Post-merge state after PR #6 (November 4, 2026)

**PR #6 is merged.** The runner merged the reviewed architect head into GitHub
`main` while this lane was recording, so the exact-head record now has a
post-merge state to capture.

| Item | Value |
| --- | --- |
| PR #6 | `codex/opencode-architect@b5294fac8de4a68a24eb3b207a6466695dde9fd6` -> `main@2aba6380bc66e82d6e181d61166a9b84b0ef7e4e`; **merged** 2026-11-04T23:41:00Z |
| Merge commit | `c543720d5cf838e80f645de15c85789f2bcbd44c` "Merge reviewed architect integration", parents `2aba6380…` and `b5294fac…` |
| Published main | `main` -> `c543720d5cf838e80f645de15c85789f2bcbd44c`, 373 tracked paths |
| Merge content delta | `git diff b5294fa c543720` is **empty**; the merge added nothing beyond the reviewed candidate head |
| Published main contains | `b5294fac…` (the reviewed head) and `e700373…` (local main) |
| Published main does not contain | the verification record (`92f0abf` is not an ancestor of `c543720`); PR #6 merged the architect branch only |
| Open pull requests | only PR **#3**, the geometry draft; PR #6 is closed and merged |

### CI at the merged head (read-only, GitHub API, 2026-11-04T23:47Z)

| Item | Value |
| --- | --- |
| Merge run | id `41935647475`, "Preparation checks", event `push`, branch `main`, attempt 1, `completed` / **success** (created 23:41:03Z, updated 23:41:53Z) |
| Merge run jobs | `126943303039` "preparation (ubuntu-latest)" success; `126943302774` "preparation (windows-latest)" success |
| Gate runs at the candidate head | `41927475606` (push) and `41927492924` (pull_request), both `success` — the runs that gated PR #6 |
| URL | https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41935647475 |

### Post-merge precheck

- `node tools/verify-exact-head.mjs b5294fa --base-ref origin/main` -> exit 0.
  The published base is now the merge commit `c543720`, whose two sides
  (`2aba6380…` and `b5294fac…`) are both in the candidate head's history: the
  relation is **parents-contained**, head ahead 0 / behind 1, **0 missing base
  paths**.
- `node tools/verify-exact-head.mjs c543720` -> exit 1 with "candidate
  c543720 is not the architect head b5294fa". This is structural, not a
  failed candidate: the merge commit lives on `main`, while the architect
  branch head is still `b5294fa`. The tool's identity rule asks whether the
  commit under review is the architect head, and a merge commit on the
  published line is not.

### Lane-publication gap: what the runner can and cannot do

The runner publishes a lane branch with a plain, **non-force** push
(`git push origin HEAD:refs/heads/<branch>`, `tools/Invoke-OpenCodeCycle.ps1`
line 52). This lane's branch was rebased onto the integration head, so the
published lane head `origin/codex/opencode-verification` (`077ee14`) is not an
ancestor of the lane head; that push is a non-fast-forward and the runner will
record `publication = 'retry'`. This lane does not merge and does not push, and
the sandbox denies a merge command for it.

Everything else the runner's publish step requires has been verified locally:

| Runner publish precondition | Result |
| --- | --- |
| Current branch is `codex/opencode-verification`, clean worktree | yes |
| Lane ownership: `origin/main..HEAD` touches only `tools/verify-exact-head.mjs`, `tests/integration-verification.test.mjs`, `tests/verification-exact-head.test.mjs`, `docs/INTEGRATION_VERIFICATION_CHECKPOINT.md` | yes, no other path |
| No forbidden publication path in that set | yes |
| `npm run check` | passes (376 source files in this tree) |
| `git diff --check origin/main..HEAD` | clean |
| Fast-forward of the integration branch onto the lane head | possible: `b5294fac…` **is** an ancestor of the lane head, and the delta over it is exactly the four owned paths |

So the record can reach the published line without a force push by
fast-forwarding the integration branch `codex/opencode-architect` onto the lane
head in the architect worktree; the runner then publishes that branch normally.
That action belongs to the architect or the runner, not to this lane.

### Evidence limits

- The merge, CI and pull-request facts are a read-only observation of the
  public GitHub API without credentials at 2026-11-04T23:47Z.
- The lane-publication resolution above is a local, verified fact about
  ancestry and the runner's publish preconditions. It is a recommendation with
  the evidence attached, not an executed publication.
- The `0x1FCCB0` admission slice is the primary lane's reviewed result. This
  lane checked head identity, ancestry and CI only, and does not judge that
  evidence.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs b5294fa --base-ref origin/main` -> exit 0.
- `node tools/verify-exact-head.mjs c543720` -> exit 1 (merge commit, not the
  architect head; expected).
- `git merge-base --is-ancestor b5294fa HEAD` -> 0 (fast-forward onto the lane
  head is possible).
- `git diff --name-only b5294fa HEAD` -> the four owned paths only.
- `git diff --check origin/main..HEAD` -> clean.
- `npm run test:runtime` -> 188 tests, 187 pass, 0 fail, 1 pre-existing skip.
- `npm run check` -> publication guard passed for 376 source files.
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the bounded
`0x1FCCB0` admission slice once the architect reviews it, or to whichever head
the architect publishes next; the same command checks that head against the
then-current architect head and the published merge target. No further tooling
work is authorized until the architect asks for it.

## Earlier record: exact-head precheck for `b5294fa` (pre-merge, November 4, 2026)

**Recorded marker:** `b5294fac8de4a68a24eb3b207a6466695dde9fd6`
("Record 0x1FC780 admission integration and 0x1FCCB0 dispatcher
characterization"). That commit is the head of `codex/opencode-architect` and of
`origin/codex/opencode-architect`, so the reviewed candidate **is** the published
architect head.

The lane branch was rebased onto that integration head first, as directed, so
this record sits on the same base as the candidate it describes:
`b5294fa` -> `649de14` (exact-head precheck tool and `c9dc22f` record) ->
`ab7710f` (`ae4b7ae` record and base-gate refinement) -> the new record commit.
The rebase was clean; `tests/integration-verification.test.mjs` was unchanged
between the previous base and `b5294fa`.

```
node tools/verify-exact-head.mjs b5294fa      # exit 0
```

### Established locally (source-only, offline)

| Fact | Value |
| --- | --- |
| Candidate commit | `b5294fac8de4a68a24eb3b207a6466695dde9fd6` |
| Architect ref | `codex/opencode-architect` -> `b5294fac8de4a68a24eb3b207a6466695dde9fd6` (match) |
| Published merge target | `origin/main` -> `2aba6380bc66e82d6e181d61166a9b84b0ef7e4e`, relation **ancestor** (head ahead 37, behind 0, missing base paths 0) |
| Local main branch | `main` -> `e70037316c22b1ff10d6e63f43e89cabeb60273c`, contained in the head (head ahead 266, behind 0) |
| Tracked paths at the head | 373 |
| Local conflict probe | `git merge-tree --write-tree origin/main b5294fa` -> tree `ff0be3d39d90dc9b8875cd75bc3227e2389b640c`, exit 0 (no conflict) |
| Head delta over published main | 37 commits, 15 changed paths, all documentation or lane tooling |

Unlike the previous head, the published target `origin/main` is now a **strict
ancestor** of the candidate, and no published path is missing from it.

### Exact-head CI evidence (read-only, GitHub API, 2026-11-04T22:48Z)

| Item | Value |
| --- | --- |
| Push run | id `41927475606`, "Preparation checks", event `push`, branch `codex/opencode-architect`, attempt 1, `completed` / **success** (created 22:41:32Z, updated 22:42:29Z) |
| Push run jobs | `126915535340` "preparation (ubuntu-latest)" success; `126915534912` "preparation (windows-latest)" success |
| Pull-request run | id `41927492924`, "Preparation checks", event `pull_request`, attempt 1, `completed` / **success** (created 22:41:40Z, updated 22:42:25Z) |
| Pull-request run jobs | `126915596355` "preparation (ubuntu-latest)" success; `126915595321` "preparation (windows-latest)" success |
| Run head | `b5294fac8de4a68a24eb3b207a6466695dde9fd6` (the exact candidate) in both runs |

### Pull request state at that moment (read-only)

- PR **#6** `codex/opencode-architect@b5294fac8de4a68a24eb3b207a6466695dde9fd6`
  -> `main@2aba6380bc66e82d6e181d61166a9b84b0ef7e4e`: **open, not a draft**,
  `mergeable: true`, `mergeable_state: clean`, 37 commits, 15 changed files,
  +404/-12, created 2026-11-04T22:41:35Z.
- PR **#5** remains the merged predecessor (`becc879` -> `main`, merged
  2026-11-04T21:39:32Z); PR **#3** is still the open geometry draft; PR **#2**
  and **#1** are closed.
- https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/6

So all three gates the architect listed for this head are now **observed** at
the exact SHA: CI green on both operating systems for both the push and the
pull-request runs, the published target strictly contained in the head, and the
pull request reported clean/mergeable. Whether to promote it is the
architect's decision; this lane records the evidence and requests nothing.

### Gates that remain external (listed by the tool, never asserted here)

The tool prints these as requirements for a reviewer, not as results:

- `ci:` GitHub Actions "Preparation checks" on `ubuntu-latest` reports success
  at the exact head SHA.
- `ci:` GitHub Actions "Preparation checks" on `windows-latest` reports success
  at the exact head SHA.
- `pr:` the pull request for the exact head is open, not a draft, and reported
  clean/mergeable.
- `marker:` the architect issued an exact-head marker for this SHA. The tool
  neither issues nor accepts one.
- `publication:` the source publication guard and whitespace gate are re-run
  against the exact head.

The CI and PR lines above were read once from the public GitHub API without
credentials. A green local precheck means "the mechanically observable part
holds", not "this candidate is approved".

### Evidence limits

- The precheck reads refs, commit metadata and path counts only. It reads no
  original content, no private research artifact, no XBE and no title module,
  and it executes no guest code.
- The CI and PR observations are a snapshot read at 2026-11-04T22:48Z. They
  describe those runs and that pull-request state only.
- The mergeability probe is a local `git merge-tree` conflict test; the
  `mergeable_state: clean` line is GitHub's own report for PR #6.
- The `0x1FC780` admission evidence and the `0x1FCCB0` characterization and
  admission set are the primary lane's reviewed result. This lane checked the
  head identity, ancestry and CI of that head only; it did not review, replay
  or approve the admission evidence itself, and it does not judge the pending
  `0x1FCCB0` translator-resync proof.
- Node `v24.22.1` and npm `11.19.1` are inside the declared ranges but are not
  the exact `24.14.0` runtime pin.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim. No merge request
  was made and no source or runtime file was changed by this lane.

### Checks run for this result

- `git rebase b5294fa` -> clean; lane branch is now `b5294fa` plus the two
  replayed lane commits.
- `node tools/verify-exact-head.mjs b5294fa` -> exit 0 (exact head matches the
  architect head; published target is a strict ancestor with no missing paths).
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 188 tests, 187 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 376 source files (373 at the
  candidate head plus this lane's three new paths).
- `git diff --check` -> clean.

Re-verification cycle (2026-11-04T23:30Z), same result set re-run:

- `node tools/verify-exact-head.mjs b5294fa` -> exit 0 again.
- `git merge-base --is-ancestor 077ee14 2309251` -> 1, and the reverse also 1
  (the published lane branch and the rebased local branch diverge at
  `c9dc22f`).
- `npm run test:runtime` -> 188 tests, 187 pass, 0 fail, 1 pre-existing skip.
- `npm run check` -> publication guard passed for 376 source files.

### Re-verification of this record (2026-11-04T23:30Z)

The lane task and the architect directive for this cycle were the ones already
delivered by commit `2309251` (rebase onto `b5294fa`, precheck, CI record, no
source or runtime change, no merge request). The recorded facts were
re-read rather than assumed, and nothing in them has moved:

- `codex/opencode-architect` and `origin/codex/opencode-architect` are still
  `b5294fac8de4a68a24eb3b207a6466695dde9fd6`, so the recorded exact head is
  still the published architect head. `node tools/verify-exact-head.mjs
  b5294fa` still exits 0.
- The published `main` is still `2aba6380bc66e82d6e181d61166a9b84b0ef7e4e`, so
  the recorded ancestry still holds.
- PR **#6** is still open, not a draft, `mergeable: true`,
  `mergeable_state: clean`, head `b5294fac…`, base `2aba6380…`, with its
  `updated_at` still 2026-11-04T22:41:35Z.
- The same two runs are still the only runs at that SHA (`41927475606` push,
  `41927492924` pull_request), both `success`, with no new run.
- The primary lane advanced to `f897c7ee85f6dcd41e27f2c68988b202632131ab`
  ("Preflight the shared 0x1FC980 seed read-only"). That is a source-lane
  commit, not an integration head, so it does not move this record.

### Publication finding for the architect

The published lane branch is still `origin/codex/opencode-verification` at
`077ee14`, the **pre-rebase** head. The rebased lane commits (`649de14`,
`ab7710f`, `2309251`) are local only: `077ee14` is not an ancestor of
`2309251` and the two diverge at `c9dc22f` (2 commits published-only, 47
local-only). Publishing the rebased lane branch would therefore rewrite
published history, which the project's no-force-push rule does not allow, and
this lane does not push. The architect or runner has to decide how the
verification record reaches the published lane branch; until then the
`b5294fa` exact-head record exists only in this worktree.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the bounded
`0x1FCCB0` admission result once the primary reports it, or to whichever head
the architect publishes next; the same command checks that head against the
then-current architect head and the published merge target. No further tooling
work is authorized until the architect asks for it.

## Earlier record: exact-head precheck for `ae4b7ae` (November 4, 2026)

**Recorded marker:** `ae4b7ae6b3332f67104eb812470d785dc5caad87`
("Record 0x1F6EB0 evidence-slice integration and request main PR at the merged
head"). That commit is the head of `codex/opencode-architect` and of
`origin/codex/opencode-architect`, so the reviewed candidate **is** the published
architect head.

```
node tools/verify-exact-head.mjs ae4b7ae      # exit 0
```

### Established locally (source-only, offline)

| Fact | Value |
| --- | --- |
| Candidate commit | `ae4b7ae6b3332f67104eb812470d785dc5caad87` |
| Architect ref | `codex/opencode-architect` -> `ae4b7ae6b3332f67104eb812470d785dc5caad87` (match) |
| Published merge target | `origin/main` -> `2aba6380bc66e82d6e181d61166a9b84b0ef7e4e`, relation **parents-contained** (head ahead 23, behind 1, missing base paths 0) |
| Local main branch | `main` -> `e70037316c22b1ff10d6e63f43e89cabeb60273c`, contained in the head (head ahead 251, behind 0) |
| Tracked paths at the head | 371 |
| Local conflict probe | `git merge-tree --write-tree origin/main ae4b7ae` -> tree `e9c6b8575c9c80e06bddf69d92ee262ed3130523`, exit 0 (no conflict) |

`origin/main` is now the merge commit `2aba638` of `b62989f` (previous main) and
`becc879` (this branch), so the target commit itself is not an ancestor of the
head. Both of its sides **are** in the head's history and no path tracked by
`origin/main` is missing from the head, which is the "parents-contained"
relation the tool reports.

### Exact-head CI evidence (read-only, GitHub API, 2026-11-04T21:48Z)

| Item | Value |
| --- | --- |
| Workflow run | id `41918362517`, "Preparation checks", event `push`, branch `codex/opencode-architect` |
| Run head | `ae4b7ae6b3332f67104eb812470d785dc5caad87` (the exact candidate), attempt 1 |
| Run state | `completed`, conclusion **success**; created 2026-11-04T21:41:02Z, updated 2026-11-04T21:41:54Z |
| Jobs | `126883968837` "preparation (ubuntu-latest)" success; `126883969767` "preparation (windows-latest)" success |
| URL | https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41918362517 |

This is a read-only observation of the public API without credentials at the
recorded time. It is a snapshot of that run, not a statement about any later
run, and it does not assert the review outcome of the candidate.

### Pull request state at that moment (read-only)

- PR **#5** `codex/opencode-architect@becc879` -> `main@b62989f`: closed and
  **merged** 2026-11-04T21:39:32Z. This is what advanced `origin/main` to
  `2aba638`.
- PR **#2** `codex/opencode-source@de1e59a` -> `codex/opencode-architect@becc879`:
  closed (no longer a draft); the architect records it as now fully contained
  in the integration.
- PR **#3** `codex/opencode-geometry@c12d17a`: open draft, unchanged.
- **No main pull request is open for `ae4b7ae` yet.** The architect's
  `OPEN_MAIN_PR` marker for that reviewed head has not produced an open main
  PR at the time of this record, so the "PR open / non-draft / clean /
  mergeable" gate for the new main PR is still **unobserved**, not failed.

### The `becc879f` request is superseded

The lane task named published head `becc879f`; the architect's current
directive names `ae4b7ae`, and the architect prevails on that conflict.
`becc879f` is an ancestor of `ae4b7ae`, so it is a superseded head: running the
tool against it today reports "candidate is not the architect head" and exits 1,
which is the expected result for an old head rather than a defect. For
completeness, the CI at that head was also green (run `41916226715` `push` and
run `41916376757` `pull_request`, both success), and the architect already
recorded that head's precheck result in `e1804fb`.

### Tool change in this cycle

The base gate now distinguishes three relations instead of one strict
ancestor test, and adds a path-level check:

- `ancestor` - the published target is itself in the head's history;
- `parents-contained` - the target is a merge commit and both of its sides are
  in the head's history;
- `divergent` - anything else, which fails the check.

A head also fails when it does not track a path that the published target
tracks. Without this, the strict ancestor test reported a false failure for
`ae4b7ae`, because `origin/main` was created by merging this very branch.

### Gates that remain external (listed by the tool, never asserted here)

The tool prints these as requirements for a reviewer, not as results:

- `ci:` GitHub Actions "Preparation checks" on `ubuntu-latest` reports success
  at the exact head SHA.
- `ci:` GitHub Actions "Preparation checks" on `windows-latest` reports success
  at the exact head SHA.
- `pr:` the pull request for the exact head is open, not a draft, and reported
  clean/mergeable.
- `marker:` the architect issued an exact-head marker for this SHA. The tool
  neither issues nor accepts one.
- `publication:` the source publication guard and whitespace gate are re-run
  against the exact head.

No GitHub request was made from this lane to decide anything; the CI and PR
lines above are read-only observations recorded as evidence. A green local
precheck means "the mechanically observable part holds", not "this candidate is
approved".

### Evidence limits

- The precheck reads refs, commit metadata and path counts only. It reads no
  original content, no private research artifact, no XBE and no title module,
  and it executes no guest code.
- The CI and PR observations come from the public GitHub API without
  credentials, read once at 2026-11-04T21:48Z. They are a snapshot; a later run
  or a later pull-request state is not covered by this record.
- The mergeability probe is a local `git merge-tree` conflict test between
  `origin/main` and the head. It is not GitHub's mergeability verdict and it
  does not replace the `pr:` gate.
- The `0x1F6EB0` admission evidence, the `0x1FC780` characterization and the
  `0x1FC780` admission set are the primary lane's reviewed result. This lane
  checked the head identity, ancestry and CI of that head only; it did not
  review, replay or approve the admission evidence itself.
- Node `v24.22.1` and npm `11.19.1` are inside the declared ranges but are not
  the exact `24.14.0` runtime pin.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim. No main merge
  request was made and no source or runtime file was changed by this lane.

### Checks run for this result

- `node tools/verify-exact-head.mjs ae4b7ae` -> exit 0 (exact head matches the
  architect head; published target parents-contained with no missing paths).
- `node tools/verify-exact-head.mjs becc879f` -> exit 1 (superseded head, not
  the current architect head).
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 188 tests, 187 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 367 source files (the lane's
  own tree; the architect head itself tracks 371 paths).
- `git diff --check` -> clean.

### Stop condition of that cycle

Stop after this document-and-test commit. The next exact-head precheck applies
to the bounded `0x1F6EB0` helper result once the primary reports it; the same
command checks that head against the then-current architect head and the
published merge target. No further tooling work is authorized until the
architect asks for it. That result is the record above.

## Earlier record: exact-head precheck for `c9dc22f` (November 4, 2026)

**Recorded marker:** `c9dc22f9f34c86e9403ef5ca22bc9911658c73bc`
("Record review-complete plain union candidate"). At the time of this check that
commit was the head of `codex/opencode-architect` and of
`origin/codex/opencode-architect`, so the reviewed candidate **is** the architect
head.

The check is one command:

```
node tools/verify-exact-head.mjs c9dc22f
```

It exits 0 when the candidate resolves, is identical to the architect branch
head, and already contains the published merge target. It exits 1 when one of
those locally observable preconditions fails, and 2 for an unresolved reference
or a usage error. The report prints an exact-head marker line only when the
candidate really is the architect head; a candidate that is not the architect
head is reported with `exact-head marker: none`.

### Established locally (source-only, offline)

| Fact | Value |
| --- | --- |
| Candidate commit | `c9dc22f9f34c86e9403ef5ca22bc9911658c73bc` |
| Architect ref | `codex/opencode-architect` -> `c9dc22f9f34c86e9403ef5ca22bc9911658c73bc` (match) |
| Published merge target | `origin/main` -> `b62989f70d15a18c8f6ef9a4796d4d54b538bb68`, contained in the head (head ahead 132, behind 0) |
| Local main branch | `main` -> `06cfc309db0765c2edb7e208cc7a4d90058bce9f`, **not** contained in the head (head ahead 228, behind 3) |
| Tracked paths at the checked head | 364 |
| Publication guard at that head | `npm run check` -> "passed for 364 source files" (367 with this lane's three new paths) |
| Whitespace over the published range | `git diff --check origin/main..c9dc22f` -> clean |

### Gates that remain external (listed by the tool, never asserted here)

The tool prints these as requirements for a reviewer, not as results:

- `ci:` GitHub Actions "Preparation checks" on `ubuntu-latest` reports success
  at the exact head SHA.
- `ci:` GitHub Actions "Preparation checks" on `windows-latest` reports success
  at the exact head SHA.
- `pr:` the pull request for the exact head is open, not a draft, and reported
  clean/mergeable.
- `marker:` the architect issued an exact-head marker for this SHA. The tool
  neither issues nor accepts one.
- `publication:` the source publication guard and whitespace gate are re-run
  against the exact head.

No GitHub request was made from this lane, so **none of those gates is asserted
here**. A green local precheck means "the mechanically observable part holds",
not "this candidate is approved" and not "CI is green".

### New local finding for the architect

Local `main` (`06cfc30`) carries three commits that the architect head does not
contain:

- `06cfc30` "Defer publishing unfinished builder work and report dirty lanes"
  (`tools/Invoke-OpenCodeCycle.ps1`, `tools/Invoke-OpenCodeManager.ps1`);
- `87b1b3f` "Add GLM Flash manager and three distinct low-cost builder lanes"
  (`.opencode/*`, `AGENTS.md`, `docs/DECISIONS.md`, `docs/OPENCODE_GOAL.md`,
  `docs/OPENCODE_WORKFLOW.md`, `docs/PROJECT_STATUS.md`,
  `tools/Invoke-OpenCode*.ps1`, `tools/Publish-OpenCodeStatus.ps1`,
  `tools/Register-OpenCodeManagerTasks.ps1`, `tools/opencode-builder-models.json`);
- `86eceb3` "Post architect cycle updates to GitHub issue comments"
  (`docs/OPENCODE_WORKFLOW.md`, `tools/Publish-OpenCodeStatus.ps1`).

They are unpublished local governance/manager commits, so the architect head is
not a descendant of local `main`, and a local fast-forward of `main` to the head
is not possible. The **published** target `origin/main` (`b62989f`) is contained
in the head, so promoting the head into GitHub `main` is ancestry-clean for the
published line. Whether the local governance commits must travel with the head
is an architect decision; this lane only records the divergence. The existing
`tools/verify-integration.mjs` union check (`0b4cae4`) remains the mechanism
that catches dropped decision rows and dropped source files across such a merge,
and it is unchanged by this work.

Local `main` advances independently of the architect head during a cycle, so the
divergence row above is a snapshot taken when this check ran, not a stable
property of the repository.

### Evidence limits

- The precheck reads refs, commit metadata and path counts only. It reads no
  original content, no private research artifact, no XBE and no title module,
  and it executes no guest code.
- CI status, pull-request state and the architect's marker live on GitHub and
  are listed only. The tool is deliberately incapable of asserting them.
- The 364-file `npm run check` figure was produced in a clean working tree at
  `c9dc22f`, and the 367-file figure is the same guard with this lane's three
  new paths. Neither is a detached re-verification of a published SHA.
- Node `v24.22.1` and npm `11.19.1` are inside the declared ranges but are not
  the exact `24.14.0` runtime pin.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim. `0x1F6EB0` and
  `0x1F79A0` remain unadmitted, and no controllable original screen exists.

### Checks run for this result

- `node tools/verify-exact-head.mjs c9dc22f` -> exit 0 (exact head matches the
  architect head; published base contained).
- `node tools/verify-exact-head.mjs HEAD --architect-ref codex/opencode-source`
  -> exit 1 (a candidate that is not the architect head is refused).
- `node tools/verify-exact-head.mjs c9dc22f --base-ref main` -> exit 1 (a head
  that does not contain the chosen base is refused).
- `node --test tests/verification-exact-head.test.mjs` -> 10/10 pass.
- `node --test tests/integration-verification.test.mjs` -> 6/6 pass.
- `npm run test:runtime` -> 187 tests, 186 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE). The same suite before this change
  reported 186/185/0/1, so this adds exactly one wired test.
- `npm run check` -> publication guard passed for 367 source files with this
  lane's three new paths (364 at the checked head `c9dc22f`).
- `git diff --check` -> clean.

### Stop condition of that cycle

Stop after this document-and-test commit. The next exact-head precheck applies
to the bounded `0x1F6EB0` helper result once the primary reports it; the same
command checks that head against the then-current architect head and the
published merge target. No further tooling work is authorized until the
architect asks for it. That result is the record above.
