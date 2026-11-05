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
