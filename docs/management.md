# Project management

The project follows CTR-2-WUP's separation of reviewed milestone scope, native GitHub tracking and exported progress. Five milestone issues and ten initial work packages cover the plan. No implementation is assigned or started by creating that backlog.

## Sources of truth

- [profiles/project.json](../profiles/project.json) defines the five outcomes, criteria, stable tracking identities and acceptance dependencies.
- Native GitHub milestones and issues hold live status, implementation notes, review/evidence and native parent/dependency links.
- [milestones.md](milestones.md) is generated from reviewed scope; [plans.md](../plans.md) gives the readable direction. [Status](status.md) retains actual main-branch runtime evidence.
- Snapshots and the offline progress view are derived artifacts. Their source revision and generation time identify the observation; unavailable tracking is reported as unavailable.

The current adapter uses repository milestones and status labels. CTR's separate user-owned Projects board requires project-level access; no such board or private management credential is configured here. Repository tracking uses the workflow's scoped GitHub credential.

## Status and acceptance

Use one status label: Planned, In progress, Blocked, Awaiting evidence or Ready for review. Close completed work as completed; a closed-not-planned item is Closed. Record a concrete waiting resource and the useful independent work before calling an item blocked.

A milestone is Accepted only after all its exact criterion checkboxes pass, prerequisites are accepted, and its parent issue is closed as completed with `status:accepted`. Its acceptance record must contain:

```text
Source commit: <40-character source SHA>
Evidence: <named scenario/report/audit identities and curated result>
Review: https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/<review-number>
```

Keep original media, derived title code, memory/save states, raw reports and private analysis local. Closing child tasks or merging code does not accept the parent. Close the matching native GitHub milestone after accepting the parent gate.

Work can overlap; dependency links govern acceptance. Implementation issues still need bounded changes, an observable result and targeted validation. Add further work packages only when a concrete dependency warrants them; do not create a task for every diagnostic counter.

## Synchronization and progress

[Project tracking](../.github/workflows/project.yml) runs from trusted main. Reviewed scope changes synchronize owned labels, native milestones, marker-identified issues and native parent/dependency links. Existing human evidence and checked unchanged criteria are preserved. It neither closes gates automatically nor runs a coding agent.

Issue events export current progress. Workflow dispatch supports snapshot or sync, with preview unless Apply is selected. The workflow verifies source identity, issue identity, parent/dependency read-back and the public snapshot. No cross-repository or project-account management is performed.

The run's `project-tracking-*` artifact contains JSON receipts and `progress/index.html`, a standalone offline view. It separates accepted gates from completed tasks, provides status filtering and links to native issues. It is a downloadable report; no documentation site deployment is configured by this work.

## Local commands

```sh
npm run project:docs
npm run check
npm run test:project
```

With an appropriately scoped GitHub API connection, `npm run project:preview` previews metadata operations; `node tools/project.mjs sync --apply` applies them only from the current main revision. `npm run project:snapshot` and `npm run project:render` produce ignored artifacts. Prefer the GitHub workflow when local API access is unavailable. Credentials stay in the environment and are never printed.

Synchronization uses stable markers rather than issue titles. Do not remove those markers or register the same issue twice. An unknown/duplicate identity, conflicting status, changed parent, missing acceptance evidence or unavailable API fails verification instead of fabricating progress.
