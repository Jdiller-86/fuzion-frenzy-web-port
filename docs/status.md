# Project status: startup batch blocked on private inputs

Recorded November 6, 2026. The user-directed attempt to advance `FUN_001e93e4` stopped during prerequisite verification in this cloud workspace. The retained main-branch checkpoint below is historical; no fresh title run was possible. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

## Current goal and blocker

The requested scope was one bounded translation/admission batch centered on `FUN_001e93e4`, with actual browser advancement, control/inert invariance and an independent audit. The goal explicitly requires stopping when private prerequisites are missing.

Two read-only worker checks and a coordinator metadata check found these documented local paths absent:

| Required baseline material | Documented location | Observation |
| --- | --- | --- |
| Matching player-supplied executable | `research/reference/default.xbe` | Missing |
| Retained analysis project | `research/ghidra-projects/FuzionFrenzySetup.gpr` | Missing |
| Seeds, prepared profiles/modules, paired reports, audit and provenance | `research/verification/` | Directory missing |

The documented Emscripten checkout and preparation environment under `.tools/emulation/emsdk/` and `.tools/emulation/recomp-venv/` are also absent. The source-owned synthetic XBE and browser-test fixtures present under ignored research/tool directories do not establish the matching original baseline.

The retained profile 96, module 171 and control/inert/experiment identities therefore cannot be verified here. No translation admission, private rebuild, original-browser replay, control/inert comparison or runtime correctness audit was performed. Source-only checks cannot satisfy that evidence requirement; M1 remains unaccepted.

The next dependency is access to the existing private baseline on a research workstation or in an appropriately configured local session. Verify its executable, seeds, toolchain, prepared module and report/audit identities, then reproduce the retained stops before attempting the bounded routine batch. Keep original inputs, generated title code and raw evidence local; missing inputs do not authorize downloading replacements or inventing behavior. See [analysis setup](analysis.md).

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

Keep this page focused on main-branch facts and update the handoff with it. The five-milestone roadmap and native tracking issues retain planned work. This session ended at the missing-input stop condition; implementation remains paused. Source paths and management were reorganized after CTR-2-WUP without a new title run. Earlier evidence remains consolidated in [research notes](research.md) and [the reference inventory](reference.md).
