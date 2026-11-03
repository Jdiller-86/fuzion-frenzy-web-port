# OpenCode model routing and development workflow

The continuous target and local five-minute scheduled cycle are in [the OpenCode goal](OPENCODE_GOAL.md).

This repository uses [the project OpenCode V2 configuration](../.opencode/opencode.jsonc). It leaves the working global OpenCode Go, OpenAI OAuth, Unsloth and REA connections intact. OpenCode 2.2.5 discovers the project config when started in this repository. A fresh session defaults to `builder` with `opencode-go/deepseek-v4.1-flash`; existing sessions retain their own selected agent and model until changed explicitly.

| Agent | Provider/model | Use | Tool boundary |
| --- | --- | --- | --- |
| `builder` | `opencode-go/deepseek-v4.1-flash` | Bounded implementation, tests, task coordination | May edit source and delegate to `scout`, `reviewer` or `architect`; no REA |
| `scout` | `unsloth/qwen3.8-27b` | Narrow local source search, classification, REA evidence | Read-only; no shell, edit or further delegation; only selected REA inspection tools |
| `reviewer` | `opencode-go/glm-5.3` | Independent correctness review of high-risk changes or unresolved bugs | Read-only; no shell, REA or further delegation |
| `architect` | `openai/gpt-6.1-sol` | Architecture, milestone design, difficult decisions and final evidence review | Primary or bounded subagent, read-only; a primary session may delegate work to cheaper agents |

Project policy allows only `builder`, `scout`, `reviewer` and `architect` child agents. The builder may call Sol without a separate user approval when the escalation rules below apply; a subagent cannot delegate again at the default depth limit. Its OpenAI provider is the active linked ChatGPT/Codex OAuth method (`chatgpt-token-sharing`), not an API key. To start with Sol as the primary architect, select both `architect` and `openai/gpt-6.1-sol` in a new Desktop session. Selecting a primary agent alone does **not** switch an existing session's model. On the CLI, use `opencode run --agent architect --model openai/gpt-6.1-sol "<architecture question>"`. The default CLI run uses DeepSeek Go. Never substitute `opencode/deepseek-v4.1-flash` or `opencode/gpt-6.1-sol`: those are different provider routes.

## One bounded task

1. Read `AGENTS.md`, the current handoff/status, the relevant checkpoint and the affected source. Capture a task card with a concrete result, files in scope, acceptance checks, evidence boundary and unresolved questions.
2. Delegate a narrow search or local REA question to `scout` when it fits its 8,192-token context. Ask for paths, evidence IDs and uncertainty, not a dump of proprietary bytes. Direct Ghidra remains the XBE route documented in `ANALYSIS_SETUP.md`; REA's Windows XBE admission is not assumed.
3. Use `builder` for the smallest coherent implementation. For CPU instruction semantics, memory accounting, ABI, GPU compatibility or an unresolved correctness defect, obtain an independent `reviewer` finding against actual changed code and tests. A review is evidence to inspect, not a substitute for executing the original title.
4. Escalate automatically to `architect` when a decision changes the project route or multiple subsystems, two targeted attempts fail, or a GLM review leaves a material CPU, memory, ABI or GPU correctness question unresolved. Send: objective; observed behavior; exact evidence/checks; attempts; remaining hypotheses; decision needed. Use one bounded Sol call per decision; return implementation to DeepSeek. Stop and report if that call cannot resolve the decision.
5. Finish with changed files, checks actually run, observed original-runtime behavior, remaining gaps and next task. Update `PROJECT_STATUS.md`; record consequential choices in `DECISIONS.md`. Do not claim a playable game from translated-function counts or component fixtures.

Agent step caps (builder 48, scout 8, reviewer 12, architect 8) stop a single prompt sequence; a new user input resets the cap. They are usage brakes, not billing limits. The project policy blocks environment-file reads/edits, external-directory tool access and direct Git push/merge commands. Shell commands and plugin code are **not** contained by these project rules, so do cloud-model work in a clean source-only worktree and inspect tool prompts. Do not put original game files, converted assets, pseudocode, credentials or raw research logs in prompts, issues, PRs or model attachments. Local Qwen can analyze local evidence; send cloud models only curated findings.

OpenCode Go's **Extra Usage** setting can fall back to pay-as-you-go balance after the included limit. The Personal workspace currently shows Extra Usage **Off**, so requests pause at a limit. This repository cannot enforce that account setting; verify it again if billing preferences change. If a model is unavailable or a quota is reached, stop and report it; do not silently switch provider or model.

**Live validation, November 2:** the user authorized Global regions for the Personal workspace; the Console shows Global selected. A minimal `opencode-go/deepseek-v4.1-flash` request then returned `READY`. Minimal requests also returned `READY` from Go GLM 5.3 and linked ChatGPT OAuth Sol. Paid model training remains disabled in Console Privacy. The local Unsloth API accepts the existing user-level `UNSLOTH_API_KEY` and answers Qwen requests, but this shell did not inherit that variable automatically. Pass it into the OpenCode CLI process when needed; do not store it in this repository. The project `scout` run hit `Nothing to compact yet` before its first answer, consistent with fixed instructions/tools filling the local model's 8,192-token context; its OpenCode handoff remains unverified. Keep Qwen tasks narrow and do not rely on that route until a bounded handoff succeeds.

## Source control and GitHub

Use a clean worktree from a reviewed source-only commit for cloud agents. The main checkout contains ignored `research/` and player-supplied game files, so a Git ignore rule alone is not an isolation boundary. The local `.worktrees/` directory is ignored. Keep one branch/task owner at a time; use `codex/<task>` names. Do not copy ignored material into a worktree. The current main checkout also has in-progress memory-statistics changes; leave them with their owner.

Use a GitHub issue with a task card when useful, then a scoped branch/worktree, targeted tests, a source-only pull request and human review before merge. The public GitHub repository and source-only CI already exist. OpenCode does not yet have a verified GitHub issue/PR connector, and `gh` is not installed; the existing `git` remote is sufficient for local branch work. Do not assume GitHub Actions can access localhost Unsloth, desktop OAuth or REA. Before any stage/push, run `npm run check`, inspect the staged paths and `git diff --cached --stat`, and keep all original content and credentials out of Git history. Push and merge remain manual outside the OpenCode agent policy.

## Quick CLI checks

Run from the repository or its clean worktree:

```powershell
opencode debug config
opencode debug agents
opencode models
opencode auth list
opencode mcp list
```

`debug config` should include `.opencode/opencode.jsonc`. `debug agents` should show the four configured roles and their exact provider IDs. `auth list` shows credential presence only; it does not prove a live model request. `mcp list` should report `rea` connected. A live smoke test is separate from these configuration checks and consumes the selected provider's allowance.
