# Integration verification checkpoint

Independent verification lane. Owned paths: `tools/verify-*`, `tools/check-*`,
`tests/integration-*`, `tests/verification-*` and this file. The lane does not
touch the runtime, the general status files, the model/scheduler scripts or
builder-owned tests, does not read private research, does not delegate, and does
not merge or push. It records what is mechanically checkable and leaves every
decision to the architect.

## PR #11 exact-head evidence at `6bc05f7`, and the `c8c965c..6bc05f7` attribution (November 5, 2026)

### Head/base SHA equality for PR #11 at its current head

| Item | Value |
| --- | --- |
| PR #11 | `codex/opencode-architect@6bc05f766dd23f31130872df2ac3b6316786967e` -> `main@ea6e15e1a5cbe43cc54735f808f0f5c235307293` |
| Head equals the published architect head | yes: `origin/codex/opencode-architect` is `6bc05f766dd23f31130872df2ac3b6316786967e` |
| Base equals the published main | yes: `origin/main` is `ea6e15e1a5cbe43cc54735f808f0f5c235307293`, and it is a **strict ancestor** of the head |
| PR state | open, not a draft, `mergeable: true`, `mergeable_state: clean`, updated 2026-11-05T14:35:59Z |

### Both green check-runs at the exact head (read-only, GitHub API)

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `42046563845` | `push`, branch `codex/opencode-architect` | `completed` / **success** (14:36:00Z -> 14:36:58Z) | `127310984199` "preparation (ubuntu-latest)" success; `127310984627` "preparation (windows-latest)" success |
| `42046577727` | `pull_request` | `completed` / **success** (14:36:05Z -> 14:37:33Z) | `127311032968` "preparation (windows-latest)" success; `127311033638` "preparation (ubuntu-latest)" success |

Both runs report head `6bc05f766dd23f31130872df2ac3b6316786967e`.

### `verify-exact-head` pass at `6bc05f7`

`node tools/verify-exact-head.mjs 6bc05f7` -> **exit 0**: the candidate is the
published architect head; the published base `ea6e15e` is a strict
**ancestor** (head ahead 12, behind 0, **0 missing base paths**); 465 tracked
paths at the head. The local `main` branch is `7452dca` and has **two**
commits the head does not contain; that divergence is recorded for the
architect, not decided here.

### Merge outcome: still pending

PR #11 has **not** merged, so no merge commit exists to record. The
`merge_commit_sha` reported for an open pull request is its *test* merge
commit, not a landed merge, and is deliberately not repeated here as if it were
one. The three promotion gates the architect named - green CI at the exact
head, a current base, and clean mergeability - remain **observed** at
`6bc05f7`.

### Attribution of the `c8c965c..6bc05f7` delta

The architect asked for every commit in that range to be attributed before
integrating. The range is exactly three commits, all lane-orchestration and
routing governance, with no source, runtime, fixture or game-content change:

| Commit | Subject | Paths |
| --- | --- | --- |
| `d8c6fa5` | Use Go GLM Flash for project architect routing | `.opencode/agents/architect_fallback.md`, `.opencode/opencode.jsonc`, `AGENTS.md` |
| `7fb64e1` | Use GLM Flash first in scheduled architect runner | `tools/Invoke-OpenCodeArchitect.ps1`, `tools/opencode-builder-models.json` |
| `6bc05f7` | Use current checkpoints and concise prompts for orchestration | `.opencode/plugins/orchestrator/index.ts`, `AGENTS.md`, `docs/OPENCODE_GOAL.md`, `tools/Invoke-OpenCodeArchitect.ps1`, `tools/Invoke-OpenCodeManager.ps1` |

The local architect branch also carries one further **unpublished** commit on
top of the published head (`edc8540` "Give scheduled Flash architect bounded
review tools", touching `.opencode/agents/architect_primary.md` and
`tools/Invoke-OpenCodeArchitect.ps1`), so the next publication will move the
head past `6bc05f7` and the exact-head evidence will have to be re-read then.

### Tool change in this cycle

