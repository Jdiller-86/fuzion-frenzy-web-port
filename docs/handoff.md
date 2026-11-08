# Continue the original shared continuation

Follow the user's [resource reuse and fast M1 workflow](resource-reuse.md) and [measured scope ledger](m1-work-scope.md). M1 remains active and incomplete. Keep original execution browser-only and content/generated title artifacts local.

## Current boundary

Profile twenty reaches guarded **`0x0004D555` fallthrough**, SP **`0x03C10E60`**, after **457 distinct original routines / 837 completed kernel operations** in Chrome 156.0.8078.80. The original `0x4BF10` invocation returns actual pointer **`0x006F4030`**; actual parent continuation `0x4D539` has SP `0x03C10E48` before 24-byte caller cleanup. `0x4D4F0` and `0x48880` remain pending. There is no new CALL at `0x4D555`; serialized `returnAddress` `0x002CC1B0` is ambient saved EDI, not a guest return address. No continuation body, original entry/poll or `0x488B9` tail is admitted.

The final private module is **1,251,793 bytes**, SHA-256 `38c6b1701f702212e195163d1f4358f64a69f475d25de28ee1fa6c7230e757ef`, ABI 25, fixed 128 MiB / sole clock import / 183 exports / no implicit start. Charges: **498 bodies / 22,771 instructions / 32 groups / last occupancy twenty-one / 17 SDK maps / 63 represented imports**. Six units compiled; 70 of 76 verified objects were reused. Include/profile/shared ABI files are byte-identical. No imports or limits were added.

Three existing wait bodies and two unchanged generated helpers reuse the existing authoritative local-mutant models. Exact wait acquisition and release move registered object 3 through unowned → thread 1 → unowned. The original release helper computes EAX 1 and the surrounding original routine returns its guest pointer; post-RET frames/nonvolatiles/DF/SEH and six incoming words are checked. The node helper's EBP/ESI `0x006F4000` differs from SEH `0x03C10E58`; original `POP EBP` restores the parent before release. Other callers, negative status, `0x4BEC0` and new logging branches remain guarded. These are declared models, not retail scheduling evidence.

Final default reproduces profile nineteen's full canonical report except module identity. All 653 diagnostic words also match exactly. Selected execution adds three CPU rows/one entry/one kernel operation, 27 VM checks, 20 PFIFO charges and two clock samples. CPU totals: 10,718 rows / 5,379 entries; kernel totals: 838 events / 837 completions. Captured prefixes/digests, VM state/accounting, dirty pages and legacy ABI evidence are preserved with the qualifications in [status](status.md) and [research](research.md). Independent source/build/browser review of all three steps passed. Public baseline `afd689c` passes Ubuntu/Windows CI. No original menu, audiovisual/input round or M1 acceptance exists.

## Next bounded task

SCOPE-13: coordinator owns the private `0x4D555` continuation and existing SDK callback/guard interfaces. Reuse its retained generated block as continuation of the pending `0x4D4F0` invocation. Preserve actual enclosing frame, pointer, registers, DF/flags and the shared `0x4D582` epilogue. Do not create a guest CALL, stack return word, separate original function entry or poll. Independently verify source/layout, actual original return to the parent and old-profile preservation; stop before unadmitted `0x488B9` or earlier unsupported behavior. Other callers/services remain out of scope.

Keep all M1 gates unchecked until three original menu/minigame/results/menu sequences demonstrate graphics, audible audio, recorded player input and reviewed reproducibility evidence.
