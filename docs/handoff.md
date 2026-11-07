# Startup creates a second guest-thread context

Admitting the exact published start `0x0004CBE0` lets the existing thread service create context 2 and the original client return handle `0x104`. The new context remains READY; its callback has not executed. Startup next stops with status 9/`0xC00000BB` at ordinal 192 (`NtCreateMutant`), return `0x000307E3`, with 434 routines and 643 completed kernel operations in Chrome 156.0.8078.80 on Windows. The unnamed-only mutant model refuses the newly reached named request. [Current status](status.md) owns exact inputs, comparisons and limits.

## Retained checkpoint

The compatible private XBE and pinned toolchain remain verified. The profile has 469 seeds, three checked direct views, 22,004 instructions and 31 exports. Identical rebuilds produce the 1,161,612-byte ABI 25 module, SHA-256 prefix `8650657e`. All 57 bindings, prior bodies, timestamp guards and resource caps remain unchanged, including 64 MiB guest RAM inside fixed 128 MiB Wasm memory and four guest-thread contexts.

The retained Ghidra project had no function at or containing the callback address. Existing decoded code, the caller's exact pointer publication, a non-fallthrough predecessor return, closed flow/return and following padding support an 11-instruction/43-byte entry. Ghidra automatically derived that exact function in a read-only session. Seed/provenance distinguish newly derived analysis from retained metadata; actual headless output and a reopened project verify no saved definition. New boundaries agree exactly with the independent decoder. The inherited false aggregate boundary flag reflects one unchanged FSTCW versus WAIT/FNSTCW normalization. No callback execution follows from admission, and its nested callee remains guarded.

Context 1 is unchanged. Context 2 is READY with open handle `0x104`, object/FS `0x03C11000`/`0x03C11200`, a 64 KiB stack at `0x03C12000`, TLS size 20, zero context and system routine `0x00034307`. The 17-page arena footprint stays inside the prior reservation. Reclassifying 16 already committed pages changes pool 1,008 → 992 and stack 16 → 32; VM regions/free/reserve/commit are unchanged. No scheduling or native thread was added.

The next named-mutant request retains Win32 root -4, open-if `0x80`, a 13-byte counted name (maximum 14) and full-word zero initial ownership. The original name stays private. Existing mutant count/handle remain one/`0x00080004`; only the last diagnostic status changes to the explicit unsupported result. No named handle or successful service return is supplied. The prior named event remains object 1/handle `0x00090004`; event, disk/FATX, debug and aggregate ABI state otherwise remain unchanged.

All 7,923 prior raw CPU events and 130 prior call objects/outputs compare exactly. The selected thread request completes, then one string initializer completes and the named mutant refuses. Final deterministic reports repeat byte-for-byte using identical rebuilds: 7,927 traces and 3,986 entries. A separate live-monotonic run reaches the same boundary with 7,923 traces/3,984 entries and a measured 501.6899 ms pause for the declared 500 ms request. Synthetic fixed clock/frequency inputs and live variation remain explicit.

Capture contains 133 suffix rows, 645 events and 643 completions, retaining the first-512 digest. Missing legacy argument tails keep the full digest runtime-reported. Aggregate ABI attribution remains unresolved; legacy ESI labels count stack inequality and unused zeros do not prove absence of ESP faults.

Independent Sol High review passes static/provenance and the explicit runtime transition. Actual metadata confirms Luna Medium inspection and Luna Low fixture registration. Runtime checks pass 279/279, native runs 72/72 including sanitizers, Python evidence suites 13/13 and all 31 seed exports verify. Thread-bootstrap and memory-statistics fixtures now run in the standard native suite. Preparation/documentation and the 17-file source-only build pass. CI is pending; local Node/npm differ from its pins.

## Next task and acceptance

Implement and independently review explicitly selected, bounded named-mutant creation/open-if for the reached request. Preserve counted-name/object-type identity in the Win32 namespace, checked output/attributes, zero initial ownership and current unnamed handles. Retain unsupported ownership/wait/release/close guards and control/inert/experiment comparisons. Keep the READY callback on its admitted scheduler path. See [analysis setup](analysis.md), [research provenance](research.md), [development](development.md) and [agent defaults](agents.md).

M1 remains unaccepted. A controllable original screen, audible original gameplay, recorded input and three complete menu/minigame/results/menu sequences still require evidence. Keep original inputs/generated code local and preserve the prototype as research.