The precheck now compares the candidate against **both** the local architect
branch and the published architect head (`origin/codex/opencode-architect`),
and the report names which one matched. Before this change the tool compared
only the local branch, so a published candidate whose branch had unpublished
work on top was reported as a mismatch - a false failure that appeared exactly
this cycle, when the local branch was ahead at `17b5775` while the published
head was `6bc05f7`. A candidate that matches neither reference is still
rejected. The CI/PR/marker gates remain listed and never asserted.

### Publication state

- The lane's PR #9 records (`cc64854`, `ca0b495`) are in the published main
  `ea6e15e`; the lane's later records (`c466018`, `4585f05`) are still
  lane-local and reach the published line when the architect integrates this
  lane branch.
- `origin/codex/opencode-verification` remains the pre-rebase `077ee14`, so the
  runner's plain lane push still cannot fast-forward it; this is the recorded
  runner-side limitation, not a content divergence.

### Evidence limits

- The CI and pull-request facts are a read-only observation of the public
  GitHub API without credentials on 2026-11-05, while PR #11 was open.
- The attribution table lists commit subjects and changed paths read from the
  local repository. It says nothing about whether the orchestration changes are
  correct or desirable; that review is the architect's.
- The vblank/display-mode ratification, the ordinal-159 frontier, the second
  ordinal-2 stop and the documented unknown-option default are the primary and
  architect lanes' reviewed results. This lane checked head identity,
  ancestry, CI, mergeability and commit attribution only.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs 6bc05f7` -> exit 0.
- `git merge-base --is-ancestor ea6e15e 6bc05f7` -> 0 (the PR #11 base is
  contained in the head).
- `git log --oneline c8c965c..6bc05f7` -> three commits, all orchestration or
  routing governance.
- `node --test tests/verification-exact-head.test.mjs` -> 17/17 pass.
- `node --test tests/integration-verification.test.mjs` -> 8/8 pass.
- `npm run test:runtime` -> 200 tests, 199 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 465 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request, and to the merge commit
once it lands; the same command checks that head against the then-current
architect head and the published merge target. No further tooling work is
authorized until the architect asks for it.

## PR #11 exact-head evidence at `7fb64e1` (open, not yet merged) (November 5, 2026)

### Lane and head context

The lane branch advanced to `89922a4` ("Use Go GLM Flash for project architect
routing") before this cycle: that is a routing-governance commit applied to the
lane branch, not lane-owned content. The architect branch carries its own
routing commit `d8c6fa5` under the same subject, so `89922a4` is **not** an
ancestor of the architect head. The architect branch head is now
`7fb64e1dee715c1e698d581ace0019111a3ffce4` ("Use GLM Flash first in scheduled
architect runner"), and PR #11 now points at that head.

### Head/base SHA equality for PR #11

| Item | Value |
| --- | --- |
| PR #11 | `codex/opencode-architect@7fb64e1dee715c1e698d581ace0019111a3ffce4` -> `main@ea6e15e1a5cbe43cc54735f808f0f5c235307293` |
| Head equals the architect branch head | yes: `codex/opencode-architect` and `origin/codex/opencode-architect` are both `7fb64e1dee715c1e698d581ace0019111a3ffce4` |
| Base equals the published main | yes: `origin/main` is `ea6e15e1a5cbe43cc54735f808f0f5c235307293`, and it is a **strict ancestor** of the head |
| PR state | open, not a draft, `mergeable: true`, `mergeable_state: clean` |
| PR size | 11 commits, 27 changed paths, +682/-51, created 2026-11-05T13:42:08Z, updated 2026-11-05T14:24:38Z |

### Both green check-runs at the exact head (read-only, GitHub API)

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `42044734785` | `push`, branch `codex/opencode-architect` | `completed` / **success** (14:24:39Z -> 14:25:52Z) | `127304502709` "preparation (windows-latest)" success; `127304503394` "preparation (ubuntu-latest)" success |
| `42044747022` | `pull_request` | `completed` / **success** (14:24:43Z -> 14:25:45Z) | `127304544723` "preparation (windows-latest)" success; `127304545108` "preparation (ubuntu-latest)" success |

Both runs report head `7fb64e1dee715c1e698d581ace0019111a3ffce4`.

### `verify-exact-head` pass at `7fb64e1`

`node tools/verify-exact-head.mjs 7fb64e1` -> **exit 0**: the candidate is the
architect head; the published base `ea6e15e` is a strict **ancestor** (head
ahead 11, behind 0, **0 missing base paths**); 465 tracked paths at the head.
The local `main` branch is `fb50e2a` ("Route hourly OpenCode architect through
Go GLM Flash") and has **one** commit that the head does not contain; that
divergence is recorded for the architect, not decided here.

### Merge outcome: pending

PR #11 has **not** merged, so no merge commit exists to record. The
`merge_commit_sha` `7e94598b0888b73672d3224ac42a4dc3fb88f458` that GitHub
reports for an open pull request is its *test* merge commit, not a landed
merge, and is noted only so the identifier is not mistaken for one. The three
promotion gates the architect named - green CI at the exact head, a current
base, and clean mergeability - are all **observed** at `7fb64e1`; the promotion
decision stays with the architect.

### Publication state

- The lane's PR #9 records (`cc64854`, `ca0b495`) are in the published main
  `ea6e15e`.
- The lane's most recent record (`c466018`, the PR #10 outcome and the PR #11
  evidence) is still **lane-local**: the published copy of this checkpoint at
  `7fb64e1` is blob `24990eaa`, while the lane copy is `e1d62650`, and the
  difference is exactly the "Checkpoint state, PR #10 merge outcome, and PR #11
  exact-head evidence" section. That record reaches the published line when the
  architect integrates this lane branch.
- `origin/codex/opencode-verification` remains the pre-rebase `077ee14`, so the
  runner's plain lane push still cannot fast-forward it. This is the known
  runner-side limitation already recorded; the content itself reaches the
  published line by the architect's integration.

### Evidence limits

- The CI and pull-request facts are a read-only observation of the public
  GitHub API without credentials on 2026-11-05, while PR #11 was open.
- The vblank/display-mode ratification, the ordinal-159 frontier, the second
  ordinal-2 stop and the documented unknown-option default are the primary and
  architect lanes' reviewed results. This lane checked head identity,
  ancestry, CI and mergeability only.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs 7fb64e1` -> exit 0.
