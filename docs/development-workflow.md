# Measured development workflow

Use the existing source and evidence tools in small functional batches. This workflow adds no service, scheduler, application dependency or original-runtime capability. Runtime acceptance still belongs to the [milestone criteria](milestones.md); [resource reuse](resource-reuse.md) and the [scope ledger](m1-work-scope.md) govern implementation.

## Start from the current state

```sh
npm run dev:status
npm run dev:preflight
```

[profiles/development-state.json](../profiles/development-state.json) owns the recorded checkpoint identity, current task and admission counts. Its source commit identifies the accepted evidence checkpoint, not necessarily the checkout HEAD. [Status](status.md) owns detailed observations, qualifications and private evidence identities. The generated pointer in the entry documents is checked by `npm run check`; it does not start a task or accept a milestone.

When a new runtime checkpoint is accepted, update the structured state and detailed status/handoff together, then run:

```sh
npm run dev:state:docs
npm run check
```

Review the generated changes. Update module/audit/input identities only from the newly reviewed evidence; a documentation or source-only change does not advance the runtime checkpoint. Keep the task forecast's named near-term work distinct from unresolved allowances and accepted capabilities. Update the scope ledger when a package closes, splits, merges, or new evidence changes its allowance; do not subtract routine/checkpoint counts from the forecast.

## Verify the private baseline locally

The default preflight succeeds in a source-only checkout while reporting `baselineIntegrityVerified: false`. This means public work can proceed; it is not a skipped runtime pass. For a task that requires original execution, require explicit private verification:

```sh
npm run dev:preflight -- --manifest research/verification/development-baseline.json --require-private
```

Exit 0 means the named files match the manifest. Exit 2 means required inputs are missing or mismatched. Exit 1 means invalid arguments/manifest or a tool error. No compiler, analysis tool, target code or manifest-supplied command is executed. Complete database state, tool versions and actual retained browser replay still need verification before implementation.

Create a starting template **locally**, under ignored research, using direct Node so npm's banner does not enter the JSON:

```sh
node tools/development-preflight.mjs --template > research/verification/development-baseline.json
```

Use this redirection only for a new manifest; preserve an existing retained manifest instead of overwriting it. Create the ignored directory first if needed; the template command does not create it. Replace placeholders with actual retained file paths, byte counts and SHA-256 values. Paths resolve relative to the manifest's directory; absolute local paths are supported. The three known XBE/module/audit digests and profile/source identity must match the recorded checkpoint. Remaining digests must come from retained evidence, not values freshly chosen to make an unknown candidate pass. Keep the manifest private and do not attach it to issues or PRs.

Required file identities are `xbe`, `analysisProject`, `seedIndex`, `preparedProfile`, `module`, `controlReport`, `inertReport`, `previousReport`, `experimentReport`, `audit`, and `toolchainManifest`. The analysis-project hash covers that named file only, not the full Ghidra database. The toolchain manifest must identify the actual retained compiler and preparation setup; its presence does not prove those tools execute. Never generate missing reports or invent expected hashes. The preflight streams file hashes and reports identity/status only, without paths or contents.

The conventional default manifest is `research/verification/development-baseline.json`. Do not repeatedly download tools, reacquire game files or reconstruct a known baseline when retained artifacts exist elsewhere. Use the [local-agent handoff](local-agent-handoff.md) on that workstation.

## Run one selected source check

```sh
npm run dev:task -- workflow --scope WORKFLOW-01
npm run dev:task -- runtime --scope SCOPE-37
```

Available selections are `workflow`, `runtime`, `native`, `evidence`, `formats`, `gameplay`, and `project`. They invoke the corresponding existing test script through the active Node/npm without shell interpolation. The task stops on a failed check, propagates failure, and records elapsed time under ignored `artifacts/development/iterations.jsonl`. These tasks execute source-owned checks only; they neither build nor run the private title. Choose the affected suite; the workflow does not run all suites automatically. Run required publication checks at coherent source checkpoints.

## Record local work without collecting private evidence

Record measured phase time after a local prepare/build/browser/review activity:

```sh
npm run dev:task -- record --scope SCOPE-37 --phase browser --seconds 72.5 --outcome passed
npm run dev:task -- record --scope SCOPE-37 --phase prepare --seconds 18 --outcome failed --failure-class harness
npm run dev:task -- summary
```

Phases are `setup`, `prepare`, `build`, `browser`, `review`, `report`, and `source-checks`. Outcomes are `passed`, `failed`, and `blocked`. Failure classes are `harness`, `runtime`, `prerequisite`, `source-check`, or `unknown`; successful samples use `none`. Scope is a short identifier such as SCOPE-37, not a path or free-form description. Report zero seconds only when no measurable work ran; do not infer time from commits. Manual samples remain labeled `reported-local-work` and never establish acceptance.

Records contain only source commit, a dirty-working-tree indicator, scope, phase, duration, outcome/class and timestamp. A dirty sample must not be described as verification of the named commit alone. No raw report, original content, state snapshot, file path, environment, log, command text or attachment is recorded. The summary keeps automated checks and reported local work separate. Samples can overlap and tasks vary in size, so their sum is neither project elapsed time nor velocity. Use a small set of comparable tasks to locate setup/build/browser/review delays and failure causes before choosing the next optimization.

## Task and review boundaries

Give each task a functional output, owned files, evidence prerequisites, focused checks and a stop condition. Prefer an existing provider/shared-interface repair over a new implementation per address. Preserve runtime invariance and independent review for changed CPU, memory, ABI or GPU behavior. Generate a compact review packet from retained manifests/reports; do not copy private content into public summaries.

The [local handoff](local-agent-handoff.md) provides the next startup task and separate graphics, resource-read, scaling, browser-conversion and authority goals. Work on one assigned goal at a time, with independent source assessment or review in the second worker slot. The [authority assessment](online-authority-assessment.md) resolves what can be decided from public contracts and names the experiment still required before online implementation.
