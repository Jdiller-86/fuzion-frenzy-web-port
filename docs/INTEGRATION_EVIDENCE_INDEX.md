# Integration evidence index and hold

November 4, 2026. This index hands off the corrected report/helper slice and the retained admission evidence for the architect's exact-head review. It is not a merge or promotion request. The source lane is parked at `cae269f` after the Sol architect's `09:30` decision to hold further admission pending that review.

## Source lane

- Branch/head: `codex/opencode-source`, `cae269f` (`Re-verify the 0x1F79A0 admission seeds and translation guards`).
- Corrected report/helper slice, `3f97afa`..`5e492e1`:
  - `3f97afa` truthful configured denials, exact success shape/address, paired call arguments, full supplied/declared input comparison
  - `35a664a` ordinal-2 denial pairs with the status-1 unsupported-service stop
  - `12deaad` shared `validateSuppliedDeviceProfile` in the report contract and comparison tool
  - `0ffb634` full declared input profile comparison
  - `b9d47cf` review fixes: mixed completed+denial pairing and required declared-input presence
  - `50af71a` configured-prequery acceptance plus the bounded `0x1F79A0` admission
  - `5e492e1` reject a no-call prequery that claims the ordinal-2 stop
- Source files touched: `src/runtime/proof-report.mjs`, `src/runtime/device-report.mjs`, `src/runtime/thread-proof-worker.mjs`, `tools/compare-proof-profiles.mjs`, `tools/fixtures/thread_startup_probe.c`, `tests/proof-report.test.mjs`, `tests/compare-proof-profiles.test.mjs`.
- Targeted checks on this slice: runtime 178/178, evidence 4/4, native 20/20, `npm run check` 352 source files, `git diff --check` clean; pair comparison exits 0.
- Narrow GLM review (`2026-11-04T09:15:22`, step-capped after the checks) found no material defect; the control stop target `0xFE000094` and the `0x1F79A0` enter/exit trace were confirmed from the retained reports.

## Retained admission evidence

- Module `recomp-post-memory-build-71`, SHA-256 `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`.
- Control `browser-71-reset3.json`: status 1 at `0xFE000094`, ordinal 2, 279 routines / 404 kernel ops.
- Explicit `browser-71-reset-avquery.json`: status 2 at `0x1F6EB0`, 281 / 405; `0x1F79A0` executed and returned; next frontier entry `0x1F6EB0` called by executed `0x1F83C0`.
- Translation `recomp-title-recomp-title-post-memory-50`: 419 functions / 17,448 instructions from 32 retained seeds. `verify-seeds.py` reports 32 recorded / 32 offered / 0 unexpected / 0 missing; `verify-translation.py` reports 517 functions, 0 needing rewriting, fully instrumented.
- `0x1F6EB0` is **not** admitted or exported. No storage/framework/substitute work was added.

## Main/PR integration state

- `origin/codex/opencode-architect` is `7bf8ed1` (the directive's integration head). It contains both `385dae5` and `ac207ba`.
- `385dae5` is an ancestor of local `main` (`98799dc`). `ac207ba` is **not** an ancestor of `main`; it lives on the architect branch. The guarded-promotion PR therefore remains unmerged.
- `ac207ba` imports the reviewed unconnected `src/runtime/av_query.c`, `av_query.h`, `tests/av_query_fixture_test.c`, and adds Linux plain plus ASan/UBSan fixture checks to `.github/workflows/prepare.yml`. No guest binding, profile activation or title claim.
- Local `codex/opencode-architect` is `e199a6e`, one commit ahead of the remote `7bf8ed1`; that extra commit is not part of the exact integration head. Local `main` (`98799dc`) and `origin/main` (`1a67c4f`) differ.
- PR [#1](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/1) is open from `codex/opencode-architect` at the exact head `7bf8ed1` into `main` (`1a67c4f`). GitHub reports `MERGEABLE`/`CLEAN`; all four preparation checks (Ubuntu and Windows across two workflow runs) completed successfully, including the Linux native AV-query fixture added by `ac207ba`.
- `reviewDecision` is empty and `reviews` is empty: no architect review is recorded on PR #1 yet, so the hold's review/authorization condition remains unmet. The PR is open and unmerged; `ac207ba` is still not in `main`.
- The local native suite also passes 20/20, but the exact-head PR checks above are the remote CI record.

## Hold and next

The Sol architect (`2026-11-04T09:30`) selected **A: hold further admission pending exact-head integration review**. After this handoff the lane is parked: do not export/admit `0x1F6EB0`, expand storage/frameworks, or add substitute work. Geometry remains paused. Release requires the architect's independent review of the corrected slice and exact current PR head, resolution or naming of material blockers, and an explicit next-admission authorization.

The control path remains unsupported; `avQueryWord` stays explicit and synthetic. No controllable original screen, fidelity, online/social or authority claim is established.
