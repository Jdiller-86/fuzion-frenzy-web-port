# Original copy-helper and mixed-layout checkpoint

Later progress is recorded in [the checked-memory checkpoint](MEMORY_STARTUP_CHECKPOINT.md). Allocation and lock initialization now advance original startup to `0x00031da0`; this document retains the earlier helper proof and limits.

November 1, 2026: original helper `0x001df9c0` now executes and returns in the browser. Both explicit synthetic settings profiles pass 22 startup checks and 8,410 owned buffer/ABI cases each. Startup advances to unsupported kernel ordinal 184, `NtAllocateVirtualMemory`, return `0x00032530`. **No playable original screen, complete runtime, full fidelity or planned online/social feature acceptance is established.** Static recompilation and the complete product objective remain selected.

This extends the [timer startup proof](TIMER_STARTUP_CHECKPOINT.md). Original files, generated title C/Wasm, raw analysis and screenshots stay ignored/local. Original instructions execute only in a browser worker; developer translation/compilation is not the required browser-local end-user conversion pipeline. No firmware, extraction, global settings change or vendor-source edit was needed.

## Established layout and translation

Read-only reuse of the existing Ghidra project produced `research/verification/recomp-copy-layout-01/seed.json`. The entire 821-byte envelope is retained:

| Classification | Observed coverage | Admission boundary |
| --- | --- | --- |
| Established instructions | 231 instructions / 664 bytes, in the existing 21 body ranges | Original bytes and an independent decoder agree on every boundary |
| Defined pointer data | 120 bytes / 30 pointers in six contiguous tables | Each value targets a checked instruction start inside this helper |
| Undefined alignment spans | 37 bytes in twenty spans | Only known no-effect NOP/self-move/self-LEA encodings are admitted; no edge enters them |
| Computed jumps | Sixteen sites using seven operand anchors | Each indexed, four-byte operand matches exactly one complete declared table and its established target set |

Some anchors are biased relative to the table entries. The checker preserves the actual loaded original pointer, rather than inventing an entry or rewriting the table. Unsupported/ambiguous layouts, non-code flow, mismatched bytes, omitted jumps, unaligned anchors and overlapping/incomplete envelopes are rejected. The existing undefined-gap decoder is not used to force data into instructions.

The pinned clean translator's sparse-CFG interface consumes the validated instruction set and authoritative table census. Seven preserved seeds now compose **17 functions / 977 instructions**, covering 3,123 envelope bytes and 2,928 declared body bytes. Counts are translation coverage, not executed-instruction coverage. All original envelope bytes are accounted for; there is no guessed native-copy replacement. Final translation `recomp-title-copy-04` is byte-identical to build input `recomp-title-copy-03`, including its report and generated includes.

## Browser observations and diagnostic correction

Fourteen distinct original routines enter, with 25 trace records. The original helper returns during initializer `0x000322ef`; nine completed boot-path kernel operations remain unchanged. One real six-hour timer stays pending and no original deferred callback runs. Both synthetic inputs retain their respective callback state words, without establishing original-console settings.

The diagnostic invokes the actual translated helper on owned scratch buffers. It checks the entire 8 KiB buffer against an independent initial-pattern snapshot, 32-byte guards, cdecl return-stack cleanup, returned destination pointer, preserved nonvolatile registers, cleared direction flag and absence of kernel calls. Sizes include zero, all 0..128 short lengths, selected larger boundaries through 4,096 bytes, all short source/destination alignments, both overlap directions, same pointers and buffer edges. Six invalid range/entry cases must reject before execution. The 8,410 cases comprise 4,128 short disjoint, 1,920 per overlap direction, 120 same-pointer, 256 large disjoint, 60 edge and six rejected-input cases. Some direction-family cases are disjoint when the length is shorter than the displacement; this is bounded sampling, not exhaustive memory-function equivalence.

The first diagnostic failed its EBP check because it read `g_ebp`, which the pinned translator uses as a last-established-frame bridge. Its architectural EBP is function-local. An owned observation hook now reads that local value at existing exit-trace sites, after the original epilogue, without replacing instructions or altering stack/register behavior. All eight helper return sites are instrumented; the local restored EBP and other nonvolatile registers pass. Full CPU state is restored after each synthetic case or explicit fault, and these cases do not advance the boot clock or overwrite its trace/call counts. Original symbol recovery remains unknown; the observed copying includes overlap-safe cases, rather than establishing an original library name.

