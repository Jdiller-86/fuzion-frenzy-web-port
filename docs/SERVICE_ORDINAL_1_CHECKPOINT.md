# Service ordinal 1 is AvGetSavedDataAddress

November 5, 2026. Sections 1-5 are the read-only characterization of the new frontier after the unknown-AV-option model; sections 6-7 record the architect-authorized default-off model, its guard correction, the pair and the next genuine stop. No translation admission, export, seed or hook change beyond the default-off probe input. Every experimental path remains default-off.

## 1. Service identity

| Property | Value |
| --- | --- |
| Ordinal | **1** |
| Name | **`AvGetSavedDataAddress`** |
| Arguments | none |
| Return | ULONG in EAX (the saved-data address) |
| Pinned implementation | `kernel_hal.c:1445` — returns the static `g_av_saved_data_address` |
| Setter | `AvSetSavedDataAddress` (ordinal 4, 1 arg) — not called on this path |

The pinned HAL initializes `g_av_saved_data_address` to **0** and only `AvSetSavedDataAddress` changes it; no such call appears in the retained run, so the documented return is 0.

## 2. Observed call and caller

- Call site: `D3DDevice_Present` (`0x00202150..0x002022D0`) at `0x00202284`, immediately after the second `AvSendTVEncoderOption` call; return `0x0020228A`.
- The result is moved to `EDI` and tested. If nonzero, Present enters the saved-data path (calls through `0x00253454`, `0x00253360`, `0x0025336C`, `0x0025345C`); if zero, it skips to `0x002022B0`.
- Stop: status 1 at `0xFE000098`, ordinal 1, `modeled: false`, one recorded argument (`0x20228A`-adjacent stack value), 340 routines / 408 kernel ops.

## 3. Service decision (for the architect)

The pinned return is a static 0 with no side effects and no arguments. A bounded model would return that documented value in EAX, exact no-argument shape, default-off, with native tests and a pair run; the zero branch skips the saved-data block and reports the next genuine stop. The alternative is to hold.

No modeled success beyond the pinned static return; the saved-data path (nonzero) remains unmodeled and would be a fresh decision.

## 4. Process note

The published integration head `c8c965c` (source `5d7ebf5`) does not contain the unknown-option model `3d4cf7d`, and it is not an ancestor of the lane head; a merge is denied in this session and no fast-forward is possible. The binding "sync before any DECISIONS row" rule therefore blocks a row this cycle; this characterization is recorded in the checkpoint and status only, and the row can be added after the architect integrates `3d4cf7d`.

## 5. Limits

Static references plus the retained synthetic runs; the pinned HAL is the retained replacement, not an original-console observation. Only the zero branch is validated. No display, encoder or playability claim.

## 6. Bounded fixed-0 model, guard correction and pair

The architect authorized a default-off fixed-0 model with a machine-checked no-setter guard. The first guard implementation proved **unsatisfiable** and was corrected by the architect:

- The retained title profile `research/verification/recomp-title-recomp-title-post-memory-83/title-profile.h` binds `{0x0025345Cu, 4u}`; `0x25345C` is one of the four calls in `D3DDevice_Present`'s **nonzero** saved-data branch (skipped when the getter returns 0). The guest image therefore legitimately imports and references `AvSetSavedDataAddress`, so neither an import-table check nor a static call-site scan can pass. The first implementation refused with `FF_BOOT_INVALID` before the run — a real blocker, not a model failure.
- The architect then authorized **option (a): a runtime no-prior-setter guard** — accept only when the recorded dispatch journal contains no ordinal-4 call before the read; the value is fixed at read time, so a later setter cannot invalidate it. The setter itself stays unmodeled.

Implemented (`src/runtime/av_saved_data.c/.h`):

| Function | Contract |
| --- | --- |
| `ff_av_saved_data_prior_setter(ordinals, count)` | `1` if any prior ordinal is 4; fails closed on an unreadable journal |
| `ff_av_saved_data_value(declared, prior_setter, out)` | declared and no prior setter ⇒ write 0; undeclared or prior setter ⇒ refusal with no write |

Harness/tooling: default-off `avSavedData` setter (no static scan), an ordinal-1 handler that copies the recorded journal ordinals, evaluates the guard before recording the read, returns 0 with no arguments or stack change, and reports diagnostics including `priorSetter`; parser input, `avSavedDataState`, validator pairing and pinned acceptance (value 0, EAX = 0, `esp` unchanged, `priorSetter` 0). Native tests cover the clean journal, a prior setter, an unreadable journal, the undeclared refusal, the no-write-on-refusal rule and a repeated read after a recorded setter.

Module `recomp-post-memory-build-120`: **720,149 bytes**, SHA-256 `447c807370f7237994368b4d105ef2bb84906172963f4c5d3407e282c04baa88`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert (display profile, no saved-data input) | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| saved-data input | **status 1 at `0xFE000094`, ordinal 2** | **340** | **409** |

The ordinal-1 read was accepted: `priorSetter 0`, `value 0`, EAX `1 -> 0`, `esp 62983668 -> 62983668` (unchanged). This validates only the zero branch (`0x202284` → `0x20228A`, EDI=0); the nonzero saved-data path remains unmodeled and unclaimed.

## 7. Next genuine stop: a third ordinal-2 call with a NULL Result

After the zero branch the run stops at ordinal 2 (`AvSendTVEncoderOption`), status 1 at `0xFE000094`, 340 routines / 409 kernel operations. Read-only characterization of the observed call:

- Return address `0x2022BD` in `D3DDevice_Present`; stack `[0x03920000, 9, 0, 0]` — handle `0x03920000` (AV encoder register base), option `9` = **`AV_OPTION_QUERY_AV_CAPABILITIES`** (documented, `kernel.h:1152`), param 0, **Result = NULL**.
- The pinned HAL (`kernel_hal.c:1458`) begins with `if (!Result) return;`, so this shape is a **log-and-return with no write, no failure, EAX untouched and the stdcall frame popped by 20**. That is reference-grounded, but the shape has not been modeled; it is the next bounded decision. The existing default-off ordinal-2 router already refuses it honestly.

No DECISIONS row: the published integration head `c8c965c` predates `3d4cf7d` and is not an ancestor; merge is denied here.
## 8. Bounded null-Result model (authorized) and the next stop

The architect authorized one default-off sibling model for the observed third ordinal-2 call with binding amendments: exact tuple only (a nonzero Result reaches the option-9 query path and must be refused), a validator no-write assertion (guest-window checksum equality **and** no new write-map mark), and a measured 20-byte pop.

| Path | Purpose |
| --- | --- |
| `src/runtime/av_null_result.c/.h` | Pure decision: declared + exact tuple (`0x03920000`, option 9, param 0, Result 0) ⇒ accept; nonzero Result ⇒ separate refusal; every other handle/option/param ⇒ refusal |
| `tests/av_null_result_test.c` | Exact tuple, wrong handle/option/param, nonzero Result, undeclared refusal |
| `tools/fixtures/thread_startup_probe.c` | Default-off `avNullResult` setter; a third exact router branch that performs no write, leaves EAX, pops 20 bytes, and records the argument-frame checksum and write-map count before/after |
| parser/worker/report | `avNullResult` input, `avNullResultState`, validator pairing and pinned no-op acceptance (no write, equal frame checksum, equal write-map count, EAX unchanged, `esp + 20`); the accepted null shape is excluded from the startup-avQuery success checks |

Module `recomp-post-memory-build-121`: **721,389 bytes**, SHA-256 `4ec6816d1c1fb0aec3e1974666fa3957c59fa1daf0b22b1418fb0454feb26a52`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert (display + unknown-option + saved-data inputs, no null-result input) | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| null-result input | **status 2 at `0x001F54D0`** | **340** | **410** |

The call was accepted with the exact observed tuple (`handle 0x03920000`, option 9, param 0, Result 0): write-map marks `47 -> 47` and argument-frame checksum unchanged, EAX `0 -> 0`, `esp 62983648 -> 62983668` (**+20**, matching the measured stdcall pop). The run then advanced past Present to a translation boundary:

- `ff_title_00042BE0` (`0x00042BE0`) calls `D3DDevice_Present`, then pops EDI, adds `0x38` and **tail-jumps to `0x001F54D0`** (`title-functions.inc:29512-29519`).
- `0x001F54D0` is an **unadmitted placeholder** in the retained private translation: `void ff_title_001F54D0(void) { ff_probe_stop_direct(0x001F54D0u); }` (`title-functions.inc:43832`). Status 2 is the probe's direct-stop record; the stop's stack carries `[640, 480, 7, 1, 0, 1, 0, 0, 1, 42]`.

This is a genuine stop: admitting `0x001F54D0` needs a new translation/seed admission, not a service model. The next bounded task is to characterize and admit it (or hold); nothing about it is claimed here.

