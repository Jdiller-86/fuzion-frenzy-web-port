# Project status: startup reaches `0x001E7267`

Observed November 6, 2026 in Chrome 156.0.8078.80 on Windows. The original-startup experiment created an empty debug log on the mounted synthetic FATX Partition 5 and continued through guarded CRT batches to **status 2 at `0x001E7267`**, with return address `0x001E4CE3`, 377 distinct routines and 447 completed kernel operations. [Handoff](handoff.md) · [Roadmap](../plans.md) · [Project milestones](milestones.md)

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

Read-only Ghidra exports added one established routine per batch. When the cumulative seed group reached 32 entries, later exports were appended to a group with room. Existing rows and the 32-export, 512-function and 32,768-instruction limits were preserved. The initial `0x001E4D58` export failed the complete-layout guard on three undefined bytes. The matching XBE's exception scope record points to that address. Bounded read-only disassembly established one handler instruction, preserved all 46 prior rows and passed the existing byte, decoder, layout and memory checks.

Independent review confirmed unchanged control and inert stops, entry sequences, trace kind/address/stack and kernel ordinal/status/argument sequences. The experiment preserved those prefixes across the batches. Clock-dependent outputs varied between runs.

The runtime checks passed 200/200, native checks passed 56/56 including sanitizer fixtures, and Python evidence suites passed 6/6. The replay harness and local checks used the workstation's Node 24.22.1 and npm 11.19.1. CI on the repository's pinned Node version remains pending for these local commits. The browser comparisons and independent reviews are retained locally.

## Next bounded dependency

Build and replay the locally prepared `0x001E7267` candidate with the fixed control and inert cases, then obtain independent review. The current Chrome-verified module remains the `0x001E4CD7` admission.

M1 and the other project milestones remain unaccepted. The experiment is bounded startup with explicit synthetic service inputs. Original presentation, a controllable original screen and gameplay acceptance are still pending.
