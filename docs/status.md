# Project status: startup continues through original time conversion

Observed November 6, 2026 in Chrome 156.0.8078.80 on Windows. Original startup now reaches **status 2 at untranslated routine `0x001E8E74`**, return address `0x0004C526`, with 411 distinct routines and 605 completed kernel operations. The current comparison profile explicitly supplies synthetic UTC OS settings and a fixed system-time fixture while retaining the live monotonic clock. Live-clock runs reach the same stop with time-dependent formatting traces. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

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

## Written-file services and capture ceiling

| Batch | Module SHA-256 prefix | Browser experiment stop | Routines / completed kernel operations |
| --- | --- | --- | --- |
| Reopen written single-cluster file | `9fade11f` | Status 9, ordinal 211, `0xC00000BB` | 402 / 494 |
| Query written extent | `9c4cc5e8` | Status 9, ordinal 226, `0xC00000BB` | 402 / 495 |
| Seek within the written extent | `fee97ce6` | Status 9, ordinal 236, `0xC00000BB` | 402 / 502 |
| Append within the allocated cluster | `af157649` | Status 4, target 0, attempted ordinal 277, return `0x001E6388` | 402 / 511 |

The reopen validates a unique archive entry with paired empty fields or a bounded single-cluster extent and a backed FAT end marker. Sparse data retains the owned disk's zero-read behavior. It allocates a fresh handle with a zero cursor while preserving all backing. The class 34 query returns allocation and EOF from that entry, and class 14 admits a nonnegative position within the extent.

Chrome observed the reopened handle `0x0007000C`, allocation 16,384 and EOF 45, then captured a seek to offset 45. The next 43-byte write appended within the existing cluster, producing EOF and cursor 88. Revision advanced 75 to 77, with the same 65,517 free clusters and eight backing pages. The append's backing equality observation covers the appended span. Source review and synthetic readback fixtures cover preservation of the earlier prefix. The handle then closed successfully. Fixed control and inert stops and stable sequences matched, and each experiment preserved the prior stable prefixes. Native checks passed 58/58 including sanitizers, and runtime checks passed 201/201.

That stop was the probe's evidence ceiling. Window zero retains all 512 kernel-call rows; its attempted next critical-section call has no row or supplied result. The profile uses 31 exports, 444 of 512 function slots and 20,691 of 32,768 instructions. Kernel record storage and its auxiliary observation arrays remain fixed at 512, and trace storage remains fixed at 8,192 rows.

## Bounded capture continuation

The optional ABI 25 capture profile selects start 0 or 512. A second-window run executes and hashes the first 512 events, saves their digest and success count, then reuses the fixed record and indexed auxiliary arrays for at most 512 further events. Total admission stops at 1,024 events, with the independent trace limit unchanged. Callback polling precedes row allocation; semantic AV-setter history and the copy-service counter survive rotation.

The canonical SHA-256 stream records the domain, ordinal, original argument count and original arguments, then status. The digest excludes output metadata and clocks. Legacy displayed argument arrays sometimes include overwritten outputs, so they cannot reconstruct this canonical stream. A suffix report explicitly omits prior call rows and carries their digest and completion count; it does not claim a complete call list. Native fixtures verify the digest against an independent expected value, rotation, finalization and the 1,024-event refusal boundary.

Two pinned-compiler builds reproduced module SHA-256 prefix `aabcd951`. Fresh Chrome window-zero runs matched the earlier fixed control, inert and experiment stops, original entries, trace kind/address/stack sequences and displayed kernel arguments/status. The corresponding stable backend observations matched with clock-derived disk serials excluded. Separate control runs using the candidate module at both window selections matched each other and the fixed-control sequences. The inert case stops before rotation and retains its complete first window.

The experiment's second window retains 59 call rows, 571 total events and 570 completed operations. Its prefix digest matches the first-window digest `14fcb4cd259e4482e53b54e82a3bc6f08f13631a45e23f1563cc40ab4e32a785`; all 5,976 earlier trace events and 3,011 original-entry events remain prefixes. Two more reopen/seek/append/close cycles advanced EOF from 88 to 129 to 172 and mount revision from 77 to 81, with the same cluster, 65,517 free clusters and eight backing pages. No FATX handle remains live. Repeated second-window runs matched the stop, digests and stable sequences.

