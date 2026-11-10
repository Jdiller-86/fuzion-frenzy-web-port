# Project management

The public [GitHub Project](https://github.com/users/Jdiller-86/projects/3) follows CTR-2-WUP's separation of reviewed scope, native GitHub tracking and exported progress. Five milestone issues and ten native work packages cover the plan. The 24 stable MB01–MB24 rows and seven largest-first groups are the detailed M1 work map, surfaced in the Project overview, M1 implementation issue (#23) and interactive graph; they are not additional issues or forecast tasks. No implementation is assigned or started by creating that backlog.

## Sources of truth

- [profiles/project.json](../profiles/project.json) defines the five complete-product goals, full criteria, stable milestone/work-package identities and acceptance dependencies.
- [profiles/m1-browser-work.json](../profiles/m1-browser-work.json) defines the M1 MB01–MB24 detail and largest-first priority groups, including size, evidence/state boundaries and completion checks. It is detailed scope beneath the existing M1 implementation package, not a competing issue hierarchy.
- Native GitHub milestones and issues hold live status, implementation notes, review/evidence and native parent/dependency links. Issue status and acceptance are authoritative there; board fields mirror validated issue status.
- [milestones.md](milestones.md), the M1 map documentation and README summaries are generated from reviewed scope. Edit the structured source, then run `npm run project:docs` and `npm run check`; do not hand-edit generated summaries. [plans.md](../plans.md) gives readable direction. [Status](status.md), the development-state profile and handoff retain actual runtime evidence and current boundary.
- Project overview, issue/board views and progress reports are source-only projections. Their source SHA and generation/snapshot time identify what they show. A missing or failed tracking fetch is displayed as unavailable, never as a stale value presented as current or as inferred completion.

Repository milestones and issue status labels own acceptance. The user-owned GitHub Project mirrors their status and the Milestone/Task work type, with a Waiting resource field for human coordination. Its field definitions and saved views are copied from CTR-2-WUP's Project #2. Issue cards, drafts and their field values are excluded; the copied Project description is replaced before publication. [Project identity](../profiles/project-board.json) pins the verified owner, project number and stable IDs.

Repository tracking uses the workflow's scoped GitHub credential. Board operations use the repository Actions secret `FUZION_GITHUB_MANAGEMENT_TOKEN`, configured by the owner with `project` and `public_repo` access. Keep the credential in Actions; never put it in source, chat, arguments, receipts or issue bodies. A missing/expired credential fails the board operation instead of inventing progress. Source-backed scope edits become current only after review/merge and successful trusted-main synchronization; local generation does not claim the public Project or graph has deployed.

## Status and acceptance

Use one status label: Planned, In progress, Blocked, Awaiting evidence or Ready for review. Close completed work as completed; a closed-not-planned item is Closed. Record a concrete waiting resource and the useful independent work before calling an item blocked.

A milestone is Accepted only after all its exact criterion checkboxes pass, prerequisites are accepted, and its parent issue is closed as completed with `status:accepted`. Its acceptance record must contain:

```text
Source commit: <40-character source SHA>
Evidence: <named scenario/report/audit identities and curated result>
Review: https://github.com/Jdiller-86/fuzion-frenzy-web-port/issues/<review-number>
```

Keep original media, derived title code, memory/save states, raw reports and private analysis local. Closing child tasks or merging code does not accept the parent. Close the matching native GitHub milestone after accepting the parent gate.

Work can overlap; dependency links govern acceptance. Implementation issues still need bounded changes, an observable result and targeted validation. Contributors and agents check current main and ownership, use stable milestone/work-package and MB IDs, and update affected scope, remaining-feature and evidence records in the same PR. Preserve human evidence and native status. Add further native work packages only when a concrete dependency warrants them; do not create issues for every MB row or diagnostic counter. External issues and PRs added manually remain unowned and are never silently adopted into the managed hierarchy.

## Synchronization and progress

[Project tracking](../.github/workflows/project.yml) runs from trusted main. Reviewed scope changes synchronize owned labels, native milestones, marker-identified issues and native parent/dependency links. Existing human evidence and checked unchanged criteria are preserved. It neither closes gates automatically nor runs a coding agent.

Every push to trusted main and issue event reconciles reviewed metadata, exports current progress and mirrors validated issue status to the board. Hourly tracking reconciliation repairs missed-event or manual board drift; hourly graph publication refreshes the public report independently. Each run resolves current main before checkout, and reports identify that source SHA and snapshot time. Edit issue labels and acceptance records to change status; a manual board Status change does not accept a gate and is replaced on the next synchronization. Waiting resource values and human notes outside the managed Project description are retained. Further public issues and pull requests added manually from this repository are left untouched.

The copied views include the full table, Milestones board, Delivery gates, Implementation work, Evidence handoff and Pull requests. Parent issues have `milestone`; the ten child tasks have `work-item` plus their implementation/evidence label so the saved filters match the backlog. The M1 detail view presents MB IDs and priority groups without creating managed cards for each row. Empty Evidence handoff or Pull requests views mean no matching items are present in the validated snapshot.

Workflow dispatch supports snapshot or sync, with preview unless Apply is selected. The workflow verifies source identity, issue identity, parent/dependency read-back, board membership/fields and the public snapshot. Managed cards must be unarchived; unexpected archival fails verification for review. Writes are limited to this repository and its owned board; CTR's template remains read-only. Board creation is refused when an unregistered same-named project exists, so retries do not silently create duplicates or adopt unrelated work.

The run's `project-tracking-*` artifact contains repository/board JSON receipts and `progress/index.html`, a standalone offline view. It separates accepted gates from completed tasks, provides status filtering and links to native issues, and includes the reviewed M1 work map. Arbitrary Project notes and Waiting resource text are excluded from the exported receipt. The deployed report follows successful trusted-main tracking and the hourly fallback; no documentation change should promise deployment before merge and a successful run.

## User-directed progress checkpoint policy

For ongoing M1 work, keep verified coherent progress visible on main through a reviewed source-only pull request with passing CI, then update native issue notes with the actual checkpoint, evidence identities, review state and remaining limits. Keep all milestone acceptance boxes and parent gates unchecked until their stated criteria are met. This is a user-directed publication workflow, not an automatic coding agent, release, or permission to publish original or derived game content.

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
