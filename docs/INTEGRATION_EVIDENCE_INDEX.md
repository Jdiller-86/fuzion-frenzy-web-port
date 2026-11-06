# Integration evidence index and hold

November 4, 2026. This index hands off the corrected report/helper slice and the retained admission evidence for the architect's exact-head review. It is not a merge or promotion request. The source lane is parked; the last source/CI commit is `3ef6024`, after the Sol architect's `09:30` decision to hold further admission pending that review. Later commits are docs-only hold records. The exact-head review package for the corrected candidate is [the candidate review](INTEGRATION_CANDIDATE_REVIEW.md).

## Latest: faulting read located at 0x1FC091; object is not the static device

- [Ordinal-1 checkpoint §13](SERVICE_ORDINAL_1_CHECKPOINT.md#13-faulting-read-characterization-read-only-diagnostic-probe): default-off `guestWatch` diagnostic (first write/read site per address; VM fault record now carries the access site). Fault is a **read at site `0x1FC091`** (`MOV EDX,[EAX]` in `FUN_001FC010`) of `0x6B6A6968`; `EAX` from `[ESI+0x47C]` at `0x1FC087`. `D3D_g_pDevice` (`0x206720`) has exactly one write — site `0x1F7676` in `Direct3D_CreateDevice`, value `0x203B90`, refcount 1 at `0x203F98` — and `0x20400C` (device `+0x47C`) has **zero reads and zero writes**, so the faulting object is not the static device (or the read bypasses the instrumented path); the writer is not identified. Module 130 (724,074 bytes, profile 87); stop remains **status 10 at `0x6B6A6968`, 346 routines / 414 kernel ops**.
- **Integration facts:** published lane head `36bf5f7`; integration head `864d33c` contains the lane commits through `36bf5f7`; main `3a5be52`. D374/D376 already record the models/admissions, so this diagnostic adds no new row.

## Prior: contiguous-free model passes; next stop was a guest read fault

- [Ordinal-1 checkpoint §12](SERVICE_ORDINAL_1_CHECKPOINT.md#12-ordinal-171-contiguous-free-model-and-the-next-stop): architect-authorized default-off ordinal-171 `MmFreeContiguousMemory` model (exact mapped/aligned one-argument shape, EAX 0, `esp + 8`, no guest write, declaration-level-only disclosure; the observed address is diagnostics, not the predicate). Module 126 (profile 87, updated sources): 723,629 bytes, SHA `6849f0b7...`. Control 339/405; inert 340/406 at ordinal 3; experiment accepted two frees (`0x037F2000`, `0x03920000`; write-map `47 -> 47`; EAX 0; `esp + 8` each) and reached **status 10 (guest read fault, access 1) at `0x6B6A6968`, 346 routines / 414 kernel ops** — `MOV EAX,[ESI+0x47c]` then `MOV EDX,[EAX]` at `0x001FC091` in `FUN_001FC010`, where the device field `+0x47C` holds ASCII `"hijk"`.
- **No DECISIONS row:** integration head still lacks the lane commits.

## Prior: FUN_001fc910 admitted; next stop was MmFreeContiguousMemory

- [Ordinal-1 checkpoint §11](SERVICE_ORDINAL_1_CHECKPOINT.md#11-admission-of-fun_001fc910-and-the-next-service-stop): `0x001FC910` is `FUN_001fc910` (98 bytes, one stack argument, D3D format helper). Admitted as one bounded increment via a read-only Ghidra export of `0x00042BE0` + `0x001F54D0` + `0x001FC910` replacing the previous two-function seed; profile 87 has 463 functions / 18,990 instructions. `verify-seeds.py` 32 recorded / 32 offered / 0 unexpected / 0 missing; translation check 554 functions, 0 needing rewriting. Module 125 (profile 87, same sources): 722,950 bytes, SHA `f77f6066...`. Control 339/405; inert 340/406 at ordinal 3; experiment **status 1 at `0xFE000004`, ordinal 171**, **346/412** — `MmFreeContiguousMemory(0x037F2000)` from `CDevice_FreeFrameBuffers` (return `0x001FB6D9`), refused pending a bounded allocator-service decision.
- **No DECISIONS row:** integration head still lacks the lane commits.

## Prior: null-result model generalized; next stop was placeholder 0x1FC910

- [Ordinal-1 checkpoint §10](SERVICE_ORDINAL_1_CHECKPOINT.md#10-generalized-null-result-admission-and-the-next-translation-stop): the pinned HAL tests `Result` before its option switch, so the model now accepts any declared `Result == 0` (nonzero Result stays refused). Native tests cover both observed pairs plus a mechanism-only tuple. Module 124 (profile 86, updated sources): 722,209 bytes, SHA `f9b29aee...`. Control 339/405; inert 340/406 at ordinal 3; experiment accepted both null calls (last `0xFD000000`, option 9, param 1, Result 0; write-map `47 -> 47`; checksum unchanged; EAX unchanged; `esp + 20`) and reached **status 2 at `0x001FC910`, 344 routines / 412 kernel ops** — an unadmitted `FUN_001fc910` placeholder (~97 bytes, one stack argument).
- **No DECISIONS row:** integration head still lacks the lane commits, so the sync-first rule blocks the row.

## Prior: D3DDevice_Release admitted; next stop was another NULL-Result AV call

- [Ordinal-1 checkpoint §9](SERVICE_ORDINAL_1_CHECKPOINT.md#9-admission-of-d3ddevice_release-and-the-next-stop): `0x001F54D0` is `D3DDevice_Release` (18 instructions; refcount at `[device+0x408]`; teardown calls `0x001FC010` and clears `0xAE4` dwords). Admitted as one bounded translation increment via a read-only Ghidra export of `0x00042BE0` + `0x001F54D0` merged into the retained 32-seed set, with `--guard-memory`; profile 86 has 462 functions / 18,958 instructions and agreeing original bytes/decoder instructions. Module 123 (same probe sources): 722,259 bytes, SHA `9f883b09...`. Control 339/405; inert 340/406 at ordinal 3; experiment passes Release (+4 routines) and reaches **status 1 at `0xFE000094`, ordinal 2**, **344/411** — `AvSendTVEncoderOption(0xFD000000, 9, 1, NULL)` from `CDevice_FreeFrameBuffers` (return `0x001FB664`), refused because the tuple differs from the authorized exact shape.
- **No DECISIONS row:** integration head `6bc05f7` contains `c8c965c` but not the lane commits, so the sync-first rule still blocks the row.

## Prior: null-result AV model passes; next stop was translation placeholder 0x1F54D0

- [Ordinal-1 checkpoint §8](SERVICE_ORDINAL_1_CHECKPOINT.md#8-bounded-null-result-model-authorized-and-the-next-stop): architect-authorized default-off model for the observed third ordinal-2 call, exact tuple (`0x03920000`, option 9, param 0, Result NULL) with nonzero Result refused. Module 121 (721,389 bytes, SHA `4ec6816d...`). Control 339/405 at `0xFD800044`; inert 340/406 at ordinal 3; experiment accepted with no write (write-map `47 -> 47`, frame checksum unchanged), EAX `0 -> 0`, `esp` +20, then **status 2 at `0x001F54D0`, 340 routines / 410 kernel ops**. That target is an unadmitted translation placeholder reached by `ff_title_00042BE0` tail-jumping there after `D3DDevice_Present`; admission needs a new translation/seed step.
- **No DECISIONS row:** integration head `6bc05f7` contains `c8c965c` but not `3d4cf7d`/`bfe9dec`/`753711f`/`eb685e0`, so the sync-first rule still blocks the row.

## Prior: ordinal-1 saved-data model passes; NULL-Result AV query was next

- [Ordinal-1 checkpoint §6-§7](SERVICE_ORDINAL_1_CHECKPOINT.md#6-bounded-fixed-0-model-guard-correction-and-pair): architect-authorized default-off fixed-0 `AvGetSavedDataAddress` model. The authorized static no-setter guard was proven unsatisfiable — the retained profile binds `{0x0025345C, 4u}` and that call site is in Present's nonzero saved-data branch — and the architect replaced it with a runtime invariant: accept only when the recorded journal has no prior ordinal-4 dispatch, fail closed on an unreadable journal. Module 120 (720,149 bytes, SHA `447c8073...`). Control 339/405 at `0xFD800044`; inert 340/406 at ordinal 3; experiment accepted (`priorSetter 0`, value 0, EAX 1→0, ESP unchanged) and advanced to **status 1 at `0xFE000094`, ordinal 2**, **340/409**. Next stop: `AvSendTVEncoderOption(0x03920000, 9, 0, NULL)` — documented `AV_OPTION_QUERY_AV_CAPABILITIES` with a NULL Result, which the pinned HAL's `if (!Result) return;` makes a log-and-return with no write. Characterized read-only, not modeled. Setter and nonzero path unmodeled.
- **No DECISIONS row:** integration head `6bc05f7` contains `c8c965c` but not `3d4cf7d`/`bfe9dec`/`753711f`, so the sync-first rule still blocks the row.

## Prior: ordinal-1 read-only characterization

- [Ordinal-1 checkpoint](SERVICE_ORDINAL_1_CHECKPOINT.md): ordinal 1 = `AvGetSavedDataAddress`, no arguments, pinned static return `g_av_saved_data_address` = **0** (no `AvSetSavedDataAddress` call). `D3DDevice_Present` calls it at `0x202284` (return `0x20228A`) and skips the saved-data block when zero. Next: model the documented 0 return with the exact no-argument shape, or hold. **No DECISIONS row this cycle:** the published integration head `c8c965c` (source `5d7ebf5`) predates `3d4cf7d` and is not an ancestor; merge denied, no fast-forward possible.

## Prior: unknown AV option model passes; ordinal-1 frontier

- [Second-query checkpoint §5](SERVICE_ORDINAL_2_SECOND_QUERY_CHECKPOINT.md#5-bounded-unknown-option-model-authorized): architect-authorized default-off sibling model (exact tuple, one zero write, EAX untouched, ESP +20). Module 118 (719,152 bytes, SHA `0b24204e...`). Control 339/405 at `0xFD800044`; inert 340/406 at ordinal 3; experiment accepted the late call (`0xFD000000`, option `0x0F`, written 0, EAX unchanged, ESP +20) and advanced to **status 1 at `0xFE000098`, ordinal 1**, 340/408. This is `D370`.
## Latest: later ordinal-2 unknown-option characterization (read-only)

- [Second-query checkpoint](SERVICE_ORDINAL_2_SECOND_QUERY_CHECKPOINT.md): `D3DDevice_Present` calls `AvSendTVEncoderOption` again at `0x202268` with `0xFD000000`, option `0x0F`, `0`, Result pointer. The pinned HAL handles options `0x01`-`0x0E` and documents the unknown-option default as writing 0 through `Result` without failing; the synthetic adapter only accepts the startup `(0,6,0,Result)` query, so this call is denied (status 1). Next: model the documented unknown-option default with the exact shape, or hold. This is `D369`.
## Latest: ordinal-3 display-mode declaration model

- [Ordinal-3 checkpoint §6](SERVICE_ORDINAL_3_CHECKPOINT.md#6-bounded-declaration-model-authorized): architect-authorized default-off declaration model (exact tuple, no descriptor decode, no writes, success as declaration acceptance). Module 117 (718,230 bytes, SHA `05f9ecad...`). Control 339/405 at `0xFD800044`; inert 340/406 at ordinal 3; display run accepted `mode 0x60030307`, `pitch 0xA00`, `fb 0x03920000`, window `0x83920000` computed-not-applied, and advanced to a later ordinal-2 query at `0xFE000094`, 340/407. The earlier mode value `0x600302C7` is corrected. This is `D368`.
## Latest: service ordinal 3 characterization (read-only)

- [Ordinal-3 checkpoint](SERVICE_ORDINAL_3_CHECKPOINT.md): ordinal 3 = `AvSetDisplayMode(addr, step, mode, format, pitch, fb)`, returning 0; called from `D3DDevice_Present` after the vblank wait (`0x20222B`, return `0x202231`) with `addr 0x3C10CFC`, step 0, mode `0x600302C7`, format 0, pitch `0xA00`, fb `0x03920000`. The pinned bridge records the scanout state, resolves the fb VA into the contiguous window, starts the window and returns success. Next: model the documented display state or hold. This is `D367`.
## Latest: vblank wait model passes; ordinal-3 frontier

- [Wait-model checkpoint](VBLANK_WAIT_MODEL_CHECKPOINT.md#type-gate-correction-and-result): architect-accepted correction decodes the full event header (low byte 0/1, Size byte 4) and consumes only Type 1; the title's notification event `0x00040000` is accepted. Module 115 (717,097 bytes, SHA `c7b94e38...`). Control unchanged (339/405, status 13); experiment (consumer + 3 vblanks) decision 0, declared 3 -> 2, the wait exits, and the run advances to **status 1 at `0xFE0000A4`, ordinal 3**, 340/406, return `0x202231` in `D3DDevice_Present`'s submit loop. Supersedes D364's type-1 restriction and D365's flag reading for this object. This is `D366`.
## Latest: ordinal-159 wait object characterization (read-only)

- [Vblank object checkpoint](VBLANK_OBJECT_CHECKPOINT.md): object `0x00206080` = D3D device `0x00203B80` + `0x2500`, first dword `0x00040000`, `+4` word 0; static writers treat it as a flag/state field, not a KEVENT. The event gate refusal is confirmed; modeling it as an event would be an assumption. Next: initializer/signal characterization, a D3D-specific path, or hold. This is `D365`.
## Latest: ordinal-159 wait model (default-off, gate refused)

- [Vblank wait model checkpoint](VBLANK_WAIT_MODEL_CHECKPOINT.md): architect-authorized default-off model (`pcrtcVblanks` pool, exact five-argument guard, pinned event gate). Module 114 (717,065 bytes, SHA `b3ec6b2b...`). Control unchanged (339/405, status 13); experiment (consumer + 3 vblanks) drains 499 words, releases the fence 3 -> 9, then calls ordinal 159 once with object type `0x40000`, signal 0 → decision 3 (EVENT). Gate refused, no writes, stop unchanged (status 1, `0xFE000094`, ordinal 159, 340/405). The object is not the pinned KEVENT; next is a read-only characterization. This is `D364`.
## Latest: service ordinal 159 characterization (read-only)

- [Ordinal-159 checkpoint](SERVICE_ORDINAL_159_CHECKPOINT.md): ordinal 159 (`0x9F`) = `KeWaitForSingleObject` (5 arguments, NTSTATUS); `D3DDevice_BlockUntilVerticalBlank` calls it with Object `0x00206080`, WaitReason 6, WaitMode 1, Alertable 0, Timeout NULL, retrying while nonzero. The pinned bridge treats the guest event SignalState as truth and delivers vblank via PCRTC_INTR_0 bit 0 + PMC_INTR_0 bit 24; the current profile delivers none, so no modeled success is permitted. Branch fast-forwarded to `1e35f8a` (main ancestry restored). This is `D363`.
## Latest: PFIFO responder reaches service 159

- [Completion-flag checkpoint §7](PFIFO_COMPLETION_FLAG_CHECKPOINT.md): architect-authorized default-off responder (drain to PUT + one-shot sequence release; latch unmodeled). Module 112 (716,327 bytes, SHA `0304f2f6...`). Control unchanged (status 13 at `0xFD800044`, 339/405); consumer drained 173 packets / 499 words, released the fence 3 -> 9, exited the wait, and stopped at status 1, `0xFE000094`, ordinal 159 (unsupported service), 340/405. This is `D354`.
## Latest: PFIFO completion-flag characterization (read-only)

- [The completion-flag checkpoint](PFIFO_COMPLETION_FLAG_CHECKPOINT.md): the post-consumption wait polls the dword at **`0x03BFF000`** (`[context 0x00203B90 + 0x3F0]`) from `D3D_BlockOnTime` at PC `0x1FB0D4`, looping until `fence >= target`. Observed fence 3, context sequence 9, GET `0x03B7D008`, PUT `0x03B7D7CC`. The value comes from the GPU semaphore release (`0x1D70`) with the kick marker `0x0310`/PGRAPH `0x400B10` latch; a responder must consume the stream and publish the release. No implementation, hook or admission. This is `D353`.

## Prior: PFIFO boundary design note (design-only)

- [The PFIFO boundary design note](PFIFO_BOUNDARY_DESIGN_NOTE.md): validates the default-off pusher against `pfifo-decode.mjs`, `pfifo-ring.mjs`, `pfifo_channel.c`, `pfifo_status.c` and `pfifo_dma.c`; records the exact MMIO sequence, the GET-site order, the control/consumer observations, and the minimal responder requirements. No hook, guest connection, admission, export, seed or translation change.
- Checks: `test:native` 36/36, PFIFO JS component tests 19/19, `test:runtime` 197/0. Control remains 339/405 at `0xFD800044`; the consumer run still ends in a generic `RuntimeError: unreachable` trap with no classified stop. This record is `D349`.
- The retained control device log pins the GET-site sequence: 93 PFIFO accesses, GET served as the ring base (`0x03B7D000`) while PUT equals the base, status LOW_MARK/LOW_MARK and DMA_PUSH=1, then submissions at `0x03B7D7B4`/`0x03B7D7CC` and the unlogged status-13 stop. At the consumer site: base `0x03B7D000`, GET `0x03B7D000`, PUT `0x03B7D7CC`, span just under 2 KiB. This refinement is `D350`.
- Module 108 scopes the VM budget to real call sites (excluding probe reads): control still 339/405 at `0xFD800044`, consumer still the same generic trap, so the trap is not a probe read consuming the budget. The worker trap classification remains the open gap. This fix is `D351`.
- Module 110 completes the evidence path (post-run disarm; reset-seam consumer updates allowed when declared): the consumer run now classifies as **status 13 at `0x03BFF000`** (guest RAM), 339/405, classification **8 (budget)**, `budgetHit 1`, `steps 1`, `words 2`, GET `0x03B7D008`, PUT `0x03B7D7CC`, `vmChecks 500000`; one packet consumed, then a guest-memory completion-flag poll. This is `D352`.

## Prior: PFIFO pusher prototype (default-off)

- [PFIFO consumer prototype](PFIFO_CONSUMER_PROTOTYPE_CHECKPOINT.md): new `src/runtime/pfifo_pusher.c/.h` with a native fixture (36/36 component runs, optimized + ASan/UBSan), `pfifoConsumer` profile input, report state and validator, and a gated 64-step hook at the status-13 DMA_GET site. No admission, export or seed change.
- Control on module 107 (715,740 bytes, SHA `03f94490...`): status 13 at `0xFD800044`, 339/405, no consumer state — the default is unchanged.
- The opt-in consumer run originally wrote no report and did not terminate within 300 s (an earlier attempt exceeded 900 s); the 64-step GET budget did not fire, so the run passed the PFIFO wait. A 500,000-access VM budget was then added: the consumer now writes `{"kind":"error","message":"RuntimeError: unreachable"}` — a stop trap surfaced as a generic error, with no classified stop or consumer state. The consumer remains unproven and default-off. The next step is a worker-level fix that classifies an expected stop trap, then re-run. This record is `D348` (prototype `D347`).

## Prior: 0x202150 read-only characterization

- [Present characterization](PRESENT_STATIC_CHARACTERIZATION_CHECKPOINT.md): no admission, export or seed change. `0x202150` (`D3DDevice_Present`) is `body 0x202150..0x2022D0`, 115 instructions / 385 bytes, 115/115 decoded, one `RET 0x10`, 20 basic blocks, no tables; two loops (fence wait, submit retry).
- Conflict recorded: Present and its three untranslated direct callees were already admitted in `7dae2e4` under the previous architect-subagent authorization, so the directive's post-table/no-admission premise is stale. Seven direct callees classified: `D3D_SetFence`, `0x1FACA0`, `CDevice_KickOff`, `D3D_KickOffAndWaitForIdle` pre-existing; `0x201E80`, `0x202060`, `0x1F5520` added in translation 83. Nine calls use seven kernel-ordinal pointer sites (`0x80000001/2/3/4/0xAB/0xB2/0xB4`), not code. Caller `0x42C5E` in `FUN_00042be0` builds a 640×480 format-7 descriptor first.
- The retained run still stops at status 13, `0xFD800044` (PFIFO MMIO), `stopReason: "pushbuffer consumption required"`: the next requirement is a runtime pushbuffer consumer, not translation. No ambiguous boundary was found. This record is `D346`.

## Prior: 0x202150 present admission and PFIFO boundary

- [The 0x202150 batch](FRONTIER_DEP_202150_CHECKPOINT.md): translation 83 (461 functions / 18,940 instructions), module 104 (714,276 bytes, SHA `ff29e855...`), control 279/404 (status 1, ordinal 2) vs experiment 339/405 (status 13 at `0xFD800044`, ordinal 1), comparator exit 0 (+60 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 554/0, package verifier 46/46 digests, frontier 554/93/92.
- The post-table region decision was escalated once to the project architect, which authorized one bounded set. `0x202150` (`D3DDevice_Present`) was **already established**, so no create-function was made; Present plus its three untranslated immediate direct callees (`0x201E80`, `0x202060`, `0x1F5520`) were admitted — 4 functions, no transitive expansion — with an existing 2-function export folded into the 6-function seed to keep 32 exports.
- The status-2 stop at `0x202150` is gone. The new stop is a **PFIFO MMIO register**, not code (`stopReason: "pushbuffer consumption required"`): the title submitted pushbuffer commands and is waiting for consumption, and the synthetic harness has no consumer. PFIFO pushbuffer consumption is the next fresh architect decision. This batch is `D345`.

## Prior: 0x1F65F0 table-coverage completion

- [The 0x1F65F0 batch](FRONTIER_DEP_1F65F0_CHECKPOINT.md): translation 82 (457 functions / 18,626 instructions), module 103 (705,795 bytes, SHA `6e298d44...`), table coverage **28/28**, control 279/404 at ordinal 2 vs experiment 332/405 at post-table `0x00202150`, comparator exit 0 (+53 routines / +1 operation, unchanged from module 102), `verify-seeds` 32/32, `verify-translation` 547/0, package verifier 45/45 digests.
- `0x1F65F0` (`D3DDevice_SetRenderState_MultiSampleType`, index 134) is never set by this profile and was the only untranslated table handler. It needed one bounded create-function at that address only; the 9-instruction / 37-byte leaf calls the already-translated `0x1F5640`.
- The merged seed hit the 32-function-per-export limit; two existing single-function exports were regrouped into one 2-function export, keeping **32 exports**. The post-table `D3DDevice_Present` stop is still unadmitted and the next region remains a fresh architect decision. This batch is `D344`.

## Prior: 0x1F7570 admission and render-state table completion

- [The 0x1F7570 batch](FRONTIER_DEP_1F7570_CHECKPOINT.md): translation 81 (456 functions / 18,617 instructions), module 102 (705,414 bytes, SHA `27ceaa9e...`), control 279/404 at ordinal 2 vs experiment 332/405 at post-table `0x00202150`, comparator exit 0 (+53 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 546/0, package verifier 43/43 digests.
- `0x1F7570` (index 142) and `0x1F7590` (index 143) were the last render-state handlers; each needed one bounded create-function at that address only. Both are 4-instruction / 17-byte wrappers calling the already-admitted `0x1F7230`.
- **The render-state table is complete.** The new stop is status 2 at `0x00202150`, the code symbol `D3DDevice_Present`, called from `0x42C5E` in `FUN_00042be0`. It was not admitted; the next region is a fresh architect decision. This batch is `D343`.

## Prior: 0x1F7550 admission

- [The 0x1F7550 batch](FRONTIER_DEP_1F7550_CHECKPOINT.md): translation 80 (454 functions / 18,609 instructions), module 101 (705,079 bytes, SHA `cfb7b918...`), control 279/404 at ordinal 2 vs experiment 324/405 at untranslated `0x1F7570`, comparator exit 0 (+45 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 544/0, package verifier 41/41 digests.
- `0x1F7550` (`D3DDevice_SetRenderState_RopZCmpAlwaysRead`, table index 141) was a named symbol without a function; one bounded create-function at that address only made it admissible. The 4-instruction / 17-byte seed (`610fda82...`) stores its argument and calls `0x1F7230`, an already-established 45-instruction / 200-byte function (`2038c102...`) that calls translated `0x1FB3B0`.
- The render-state table still does **not** end here: index 143 (`0x1F7590`) remains and index 144 onward is non-pointer data, so `0x1F7570` is an ordinary in-region stop and no post-table region is declared. This batch is `D342`.

## Prior: 0x1F74F0 admission

- [The 0x1F74F0 batch](FRONTIER_DEP_1F74F0_CHECKPOINT.md): translation 79 (452 functions / 18,560 instructions), module 100 (703,605 bytes, SHA `c0300987...`), control 279/404 at ordinal 2 vs experiment 322/405 at untranslated `0x1F7550`, comparator exit 0 (+43 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 542/0, package verifier 39/39 digests.
- `0x1F74F0` (`D3DDevice_SetRenderState_StencilCullEnable`, table index 140) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 26-instruction / 92-byte seed (`8b53384b...`) calls already-translated `0x1FB3B0`, has internal branches, and ends in `RET 4`. The render-state table does **not** end at index 140 (indices 141-143 remain), so `0x1F7550` is an ordinary in-region stop.
- Published architect `d4e0107` was merged at `a4c7f7c` before this row, with the `D339` collision resolved by keeping the architect merge row and renumbering the `0x1F7490` admission to `D340`; this batch is `D341`.

## Prior: 0x1F7490 admission

- [The 0x1F7490 batch](FRONTIER_DEP_1F7490_CHECKPOINT.md): translation 78 (451 functions / 18,534 instructions), module 99 (702,974 bytes, SHA `29e93797…`), control 279/404 at ordinal 2 vs experiment 321/405 at untranslated `0x1F74F0`, comparator exit 0 (+42 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 541/0, package verifier 38/38 digests.
- `0x1F7490` (`D3DDevice_SetRenderState_OcclusionCullEnable`, table index 139) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 26-instruction / 92-byte seed (`7dfc9582…`) calls already-translated `0x1FB3B0`, has internal branches, and ends in `RET 4`. `0x1F74F0` is the next ordinary stop (table index 140).
- Main ancestry remains restored: PR #7 merged at `42287210`, source merged it at `a0be99b`, and `8c49a22` cleared the publication whitespace gate. Rows run `D327` (main) then `D328`–`D340` (primary; `D339` is the architect merge row).

## Prior: 0x1F7460 admission

- [The 0x1F7460 batch](FRONTIER_DEP_1F7460_CHECKPOINT.md): translation 77 (450 functions / 18,508 instructions), module 98 (702,324 bytes, SHA `60d2a65f…`), control 279/404 at ordinal 2 vs experiment 320/405 at untranslated `0x1F7490`, comparator exit 0 (+41 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 540/0, package verifier 37/37 digests.
- `0x1F7460` (`D3DDevice_SetRenderState_YuvEnable`, table index 138) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 12-instruction / 37-byte seed (`0b8d447b…`) calls already-translated `0x1FB3B0` and `0x1F7080`, and ends in `RET 4`.

## Prior: 0x1F6760 admission

- [The 0x1F6760 batch](FRONTIER_DEP_1F6760_CHECKPOINT.md): translation 76 (449 functions / 18,496 instructions), module 97 (701,883 bytes, SHA `c7eb34ab…`), control 279/404 at ordinal 2 vs experiment 319/405 at untranslated `0x1F7460`, comparator exit 0 (+40 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 539/0, package verifier 36/36 digests.
- `0x1F6760` (`D3DDevice_SetRenderState_ShadowFunc`, table index 135) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 13-instruction / 47-byte seed (`3a4e953d…`) calls already-translated `0x1FB3B0` and ends in `RET 4`.

## Prior: 0x1F7600 admission

- [The 0x1F7600 batch](FRONTIER_DEP_1F7600_CHECKPOINT.md): translation 75 (448 functions / 18,483 instructions), module 96 (701,399 bytes, SHA `8da3c4b5…`), control 279/404 at ordinal 2 vs experiment 318/405 at untranslated `0x1F6760`, comparator exit 0 (+39 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 538/0, package verifier 35/35 digests.
- `0x1F7600` (`D3DDevice_SetRenderState_MultiSampleMask`, table index 133) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 22-instruction / 76-byte seed (`68d64fb8…`) calls already-translated `0x1F82C0`, `0x1FB3B0` and `0x1F6EB0`, and ends in `RET 4`.

## Prior: 0x1F75B0 admission

- [The 0x1F75B0 batch](FRONTIER_DEP_1F75B0_CHECKPOINT.md): translation 74 (447 functions / 18,461 instructions), module 95 (700,698 bytes, SHA `286daba7…`), control 279/404 at ordinal 2 vs experiment 317/405 at untranslated `0x1F7600`, comparator exit 0 (+38 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 537/0, package verifier 34/34 digests.
- `0x1F75B0` (`D3DDevice_SetRenderState_MultiSampleAntiAlias`, table index 132) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 22-instruction / 76-byte seed (`d75806bf…`) calls already-translated `0x1F82C0`, `0x1FB3B0` and `0x1F6EB0`, and ends in `RET 4`.

## Prior: 0x1F6730 admission

- [The 0x1F6730 batch](FRONTIER_DEP_1F6730_CHECKPOINT.md): translation 73 (446 functions / 18,439 instructions), module 94 (700,022 bytes, SHA `e9b108f2…`), control 279/404 at ordinal 2 vs experiment 316/405 at untranslated `0x1F75B0`, comparator exit 0 (+37 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 536/0, package verifier 33/33 digests.
- `0x1F6730` (`D3DDevice_SetRenderState_EdgeAntiAlias`, table index 131) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 13-instruction / 44-byte seed (`fe75b542…`) calls already-translated `0x1FB3B0` and ends in `RET 4`.

## Prior: 0x1F6A70 admission

- [The 0x1F6A70 batch](FRONTIER_DEP_1F6A70_CHECKPOINT.md): translation 72 (445 functions / 18,426 instructions), module 93 (699,510 bytes, SHA `855ce4ec…`), control 279/404 at ordinal 2 vs experiment 315/405 at untranslated `0x1F6730`, comparator exit 0 (+36 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 535/0, package verifier 32/32 digests.
- `0x1F6A70` (`D3DDevice_SetRenderState_LogicOp`, table index 130) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 22-instruction / 76-byte seed (`187d2bdb…`) calls already-translated `0x1FB3B0` and has two return sites.

## Prior: 0x1F69F0 admission

- [The 0x1F69F0 batch](FRONTIER_DEP_1F69F0_CHECKPOINT.md): translation 71 (444 functions / 18,404 instructions), module 92 (698,963 bytes, SHA `57e245de…`), control 279/404 at ordinal 2 vs experiment 314/405 at untranslated `0x1F6A70`, comparator exit 0 (+35 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 534/0, package verifier 31/31 digests.
- `0x1F69F0` (`D3DDevice_SetRenderState_ZBias`, table index 129) was already Ghidra-established; its 41-instruction / 116-byte seed (`a0e4178d…`) calls translated `0x1F66A0` five times, performs the FPU sequence, and ends in `RET 4`.

## Prior: 0x1F68C0 admission

- [The 0x1F68C0 batch](FRONTIER_DEP_1F68C0_CHECKPOINT.md): translation 70 (443 functions / 18,363 instructions), module 91 (697,055 bytes, SHA `21ef3655…`), control 279/404 at ordinal 2 vs experiment 313/405 at untranslated `0x1F69F0`, comparator exit 0 (+34 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 533/0, package verifier 30/30 digests.
- `0x1F68C0` (`D3DDevice_SetRenderState_TextureFactor`, table index 128) was already Ghidra-established; its 25-instruction / 76-byte seed (`bc293f77…`) calls only translated `0x1FB3B0`, performs one `rep stosd` of 16 dwords, and has two return sites.

## Prior: 0x1F6850 tail-call batch

- [The 0x1F6850 batch](FRONTIER_DEP_1F6850_CHECKPOINT.md): translation 69 (442 functions / 18,338 instructions), module 90 (696,254 bytes, SHA `f74a2161…`), control 279/404 at ordinal 2 vs experiment 312/405 at untranslated `0x1F68C0`, comparator exit 0 (+33 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 532/0, package verifier 29/29 digests.
- `0x1F6850` (`D3DDevice_SetRenderState_FrontFace`, table index 126) was created with its original tail `JMP 0x1F67E0` preserved; `0x1F67E0` (`D3DDevice_SetRenderState_CullMode`, table index 127) was already established. Seeds `80bdcbee…` (14/52) and `2d5f7bf5…` (29/98) both preflight cleanly and were merged into the 18-function seed (`3bbc774a…`).

## Prior: 0x1F73F0 admission

- [The 0x1F73F0 batch](FRONTIER_DEP_1F73F0_CHECKPOINT.md): translation 68 (440 functions / 18,295 instructions), module 89 (694,956 bytes, SHA `4f64855e…`), control 279/404 at ordinal 2 vs experiment 310/405 at untranslated `0x1F6850`, comparator exit 0 (+31 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 530/0, package verifier 27/27 digests.
- `0x1F73F0` (`D3DDevice_SetRenderState_StencilFail`, table index 125) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 29-instruction / 105-byte seed (`856b5a10…`) calls already-translated `0x1FB3B0`, has internal branches, and preflights 29/29.

## Prior: 0x1F7370 admission

- [The 0x1F7370 batch](FRONTIER_DEP_1F7370_CHECKPOINT.md): translation 67 (439 functions / 18,266 instructions), module 88 (694,246 bytes, SHA `5440f6e0…`), control 279/404 at ordinal 2 vs experiment 309/405 at untranslated `0x1F73F0`, comparator exit 0 (+30 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 529/0, package verifier 26/26 digests.
- `0x1F7370` (`D3DDevice_SetRenderState_StencilEnable`, table index 124) was already Ghidra-established; its 37-instruction / 128-byte seed (`315b7776…`) calls only translated `0x1FB3B0`, has internal conditional branches, and preflights 37/37.

## Prior: 0x1F7300 admission

- [The 0x1F7300 batch](FRONTIER_DEP_1F7300_CHECKPOINT.md): translation 66 (438 functions / 18,229 instructions), module 87 (693,474 bytes, SHA `3f001885…`), control 279/404 at ordinal 2 vs experiment 308/405 at untranslated `0x1F7370`, comparator exit 0 (+29 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 528/0, package verifier 25/25 digests.
- `0x1F7300` (`D3DDevice_SetRenderState_ZEnable`, table index 123) was already Ghidra-established; its 34-instruction / 108-byte seed (`8552c475…`) calls only translated functions and preflights 34/34.

## Prior: 0x1F6890 admission

- [The 0x1F6890 batch](FRONTIER_DEP_1F6890_CHECKPOINT.md): translation 65 (437 functions / 18,195 instructions), module 86 (692,563 bytes, SHA `2a0ff87b…`), control 279/404 at ordinal 2 vs experiment 307/405 at untranslated `0x1F7300`, comparator exit 0 (+28 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 527/0, package verifier 24/24 digests.
- `0x1F6890` (`D3DDevice_SetRenderState_NormalizeNormals`, table `0x2025D8`, index 122) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 13-instruction / 45-byte seed (`67887947…`) calls already-translated `0x1FB3B0` and preflights 13/13.

## Prior: 0x1F6B50 admission

- [The 0x1F6B50 batch](FRONTIER_DEP_1F6B50_CHECKPOINT.md): translation 64 (436 functions / 18,182 instructions), module 85 (692,002 bytes, SHA `d136484b…`), control 279/404 at ordinal 2 vs experiment 306/405 at untranslated `0x1F6890`, comparator exit 0 (+27 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 526/0, package verifier 23/23 digests.
- `0x1F6B50` (`D3DDevice_SetRenderState_TwoSidedLighting`, table `0x2025D4`, index 121) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 24-instruction / 84-byte seed (`f520ecd5…`) calls already-translated `0x1FB3B0`, has one internal `JNE`, and preflights 24/24.

## Prior: 0x1F6B00 admission

- [The 0x1F6B00 batch](FRONTIER_DEP_1F6B00_CHECKPOINT.md): translation 63 (435 functions / 18,158 instructions), module 84 (691,173 bytes, SHA `e1f59842…`), control 279/404 at ordinal 2 vs experiment 305/405 at untranslated `0x1F6B50`, comparator exit 0 (+26 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 525/0, package verifier 22/22 digests.
- `0x1F6B00` (`D3DDevice_SetRenderState_BackFillMode`, table `0x2025D0`, index 120) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 22-instruction / 75-byte seed (`b126ca7b…`) calls already-translated `0x1FB3B0`, has one internal `JNE`, and preflights 22/22.

## Prior: 0x1F6AC0 admission

- [The 0x1F6AC0 batch](FRONTIER_DEP_1F6AC0_CHECKPOINT.md): translation 62 (434 functions / 18,136 instructions), module 83 (690,455 bytes, SHA `9a84bdc1…`), control 279/404 at ordinal 2 vs experiment 304/405 at untranslated `0x1F6B00`, comparator exit 0 (+25 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 524/0, package verifier 21/21 digests.
- `0x1F6AC0` (`D3DDevice_SetRenderState_FillMode`, table `0x2025CC`, index 119) was a named symbol without a function; one bounded persistent create-function at that address only made it admissible. The 18-instruction / 62-byte seed (`628ba62b…`) calls already-translated `0x1FB3B0`, has one internal `JNE`, and preflights 18/18.

## Prior: 0x1F6790 admission

- [The 0x1F6790 batch](FRONTIER_DEP_1F6790_CHECKPOINT.md): translation 61 (433 functions / 18,118 instructions), module 82 (689,834 bytes, SHA `b3c354bd…`), control 279/404 at ordinal 2 vs experiment 303/405 at untranslated `0x1F6AC0`, comparator exit 0 (+24 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 523/0, package verifier 20/20 digests.
- `0x1F6790` (`D3DDevice_SetRenderState_FogColor`, table `0x2025C8`, index 118) was already Ghidra-established; the 22-instruction / 71-byte seed (`3de7c154…`) calls already-translated `0x1FB3B0` and preflights 22/22.

## Prior: 0x1F6BB0 symbol-entry admission

- [The 0x1F6BB0 batch](FRONTIER_DEP_1F6BB0_CHECKPOINT.md): translation 60 (432 functions / 18,096 instructions), module 81 (689,176 bytes, SHA `d795973b…`), control 279/404 at ordinal 2 vs experiment 302/405 at untranslated `0x1F6790`, comparator exit 0 (+23 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 522/0, package verifier 19/19 digests.
- `0x1F6BB0` (`D3DDevice_SetRenderState_VertexBlend`, table `0x2025C4`, index 117) was established persistently at that address only; the 15-instruction / 50-byte seed (`70add3a6…`) calls already-translated `0x1FB3B0` and preflights 15/15 with no tables or normalizations.

## Prior: 0x1F7080 admission and observed branch

- [The admission checkpoint](FRONTIER_1F7080_ADMISSION_CHECKPOINT.md): translation 54 (423 functions / 17,620 instructions), module 75 (664,115 bytes, SHA-256 `687c34c9…`), control 279/404 at ordinal 2 vs experiment 286/405 at untranslated `0x1F70E0`, comparator exit 0 (+7 routines / +1 operation), `verify-seeds` 32/32/0/0, `verify-translation` 517/0.
- Executed `0x1F7080` is in the reached list; the observed taken branch is Path A (`0x1F70E0` needs `0x1FC980`+`0x1FCB70`). No Path A/Path B batch was started; the lane stops at the observed stop.
- [The Path A preflight](FRONTIER_PATH_A_PREFLIGHT_CHECKPOINT.md): `0x1F70E0` is 98 instructions / 329 bytes with four calls; only `0x1FC980` and `0x1FCB70` are untranslated (`0x1F66A0`/`0x1FB3B0` already translated); preflight 98/98. A proposed grouping keeps 32 exports (front seed 30→32 with `0x1F70E0`+`0x1FC980`; `0x1FCB70` merged into an existing seed), 426 functions / 17,760 instructions; no batch started.

- [The Path A admission](FRONTIER_PATH_A_ADMISSION_CHECKPOINT.md): translation 55 (426 functions / 17,779 instructions), module 76 (675,058 bytes, SHA `5d596ab2…`), control 279/404 at ordinal 2 vs experiment 289/405 at untranslated `0x1F6910`, comparator exit 0 (+10 routines / +1 operation), `verify-seeds` 32/32/0/0, `verify-translation` 517/0, package verifier 13/13 digests.
- The provisional acceptance's 35 offered seeds were corrected to the frozen 32-export limit; no tooling change was made. `0x1F70E0`/`0x1FC980` executed; `0x1FCB70` was translated but not reached. The next stop `0x1F6910` is ordinary; no further batch this cycle.

- [The 0x1F6910 dependency batch](FRONTIER_DEP_1F6910_CHECKPOINT.md): translation 56 (427 functions / 17,807 instructions), module 77 (676,813 bytes, SHA `0f41fd06…`), control 279/404 at ordinal 2 vs experiment 292/405 at untranslated `0x1F5B70`, comparator exit 0 (+13 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 518/0, package verifier 14/14 digests.
- The next stop is ordinary Path B (`0x1F5B70` + `0x1F6300` + translated `0x1FC980`); no batch this cycle. The provisional continuation remains `approved: false`.

- [The Path B admission](FRONTIER_PATH_B_ADMISSION_CHECKPOINT.md): translation 57 (429 functions / 18,038 instructions), module 78 (687,271 bytes, SHA `d6540c1b…`), control 279/404 at ordinal 2 vs experiment 296/405 at untranslated `0x1F6970`, comparator exit 0 (+17 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 519/0, package verifier 16/16 digests.
- `0x1F5B70`/`0x1F6300` executed; `0x1F6970` is the next ordinary stop. No further batch this cycle; the provisional continuation remains `approved: false`.

- [The 0x1F6970 dependency batch](FRONTIER_DEP_1F6970_CHECKPOINT.md): translation 58 (430 functions / 18,073 instructions), module 79 (688,315 bytes, SHA `8baa111d…`), control 279/404 at ordinal 2 vs experiment 300/405 at untranslated `0x1F6620`, comparator exit 0 (+21 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 520/0, package verifier 17/17 digests.
- The next stop `0x1F6620` is ordinary but not a Ghidra-established function (`established: false`). A read-only worklist characterizes it as an 8-instruction / 36-byte D3D leaf (one return `0x1F6641`; no calls, tails or unresolved branches). It is a named symbol reached through indirect table `0x2025C0` (index 116); a later bounded read-only in-memory definition exported a clean seed. It was admitted in translation 59/module 80; see the next entry.

- [The 0x1F6620 symbol-entry batch](FRONTIER_DEP_1F6620_CHECKPOINT.md): translation 59 (431 functions / 18,081 instructions), module 80 (688,623 bytes, SHA `cfc5786a…`), control 279/404 at ordinal 2 vs experiment 301/405 at untranslated `0x1F6BB0`, comparator exit 0 (+22 routines / +1 operation), `verify-seeds` 32/32, `verify-translation` 521/0, package verifier 18/18 digests.
- `0x1F6620` executed; `0x1F6BB0` is the next ordinary indirect render-state entry (table `0x2025C4`, index 117). No further batch this cycle.
- The approved architect directive then authorized a persistent create-function at `0x1F6620` only. A read-only re-open reports `FUNCTION_AT 001f6620`; the re-export is byte-identical (`55977f49…`), and the same 18/18 package verifies. No duplicate translation was run.
- Runner fix: `tools/Invoke-OpenCodeArchitect.ps1` now captures `$fullResponse` before the 1800-character `lastSummary` truncation and parses all six markers from it; `lastSummary`/1200 cap unchanged, PowerShell AST parse 0 errors, no other behavior change.

## Source lane

- Branch/head: `codex/opencode-source`, last source/CI commit `3ef6024` (`Run the full Python evidence suite in preparation CI`); docs-only hold records follow.
- Corrected report/helper and verification slice:
  - `3f97afa` truthful configured denials, exact success shape/address, paired call arguments, full supplied/declared input comparison
  - `35a664a` ordinal-2 denial pairs with the status-1 unsupported-service stop
  - `12deaad` shared `validateSuppliedDeviceProfile` in the report contract and comparison tool
  - `0ffb634` full declared input profile comparison
  - `b9d47cf` review fixes: mixed completed+denial pairing and required declared-input presence
  - `50af71a` configured-prequery acceptance plus the bounded `0x1F79A0` admission
  - `5e492e1` reject a no-call prequery that claims the ordinal-2 stop
  - `2450b7e` truthful avQuery outcome presentation in the local startup checker
  - `d5ee7af` require the ordinal-2 call record for every unsupported ordinal-2 stop
  - `e1b0a28` run the controller native fixtures in the native suite
  - `a74e2ce` run the previously unrun guest/pfifo native fixtures
  - `79f20c2` run the native component suite in preparation CI
  - `3ef6024` run the full Python evidence suite in preparation CI
- Source files touched: `src/runtime/proof-report.mjs`, `src/runtime/device-report.mjs`, `src/runtime/thread-proof-worker.mjs`, `src/runtime/local-title-proof.mjs`, `tools/compare-proof-profiles.mjs`, `tools/fixtures/thread_startup_probe.c`, `tools/fixtures/thread_startup_check.html`, `tools/run-native-tests.mjs`, `.github/workflows/prepare.yml`, `tests/proof-report.test.mjs`, `tests/compare-proof-profiles.test.mjs`, `tests/av-query-outcome.test.mjs`, `package.json`.
- Targeted checks on this slice: runtime 179/179, evidence 4/4, native 34/34, `npm run check` 354 source files, `git diff --check` clean; pair comparison exits 0. Preparation CI runs `npm run test:native` on Linux and the full `npm run test:evidence` suite.
- Narrow GLM review (`2026-11-04T09:15:22`, step-capped after the checks) found no material defect; the control stop target `0xFE000094` and the `0x1F79A0` enter/exit trace were confirmed from the retained reports. A follow-up narrow review at `2026-11-04T10:05:11` of the no-call prequery guard also found no material defect; minor unexercised control-path cases are noted in the binding checkpoint.
- Presentation: `src/runtime/local-title-proof.mjs` exports `describeAvQueryOutcome`, used by `tools/fixtures/thread_startup_check.html` to label the result as completed, denied/unsupported or prequery; `tests/av-query-outcome.test.mjs` covers it. Runtime 179/179.

## Integrable slice series

- Local branch: `slice-integrable-cc07556` (not pushed).
- Base: `e406cb3` (`3f97afa^`) plus `21c6c25` “Base: ac207ba native AV-query CI”, which applies only the 7-line `ac207ba` native-AV-query hunk to `.github/workflows/prepare.yml`. The component and fixture blobs already match `ac207ba`.
- Cherry-picked range: `3f97afa^..d5ee7af` (20 commits) applied with **zero conflicts**; head `7b25c88e56c4d5a21e2777999e9752cadb014b41` (21 commits including the base).
- Resulting tree differs from `d5ee7af` only by the 7-line `prepare.yml` hunk.
- Checks in the temporary worktree: `npm run check` passes 354 source files; `npm run test:runtime` passes 178 with 1 skipped (the ignored local XBE is absent in the temporary worktree; the source worktree runs 179/179).
- No push, merge, `0x1F6EB0` admission or geometry change. This series is ready for the architect's exact-head integration review.

## Retained admission evidence

- Module `recomp-post-memory-build-71`, SHA-256 `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`.
- Control `browser-71-reset3.json`: status 1 at `0xFE000094`, ordinal 2, 279 routines / 404 kernel ops.
- Explicit `browser-71-reset-avquery.json`: status 2 at `0x1F6EB0`, 281 / 405; `0x1F79A0` executed and returned; next frontier entry `0x1F6EB0` called by executed `0x1F83C0`.
- Translation `recomp-title-recomp-title-post-memory-50`: 419 functions / 17,448 instructions from 32 retained seeds. `verify-seeds.py` reports 32 recorded / 32 offered / 0 unexpected / 0 missing; `verify-translation.py` reports 517 functions, 0 needing rewriting, fully instrumented.
- `0x1F6EB0` is **not** admitted or exported. No storage/framework/substitute work was added.
- **Read-only `0x1F6EB0` characterization:** 110 instructions / 455 bytes in the `D3D` section, no direct calls, return sites `0x1F7033`/`0x1F7076`, no indirect or undecodable branch; unadmitted. See [the frontier characterization](FRONTIER_1F6EB0_CHECKPOINT.md).
- **Helper result synced to the integration head:** `4917e83` merges `c9dc22f` into the source lane so PR #2 is mergeable. The decision log is a 287-row union with no duplicates; `npm run check` 367 files, `test:runtime` 186/186; only governance tools/CI differ from the helper result. The bounded `0x1F6EB0` result and the `0x1FC780` stop are unchanged.
- **Exact-head admission evidence (`becc879`):** module `9d8689ff?`; control profile `d01ac221?`; experiment profile `351383c3?` (only `avQueryWord` differs); control report `82dd1464?`; experiment report `f92976d4?`; next stop `0x1FC780`. The source lane is synced to `becc879`; PR #2 head `e1804fb` is `CLEAN`/`MERGEABLE`; preparation CI runs `41917187801` and `41917176661` success; no further admission until reviewed.
- **Verification lane:** `tools/verify-exact-head.mjs` is on `origin/codex/opencode-verification` (`b2e4558`), not in `becc879`; the exact-head record is that lane's deliverable, not a primary change.
- **Main-PR ancestry gate (2026-11-04T16:40):** architect head `ae4b7ae` contains the merged `0x1F6EB0` evidence slice (`f55b633`) but not `origin/main`'s `2aba638`; the exact-head precheck fails with "published base not contained" (head ahead 23, behind 1). Merging `origin/main` into `ae4b7ae` is clean and zero-content in a temporary worktree, so the gate is fixable without changing the tree. The primary did not modify the architect branch or open a main PR.
- **Main integration (2026-11-04T16:40):** PR #5 merged as `2aba638`; its tree equals `becc879`. The source lane is synced with `origin/main` (`ac87005`), so the helper slice is ancestry/mergeability ready for a main-PR evaluation. PR #2 remains `CLEAN`/`MERGEABLE`; the primary created no main PR or merge.
- **Exact-head precheck (`becc879`):** the verification-lane `tools/verify-exact-head.mjs` (from `origin/codex/opencode-verification`, `b2e4558`) reports the head matches the architect head and contains `origin/main`; local `main` `e700373` has 2 commits not in the head; the external CI/PR/marker/publication gates are listed. The primary lane ran it read-only; the verification lane owns the record.
- **Exact-head review state (2026-11-04T16:25):** integration head `c9dc22f` CI run `41908980383` success; PR #2 head `558af53` is `CLEAN`/`MERGEABLE`; its preparation CI runs `41915416295` and `41915405699` are success; the diff versus `c9dc22f` is docs-only. The bounded `0x1F6EB0` helper result and the `0x1FC780` stop are unchanged.
- **0x1FC780 admitted (`ae4b7ae` directive):** translation 52 (421 functions / 17,575 instructions), module 73 (`f8901660?`, 662,080 bytes). Same-module control 279/404 at ordinal 2; experiment 284/405, status 2 at `0x1FCCB0`; comparison exits 0; frontier enters `0x1FCCB0` called by `0x1F5640`. `0x1FCCB0` is unadmitted; the lane stops.
- **0x1FC780 slice review readiness (2026-11-04T17:00):** source head `09a93a0` contains `ae4b7ae` and `origin/main` `2aba638`; the diff versus `ae4b7ae` is docs-only; preparation CI run `41920638186` is green. A new PR into the architect branch would be clean; the primary has not opened one.
- **0x1FCCB0 admitted (`b5294fa` directive):** resync fixture passed (8-entry table, four `ret 8` cases, all targets emitted); table-aware seed merged (32 exports); translation 53 (422 functions / 17,593 instructions), module 74 (`c8c27cb4?`, 663,028 bytes). Control 279/404; experiment 285/405 at `0x1F7080`; comparison exit 0; frontier enters `0x1F7080` from `0x1F5640`. `0x1F7080` unadmitted; stop.
- **Read-only `0x1F7080` characterization:** 27 instructions / 89 bytes in `D3D`, no calls or indirect branches, return `0x1F70D8`; reached from `0x1F5640`; unadmitted. Exported seed: 89 bytes / 27 instructions, preflight passes; not added. See [the characterization](FRONTIER_1F7080_CHECKPOINT.md).
- **Static continuation candidates:** `0x1F70E0` (98 instructions / 329 bytes; needs untranslated `0x1FC980`/`0x1FCB70`) and `0x1F5B70` (118 / 347; unclassified `0x1F6300`); the taken branch is unproven and neither is a single-leaf admission. See [the candidates checkpoint](FRONTIER_1F70E0_1F5B70_CHECKPOINT.md).
- **Batch callees:** `0x1FC980` (2 instructions / 9 bytes, shared), `0x1FCB70` (40 / 156, two indirect tables), `0x1F6300` (113 / 392, calls `0x1FC980`, uses `rep movs`). The `0x1F70E0` path needs `0x1FC980` + `0x1FCB70`; the `0x1F5B70` path needs `0x1F6300` + `0x1FC980`; static only. The shared `0x1FC980` seed is preflighted (9 bytes / 2 instructions); not added. `0x1FCB70` has two outside tables (11+2 targets) separated by a 28-byte byte-data gap; the guard now accepts bounded defined 1-byte data and the real table-aware envelope passes layout and dispatch census (11+2 tables, 80 data bytes); Path A is structurally unblocked but still needs a fresh directive, while Path B is the seed-ready path. The shared `0x1FC980` seed was re-preflighted at `2026-11-04T20:50-05:00` (2/2 decoded, SHA `18744bb3…`); both Path A dependencies are now preflighted and unadmitted. The layout, flow/envelope and dispatch-table contracts are pinned by `tests/bounded_layout_test.py` (23 cases), including two pointer tables separated by defined bytes and a 256-byte byte-data budget. Path B seeds `0x1F5B70` and `0x1F6300` are preflighted; the three-function batch (plus shared `0x1FC980`) is seed-ready. See [the callee scope](FRONTIER_BATCH_CALLEES_CHECKPOINT.md).
- **0x1FCCB0 evidence readiness (2026-11-04T18:20):** source head `d22e378` contains `b5294fa` and `origin/main`; the diff versus `b5294fa` is docs-only; preparation CI run `41932333143` is green. A PR into the architect branch would be clean; the primary has not opened one.
- **Post-merge state (PR #6):** main `c543720` merges `b5294fa` into `2aba638`; main CI run `41935647475` success. The architect branch remains `b5294fa`, an ancestor of main. Post-merge exact-head precheck on `b5294fa` passes: head match, published base `c543720` parents-contained (behind 1, missing base paths 0), local main `e700373` contained, 373 paths.
- **0x1FCCB0 review submission:** [the admission checkpoint](FRONTIER_1FCCB0_ADMISSION_CHECKPOINT.md) carries the same-module pair (control 279/404 at ordinal 2; experiment 285/405 at `0x1F7080`; only `avQueryWord`; comparison exit 0), the `0x1F7080` next-stop record and all digests. The architect integrated the slice at `92c31ef` (source `33cfb36`, CI run `41940691880`), so it is no longer pending; newer lane commits `f576c21`, `f9cba5b` and `6330db8` are not yet integrated. No new admission or Path B batch without a fresh directive. On recheck, all ten declared digests match the retained artifacts and the comparator still exits 0. The one missing recorded seed (`ca2e5d57df7e25270dbc2ce2bc3ab4d326bd1d0b62310ac57d9adc0d0b593b4b`, generation index 20; function `0x002022E0`, 2 instructions) was re-exported read-only from the retained project and matches the recorded digest exactly; `verify-seeds` now reports 32 recorded / 32 offered / 0 unexpected / 0 missing. The recovered seed stays local/ignored. The shared report contract also rejects an unconfigured ordinal-2 call record with a status outside completed (0)/denied (1), requires an actual denial record for an unsupported ordinal-2 stop, and requires `distinctOriginalFunctions` to match the unique reached addresses. The reached list must also equal the trace's enter-address sequence. The comparator also requires both reached-routine lists when either is present, rejects a pair where the experiment drops a control-reached routine, and requires each report's completed-operation count to match its successful call records. `tools/verify-recomp-package.mjs` reruns the whole package in one command and reports 32/32 seeds, 422 admitted / 517 generated / 0 rewrites, 10/10 declared digests, pair ok, next stop `0x1F7080`. The synthetic 96-byte fixture and LF-normalized generated C are committed with a regression that pins both digests, the four targets and the guard census. The `0x1F7080` seed preflight was re-run at `2026-11-04T20:26-05:00` (27/27 instructions, no tables, no normalizations) and a review-ready manifest is recorded, but it remains unadmitted; a read-only manifest preflight at `2026-11-04T20:30-05:00` confirms 32/32 seeds, no duplicate, and the 30-function / 423-function / 17,620-instruction / 32-export budget fit. The Sol architect escalation was retried at `2026-11-04T20:25-05:00` and again at `2026-11-04T20:35-05:00`, returning the same `subscription_sharing_usage_limit_exceeded`; the decision remains unresolved.
- **Exact-head verification (`b5294fa`):** head matches the architect head and contains `origin/main` (`2aba638`, ahead 37, behind 0); local main `e700373` contained; PR #6 `CLEAN`/`MERGEABLE`; CI runs `41927475606`/`41927492924` pass. The ancestry gate is satisfied.
- **Read-only `0x1FCCB0` characterization:** 7 direct instructions / 26 bytes in `D3D`, one indirect jump through table `0x1FCCFC` (selector bound `0x8`), return `0x1FCCF8`; the 8 entries map to four three-instruction `ret 8` cases inside `0x1FCCB0..0x1FCCF8`, with a 32-byte table and padding to `0x1FCD20`; reached from `0x1F5640`; unadmitted. Retained export: 18 instructions / 75-byte envelope; table outside the body; seed not added. A read-only table-aware envelope satisfies the guard (112-byte envelope, one 8-target table) and the byte/decoder preflight; seed not added; unadmitted. Static caller continuation after `0x1FCCB0` names three untranslated call candidates (`0x1F7080`, `0x1F70E0`, `0x1F5B70`); the taken path is unproven. See [the dispatcher checkpoint](FRONTIER_1FCCB0_CHECKPOINT.md).
- **Read-only `0x1FC780` characterization:** 17 instructions / 53 bytes in `D3D`, one self-recursive call, `ret 4`, reached from executed `0x1F5640`; unadmitted. See [the characterization](FRONTIER_1FC780_CHECKPOINT.md).
- **Obsolete baseline request rejected:** `recomp-title-post-utility-01` (317 functions / 10,612 instructions, 19 seeds) neither contains nor references `0x1F6EB0`; its utility path stops at `MmQueryStatistics`. The current-lineage admission (translation 51) and the `0x1FC780` stop stand under the architect's directive.
- **0x1F6EB0 admitted** under the `c9dc22f` directive: translation 51 (420 functions / 17,558 instructions), module 72 (`9d8689ff?`, 660,060 bytes). Same-module control 279/404 at ordinal 2; experiment 283/405, status 2 at `0x1FC780`; frontier enters `0x1FC780` called by `0x1F5640`. `0x1FC780` is unadmitted; the lane stops. Independent guards: seeds 32 recorded / 32 offered / 0 unexpected / 0 missing; translation 517 functions, 0 needing rewriting.

## Main/PR integration state

- PR [#1](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/1) was **merged** as `b62989f`; `origin/main` is now `b62989f`. It contains `cc07556`, `385dae5`, `62149dd`, `3a5d94c` and `ac207ba`.
- `origin/codex/opencode-architect` remains `cc07556`. The report/helper slice commits (`3f97afa`, `35a664a`, `12deaad`, `0ffb634`, `b9d47cf`, `50af71a`, `5e492e1`, `2450b7e`, `d5ee7af`) are **not** ancestors of `main`, and `main` lacks `src/runtime/proof-report.mjs`, `tools/compare-proof-profiles.mjs` and `tests/av-query-outcome.test.mjs`. `ac207ba` is now in `main`; the unconnected component and fixture are present.
- **Direct-on-main conflict:** cherry-picking `3f97afa^..d5ee7af` onto `b62989f` conflicts. Modify/delete: `docs/AV_QUERY_BINDING_CHECKPOINT.md`, `src/runtime/proof-report.mjs`, `tests/proof-report.test.mjs`, `tests/compare-proof-profiles.test.mjs`, `tools/compare-proof-profiles.mjs`. Content conflicts: `docs/DECISIONS.md`, `docs/DEVELOPMENT_HANDOFF.md`, `docs/PROJECT_CONCERNS.md`, `docs/PROJECT_STATUS.md`, `src/runtime/thread-proof-worker.mjs`, `tools/fixtures/thread_startup_probe.c`. The slice assumes pre-slice source files that `main` never received.
- The clean prepared series `slice-integrable-cc07556` (on `3f97afa^` + the `ac207ba` CI hunk) remains the integrable deliverable: 20 slice commits, zero conflicts, head `7b25c88`. It requires its pre-slice source history, not just the slice commits.
- Source-lane integration PR [#2](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/2) is OPEN: head `aead74f` (source branch), base branch `codex/opencode-architect` at the recorded `54f062e`, `CONFLICTING`/`DIRTY`, no reviews; the Ubuntu preparation check failed and Windows was cancelled. Against the current architect head `cc07556`, the merge conflicts in `docs/DECISIONS.md`, `docs/DEVELOPMENT_HANDOFF.md`, `docs/PFIFO_RESET_STATE_CHECKPOINT.md` (add/add) and `docs/PROJECT_STATUS.md`; source and CI auto-merge. The source head contains the slice but not `ac207ba`.
- Independent check of the provisional 20-commit-only rebase onto `b62989f`: the base lacks the pre-slice slice-only files, so the first slice commit conflicts with modify/delete for `docs/AV_QUERY_BINDING_CHECKPOINT.md`, `src/runtime/proof-report.mjs`, `tests/proof-report.test.mjs`, `tests/compare-proof-profiles.test.mjs` and `tools/compare-proof-profiles.mjs`; content conflicts remain in `DECISIONS.md`, `DEVELOPMENT_HANDOFF.md`, `PROJECT_CONCERNS.md`, `PROJECT_STATUS.md`, `thread-proof-worker.mjs` and `thread_startup_probe.c`. It is not a simple A023/`DECISIONS.md` rebase. The prepared series remains the viable complete slice.
- Completed rebase verification: `slice-rebase-b62989f-full` restores the five slice-only files, resolves content conflicts toward the slice, and appends A023; head `b092a6f` (`a3a3e4e` slice tip + A023). `npm run check` passes **308** files and `test:runtime` **142** tests, but `d5ee7af` has 354/179. **28 pre-slice source/test files are missing** (including `av_query_adapter`, `kernel-service`, `disc-session`, `pcrtc_state`, `pfifo_status`, `av-observation`). The 20-commit-only rebase is an incomplete slice; the prepared series remains the complete deliverable.
- **Complete main-merge candidate:** `slice-merge-main` at `88966cc` = merge `0c0150c` of `1988c9b` into `b62989f` (`-X theirs`) plus `88966cc` keeping A023. All pre-slice files are present (0 deletions vs `d5ee7af`); `npm run check` passes **361** files and `test:runtime` **178 pass + 1 skipped** (the ignored local XBE is absent in the temporary worktree; the source worktree runs 179/179). That merge is governance-incomplete (see the next bullets) and is superseded below.
- **Governance-lossy merge defect:** `88966cc` (and any `-X theirs` source merge) drops main-only `A001-A022` from `docs/DECISIONS.md`; the result has 245 rows and only an explicitly re-appended `A023`. Verified by row-set comparison against `b62989f` and `a16ce50`.
- **Corrected integration candidate:** `slice-merge-correct` at `1d4c16a` is a plain three-way merge of `a16ce50` into `b62989f`. `docs/DECISIONS.md` is a 282-row union (166 main + 247 source - 131 shared; no duplicates; all A/W and D rows), with `D128-D131` corrected to the source text and `D132-D247` appended; lane-owned handoff/status/PFIFO checkpoint take source. `npm run check` 361 files, `test:runtime` 178 pass + 1 skip, `test:native` 34/34, `git diff --check` clean; main governance docs and the AV-query CI step are preserved. No runtime/test source file differs from `a16ce50` except main three governance tools.
- **Integration-preservation check:** `tools/verify-integration.mjs <base> <source> <candidate>` fails when a candidate drops any parent decision row, duplicates an ID, or omits a source-tracked path. It reproduces both prior defects (`88966cc` missing `A001-A022`; `b092a6f` missing source files) and passes the corrected candidate (166 base + 247 source = 284 rows). Unit tests use synthetic data.
- **Exact-head CI blocker fixed:** the Ubuntu preparation job had never run the native fixtures. `tools/run-native-tests.mjs` passed a literal `gcc` token to the detected driver and used the compiler as the program for the run step, so every non-WSL host failed with `/usr/bin/ld: cannot find gcc`. The launcher now builds an explicit native/WSL `commandPlan`; `tests/run-native-tests.test.mjs` covers both paths. Local runtime 181/181, native 34/34, check 355; exact-head CI run `41905648969` on `d143a9c` passed Ubuntu and Windows, with the Ubuntu native step executing all 34 fixture runs.
- The source lane's native suite passes 34/34. The prepared `slice-integrable-cc07556` series is separate from the source-lane PR #2 branch.

## Hold and next

The Sol architect (`2026-11-04T09:30`) selected **A: hold further admission pending exact-head integration review**. A second escalation at `2026-11-04T09:55:09` failed with `subscription_sharing_usage_limit_exceeded`, and a third at `2026-11-04T10:20:10` failed with the same provider limit; a fourth at `2026-11-04T11:10:10` failed the same way; a fifth at `2026-11-04T12:00:17` also failed with the provider limit; a sixth at `2026-11-04T13:00:07` failed the same way. The hold remains unresolved; a seventh escalation at `2026-11-04T15:07` failed with `subscription_sharing_usage_limit_exceeded`, and an eighth at `2026-11-04T15:26` failed the same way. The corrected `slice-merge-correct` candidate supersedes the governance-lossy merge. PR #1 was merged as `b62989f`, but the merged main does not contain the report/helper slice; a direct cherry-pick of the slice onto main conflicts (modify/delete and content conflicts). The clean `slice-integrable-cc07556` series remains the integrable deliverable and requires the pre-slice source history. The `0x1F6EB0` admission gate remains closed. After this handoff the lane is parked: do not export/admit `0x1F6EB0`, expand storage/frameworks, or add substitute work. Geometry remains paused. Release requires the architect's independent review of the corrected slice and exact current PR head, resolution or naming of material blockers, and an explicit next-admission authorization.

The control path remains unsupported; `avQueryWord` stays explicit and synthetic. No controllable original screen, fidelity, online/social or authority claim is established.