No DECISIONS row: integration head `6bc05f7` contains `c8c965c` but not `3d4cf7d`/`bfe9dec`/`753711f`/`eb685e0`, so the sync-first rule still blocks it.
## 9. Admission of D3DDevice_Release and the next stop

The prior stop at `0x001F54D0` is `D3DDevice_Release` (Ghidra, 18 instructions, body `0x001F54D0..0x001F5506`): it loads the global device pointer `[0x00206720]`, and when `[device+0x408] == 1` calls `0x001FC010`, zeroes `[0x00206724]`/`[0x00206720]` and clears `0xAE4` dwords; otherwise it simply decrements the reference count and returns.

Admission was one bounded translation increment using the retained pipeline, not a new service model:

- Read-only Ghidra export `research/verification/recomp-admit-1f54d0-01/seed-pair.json` (`0x00042BE0` + `0x001F54D0`) merged into the retained 32-seed set (replacing the single-function `0x42BE0` seed, so the 32-export cap holds).
- `tools/prepare-title-recomp.py --function-limit 512 --instruction-limit 32768 --guard-memory` produced private profile `recomp-title-recomp-title-post-memory-86` (462 functions, 18,958 instructions; the new function is 55 bytes / 18 instructions).
- Generation checks: `originalBytesMatchGhidra` true, `decoderInstructionsAgree` true, no unimplemented instructions; the single `decoderBoundariesAgree: false` normalization (`FSTCW`) matches the accepted baseline.

Module `recomp-post-memory-build-123` (same probe sources, profile 86): **722,259 bytes**, SHA-256 `9f883b098a97de324e5c50fb8c1a75b8d1624311dac7e60e0f33bf657f702e5b`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| experiment (saved-data + null-result inputs) | **status 1 at `0xFE000094`, ordinal 2** | **344** | **411** |

The experiment now passes `D3DDevice_Release` (+4 reached routines) and reaches a **new ordinal-2 call** that the default-off router honestly refuses: `AvSendTVEncoderOption(0xFD000000, 9, 1, NULL)` from `CDevice_FreeFrameBuffers` (return address `0x001FB664`). The pinned HAL's `if (!Result) return;` again makes a NULL-Result call a log-and-return, but this handle/option/param tuple differs from the authorized exact tuple, so it needs its own bounded decision. Two ordinal-1 reads and the null-result call still completed in this run.

No DECISIONS row: integration head `6bc05f7` contains `c8c965c` but not the lane commits, so the sync-first rule still blocks it.

## 10. Generalized null-Result admission and the next translation stop

The architect authorized generalizing the declared null-result model because the pinned early return is a causal source fact, not data-fitting: `kernel_hal.c:1458` tests `Result` **before** the option switch, unconditionally on handle, option and param. A nonzero Result still reaches the unmodeled query switch and stays refused.

- `src/runtime/av_null_result.c/.h` now accepts a declared call with `Result == 0` for any handle/option/param, and refuses nonzero Result and undeclared profiles.
- Native tests cover both observed pairs (the startup-range `0x03920000,9,0` and the device `0xFD000000,9,1`), one deliberately unobserved tuple labeled as a mechanism test, nonzero-Result refusal, and undeclared refusal.
- Harness: the same default-off `avNullResult` branch accepts any null Result, writes nothing, leaves EAX, pops 20 and records the call count and last full tuple.
- Validator: an accepted null call must carry `result 0`, an observed shape, no write-map delta, an equal argument-frame checksum, unchanged EAX and `esp + 20`; the pair must reproduce **both** observed shapes; nonzero-Result calls stay on the unmodeled path.

Module `recomp-post-memory-build-124` (same profile 86, updated sources): **722,209 bytes**, SHA-256 `f9b29aeed26b4230ed0d4e0385a7ea507af72fa63e9ab3d1d3720feb6ab54e64`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| experiment | **status 2 at `0x001FC910`** | **344** | **412** |

Both null-Result calls were accepted (`calls 2`; last tuple `0xFD000000`, option 9, param 1, Result 0; write-map `47 -> 47`; frame checksum unchanged; EAX `0xFD000000` unchanged; `esp + 20`). The run then reaches a **translation placeholder**: `0x001FC910` (`FUN_001fc910`, body `0x001FC910..0x001FC971`, ~97 bytes, one stack argument). Admitting it is the next bounded translation increment; nothing about its behavior is claimed here.

No DECISIONS row: integration head still lacks the lane commits.

## 11. Admission of FUN_001fc910 and the next service stop

`0x001FC910` is `FUN_001fc910` (body `0x001FC910..0x001FC971`, 98 bytes, one stack argument): it inspects a D3D format word (`[arg]` masked with `0x780000`/`0x70000`) and branches, a small format/helper function reached from the device teardown.

Admission was one bounded increment under the standing policy:

- Read-only Ghidra export `research/verification/recomp-admit-1fc910-01/seed-triple.json` (`0x00042BE0` + `0x001F54D0` + `0x001FC910`) replaced the previous two-function seed, keeping the **32-export** set (one function admitted this set, well under the eight-function bound).
- `prepare-title-recomp.py --function-limit 512 --instruction-limit 32768 --guard-memory` produced private profile 87: **463 functions, 18,990 instructions** (the new function is 98 bytes / 32 instructions).
- `verify-seeds.py` against profile 87 reports **32 recorded / 32 offered / 0 unexpected / 0 missing** ("the offered seeds match this translation's record exactly"). `verify-recomp-package.mjs` re-ran the seed check and `verify-translation.py`: 554 functions reached, 0 needing rewriting, "The translation is fully instrumented by the current guard." Its controlled-pair sub-check targets the avQuery-pair shape and does not apply to this staged pair.

Module `recomp-post-memory-build-125` (profile 87, same sources): **722,950 bytes**, SHA-256 `f77f606669311327be9a33424b77ac634f6fe4fce3b35da6e885cf8925f178ea`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| experiment | **status 1 at `0xFE000004`, ordinal 171** | **346** | **412** |

The experiment now runs two more original routines and reaches a **named-service stop**: ordinal 171 = `MmFreeContiguousMemory(PVOID BaseAddress)` with argument `0x037F2000`, return address `0x001FB6D9` inside `CDevice_FreeFrameBuffers`. The pinned bridge frees the address from the contiguous arena (falling back to the heap) and returns EAX 0; the stdcall frame is four argument bytes. Modeling that allocator behavior is a fresh service decision, so the default dispatch refuses it and the run halts here. Nothing about the free outcome is claimed.

No DECISIONS row: integration head still lacks the lane commits.

## 12. Ordinal-171 contiguous-free model and the next stop

The architect authorized one default-off model with binding stipulations: exact one-argument service shape (mapped, 4-byte-aligned, non-zero guest address), EAX 0, `esp += 8`, no guest write, no allocator state, and an unconditional disclosure that this is the pinned **declaration-level** effect only.

- `src/runtime/mm_free_contiguous.c/.h`: declared + mapped/aligned address ⇒ accept; zero, unmapped, unaligned or undeclared ⇒ refusal. The observed `0x037F2000` is diagnostics, not part of the predicate.
- Native tests cover accept, zero/unmapped/unaligned refusal and undeclared refusal.
- Harness: default-off `mmFreeContiguous` input; the ordinal-171 handler records the argument, EAX and `esp` before/after and the write-map count, sets EAX 0, pops 8, and writes nothing.
- Validator: pairing plus pinned acceptance (`address` non-zero and aligned, EAX 0, `esp + 8`, write-map count unchanged).
- Disclosure: the pinned bridge frees from its contiguous arena (heap fallback) and never changes guest memory; this harness owns no host allocator and never reuses freed blocks, so the model reproduces only that observable no-op. It is **not** allocator fidelity and makes no later-reuse claim. Ordinal 170 (`MmAllocateContiguousMemory`) never appears in the retained run, so the freed block's provenance is not determinable from the journal.

Module `recomp-post-memory-build-126` (profile 87, updated sources): **723,629 bytes**, SHA-256 `6849f0b7d76d7e860eb9b37a0ca148965310136bc8c601f577ccf956e29afaa4`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| experiment | **status 10 at `0x6B6A6968`** | **346** | **414** |

The experiment accepted **two** contiguous frees (`calls 2`): `0x037F2000` and `0x03920000`, each with write-map `47 -> 47`, EAX 0 and `esp + 8`. The run then reached a **new class of stop: a guest read fault (status 10, access 1) at `0x6B6A6968`** — the ASCII bytes `"hijk"` treated as a pointer — with return address `0x001FC087` inside `FUN_001FC010`: `MOV EAX,[ESI+0x47c]`, then `MOV EDX,[EAX]` at `0x1FC091` reads the bogus pointer. The device object's field `+0x47C` holds non-pointer data in this run.

