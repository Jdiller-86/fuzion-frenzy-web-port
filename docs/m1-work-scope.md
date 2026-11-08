# M1 measured work scope

Use this ledger to select bounded development tasks and reuse existing implementations. It records measured coverage, source candidates and unresolved work separately. It is not a count of manual fixes, an M1 percentage or a delivery estimate. [Status](status.md) owns the current runtime frontier; [milestones](milestones.md#m1--original-browser-execution) owns acceptance.

## Current planning refresh: profile sixteen

The current prefix enters `0x0004D4F0`, returns from `0x0004DC30`, then stops at guarded `0x0004BF10`, return `0x0004D539`, SP `0x03C10E44`, six words `[0x50, 0x0025BFA8, 0x5B, 0x2001, 0x0025BFCC, 0]`, after **455 routines / 835 operations**. Module `8cd4fce25113ca90dfaceb95f18fb3c2bb3de85790640931d10f26ca3f8e2201` is 1,233,429 bytes. Profile fifteen canonical report and all 413 words reproduce except module and the expected mode/entry count fields. See [status](status.md) and private `sdk-04d4f0-prefix-20261108/` comparison/review.

SCOPE-08 is verified only through the reached `0x0004D4F0` prefix and actual `0x0004DC30` helper return; the full body and shared continuation remain incomplete. SCOPE-09 proposes the next bounded work at the guarded `0x0004BF10` frame. Its purpose and ABI remain to be established; static source is not admission.

The affected import join remains **63 of 111 represented**, leaving **48 unrepresented (45 functions / 3 data)** after ordinal 156. M1 necessity is unknown. The old `0x49990` closure remains **188 candidates / 102 providers / 86 without either**; the `0x4B230` closure remains **126 / 100 / 26**. Two new mappings are outside both closures. These joins include three checked C views outside lookup and do not establish callable support. Current-scope proof: `sdk-04d4f0-prefix-20261108/current-scope.json`.

Affected current-scope evidence confirms provider/import classifications are unchanged. M1-required subset remains unknown; no whole-image rescan or routine-count subtraction was needed. The original observation below remains a dated baseline.

## Reviewed observation: 2026-11-07

Source checkpoint: `9c434ac0dd29b9a1278d7001d8b04bbdf5b0d780`. A fresh four-case Chrome 156.0.8078.80 startup comparison reproduced the retained default and selected reports on the same module. Selected profile nine uses the declared cold-start model and empty-input HOST device initializer; it does not execute the original USB initializer.

| Scope | Measured amount | Development implication |
| --- | --- | --- |
| Actual selected startup | 450 distinct original routines entered; 741 kernel operations completed; first stop `0x00049990`, return `0x00049D80`, SP `0x03C10E54` | Start from this reached dependency. Routine entry is not successful completion or gameplay acceptance. |
| Direct-source closure rooted at `0x00049990` | 188 candidate functions: 100 with retained full C bodies; 88 without a retained full C body or current SDK mapping | Check reuse before writing adapters or translating manually. All 188 have generated definitions; source presence does not establish correct call support. |
| Shared-helper occurrences in that closure | REP helpers in 10 functions / 19 sites; indirect calls in 16 functions / 30 sites; resume operations in 2 functions | Group reached incompatibilities by shared interface. These categories overlap and are not separate fix totals. No explicit codegen `unimplemented:` markers occur in this closure, but runtime/helper incompatibilities remain. |
| Original kernel import inventory | 111 unique slots/ordinals; 60 represented, comprising 58 function and 2 data imports; 51 unrepresented, comprising 47 function and 4 data imports | Use the import inventory when a service is reached. Representation is not general semantic support or SDK bridge admission. |
| Whole-image generated candidate | 10,589 unique generated definitions; 472 explicit unsupported-instruction markers across 180 functions | Retain as whole-game planning evidence. Other minigames/library code and decode gaps prevent treating this as the M1 task list. |

The 59 binding-table pairs plus typed ordinal 164 account for the 60 represented imports. Ordinal 40 is data despite appearing in the binding table. The SDK shared kernel bridge admits only its separately checked ordinal-171/234 paths; 60 represented imports do not mean 60 arbitrary SDK calls work. The native route's reported 111/111 coverage is a source claim, not browser or retail-kernel evidence.

The broader original-source closure rooted at `0x0004F7A0` contains 688 candidates, with 177 retained C/current SDK providers and 511 without either. It traverses the original USB implementation that selected profile nine replaces with HOST behavior. It is therefore reference scope, not the active profile's required backlog.

## Tracked work and selection order

The coordinator owns integration and acceptance. SCOPE-01/02/03/06/07 have bounded observations; SCOPE-08 has only a partial prefix observation. SCOPE-09 static preparation is delegated, with coordinator integration and independent correctness review required before admission. Other implementation entries remain unassigned. Private candidates and reports stay under ignored verification directories; public changes contain curated findings only. Source candidates become observed dependencies only when a valid path reaches them.

| ID | Evidence and state | Functional objective / scope | Acceptance and stop condition |
| --- | --- | --- | --- |
| SCOPE-01 | Empty-state original initializer returned in profile thirteen; broader input behavior unaccepted | Advance `0x00049990` using the existing private SDK slice/profile and common checked runtime. Review its reached 36-DWORD zero fill rather than invent another per-function memory implementation. | Original-byte/layout and caller-frame agreement; targeted helper check and independent review for changed CPU/memory/ABI; one focused browser comparison. Preserve older profiles and caps; stop at the next unsupported behavior. |
| SCOPE-02 | Both exact unconditional zero-mask logging calls verified; other callers guarded | Reuse retained C `0x0004B230` through the checked SDK-to-C boundary. Its 108-instruction body is reused without duplication; retain explicit callsite/frame and zero-control policy for later callers. | Establish actual argument frame, inherited SEH/frame state, registers, flags/FP requirements and reentry policy. Two direct callees, `0x0004C5F0` and `0x0004C670`, remain C stop stubs. Preserve their stops; source presence alone cannot retire a guard. |
| SCOPE-03 | Both exact queries returned in profile thirteen; initialized memory-unit zero is policy, not hardware evidence | Assess generated `XGetDevices` / `XInputOpen` interfaces against the matching SDK and existing browser controller components. Reuse the current host policy only where its declared contract applies. | Admit only reached calls with original frame/data ownership evidence. Keep empty snapshot, connected input and live polling distinct. Stop at unsupported device/service behavior. |
| SCOPE-04 | Two scoped functions and the reached clock data export observed; 48 unrepresented (45 functions / 3 data), M1 subset unknown | Select a reached missing service, group related work at existing memory/thread/file/dispatch interfaces, and consult pinned native implementations before writing a new handler. | Record slot/ordinal, function versus data, caller, model/profile and targeted check. Bind/admit only the scoped service; do not preemptively implement the remainder or exceed the 64-import cap. |
| SCOPE-05 | M1 outcome gaps; remaining function count unknown | Integrate original graphics, audible audio, recorded player input and round progression using applicable retained SDK/OpenCE/browser components as reached. | Three original menu/minigame/results/menu sequences and the reproducibility evidence in the milestone gate. Component demos and startup counts do not close this entry. |
| SCOPE-06 | Single reached getter returned in profile fourteen; caller global readback agrees | Original three-instruction getter reuses current timer state through typed ordinal 156 and a distinct checked existing RW cell. Source, build and focused browser evidence live in `sdk-tick-count-20261107/`. | Exact slot/raw/frame/cell owner/vacancy/permissions, metered update/two original reads and actual EAX/store pass; old-profile report and caps preserved. Fresh-zero RAM and declared clock origin/cadence remain limits; no arbitrary caller or retail timing acceptance. |
| SCOPE-07 | Verified in profile fifteen through third C logger, plain-RET parent return and checked 784-DWORD fill | Reuse retained C logger and common STOS helper for the exact reached parent setup path. | Preserve actual return/frame evidence and exclude endpoint boundary; stop at untranslated `0x4D4F0`. No gameplay acceptance follows. |
| SCOPE-08 | Prefix observed; helper returned; full body/caller continuation remain pending | Continue only the reached `0x4D4F0` generated prefix through its checked `0x4DC30` helper, preserving exact frame and stopping at `0x4BF10`. Do not admit the shared `0x4D555` continuation or infer a guest call at `0x4D54E`. | Preserve module/resource caps; retain exact helper return; stop at guarded `0x4BF10`. No full-body or parent acceptance. |
| SCOPE-09 | Guarded six-word `0x4BF10` frame observed; static source plan only | Assess its retained 167-comment-instruction SDK body and existing scoped providers (`0x3080D`, `0x30943`, logging) only when frame, original bytes/layout and ABI justify reuse. Private `goal-04bf10-20261108` contains static preparation. | Do not infer service purpose or guess pointer return. Independently verify original bytes/layout and actual frame/ABI before admission. Preserve `0x4BE40`, `0x4BEC0`, `0x4D555` and `0x488B9` as guarded. Stop at unsupported nested behavior and require a focused comparison. |

The current 48 unrepresented imports group into contiguous GPU-accessible memory (1), events/semaphores (2), file I/O (2), HAL/system (8), I/O manager (10), object manager (2), runtime library (2), synchronization (2), system/pool memory (9), threading (3), timers/DPCs (3), virtual memory (1), and data exports (3). The two scoped IRQL imports were removed from the initial HAL/system group; the clock data export was removed from the data group. These source classifications help choose reuse seams; they do not prove reachability or independent task boundaries.

## Keep the ledger useful

1. Before selecting work, consult this ledger and the current runtime stop. Prefer an existing full provider or a shared adapter improvement that serves the reached path. Use automatic extraction/preparation for generated bodies.
2. In the selected entry, record ownership, actual caller/stop, intended files, dependencies, targeted checks and stop condition. Keep proposals separate from observed failures; create a native work package only for a concrete functional dependency, not each static candidate.
3. At a coherent checkpoint, record the result, source/module/profile identity, new stop and any changed counts. Update this ledger, status and handoff together when scope or the runtime boundary changes. Mark an old inventory superseded rather than subtracting executed routines from candidate counts.
4. Recompute only affected inventories when source mappings, provider availability, imports or the selected profile change. Reuse unchanged evidence; no full graph scan or browser matrix is required for every routine. Keep simple targeted verification before M1, with correctness review where required.
5. Keep milestone acceptance in native tracking. Do not add these overlapping counts, convert them into a percentage, close M1 from them, bypass initialization with dummy returns or expand admission caps to fit a static closure.

## Evidence identities and limits

Private evidence remains in ignored `research/verification/m1-scope-audit-20261107/`: `runtime-summary.json`, `merged-scope.json`, `static/`, `providers/` and `kernel/`. This guide contains curated findings only; no generated title code, original strings/bytes, raw traces or game files are published.

- Original input SHA-256: `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.
- Module: 1,209,866 bytes; SHA-256 `058ac109bf401c50541d9ccb1add1715ced6802eaac95ad1fa9a738be33f9a11`.
- Selected settings SHA-256: `c74669f8bd0fbc347351d5f3d165510b3fd786f6f813e0a0a59dc74c277ae2ad`; observed controller snapshot mask zero.
- Static summary SHA-256: `52c43945588cb1f6f3a0e8427f65ff47988bdfbf3b949f6e782c3a2e647922b5`.
- Provider inventory SHA-256: `cf613471dc1e26403737354fbc8f45e3f8dc0525284608ed6105eaf24773eec3`.
- Kernel summary SHA-256: `7333a1daa01c278531a07851d7e44a393f0e923538d3fd3d45f7020843e488a2`.

The static scan hash-checked all 64 generated partitions. Its direct graph includes untaken branches and omits unresolved indirect/callback/data edges. Complete parsing of generated definitions does not establish complete original-image decoding; overlapping decodes and incomplete byte coverage remain. The raw runtime trace retains a 1,277-row suffix plus an omitted-prefix digest, while the distinct-routine list contains all 450 observed addresses. Later paths cannot be dynamically inventoried until initialization succeeds. The aggregate merge script is a frozen snapshot, not a generalized live counter.

Independent Sol review recomputed the headline joins/counts and checked input/module/report identities. Execution metadata showed `gpt-6-luna` medium for the static scan, `gpt-6-luna` low for each of the provider/import inventories, and `gpt-6.1-sol` high for review. This does not establish a four-Luna-Low session verification. No original menu, audible gameplay, live player-input round or M1 acceptance follows.