The earlier build/report and failed translation are retained. Initial records `01/02` preserve the changed boundary and incorrect diagnostic; `03/04` record the corrected proof before allocation-input reporting. The first screenshot `10.jpg` captured an older painted status; settled screenshot `11.jpg` visibly shows the passing result. Original runtime behavior is supported by the structured browser results, not the stale image.

The allocation stop exposes five arguments: base-address pointer `62983944` currently holding zero; zero-bits value zero; region-size pointer `62983988` currently holding `1048576`; allocation type `0x2000`; protection `0x04`. The adjacent sixth stack word is not an argument. The public [pinned nxdk declaration and constants](https://github.com/XboxDev/nxdk/blob/f8c52e4be65bab7d6e10455db8d86d69d232bbe3/lib/xboxkrnl/xboxkrnl.h#L3095) corroborate the five-argument contract, reserve flag and read/write protection. They do not prove matching-kernel allocation behavior. The synthetic kernel address is resolved through the actual binding index, not derived from the ordinal.

## Evidence and validation

| Local-only artifact | SHA-256 |
| --- | --- |
| `research/verification/recomp-copy-layout-01/seed.json` | `705491db633f5398b9004d359f44dabb6b3338b3e16981cd40d8b5ce7cd30541` |
| `research/verification/recomp-title-copy-04/generation.json` | `224b4ca0aff82d075c9699c967258dbebb27cda16d38a97fb2a5cc1b49df9379` |
| `research/verification/recomp-copy-build-02/build.json` | `4befb6b59d7c26d6db9db5982837370ad7099eb667794fd3728ba241f0d7a0de` |
| Matching 71,973-byte `startup.wasm` | `165df54e0c83c795b74f836ce04c67c6d6713ccbb5056d40e5ea3bb8e3b2b1f1` |
| `research/verification/copy-startup-browser-20261101-05.json` (synthetic 0) | `0bfb3e559b2a7eb0aefa7f3dbef5227bd1d7a5150f1c65b602c849190083b90a` |
| Matching `06.json` (synthetic 1) | `122ed78b1ac2724790d4e0805552b3d3e106459c63eb7e2f0333bfc3ff9cd204` |
| Matching `07.json` (20-check regression) | `a6e61fe94c8bb1d0b944d6816507398e91e0c23aaff0cf69d92c48d3ac4f2fb1` |
| Matching `08.json` (16-check no-import regression) | `a26c7425561b765bba79d4e01f3685cb122f356fbfedcbb81c0775ec84f2cb7d` |
| Matching `09.json` (rejection/Clear) | `5c57223a0fa5496d991557ac5ca9e67c5de30b41427aae839da4d50d102dbe09` |
| Matching `10.json` (recovery and empty warning/error log) | `80cec4b095bb2d536f8aca8d2733af4a760fb2ef6da301d552aced35b8a19bfd` |
| Matching settled `11.jpg` | `bc0f3cef09d7c6ede6839c24a4c026cf57cdca9df5f88b0974ef969a27e18c4d` |

`copy-checkpoint-audit-01.json` verifies all thirteen compiled owned source/header/declaration identities, final translation identity, original executable digest and the clean vendor pin. Fourteen Python envelope/layout/composition/observation tests and twelve owned runtime/admission/delivery tests pass. The module retains fixed unshared 128 MiB memory / 64 MiB guest RAM, ABI 4's sole bounded clock import and no implicit start. CI uses no original game; publication results are recorded separately. Clear empties all three selected inputs and output; the selected proof tab was closed and verified temporary source host stopped.

## Next task: real virtual-memory semantics

Reuse initializer `0x000322ef` and its observed ordinal-184 request. Establish the relevant reserve/commit/release/protection and failure contracts from pinned primary declarations/implementations before connecting allocation. The current flat guest buffer does not enforce reserved versus committed pages: returning a pointer into it is not evidence of a compatible memory service. Preserve image, TLS/boot stacks, timer and diagnostic ownership; prevent overlaps, overflow and invalid output pointers. Add meaningful owned page/range/error scenarios, then execute the actual initializer in the browser to the next genuine dependency. Report unsupported variants explicitly, rather than returning unconditional success.

The complete game, renderer/audio/input integration, general scheduling/exceptions/clock/suspend fidelity, original settings, browser-local end-user conversion, all online/social/control requirements and server authority remain open in the [full product ledger](ORIGINAL_EXECUTION_PROGRESS.md). This checkpoint does not reduce any requirement or authorize a route change.
