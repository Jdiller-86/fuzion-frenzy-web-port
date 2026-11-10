# Local agent execution

<!-- development-state:start -->
Recorded development state: **M1 / awaiting-evidence**, profile **44**. Retained startup resume task: **SCOPE-37**.

The selected C allocation wrapper and SDK allocator return; startup reaches the existing logger provider's new-caller guard.

[Current runtime evidence](status.md) owns detailed observations and limits. [Structured state](../profiles/development-state.json) owns this generated pointer; [workflow](development-workflow.md) explains preflight and updates. [Goal G](local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) owns the SDK integration assignment; the retained startup stop is a reference boundary. Source-only work can proceed without private inputs; original execution requires the matching local baseline. No milestone is accepted by this record.
<!-- development-state:end -->

The checked-in [Codex configuration](../.codex/config.toml) and [repository instructions](../AGENTS.md) carry the agreed session structure to a local checkout:

| Role | Default | Responsibility |
| --- | --- | --- |
| Coordinator | `gpt-6.1-sol`, high reasoning | Scope, delegation, integration, review and publication |
| Workers | `gpt-6-luna`, low reasoning | Bounded, independently useful work with explicit ownership and checks |
| Independent correctness reviewer | Sol High when needed | CPU/memory/ABI/GPU review, within the same four-worker cap |

The project allows up to four concurrent spawned workers in a fresh trusted session, subject to the client's effective limit. The coordinator can request medium reasoning for a bounded Luna task. It keeps serial work together and uses task-specific context for workers. Workers report back to the coordinator, who owns commits and pushes. No persistent scheduler or unattended development service is installed.

## Apply locally

1. Update the local checkout with `git pull --ff-only`; preserve any uncommitted work first.
2. Open the repository root as the Codex working folder. Review and trust the project through the client's normal trust flow so project configuration can load.
3. Use a current Codex build that supports the configured agent options and exposes the Sol/Luna model IDs to your account. The [upstream configuration schema](https://github.com/openai/codex/blob/main/codex-rs/core/config.schema.json) documents the `agents` options; check your installed client's supported version.
4. Start a fresh session with Sol High selected. A session or model-picker override can supersede the project default, and an existing conversation may retain earlier configuration.
5. Run the read-only check below and inspect actual execution metadata before starting development.

These are project defaults for future sessions. They do not change the active chat's model, user-global configuration, credentials or account model access. If your local client does not expose a model or multi-agent tools, the files cannot provide that capability. Report the limitation and use the client's supported configuration; do not claim that parsing a settings file proves the requested models ran.

## Verify execution

Use this first-session prompt:

```text
Read AGENTS.md and .codex/config.toml. Verify the intended Sol High lead
and four-worker Luna Low setup. Spawn four fresh-context, read-only workers
concurrently: one summarizes docs/status.md, one summarizes docs/handoff.md,
one lists the package.json scripts, and one lists the milestone acceptance
criteria in docs/milestones.md. Do not edit files, run tests, or begin runtime work.
Report each worker's actual model/reasoning and the concurrency limit
from execution metadata if available. State any unavailable capability.
```

Check the primary model selection and the model and reasoning shown for each worker in the agent details or session records. Verify that all four Luna Low worker turns overlap while Sol High leads. Only the coordinator integrates their findings. Use execution records to confirm model selection and concurrency. If the client does not expose those records, report the verification limit.

`npm run check` validates the repository's saved defaults and publication guards. It does not validate account entitlements, runtime configuration precedence or actual worker execution on another machine.

## Development gates still apply

Use one local lead with source, private inputs and toolchain to keep the SDK build-run-fix loop moving. [Goal G](local-agent-handoff.md#g-sustained-browser-execution-and-host-delivery-mb05) prioritizes original browser execution; detailed testing and independent correctness review follow the working baseline. Preserve ownership, M44 and private inputs. The 150-minute point is a short progress checkpoint, not a handoff or stop while productive work remains. [Status](status.md) records accepted evidence separately from the prototype.