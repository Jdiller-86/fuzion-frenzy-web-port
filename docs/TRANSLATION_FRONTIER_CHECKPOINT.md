> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Read-only characterization of the 0x202150 call surface

November 5, 2026. No admission, export or seed change. The directive's premise is stale: `0x202150` (`D3DDevice_Present`) was already admitted in `7dae2e4` under the previous architect-subagent authorization. Characterization confirms `body 0x202150..0x2022D0`: 115 instructions / 385 bytes, 115/115 decoded, one `RET 0x10`, 20 basic blocks, no tables, two loops (fence wait, submit retry). Seven direct callees — `0x1FAF80` `D3D_SetFence`, `0x1FACA0`, `0x1FAED0` `CDevice_KickOff`, `0x1FB300` `D3D_KickOffAndWaitForIdle` pre-existing; `0x201E80` (476 bytes), `0x202060` (228), `0x1F5520` (36) added in translation 83. Nine calls resolve through seven pointer sites whose targets are kernel ordinals (`0x80000001/2/3/4/0xAB/0xB2/0xB4`), not code. Caller `0x42C5E` in `FUN_00042be0` builds a 640×480 format-7 descriptor first. Retained run: status 13 at `0xFD800044` (PFIFO MMIO), `pushbuffer consumption required`. Frontier at translation 83 remains 554 functions / 93 placeholders / 92 referenced. These are static facts and a retained synthetic stop, not rendering/gameplay evidence.

## Prior: The 0x202150 batch reaches the PFIFO pushbuffer boundary

November 5, 2026. After Published head `6cbe40d` and local `2618783`, the post-table region decision was escalated once to the project architect, which authorized one bounded `D3DDevice_Present` set. `0x202150` was already an established function (`body 0x202150..0x2022D0`), so no create-function was made; the set admitted it plus its three untranslated immediate direct callees `0x201E80` (125 instructions / 476 bytes), `0x202060` (64 / 228) and `0x1F5520` (10 / 36) — 4 functions, no transitive expansion. A 6-function seed (`6c5a69b4?`) also carries two functions from a replaced 2-function export, keeping 32 exports; all six preflights are clean with 0 tables.

Translation `recomp-title-recomp-title-post-memory-83` contains **461 functions / 18,940 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 554/0. Module `recomp-post-memory-build-104` is 714,276 bytes, SHA-256 `ff29e85547e6567fa417538d713429632c9e46bc8ef374a3366d80586697e543`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE00009C`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 13 at `0xFD800044`**, ordinal 1 | 339 | 405 |

The comparator exits 0 with +60 routines / +1 operation. The status-2 stop at `0x202150` is gone; the new stop is a PFIFO MMIO register (not code), return `0x1FB06E`, call site `0x1FB069` inside `D3D_BlockOnTime`, `stopReason: "pushbuffer consumption required"`. Frontier: **554 functions, 93 placeholders, 92 referenced**. The next boundary is PFIFO pushbuffer consumption, a fresh architect decision; this cycle halts after one set. These are synthetic startup observations, not command-correctness, rendering, gameplay or playability evidence.

## Prior: The 0x1F65F0 batch closes the last render-state translation gap

November 5, 2026. Starting from published head `6cbe40d`, a coverage audit of the completed render-state table found **27 of 28** handlers translated: index 134, `0x1F65F0` (`D3DDevice_SetRenderState_MultiSampleType`), is never set by this profile and was missing. It was admitted this cycle after one bounded create-function at that address only (`body 0x1F65F0..0x1F6614`, index 134, `TABLE_MATCHES 1`). The leaf is 9 instructions / 37 bytes, calls already-translated `0x1F5640`, and ends in `RET 4`; seed `f338d481?`, preflight 9/9, no tables or normalizations.

The merged seed had reached the pipeline's 32-function-per-export limit, so two existing single-function exports (`0x31973`, `0x2D540`) were regrouped into one 2-function export (`25074fa1?`), keeping 32 exports; the new function was appended to the merged seed (33 functions, `f4dca8ea?`). Translation `recomp-title-recomp-title-post-memory-82` contains **457 functions / 18,626 instructions** with table coverage **28/28**; `verify-seeds` 32/32/0/0 and `verify-translation` 547/0. Module `recomp-post-memory-build-103` is 705,795 bytes, SHA-256 `6e298d4440194fa7373cbe6b93de2014e124d04899c80b99931a67db21f592ff`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x00202150`** | 332 | 405 |

The comparator exits 0 with +53 routines / +1 operation; the stop and counts are unchanged from module 102 because this profile never sets the new state. `0x1F65F0` was not reached. Frontier: **547 functions, 90 placeholders, 89 referenced**. The post-table `D3DDevice_Present` stop remains unadmitted and the next region is a fresh architect decision. These are synthetic startup observations, not fidelity, renderer, gameplay or playability evidence; translated coverage is not proof that a render state is exercised or correct.

## Prior: The 0x1F7570 batch completes the render-state table; the run stops at D3DDevice_Present

November 5, 2026. Starting from published head `45c0327`, this cycle admitted the last two render-state handlers, `0x1F7570` (`RopZRead`, index 142) and `0x1F7590` (`DoNotCullUncompressed`, index 143). Both are 4-instruction / 17-byte wrappers that store their argument and call the already-admitted `0x1F7230`; each needed one bounded create-function at that address only. Seeds `8f6f0496?` and `024f834b?`; preflights 4/4, no tables or normalizations. They were appended to the merged 30-function seed (`8f1b7667?`, 32 functions), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-81` contains **456 functions / 18,617 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 546/0. Module `recomp-post-memory-build-102` is 705,414 bytes, SHA-256 `27ceaa9eb30f413d456692333f6d303fbdc75db780b829b88617d355b3489453`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x00202150`** | 332 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +53 routines, +1 operation. Both new handlers executed; `0x202150` was not entered. Frontier: **546 functions, 90 placeholders, 89 referenced**, 354 direct call targets.

The table is complete (index 144 onward is non-pointer data). The post-table stop is a status-2 direct call to the code symbol `D3DDevice_Present` from `0x42C5E` in `FUN_00042be0`; it was not admitted and the next region is a fresh architect decision. These are synthetic startup observations, not fidelity, renderer, gameplay or playability evidence.

## Prior: The 0x1F7550 batch advances the stop to 0x1F7570

