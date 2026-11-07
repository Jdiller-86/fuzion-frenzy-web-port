# Corrected startup stops at untranslated helper

Current Chrome 156.0.8078.80 boundary: status 2 at untranslated `0x001E87E0`, return `0x0004BC50`, with 436 original routines and 675 completed kernel operations. `0x0004C380` and `0x0004BC10` remain pending; this is not an accepted allocator boundary. See [current status](status.md) for exact module identity, browser evidence and limits.

## Current checkpoint

The reproducible ABI 25 module is 1,172,509 bytes, SHA-256 `c53a8fb12fb1c0b58a9ded11604883015bb5bcb736821b44b1c0f4c26354199e`. The profile remains at 471 seeds, three checked views, 22,094 instructions, 31 exports and 57 bindings.

Stdcall cleanup is fixed for reached ordinals 159 (`wait_vblank`, +24 bytes) and 3 (`set_display_mode`, +28 bytes). Their corrected frames remove the earlier −24/−52 SP deltas at `0x001F5520` and parent `0x00202150`; paired entries/exits now have equal SP, with FS and exception head stable. This does not prove all-register or global ABI fidelity.

Display setup now validates observed register base `0xFD000000`, format `0x1E`, and the existing MMIO provider's ready/base/contains checks. It records physical framebuffer `0x03920000` and resolved guest address `0x83920000`, but performs no MMIO reads/writes or rendering. The core retains its existing bounded framebuffer range; the report pins this observed value. The prior `0x03C10CFC` RAM pointer and format 0 are rejected. See status for refusal values and null validation scope.

The corrected debug continuation retains the original saved-project body and was derived through a temporary trap-fallthrough override and automatic owner recreation from read-only Ghidra evidence; the saved project was not mutated. Seven wrapper/parent pairs have zero SP delta. The first 4,814 old raw CPU rows remain the literal prefix; subsequent old-module frame data is forensic.

Deterministic repeatable traces contain 8,448 rows/4,243 entries; live has 8,450 rows/4,244 entries and measured pause 505.06 ms for 500 ms requested. Kernel capture contains 164 suffix rows, 676 events and 675 completions. Old raw-report invariance is not claimed because corrected frames and null shapes intentionally differ. A synthetic named-mutant model, where selected, is not retail-kernel evidence.

The current helper frame starts `[0x03100000, 0xFFFFFFFF, 0, 4]`. Its first word is a requested size: `0x0004C380` entry EAX matches the static size path at `0x0002CD80`. This request has not entered the guarded helper or reached its allocation service; earlier successful ordinal-166 calls remain separate evidence. The READY callback at `0x0004CBE0` remains unexecuted. No original screen, rendering, audio, input or three-round acceptance exists.

## Next bounded task

Prioritize a bounded [native-port reuse assessment](research.md#native-recompilation-reference-and-browser-reuse) before further bespoke services. The reference matches the retained executable identity but demonstrates no browser compatibility; the current route and M1 gates remain unchanged.

The next reached runtime dependency remains `0x001E87E0`, an established 67-byte/28-instruction body ending `RET 16`. Its private static preparation is separate from runtime admission. Validate its call at `0x0004BC4B` and return at `0x0004BC50`, original bytes, complete layout, decoder agreement, preserved bodies and resource limits before replaying to the next actual guard.

Private checks report runtime 306, native 76, Python 17, all 31 seed groups for the unchanged generated profile and 17 source-only build files passing. CI remains pending; local Node 24.22.1/npm 11.19.1 differ from pins Node 24.14.0/npm 11.9.0. Component checks do not establish playability. M1 still requires a controllable original screen, audible gameplay, recorded input and three complete menu/minigame/results/menu sequences.
