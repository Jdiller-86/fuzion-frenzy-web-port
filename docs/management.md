# Project management

The public [GitHub Project](https://github.com/users/Jdiller-86/projects/3) follows CTR-2-WUP's separation of reviewed milestone scope, native GitHub tracking and exported progress. Five milestone issues and ten initial work packages cover the plan. No implementation is assigned or started by creating that backlog.

## Sources of truth

- [profiles/project.json](../profiles/project.json) defines the five outcomes, criteria, stable tracking identities and acceptance dependencies.
- Native GitHub milestones and issues hold live status, implementation notes, review/evidence and native parent/dependency links.
- [milestones.md](milestones.md) is generated from reviewed scope; [plans.md](../plans.md) gives the readable direction. [Status](status.md) retains actual main-branch runtime evidence.
- Snapshots and the offline progress view are derived artifacts. Their source revision and generation time identify the observation; unavailable tracking is reported as unavailable.

Repository milestones and issue status labels own acceptance. The user-owned GitHub Project mirrors their status and the Milestone/Task work type, with a Waiting resource field for human coordination. Its field definitions and saved views are copied from CTR-2-WUP's Project #2. Issue cards, drafts and their field values are excluded; the copied Project description is replaced before publication. [Project identity](../profiles/project-board.json) pins the verified owner, project number and stable IDs.

Repository tracking uses the workflow's scoped GitHub credential. Board operations use the repository Actions secret `FUZION_GITHUB_MANAGEMENT_TOKEN`, configured by the owner with `project` and `public_repo` access. Keep the credential in Actions; never put it in source, chat, arguments, receipts or issue bodies. A missing/expired credential fails the board operation instead of inventing progress.

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

Issue events export current progress and mirror validated issue status to the board. Edit issue labels and acceptance records to change status; a manual board Status change does not accept a gate and is replaced on the next synchronization. Waiting resource values and human notes outside the managed Project description are retained. Further public issues and pull requests added manually from this repository are left untouched.

The copied views include the full table, Milestones board, Delivery gates, Implementation work, Evidence handoff and Pull requests. Parent issues have `milestone`; the ten child tasks have `work-item` plus their implementation/evidence label so the saved filters match the backlog. Evidence handoff and Pull requests can be empty at the paused baseline.

Workflow dispatch supports snapshot or sync, with preview unless Apply is selected. The workflow verifies source identity, issue identity, parent/dependency read-back, board membership/fields and the public snapshot. Managed cards must be unarchived; unexpected archival fails verification for review. Writes are limited to this repository and its owned board; CTR's template remains read-only. Board creation is refused when an unregistered same-named project exists, so retries do not silently create duplicates or adopt unrelated work.

The run's `project-tracking-*` artifact contains repository/board JSON receipts and `progress/index.html`, a standalone offline view. It separates accepted gates from completed tasks, provides status filtering and links to native issues. Arbitrary Project notes and Waiting resource text are excluded from the exported receipt.

## README graph and public report

The [public graph](https://jdiller-86.github.io/fuzion-frenzy-web-port/progress/) follows CTR-2-WUP's presentation: a dark SVG summary in the README opens an interactive milestone/task dependency graph. Select a node to inspect source-reviewed scope and acceptance criteria, then follow its native issue for current evidence. Status and milestone filters narrow the graph. Completed work never accepts a gate automatically.

[Graph publication](../.github/workflows/publish-progress.yml) runs after successful tracking updates, relevant source changes and hourly. It resolves trusted main, validates the same public issue snapshot, publishes only the five allowlisted report files and verifies their deployed bytes. The graph records its source revision and snapshot time; API failure displays unavailable data rather than invented progress. GitHub's image cache may lag a fresh publication.

Only this tracking report is deployed to GitHub Pages. The research app, original content and private evidence are excluded. Publication preserves any unrelated existing Pages site for review. Initial Pages enablement can use the owner's existing management secret; normal deployment uses the workflow's Pages credential. The workflow neither starts development agents nor changes milestone acceptance.

## Local commands

```sh
npm run project:docs
npm run check
npm run test:project
```

With an appropriately scoped GitHub API connection, `npm run project:preview` previews metadata operations; `node tools/project.mjs sync --apply` applies them only from the current main revision. `npm run project:snapshot` and `npm run project:render` produce ignored artifacts. Prefer the GitHub workflow when local API access is unavailable. Credentials stay in the environment and are never printed.

`npm run project:site` consumes `artifacts/project.json` and builds the graph report in ignored `artifacts/project-site/`. The snapshot's source revision must match the checkout. `npm run project:site:verify` compares the live report with the local publication receipt after deployment.

`node tools/project-board.mjs` previews board changes; `--apply` copies/configures the board or updates its managed cards only from current main. The trusted-main tracking workflow runs this after validating repository tracking. A new board copies saved views exactly; repository-specific template filters require review before copying. Existing view edits are retained after creation.

Synchronization uses stable markers rather than issue titles. Do not remove those markers or register the same issue twice. An unknown/duplicate identity, conflicting status, changed parent, missing acceptance evidence or unavailable API fails verification instead of fabricating progress.
