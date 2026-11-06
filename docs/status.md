# Baseline status: implementation paused on main

Recorded November 6, 2026. This is the retained main-branch checkpoint, not a fresh title run. The pending PR is closed, retired branches are removed, and no implementation task is assigned. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

## Retained execution boundary

Commit `ab5b3f7` admits `FUN_001e733d` and `FUN_001e72a8`. The experiment advances through stream-table setup to **status 2 at `0x001E93E4`**, the next untranslated routine.

| Run/profile | Recorded stop | Executed routines / kernel operations |
| --- | --- | --- |
| Control | Status 13 at `0xFD800044` | 339 / 405 |
| Inert | Status 1 at `0xFE0000C8`, ordinal 100 | 347 / 419 |
| Experiment | Status 2 at `0x001E93E4` | 363 / 441 |

Prepared profile 96 contains **487 functions / 20,115 instructions**. Module 171 is **1,045,793 bytes**, recorded SHA prefix `57576342`. Prepared functions, observed routines, and kernel operations are distinct counts.

The original source record reports native fixtures 56/56, proof-report checks 23/23, runtime checks passing, and seed verification 32/32/0/0. These are historical results, not new checks run by editing this page. Raw reports, generated modules, and independent audits remain private local research; earlier detailed records remain in Git history.

## Acceptance and limits

- All five new project milestones are unaccepted. The prior runtime M1/M2 startup/device-policy checkpoints are supporting research; synthetic storage is not an NV2A implementation.
- Legacy runtime M6 is recorded met by D429 and repair commit `ad0bffa`: consuming the missing one-dword return address preserves the control and moves the experiment beyond `0x6B6A6968`.
- Graphics-return/presentation acceptance is not established. No controllable original screen, complete renderer/audio/controllers, full runtime, end-user conversion, full parity, or online/social delivery is accepted.
- The offline Twisted System app and format/geometry components remain research. Pending branch work was closed out without a merge.

## Baseline maintenance

Keep this page focused on main-branch facts and update the handoff with it. The five-milestone roadmap and native tracking issues retain planned work; there is no automatic implementation task. Source paths and management were reorganized after CTR-2-WUP without a new title run. Obsolete checkpoint files and duplicate snapshots have been removed, with useful reference facts consolidated in [research notes](research.md) and [the reference inventory](reference.md).
