# Continue the reached generated prefix

Follow the user's [resource reuse and fast M1 workflow](resource-reuse.md) and [measured scope ledger](m1-work-scope.md). Preserve local-content policy, fixed resources and checked runtime interfaces. M1 remains active and incomplete.

## Current boundary

Profile sixteen reaches **guarded `0x0004BF10`**, return **`0x0004D539`**, SP **`0x03C10E44`**, with frame words `[0x50, 0x0025BFA8, 0x5B, 0x2001, 0x0025BFCC, 0]`, after **455 original routines / 835 completed kernel operations**. Original `0x0004D4F0` has entered but remains pending; `0x0004DC30` returns actual EAX `0x582664ED`. `0x0004BF10` itself has not executed. No shared `0x0004D555` continuation or `0x000488B9` tail is admitted; parent `0x00048880` remains pending. Do not infer the guarded routine's service purpose.

The ABI-25 module is **1,233,429 bytes**, SHA-256 `8cd4fce25113ca90dfaceb95f18fb3c2bb3de85790640931d10f26ca3f8e2201`, fixed 128 MiB / sole clock import. Charges: **495 bodies / 22,549 instructions / 32 groups / last occupancy eighteen**, fourteen mappings and 63 imports. Ten units compiled and 63 of 73 objects were reused after verification. Shared ABI is byte-identical; three unused C++ function declarations were appended to the local prototype header. The admitted `0x4D4F0` prefix contains 37 comment rows in two spans; its separate `0x4DC30` helper has 21 instructions. The full envelope is 154 bytes / 55 unique instructions, including 18 unadmitted continuation rows. At `0x4D54E`, `MOV` falls through into `0x4D555`; it is not a guest call.

The default reproduces profile fifteen's full canonical report except module identity, with all 413 diagnostic words exact. Selected execution changes only earlier indices 0 (mode) and 10 (entry count). Selected execution retains the prior captured CPU prefix/digest and entire kernel calls/capture, adding three CPU rows/two entries. Current totals: 10,706 rows / 5,373 entries (SHA-256 `aa9d08676e554745040c6beef6462f7a829bf8a578a40af0bc59d8e945e79a5e`) and 836 kernel events / 835 completions (SHA-256 `29ed1d93ff6d1a44471914ff7dea7ac521c3e90d655a6b0b7edf16e674485ef5`). Changes are 46 VM checks, 40 PFIFO charges and two clock samples; regions, permissions, faults, memory accounting, dirty pages and legacy ABI count remain exact. Four pop-ring entries are read telemetry, not guest POPs; the +12 ESI site-watch is a coarse outer tag, not guest PC evidence. Diagnostic words 413–460 capture exact frames, nonvolatiles, DF and SEH state.

Affected scope joins are unchanged: 48 unrepresented imports (45 functions / 3 data); prior closures remain 188/126 candidates with 102/100 providers. M1 subset is unknown. Source checkpoint `7d5a3fd` passes Ubuntu/Windows CI. Independent source/build/browser review passes, with exact evidence identities retained. No original menu, audiovisual gameplay, player-input round or M1 acceptance exists.

## Next bounded task

SCOPE-09: assess the six-word `0x0004BF10` frame using its retained 167-comment-instruction SDK body and existing scoped providers such as `0x0003080D`, `0x00030943` and the logging provider. Establish original bytes/layout, actual frame and ABI before admission; do not guess a pointer return or assign service purpose from the frame alone. The scoped static source plan is in `goal-04bf10-20261108`. Stop at unsupported nested behavior; preserve `0x0004BE40`, `0x0004BEC0`, `0x0004D555` and `0x000488B9` as guarded/unadmitted. Require independent review and a focused comparison before accepting a new runtime boundary.

Keep all M1 gates unchecked until three original menu/minigame/results/menu sequences have graphics, audible audio, recorded input and reviewed reproducibility evidence.