- `git merge-base --is-ancestor ea6e15e 7fb64e1` -> 0 (the PR #11 base is
  contained in the head).
- `git rev-parse 7fb64e1:docs/INTEGRATION_VERIFICATION_CHECKPOINT.md` vs
  `c466018:docs/INTEGRATION_VERIFICATION_CHECKPOINT.md` -> `24990eaa` vs
  `e1d62650` (the last lane record is not yet in the published copy).
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 199 tests, 198 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 465 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request, and to the merge commit
once it lands; the same command checks that head against the then-current
architect head and the published merge target. No further tooling work is
authorized until the architect asks for it.

## Checkpoint state, PR #10 merge outcome, and PR #11 exact-head evidence (November 5, 2026)

### Checkpoint state confirmed

The lane task was reissued because no **published** lane commit was visible.
The lane did commit in the previous cycle: `cc64854` (PR #9 pre-merge
evidence) and `ca0b495` (PR #9 merge outcome and the pending next head). Both
are now published through the architect's integration - `ca0b495` is an
ancestor of PR #10's head `1e35f8a`, of the published main `ea6e15e` and of the
current architect head `c8c965c`, and `cc64854` is in `main` as well. The
published lane ref `origin/codex/opencode-verification` is still the pre-rebase
`077ee14`, which is why the lane's own ref shows no new commit; the runner's
plain lane push still cannot fast-forward a rebased branch, and the content
reaches the published line by the architect's integration.

### PR #10 exact-head evidence and merge outcome

| Item | Value |
| --- | --- |
| PR #10 | `codex/opencode-architect@1e35f8a69e99ce8a68614cc89f2de5ba3fd3c6ae` -> `main@52b3fa1642bc2379072334c42422601d6e2584f3` |
| Head equals the architect branch head at PR time | yes: `1e35f8a` ("Ratify the PFIFO responder and record the ordinal-159 frontier") was the architect head when the PR opened |
| Base equals the published main at PR creation | yes: published `main` was `52b3fa1…` |
| PR size | 12 commits, 16 changed paths, +607/-29, created 2026-11-05T11:42:31Z |
| Merge outcome | **merged** 2026-11-05T12:40:59Z; merge commit `ea6e15e1a5cbe43cc54735f808f0f5c235307293` "Merge reviewed architect integration" |
| Published main | `main` -> `ea6e15e1a5cbe43cc54735f808f0f5c235307293`, 454 tracked paths |
| Merge content delta | `git diff 1e35f8a ea6e15e` changes **no** path |

Green preparation check-runs at the exact head `1e35f8a` (read-only, GitHub
API):

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `42021136378` | `push`, branch `codex/opencode-architect` | `completed` / **success** (11:42:28Z -> 11:43:18Z) | `127223904238` "preparation (windows-latest)" success; `127223904699` "preparation (ubuntu-latest)" success |
| `42021153065` | `pull_request` | `completed` / **success** (11:42:35Z -> 11:43:22Z) | `127223959401` "preparation (windows-latest)" success; `127223959798` "preparation (ubuntu-latest)" success |

CI at the merge commit `ea6e15e`: run `42029221851` "Preparation checks",
event `push`, branch `main`, `completed` / **success** (12:41:01Z -> 12:41:53Z);
jobs `127251062277` "preparation (ubuntu-latest)" success and `127251062657`
"preparation (windows-latest)" success.

Re-running the precheck at `1e35f8a` today exits 1 with "candidate 1e35f8a is
not the architect head c8c965c". That is structural, not a failed candidate:
the architect branch advanced after PR #10 was merged, and the published-base
part of the check still passes (`ea6e15e` is **parents-contained**, head ahead
0 / behind 1, 0 missing base paths). At the time PR #10 was open, `1e35f8a`
was the architect head and its base `52b3fa1` was contained in it.

### PR #11 exact-head evidence (open, not yet merged)

| Item | Value |
| --- | --- |
| PR #11 | `codex/opencode-architect@c8c965cfe8786ead511c61e49e4fa792a1db75ae` -> `main@ea6e15e1a5cbe43cc54735f808f0f5c235307293` |
| Head equals the architect branch head | yes: `codex/opencode-architect` and `origin/codex/opencode-architect` are both `c8c965cfe8786ead511c61e49e4fa792a1db75ae` |
| Base equals the published main | yes: `origin/main` is `ea6e15e1a5cbe43cc54735f808f0f5c235307293`, and it is a **strict ancestor** of the head |
| PR size | 9 commits, 22 changed paths, +659/-16, created 2026-11-05T13:42:08Z |
| PR state | open, not a draft, `mergeable: true`, `mergeable_state: clean` |

Green preparation check-runs at the exact head `c8c965c` (read-only, GitHub
API):

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `42038083305` | `push`, branch `codex/opencode-architect` | `completed` / **success** (13:42:03Z -> 13:42:58Z) | `127281359575` "preparation (ubuntu-latest)" success; `127281360554` "preparation (windows-latest)" success |
| `42038103518` | `pull_request` | `completed` / **success** (13:42:12Z -> 13:43:08Z) | `127281430019` "preparation (windows-latest)" success; `127281430607` "preparation (ubuntu-latest)" success |

`node tools/verify-exact-head.mjs c8c965c` -> **exit 0**: the candidate is the
architect head; the published base `ea6e15e` is a strict **ancestor** (head
ahead 9, behind 0, **0 missing base paths**); 465 tracked paths at the head.

PR #11 has not merged, so there is no merge commit to record. The
`merge_commit_sha` `908de1ba2a5c6df7ba38a78bbac3faa6209a92c0` that GitHub
reports for an open pull request is its *test* merge commit, not a landed
merge, and is noted only so the identifier is not mistaken for one. The three
promotion gates the architect named - green CI at the exact head, a current
base, and clean mergeability - are all **observed** at `c8c965c`; the promotion
decision stays with the architect.

### Publication state

- The lane's PR #9 records (`cc64854`, `ca0b495`) are in the published main
  `ea6e15e` and in the current candidate `c8c965c`.
- The lane branch was fast-forwarded onto `c8c965c` (a plain fast-forward, no
  merge commit, no history rewrite), so this record sits on the same base as
  the candidate it describes.
- `origin/codex/opencode-verification` remains the pre-rebase `077ee14`; the
  runner's plain lane push still cannot fast-forward it, so the published lane
  ref lags the published content. That is a known, recorded runner-side
  limitation, not a content divergence.

### Evidence limits

- The merge, CI and pull-request facts are a read-only observation of the
  public GitHub API without credentials on 2026-11-05.
- The vblank/display-mode ratification, the ordinal-159 frontier and the
  second ordinal-2 stop are the primary and architect lanes' reviewed results.
  This lane checked head identity, ancestry, CI and mergeability only, and
  makes no claim about admission, export, seed or translation state.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs c8c965c` -> exit 0.
- `node tools/verify-exact-head.mjs 1e35f8a` -> exit 1 (the architect head has
  advanced past that candidate; the published-base part still passes).
- `git diff --name-only 1e35f8a ea6e15e` -> no path.
- `git merge-base --is-ancestor ca0b495 ea6e15e` -> 0 and
  `git merge-base --is-ancestor cc64854 ea6e15e` -> 0 (both lane records are in
  the published main).
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 199 tests, 198 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 465 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request, and to the merge commit
once it lands; the same command checks that head against the then-current
architect head and the published merge target. No further tooling work is
authorized until the architect asks for it.

## PR #9 merge outcome and the next published head (November 5, 2026)

### PR #9 landed

| Item | Value |
| --- | --- |
| PR #9 | `codex/opencode-architect@460549ed5c0a537b08528917cf06562a091b4fa3` -> `main@4b94a64aebc5d3bbf69629df67f37c19328610ac`; **merged** 2026-11-05T10:40:58Z |
| Merge commit | `52b3fa1642bc2379072334c42422601d6e2584f3` "Merge reviewed architect integration", parents `4b94a64a…` and `460549ed…` |
| Published main | `main` -> `52b3fa1642bc2379072334c42422601d6e2584f3`, 452 tracked paths |
| Merge content delta | `git diff 460549e 52b3fa1` changes **no** path |
| CI at the merge commit | run `42012737484` "Preparation checks", event `push`, branch `main`, `completed` / **success** (10:41:00Z -> 10:41:48Z); jobs `127195570927` "preparation (ubuntu-latest)" success, `127195571213` "preparation (windows-latest)" success |

The pre-merge evidence recorded while PR #9 was open still holds and was
re-read unchanged: run `42004605504` (`push`, branch `codex/opencode-architect`,
`completed` / **success**, jobs `127168093982` ubuntu-latest and `127168094439`
windows-latest both success) and run `42004623787` (`pull_request`,
`completed` / **success**, jobs `127168155897` ubuntu-latest and `127168155491`
windows-latest both success), both at head
`460549ed5c0a537b08528917cf06562a091b4fa3`.

### `verify-exact-head` against the landed base

`node tools/verify-exact-head.mjs 460549e` -> **exit 0** with the published
base now at the merge commit `52b3fa1`: the candidate is still the architect
head, the base is **parents-contained** (both of its sides are in the head's
history), head ahead 0 / behind 1, **0 missing base paths**, 452 tracked paths.
Against the merge commit itself the tool exits 1 with "not the architect head",
which is structural: a merge commit lives on `main` while the architect branch
head stays at `460549e`.

### The next head has not been published yet

The architect branch head is still `460549ed5c0a537b08528917cf06562a091b4fa3`
on both `codex/opencode-architect` and `origin/codex/opencode-architect`, and
**no new main pull request is open** (only the geometry draft PR #3). The
source lane has advanced to `bd3b1cb` ("Complete the PFIFO consumer evidence
path and classify the wait"), but the architect has not published or integrated
it, so there is no new head/base pair and no new check-run set to record this
cycle. The lane therefore records the PR #9 outcome and holds for the next
published head rather than inventing one.

### Publication state

- The PR #7 record `fe1b98e` **is** an ancestor of the merged main `52b3fa1`,
  so the verification record and its tooling are published in `main`.
- The PR #9 record `cc64854` is still lane-local: it is **not** an ancestor of
  `52b3fa1`, and it reaches the published line only when the architect
  integrates this lane branch.
- The lane's own published ref `origin/codex/opencode-verification` is still
  the pre-rebase `077ee14`, so the runner's plain lane push still cannot
  fast-forward it; the content reaches the published line through the
  architect's integration, unchanged from the previous cycles.

### Evidence limits

- The merge, CI and pull-request facts are a read-only observation of the
  public GitHub API without credentials on 2026-11-05, after PR #9 landed.
- The absence of a next head is the state observed at that moment; the
  architect may publish one immediately afterwards.
- The PFIFO consumer evidence and the planned completion-flag
  characterization belong to the primary and architect lanes. This lane
  checked head identity, ancestry and CI only.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs 460549e` -> exit 0.
- `git diff --name-only 460549e 52b3fa1` -> no path.
- `git merge-base --is-ancestor fe1b98e 52b3fa1` -> 0 (the PR #7 record is in
  the published main).
- `git merge-base --is-ancestor cc64854 52b3fa1` -> 1 (the PR #9 record is
  still lane-local).
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 197 tests, 196 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 452 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request, and to the merge commit
once it lands; the same command checks that head against the then-current
architect head and the published merge target. No further tooling work is
authorized until the architect asks for it.

## PR #9 exact-head evidence (open, not yet merged) (November 5, 2026)

The lane task's condition is met: `460549e` is published as the architect head
and the next main pull request is open.

### Head/base SHA equality

| Item | Value |
| --- | --- |
| PR #9 | `codex/opencode-architect@460549ed5c0a537b08528917cf06562a091b4fa3` -> `main@4b94a64aebc5d3bbf69629df67f37c19328610ac` |
| Head equals the architect branch head | yes: `codex/opencode-architect` and `origin/codex/opencode-architect` are both `460549ed5c0a537b08528917cf06562a091b4fa3` |
| Base equals the published main | yes: `origin/main` is `4b94a64aebc5d3bbf69629df67f37c19328610ac`, and it is a **strict ancestor** of the head |
| PR size | 13 commits, 31 changed paths, +2666/-13, created 2026-11-05T09:42:42Z |
| PR state | open, not a draft, `mergeable: true`, `mergeable_state: clean` |

### Green preparation check-runs at the exact head (read-only, GitHub API)

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `42004605504` | `push`, branch `codex/opencode-architect` | `completed` / **success** (09:42:37Z -> 09:43:22Z) | `127168093982` "preparation (ubuntu-latest)" success; `127168094439` "preparation (windows-latest)" success |
| `42004623787` | `pull_request` | `completed` / **success** (09:42:45Z -> 09:43:39Z) | `127168155491` "preparation (windows-latest)" success; `127168155897` "preparation (ubuntu-latest)" success |

Both runs report head `460549ed5c0a537b08528917cf06562a091b4fa3`.

### `verify-exact-head` pass at `460549e`

`node tools/verify-exact-head.mjs 460549e` -> **exit 0**: the candidate is the
architect head; the published base `4b94a64a` is a strict **ancestor** (head
ahead 13, behind 0, **0 missing base paths**); 452 tracked paths at the head.
The local conflict probe `git merge-tree --write-tree origin/main 460549e`
returns tree `f34a5b906eb4671fc4fc3cf7d49cc39492d71d86` with exit 0.

### Merge outcome: pending

PR #9 is **open and not merged**; no merge commit has landed and the published
`main` is still `4b94a64a`. The `merge_commit_sha` `d282811507ca86cc2d41a3229ac50f28c0d86f2d`
that GitHub reports for an open pull request is its *test* merge commit, not a
landed merge, and is recorded here only so the identifier is not mistaken for
one. The three gates the architect named for promotion - green CI at the exact
head, a current base, and clean mergeability - are all **observed** at
`460549e`; whether to promote is the architect's decision, and this lane
requests nothing.

### Publication state

- The lane head `fe1b98e` (the PR #8 record) is an ancestor of `460549e`, so
  the verification record and its tooling are inside the candidate and reach
  `main` when PR #9 merges.
- The lane's own published ref `origin/codex/opencode-verification` is still the
  pre-rebase `077ee14`, so the runner's plain lane push still cannot
  fast-forward it; the content reaches the published line through the
  architect's integration, unchanged from the previous cycle.
- The lane branch was fast-forwarded onto `460549e` (a plain fast-forward, no
  merge commit, no history rewrite; the lane's earlier records stay in that
  history), so this record sits on the same base as the candidate it describes.

### Scope of the candidate (not reviewed by this lane)

The delta over published `main` is **not** documentation-only: it adds
`src/runtime/pfifo_pusher.c` and `src/runtime/pfifo_pusher.h` with the native
fixture `tests/pfifo_pusher_test.c`, touches `src/runtime/proof-report.mjs`,
`src/runtime/local-title-proof.mjs`, `src/runtime/thread-proof-worker.mjs`,
`tools/run-native-tests.mjs` and `tools/fixtures/thread_startup_probe.c`, and
adds the Present-region and PFIFO-boundary checkpoints and package manifests.
This lane checked head identity, ancestry, CI and mergeability only; it did not
review that runtime work, and it makes no claim about admission, export, seed
or translation state.

### Evidence limits

- The CI and pull-request facts are a read-only observation of the public
  GitHub API without credentials on 2026-11-05, taken while PR #9 was open.
  They describe that moment; a later run or a merged state is not covered here.
- The lane resync is a local, verified change; the architect's integration of it
  is its own action.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs 460549e` -> exit 0.
- `git merge-base --is-ancestor 4b94a64a 460549e` -> 0 (the PR #9 base is
  contained in the head).
- `git merge-base --is-ancestor fe1b98e 460549e` -> 0 (the lane record is in
  the candidate).
- `git merge-tree --write-tree origin/main 460549e` -> exit 0, no conflict.
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 197 tests, 196 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 452 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request, and to the merge commit
once it lands; the same command checks that head against the then-current
architect head and the published merge target. No further tooling work is
authorized until the architect asks for it.
## PR #8 exact-head evidence and merge outcome (November 5, 2026)

### Pre-merge head/base SHA equality

| Item | Value |
| --- | --- |
| PR #8 | `codex/opencode-architect@d4e0107ec1ca1cd2bac245cecffcc165778f88d8` -> `main@42287210e508575e82951fbd8dd2f9b413721bce` |
| Head equals the architect branch head | yes: `codex/opencode-architect` and `origin/codex/opencode-architect` are both `d4e0107ec1ca1cd2bac245cecffcc165778f88d8` |
| Base equals the published main at PR creation | yes: published `main` was `42287210e508575e82951fbd8dd2f9b413721bce`, and it is an ancestor of the head |
| PR size | 17 commits, 31 changed files, +2797/-10 |
| Opened / merged | created 2026-11-05T07:42:01Z, merged 2026-11-05T08:41:06Z |

### Green preparation check-runs at the exact head (read-only, GitHub API)

| Run | Event | State | Jobs |
| --- | --- | --- | --- |
| `41989058506` | `push`, branch `codex/opencode-architect` | `completed` / **success** (07:41:58Z -> 07:42:44Z) | `127116665260` "preparation (windows-latest)" success; `127116665622` "preparation (ubuntu-latest)" success |
| `41989072434` | `pull_request` | `completed` / **success** (07:42:05Z -> 07:42:55Z) | `127116710662` "preparation (ubuntu-latest)" success; `127116710973` "preparation (windows-latest)" success |

Both runs report head `d4e0107ec1ca1cd2bac245cecffcc165778f88d8`.

### `verify-exact-head` pass at `d4e0107`

`node tools/verify-exact-head.mjs d4e0107` -> **exit 0**: the candidate is the
architect head; the published base (the merge commit `4b94a64`, whose two sides
are both in the head's history) is **parents-contained**, head ahead 0 / behind
1, **0 missing base paths**; 435 tracked paths at the head. Against the merge
commit itself the tool exits 1 with "not the architect head", which is
structural: a merge commit lives on `main` while the architect branch head
stays at `d4e0107`.

### Merge outcome

| Item | Value |
| --- | --- |
| PR #8 | **merged** at 2026-11-05T08:41:06Z |
| Merge commit | `4b94a64aebc5d3bbf69629df67f37c19328610ac` "Merge reviewed architect integration", parents `42287210…` and `d4e0107e…` |
| Published main | `main` -> `4b94a64aebc5d3bbf69629df67f37c19328610ac` |
| Merge content delta | `git diff d4e0107 4b94a64a` changes **no** path |
| CI at the merge commit | run `41996378818` "Preparation checks", event `push`, branch `main`, `completed` / **success** (08:41:08Z -> 08:41:57Z); jobs `127140449105` "preparation (windows-latest)" success, `127140449344` "preparation (ubuntu-latest)" success |

### Publication state

The lane head `2c8af62` (the PR #7 record and the identity fix) is an ancestor
of both `d4e0107` and the merged main `4b94a64a`, so the verification record
and its tooling are published in `main` again. The lane's own published ref
`origin/codex/opencode-verification` is still the pre-rebase `077ee14`, so the
runner's plain lane push still cannot fast-forward it; the content reaches the
published line through the architect's integration, as in the previous cycle.

The lane branch was fast-forwarded onto the new published integration head
`d4e0107` (a plain fast-forward, no merge commit, no history rewrite; the
lane's earlier record commit stays in that history), so this record sits on the
same base as the candidate it describes.

### Evidence limits

- The merge, CI and pull-request facts are a read-only observation of the public
  GitHub API without credentials on 2026-11-05.
- The lane resync is a local, verified change; the architect's integration of
  it is its own action.
- The render-state batch admissions through `0x1F7460` and the planned
  read-only `0x00202150` (`D3DDevice_Present`) characterization are the primary
  and architect lanes' work. This lane checked head identity, ancestry and CI
  only, and makes no claim about admission, export, seed or translation state.
- Nothing here approves a merge, admits a helper, or supports any rendering,
  fidelity, playability, online, social or authority claim.

### Checks run for this result

- `node tools/verify-exact-head.mjs d4e0107` -> exit 0.
- `node tools/verify-exact-head.mjs 4b94a64a` -> exit 1 (merge commit, not the
  architect head; expected).
- `git merge-base --is-ancestor 2c8af62 4b94a64a` -> 0 (the lane record is in
  the published main).
- `git merge-base --is-ancestor 4228721 d4e0107` -> 0 (the PR #8 base is
  contained in the head).
- `git diff --name-only d4e0107 4b94a64a` -> no path.
- `node --test tests/verification-exact-head.test.mjs` -> 14/14 pass.
- `node --test tests/integration-verification.test.mjs` -> 7/7 pass.
- `npm run test:runtime` -> 196 tests, 195 pass, 0 fail, 1 pre-existing skip
  (the absent private production XBE).
- `npm run check` -> publication guard passed for 435 source files.
- `git diff --check` -> clean.

### Next task for this lane

Stop after this record. The next exact-head precheck applies to the next
published architect head and its main pull request; the same command checks
that head against the then-current architect head and the published merge
target. No further tooling work is authorized until the architect asks for it.

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
