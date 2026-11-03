# Architect integration checkpoint: local-main governance merged; source XMM repair verified as a candidate

November 3, 2026. This records one bounded integration recovery in the persistent source-only `codex/opencode-architect` worktree. It merges governance commits from local main and resolves the existing conflicts. It does **not** merge either builder lane, does not promote `main`, and does not establish original-game execution, rendering or playability. There is no playable original screen.

## Merged and resolved

Local main `6d8cc2d` (`Route bounded source reconnaissance to local Qwen Scout`) is merged into integration HEAD `d8239bf`. Two files conflicted and both were resolved by hand, preserving both histories rather than dropping either side.

- `.opencode/opencode.jsonc` — kept this worktree's integration defaults: top-level `openai/gpt-6.1-sol` with `default_agent: architect`, architect `steps: 32`, builder `opencode-go/deepseek-v4.1-flash` (`steps: 48`), reviewer `opencode-go/glm-5.3`, scout `unsloth/qwen3.8-27b`, Qwen context `32768`, and every existing policy/permission entry unchanged. Main's top-level builder defaults are correct for the builder worktree but were not adopted here; no global setting or active session model was touched.
- `docs/DECISIONS.md` — runtime rows D067–D131 (the independent runtime ledger, including the PFIFO/FATX/controller/geometry-component rows) are preserved verbatim. Local-main workflow rows keep their stable `W` IDs: W001–W007 were already integrated, and main's new `D074` (local Qwen Scout governance) is now `W008` with the old-ID mapping stated in the row and in the section note. Main's `D073` is the same decision as the already-integrated W007 and was not duplicated. The section note now records that former `D067`–`D074` map to `W001`–`W008`.

Governance content that came in with the merge and is now staged: the `AGENTS.md` Scout paragraph, Scout routing and the 32-step architect cap in `docs/OPENCODE_GOAL.md` / `docs/OPENCODE_WORKFLOW.md`, the November 3 Scout and architect-verification entries in `docs/PROJECT_STATUS.md`, user-level `UNSLOTH_API_KEY` inheritance (process-local, never printed or stored) in `tools/Invoke-OpenCodeCycle.ps1` and `tools/Invoke-OpenCodeArchitect.ps1`, and the architect prompt's bounded Scout-first instruction.

A new workflow row `W009` records this merge-resolution and ID-remapping decision.

## Integrated baseline versus candidate reports

The integrated baseline on this branch is unchanged: the adopted empty-ring profile reaches untranslated `0x1FB5B0` inside `FUN_001FB580`, build `recomp-post-memory-build-58` (527,379 bytes, SHA-256 `6b4ddf6f…`) with `settings-pfifo.json` and status 13 (261 routines) without it. The guard-memory blocker at `0x1FA460` and the empty-ring PUT guard (build-60) are as recorded in D130/D131.

Committed source head `0431e8b` (`Repair XMM type ownership and make vector helper admission fail-closed`) is a **candidate, not integrated**. It addresses the two reasons this branch previously withheld the earlier XMM work (`ee34a5c`: shared header failed standalone XMM compilation, and vector-helper admission was implicit):

- `src/runtime/recomp_memory_access.h` no longer declares `RecompXmm`; it owns only the checked `FF_MEMORY_SITE_POINTER` range contract, and `tools/recomp_memory_access.py` casts to `RecompXmm` at the generated use site.
- An `XMM_STORE` whose value probes memory is split into a temporary before the checked store.
- Only an explicit `PURE_XMM` allowlist is admitted; unknown `XMM_*` helpers, half-vector memory helpers, unrewritten `XMM_MEM`/`XMM_LOAD`/`XMM_STORE`, escaped pointers, `XBOX_PTR`, MMX/atomic helpers and malformed surfaces fail closed.

Independent verification performed here (detached worktree at exactly `0431e8b`, then removed): `tests/recomp_memory_access_test.py` passes 11/11, and the header inspection confirms no `RecompXmm` definition plus the generated-use cast. **Not** reproduced here: the owned C test `tests/recomp_memory_access_xmm_test.c` needs GCC with ASan/UBSan, and this host has no C compiler on `PATH` or under `.tools`. The committed optimized/ASan pass in that commit is the source lane's evidence, not an independent architect result.

Candidate browser reports from the same source head, unmerged and unverified on this branch: guarded recomposition of 399 functions / 16,266 instructions; private module `recomp-post-memory-build-64` (595,242 bytes, SHA-256 `e6a2bbdd…`) reproduces the same stop at `0x1FCB30` with 268 routines and still stops at status 13 without the profile with 261 routines.

## Address correction

The earlier architect directive's "`0x1FB600`" is stale and is corrected here. The verified original-code progression is `0x1FB3B0` → `0x1FB400` → `0x1FCA20` → `0x1FCB30`. `0x1FB5B0` remains the correct integrated-baseline stop; `0x1FB600` was never a reached stop and must not be cited. No document on this branch should carry `0x1FB600` forward.

## Geometry hold

Geometry head `9210a3b` (`Document retained-reference comparison contract`, on top of `94e7c28` triangle stream assembly) remains **unmerged**. The retained-reference comparison contract is documented, but commit-level and reference review are still pending, so the topology hold stays in place. No geometry component is a dependency of the current original-runtime blocker.

## Checks and boundary

`npm run check`, `npm run test:runtime`, `git diff --check` and staged-path inspection were run for this integration commit. These are source-publication and source-owned runtime-resource checks; they do not execute the original title.

Remaining gaps: full original fidelity, complete runtime/graphics/audio/controller behavior, player-supplied local conversion and every planned feature are unfinished; eventual online/social server authority is unproved. XMM repair review and a real C compile remain the immediate priority before further translation exports, with owned memory checks compiled rather than reasoned about. The geometry contract review stays pending. No original content, private research, pseudocode or credentials are committed.
