# Architect integration checkpoint: candidate admission gaps block integration

## Current reviewed disposition

Git verifies integration `838f825dfaa8cc6592bd1065fffbe8d4b2b2da5e` has parents `d8239bf` and local main `6d8cc2d30b3a13b4217e5a1233b852b972ed443e`. There is no pending merge. The only unfinished artifact was `docs/XMM_CANDIDATE_REVIEW.patch`, a source-only diff from the prior cycle. It was inspected and preserved at `C:/Users/jacob/AppData/Local/Temp/opencode/XMM_CANDIDATE_REVIEW-838f825-retained.patch`, not discarded or staged for publication.

Reviewed candidate refs: source `f54e411f51348bc6315e9c0dbc3f713dd1b9440e`, geometry `0fd820a23f8e91b60fb20ef68ab8ab31a7e85fe4`. Neither is integrated. Source spans 28 changed paths since its merge base with this branch; geometry spans 14, including lane configuration/workflow history that must not overwrite architect defaults. Source reports private build-68 (637,395 bytes, SHA-256 `75f66f41…`) reaching 277 routines, 83 initializer returns and 403 kernel operations, then status 9 / `0xC00000BB`, diagnostic ordinal 149, return `0x200F73`. No private input or original module was accessed or executed by this review.

### Reproduced blockers and bounded acceptance

- Exact committed `tools/recomp_memory_access.py` accepts owned input `XMM_MEM(eax) = xmm0;` and emits a store through `FF_MEMORY_SITE_POINTER(..., 16u, 1u, site)`, i.e. **read** intent, counting zero writes. Reject unsupported vector lvalues/address escapes or classify their true access intent. These are adversarial admission probes, not a claim that the pinned title emitter produces them.
- Owned input `XMM_Unknown(eax);` is returned unchanged: the final helper regex matches only uppercase suffixes. Reject every unadmitted helper spelling, not merely uppercase unknowns. Preserve the explicit pure-helper allowlist and nested-store temporary.
- The 42-line XMM fixture uses a 64-byte array and a stub probe returning aligned in-bounds pointers without testing access permissions. It never compiles the generated vector dereference. Keep it as a forwarding/header test, but add compiled generated loads/stores with a minimal independently authored compatible vector contract and real VM checks: unaligned supported RAM, page ends/cross-page denial, read-only/reserved denial before mutation, site/width attribution, nested-store ordering and vector-MMIO rejection. Do not copy original code or the entire upstream type system.
- Candidate `ff_port_io_read` returns successful zero for unknown ports/invalid widths, and `xbox_PortIn` passes it through. Require explicit unsupported stops instead. Treat the 0x80C0 per-read toggle as opt-in synthetic proof behavior, not original vblank timing. Do not acquire progress by implicitly supplying more device values.

An independent GLM reviewer read the retained patch and confirmed both vector admission findings and the fixture limitation. The underlying repairs are bounded and do not reject static recompilation or require a new runtime architecture. The prior provisional demand to make upstream `RecompXmm`/`RecompMmx` types source-owned was unsupported and is withdrawn: pinned generated-use ownership is valid; independently authored compatible test types are sufficient for owned fixtures.

After these safety repairs, localize the timer/clock failure in the retained run. `sample_clock` tags errors from `ff_timers_advance` as ordinal 149; `set_timer` also tags errors from `ff_timer_set`. The number alone does not establish a new missing service or rejected due time. Capture the branch and first invariant failure without forcing time, clearing queued timers or suppressing guest mutations.

Geometry's latest delta from `9210a3b` adds a heap-boundary test and documentation for the four-word reference-compatible triangle state; it adds no renderer execution or reference result. The complete unmerged topology chain still lacks whole-chain review and new reference comparisons. Hold new topology expansion; resume only for a concrete original-renderer dependency or agreed retained-reference validation. Do not create a duplicate oracle/decoder pipeline.