November 5, 2026. Starting from published head `b576fa2`, this cycle admitted `0x1F7550` (`D3DDevice_SetRenderState_RopZCmpAlwaysRead`, dispatch table index 141) plus its direct dependency `0x1F7230` (`D3D_CommonSetDebugRegisters`, already established). The new leaf is 4 instructions / 17 bytes and calls `0x1F7230`; only it needed a bounded create-function at that address only. The callee is 45 instructions / 200 bytes and calls translated `0x1FB3B0`. Seeds: `610fda82?` and `2038c102?`; preflights 4/4 and 45/45, no tables or normalizations. The pair was appended to the merged 28-function seed (`adce1396?`, 30 functions), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-80` contains **454 functions / 18,609 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 544/0. Module `recomp-post-memory-build-101` is 705,079 bytes, SHA-256 `cfb7b918eb44ba742a34d51371c2e762f839b47916063785da1911f60a77047e`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7570`** | 324 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +45 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7550` and `0x1F7230` executed; `0x1F7570` was not entered. Frontier: **544 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics; index 143 remains, so the render-state table is not complete and no post-table region is declared.

## Prior: The 0x1F74F0 batch advances the stop to 0x1F7550

November 5, 2026. After syncing the published architect head `d4e0107` (rows renumbered so `D339` stays the architect merge row and the `0x1F7490` admission is `D340`), this cycle admitted `0x1F74F0` (`D3DDevice_SetRenderState_StencilCullEnable`, dispatch table index 140). It was a named symbol without a function, so one bounded create-function at that address only was added (`body 0x1F74F0..0x1F754B`); the table does not end there (indices 141-143 remain), so this is not a post-table region. Its 26-instruction / 92-byte leaf calls already-translated `0x1FB3B0`, has internal conditional branches, and ends in `RET 4`; preflight is 26/26, no tables or normalizations. The seed was appended to the merged 27-function seed (`877657f2?`, 28 functions), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-79` contains **452 functions / 18,560 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 542/0. Module `recomp-post-memory-build-100` is 703,605 bytes, SHA-256 `c03009873b3f46c7a248da4abcad6b74a34af8d5862305108afd8aaf88adcf96`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7550`** | 322 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +43 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F74F0` executed; `0x1F7550` was not entered. Frontier: **542 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics; the render-state table is not complete and no post-table region is declared.

## Prior: The 0x1F7490 batch advances the stop to 0x1F74F0

