# Continuous development goal

Deliver the actual original Xbox Fuzion Frenzy executing entirely inside a desktop browser through static recompilation to WebAssembly, then complete the planned fidelity, controls, online and social features. Original game content stays player supplied and local. Do not substitute a streamed/native runtime or approximate replica, and do not call a component fixture or translated-function count playable.

## Operating loop

Use the clean source-only OpenCode worktree. At each cycle, read `AGENTS.md`, `docs/DEVELOPMENT_HANDOFF.md`, `docs/PROJECT_STATUS.md`, the active checkpoint and `docs/DECISIONS.md`. Select one **unowned** task that advances original execution toward a controllable original screen. Give it an observable acceptance result, scope and targeted checks. Do not overlap the in-progress memory-statistics work in the main checkout. If that is the only next task, wait for its owner to finish and incorporate the reviewed commit before continuing.

Delegate source discovery to local Qwen only after a real OpenCode Scout handoff passes; its current first-turn context error makes it optional. DeepSeek Go implements. Ask Go GLM 5.3 for independent review of CPU, memory, ABI and GPU changes. Call Sol automatically for a route or cross-subsystem decision, after two targeted failed attempts, or when GLM leaves a material correctness question unresolved. Give Sol the concise evidence and decision needed; accept one bounded recommendation per decision, then return implementation to DeepSeek. Do not invoke Sol for routine coding or repeat the same escalation without new evidence.

After each task, run targeted checks, update `PROJECT_STATUS.md` and consequential decisions, review the staged source-only paths, and commit the bounded result on the development branch. Record observed runtime behavior and remaining gaps. Continue to the next unowned task only after the prior task has a reviewed result and an identifiable next step. Stop the loop on failed checks, quota/auth failure, repeated uncertainty, dirty or divergent worktree, missing original evidence, or any step requiring user judgment. Never push or merge to main automatically.

## Round-the-clock local cycle

Windows Task Scheduler runs `tools/Invoke-OpenCodeCycle.ps1` every 15 minutes while this user is signed in and the PC is awake. Task name: `FuzionFrenzy-OpenCode-Cycle`. One invocation runs at most one DeepSeek builder prompt, with project step caps and a lock against overlapping runs. The script waits while the main checkout has in-progress edits. It may fast-forward the clean worktree to a newer reviewed main commit; it stops if the branches diverge. It also stops after a failed run, uncommitted work or no new commit, rather than repeating the same attempt. State and local-only logs live under ignored `.worktrees/`. Go Extra Usage is off; a quota or authentication failure pauses the cycle. Sol may be called as a bounded subagent under the escalation rules.

The schedule cannot run while the computer is asleep or signed out. Inspect it with `Get-ScheduledTask -TaskName FuzionFrenzy-OpenCode-Cycle`; stop it with `Disable-ScheduledTask -TaskName FuzionFrenzy-OpenCode-Cycle`. The current main checkout has uncommitted memory-statistics work, so the task is registered and waiting, with no development call launched yet.

## Goal prompt for a new architect session

> Coordinate steady progress toward actual original Fuzion Frenzy execution in a desktop browser by static recompilation to WebAssembly, preserving the complete project goal and local-only original content. Read AGENTS.md, the current handoff, status, decision log and active checkpoint. Define the next unowned milestone with acceptance criteria. Delegate bounded implementation to the DeepSeek Go builder and independent CPU/memory/ABI/GPU review to Go GLM where needed. Use local Qwen only after its OpenCode handoff works. Resolve consequential design questions with evidence, update the project record, and advance to the next milestone when the previous one is verified. Keep each cycle bounded; stop on failed evidence, quota, conflicts or decisions that require me. Do not duplicate the in-progress memory-statistics task or claim playability without a controllable original screen.

For lower Sol usage, start the same goal with `builder` as primary. It may now summon `architect` automatically under the escalation rules. Starting `architect` as the primary means Sol handles every coordinator turn, including routine ones, so it consumes more of the linked account allowance.
