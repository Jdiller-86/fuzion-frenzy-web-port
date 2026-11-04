# Integration evidence index and hold

November 4, 2026. This index hands off the corrected report/helper slice and the retained admission evidence for the architect's exact-head review. It is not a merge or promotion request. The source lane is parked; the last source/CI commit is `3ef6024`, after the Sol architect's `09:30` decision to hold further admission pending that review. Later commits are docs-only hold records.

## Source lane

- Branch/head: `codex/opencode-source`, last source/CI commit `3ef6024` (`Run the full Python evidence suite in preparation CI`); docs-only hold records follow.
- Corrected report/helper and verification slice:
  - `3f97afa` truthful configured denials, exact success shape/address, paired call arguments, full supplied/declared input comparison
  - `35a664a` ordinal-2 denial pairs with the status-1 unsupported-service stop
  - `12deaad` shared `validateSuppliedDeviceProfile` in the report contract and comparison tool
  - `0ffb634` full declared input profile comparison
  - `b9d47cf` review fixes: mixed completed+denial pairing and required declared-input presence
  - `50af71a` configured-prequery acceptance plus the bounded `0x1F79A0` admission
  - `5e492e1` reject a no-call prequery that claims the ordinal-2 stop
  - `2450b7e` truthful avQuery outcome presentation in the local startup checker
  - `d5ee7af` require the ordinal-2 call record for every unsupported ordinal-2 stop
  - `e1b0a28` run the controller native fixtures in the native suite
  - `a74e2ce` run the previously unrun guest/pfifo native fixtures
  - `79f20c2` run the native component suite in preparation CI
  - `3ef6024` run the full Python evidence suite in preparation CI
- Source files touched: `src/runtime/proof-report.mjs`, `src/runtime/device-report.mjs`, `src/runtime/thread-proof-worker.mjs`, `src/runtime/local-title-proof.mjs`, `tools/compare-proof-profiles.mjs`, `tools/fixtures/thread_startup_probe.c`, `tools/fixtures/thread_startup_check.html`, `tools/run-native-tests.mjs`, `.github/workflows/prepare.yml`, `tests/proof-report.test.mjs`, `tests/compare-proof-profiles.test.mjs`, `tests/av-query-outcome.test.mjs`, `package.json`.
- Targeted checks on this slice: runtime 179/179, evidence 4/4, native 34/34, `npm run check` 354 source files, `git diff --check` clean; pair comparison exits 0. Preparation CI runs `npm run test:native` on Linux and the full `npm run test:evidence` suite.
- Narrow GLM review (`2026-11-04T09:15:22`, step-capped after the checks) found no material defect; the control stop target `0xFE000094` and the `0x1F79A0` enter/exit trace were confirmed from the retained reports. A follow-up narrow review at `2026-11-04T10:05:11` of the no-call prequery guard also found no material defect; minor unexercised control-path cases are noted in the binding checkpoint.
- Presentation: `src/runtime/local-title-proof.mjs` exports `describeAvQueryOutcome`, used by `tools/fixtures/thread_startup_check.html` to label the result as completed, denied/unsupported or prequery; `tests/av-query-outcome.test.mjs` covers it. Runtime 179/179.

## Retained admission evidence

- Module `recomp-post-memory-build-71`, SHA-256 `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`.
- Control `browser-71-reset3.json`: status 1 at `0xFE000094`, ordinal 2, 279 routines / 404 kernel ops.
- Explicit `browser-71-reset-avquery.json`: status 2 at `0x1F6EB0`, 281 / 405; `0x1F79A0` executed and returned; next frontier entry `0x1F6EB0` called by executed `0x1F83C0`.
- Translation `recomp-title-recomp-title-post-memory-50`: 419 functions / 17,448 instructions from 32 retained seeds. `verify-seeds.py` reports 32 recorded / 32 offered / 0 unexpected / 0 missing; `verify-translation.py` reports 517 functions, 0 needing rewriting, fully instrumented.
- `0x1F6EB0` is **not** admitted or exported. No storage/framework/substitute work was added.

## Main/PR integration state

- `origin/codex/opencode-architect` is `cc07556` (the directive's new integration head). It contains `385dae5`, `62149dd`, `3a5d94c` and `ac207ba`.
- `385dae5` is an ancestor of `main`; `ac207ba` is **not** an ancestor of `main`; the guarded-promotion PR remains unmerged.
- `ac207ba` imports the reviewed unconnected `src/runtime/av_query.c`, `av_query.h`, `tests/av_query_fixture_test.c`, and adds Linux plain plus ASan/UBSan fixture checks to `.github/workflows/prepare.yml`. No guest binding, profile activation or title claim.
- Local `codex/opencode-architect` and its remote are both `cc07556`. Local `main` is `3a5d94c`; `origin/main` is `1a67c4f`; the source branch is 182 ahead / 27 behind local `main`.
- PR [#1](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/1) is open from `codex/opencode-architect` at the exact head `cc07556` into `main` (`1a67c4f`). GitHub reports `MERGEABLE`/**`CLEAN`**: all four preparation checks (Ubuntu and Windows across two workflow runs) pass; no reviews.
- **Integration conflict:** the report/helper slice commits (`3f97afa`, `35a664a`, `12deaad`, `0ffb634`, `b9d47cf`, `50af71a`, `5e492e1`, `2450b7e`, `d5ee7af`) are **not** ancestors of `cc07556`, and the head does **not** contain `src/runtime/proof-report.mjs`, `tools/compare-proof-profiles.mjs` or `tests/av-query-outcome.test.mjs`. The directive's statement that the new head integrates the primary's report/helper slice is not reflected in the exact head. The source branch (`4ff87de`) still carries that slice.
- Merge/conflict check against local `main` (`3a5d94c`): 182 ahead / 27 behind; the only conflict is `docs/DECISIONS.md`; `AGENTS.md` and `docs/PROJECT_STATUS.md` auto-merge. The AV component blobs are identical between `ac207ba` and the source-lane `HEAD`; code and CI merge cleanly, and the decision log needs an ordered append.
- The source lane's native suite passes 34/34; the exact-head PR CI above is the gate.

## Hold and next

The Sol architect (`2026-11-04T09:30`) selected **A: hold further admission pending exact-head integration review**. A second escalation at `2026-11-04T09:55:09` failed with `subscription_sharing_usage_limit_exceeded`, and a third at `2026-11-04T10:20:10` failed with the same provider limit; a fourth at `2026-11-04T11:10:10` failed the same way; a fifth at `2026-11-04T12:00:17` also failed with the provider limit; a sixth at `2026-11-04T13:00:07` failed the same way. The hold remains unresolved. The new exact head `cc07556` contains the governance commits but not the report/helper slice; its CI is now green and no review is recorded, so the gate remains unmet. After this handoff the lane is parked: do not export/admit `0x1F6EB0`, expand storage/frameworks, or add substitute work. Geometry remains paused. Release requires the architect's independent review of the corrected slice and exact current PR head, resolution or naming of material blockers, and an explicit next-admission authorization.

The control path remains unsupported; `avQueryWord` stays explicit and synthetic. No controllable original screen, fidelity, online/social or authority claim is established.
