# Continue the guarded wait-chain caller

Follow the user's [resource reuse and fast M1 workflow](resource-reuse.md) and [measured scope ledger](m1-work-scope.md). Reuse existing checked services and keep fixed resource/content guards. M1 remains active and incomplete.

## Current boundary

Profile seventeen reaches **status 2 at guarded `0x00030943`**, return **`0x0004BF85`**, SP **`0x03C10E2C`**, arguments **`0x0008000C, 0xFFFFFFFF`**, after **456 distinct original routines / 835 completed kernel operations** in Chrome 156.0.8078.80. Original `0x0004BF10` has entered, called the guarded wait-chain target, and remains pending; the wait body has not executed for this caller and neighboring providers remain guarded. Parent `0x00048880` remains pending. This call does not establish the overall purpose or pointer-return semantics of `0x4BF10`.

The private ABI-25 module is **1,238,999 bytes**, SHA-256 `9bcdfe76b9d8655f4d84c12710e9039671f73cd0d9b612d461d744fc88b805f1`, fixed 128 MiB / sole clock import. Charges are **496 bodies / 22,716 instructions / 32 groups / last occupancy nineteen**, fifteen mappings and 63 represented imports. The final build compiled seven units and reused 69 of 76 verified objects; shared ABI is byte-identical, with three unused C++ declarations appended only to the local prototype header. The earlier Werror parentheses build produced no module and was corrected before the successful build.

Default profile sixteen reproduces all 461 diagnostic words and the full canonical report except module identity. Selected execution changes only old indices 0 (mode) and 10 (entry count), adds one CPU row/entry and 14 VM / 12 PFIFO checks plus one clock sample. CPU totals are 10,707 rows / 5,374 entries (SHA-256 `6144890c647c06a600c4b6924ef1fb0ab7b212be4c55d85c34b8c8cd952a251b`); kernel calls/capture remain entirely exact at 836 events / 835 completions (SHA-256 `29ed1d93ff6d1a44471914ff7dea7ac521c3e90d655a6b0b7edf16e674485ef5`). VM regions, permissions, faults, memory accounting, dirty pages and ABI diagnostic count 572 are unchanged. New diagnostic words 461–492 record the original `0x4BF10` frame, callee-saved registers, DF and SEH.

Source checkpoint `83a7abe` passes Ubuntu/Windows CI. Independent source/build/browser review passed. The first Chrome launch failed on local permission before page load. A subsequent exact escalated local CLI run exited zero and is recorded only as qualified research provenance. No original menu, audio, player-input round or M1 acceptance exists.

## Next bounded task

SCOPE-10: inspect the exact `0x00030943` wait caller/frame and reuse the existing SDK wait chain plus modeled ordinal-234 kernel service only after validating object identity, handle and full caller frame. Stop at unsupported object state or services. Do not admit new `0x00030943`, `0x0003080D`, logger, `0x0004BE40`, `0x0004BEC0`, `0x0004D555` or `0x000488B9` behavior based on static presence alone. Require independent review and a focused comparison before accepting a new boundary.

Keep all M1 gates unchecked until three original menu/minigame/results/menu sequences have graphics, audible audio, recorded player input and reviewed reproducibility evidence.