November 5, 2026. `0x1F7490` (`D3DDevice_SetRenderState_OcclusionCullEnable`, table index 139) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 26-instruction / 92-byte leaf calls already-translated `0x1FB3B0`, has internal conditional branches, and ends in `RET 4`; preflight is 26/26, no tables or normalizations. The seed was appended to the merged 27-function seed (`df3d4c44?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-78` contains **451 functions / 18,534 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 541/0. Module `recomp-post-memory-build-99` is 702,974 bytes, SHA-256 `29e937977e0c435d12dcb1d253eef090d0cb4ad88354f0b1fa5e224bc4e529b5`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F74F0`** | 321 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +42 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7490` executed; `0x1F74F0` was not entered. Frontier: **541 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F7460 batch advances the stop to 0x1F7490

November 5, 2026. `0x1F7460` (`D3DDevice_SetRenderState_YuvEnable`, table index 138) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 12-instruction / 37-byte leaf calls already-translated `0x1FB3B0` and `0x1F7080`, and ends in `RET 4`; preflight is 12/12, no tables or normalizations. The seed was appended to the merged 26-function seed (`dc0ab154?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-77` contains **450 functions / 18,508 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 540/0. Module `recomp-post-memory-build-98` is 702,324 bytes, SHA-256 `60d2a65f73914f6ed9807516e316aa2d19bdfd2931e562a86eae44714de515e6`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7490`** | 320 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +41 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7460` executed; `0x1F7490` was not entered. Frontier: **540 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6760 batch advances the stop to 0x1F7460

November 5, 2026. `0x1F6760` (`D3DDevice_SetRenderState_ShadowFunc`, table index 135) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 13-instruction / 47-byte leaf calls already-translated `0x1FB3B0` and ends in `RET 4`; preflight is 13/13, no tables or normalizations. The seed was appended to the merged 25-function seed (`9cb1f5c6?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-76` contains **449 functions / 18,496 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 539/0. Module `recomp-post-memory-build-97` is 701,883 bytes, SHA-256 `c7eb34ab8fd1160cf6211d762f9bf6c0eec505d8cc5e24ab784a310fe407c17e`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7460`** | 319 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +40 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6760` executed; `0x1F7460` was not entered. Frontier: **539 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F7600 batch advances the stop to 0x1F6760

November 5, 2026. `0x1F7600` (`D3DDevice_SetRenderState_MultiSampleMask`, table index 133) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 22-instruction / 76-byte leaf calls already-translated `0x1F82C0`, `0x1FB3B0` and `0x1F6EB0`, and ends in `RET 4`; preflight is 22/22, no tables or normalizations. The seed was appended to the merged 24-function seed (`b95fe13c?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-75` contains **448 functions / 18,483 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 538/0. Module `recomp-post-memory-build-96` is 701,399 bytes, SHA-256 `8da3c4b55a2e453221ef87ca786b49c540bb5ab3c3e679aba177fa87f52c8819`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6760`** | 318 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +39 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7600` executed; `0x1F6760` was not entered. Frontier: **538 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F75B0 batch advances the stop to 0x1F7600

November 5, 2026. Main ancestry was restored from PR #7 (`42287210` merged at `a0be99b`; EOF fix `8c49a22`; rows renumbered `D328`–`D334`). `0x1F75B0` (`D3DDevice_SetRenderState_MultiSampleAntiAlias`, table index 132) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 22-instruction / 76-byte leaf calls already-translated `0x1F82C0`, `0x1FB3B0` and `0x1F6EB0`, and ends in `RET 4`; preflight is 22/22, no tables or normalizations. The seed was appended to the merged 23-function seed (`d74b0d28?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-74` contains **447 functions / 18,461 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 537/0. Module `recomp-post-memory-build-95` is 700,698 bytes, SHA-256 `286daba75b816b3fb7bf933fc614bc63d5c289c0eca01633d27f87093b77f182`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7600`** | 317 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +38 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F75B0` executed; `0x1F7600` was not entered. Frontier: **537 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6730 batch advances the stop to 0x1F75B0

November 5, 2026. `0x1F6730` (`D3DDevice_SetRenderState_EdgeAntiAlias`, table index 131) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 13-instruction / 44-byte leaf calls already-translated `0x1FB3B0` and ends in `RET 4`; preflight is 13/13, no tables or normalizations. The seed was appended to the merged 22-function seed (`61fd9a0b?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-73` contains **446 functions / 18,439 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 536/0. Module `recomp-post-memory-build-94` is 700,022 bytes, SHA-256 `e9b108f222addd30b65f9687a63879698d29e8ef41148adff744f696b94b9704`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F75B0`** | 316 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +37 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6730` executed; `0x1F75B0` was not entered. Frontier: **536 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6A70 batch advances the stop to 0x1F6730

November 5, 2026. `0x1F6A70` (`D3DDevice_SetRenderState_LogicOp`, table index 130) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 22-instruction / 76-byte leaf calls already-translated `0x1FB3B0` and has two return sites; preflight is 22/22, no tables or normalizations. The seed was appended to the merged 21-function seed (`ca4e5c98?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-72` contains **445 functions / 18,426 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 535/0. Module `recomp-post-memory-build-93` is 699,510 bytes, SHA-256 `855ce4ec17a8948a71b70547105a72f1525bc6b61f1b1bbf8130a8d0f7cbd1fe`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6730`** | 315 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +36 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6A70` executed; `0x1F6730` was not entered. Frontier: **535 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F69F0 batch advances the stop to 0x1F6A70

November 5, 2026. `0x1F69F0` (`D3DDevice_SetRenderState_ZBias`, table index 129) was already Ghidra-established. Its 41-instruction / 116-byte leaf calls translated `0x1F66A0` five times, performs the FPU sequence `FILD/FADD/FCHS/FST/FMUL/FSTP`, and ends in `RET 4`; preflight is 41/41, no tables or normalizations. The seed was appended to the merged 20-function seed (`846cac1b?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-71` contains **444 functions / 18,404 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 534/0. Module `recomp-post-memory-build-92` is 698,963 bytes, SHA-256 `57e245de1a8ad896f57d1e67bee3fd69eb742f240561eeaacf059fac8f34e9d0`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6A70`** | 314 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +35 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F69F0` executed; `0x1F6A70` was not entered. Frontier: **534 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F68C0 batch advances the stop to 0x1F69F0

November 5, 2026. `0x1F68C0` (`D3DDevice_SetRenderState_TextureFactor`, table index 128) was already Ghidra-established. Its 25-instruction / 76-byte leaf calls already-translated `0x1FB3B0`, performs one `rep stosd` of 16 dwords, and has two return sites; preflight is 25/25, no tables or normalizations. The seed was appended to the merged 19-function seed (`347da100?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-70` contains **443 functions / 18,363 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 533/0. Module `recomp-post-memory-build-91` is 697,055 bytes, SHA-256 `21ef36559c62ea4cce52c9b6719cd3d0ffc69c484bb2ccb5c480b8d6261e45ae`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F69F0`** | 313 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +34 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F68C0` executed; `0x1F69F0` was not entered. Frontier: **533 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6850 tail-call batch advances the stop to 0x1F68C0

November 5, 2026. `0x1F6850` (`D3DDevice_SetRenderState_FrontFace`, table index 126) was a named symbol without a function and ends in a tail `JMP 0x1F67E0`; one bounded persistent create-function preserved that tail, and `0x1F67E0` (`D3DDevice_SetRenderState_CullMode`, table index 127) was already established. Seeds: 14/52 bytes (`80bdcbee?`) and 29/98 (`2d5f7bf5?`), both preflight-clean. They were appended to the merged 18-function seed (`3bbc774a?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-69` contains **442 functions / 18,338 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 532/0. Module `recomp-post-memory-build-90` is 696,254 bytes, SHA-256 `f74a2161de4d8d3761b1ac601bef65913b564b57eaf94d783df4b35cef466a88`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F68C0`** | 312 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +33 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6850` executed and tail-called `0x1F67E0`; `0x1F68C0` was not entered. Frontier: **532 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F73F0 batch advances the stop to 0x1F6850

November 5, 2026. `0x1F73F0` (`D3DDevice_SetRenderState_StencilFail`, table index 125) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 29-instruction / 105-byte leaf calls already-translated `0x1FB3B0`, has internal conditional branches, and ends in `RET 4`; preflight is 29/29, no tables or normalizations. The seed was appended to the merged 16-function seed (`14cc6913?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-68` contains **440 functions / 18,295 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 530/0. Module `recomp-post-memory-build-89` is 694,956 bytes, SHA-256 `4f64855e0f6e39583dd9887da58ffa240ab4b74cefb46d9cbc94c397b4ea36d9`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6850`** | 310 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +31 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F73F0` executed; `0x1F6850` was not entered. Frontier: **530 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F7370 batch advances the stop to 0x1F73F0

November 5, 2026. The publication backlog was cleared first (`c28399a` removed the committed EOF blank line; source remote advanced to `c28399a` with CI success). `0x1F7370` (`D3DDevice_SetRenderState_StencilEnable`, table index 124) was already Ghidra-established; its only call was translated. Its 37-instruction / 128-byte leaf has internal branches and ends in `RET 4`; preflight is 37/37, no tables or normalizations. The seed was appended to the merged 15-function seed (`15239924?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-67` contains **439 functions / 18,266 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 529/0. Module `recomp-post-memory-build-88` is 694,246 bytes, SHA-256 `5440f6e0bb7ceb8c27eb0e5f411e66b1dd0e40891b20acdd49bb94c7d709b3ec`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F73F0`** | 309 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +30 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7370` executed; `0x1F73F0` was not entered. Frontier: **529 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F7300 batch advances the stop to 0x1F7370

November 5, 2026. `0x1F7300` (`D3DDevice_SetRenderState_ZEnable`, table index 123) was already a Ghidra-established function; all five calls (`0x1FB3B0` twice, `0x1F85E0`, `0x1F82C0`, `0x1F7080`) were already translated. Its 34-instruction / 108-byte leaf has internal conditional branches and ends in `RET 4`; preflight is 34/34, no tables or normalizations. The seed was appended to the merged 14-function seed (`a48daeda?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-66` contains **438 functions / 18,229 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 528/0. Module `recomp-post-memory-build-87` is 693,474 bytes, SHA-256 `3f001885c215ace4b866ae300a1333c1fd1c803e7afd4f9a7753e07beef66ee9`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7370`** | 308 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +29 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7300` executed; `0x1F7370` was not entered. Frontier: **528 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6890 batch advances the stop to 0x1F7300

November 5, 2026. `0x1F6890` (`D3DDevice_SetRenderState_NormalizeNormals`, table `0x2025D8`, index 122) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 13-instruction / 45-byte leaf calls already-translated `0x1FB3B0` and ends in `RET 4`; preflight is 13/13, no tables or normalizations. The seed was appended to the merged 13-function seed (`a41d2643?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-65` contains **437 functions / 18,195 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 527/0. Module `recomp-post-memory-build-86` is 692,563 bytes, SHA-256 `2a0ff87ba332b1dda66589dc4270b05dc7a5cda7fdcd77a469f558402d2117ed`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F7300`** | 307 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +28 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6890` executed; `0x1F7300` was not entered. Frontier: **527 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6B50 batch advances the stop to 0x1F6890

November 5, 2026. `0x1F6B50` (`D3DDevice_SetRenderState_TwoSidedLighting`, table `0x2025D4`, index 121) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 24-instruction / 84-byte leaf calls already-translated `0x1FB3B0`, contains one internal `JNE` to `0x1F6B88`, and ends in `RET 4`; preflight is 24/24, no tables or normalizations. The seed was appended to the merged 12-function seed (`831991b3?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-64` contains **436 functions / 18,182 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 526/0. Module `recomp-post-memory-build-85` is 692,002 bytes, SHA-256 `d136484b1c5460fe17df8db9a2217017cb5d8fbaafdb171b762a262bc9ab8b01`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6890`** | 306 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +27 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6B50` executed; `0x1F6890` was not entered. Frontier: **526 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6B00 batch advances the stop to 0x1F6B50

November 4, 2026. `0x1F6B00` (`D3DDevice_SetRenderState_BackFillMode`, table `0x2025D0`, index 120) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 22-instruction / 75-byte leaf calls already-translated `0x1FB3B0`, contains one internal `JNE` to `0x1F6B2F`, and ends in `RET 4`; preflight is 22/22, no tables or normalizations. The seed was appended to the merged 11-function seed (`e4c3222d?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-63` contains **435 functions / 18,158 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 525/0. Module `recomp-post-memory-build-84` is 691,173 bytes, SHA-256 `e1f59842bf7dcd58720f54ee41e421a8a8e704dd3398dea57c3c255b8e03bfec`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6B50`** | 305 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +26 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6B00` executed; `0x1F6B50` was not entered. Frontier: **525 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6AC0 batch advances the stop to 0x1F6B00

November 4, 2026. `0x1F6AC0` (`D3DDevice_SetRenderState_FillMode`, table `0x2025CC`, index 119) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 18-instruction / 62-byte leaf calls already-translated `0x1FB3B0`, contains one internal `JNE` to `0x1F6AE3`, and ends in `RET 4`; preflight is 18/18, no tables or normalizations. The seed was appended to the merged 10-function seed (`aeb158b5?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-62` contains **434 functions / 18,136 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 524/0. Module `recomp-post-memory-build-83` is 690,455 bytes, SHA-256 `9a84bdc1cfe8ac7e1a56871b4415d0a2f6ed2bd33475be93eb71b93cfa95cf15`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6B00`** | 304 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +25 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6AC0` executed; `0x1F6B00` was not entered. Frontier: **524 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6790 batch advances the stop to 0x1F6AC0

November 4, 2026. `0x1F6790` (`D3DDevice_SetRenderState_FogColor`, table `0x2025C8`, index 118) was already a Ghidra-established function. Its 22-instruction / 71-byte leaf calls already-translated `0x1FB3B0` and ends in `RET 4`; preflight is 22/22, no tables or normalizations. The seed was appended to the merged 9-function seed (`f7a2ee44?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-61` contains **433 functions / 18,118 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 523/0. Module `recomp-post-memory-build-82` is 689,834 bytes, SHA-256 `b3c354bdb3c561924f2d76c1018166a91a3136c2c63b2c704d5f0f68742f0bf0`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6AC0`** | 303 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +24 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6790` executed; `0x1F6AC0` was not entered. Frontier: **523 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6BB0 symbol-entry batch advances the stop to 0x1F6790

November 4, 2026. `0x1F6BB0` (`D3DDevice_SetRenderState_VertexBlend`, table `0x2025C4`, index 117) was a named symbol without a function; a bounded persistent create-function at that address only made it admissible. Its 15-instruction / 50-byte leaf calls already-translated `0x1FB3B0` and ends in `RET 4`; preflight is 15/15, no tables or normalizations. The seed was appended to the merged 8-function seed (`4fcdcd02?`), keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-60` contains **432 functions / 18,096 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 522/0. Module `recomp-post-memory-build-81` is 689,176 bytes, SHA-256 `d795973beea54c70344e363f587206f1268c98dbdac86c951c647f412cdc3d67`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6790`** | 302 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +23 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6BB0` executed; `0x1F6790` was not entered. Frontier: **522 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6620 symbol-entry batch advances the stop to 0x1F6BB0

November 4, 2026. The observed `0x1F6620` stop was a named indirect-dispatch symbol (`D3DDevice_SetRenderState_PSTextureModes`, table `0x2025C0`, index 116) without a Ghidra function. The initial admission used a bounded read-only in-memory definition; the approved architect directive then authorized a persistent create-function at that address only. A read-only re-open reports `FUNCTION_AT 001f6620`, and the re-export is byte-identical (SHA `55977f49…`), so the seed and admission are unchanged. The seed was appended to the merged 7-function seed, keeping 32 exports. Translation `recomp-title-recomp-title-post-memory-59` contains **431 functions / 18,081 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 521/0. Module `recomp-post-memory-build-80` is 688,623 bytes, SHA-256 `cfc5786ab4fb9cbe365f4bf662be6fe62333c26e5701427264e65249fc450c90`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6BB0`** | 301 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +22 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6620` executed; `0x1F6BB0` was not entered. Frontier: **521 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6970 dependency batch advances the stop to 0x1F6620

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-58` admits the ordinary dependency `0x1F6970` (35 instructions / 119 bytes) in the merged 6-function seed, keeping 32 exports. It contains **430 functions / 18,073 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 520/0. Module `recomp-post-memory-build-79` is 688,315 bytes, SHA-256 `8baa111d9e5a1505f719a2086ccdd5952eb9a7d833194d6118208791c3467e39`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6620`** | 300 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +21 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6970` executed; `0x1F6620` was not entered. Frontier: **520 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The Path B batch advances the stop to 0x1F6970

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-57` admits the observed Path B set `0x1F5B70` + `0x1F6300` (with `0x1F85E0`, `0x1FB3B0`, `0x1F6EB0` and `0x1FC980` already translated). It contains **429 functions / 18,038 instructions**; both new bodies are real and guarded; `verify-seeds` 32/32/0/0 and `verify-translation` 519/0. Module `recomp-post-memory-build-78` is 687,271 bytes, SHA-256 `d6540c1b2259c2b9025f7e8786f7cc2510480ef0c28f3cb2675a86835cc7fa92`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6970`** | 296 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +17 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F5B70` and `0x1F6300` executed; `0x1F6970` was not entered. Frontier: **519 functions, 90 placeholders, 89 referenced**. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F6910 dependency batch advances the stop to 0x1F5B70

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-56` admits the ordinary dependency `0x1F6910` (28 instructions / 95 bytes) in the merged 3-function seed while keeping 32 exports. It contains **427 functions / 17,807 instructions**; `verify-seeds` 32/32/0/0 and `verify-translation` 518/0. Module `recomp-post-memory-build-77` is 676,813 bytes, SHA-256 `0f41fd06b47743dc7c86cbdb6f566fa4689bcc5d32ba92155e26d30309e86635`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F5B70`** | 292 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +13 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F6910` executed; `0x1F5B70` was not entered. The next stop is Path B (`0x1F5B70` + `0x1F6300` + the translated `0x1FC980`). Frontier: **518 functions, 91 placeholders, 90 referenced**; `0x1F5B70` called by executed `0x1F5640`. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The Path A admission advances the stop to 0x1F6910

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-55` admits the observed Path A set `0x1F70E0` + `0x1FC980` + table-aware `0x1FCB70` under the frozen 32-export grouping. It contains **426 functions / 17,779 instructions**; all three new functions have real guarded bodies; `verify-seeds` 32/32/0/0 and `verify-translation` 517/0. Module `recomp-post-memory-build-76` is 675,058 bytes, SHA-256 `5d596ab2c24d022b267faf4409fcd189ee62bcd059004662fdae885062dd5518`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 3 at `0x1F6910`** | 289 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +10 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F70E0` and `0x1FC980` executed; `0x1FCB70` was translated but not on this path. The next stop `0x1F6910` returns to `0x1F66F6` (call site `0x1F66F1`) from translated `0x1F66A0`. Frontier: **517 functions, 91 placeholders, 90 referenced**.

The observed stop is an ordinary untranslated routine; no further batch this cycle. The package verifier reports 32/32 seeds, 426/517/0, 13/13 digests. These are synthetic startup observations, not fidelity/gameplay/original service semantics.

## Prior: The 0x1F7080 admission observes the stop at 0x1F70E0

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-54` admits the reached `0x1F7080` leaf (27 instructions / 89 bytes, no calls/tables) after merging its seed into the 29-function front seed. It contains **423 functions / 17,620 instructions** from 32 seed exports; `verify-seeds` 32/32/0/0 and `verify-translation` 517/0. Module `recomp-post-memory-build-75` is 664,115 bytes, SHA-256 `687c34c98794d8ac71387ec1bf00aec9e88e4c23a38463746a725008b39b133f`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F70E0`** | 286 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +7 routines, +1 operation; the experiment preserves every control-reached routine. `0x1F7080` executed and returned; `0x1F70E0` is the next untranslated call target (not entered), returning to `0x1F57CB` from call site `0x1F57C6` in executed `0x1F5640`. The frontier has **94 placeholders, 93 referenced**.

The observed taken branch is Path A (`0x1F70E0` needs `0x1FC980`+`0x1FCB70`). No batch was started; the lane stops here. These are synthetic startup observations, not fidelity, gameplay or original service semantics.

## Prior: The 0x1FCCB0 admission advances the stop to 0x1F7080

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-53` admits the reached mixed-layout function `0x1FCCB0` (112-byte envelope, 8-entry table, four `ret 8` cases) after a synthetic resync fixture passed. It contains **422 functions / 17,593 instructions** from 32 seed exports. Module `recomp-post-memory-build-74` is 663,028 bytes, SHA-256 `c8c27cb4ec232cff478d16752d649767a34c30ebca45cb8c38195c552aa67657`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F7080`** | 285 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +6 routines, +1 kernel operation. The experiment reaches and executes `0x1FCCB0`. The frontier has **95 placeholders, 1 entered**: `0x1F7080` called by executed `0x1F5640` (71 called by an executed caller, 22 static-only).

`0x1F7080` is **not** admitted or analyzed further. The lane stops here. These are synthetic startup observations, not fidelity, renderer, gameplay or original service semantics.

## Prior: The 0x1FC780 admission advances the stop to 0x1FCCB0

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-52` admits the reached leaf `0x1FC780` (53 bytes / 17 instructions, one self-recursive call) and contains **421 functions / 17,575 instructions** from the same 32 seed exports. Module `recomp-post-memory-build-73` is 662,080 bytes, SHA-256 `f89016609cde54d9fd0a612d9da5cb6f6ae24d0999391760ac27ef2c9ccc7326`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1FCCB0`** | 284 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +5 routines, +1 kernel operation. The experiment reaches and executes `0x1FC780`. The frontier on the new translation has **96 placeholders, 1 entered**: `0x1FCCB0` called by executed `0x1F5640` (72 called by an executed caller, 22 static-only).

`0x1FCCB0` is **not** admitted or analyzed further. The lane stops here under the directive. These are synthetic startup observations, not fidelity, renderer, gameplay or original service semantics.

## Prior: The 0x1F6EB0 admission advances the stop to 0x1FC780

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-51` admits the reached leaf `0x1F6EB0` (455 bytes / 110 instructions, no calls) and contains **420 functions / 17,558 instructions** from the same 32 seed exports. Module `recomp-post-memory-build-72` is 660,060 bytes, SHA-256 `9d8689ff916dbed7d264e378ca23e5e8b1d26853a5a2b11fa30ca0780e42ccaf`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1FC780`** | 283 | 405 |

The comparison exits 0: same module, same full supplied profile, `avQueryWord` the only added input, +4 routines, +1 kernel operation. The experiment reaches and executes `0x1F6EB0`. The frontier on the new translation has **97 placeholders, 1 entered**: `0x1FC780` called by executed `0x1F5640` (73 called by an executed caller, 22 static-only).

`0x1FC780` is **not** admitted or analyzed further. The lane stops here under the directive. These are synthetic startup observations, not fidelity, renderer, gameplay or original service semantics.

## Prior: The 0x1F79A0 admission advances the stop to 0x1F6EB0

November 4, 2026. A bounded read-only export admitted only the reached leaf `0x1F79A0` (281 body bytes / 289-byte envelope / 95 instructions, no calls) into the retained translation. `recomp-title-recomp-title-post-memory-50` now contains 419 functions / 17,448 instructions from the same 32 seed exports, with original-byte identity, decoder agreement and 419 checked memory accesses retained.

Module `recomp-post-memory-build-71`, SHA-256 `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`:

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F6EB0`** | 281 | 405 |

The frontier on the new translation has 98 placeholders (97 referenced) and 352 call targets. The control enters 0 placeholders (66 called by an executed caller, 31 static-only). The explicit run executes 281 functions and enters exactly one placeholder: **`0x1F6EB0`, called by executed original function `0x1F83C0`** (68 called-by-executed, 28 static-only). The `0x1F79A0` function executed and returned (trace enter/exit), so the next genuine stop is `0x1F6EB0`.

This is static reachability plus the retained report; it does not prove the call edge dynamically beyond the status-2 stop. `0x1F6EB0` is **not** admitted, translated, exported or treated as progress. No further admission, framework, storage expansion or fake success is added; the geometry lane stays paused.

Reproduce with `node tools/translation-frontier.mjs research/verification/recomp-title-recomp-title-post-memory-50 --run research/verification/pfifo-setup-01/browser-71-reset-avquery.json`; the text output names `ENTERED 0x001F6EB0 called by 0x001F83C0`.

### `0x1F6EB0` call context (static)

The retained `recomp-post-memory-front-01` seed contains the already-admitted caller `0x1F83C0` (`D3DDevice_SetVertexShader`). At `0x1F8461` it issues a direct `CALL 0x001F6EB0`. Immediately before:

- `ESI` holds `[0x00206720]` and `ECX=ESI` (the device context);
- `EAX` is the buffer returned by `CALL 0x001FB3B0`; the caller writes `[EAX]=0x41EA0`, `[EAX+4]=0`, `[EAX+8]=0x81E94`, `[EAX+0xC]=6`, `[EAX+0x10]=0` and sets `EDX=[EAX+0x14]`;
- after `0x1F6EB0` returns, the caller stores `[ESI]=EAX` and calls `0x001F82C0` before its epilogue.

This is static call-site/register evidence only. The callee body is not exported, its semantics are unknown, and no dynamic edge beyond the status-2 stop is proved. `0x1F6EB0` remains unadmitted under the hold; this context is preparation for a future authorized admission, not an admission or fidelity claim.

An independent narrow GLM review (attempted `2026-11-04T09:15:22`; step-capped after completing the checks) found no material defect in the prequery fix or the admission evidence. It confirmed the experiment's `0x1F79A0` enter/exit trace, the `0x1F6EB0` status-2 stop, the 281/405 counts and the module digest; the control stop target `0xFE000094` was then confirmed directly from `browser-71-reset3.json`. The review treats `0x1F6EB0` as a static-reachability boundary, not a proven dynamic edge beyond the stop.

The source-owned admission guards were re-run against the new translation: `verify-seeds.py` reports **32 recorded / 32 offered / 0 unexpected / 0 missing** (the offered seeds match the translation's record exactly), and `verify-translation.py` reports **517 functions, 0 needing rewriting, fully instrumented**.

## Prior: The binding advances the frontier to translation target 0x1F79A0

November 4, 2026. The explicit synthetic ordinal-2 binding moves the frontier from the kernel service to a translation target. On module `recomp-post-memory-build-70` (`cd2fac97df95a4289b20742768555a080a995e2f16c774a36da25a856720a1a0`):

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F79A0`** | 280 | 405 |

The retained translation `recomp-title-recomp-title-post-memory-49` has 517 `ff_title_*` definitions (418 non-placeholder functions), 99 placeholders (98 referenced) and 352 call targets. With the control run, 279 functions execute, the run enters 0 placeholders, 66 placeholders are called by an executed caller and 32 have only unexecuted callers. With the explicit-word run, 280 functions execute and the run enters exactly one placeholder: **`0x1F79A0`, called by executed original function `0x1F83C0`**. The frontier partition is 69 called-by-executed and 28 static-only.

This is static reachability plus the retained run report; it does not prove the call edge was taken dynamically beyond the status-2 stop. `0x1F79A0` is **not** admitted, translated, exported or treated as progress. The next dependency is now a translation target rather than the kernel service; admitting it or its dependencies remains frozen pending the architect's decision. The canonical path still stops at ordinal 2, the geometry lane stays paused, and no default or fidelity claim is added.

Reproduce with `node tools/translation-frontier.mjs research/verification/recomp-title-recomp-title-post-memory-49 --run research/verification/pfifo-setup-01/browser-70-reset-avquery.json`; the text output names `ENTERED 0x001F79A0 called by 0x001F83C0`.

## Prior: Ordinal 2 remains the frontier; the option-6 model is withdrawn

November 4, 2026. The reset run stops at **status 1**, target `0xFE000094`, ordinal **2** (`AvSendTVEncoderOption`), return `0x1FD429`, with 279 routines and 404 kernel operations. The frontier reports 99 placeholders (98 referenced) and the run enters **0** of them: 66 are called by an executed caller and 32 have only unexecuted callers. The next dependency is the kernel service, not a translation target.

Module `recomp-post-memory-build-70`, SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`.

## Withdrawn option-6 model

The explicit AV-pack profile, its handler and its report fields are removed. Cxbx-Reloaded at commit `585c49a50af1255ab155099e06f24505f9c5a800` maps numeric option 6 to `AV_QUERY_AV_CAPABILITIES` and handles it, while the pinned `xboxrecomp-source` maps the same numeric 6 to `AV_OPTION_QUERY_AVPACK`. The replacement semantics differ, so option 6 is neutral and ordinal 2 is unsupported. Untranslated `0x1F79A0` is **not** reached, and no frontier expansion was performed. See [the AV encoder checkpoint](AV_ENCODER_SERVICE_CHECKPOINT.md).

## Next task

Obtain original-console or otherwise authoritative evidence for ordinal-2 option-6 semantics, or a definitive adjudication of the reference conflict, before any service model. Do not export `0x1F79A0` as grounded progress, do not invent a default pack word, and do not take a broad batch.

## Prior: the 0x1FD42F admission reaches AvSendTVEncoderOption (ordinal 2)

November 4, 2026. Translation `recomp-title-recomp-title-post-memory-49` admits the next reached helper `0x1FD42F` (49 instructions) and its direct dependency `0x1FD410` (10 instructions) for **418 functions / 17,353 instructions**. The reset run executes both and stops at **status 1** (unsupported Xbox service), target `0xFE000094`, ordinal **2**, return `0x1FD429`, with 279 routines and 404 kernel operations. The retained callers evidence names ordinal 2 as `AvSendTVEncoderOption`; no encoder behaviour is modeled.

The frontier now has 99 placeholders (98 referenced), but the run enters **0** of them: the next blocker is the kernel service, not a translation target. 66 placeholders are called by an executed caller and 32 have only unexecuted callers; none can advance this run while ordinal 2 is unsupported.

## Narrow admission

- One merged seed within the existing 32-export budget, recomposed with `--guard-memory`.
- `0x1FD42F` calls `0x1FD410`; `0x1FD410` calls ordinal 2 through IAT `0x00253450`. No other dependency was added.
- `tools/verify-seeds.py`: 32 recorded / 32 offered / 0 unexpected / 0 missing.
- `tools/verify-translation.py`: 0 functions needing rewriting.
- Module `recomp-post-memory-build-70`, SHA-256 `b6314c57b4cc8b78e73c54262237a8761e2619da92b19083fc59f174a856a4c6`.

## Measured result

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pcrtc` control | status 15 at `0xFD003214` | 276 | 403 | 32,768 |
| `pfifoReset` | **status 1 at `0xFE000094`** (ordinal 2, return `0x1FD429`) | 279 | 404 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`.

## Limits

- No encoder capabilities, mode, timing or return values are guessed.
- One helper and its direct dependency were added; no broad closure or frontier batch.
- No display, renderer, fidelity or playability.

## Next task

Do not export more frontier targets for this run: the next dependency is the kernel service. Decide whether to model a bounded, evidence-backed `AvSendTVEncoderOption` service or take another independent prerequisite. Keep the canonical profile fail-closed and the geometry lane paused.

## Prior: The 0x1FCB00 admission advances the stop to 0x1FD42F

November 4, 2026. The first evidence-based admission after the reset breakthrough is complete. Translation `recomp-title-recomp-title-post-memory-48` admits exactly the reached helper `0x1FCB00` (10 instructions, a leaf that returns `0xF` for inputs `3`/`0x1C`, otherwise reads `[EAX + 0x2024F0]` and masks `0x3C`) and contains **416 functions / 17,294 instructions**. The reset run now executes it and stops at the next reached helper, untranslated `0x1FD42F` (status 2, return `0x1FD4C9`, 277 routines, 404 kernel operations). The frontier filter reports **entered = 1** for `0x1FD42F` (called by `0x1FD4A8`), so the next admission is evidence-based.

## Narrow admission

- One seed function merged into an existing export; 32 exports total. No broad closure, frontier batch, consumer or guessed timing.
- The helper has no calls, so no dependencies were needed.
- `tools/verify-seeds.py` reports 32 recorded / 32 offered / 0 unexpected / 0 missing; `tools/verify-translation.py` reports 0 functions needing rewriting.
- The `--guard-memory` recomposition preserves the byte/decoder/layout/memory/ABI guards.
- The canonical, `engineWait` and `pcrtc` controls are unchanged.

## Measured result

Module `recomp-post-memory-build-70`, SHA-256 `2c0b69dfbf55d0de2a183fd05c951283a8cc7f847fab3d9235f89ea0434dfb22`.

| run | stop | routines | kernel ops | events |
| --- | --- | --- | --- | --- |
| canonical | status 10 at `0xFD000100` | 276 | 403 | 12,535 |
| `engineWait` control | status 12 at `0xFD003214` | 276 | 403 | 13,233 |
| `pfifoReset` | **status 2 at `0x1FD42F`** (return `0x1FD4C9`) | 277 | 404 | 12,584 |

The reset run still reports `pfifoResetState: { scope: 'seam', cache1: 16, runout: 16, dmaPush: 0, consumerUpdates: 0, resets: 1 }`.

## Limits

- This is one helper, not a closure or a playable path.
- The admitted helper's byte read at `[EAX + 0x2024F0]` is exercised only as reached; no broader table ownership is claimed.
- No engine, renderer, timing source, fidelity or playability.

## Next task

Export `0x1FD42F` (and only demonstrably necessary dependencies) from the retained project, recompose with the guard, build and rerun. Keep the canonical profile fail-closed.

## Prior: the reset run enters one frontier placeholder: 0x1FCB00

November 4, 2026. With the default-off `pfifoReset` experiment the run leaves the PFIFO wait and stops at untranslated `0x1FCB00` (status 2, return `0x1FD4C4`). The reachability filter reports **entered = 1** (`0x1FCB00` called by `0x1FD4A8`), 68 called by an executed caller and 32 with only unexecuted callers. This is the first evidence-based next admission target; **no frontier expansion was performed this cycle**.

## Source-owned tool fix

A status-2 stop target is the untranslated routine itself and does not appear in `originalFunctionsReached`. `parseRunEvidence` now adds it to the entered set, and `tests/translation-frontier.test.mjs` covers a stop target absent from the reached set.

## Measured result

| item | pre-reset | reset |
| --- | --- | --- |
| executed functions | 276 | 276 |
| placeholders entered | 0 | **1 (`0x1FCB00`)** |
| called by an executed caller | 66 | 68 |
| only unexecuted callers | 35 | 32 |

The reset run executes `0x1FD4A8`, which statically calls `0x1FCB00`; the run enters the placeholder and stops with status 2. The canonical profile still reports 0 entered.

## Limits

- "Called by an executed caller" remains a static upper bound.
- The entered target is one run's evidence; a different profile or a later run changes it.
- The filter does not translate, build, execute, or prove fidelity.

## Next task

Export `0x1FCB00` and its closure, recompose with the guard, build, and rerun with the `pfifoReset` profile. Do not export the remaining untaken-branch targets.

## Prior: pre-reset static-only run

November 4, 2026. The static frontier is now intersected with a retained run report, so the next translation batch can be scoped by executed callers instead of every static target. Against `browser-70.json` (276 executed functions), **0 of 101 referenced placeholders were entered**, 66 have at least one statically-calling function that executed, and 35 have only unexecuted callers. The 66 are untaken branches, not reachable calls: the run stops at the PFIFO wait, not at an untranslated routine. **There is no engine, renderer, timing source or playable screen.**

## Source-owned change

`tools/translation-frontier.mjs` gains `--run <report.json>` and the exported `parseRunEvidence` / `analyzeReachability`:

- accepts `originalFunctionsReached` and/or a `trace`; validates the report object, address range and `enter`/`exit` kinds;
- uses only entered addresses from the trace, because the retained traces contain unmatched exits from tail calls and returns; it deliberately does not reconstruct a call stack or claim dynamic call edges;
- partitions each referenced placeholder into `entered`, `calledByExecutedCaller` (a static upper bound) and `staticOnly`;
- reports the executed-function count and warns when the trace reached its declared capacity;
- `--json` includes `reachability`; the human output prints counts and `ENTERED` / `EXECUTED-CALLER` addresses only.

`tests/translation-frontier.test.mjs` adds five cases (9 total): the three-way partition, malformed addresses and event kinds, tolerance of an unmatched exit, a trace-only report with truncation, and a CLI test that proves `--run` is not mistaken for the translation path. No original code is reproduced.

## Measured result

Report `browser-70.json`, translation `recomp-title-recomp-title-post-memory-46`:

| item | count |
| --- | --- |
| translation functions (including 102 placeholders) | 517 |
| referenced placeholders | 101 |
| executed functions | 276 |
| placeholders entered | **0** |
| called by an executed caller (untaken branch) | 66 |
| only unexecuted callers | 35 |

The 66 cluster on `0x2CD80` (62), with singletons from `0x34307` (`0x341D0`), `0x1E0DAB` (`0x1E0E52`), `0x200F59` (`0x20010A`) and `0x42BE0` (`0x202150`). Because the run enters none of them, admitting any of these placeholders cannot advance the current execution; it would only prepare untaken branches. The current blocker remains PFIFO consumption, owned by the engine workstream.

## Limits

- "Called by an executed caller" is a static upper bound: the caller's branch may not be taken.
- The executed set is one profile's run; a different profile or a later run changes it.
- The filter does not prove reachability from the current stop, decode a call stack, translate, build or execute anything.
- No engine, renderer, timing source, rendering, fidelity or playability.

## Validation

`tests/translation-frontier.test.mjs` 9/9; runtime suite 131/131; `npm run test:evidence` 4/4; `npm run test:native` 14/14; title-seed 25/25; `npm run check` and whitespace checks pass.

## Next task

Wait for a run that actually enters a placeholder (then `entered` is non-empty) before exporting that address, or for the engine workstream to unblock PFIFO consumption. Do not export the 66 untaken-branch targets as if they were reachable.

## Static inventory

November 4, 2026. An inventory of the boundary placeholders in the current translation quantifies the remaining admission work. It is a planning datum only: nothing here is a fidelity or progress claim, and no new routine was translated. **There is no engine, renderer, timing source or playable screen.**

## What was measured

The regenerated translation (`recomp-title-recomp-title-post-memory-46`, 415 functions / 17,284 instructions) emits one placeholder per known boundary: `void ff_title_XXXXXXXX(void) { ff_probe_stop_direct(0xXXXXXXXXu); }`. Counting those against the call graph of the admitted functions gives:

| item | count |
| --- | --- |
| boundary placeholders (known-untranslated routines) | **102** |
| placeholders that are direct call targets of admitted code | **101** |
| distinct call targets across admitted code | 352 |

Nearly every known boundary is already referenced by admitted code, so these are the next translation frontier rather than hypothetical gaps. A run that reached one would stop with status 2 (untranslated original routine); the current runs do not, because the 276 reached routines stay inside the admitted set.

## Why this matters for the blocked path

The reached loop's exit needs PFIFO engine state (unassigned owner), but when that unblocks, execution moves into code whose callees include 101 known-untranslated routines. This list lets the next translation batch be scoped from evidence instead of being rediscovered at the stop.

## Inventory

Placeholder addresses from the private translation — addresses only; no original code, bytes or disassembly is reproduced:

```
  0x00013A10, 0x00018350, 0x00026C60, 0x00027540, 0x0002B470, 0x0002B4B0
  0x0002B520, 0x0002B640, 0x0002B790, 0x0002B870, 0x0002B8A0, 0x0002C020
  0x0002C270, 0x0002C400, 0x0002C460, 0x0002C520, 0x0002C5B0, 0x0002C680
  0x0002C8E0, 0x0002CC90, 0x0002FA20, 0x00030603, 0x0003080D, 0x00030943
  0x0003097D, 0x00031227, 0x000341D0, 0x00036E70, 0x000393E0, 0x00041EE0
  0x00042530, 0x00042570, 0x00042590, 0x00043440, 0x000438B0, 0x00043B70
  0x00043D20, 0x00047070, 0x00047CB0, 0x00047DE0, 0x00048D60, 0x0004AE80
  0x0004B110, 0x0004B160, 0x0004B210, 0x0004B230, 0x0004B540, 0x0004B620
  0x0004B7A0, 0x0004BB30, 0x0004BE40, 0x0004C480, 0x0004C5F0, 0x0004D4F0
  0x0004D620, 0x0004EED0, 0x0004F710, 0x0004F7A0, 0x0004F860, 0x0004F890
  0x0004F8B0, 0x0004F8C0, 0x0004FF30, 0x00050150, 0x00050250, 0x000502A0
  0x00055240, 0x000559A0, 0x00071B10, 0x000762E0, 0x00082860, 0x00082870
  0x00082A00, 0x000A6FA0, 0x001DF03E, 0x001E056D, 0x001E0E52, 0x001E2038
  0x001E50E3, 0x001E55A3, 0x001E6DA9, 0x001E6DED, 0x001F54D0, 0x001F5B70
  0x001F6EB0, 0x001F7080, 0x001F70E0, 0x001F79A0, 0x001F8090, 0x001F80F0
  0x001FB320, 0x001FC580, 0x001FC780, 0x001FC910, 0x001FC980, 0x001FCB00
  0x001FCB70, 0x001FCCB0, 0x001FD410, 0x001FD42F, 0x0020010A, 0x00202150

## Limits

- The list reflects the current seed set; a different set changes it.
- Being referenced is not the same as being reachable on the current runtime path: static call targets include conditional branches today's run never takes.
- No placeholder was translated, no seed added, and no module rebuilt for this audit.

## Validation

Documentation-only change; the standing suites still pass (`npm run test:evidence` 3/3, `npm run test:native` 14/14, runtime suite 114/114, title-seed 25/25, `npm run check`, whitespace).

## Next task

Scope the next translation batch from this inventory — largest referenced clusters first — and keep the PFIFO engine/consumer assignment separate, since neither substitutes for the other.
