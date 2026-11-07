# M1 measured work scope

Use this ledger to select bounded development tasks and reuse existing implementations. It records measured coverage, source candidates and unresolved work separately. It is not a count of manual fixes, an M1 percentage or a delivery estimate. [Status](status.md) owns the current runtime frontier; [milestones](milestones.md#m1--original-browser-execution) owns acceptance.

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

These entries are unassigned planning records. The current task is documentation/tracking only. When implementation resumes, claim one bounded task, name its owned files and checks, and update its state here. Static candidates become observed dependencies only when a valid runtime path reaches them.

| ID | Evidence and state | Functional objective / scope | Acceptance and stop condition |
| --- | --- | --- | --- |
| SCOPE-01 | Observed stop; generated slice prepared offline, not admitted | Advance `0x00049990` using the existing private SDK slice/profile and common checked runtime. Review its reached 36-DWORD zero fill rather than invent another per-function memory implementation. | Original-byte/layout and caller-frame agreement; targeted helper check and independent review for changed CPU/memory/ABI; one focused browser comparison. Preserve older profiles and caps; stop at the next unsupported behavior. |
| SCOPE-02 | Static reuse opportunity; not a newly observed stop | Reuse retained C `0x0004B230` through a checked SDK-to-C call interface if reached. Its 108-instruction full body already exists, but the SDK currently links a terminal guard. Scope the bridge before duplicating this code. | Establish actual argument frame, inherited SEH/frame state, registers, flags/FP requirements and reentry policy. Two direct callees, `0x0004C5F0` and `0x0004C670`, remain C stop stubs. Preserve their stops; source presence alone cannot retire the guard. |
| SCOPE-03 | Static device-call candidates under SCOPE-01 | Assess generated `XGetDevices` / `XInputOpen` interfaces against the matching SDK and existing browser controller components. Reuse the current host policy only where its declared contract applies. | Admit only reached calls with original frame/data ownership evidence. Keep empty snapshot, connected input and live polling distinct. Stop at unsupported device/service behavior. |
| SCOPE-04 | Static inventory: 51 unrepresented imports; M1-required subset unknown | Select a reached missing service, group related work at existing memory/thread/file/dispatch interfaces, and consult pinned native implementations before writing a new handler. | Record slot/ordinal, function versus data, caller, model/profile and targeted check. Bind/admit only the scoped service; do not preemptively implement all 51 or exceed the 64-import cap. |
| SCOPE-05 | M1 outcome gaps; remaining function count unknown | Integrate original graphics, audible audio, recorded player input and round progression using applicable retained SDK/OpenCE/browser components as reached. | Three original menu/minigame/results/menu sequences and the reproducibility evidence in the milestone gate. Component demos and startup counts do not close this entry. |

The unrepresented imports group into contiguous GPU-accessible memory (1), events/semaphores (2), file I/O (2), HAL/system (10), I/O manager (10), object manager (2), runtime library (2), synchronization (2), system/pool memory (9), threading (3), timers/DPCs (3), virtual memory (1), and data exports (4). These source classifications help choose reuse seams; they do not prove reachability or independent task boundaries.

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