This is a genuine halt, not a model invariant failure: both free calls matched the pinned declaration-level contract. Whether the bogus field comes from a translation gap, an earlier declaration-only model, or guest data that the pinned allocator would have changed is **not established**; the next task is a bounded read-only investigation of `FUN_001FC010` and the `+0x47C` field, not another model.

No DECISIONS row: integration head still lacks the lane commits.

## 13. Faulting-read characterization (read-only, diagnostic probe)

The faulting access is now located exactly. A default-off diagnostic watch (`guestWatch`) records the first instrumented write and read site covering a selected guest address, and the VM fault record now carries the access site as a fifth word.

Retained run (`browser-130-watch.json`, module 130, 724,074 bytes, same profile 87):

- Fault: `status 10`, address `0x6B6A6968`, 4 bytes, **read**, **site `0x1FC091`**.
- `0x1FC091` is `MOV EDX,[EAX]` in `FUN_001FC010`; `EAX` was loaded at `0x1FC087` from `[ESI+0x47C]` (after `FUN_001FC010` called `CDevice_FreeFrameBuffers` with `ECX = ESI` at `0x1FC080`).
- `D3D_g_pDevice` (`0x206720`) has **exactly one write**: site `0x1F7676` in `Direct3D_CreateDevice`, value `0x203B90`; the same site sets `[0x203F98] = 1` (the refcount) and `[0x206724] = 0x203B90`.
- Watching the device object's `+0x47C` (`0x20400C`) records **zero reads and zero writes** for the whole run.

Consequence: the object whose `[ESI+0x47C]` produced the pointer `0x6B6A6968` is **not** the static object at `0x203B90` reached through `D3D_g_pDevice`, or that field read does not pass through the instrumented access path. The writer of the pointer is therefore **not identified**, and no write-side model is warranted. The probe is diagnostic only: it changes no memory behavior and is default-off.

Next bounded task: record the faulting base (`ESI` at `0x1FC087`) and the `FUN_001FC010` entry `ECX`/caller, so the object is identified before any writer search.

Sync status: integration head `864d33c` now contains the lane commits through `36bf5f7`, and its `D374`/`D376` rows already record the null-result model, the `D3DDevice_Release` and `FUN_001fc910` admissions and the ordinal-171 model. This diagnostic characterization adds no new decision, so no additional DECISIONS row is needed.

## 14. Faulting object and its instrumented writers (read-only)

A second default-off diagnostic (`siteWatch`) records the guest address and stack words at the `FUN_001FC010` entry access (`0x1FC010`) and the `[ESI+0x47C]` read (`0x1FC087`); `guestWatchAddr` retargets the write/read watch to any 32-bit guest address.

Run `browser-133-writer.json` (module 133, same profile 87):

- The `[ESI+0x47C]` read at `0x1FC087` accessed **`0x2061D4`**, so the faulting object base is **`0x205D58`** — not `D3D_g_pDevice` (`0x203B90`). The pointer value read through it is `0x6B6A6968`.
- Watching `0x2061D4` records **two writes**, both first seen at site **`0x1FDFAA`** in `FUN_001fdf83` — a strided byte-fill helper that computes `arg*3<<8 + ecx + 0x214` and stores `CL` to several planes. No other instrumented write covers the field.
- That fill does not explain the `"hijk"` bytes: its `CL` value would have to differ per store, and the recorded site alone is not enough to show the final stored value. The final writer is therefore **not established**, nor is the object's creator/initializer.

Consequence: the fault is a read of a bogus pointer field on an object that is not the created device. No write-side or object model is warranted yet; the next bounded task is to identify the object at `0x205D58` (creator and initialization) and the exact store that leaves `0x6B6A6968`, with the fill argument/`CL` recorded.

Sync status unchanged: integration head `864d33c` contains the lane commits through `36bf5f7`; `D374`/`D376` already record the models/admissions, so this diagnostic adds no new decision.

## 15. Writer classification: the `"hijk"` value is palette ramp data

Read-only classification of the writer and the region:

- `FUN_001fdf83` (body `0x1FDF83..0x1FDFC2`) computes `base = ECX + arg*0x300 + 0x214`, then fills a 0x100-byte lane with incrementing bytes (`CL` from 0 upward): a **3-lane x 0x100 palette/ramp initializer**. It is called once from `FUN_001fe0c6` at `0x1FE1A4`, immediately before that function sets `[ESI+0x134] = 2` and calls `FUN_001ffff4`/`FUN_0020002a`/`FUN_0020005a` with `ECX = ESI` — a D3D device/state initialization sequence.
- The two instrumented stores at `0x2061D4` come from that ramp loop. Consecutive ramp bytes are exactly `0x68, 0x69, 0x6A, 0x6B`, i.e. the `0x6B6A6968` "pointer" is a run of ramp values, not a pointer.
- `0x2061D4` lies in image section 1's zero-filled virtual tail (`0x1F5340..0x206970`; file-backed only to `0x20371C`), so the ramp initializer is the only source of those bytes.

Conclusion: the field `+0x47C` of the faulting base `0x205D58` falls inside a palette-ramp-initialized region, so it holds ramp bytes rather than a pointer. The anomaly is therefore the **object base passed to `FUN_001FC010`** (`0x205D58`), not a missing write model. The caller that supplied that ECX is still unidentified (the earlier entry capture recorded the prologue stack push, not the object register). Next bounded task: capture the caller return address and ECX at the `[ESI]` read of `FUN_001FC010` (filtering out stack-push accesses), then trace whoever passes `0x205D58`.

No write-side model: the observed ramp writer is explained and the fault value is not a pointer. No DECISIONS row needed (D374/D376 cover the related models; this is characterization).

## 16. The faulting base is a clobbered register, not an object

The site watch was corrected to record the `[ESI]` read of `FUN_001FC010` (not the prologue stack pushes) and to keep a small ring of entry tuples. Run `browser-137-site.json` (module 137, profile 87):

- `FUN_001FC010` was entered from `D3DDevice_Release` (return `0x1F54E9`) with **`ECX = ESI = 0x203B90`** — the correct static device.
- The `[ESI+0x47C]` read at `0x1FC087` then used address **`0x206314`**, i.e. base **`0x205E98`**, not `0x203B90 + 0x47C = 0x20400C`. In the earlier run the same access used `0x2061D4` (base `0x205D58`).
- The base is therefore a **run-varying guest address**, not a stable object, and it differs from the entry value across runs.

Conclusion: the global translated `esi` is **clobbered between the entry of `FUN_001FC010` and the `+0x47C` read** (the call to `CDevice_FreeFrameBuffers` at `0x1FC080` is the only intervening call), or the translated callee chain fails to preserve it. This is a register-preservation/ABI defect in the translated path, not a missing write model and not a missing service. The `0x20400C` field is never accessed because the register no longer points at the device.

