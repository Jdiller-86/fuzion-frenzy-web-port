# Integration verification checkpoint

Independent verification lane. Owned paths: `tools/verify-*`, `tools/check-*`,
`tests/integration-*`, `tests/verification-*` and this file. The lane does not
touch the runtime, the general status files, the model/scheduler scripts or
builder-owned tests, does not read private research, does not delegate, and does
not merge or push. It records what is mechanically checkable and leaves every
decision to the architect.

## Exact-head precheck for `c9dc22f` (November 4, 2026)

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

### Next task for this lane

Stop after this document-and-test commit. The next exact-head precheck applies
to the bounded `0x1F6EB0` helper result once the primary reports it; the same
command checks that head against the then-current architect head and the
published merge target. No further tooling work is authorized until the
architect asks for it.