Independent Sol High correctness review passed. Native checks passed 62/62 including sanitizers, runtime checks passed 207/207, and source/publication contracts passed. Local checks used Node 24.22.1 and npm 11.19.1; CI at the pinned Node 24.14.0 remains pending. Chrome directly verifies rotation through event 571; the hard 1,024-event ceiling has source and synthetic fixture coverage. Subsequent bounded admissions are recorded below.

## Reached time and settings helpers

Five successive admissions exported only the exact Chrome-reached functions from the retained read-only Ghidra project. Each retained original-byte identity, complete layout, independent instruction decoding and the existing x87 boundary normalization. Prior functions, all three checked direct views, import categories and admission limits stayed unchanged.

| Admission | Prepared seeds + views / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / completed kernel operations |
| --- | --- | --- | --- | --- |
| `0x0004FA60` | 442 + 3 / 20,716 | `a7ddf426` | Status 2 at `0x001E8DE0`, return `0x0004FA6E` | 403 / 570 |
| `0x001E8DE0` | 443 + 3 / 20,755 | `3512ded4` | Status 2 at `0x001E8D8A`, return `0x001E8DF9` | 404 / 571 |
| `0x001E8D8A` | 444 + 3 / 20,789 | `f90975d5` | Status 2 at `0x001E8C2E`, return `0x001E8D9E` | 405 / 571 |
| `0x001E8C2E` | 445 + 3 / 20,930 | `9bc06e19` | Status 2 at `0x001E8892`, return `0x001E8C45` | 406 / 571 |
| `0x001E8892` | 446 + 3 / 20,983 | `a61a9f14` | Status 9, ordinal 24, index `0xFF`, return `0x000309C9` | 407 / 571 |

Fresh Chrome triples preserved fixed-control and inert stops and stable sequences; each experiment retained its predecessor's entry, trace and captured-call prefixes. An additional final candidate-module control matched the prior candidate control and its stable backend observations. A repeat final triple reproduced the new stop and stable sequences. Two pinned-compiler builds reproduced the final module byte for byte. The final report retains 61 suffix call rows and 573 total events, with the same first-window prefix digest. The new successful kernel operation is a system-time query; its clock-derived output is not a stable comparison field.

The first new routine's signed divide has a source-established unsigned 16-bit dividend and constant divisor 100, excluding negative-shift, zero-divisor and quotient-overflow cases in that body. Signed low-product arithmetic in the later helper depends on the retained explicit `-fwrapv` compiler contract. These are bounded reviewed translation conditions, not a general correctness claim for every instruction in the upstream lifter. The return paths were subsequently exercised by the time-conversion batch below.

