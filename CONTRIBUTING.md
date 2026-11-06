# Contributing

Help advance the original Fuzion Frenzy toward complete browser execution and the planned online experience. The current repository is research; a public clone does not run the original game. Start with [current status](docs/PROJECT_STATUS.md), [the roadmap](DEVELOPMENT_PLAN.md), and [development setup](docs/DEVELOPMENT.md).

## Choose a bounded contribution

Source-owned parser/runtime fixes, reproducible fixtures, input/delivery improvements, reference-backed curated findings, and documentation are useful. Before work, read [AGENTS.md](AGENTS.md), the relevant checkpoint, and [architecture](docs/ARCHITECTURE.md). Check existing issues/PRs and ownership so another contributor is not editing the same slice.

Use an issue or PR to state one observable result, target milestone, files/components in scope, acceptance checks, and limits. For runtime work cite the current exact stop and required report/audit evidence. Do not claim compatibility or original behavior from a plausible model, prototype, or source fixture.

## Set up and verify

Use Node 24.14.0 and npm 11.x (pin 11.9.0):

```sh
npm ci --ignore-scripts
npm run check
```

[The development guide](docs/DEVELOPMENT.md) lists all run/build/test commands and compiler/input prerequisites. Run checks relevant to the change plus `git diff --check`. Material CPU, memory, ABI, or GPU changes need independent correctness review; no broad repeat suite is required for a low-impact prose correction.

## Keep original content local

Never attach or commit game files, extracted/converted assets, generated original-title modules, pseudocode, raw research logs/exports, private analysis projects, downloaded tools, credentials, or environment files. Keep proprietary inputs/derived artifacts in ignored local locations. Publish only independently authored source/fixtures and curated non-content findings. See [content handling](docs/PLAYER_SUPPLIED_CONTENT.md).

Review the actual diff before staging. Stage explicit paths, then inspect:

```sh
git diff --cached --stat
git diff --cached
npm run check
```

The guard checks tracked and unignored files, but it cannot establish provenance or replace manual path/content review. Preserve existing work and source-publication protections; do not force-push or rewrite published history.

## Submit a reviewable result

Use a functional title explaining the resulting behavior. Include the problem, changes, checks/results, evidence, and remaining limits using [the PR template](.github/pull_request_template.md). Update current status/handoff together if execution changes; record consequential decisions and index new evidence.

Keep the existing code style and add dependencies only when justified. Do not change global tools, credentials, model settings, production services, or distribution as a contribution side effect.

A license for new project code has not been selected; public visibility and contributing do not establish permissions to reuse original game content or project code. Do not introduce a license or imply content rights without an explicit project decision.
