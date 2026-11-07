# Project status: startup requests the written FATX file again

Observed November 6, 2026 in Chrome 156.0.8078.80 on Windows. The original-startup experiment closed the written FATX file and reached **status 9 at ordinal 190 (`NtCreateFile`), value `0xC00000BB`**, with return address `0x001E950C`, 402 distinct routines and 487 completed kernel operations. The stopped `FILE_OPEN_IF` request targets the same debug-log path. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

## Matching baseline and guarded preparation

The private baseline is available on the research workstation. Its XBE matches the [reference digest](reference.md), and the retained analysis project, seed inputs and prepared artifacts are present. The 32 seeds and dependency metadata matched the recorded inputs for both profiles. The pinned recompilation source, Capstone 5.0.6 and Emscripten 3.1.50 were verified locally.

| Profile and module | Admitted functions / instructions | Wasm bytes | SHA-256 prefix |
| --- | --- | --- | --- |
| Baseline 96 / 171, recorded at `ab5b3f7` | 487 / 20,115 | 1,045,793 | `57576342` |
| Candidate 97 / 172 | 488 / 20,251 | 1,051,395 | `56f848b6` |

The current translator reproduced both generated outputs for profile 97 byte for byte from the retained Ghidra exports. The delta is one established routine and 136 instructions. Its local callees were already admitted. Preparation retained the 32-export, 512-function and 32,768-instruction limits, checked memory, byte identity and decoder checks with the existing x87 boundary normalization. Rebuilding with the pinned compiler reproduced module 172 byte for byte.

## Browser replay and independent review

The allowlisted loopback harness loaded the XBE, Wasm and synthetic settings through local file selection. Six fresh Chrome worker runs reproduced the baseline and candidate boundaries below. The older saved comparisons used a Node worker shim.

| Run | Baseline stop and routines / kernel operations | Candidate stop and routines / kernel operations |
| --- | --- | --- |
| Fixed control, module 156 | Status 13 at `0xFD800044`, 339 / 405 | Same |
| Inert, modules 171 and 172 | Status 1 at `0xFE0000C8`, ordinal 100, 347 / 419 | Same |
| Experiment, modules 171 and 172 | Status 2 at `0x001E93E4`, 363 / 441 | Status 9, ordinal 190, value `0xC00000BB`, 364 / 442 |

Independent review confirmed the unchanged control and inert stop objects, original-entry sequences, trace kind/address/stack sequences and kernel ordinal/status sequences. The experiment retained the baseline prefixes, entered `0x001E93E4` and reached ordinal 289 successfully before stopping at ordinal 190. The failing request reports return address `0x001E94CC`, desired access `0x40100080` and create options `0x60`. The report identifies the service boundary and the out-of-range disposition. The exact internal return branch remains unobserved.

## FATX create batch

