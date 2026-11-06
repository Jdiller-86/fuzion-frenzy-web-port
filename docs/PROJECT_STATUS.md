# Current checkpoint: CRT stream setup reaches translation placeholder `0x001E93E4`

Recorded November 6, 2026. This page summarizes the newest retained runtime observation; it is not a new title execution or a claim of playability. [Handoff](DEVELOPMENT_HANDOFF.md) · [Roadmap](../DEVELOPMENT_PLAN.md) · [Runtime gates](RUNTIME_MILESTONES.md)

## Latest observed boundary

The batch admitting `FUN_001e733d` and `FUN_001e72a8` advances the experiment through stream-table setup to **status 2 at `0x001E93E4`**, the next untranslated routine. Source record: commit `ab5b3f7`, preserved at the start of the [pre-rebuild status snapshot](archive/PROJECT_STATUS_PRIOR_20261106.md); reviewed integration is present in this checkout's history.

| Run/profile | Recorded stop | Executed routines / kernel operations |
| --- | --- | --- |
| Control | Status 13 at `0xFD800044` | 339 / 405 |
| Inert | Status 1 at `0xFE0000C8`, ordinal 100 | 347 / 419 |
| Experiment | Status 2 at `0x001E93E4` | 363 / 441 |

Prepared profile 96 contains **487 functions / 20,115 instructions**. Module 171 is **1,045,793 bytes**, with recorded SHA prefix `57576342`. Prepared functions, actually executed routines, and kernel operations are different counts.

The source record reports native fixtures 56/56, proof-report checks 23/23, runtime checks passing, seed verification 32/32/0/0, publication checks passing, and clean whitespace. These are historical reported results, not checks rerun by editing this page. Raw paired browser reports, generated modules, and audit artifacts stay in private local research; the public snapshot provides curated findings.

## Milestones and limits

- M1 and M2 have bounded recorded completion evidence. Their synthetic device policy is not an NV2A implementation.
- M6 is recorded met in decision D429, citing repair commit `ad0bffa`: `AvGetSavedDataAddress` had left a one-dword return address on the stack. Consuming it preserved the control and moved the experiment past the former `0x6B6A6968` fault. Earlier translator/pop-order diagnoses were superseded.
- M3/M4 graphics-return/presentation acceptance is not established. M5 and M7–M12 remain unaccepted: no controllable original screen, complete renderer/audio/controllers, full runtime, end-user conversion, full parity, or online/social delivery.
- The Twisted System app and geometry/format components remain research evidence. They do not accept original-game behavior.

## Next bounded result

Target the M3 startup path toward M7 output: inspect/admit the reached `FUN_001e93e4` using retained seeds and the existing byte/decoder/layout/memory checks. Rebuild privately and compare the named control, inert, and experiment profiles. Stop at the next unsupported function, service, register, resource, or failed check; record the exact result.

Keep admission/preparation/frontier work and reached-service/fixture work under explicit contributor ownership. A public clone without compatible private inputs can work on source-owned prerequisites and fixtures, but cannot claim a fresh title run.

## Record maintenance

Keep this page to roughly 60 current lines. Move replaced observations into [the archive](archive/README.md), and update the handoff in the same change. Decision IDs in the legacy log have renumbering/duplicate history; cite the decision subject and commit as well as its ID. See the [research index](RESEARCH.md) and [metrics](archive/PROGRESS_METRICS_PRIOR_20261106.md) for evidence and delivery tempo.
