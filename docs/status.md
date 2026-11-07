# Project status: startup stops at untranslated helper after corrected debug return

Observed November 7, 2026 in Chrome 156.0.8078.80 on Windows. The bounded original-startup replay now returns through the corrected debug wrapper and stops at the untranslated helper `0x001E87E0`: **status 2**, return address `0x0004BC50`, with **436 routines and 675 completed kernel operations**. `0x0004C380` and `0x0004BC10` remain pending at that guard; the helper has not returned. This is not an accepted allocator boundary. M1 remains unaccepted. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

## Admitted profile and debug evidence

The local 2,834,432-byte XBE matches [the reference identity](reference.md), SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The current ABI 25 module is 1,172,460 bytes, SHA-256 `e4fbed63a5e2822626ab65fabedd824941a051d53a6f62982d2570cc807fe15e`. The profile contains 471 seeds, three checked views, 22,094 instructions, 31 exports and 57 bindings. Admission of `0x0004BC10` added 40 instructions/112 bytes while preserving 473 prior actual bodies. The subsequent debug correction changes only its wrapper and preserves the other 473 bodies in that 474-body module. The admitted helper remains pending at the next guard. Original input, generated code, modules and raw reports remain private and local.

At debug owner `0x00031227`, the retained original body remains 46 bytes/20 instructions. A temporary trap-fallthrough override and automatic owner recreation from read-only Ghidra evidence derived a 50-byte/22-instruction continuation, adding `LEAVE` at `0x00031255` and `RET 4` at `0x00031256`; no saved-project mutation was made. Preparation fails closed if the returning `INT 2D` or eligible adjacent `INT3` control-flow path falls off its layout.

The corrected wrapper completed seven debug pairs and seven parent pairs, each with zero SP delta; FS and exception head remained stable. The helper's final restore, returning parent paths beyond this point and global frame fidelity are not established. All reports after the first debug trap in the old modules are forensic because those modules omitted the original return. Preserve the first 5,187 raw CPU events as the old literal prefix; do not treat later old frames or pointers as acceptance evidence.

## Trace window and comparisons

The default trace retention remains 8,192 rows. An explicitly selected start at row 8,192 extends the observation horizon to 16,384; the legacy-off path remains unchanged. Reports preserve a six-word SHA prefix and global counts. The suffix initializer count is unavailable (`null`) because the initializer lies outside the suffix; paired full-window reports show 83/83. Deterministic reports capture 8,448 rows (8,192 prefix + 256 suffix), 4,243 entries and repeat byte-identically across repeat and rebuild. Live evidence captures 8,452 rows and 4,245 entries; the requested 500 ms pause measured 502.875 ms. The full live trace digest is runtime-reported because its omitted prefix is not retained in that run.

An optional named-mutant model remains explicitly synthetic and default-off. It models the observed anonymous and named creation path only; it does not establish retail kernel semantics. The earlier comparison retained 7,932 raw CPU events, 133 captured call objects and the full kernel-capture object, but this is not a claim of complete CPU or kernel fidelity. Read [research provenance](research.md) for evidence identities and limits.

The current stop frame's first four arguments are `[0x60030310, 0xFFFFFFFF, 0, 4]`; these were observed at the guarded call and have not been executed as a kernel call. Their meaning and the validity of `0x60030310` are unaccepted. The READY callback at `0x0004CBE0` remains unexecuted. No graphics, audio, input, or three-round M1 acceptance is established.

## Separate upstream frame anomaly and next task

An independently confirmed pre-debug anomaly remains unresolved: at `0x001F5520`, consecutive trace rows 4,813→4,814 show delta SP −24; parent `0x202150` rows 4,788→4,815 show delta SP −52. This precedes the debug-return repair and blocks any global stack-frame fidelity claim.

Next, fix and verify stdcall frame cleanup for reached `159_wait_vblank` and `3_set_display_mode`; the source currently has no SP cleanup. Establish actual argument provenance before admitting `0x001E87E0`. Do not infer `0x60030310` validity or allocator semantics. Keep work bounded to reached paths and stop at unsupported services or registers.

The private audit reports runtime 303, native 76 and Python evidence suites 16 passing, with all 31 seed groups. These are component/preparation checks, not original-game playability. Local Node 24.22.1/npm 11.19.1 differ from CI pins Node 24.14.0/npm 11.9.0; CI remains pending. No controllable original screen, audible gameplay, recorded input, or three consecutive menu/minigame/results/menu sequences has been accepted.