The browser now preserves the stopping create request alongside the earlier successful create. The trapped call used DOS root `0xFFFFFFFD`, disposition 5 (`FILE_OVERWRITE_IF`), desired access `0x40100080`, share 3 and options `0x60`. The [XboxDev declarations](https://github.com/XboxDev/nxdk/blob/master/lib/xboxkrnl/xboxkrnl.h) identify `FILE_OVERWRITE` as disposition 4.

The candidate retains profile 97 and adds the reached missing-file case to the owned FATX mount. It checks guest ranges and aliasing, follows the declared DOS link, preflights the fixed handle pool, and writes one empty root-directory entry without exposing a stale slot past the end marker. Existing targets and other file operations remain guarded. Two pinned compiler builds produced SHA-256 prefix `4f32c34c`.

Fresh Chrome comparisons preserved the fixed control and inert stop objects and stable traces. The experiment preserved its previous entry and trace prefixes. Ordinal 190 returned status 0, information 2 (`FILE_CREATED`) and owned handle `0x00070004`. The mount revision advanced by one with the same disk page and free-cluster counts. Independent review verified the ABI, refusal guards and comparison evidence. Raw reports, preparation outputs and audits remain in ignored local research.

## CRT admission batches

| Batch | Prepared functions / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / kernel operations |
| --- | --- | --- | --- | --- |
| FATX create, profile 97 | 488 / 20,251 | `4f32c34c` | Status 2 at `0x001E71D8`, return `0x001E779C` | 364 / 443 |
| Admit `0x001E71D8` | 489 / 20,272 | `56194195` | Status 2 at `0x001E731B`, return `0x001E78B1` | 366 / 443 |
| Admit `0x001E731B` | 490 / 20,282 | `ee740d85` | Status 2 at `0x001E4C85`, return `0x001E01AB` | 368 / 444 |
| Admit `0x001E4C85` | 491 / 20,299 | `f00f734b` | Status 2 at `0x001E4C33`, return `0x001DFEB1` | 370 / 445 |
| Admit `0x001E4C33` | 492 / 20,316 | `92989b85` | Status 2 at `0x001DFE39`, return `0x001DFEBC` | 371 / 446 |
| Admit `0x001DFE39` | 493 / 20,346 | `2abdfcf7` | Status 2 at `0x001E4E1E`, return `0x001DFE4E` | 372 / 446 |
| Admit `0x001E4E1E` | 494 / 20,386 | `cfaf18f9` | Status 2 at `0x001E4DF3`, return `0x001DFE56` | 373 / 446 |
| Admit `0x001E4DF3` | 495 / 20,403 | `0cb93518` | Status 2 at `0x001E4D58`, return `0x001DFE5E` | 374 / 446 |
| Admit `0x001E4D58` | 496 / 20,450 | `974d3c16` | Status 2 at `0x001E4CD7`, return `0x001E4DA9` | 376 / 447 |
| Admit `0x001E4CD7` | 497 / 20,502 | `950a72fb` | Status 2 at `0x001E7267`, return `0x001E4CE3` | 377 / 447 |
| Admit `0x001E7267` | 498 / 20,521 | `042793d0` | Status 2 at `0x001E721E`, return `0x001E4D2B` | 378 / 448 |
| Admit `0x001E721E` | 499 / 20,543 | `7df18ca6` | Status 2 at `0x001E4DCF`, return `0x001E4DC7` | 379 / 448 |
| Admit the direct entry at `0x001E4DCF` | 499 + 1 direct entry / 20,547 | `e9c8e3aa` | Status 2 at `0x001DFECE`, return `0x001DFEC9` | 380 / 449 |
| Admit `0x001DFECE` | 500 + 1 direct entry / 20,551 | `752ada81` | Status 2 at `0x0004B230`, return `0x0004B4A1` | 381 / 450 |
| Admit `0x0004B230` | 501 + 1 direct entry / 20,659 | `1296fa86` | Status 2 at `0x001E041F`, return `0x0004B2B8` | 382 / 450 |
| Admit `0x001E041F` | 502 + 1 direct entry / 20,694 | `41d1f423` | Status 2 at `0x00031227`, return `0x0004B2D3` | 383 / 450 |
| Admit `0x00031227` and guard INT `0x2D` | 503 + 1 direct entry / 20,714 | `e756b0ce` | Status 9, ordinal 190, value `0xC00000BB`, return `0x001E950C` | 384 / 457 |

Read-only Ghidra exports added one established routine per batch. When the cumulative seed group reached 32 entries, later exports were appended to a group with room. Existing rows and the 32-export, 512-function and 32,768-instruction limits were preserved. The initial `0x001E4D58` export failed the complete-layout guard on three undefined bytes. The matching XBE's exception scope record points to that address. Bounded read-only disassembly established one handler instruction, preserved all 46 prior rows and passed the existing byte, decoder, layout and memory checks.

Independent review confirmed unchanged control and inert stops, entry sequences, trace kind/address/stack and kernel ordinal/status/argument sequences. The experiment preserved those prefixes across the batches. Clock-dependent outputs varied between runs.

The `--crt-cleanup-entry` option uses four verified instructions from the existing `0x001E4D58` envelope. It admits the single original near call at `0x001E4DC2`, rejects additional incoming edges and retains the existing overlap rule. The emitted view charges one function slot and four instructions to the existing budgets. It uses checked stack operations and is excluded from indirect lookup. Chrome recorded the entry returning after its already admitted callee completed ordinal 294. Both default preparation and the reviewed opt-in preparation reproduced their respective generated outputs and manifests byte for byte.

The newly reached SEH epilog at `0x001E0505` produced one additional aggregate ABI diagnostic. The reviewed trace shows the owner restoring its entry stack and exception-chain head. The probe's legacy slots 3–5 record stack changes among logged calls, and the browser currently labels those slots as ESI. The saved trace does not resolve the diagnostic to an individual nonvolatile register.

The runtime checks passed 200/200, native checks passed 56/56 including sanitizer fixtures, and Python evidence suites passed 6/6. The replay harness and local checks used the workstation's Node 24.22.1 and npm 11.19.1. CI on the repository's pinned Node version remains pending for these local commits. The browser comparisons and independent reviews are retained locally.

## Debug print and FATX reopen

The opt-in `FF_DEBUG_SERVICE_PROOF` handler serves only INT `0x2D` service 1. It checks counted-ANSI descriptor and payload ranges through the guest VM, caps completed requests at eight, and exports bounded metadata. Chrome observed one 43-byte request with status zero. The handler leaves guest registers and memory untouched and does not expose message bytes. The fixed control and inert stops and stable sequences were preserved.

The reached ordinal 190 request used disposition 3 (`FILE_OPEN_IF`) with the same path, DOS root, access, attributes, share and options as the earlier successful create. The earlier file handle had closed. The reopen requires one case-insensitive match to an empty archive entry with no allocation in the backed, single-cluster root. A missing or duplicate entry, unbacked directory data, a different target shape or any live file object on the mount keeps the service guarded.

With the same 503 seeds, one direct entry and 20,714 emitted instructions, module `ecd8918d` served the unchanged request with status 0, information 1 (`FILE_OPENED`) and handle `0x00070008`. The mount stayed at revision 72 with 65,518 free clusters. Two ordinal 294 calls completed before the new stop, with the reopened handle still active. The fixed control and inert runs matched their prior stops, original entries, trace kind/address/stack sequences and kernel arguments/status. The experiment matched its earlier prefixes through the request whose result changed. Independent review and comparison reports are retained locally.

Native checks passed 58/58 including sanitizers, runtime checks passed 201/201, and `npm run check` passed for the reopen.

## CRT admission after the FATX reopen

| Batch | Prepared functions / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / kernel operations |
| --- | --- | --- | --- | --- |
| FATX reopen | 503 + 1 direct entry / 20,714 | `ecd8918d` | Status 2 at `0x001E0C90`, return `0x0004B302` | 384 / 460 |
| Admit `0x001E0C90` | 504 + 1 direct entry / 20,741 | `1cb37877` | Status 2 at `0x001E61C3`, return `0x001E0CB3` | 385 / 461 |
| Admit `0x001E61C3` | 505 + 1 direct entry / 20,794 | `7a2a2f0e` | Status 2 at `0x001E6DED`, return `0x001E61D0` | 386 / 461 |
| Admit `0x001E6DED` | 506 + 1 direct entry / 20,807 | `293cb08e` | Status 2 at `0x001E6DA9`, return `0x001E2300` | 388 / 461 |
| Admit `0x001E6DA9` | 507 + 1 direct entry / 20,826 | `eb2cbb53` | Status 2 at `0x001E50E3`, return `0x001E2361` | 389 / 463 |
| Admit `0x001E50E3` | 508 + 1 direct entry / 20,877 | `49d9af57` | Status 2 at `0x001E5071`, return `0x001E513A` | 390 / 464 |
| Admit `0x001E5071` | 509 + 1 direct entry / 20,920 | `7c6bae9f` | Status 2 at `0x001E90D0`, return `0x001E50A3` | 391 / 464 |
| Admit `0x001E90D0` | 510 + 1 direct entry / 21,024 | `53fb1b8c` | Status 9, ordinal 211, value `0xC00000BB`, return `0x001E9112` | 392 / 464 |

Each admission used a single read-only Ghidra export, checked complete layout and byte identity against the original XBE, and passed the pinned Capstone decoder and checked memory preparation. Independent review confirmed the four fresh Chrome triples preserved the fixed control and inert stops and stable sequences, and each experiment preserved the preceding experiment's entry, trace kind/address/stack and kernel ordinal/status/argument prefixes. The FATX reopen handle remained live and its mount revision and allocation counts stayed stable. One additional aggregate ABI event in the first batch coincides with the already admitted SEH prolog at `0x001E04CC`. Two in the last batch coincide with another prolog and epilog pair at `0x001E04CC` and `0x001E0505`. The legacy ESP-change counter increased alongside those events. The trace identifies stack changes in those frames, while the aggregate diagnostic remains unresolved at the individual nonvolatile-register level.

The original export of `0x001E50E3` failed complete-layout verification at the three-byte gap at `0x001E5167`. The original XBE's exception scope record at `0x0027C7A0` points to that handler. A bounded read-only decode added one instruction, preserved all 50 prior rows and both body ranges, and passed byte, decoder and layout checks. The separate direct target at `0x001E516A` remains guarded. Independent review confirmed that each of the three further Chrome comparisons preserved the control and inert stops and stable sequences, along with the preceding experiment's stable prefixes. Debug and FATX state stayed stable. Only the first of these batches added an aggregate ABI event and matching stack diagnostic at the reached SEH prolog.

## FATX file-information query

Ordinal 211 queried `FileNetworkOpenInformation` for the reopened handle `0x00070008` with a 56-byte buffer. The provider ties the handle to its owned directory slot and opening revision, rechecks the backed empty archive entry, and validates the aligned, nonoverlapping guest output ranges. The writer's all-ones FATX timestamp placeholders yield four explicit synthetic zero times. Other timestamp encodings stay guarded. The class and fields are defined in the [XboxDev declarations](https://github.com/XboxDev/nxdk/blob/master/lib/xboxkrnl/xboxkrnl.h).

With the same translation profile, module `06aa7f0f` returned status 0 and IOSB information 56. The query reported zero allocation and end of file, archive attributes `0x20` and zero padding. The reopened handle stayed live, and mount revision 72 and 65,518 free clusters stayed stable. The fixed control and inert observations matched, and the experiment preserved its stable prefix up to the same ordinal 211 request. Independent review passed. The saved trace has no immediate post-query register sample. Native checks passed 58/58 including sanitizers, and runtime checks passed 201/201.

## FATX file-position request

Ordinal 226 (`NtSetInformationFile`) requested `FilePositionInformation` on the same handle with an eight-byte signed offset. Retained call edges and the reached origin-2 branch show a zero displacement added to the queried EOF. Chrome directly captured both offset words as zero. The provider checks the current owned empty-file entry, synchronous open options, exact class and length, aligned input and IOSB ranges, and aliasing before setting the handle position. The response has status and IOSB information zero. [XboxDev declarations](https://github.com/XboxDev/nxdk/blob/master/lib/xboxkrnl/xboxkrnl.h)

Module `be0e7ac4` advanced the guest stack by 24 bytes for the five-argument call and return address. The reopened handle remained live and FATX revision 72 and allocation counts stayed stable. Independent review confirmed the fixed control and inert runs matched and the experiment preserved its stable prefix through the same five arguments. A legacy module replay preserved the earlier unmodeled stop and its report shape. Native checks passed 58/58 including sanitizers, and runtime checks passed 201/201.

## Checked CRT seek cleanup entry

The separate `--crt-seek-cleanup-entry` option admits four checked instructions at `0x001E516A` inside the verified `0x001E50E3` envelope. It validates the single original direct call at `0x001E515D`, the nested call to `0x001E731B`, and the exact stack and return shape. Both direct views remain outside indirect lookup. Using only the earlier flag reproduced the existing generated C, header and manifest byte for byte.

Module `f8c3352a` executed the view, returned to its owner and completed another ordinal 294 call. The fixed control and inert observations matched, and the experiment preserved the stable prefixes. The SEH epilog added one aggregate ABI event with a matching stack diagnostic. Independent review passed, and the Python evidence suites passed 6/6.

| Batch | Prepared functions / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / kernel operations |
| --- | --- | --- | --- | --- |
| Second direct entry | 510 + 2 direct entries / 21,028 | `f8c3352a` | Status 2 at `0x001E624B`, return `0x001E0CCF` | 393 / 467 |
| Remove one unused seed | 509 + 2 direct entries / 21,019 | `6b947ada` | Same | 393 / 467 |
| Admit `0x001E624B` | 510 + 2 direct entries / 21,034 | `33af364f` | Status 2 at `0x001E0CE4`, return `0x001E0CDB` | 394 / 467 |

The removed seed group contains only `0x001F65F0`, which had no selected static incoming reference or saved entry event. The reduced profile preserved all 625 retained definitions, the import bindings and both direct views. Chrome reproduced the control, inert and experiment stops and stable sequences before the freed slot was used. The next build admitted only `0x001E624B`, independently checked its 15 instruction rows and preserved the prior comparison prefixes. These observations cover the saved paths.

## Compact profile and subsequent CRT admissions

The compact profile retains observed entries, both checked direct entry contracts, the mixed-layout owners, and every source owner of a retained kernel binding. It also retains each already admitted direct callee and local tail-jump target of those routines. The private selection record maps the unchanged retained seed rows to their original export digests. Preparation preserved all 544 retained generated definitions, all 52 import bindings and their data and code-pointer categories. Indirect lookup has fewer entries, with the existing unsupported-target behavior.

Chrome reproduced the complete stops and stable sequences for the fixed control, inert and experiment cases. A separate pair also compared the full and compact modules directly with the control settings. The paired runs matched stable debug, FATX and ABI observations. Clock-dependent fields varied, including one observed PTIMER register value at the same read site. The reduction is verified for these saved paths.

| Batch | Prepared functions / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / kernel operations |
| --- | --- | --- | --- | --- |
| Compact baseline | 436 + 2 direct entries / 20,351 | `6e5f25fa` | Status 2 at `0x001E0CE4`, return `0x001E0CDB` | 394 / 467 |
| Admit `0x001E0CE4` | 437 + 2 direct entries / 20,355 | `cb48d3aa` | Status 2 at `0x001E55A3`, return `0x001E4E4F` | 395 / 469 |
| Admit `0x001E55A3` | 438 + 2 direct entries / 20,406 | `81d23f1e` | Status 2 at `0x001E5418`, return `0x001E55FA` | 396 / 470 |
| Admit `0x001E5418` | 439 + 2 direct entries / 20,546 | `a83b4cc0` | Status 2 at `0x001E7924`, return `0x001E5466` | 397 / 470 |

The original scope record at `0x0027C7C0` identifies the three-byte handler gap at `0x001E5627`. Bounded read-only decoding recovered one instruction, preserved all 50 existing rows and both body ranges, and passed original-byte, complete-layout and decoder checks. The separate direct target `0x001E562A` stays guarded. Independent review confirmed the stable comparison prefixes through the new batches.

## FATX information alignment and first reached write

| Batch | Prepared functions / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / kernel operations |
| --- | --- | --- | --- | --- |
| Admit `0x001E7924` | 440 + 2 direct entries / 20,597 | `9080381e` | Status 9, ordinal 211, value `0xC000000D`, return `0x001E9112` | 398 / 470 |
| Accept four-byte-aligned query output | Same | `d1df27ac` | Status 9, ordinal 226, value `0xC000000D`, return `0x001E9195` | 398 / 471 |
| Accept four-byte-aligned position input | Same | `e1398cbc` | Status 2 at `0x001E8F76`, return `0x001E54E6` | 398 / 472 |
| Admit `0x001E8F76` | 441 + 2 direct entries / 20,687 | `c5c487f5` | Status 9, ordinal 236, value `0xC00000BB`, return `0x001E9004` | 399 / 472 |

The second file-information wrapper uses four-byte-aligned guest stack buffers at `0x03C1079C` for class 34 and `0x03C107DC` for class 14. These were refused by the earlier eight-byte rule. Both providers now check four-byte alignment, as the local-file query already did, and retain checked ranges, nonoverlap, file identity and the original request restrictions. Chrome observed the same 56-byte metadata and a successful seek with both offset words zero and a 24-byte stack advance. FATX revision, allocation and handle state stayed stable. Independent review and 58/58 native checks including sanitizers passed.

The `0x001E8F76` admission added one verified ordinal 233 binding for an unreached call site. The reached write uses the existing ordinal 236 binding. It requests 45 bytes on FATX handle `0x00070008` at its current position, with null event, APC and explicit byte-offset arguments.

## First FATX file write

The guest write path checks the synchronous writable file, its current owned empty entry and zero cursor, the complete guest buffer and IOSB, and nonoverlap. It admits a first nonempty write of at most one cluster. The shared allocation helper preflights backed directory/FAT metadata, sparse-page capacity and all three revision increments. Data and FAT stores precede an eight-byte extent update that preserves the existing name, attributes and timestamp bytes. Cursor, free-cluster count and mount/file revisions update after those stores.

Module `5a7d6f5d` returned status 0 and IOSB information 45. Chrome independently read the written backing and matched it to the guest input without reporting its bytes. The file's cursor and size are 45, with one 16,384-byte cluster allocated. Revision advanced 72 to 75, free clusters decreased 65,518 to 65,517, and backed pages increased seven to eight. The fixed control and inert observations matched, and the experiment preserved its stable prefixes through the same write request. Independent review passed. The EAX and 36-byte stack-return effects are source-verified without an immediate post-service register sample. Native checks passed 58/58 including sanitizers, and runtime checks passed 201/201.

## Third checked cleanup entry

The `--crt-write-cleanup-entry` option validates the original near call at `0x001E561D`, the four-instruction direct entry at `0x001E562A` and its nested call to `0x001E731B`. It reuses the existing owner, byte, decoder, incoming-edge, stack and resource checks. All three entries stay outside indirect lookup. Preparation with only the earlier two flags reproduced its generated C, header and manifest byte for byte.

Module `a36e2a07` returned through the new entry, continued startup and closed handle `0x00070008` successfully. The next disposition-3 request has the same counted path and numeric request shape as the earlier empty-file reopen. The mount remains at revision 75, with no live FATX file object. The existing reopen guard requires an empty entry, so the written file is the next bounded dependency. A second checked debug request completed, reporting 42 bytes of metadata only. The aggregate ABI count and legacy ESP-change count each increased by 13 as further frames executed. Individual nonvolatile-register attribution remains unresolved.

Independent review passed. Actual worker execution records identify `gpt-6-luna` with low reasoning for the synthetic fixture and `gpt-6.1-sol` with high reasoning for the independent audit. The Python evidence suites passed 6/6.

Preserve the 45-byte contents, extent and revision when admitting that reopen. The profile uses 31 exports, 444 of 512 function slots and 20,691 of 32,768 instructions. The bounded probe currently retains 488 kernel-call rows out of 512.

M1 and the other project milestones remain unaccepted. The experiment is bounded startup with explicit synthetic service inputs. Original presentation, a controllable original screen and gameplay acceptance are still pending.
