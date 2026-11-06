# Local agent execution

The checked-in [Codex configuration](../.codex/config.toml) and [repository instructions](../AGENTS.md) carry the agreed session structure to a local checkout:

| Role | Default | Responsibility |
| --- | --- | --- |
| Coordinator | `gpt-6.1-sol`, high reasoning | Scope, delegation, integration, review and publication |
| Workers | `gpt-6-luna`, low reasoning | Bounded, independently useful work with explicit ownership and checks |
| Independent correctness reviewer | Sol High when needed | CPU/memory/ABI/GPU review, within the same two-worker cap |

At most two spawned workers run concurrently. The coordinator can request medium reasoning for a bounded Luna task. It keeps serial work together and uses task-specific context for workers. Workers report back; the coordinator owns commits and pushes. No persistent scheduler or unattended development service is installed.

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
and two-worker Luna Low setup with two fresh-context, read-only workers:
one summarizes docs/status.md and docs/handoff.md; the other lists the
package.json scripts. Do not edit files, run tests, or begin runtime work.
Report each worker's actual model/reasoning and the concurrency limit
from execution metadata if available. State any unavailable capability.
```

Check the primary model selection and the model/reasoning shown for each worker in the agent details or spawn records. Expected: Sol High leads, two Luna Low workers handle the checks, and only the coordinator integrates their findings. A worker's prose self-description is insufficient evidence of model selection. If the client does not expose execution metadata, record that verification limit.

`npm run check` validates the repository's saved defaults and publication guards. It does not validate account entitlements, runtime configuration precedence or actual worker execution on another machine.

## Development gates still apply

Agent availability does not restore the private game/research baseline. The bounded `FUN_001e93e4` goal remains blocked in a public-only checkout; follow [status](status.md), [handoff](handoff.md) and [analysis setup](analysis.md) before starting it. Private prerequisites stay local, and source-only checks cannot accept an original-runtime milestone.
