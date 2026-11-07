# Startup stops at untranslated helper after corrected debug return

The current original-startup boundary is status 2 at untranslated helper `0x001E87E0`, return `0x0004BC50`, with 436 routines and 675 completed kernel operations in Chrome 156.0.8078.80 on Windows. `0x0004C380` and `0x0004BC10` remain pending at this guard; the helper has not returned and this is not an accepted allocator boundary. Exact module/profile, trace, frame, and evidence limits are in [current status](status.md).

## Current checkpoint

The ABI 25 module is 1,172,460 bytes, SHA-256 `e4fbed63a5e2822626ab65fabedd824941a051d53a6f62982d2570cc807fe15e`; its profile has 471 seeds, three checked views, 22,094 instructions, 31 exports and 57 bindings. Admission of `0x0004BC10` preserved all 473 prior actual bodies. The subsequent debug correction changes only its wrapper and preserves the other 473 bodies in that 474-body module. The admitted helper remains pending at the next guard.

The debug owner `0x00031227` retains its original 46-byte/20-instruction body. A temporary trap-fallthrough override and automatic owner recreation from read-only Ghidra evidence derived 50 bytes/22 instructions, ending in `LEAVE` at `0x00031255` and `RET 4` at `0x00031256`; no saved-project mutation was made. Preparation fails closed when the returning `INT 2D` or eligible adjacent `INT3` control-flow path falls off the function layout.

Seven debug and seven parent pairs complete with zero SP delta, with FS and exception head stable. This is local evidence only, not global frame fidelity. Reports from old modules after the first debug trap remain forensic because the original return was missing; preserve only their first 5,187 raw CPU events as a literal old prefix. A separately confirmed earlier anomaly at `0x001F5520` changes SP by −24 over rows 4,813→4,814; its `0x00202150` parent changes SP by −52 over rows 4,788→4,815. It remains unresolved.

Trace retention remains capped at 8,192 rows by default. Selecting start 8,192 extends the horizon to 16,384; legacy-off behavior is unchanged. The six-word SHA prefix and global counts remain available; suffix initializer progress is null, versus 83/83 in paired full-window reports. Deterministic evidence has 8,448 rows/4,243 entries and repeats byte-identically across repeat and rebuild. Live evidence has 8,452 rows/4,245 entries and measured pause 502.875 ms for the requested 500 ms. The complete live digest is runtime-reported because its omitted prefix is not retained in that same run.

The optional named-mutant profile is a bounded synthetic model, not evidence of retail kernel semantics. The current stop frame begins with `[0x60030310, 0xFFFFFFFF, 0, 4]`; this helper argument array is not an executed kernel call. Address validity and allocator semantics are unknown. The READY callback `0x0004CBE0` is unexecuted. No graphics, audio, input, or three-round M1 acceptance exists.

## Next bounded task

Fix and verify stdcall frame cleanup for reached `159_wait_vblank` and `3_set_display_mode`, whose current source has no SP cleanup. Establish actual argument provenance before any admission of allocator `0x001E87E0`. Do not claim `0x60030310` is valid or infer allocator semantics. Keep the upstream frame anomaly explicit and stop at unsupported services/registers.

Retained private checks report runtime 303, native 76, Python evidence 16, and all 31 seed groups passing. CI is pending; local Node 24.22.1/npm 11.19.1 differ from CI pins Node 24.14.0/npm 11.9.0. Component checks do not establish original-game playability. M1 still requires a controllable original screen, audible gameplay, recorded input, and three complete menu/minigame/results/menu sequences.