Independent Sol High review passed all five admissions. The [nxdk declaration](https://github.com/XboxDev/nxdk/blob/master/lib/xboxkrnl/xboxkrnl.h) identifies index `0xFF` as `XC_MAX_OS`, distinct from `XC_MAX_ALL` (`0xFFFF`). The earlier DWORD provider covers explicitly configured indices 17 and 10; the separate opt-in OS model is described below. All fixed capture, trace and Wasm memory limits remain unchanged.

## Explicit modeled OS-settings value

The pinned Cxbx replacement maps `XC_MAX_OS` to a binary user-settings structure. Its fixed-width declaration describes 96 bytes; its query clears the requested buffer capacity before copying the structure. These are [replacement mapping](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/common/EmuEEPROM.h), [layout](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/common/types.h) and [query behavior](https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/585c49a50af1255ab155099e06f24505f9c5a800/src/core/kernel/exports/EmuKrnlEx.cpp), not an original-console observation. Independent original-consumer review found matching bias, name, date-rule and miscellaneous-flag offsets. The observed 256 bytes are capacity, not result length.

The new provider is default-off and admits only the reached selector 255, capacity 256 and null result-length pointer. It preflights aligned writable ranges for the complete buffer and type word, rejects overlap atomically and permits the reached adjacent type word at buffer end. It then clears the buffer, copies the configured 96 bytes, writes binary type 3 and returns status zero with a 24-byte stack advance. Other request shapes remain guarded; the existing DWORD behavior is unchanged.

The selected profile explicitly declares synthetic UTC, zero biases/date rules/names and other fields, a checksum word, and the existing miscellaneous and parental-control DWORDs. Both the serializer and module configuration check those shared DWORDs for consistency. The local fixture's checksum matches the pinned replacement's sum; the provider does not emulate physical EEPROM storage, checksum validation, factory or encrypted identity data, host settings or persistence.

Two pinned-compiler builds reproduced module SHA-256 prefix `7917aab3`. With no opt-in, fresh Chrome runs preserved the earlier control, inert and settings-refusal stops and stable backend observations. The experiment changes only the selected OS profile, retains all earlier trace/entry prefixes and all preceding captured calls, and completes the same ordinal-24 arguments. Chrome reports type 3, 96 returned bytes, EAX zero and ESP advancing from `0x03C10A44` to `0x03C10A5C`. A provider snapshot compares the first 96 bytes immediately after copying; zero padding is source/native-fixture verified, and the snapshot does not establish later consumer memory. A subsequent status-to-DOS-error call succeeds before the new untranslated stop.

That report retains 62 suffix rows, 574 total kernel events, the same first-window prefix digest and 6,655 trace events. The 446 seeds, three direct views, 20,983 instructions, 31 exports and fixed resource limits are unchanged. Repeated triples and a direct candidate-module control comparison passed. Independent Sol High correctness review passed, as did 62/62 native checks including sanitizers and 214/214 runtime checks. CI remains pending. Subsequent admissions and the comparison clock are recorded below.

## Original time conversion and comparison clock

| Admission | Prepared seeds + views / instructions | Module SHA-256 prefix | Browser experiment stop | Routines / completed kernel operations |
| --- | --- | --- | --- | --- |
| `0x001E8839` | 447 + 3 / 20,998 | `06df19a8` | Status 2 at `0x001E885A`, return `0x001E88F7` | 408 / 573 |
| `0x001E885A` | 448 + 3 / 21,017 | `fa947061` | Status 2 at `0x001DF950`, return `0x001E8DD1` | 409 / 574 |
| `0x001DF950` | 449 + 3 / 21,036 | `4dfc3aad` | Status 2 at `0x001E8E74`, return `0x0004C526` | 411 / 605 |

The three exact retained bodies passed original-byte, complete-layout and decoder checks without changing prior bodies, views, bindings or limits. Independent review checked the name/date helpers, the unsigned low-64-bit multiply, its saved register and both 16-byte callee-cleanup returns. Chrome continued through the original time-conversion path and its existing time-fields service. A further 38-byte FATX append advanced EOF from 172 to 210 and revision from 81 to 83, with the same cluster, free-cluster count and eight backing pages; the handle then closed. Each admission preserved the preceding control/inert sequences and experiment prefixes.

Two live-clock runs reached the same stop and retained identical kernel call frames and canonical capture digests, but their time fields differed. A numeric-formatting path added one division-helper call for a different seconds value, then the traces rejoined. Its aggregate ABI diagnostic also differed by one event. Those live reports are retained as a measured variation; they are not an exact-repeat pass.

The default-off `fixed-system-clock` profile now permits an explicit synthetic system-time fixture. It fixes only the wall-clock supplier; interrupt timing still uses `performance.now`. Settings and report metadata must identify the same fixed profile, and captured successful system-time queries must match its value. Without the profile, the existing live-clock behavior and metadata remain unchanged. This fixture supports comparison and does not establish original-console clock fidelity or gameplay timing.

With the first live run's system timestamp selected explicitly, the candidate reproduced all 7,101 trace events, 3,572 original-entry events, 94 suffix call frames, capture digests and ABI diagnostics of that reference. A separate fixed-profile baseline/candidate pair preserved its full prefixes, and a repeat candidate matched the complete stop, stable sequences and backend observations. The final report contains 606 total kernel events and 605 completions; storage remains 512 call rows, 8,192 trace rows and 128 MiB fixed Wasm memory, with the 1,024-event total capture ceiling unchanged.

Two pinned-compiler builds reproduced the final module. Independent Sol High review passed the admissions and the fixed-input comparisons. Runtime checks passed 219/219; the preceding native 62/62 result still covers the unchanged C components. CI remains pending. The next task is bounded admission of `0x001E8E74`, retaining the declared UTC/fixed-system-time comparison inputs and separate live-clock evidence.

M1 and the other project milestones remain unaccepted. The experiment is bounded startup with explicit synthetic service inputs. Original presentation, a controllable original screen and gameplay acceptance are still pending.
