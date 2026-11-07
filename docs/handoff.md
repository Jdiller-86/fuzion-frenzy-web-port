# Startup enters the reached routine and guards its callee

After named-mutant creation, the original `0x0004C380` routine enters and reaches its first untranslated direct callee. Current stop: **status 2, `0x0004BC10`**, return `0x0004C39D`, with **435 routines / 644 completed kernel operations** in Chrome 156.0.8078.80 on Windows. The new routine has not returned. [Current status](status.md) owns exact inputs, comparisons and limits.

## Retained checkpoint

The compatible private XBE and pinned toolchain remain verified. Exact admission adds 48 instructions/156 bytes at established `0x0004C380`: 470 seeds, three checked views, 22,052 instructions, 31 exports and 57 bindings. All 472 prior actual generated bodies are unchanged; actual original bytes and new decoder boundaries agree exactly. Fixed guards/caps and inherited FSTCW versus WAIT/FNSTCW normalization remain unchanged. Independent review covers flags, partial register, unsigned shift, checked address arithmetic/access widths and both return paths. Untranslated callees remain guarded.

Identical rebuilds produce the 1,165,948-byte ABI 25 module, SHA-256 prefix `2b423745`, with only the existing host clock import. The `16ef10c3` named-mutant checkpoint remains the immediate comparison baseline. No public runtime/emitter change is made by this private title-code admission.

The alternative default-off `synthetic-win32-mutants` profile requires the explicit named-event peer and replaces the legacy unnamed-only pool. Anonymous and named mutants share four total handles/objects. Anonymous object 1/handle `0x80004` and named object 2/handle `0x80008` remain unowned. Counted-name/namespace identity, case-folded open-if, typed collisions in both creation paths, complete checked ranges and atomic refusal are preserved. No waits, acquisition, release, close, native mutex or full kernel ABI is supplied. Fresh named creation is browser-observed; duplicate/collision cases are synthetic fixtures.

The new mutant profile intentionally uses separate `localMutantState` metadata and richer anonymous-call output; common handle/EAX/ESP fields remain exact. Its retained default-off deterministic report matches the prior checkpoint except module identity. This subsequent routine admission preserves every one of the immediate baseline's 7,932 raw CPU events, its entry prefix, all 133 captured call objects/outputs and the entire kernel-capture object.

The event, both thread contexts, modeled memory accounting, storage and debug state remain unchanged. Context 2 is READY with handle `0x104`; callback `0x0004CBE0` has not executed. Prior read-only derivation provenance and page-role reclassification remain retained. No scheduling expansion or raised resource cap follows from this service change.

Deterministic reports repeat byte-for-byte with identical rebuilds: 7,933 traces / 3,988 entries. Live-monotonic evidence reaches the same stop with 7,929 traces / 3,986 entries and a measured 505.7149 ms pause for the declared 500 ms request. Synthetic fixed clock/frequency inputs and live/control comparison limits remain explicit. Two paired-clock query outputs vary in the live run; every captured argument/status frame and non-clock call object matches.

Capture remains 133 suffix rows / 645 events / 644 completions, with the same first-512 digest. The full digest remains runtime-reported where legacy argument tails are omitted. One extra clock sample, checked-access counters and bounded watch/pop snapshots are pinned as transition fields. PFIFO work/output, VM regions/fault, modeled states and ABI aggregate 454 remain unchanged. Legacy ESI labels count stack inequality (452); unused zeros do not establish absence of ESP faults.

Independent Sol High static/evidence review passes; Luna Medium inspected/prepared the bounded entry. Public runtime/emitter source is unchanged in this batch; retained runtime 293/293, native 74/74 including sanitizers and Python evidence 14/14 passes still apply to those source-owned components. All 31 seed groups verify. Original code runs only in Chrome. CI remains pending; local Node/npm differ from its pins.

## Next task and acceptance

Validate and admit only the reached `0x0004BC10`, whose read-only retained export contains 40 instructions/112 bytes. Check the original call at `0x0004C398`, frame/argument, complete layout, memory/CPU/ABI, retained bodies and fixed budgets, then replay to the next actual guard. Keep unsupported callees guarded until reached and keep control/inert/experiment evidence plus independent correctness review. See [analysis setup](analysis.md), [research provenance](research.md), [development](development.md) and [agent defaults](agents.md).

M1 remains unaccepted. A controllable original screen, audible original gameplay, recorded input and three complete menu/minigame/results/menu sequences still require evidence. Keep original inputs/generated code local and preserve the prototype as research.
