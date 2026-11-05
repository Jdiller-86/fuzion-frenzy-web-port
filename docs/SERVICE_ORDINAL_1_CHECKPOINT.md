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
