> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Read-only characterization of `D3DDevice_Present` and its call surface

November 5, 2026. Read-only static characterization of `0x00202150` (`D3DDevice_Present`) and its caller, as directed. **No admission, export or seed change was made this cycle.** All evidence is from the retained local Ghidra project, the existing seed `recomp-present-01/seed-present-01.json`, translation 83 and the retained experiment report.

## Conflict with the directive's premise

The directive describes `0x00202150` as the post-table stop and asks for characterization **with no admission**. That premise is stale: under the previous architect-subagent authorization, commit `7dae2e4` already admitted `D3DDevice_Present` plus its three untranslated immediate direct callees (`0x201E80`, `0x202060`, `0x1F5520`) in translation 83 / module 104. The run now stops later, at the PFIFO boundary (`0xFD800044`, status 13). This checkpoint therefore characterizes the **already-admitted** function and records the conflict; it does not admit anything further.

The directive also assumes the architect has integrated `2618783`. At this cycle the architect branch is still `d4e0107`, so that integration has not happened. Main is `4b94a64` (PR #8 merged). This session's shell policy still denies `git merge`, so the lane branch remains one main merge behind; no history was rewritten.

## Envelope and shape

| Property | Value |
| --- | --- |
| Entry / body | `0x00202150` .. `0x002022D0` |
| Instructions / bytes | 115 / 385 |
| Decoded | 115/115, no normalizations |
| Returns | one `RET 0x10` at `0x002022CE` |
| Tail calls / tables | none / none |
| Basic blocks | 20 |
| Boundaries | clean; no undecodable byte, overlap or ambiguous end |

Instruction mix: `MOV` 32, `PUSH` 30, `CALL` 16, `TEST` 8, `JZ` 6, `POP` 4, `SUB` 3, `CMP` 2, `INC` 2, `LEA` 2, `XOR` 2, `JNZ` 2, `JC` 1, `JNC` 1, `JMP` 1, `SHR` 1, `ADD` 1, `RET` 1.

Control flow: 9 conditional branches, 1 unconditional `JMP`, 1 `RET`. Two loops:
- fence-wait loop, `JNC 0x0020218A` → `0x00202174`, comparing two context counters;
- submit-retry loop, `JNZ 0x00202235` → `0x00202212`, retrying while the submit call returns non-zero.

## Direct call surface

Seven direct callees, all established functions:

| Callee | Symbol | Bytes | Status at translation 82 | Classification |
| --- | --- | --- | --- | --- |
| `0x1FAF80` | `D3D_SetFence` | 153 | translated | pre-existing |
| `0x1FACA0` | `FUN_001faca0` | 29 | translated | pre-existing |
| `0x1FAED0` | `CDevice_KickOff` | 167 | translated | pre-existing |
| `0x1FB300` | `D3D_KickOffAndWaitForIdle` | 17 | translated | pre-existing |
| `0x201E80` | `FUN_00201e80` | 476 | **not translated** | added in 83 |
| `0x202060` | `FUN_00202060` | 228 | **not translated** | added in 83 |
| `0x1F5520` | `D3DDevice_BlockUntilVerticalBlank` | 36 | **not translated** | added in 83 |

`0x201E80` itself has seven direct calls of its own (`0x201930`, `0x201860`, `0x1FB3B0`, `0x201BE0` ×3, `0x201AC0`); those were **not** admitted, and `0x201E80` did not execute on the observed path.

## Indirect call surface

Nine `CALL` instructions resolve through seven distinct pointer sites; three calls share `EBX`, loaded from `0x00253450`. Every stored target is a kernel-ordinal thunk (`0x80000000 | ordinal`), not translated code:

| Pointer | Stored target | Classification |
| --- | --- | --- |
| `0x0025344C` | `0x80000001` | kernel ordinal 1 |
| `0x00253450` (→ `EBX`) | `0x80000002` | kernel ordinal 2 |
| `0x00253458` | `0x80000003` | kernel ordinal 3 |
| `0x0025345C` | `0x80000004` | kernel ordinal 4 |
| `0x0025336C` | `0x800000AB` | kernel ordinal 0xAB |
| `0x00253360` | `0x800000B2` | kernel ordinal 0xB2 |
| `0x00253454` | `0x800000B4` | kernel ordinal 0xB4 |

`0x00253448` → `0x8000009F` is used by the newly admitted `0x1F5520` (`BlockUntilVerticalBlank`), also an ordinal thunk. These sites are runtime services, not missing translations.

## Caller context

`0x42C5E` is the Present call inside `FUN_00042be0` (`body 0x42BE0..0x42C6B`, 40 instructions):

- builds a descriptor on the stack with `0x280` (640) × `0x1E0` (480) and format `7`;
- calls `0x1FC1B0` at `0x42C51`, then pushes four zero arguments;
- calls `0x202150` at `0x42C5E` (return `0x42C63`), then `POP EDI`, `ADD ESP,0x38`, `JMP 0x1F54D0`;
- four static direct callers reference Present: `0x41F3F` (`FUN_00041ee0`), `0x42481` and `0x4249D` (`FUN_00042020`), and `0x42C5E`.

The caller is already translated (seed `recomp-after-memory-next-01`), so the caller context is not a new dependency.

## Memory and context surface

Direct globals touched: `0x00206720` (device context), `0x00203B68` (render-state byte), `0x0020696C`, and the ordinal table `0x00253360..0x0025345C`. Context-relative fields: `+0x1C`, `+0x2168`, `+0x217C`, `+0x2308`, `+0x2310`, `+0x2314`, `+0x2318`, `+0x2510`, `+0x2514`, `+0x2518`, `+0x2B28`, `+0x2B60`.

## Observed runtime boundary (retained)

Module 104 experiment: `D3DDevice_Present` executed, and the run stopped **after** it at status 13, `0xFD800044` (an NV2A PFIFO MMIO register), return `0x1FB06E`, call site `0x1FB069` inside `D3D_BlockOnTime`, `stopReason: "pushbuffer consumption required"`. `0x202060` executed; `0x201E80` and `0x1F5520` did not. This confirms the call-surface reading: the remaining blocker is the runtime's missing pushbuffer consumer, not untranslated code.

## Assessment for the architect

- No undecodable or ambiguous boundary was found; the directed halt condition for ambiguity did not trigger.
- The function is a bounded 385-byte submit/present routine with 7 direct callees (4 pre-existing, 3 admitted in 83) and 7 kernel-ordinal indirect sites.
- Further translation in this surface would not move the run: the next stop is a runtime PFIFO consumption boundary.
- These are static facts plus the retained synthetic run; no rendering, command-correctness or playability is established.