Next bounded task: identify the callee that leaves `esi` changed (scan the retained translation for functions that assign `esi` without a matching push/pop, and inspect `CDevice_FreeFrameBuffers`' callees), then fix or report with evidence. No model change is warranted before that.

No DECISIONS row: characterization only.

## 17. Provenance record and honest classification

The corrected trace (`browser-137-site.json`) does **not** support a synthetic-profile state divergence:

- The supplied profile is not involved in the object register. `FUN_001FC010` runs once, entered from `D3DDevice_Release` (`ECX = ESI = 0x203B90`, the device global written only by `Direct3D_CreateDevice` at site `0x1F7676`).
- The `[ESI+0x47C]` read then uses a **heap-like, run-varying** base (`0x205E98` this run, `0x205D58` before), not `0x203B90 + 0x47C`. No guest write to the device field occurs (`0x20400C` reads/writes = 0).
- Callee register accounting in the retained translation: `FUN_001FB300`, `FUN_001FAED0`, `FUN_001FACA0` assign `esi` zero times; `FUN_001FB640` pushes and pops it once; `FUN_001FC010` itself pushes `esi` twice and pops it once. The global `esi` therefore changes across the call chain without a matching restore somewhere below this level.

Classification: this is a **translated-register preservation defect** (or a model/tooling register clobber), not a profile-fidelity gap and not a missing write model or service. The exact callee is not yet identified, so no fix is attempted here. The read-fault recording stays the standard: the run halts at the fault and reports it.

Next bounded task: instrument or scan for the specific callee that leaves `esi` changed (per-function push/pop balance across the reached call chain), then regenerate/fix with evidence. No write-side or pointer model.

## 18. ABI audit result: no ESI violation, and a measurement conflict

The toolkit's own callee-saved check (`RECOMP_ABI_CHECK`, enabled only in the private diagnostic build) was compiled into the probe with a source-owned logger:

- 310 call-target violations were recorded overall (first `0x1E04CC`, last `0x1FB640`), i.e. the translated code has callee-saved-register defects in several places.
- **ESI violations: 0.** No call returns with `g_esi` changed, including the `CDevice_FreeFrameBuffers` call at `0x1FC080`.

That contradicts the site-watch inference: entry `ESI = 0x203B90`, yet the `0x1FC087` field read appears to use `0x206314`. Since the ABI check is authoritative for callee preservation, the earlier "clobbered register" reading is **not proven**; one of the two measurements (field-address association or entry association) is misleading.

Honest status: the ESI-clobber hypothesis is withdrawn. No fix and no model. The next bounded task is to log `g_esi` **at the field access itself** (site `0x1FC087`) instead of inferring the base from the address, and to confirm how many times `FUN_001FC010` is entered (the ring reported one entry).

## 19. Reconciliation attempt: the two measurements cannot both hold

Read-only checks against the retained translation:

- Site `0x1FC087` occurs exactly once in the translation — the `[ESI+0x47C]` read at line 5240, reached only by fallthrough after the `RECOMP_ABI_CALL(0x001FB640, ...)` at `0x1FC080`. There is no alternate entry or `goto loc_001FC087`.
- The ABI check wraps that exact call (`RECOMP_ABI_CALL`), and reports 0 ESI changes across all calls. So a direct callee cannot explain a changed `g_esi` at the field read.
- The site watch recorded exactly one entry (`ESI = 0x203B90`) and one field access (`0x206314`). With the function's only `esi` assignment at entry (`esi = ecx`) and its next assignment at site `0x1FC167` (after the field read), the two records are mutually inconsistent for a single invocation.

Therefore the contradiction is **not resolved**: either the field-address watch sampled an access that is not the instrumented read (association error), or an indirect/tail-call path below `CDevice_FreeFrameBuffers` changes `g_esi` without being covered by the direct-call ABI macro. Both are tooling/measurement questions, not a guest-state or profile question, and neither justifies a workaround or model.

Next bounded task: add an ESI value capture *inside the `0x1FC087` access itself* (record `g_esi` alongside the address) and enlarge/report the entry ring, so the two diagnostics are sampled at the same instant; then re-run and compare.

No DECISIONS row: characterization only.

## 20. Single-record capture: the field access is genuine; the ABI count is the suspect instrument

`siteWatch` now records `g_esi` inside the `0x1FC087` access itself. One run (`browser-140-abi.json`, module 140, private `RECOMP_ABI_CHECK` build):

| Measurement | Value |
| --- | --- |
| Entry ESI / ECX (site `0x1FC010`) | `0x203B90` / `0x203B90`, return `0x1F54E9` (from `D3DDevice_Release`) |
| Field access address (site `0x1FC087`) | `0x206314` |
| `g_esi` at that access | `0x205E98` |
| `address - g_esi` | **`0x47C`** (exact) |
| ABI violations / ESI violations | 310 / **0** |
| Field access count | 1 |

The exact `0x47C` delta proves the access is the instrumented `[ESI+0x47C]` read and that `g_esi` really was `0x205E98` at that instant. The ABI logger recorded no ESI change across any call. Both instruments are therefore measuring the same run at different instants, and at least one is wrong about the call boundary: the direct-call ABI macro must have missed the `0x1FC080` call (or its comparison is invalid for this call), while the in-access capture cannot be an association error.

Working conclusion: the **ABI instrumentation coverage is the suspect**, not the site watch and not a guest-state/profile divergence. No workaround, no model, no translator edit.

Next bounded task: instrument the `FUN_001FB640` entry access (site `0x1FB640`) to record `g_esi` at callee entry, then compare with the post-call value in the same record; that decides whether the change happens inside that callee and why the direct-call check did not report it.

No DECISIONS row: characterization only.

## 21. Callee-access probe: the transition is inside a callee, before its first access

`siteWatch` now records `g_esi` at the first memory access of each callee of `FUN_001FC010` (sites `0x1FB300`, `0x1FAED0`, `0x1FACA0`, `0x1FB640`). Run `browser-141-abi.json` (module 141, private ABI build):

| Observation point | `g_esi` |
| --- | --- |
| `FUN_001FC010` entry | `0x203B90` |
| first access in `FUN_001FB300` | `0x203B90` |
| first access in `FUN_001FAED0` | **`0x205E98`** |
| first access in `FUN_001FACA0` | not observed (no memory access at its entry site) |
| first access in `FUN_001FB640` | `0x203B90` |
| `[ESI+0x47C]` read at `0x1FC087` | `0x205E98` |
| ABI violations / ESI | 310 / 0 |

Interpretation: the probe samples the **first memory access inside each callee**, not the call boundary. `FUN_001FAED0` already shows `0x205E98` at its first access, so ESI changes *before that access* (in the caller tail or the callee prologue); `FUN_001FB640` shows the device value again at its first access, yet the following field read shows `0x205E98`. The ABI check still reports no net ESI change per call.

Conclusion: the transition is **not localised to a single call/return boundary** by this instrument, and the ABI logger's placement (net check after the call) remains inconsistent with the in-access capture. This is a tooling/instrumentation gap, not a guest-state or profile divergence. No workaround, model, or translator edit; halt here.

Next bounded task: capture `g_esi` at the exact call boundary using the translated call site itself (e.g., a hook on the `0x1FC080` return address), which needs either a source-owned call-boundary probe or a translator-level trace — the latter is a tooling finding to record, not a local patch.

No DECISIONS row: characterization only.

## 22. Span probe: ESI is correct after the first two calls and wrong at the field read

The probe now samples `g_esi` at the caller's post-call access sites inside `FUN_001FC010`. Run `browser-142-abi.json` (module 142, private ABI build):

| Sample point | `g_esi` |
| --- | --- |
| `FUN_001FC010` entry (`0x1FC010`) | `0x203B90` |
| after call 1 (`0x1FC01F`, `FUN_001FB300`) | `0x203B90` |
| after call 2 (`0x1FC038`, `FUN_001FAED0`) | `0x203B90` |
| loop site `0x1FC055` | not accessed (the loop is skipped on this path) |
| `[ESI+0x47C]` read (`0x1FC087`) | `0x205E98` |

So `g_esi` is still the device after the second call and has changed by the field read. On this path the only remaining calls are `0x1FC077` → `FUN_001FD630` (and its callees); `FUN_001FD630` itself never assigns `esi` (push/pop/assign all 0), so the change must occur inside **its** call chain — where the direct-call ABI check should have fired but recorded 0 ESI violations.

This narrows the defect to the `0x1FC077` span and to a call form the ABI macro does not cover (indirect/tail path), but the exact edge is still not named. No workaround, model, or translation edit.

Next bounded task: probe `g_esi` around the `0x1FC077` call (`FUN_001FD630`) and its indirect callees, and check whether that call form bypasses `RECOMP_ABI_CALL` (a tooling finding for the translator, not a local patch).

No DECISIONS row: characterization only.

## 23. ESI-change log: the probes are confounded by TLS/thread-context switching

The probe now records every observed change of `g_esi` (site, access address, new value) and reports the last eight. Run `browser-143-abi.json` (module 143):

- The reported changes are `0x03F0xxxx` values and `0x55667788` at sites `0x1DF9C0`, `0x1DF9F3` and site `0`, i.e. values belonging to **other thread/TLS contexts**, not the device sequence `0x203B90 -> 0x205E98`.
- The `0x1FC087` field access still reports `g_esi = 0x205E98`, while the change log contains no transition to that value and `FUN_001FC010`'s entry still reports `0x203B90`.

Conclusion: `g_esi` is `RECOMP_TLS` storage, and the harness models thread/TLS context switches; the diagnostic reads are therefore **not from a single register context**. The ESI sequence cannot be reconstructed with these probes, and the earlier "clobber" readings are unreliable. This is a **harness/tooling instrumentation limit**, not a guest-state, profile, or translator defect demonstrated by this evidence.

No workaround, model, or translator edit. The ABI-clobber hunt is halted pending a context-aware probe (record the TLS/thread identity with each read) or a translator-level register trace supplied as tooling evidence.

No DECISIONS row: characterization only.

## 24. Thread-scoped logger repair and its residual limit

Authorized tooling repair (not a workaround): a pure `ff_tls_same_context(entry_fs, current_fs)` predicate (new `src/runtime/tls_scope.c/.h`, native test `tests/tls_scope_test.c`) now gates both diagnostics:

- The ESI-change log only records a change when the TLS base is unchanged; a context switch resets the baseline instead of being logged as a register change.
- The ABI logger skips its ESI comparison when the TLS base differs from the previous call's.

Re-run `browser-144-abi.json` (module 144, 50/50 native fixtures green):

- The log still reports `0x03F0xxxx`/`0x55667788` values at sites `0x1DF9C0`/`0x1DF9F3`/site 0 **within one `g_fs_base`**, i.e. these are real transient register uses by those functions, not TLS switches.
- The `0x1FC087` field access still reports `g_esi = 0x205E98` and the entry `0x203B90`, but **no transition to `0x205E98` is logged**, and the same call's change-check baseline differs from the captured field ESI.

Conclusion: thread-scoping removed the TLS confound but did not reconcile the samples. The remaining inconsistency means the diagnostic cannot yet observe the field access's register state consistently — a harness/instrumentation limit (possibly two register views), not a demonstrated guest-state, profile, or translator defect. The repair is committed with its fixture test; the ESI-clobber hunt remains halted pending a translator-level register trace or a single-view register capture.

No DECISIONS row: tooling characterization.

## 25. Per-context ESI baselines and the remaining capture gap

The logger was extended to keep per-TLS-context baselines (up to eight contexts) so another thread's interleaved accesses cannot swallow the device context's transition; `entryFs`/`fieldFs` are now reported alongside the samples.

Run `browser-147-abi.json` (module 147):

- Entry and field access are in the **same** TLS context (`0x03C00200`), so this is not a thread switch.
- `g_esi` is `0x203B90` at entry and `0x205E98` at the field access, yet the change log still contains only the `0x03F0xxxx`/`0x55667788` transitions at `0x1DF9C0`/`0x1DF9F3`/site 0 and **no `0x205E98` transition** — even with per-context baselines and sixteen change slots available.

Conclusion: the change log is not observing the same register value that the field capture reports for that access. Two register views (or a capture that runs outside the translated `esi` storage) remain the only consistent explanation; this is a harness/instrumentation limit, not a guest-state, profile, or translator defect. The repair is committed; the hunt stays halted pending a single-view capture or a translator-level register trace, and no workaround is applied.

No DECISIONS row: tooling characterization.

## 26. The polluter is the probe's own register canary

The change logger now also keeps a dedicated last-change record for the device TLS context. Run `browser-148-abi.json` (module 148):

- Device-context changes: **31,724**, last value `0x55667788` at site 0.
- `g_esi = 0x55667788` is written by the probe's own copy-proof canary in `ff_probe_copy_case` (`tools/fixtures/thread_startup_probe.c:1194`), which sets `g_ebx/g_esi/g_edi/g_ebp` canaries to verify register preservation.
- The canary value therefore appears in the same TLS register storage the diagnostics sample, at stack-push sites (site 0), tens of thousands of times, and masks the genuine device transition.

Conclusion: the ESI trace is polluted by the **fixture's own register canaries**, not by the guest or the translation. The 0x205E98 capture at the field access cannot be attributed until diagnostic register writes are separated from the guest view (e.g., the canary tests must save/restore the guest registers around their probes, or the trace must ignore writes made by diagnostic code). This is a source-owned fixture defect; no translator edit, no model, no `esi` backup in the guest path.

Next bounded task: isolate the copy/ABI canary writes from the guest TLS view (fixture-side save/restore around `ff_probe_copy_case`, or a diagnostic flag that suppresses logging during canary probes), then re-run the span probe.

No DECISIONS row: tooling characterization.

## 27. Freeze-at-stop fixes the trace; the transition is `FUN_001F6240` at `0x1F62C4`

The register trace is now **frozen when the run stops** (`reg_trace_frozen` set in `stop()`, reset when the site watch is enabled), so post-run copy/ABI canaries can no longer overwrite the recorded transitions. Run `browser-149-abi.json` (module 149):

- Device-context changes drop from 31,724 to **16**; the last value is **`0x205E98`**.
- Last transitions include `0x1F62C4 -> 0x203B90` and then `0x205E98` observed at the next stack-push site.
- Site `0x1F62C4` is in `FUN_001F6240`: the translated instruction is `esi = edx + ecx * 8` (with `ecx = 3 * MEM[esp+0x24]`), i.e. it assigns ESI to a computed pointer — the exact `0x205E98` pointer later read as `[ESI+0x47C]`.
- Translation accounting for `FUN_001F6240`: 1 ESI push, 2 ESI pops, 3 ESI assignments — an unbalanced save/restore with an in-body assignment.

Conclusion: the `0x203B90 -> 0x205E98` transition is produced by `FUN_001F6240` at `0x1F62C4` (`esi = edx + ecx*8`), whose ESI handling is unbalanced; the ABI logger did not attribute it because that call form is not covered by the direct-call macro (indirect/tail path). This is a translated-register/tooling finding tied to one named function — no workaround, no model, no translator edit.

Next bounded task: confirm the call form that reaches `FUN_001F6240` (direct `RECOMP_ABI_CALL` vs `RECOMP_ICALL_SAFE_AT`/tail) and its per-exit ESI restore, then report the exact defect for the translator owner.

## 28. Coverage-gap class: non-call-site register writes

Final characterization of the localized write and the logger gap:

- `FUN_001F6240` assigns `esi` in-body at `0x1F62C4` (`esi = edx + ecx*8`, with `ecx = 3*MEM[esp+0x24]`) and its four callers are direct `RECOMP_ABI_CALL` sites (`0x1FB6D0`, `0x1FB6FD`, `0x1FB92C`, `0x1FBA0C`).
- Its ESI accounting is unbalanced: one `PUSH esi`, two `POP esi`, three assignments. The value `0x205E98` is the computed pointer later read as `[ESI+0x47C]`.
- The ABI logger reports zero ESI violations because it only compares **net** register values at wrapped call boundaries; an in-body write that is consumed before the boundary (or whose save/restore is path-dependent) is invisible to it. The fixture trace could see it only because it samples every guest access and is frozen at the stop.

Coverage class: **non-call-site register writes** are outside the call-boundary ABI check. That is now pinned in the native fixture: `ff_reg_change_observable(at_call_boundary)` returns 1 only for a wrapped boundary, with assertions in `tests/tls_scope_test.c`.

This is the final tooling-repair result: the instrument localizes the transition to `FUN_001F6240`/`0x1F62C4` and names the coverage class; the defect belongs to the translator/translation (register save/restore imbalance), not to the guest, the profile, or a harness workaround. No translator edit is made here.

No DECISIONS row: tooling characterization.

## 29. Confirm-and-report: FUN_001F6240 restores ESI; the earlier defect claim is retracted

Final confirmation of the call form and per-exit restore (read-only):

- All four callers of `FUN_001F6240` are **direct** `RECOMP_ABI_CALL` sites inside `FUN_001FB640` (`0x1FB6D0`, `0x1FB6FD`, `0x1FB92C`, `0x1FBA0C`), so the ABI check covers them.
- The function pushes `esi` once at entry and **pops `esi` before every return** (`POP esp, esi` immediately precedes each `esp += 12; return /* ret 8 */`). The `esi = edx + ecx*8` assignment at `0x1F62C4` is therefore **transient**: it is restored before the function returns.
- The static "1 push / 2 pops / 3 assignments" count is an artifact of two mutually exclusive exit paths sharing one entry push; it is not an unbalanced save.

Formal translator-owner finding: **no register-preservation defect is demonstrated at `FUN_001F6240`/`0x1F62C4`.** The `0x205E98` value recorded by the trace is the transient computed pointer inside that function, and the ABI check's zero ESI violations is consistent with the pop-before-return evidence. The earlier claim that this function leaks `esi` is **retracted**. The synthetic-run fault at `0x6B6A6968` therefore remains unattributed to this function; the next investigation must explain why `FUN_001FC010`'s `[ESI+0x47C]` read reports `0x205E98` even though the callee restores ESI (a remaining capture/instrument question, not a translator defect).

No translator edit is made; the repair decision belongs to the translator owner/user. No DECISIONS row: characterization only.

## 30. Ground-truth ESI arbiter: verdict is a genuine register value

The arbiter compares two independent records of the same read:

- **Field capture** (memory hook at site `0x1FC087`): address `0x206314`, `g_esi = 0x205E98`; `address - g_esi = 0x47C` exactly.
- **Fault frame** (physical memory-guard path at the following read, site `0x1FC091`): faulting address `0x6B6A6968`, 4 bytes, read, **`g_esi = 0x205E98`**.

The worker now computes `esiArbiter { faultEsi, fieldEsi, fieldAddr, deltaOk, sameEsi }` whenever the fault site is `0x1FC091` and a field capture exists; the validator rejects any report where the two disagree. Run `browser-150-arbiter.json` (module 150): `faultEsi = fieldEsi = 0x205E98`, `deltaOk = true`, `sameEsi = true`.

**Verdict: the register genuinely held `0x205E98` at the read; the instrument does not misreport it.** The value changed from the entry's `0x203B90` (the fault-frame address chain and the exact `0x47C` delta rule out an association error). The remaining question is which executed path leaves `0x205E98` in `esi` even though `FUN_001F6240` pops it before both returns; that is a genuine register-state question, not an instrumentation one, and no fix or model is attempted here.

No DECISIONS row: characterization only.

## 31. Transition frame: writer is `0x1F62C4`; the push frame carries a sentinel return

The frozen trace now records the return word (`MEM32(g_esp+4)`) with each ESI change. Run `browser-151-writer.json` (module 151):

| Site | Access address | `g_esi` | return word |
| --- | --- | --- | --- |
| `0x1F62C4` (in `FUN_001F6240`) | `0x205D60` | `0x203B90` | `0x203B90` |
| `0` (stack push) | `0x3C10E38` | **`0x205E98`** | **`0xFFFFFFFF`** |

- The change from `0x203B90` to `0x205E98` is first observed at the access immediately after `FUN_001F6240`'s `esi = edx + ecx*8` at `0x1F62C4`; that instruction is the writer.
- The observing access is a stack push at `0x3C10E38`, whose frame word is `0xFFFFFFFF` — a sentinel, not a valid caller return address, so this frame does not name the caller and is consistent with synthetic stack use (the `FF_MEMORY_PUSH` site 0 path).
- The later `[ESI+0x47C]` read still reports `0x205E98` (arbiter-confirmed), so the value survives past the function's `POP esi` on the executed path, or the field read occurs in a different invocation before that pop.

Report: this is **translated-path behavior** (the writer instruction is identified: `0x1F62C4`, `esi = edx + ecx*8`), not a probe artifact of the value; the sentinel frame is a probe-frame limitation. Whether the pop is skipped on the executed path remains open. No fix or model is attempted.

No DECISIONS row: characterization only.

## 32. Per-invocation tuples: one FUN_001FC010 entry, two FUN_001F6240 invocations, no monitored writer

The probe now records every access at `0x1F6240`, `0x1F62C4`, `0x1FC010` and `0x1FC087` in one ring (`esiEvents`). Run `browser-152-tuples.json` (module 152), last events in order:

| Site | `g_esi` | Access address |
| --- | --- | --- |
| `0x1F6240` | `0x280` | `0x206720` / stack (earlier invocations) |
| `0x1FC010` | `0x203B90` | `0x203B90` |
| `0x1F6240` | `0x203B90` | `0x206720`, stack |
| `0x1F62C4` | `0` | stack |
| `0x1F62C4` | `0x203BA8` | `0x205D78` |
| `0x1F6240` | `0x203B90` | `0x206720`, stack |
| `0x1F62C4` | `0` | stack |
| `0x1F62C4` | `0x203B90` | `0x205D60` |
| **`0x1FC087`** | **`0x205E98`** | `0x206314` |

Classification:

- **Not a different invocation of `FUN_001FC010`**: exactly one `0x1FC010` entry is recorded, and the field read follows it.
- The field read is the next monitored event after the second `FUN_001F6240`'s `0x1F62C4` access (where `g_esi` was `0x203B90`); the `0x205E98` value appears in the **unmonitored span** between that access and the field read.
- Because the function's exit is not at a monitored site (push/pop use site 0), the probe cannot yet distinguish "pop skipped" from "value written in the unmonitored span".

Report: survival is **not** explained by a second `FUN_001FC010` invocation; the pop-skip question remains open and requires a return-boundary sample (caller post-call site) rather than more sites inside the callee. No fix or model.

No DECISIONS row: characterization only.

## 33. Return-boundary samples complete the classification

The tuple probe now also samples the four post-call sites in `FUN_001FB640` (`0x1FB6D0`, `0x1FB6FD`, `0x1FB92C`, `0x1FBA0C`). Run `browser-153-tuples.json` (module 153), ordered events after the single `FUN_001FC010` entry (`ESI = 0x203B90`):

| Event | `g_esi` |
| --- | --- |
| `FUN_001F6240` invocation A entry | `0x203B90` |
| A's `0x1F62C4` access (before/after assign) | `0` / `0x203BA8` |
| **A post-call `0x1FB6D0`** | **`0x203B90`** |
| `FUN_001F6240` invocation B entry | `0x203B90` |
| B's `0x1F62C4` access (before/after assign) | `0` / `0x203B90` |
| **B post-call `0x1FB6FD`** | **`0x203B90`** |
| `0x1FC087` field read | **`0x205E98`** |

Classification is now complete:

- **Not pop-skipped**: both invocations return with `ESI = 0x203B90` at the caller's post-call sites; the pop restores correctly.
- **Not a different invocation**: exactly one `FUN_001FC010` entry.
- The writer of `0x205E98` lies **after the `0x1FB6FD` post-call site and before `FUN_001FB640` returns** (the next executed instruction in `FUN_001FC010` after the call at `0x1FC080` is the field read itself).

So the survival is a value written in the tail of `FUN_001FB640`, not a skipped restore and not a re-entry. No fix or model; the next bounded task is to sample the remaining `FUN_001FB640` accesses after `0x1FB6FD` to name the exact instruction.

No DECISIONS row: characterization only.

## 34. Tail span: the writer is the epilogue `POP esi` (stack-accounting candidate)

The probe now records per-access events in the `FUN_001FB640` tail (`0x1FB6FD..0x1FB740`). Run `browser-154-tail.json` (module 154), last events:

- `0x1FB706` (rep-stosd loop): ESI `0x203B90`, addresses `0x205CF0..0x205D24`.
- `0x1FB71B` (`[esi+0x2148]` compare): ESI `0x203B90`, address `0x205CD8`.
- **No `0x1FB728` event** — the branch at `0x1FB71B` took `je 0x1FB72E`, so the indirect call at `0x1FB725` did **not** execute.
- Next event: `0x1FC087` field read with ESI `0x205E98`.

Between the last tail access (`0x1FB71B`, ESI `0x203B90`) and the field read, the only translated code is the epilogue `POP edi; POP esi; POP ebp; POP ebx; ret`. There is **no `esi =` assignment** in that span, so the write to ESI is the **epilogue `POP esi`** itself: it restores a stack word equal to `0x205E98` instead of the saved `0x203B90`.

Classification: the writer is the `POP esi` at `FUN_001FB640`'s epilogue, i.e. a **stack-accounting (save-slot) defect** in the translated function — the popped slot does not hold the entry value. This is a translator/translation finding, not a guest-state, profile, or probe artifact; no fix or model is applied. Confirming the exact popped word needs a pop-value capture (site 0 read value) next.

No DECISIONS row: characterization only.

## 35. Pop-word capture: the restore slots are off by one

The probe now records the value of every stack read at site 0 (`popEvents`). Run `browser-155-pop.json` (module 155), last stack reads before the field access:

| Stack slot | Value read |
| --- | --- |
| `0x3C10E34` | **`0x205E98`** |
| `0x3C10E38` | **`0x203B90`** |
| `0x3C10E3C` | `0xFFFFFFFF` |
| `0x3C10E40` | `0x3C10CFC` |

`FUN_001FB640`'s epilogue pops `edi, esi, ebp, ebx` in that order, so the first slot (`0x3C10E34 = 0x205E98`) is consumed by the **EDI** pop and the second (`0x3C10E38 = 0x203B90`) by the **ESI** pop — yet the following field read observes `ESI = 0x205E98`. The save/pop slots are therefore **off by one register**: the value intended for one register (the restore slot at `0x3C10E34`) lands in `ESI`, and the correct `0x203B90` is consumed by the next pop.

This is a translated stack-accounting/save-slot mismatch, not guest state, profile, or probe artifact. No fix or model; the repair decision belongs to the translator owner/user.

No DECISIONS row: characterization only.

## 36. Correction: the off-by-one reading is provisional, not confirmed

The pop-word capture cannot yet be tied to specific registers: the four last stack reads (`0x3C10E34 = 0x205E98`, `0x3C10E38 = 0x203B90`, `0x3C10E3C = 0xFFFFFFFF`, `0x3C10E40 = 0x3C10CFC`) are **unordered with respect to the `edi/esi/ebp/ebx` pop sequence** — the probe records the value read at each site-0 stack access, not which register consumed it. Reading them as the epilogue order is an assumption.

Two facts remain solid: (a) the field read at `0x1FC087` observes `ESI = 0x205E98` (arbiter-confirmed), and (b) the last monitored tail access `0x1FB71B` observes `ESI = 0x203B90` with only epilogue pops in between. The claim that the restore slots are off by one is therefore **provisional** and requires an order-tagged pop capture (record the pop sequence index and the destination register) before it is reported as a translator defect.

Honest status: the writer is localised to the `FUN_001FB640` epilogue, the exact pop/register mapping is unconfirmed, and no fix or model is warranted. The next bounded task is the order-tagged pop capture.

No DECISIONS row: characterization only.

## 37. Order-tagged pops confirm: ESI receives the saved-EDI slot

The probe now records `g_esi` after each stack pop (`popEvents` third field). Run `browser-156-postpop.json` (module 156):

- `FUN_001FB640` prologue pushes `ebx, ebp, esi, edi` → slots `0x3C10E40 = 0x3C10CFC` (ebx), `0x3C10E3C = 0xFFFFFFFF` (ebp), `0x3C10E38 = 0x203B90` (esi), `0x3C10E34 = 0x205E98` (edi).
- The .inc epilogue order is `pop edi; pop esi; pop ebp; pop ebx`.
- Post-pop ESI trace: ESI is `0x203B90` after every pop up to `0x3C10E30`; it becomes **`0x205E98` at the pop consuming `0x3C10E34`** (the saved-EDI slot) and stays `0x205E98` for the remaining pops (`0x3C10E38`, `0x3C10E3C`, `0x3C10E40`).

So the executed epilogue writes the **saved-EDI value into ESI**, while the saved-ESI value (`0x203B90`) is consumed by another pop. The literal source order would not do that, so the pop/register mapping in the executed translation is inconsistent with its own text — the exact defect class: **stack-slot/register mapping mismatch in the `FUN_001FB640` epilogue** (pop order or macro expansion), not guest state, profile, or probe artifact.

This is now confirmed by direct post-pop tagging. No fix or model is applied; the repair decision belongs to the translator owner/user.

No DECISIONS row: characterization only.

## 38. Save/pop-order rule pinned; EXECUTION_GOAL_NOV5 unavailable

- `docs/EXECUTION_GOAL_NOV5.md` (referenced by the current AGENTS directive) is **not present in this worktree**; Runtime-b's milestone order is therefore unavailable. Blocker recorded; independent source-owned work chosen instead.
- The confirmed epilogue mismatch is now expressed as a pure, tested rule: `ff_reg_order_is_reverse(push_order, pop_order, count)` in `src/runtime/reg_save_order.c/.h`, with native test `tests/reg_save_order_test.c`. It accepts the literal reverse order (`ebx, ebp, esi, edi` → `edi, esi, ebp, ebx`) and rejects the observed runtime order (saved-EDI slot consumed first as ESI). Native suite passes **52/52**.

Formal translator-owner finding: `FUN_001FB640`'s executed epilogue does not restore the callee-saved registers in the reverse of its push order; the saved-EDI value (`0x205E98`) lands in ESI, and the field read at `0x1FC087` uses it as `[ESI+0x47C]`. The rule that any repair must satisfy is now pinned by the native test. No translator edit is made; the repair decision belongs to the translator owner/user.

No DECISIONS row: characterization plus a pure validation rule.

## 39. Pop macro inspected: the shift is not in the macro

`src/runtime/recomp_memory_access.h:54` defines the pop rule as: `ff_pop_value = FF_MEMORY_U32(sp, 1u); (sp) += 4u; (destination) = ff_pop_value;` — a correct read-before-increment with a single destination. The macro therefore cannot by itself place a saved-EDI value into ESI.

Two possibilities remain, and neither can be decided with the memory hook alone:

1. The **executed epilogue order** differs from the `.inc` text (`pop edi; pop esi; pop ebp; pop ebx`), e.g. an alternate path or a translation mismatch; or
2. The **post-pop attribution** (recording `g_esi` at the next hook invocation) is off by one relative to the actual register assignment.

Both are register-write questions; the memory hook observes addresses and stack words, not register destinations. No translator or macro defect is asserted beyond "the executed order/mapping is inconsistent with the source text". The repair decision and any register-level trace belong to the translator owner/user.

Blocker recorded: confirming (1) vs (2) needs a register-write trace (toolchain-level), which this lane must not add. No fix, no model.

No DECISIONS row: characterization only.

## 40. Blockers package published (D401)

The defect-class capture question is closed. The blockers package is published as the newest entry in `docs/DEVELOPMENT_HANDOFF.md`, with D401 in `docs/DECISIONS.md`:

1. **Tested rule**: `ff_reg_order_is_reverse` (`src/runtime/reg_save_order.c/.h`, `tests/reg_save_order_test.c`, native 52/52). Literal reverse accepted; observed runtime order (saved-EDI `0x205E98` consumed first as ESI) rejected.
2. **Candidate owners**:
   - (A) translator emission in `tools/prepare-title-recomp.py` that produced the `FUN_001FB640` prologue/epilogue in `research/verification/recomp-title-recomp-title-post-memory-87/title-functions.inc` (prologue lines 6463-6466 `ebx, ebp, esi, edi`; epilogue lines 6612-6619 `edi, esi, ebp, ebx`);
   - (B) the `.inc` expansion of the original POP order through `src/runtime/recomp_memory_access.h:54` (`FF_MEMORY_POP` read at `sp`, `sp += 4`, single destination).
3. **Bounded question**: confirm which side is wrong and decide the repair; any repair must pass the tested predicate and restore `esi` so `[ESI+0x47C]` reads `0x20400C`, not `0x206314`. The absence of a single named writer inside the translated epilogue is the finding.

No translator edit, no model, no further hunt from this lane. Observed stop unchanged: status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops.

## 41. M2 evidence snapshot and the unblocked goal document

`docs/EXECUTION_GOAL_NOV5.md` is **available on the integration head** (`git show 18cce1d:docs/EXECUTION_GOAL_NOV5.md`); earlier "absent from this worktree" notes are corrected — Runtime-b's milestone order (M0-M5) is readable.

M2 ("device access governed by one recorded policy") evidence from the retained report `browser-156-postpop.json`:

| Metric | Value |
| --- | --- |
| Device events (`deviceRegisters.count`) | **5,302** |
| Device access log entries | **12,637** |
| Entries with `beforeKnown` provenance | **7,399** |
| Status waits | 0 |

That satisfies the ">=10 admitted device events with beforeKnown provenance" criterion with a wide margin, and the lane has advanced the boundary well past M1's graphics-init stop (through `0x001fe085`-class work to the current `FUN_001FB640` epilogue). The remaining M2 item is the **explicit recorded device-access policy decision** in the lane's DECISIONS record; the integration branch's decision rows must be checked before adding one here (next free number, no collision).

Next bounded task: record or confirm the device-access policy decision (D-row) against the existing provenance evidence, or pick the next M3 item once the translator-owned epilogue repair lands.

No DECISIONS row this cycle: evidence snapshot only.

## 42. Device-access policy recorded (D402)

M2's remaining item is recorded as D402: device accesses are governed by the declared synthetic register window only — reads of previously written addresses return the shadow value with `beforeKnown` provenance; undeclared/unknown reads fault with the exact fault record (`status 10`, address/width/access/site); widths and bounds are restricted to the declared aperture; no unknown-read value is fabricated and no write is invented. Evidence: `browser-156-postpop.json` — 5,302 device events, 12,637 log entries, 7,399 with `beforeKnown`, 0 status waits. Two boundary advances since M1 are recorded in this checkpoint. No translator or guest-state change.

Lane state remains the confirmed `FUN_001FB640` epilogue mismatch (D401 blockers package) with the observed stop at `0x6B6A6968`; the next executable step depends on the translator owner's repair decision, so no new model or translation change is attempted here.

## 43. Boundary-advance evidence for M2 (retained report chain)

The retained `research/verification/pfifo-setup-01/browser-*.json` chain documents the advances since M1 (graphics-init stop):

| Report | Stop | Routines / ops |
| --- | --- | --- |
| `browser-104-reset-avquery` | status 13 at `0xFD800044` (pushbuffer control) | 339 / 405 |
| `browser-112-consumer` | status 1 at `0xFE000094`, ordinal 159 | 340 / 405 |
| `browser-118-avunknown` | ordinal 3 after the vblank-wait pass | 340 / 406 |
| `browser-119-saved` | ordinal 2 after the display declaration | 340 / 407 |
| `browser-120-saved` | ordinal 1 at `0xFE000098` | 340 / 408 |
| `browser-121-null` | ordinal 2 at `0xFE000094` | 340 / 409 |
| `browser-123-null` | ordinal 2 after `D3DDevice_Release` admission | 344 / 411 |
| `browser-125-null` | ordinal 171 at `0xFE000004` (`MmFreeContiguousMemory`) | 346 / 412 |
| `browser-126-free` | status 10 at `0x6B6A6968` (read fault) | 346 / 414 |

That is well over the M2 requirement of two boundary advances beyond M1, together with the recorded device-access policy (D402). The lane is now blocked at the translated epilogue mismatch (D401), not at a service or resource.

No DECISIONS row: evidence consolidation for D402.

## 44. Textual check narrows the package: emitted order is correct

Targeted check of the retained translation (module 87, same bytes used at runtime):

- `FUN_001FB640` prologue pushes, in order: `ebx, ebp, esi, edi`.
- Epilogue pops, in order: `edi, esi, ebp, ebx`.
- The pop sequence **is the exact reverse** of the push sequence — the emitted `.inc` text is correct.

Candidate (A) — "the translator's emitted callee-saved order text is wrong" — is therefore **refuted for this function**. The remaining candidates are (B) the expansion/execution of that text (`src/runtime/recomp_memory_access.h:54` path, or an alternate executed exit) and the post-pop attribution. This narrows the translator-owner question: the repair is not in the emitted order but in why the executed mapping differs from correct text.

A bounded corpus audit tool was added (`tools/reg-save-order-audit.mjs`) to check prologue/epilogue order across a translation; it reports addresses and register orders only. Its prologue detection is conservative and noisy for functions whose first statement is not a save push, so it is a targeted aid, not a corpus verdict.

No DECISIONS row: narrowing evidence for D401.

## 45. Attribution proven adjacent; the shift is real and not a sampling artifact

Sequence-tagged pop capture (module 158; the two named reports for this cycle are `browser-157-control.json` and `browser-157-experiment.json`, same module `ee284034...`):

| Pop read address | Value | Hook seq | Post-ESI measured at seq |
| --- | --- | --- | --- |
| `0x3C10E30` | `0x1FB64C` | 83,980 | 83,981 → `0x203B90` |
| `0x3C10E34` | `0x205E98` | 83,981 | 83,982 → **`0x205E98`** |
| `0x3C10E38` | `0x203B90` | 83,982 | 83,983 → `0x205E98` |
| `0x3C10E3C` | `0xFFFFFFFF` | 83,983 | 83,984 → `0x205E98` |
| `0x3C10E40` | `0x3C10CFC` | 83,984 | 83,985 → `0x205E98` |

Each post value is sampled at the immediately following hook invocation (seq + 1), so the attribution is **not** a sampling artifact: the pop consuming `0x3C10E34` writes ESI, and the pop consuming `0x3C10E38` does not.

Cycle evidence for the governor's gate: control profile stops at status 13 `0xFD800044` (339/405); experiment stops at status 10 `0x6B6A6968` (346/414) — the stop has **not** moved past the fault, as expected before any repair. Build 158 includes profile 87 and the only `FF_MEMORY_POP` definition (`src/runtime/recomp_memory_access.h:54`), and that text is the correct reverse. The remaining question is therefore a compiled-code/register-identity discrepancy between the correct emitted text and the executed mapping; it is recorded for the translator owner with this adjacent-attribution evidence. No fix, no model.

No DECISIONS row: attribution evidence for D401.

## 46. Original binary matches the emitted text (REA read-only)

Read-only Ghidra check of the retained XBE (source SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`):

- `CDevice_FreeFrameBuffers` prologue (`0x1FB640..0x1FB643`): `PUSH EBX; PUSH EBP; PUSH ESI; PUSH EDI; MOV ESI,ECX`.
- Epilogue (`0x1FB72E..0x1FB732`): `POP EDI; POP ESI; POP EBP; POP EBX; RET`.

The original binary, the emitted `.inc` text and the tested rule therefore all agree: the correct restore order is `edi, esi, ebp, ebx` after pushes `ebx, ebp, esi, edi`. The observed runtime consumer (ESI takes the saved-EDI slot) is inconsistent with all three, and the attribution is adjacent-proven. The remaining discrepancy is in the **compiled-code/register-identity layer** (the built object's register binding), not in the original bytes, the emitted order, or the sampling.

Formal record for the translator owner: the repair question is now "why does the compiled module bind the `POP EDI` slot to `esi` when the original bytes, the emitted text and `ff_reg_order_is_reverse` all require `POP EDI`?" Evidence: this section, §45 attribution table, module `ee284034...`, reports `browser-157-control.json` / `browser-157-experiment.json`. No fix, no translator edit.

No DECISIONS row: evidence for D401.

## 47. Compiled-artifact verdict: the owned macro pattern is LIFO-correct

A minimal source-owned reproducer (`tests/reg_save_compiled_probe.c`) mirrors the exact emitted sequence with the same macro shapes:

- pushes `ebx(0x11), ebp(0x22), esi(0x33), edi(0x44)`;
- pops `edi, esi, ebp, ebx` via `FF_MEMORY_POP`-style read-before-increment macros;
- correct LIFO restore is `edi=0x44, esi=0x33, ebp=0x22, ebx=0x11` (packed `0x44332211`).

Results:
- **gcc (native suite)**: the reproducer returns 1 (`lifo-correct`), registered as `reg_save_compiled_probe`; native suite **54/54**.
- **Emscripten standalone Wasm under Node**: `run_probe()` returns 1 (`lifo-correct`), so the compiled artifact preserves the emitted order.

Verdict: the owned macro/expansion files (`src/runtime/recomp_memory_access.h` and the emission pattern) are **exonerated** — the compiled minimal artifact does not reproduce the shift. The full-module divergence is therefore specific to the toolkit/lifter-level code generation or register bookkeeping outside the lane's owned files. This is the M6 gate's fuzzed truth: the stop does not move and the defect is not in the lane-owned expansion.

No fix, no model, no translator edit.

No DECISIONS row: evidence for D401.

No DECISIONS row: tooling characterization.

## 48. M3 pre-registration: cluster is reached; fault blocks the return path

Read-only check of the retained experiment report (`browser-157-experiment.json`): the graphics-init cluster functions are all in the reached set — `0x1F7650` (`Direct3D_CreateDevice`), `0x1FBB00` (device constructor), `0x1FE05A`, `0x1FE085`, `0x1FB3B0`, `0x1F54D0` (`D3DDevice_Release`), `0x1FC010`, and `0x1FB640` (`CDevice_FreeFrameBuffers`).

The current fault sits inside the cluster's **return/teardown path** (`D3DDevice_Release → FUN_001FC010 → FUN_001FB640`), not at an unadmitted function or an unsupported service. M3's post-repair acceptance is therefore pre-registered as:

1. the stop must move past `0x6B6A6968` in **both** named profiles (`browser-157-control.json`, `browser-157-experiment.json`, module `ee284034...`);
2. the next observed request/system must appear in the reports (exact address/service recorded, not inferred);
3. `[ESI+0x47C]` must read the device field (`0x20400C`) as part of a normal cluster return.

Repair ownership is with the translator owner (candidate B); this lane makes no translator, toolkit, or fixture change. No code change is needed for M3 pre-registration.

No DECISIONS row: evidence only.

## 49. M2 policy application statistics (device log)

From `browser-157-experiment.json` (`deviceRegisters.log`, 12,637 entries):

| Metric | Value |
| --- | --- |
| Reads | 161 |
| Writes | 12,476 |
| Reads with `beforeKnown` | 161 (100%) |
| Reads of previously written addresses | 68 |
| Reads of unknown addresses | **0** |
| Distinct device addresses | 5,298 |
| Status waits | 0 |

The policy is applied exactly as recorded in D402: every device read in the run is of an address previously written in the same run, and no unknown-address read occurred (the current fault at `0x6B6A6968` is a **guest-memory** read, not a device-register read). This keeps the M2 device-access evidence current while the translator-owned repair (candidate B) is pending.

No DECISIONS row: evidence only.

## 50. M2 policy in both profiles (control and experiment)

Same computation on the control profile (`browser-157-control.json`, module `ee284034...`):

| Metric | Control | Experiment |
| --- | --- | --- |
| Log entries | 12,592 | 12,637 |
| Reads | 137 | 161 |
| Reads `beforeKnown` | 137 (100%) | 161 (100%) |
| Reads of previously written addresses | 54 | 68 |
| Unknown-address reads | **0** | **0** |
| Writes | 12,455 | 12,476 |
| Distinct addresses | 5,298 | 5,298 |
| Status waits | 0 | 0 |

D402's device-access policy holds in **both** profiles with zero unknown-address reads, satisfying M2's "in both profiles" requirement independently of the pending candidate-(B) repair.

No DECISIONS row: evidence only.
## 51. Whitespace-gate verification (marker-class audit)

Audit of `git diff origin/main HEAD -- docs/DECISIONS.md` (head `e74b4d6`):

- No added line carries a conflict-marker-class sequence (`<<<`, `======`, `>>>`). The only marker-like runs in the file are ordinary JavaScript snippets inside the pre-existing D217 row (`eaxAfter === eaxBefore`, `espAfter === (espBefore + ...`), which are present in `origin/main` and were not added by this lane.
- `git diff --check origin/main HEAD` returns **exit 0**; the whitespace gate passes.
- No rewrite or amend is applicable: the requested marker-class cell does not exist in the added lines. Head `e74b4d6` is clean for queueing.

No code or decision change; no amend performed because there is nothing to rewrite.