Architect decisions A001–A003 in `DECISIONS.md` govern these priorities. Neither source nor geometry is merged or approved for main promotion. Full fidelity, local-only conversion, controller/audio/video behavior, saves/disc reads, performance and all planned online/social/server-authority features remain unaccepted. The selected static-recompilation route remains active; no approximate reconstruction, streaming, native helper or upload of original-containing RAM/code/saves is authorized.

### Current validation

- On a clean detached snapshot at exact source `f54e411`, the validation builder independently compiled the header and regenerated the owned scalar/stack/bulk fixture. The memory-access, stub XMM, port and device-register fixtures passed GCC 13.3 optimized and ASan/UBSan runs through WSL Ubuntu. Python emitter tests passed 11/11, title-seed tests 25/25, and runtime tests 110/110. Both adversarial emitter findings above were independently reproduced there. The temporary worktree was removed non-force after its own synthetic test outputs were cleaned; logs and owned binaries remain in the approved temporary directory.
- These C passes do **not** compile generated vector dereferences or establish real vector VM permission coverage. Such an owned regression can be generated from synthetic input and a minimal compatible test type; it does not require original routines or private game files.
- On this integration worktree, `npm run check` passes for 291 source files, `npm run test:runtime` passes 110/110, and `git diff --check` passes. Publication/delivery fixtures do not execute the original title. Candidate tests used the installed Node 24.22.1, not the project-pinned 24.14.0; do not misreport a pinned-runtime run.
- One bounded read-only Scout lookup was attempted against the retained fixture diff. No usable Scout finding was available when recording this checkpoint; no decision depends on Scout. The architect directly checked the fixture and committed emitter and obtained the independent GLM review described above.

## Prior recovery record (corrected where noted)

November 3, 2026. This records one bounded integration recovery in the persistent source-only `codex/opencode-architect` worktree. It merges governance commits from local main and resolves the existing conflicts. It does **not** merge either builder lane, does not promote `main`, and does not establish original-game execution, rendering or playability. There is no playable original screen.

## Merged and resolved

Local main `6d8cc2d` (`Route bounded source reconnaissance to local Qwen Scout`) was merged with prior integration `d8239bf` in commit `838f825`. Two files conflicted and both were resolved by hand, preserving both histories rather than dropping either side.

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

The prior recovery verified Python tests 11/11 at `0431e8b` and inspected type ownership. Its claim that this host lacked a compiler was incorrect: GCC is available through `wsl -d Ubuntu`. The current review uses exact committed snapshots and owned fixtures; passing the stub XMM fixture must not be reported as generated-vector/real-VM coverage.

Candidate browser reports from the same source head, unmerged and unverified on this branch: guarded recomposition of 399 functions / 16,266 instructions; private module `recomp-post-memory-build-64` (595,242 bytes, SHA-256 `e6a2bbdd…`) reproduces the same stop at `0x1FCB30` with 268 routines and still stops at status 13 without the profile with 261 routines.

## Address correction

Source candidate decision D133 corrects **both** old labels: `2077616 = 0x1FB3B0`, followed by `2077696 = 0x1FB400`, then `0x1FCA20` and `0x1FCB30`. The prior recovery incorrectly retained `0x1FB5B0` as a different integrated stop; it was also a decimal-to-hex slip. Old labels elsewhere in the historical record are not new runtime evidence.

## Geometry hold

Geometry head `9210a3b` (`Document retained-reference comparison contract`, on top of `94e7c28` triangle stream assembly) remains **unmerged**. The retained-reference comparison contract is documented, but commit-level and reference review are still pending, so the topology hold stays in place. No geometry component is a dependency of the current original-runtime blocker.

## Checks and boundary

`npm run check`, `npm run test:runtime`, `git diff --check` and staged-path inspection were run for this integration commit. These are source-publication and source-owned runtime-resource checks; they do not execute the original title.

Remaining gaps: full original fidelity, complete runtime/graphics/audio/controller behavior, player-supplied local conversion and every planned feature are unfinished; eventual online/social server authority is unproved. XMM repair review and a real C compile remain the immediate priority before further translation exports, with owned memory checks compiled rather than reasoned about. The geometry contract review stays pending. No original content, private research, pseudocode or credentials are committed.
