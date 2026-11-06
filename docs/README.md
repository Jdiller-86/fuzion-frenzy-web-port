# Documentation

Fuzion Frenzy Web Port is researching original-game execution entirely in a browser through static recompilation to WebAssembly. The original screen, full runtime, fidelity, and planned multiplayer/social product remain unaccepted. Start with the [project overview](../README.md).

## Start here

| Your goal | Read |
| --- | --- |
| Understand the product and delivery sequence | [Project roadmap](../DEVELOPMENT_PLAN.md) |
| See what has actually been observed | [Current status](PROJECT_STATUS.md) and [handoff](DEVELOPMENT_HANDOFF.md) |
| Run the prototype or source checks | [Development guide](DEVELOPMENT.md) |
| Contribute a focused change | [Contributing](../CONTRIBUTING.md) and [repository instructions](../AGENTS.md) |
| Understand the implementation | [Architecture](ARCHITECTURE.md) |
| Understand game-file handling | [Player-supplied content](PLAYER_SUPPLIED_CONTENT.md) |

## Product and acceptance

- [Scope and fidelity](SCOPE_AND_FIDELITY.md): the original-game baseline and browser/local-content contract.
- [Runtime milestone gates](RUNTIME_MILESTONES.md): M0–M12 and runtime evidence requirements.
- [Product acceptance ledger](ORIGINAL_EXECUTION_PROGRESS.md): complete requirements and remaining evidence.
- [Selected execution route](ORIGINAL_EXECUTION_ROUTE.md): recompilation, supporting research, and excluded streaming/native helpers.
- [Risks and unresolved questions](PROJECT_CONCERNS.md): technical, reference, authority, content, and release gaps.

## Evidence and maintenance

- [Research index](RESEARCH.md): all retained checkpoints and package manifests, grouped by subsystem.
- [Decision history](DECISIONS.md): recorded decisions, including superseded and renumbered legacy entries.
- [Historical delivery ledger](archive/PROGRESS_METRICS_PRIOR_20261106.md): prior counting rules and tempo baseline.
- [Documentation archive](archive/README.md): prior plans, instructions, handoffs, status, and metrics.

Historical records are marked as such and retain their original filenames. Their old “current” statements do not replace the current status page. Fixtures, synthetic profiles, static analysis, and original browser observations establish different things.

## Optional research

- [Analysis setup](ANALYSIS_SETUP.md): pinned private tools, retained local inputs, read-only analysis, and the source-only startup harness.

Project automation was removed in commit `55ea550`. No project agent scheduler or model routing is configured. Archived workflow records describe historical arrangements and do not configure a current contributor session.

## Updating these docs

Keep stable goals in the roadmap, exact latest measurements in status, and the next owner/task in the handoff. Update the handoff and status together when execution changes. Archive replaced observations, link consequential decisions, and index new evidence in RESEARCH.md. Distinguish recorded results from checks performed in the current change. Never attach original game content or private raw output.
