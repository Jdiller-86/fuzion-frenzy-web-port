# PFIFO channel setup evidence: the title writes GET=0 itself; the pushbuffer claim is still empty

November 4, 2026. A bounded read-only Ghidra pass on the retained private project (`FuzionFrenzySetup`, XBE SHA-256 `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`) traced the reached PFIFO submission path. The pass confirms a correction: the title's own context-setup code writes PFIFO PUT and GET to zero before `CDevice_KickOff` writes PUT. The previous "no initial GET" statement conflated "no external/kernel write" with "no write at all". The pushbuffer allocation is still empty and no consumer is implemented. **There is no playable screen, renderer or device model.**

## Method

- Existing persistent project reused read-only with `-noanalysis -readOnly`; no new import, extraction or persistent analysis change.
- `InspectResourceCalls.java` and `FindRenderStateRefs.java` produced narrow private decompilations under ignored `research/verification/pfifo-setup-01/`.
- No pseudocode is copied here; only addresses, fields and observed runtime values are curated.

## Static findings

| Routine | Finding |
| --- | --- |
| `D3D8::CDevice_KickOff` at `0x1FAED0` | Writes the computed pushbuffer value to the USER DMA_PUT alias (`context+0x8c1` field plus `0x40`, physical `0xFD800040`); the runtime write site is `0x1FAF1C` with value `0x03B7D000`. The alternate branch reads USER DMA_GET (`+0x44`) but the routine never writes GET. |
| `FUN_001FBB00` (device initialization) | Allocates contiguous memory, creates the context DMA objects, stores the USER PUT/GET pointers, calls `CDevice_KickOff`, then waits until the observed GET matches `0x03B7D000` using `FUN_001FACA0` (a bounded delay loop). |
| `FUN_0020005A` (context setup) | Writes PFIFO PUT (`0xFD003240`) and GET (`0xFD003244`) to zero, plus PFIFO control registers (`0xFD003210`, `0xFD003270`, `0xFD003058`, `0xFD003258`, `0xFD002504`, `0xFD002508`, `0xFD00250C`, `0xFD003228`, `0xFD002410`, `0xFD002420`). |
| `FUN_00200352` | Writes PUT/GET to zero and enables the channel; `FUN_00200143` saves and restores the per-channel context entries. |
| `FUN_001FDFC3` | Zeroes software context fields and returns the USER base (`0xFD800000`) from the context; it does not write the hardware GET. |

## Runtime-log corroboration

The retained report `post-memory-browser-20261103-46.json` records:

- Zero PFIFO PUT/GET writes at sites `0x20005A`, `0x20027C` and `0x20047F`, at event indices 5193/5194, 5431/5433 and 9119/9120.
- The single USER PUT write at site `0x1FAF1C`, value `0x03B7D000`, at event index 12523.

So the observed log itself places the GET/PUT zeroing before the kickoff PUT. `submissionSummary` now reports `putZeroBeforePut` and `getZeroBeforePut` from event order, and a source-owned test covers a setup-first sequence.

## Corrected interpretation

- The initial GET is not missing: the title's context setup writes it to zero. The report's GET=0 is consistent with the original code, not an unmodeled external write.
- PUT `0x03B7D000` is the end of the 20 KB GPU-instance claim `0x03B78000`..`0x03B7D000`; the claim is entirely zero and the 4 KB before PUT is entirely zero. GET=0 therefore does not point at an established command stream.
- The user-mode `CDevice_KickOff` has written PUT without a populated pushbuffer. The command source (kernel D3D submission, a jump at physical 0, or another allocation) is not established. The DMA object target is NVM/VRAM, base 0, limit `0x3FFAFFF`; the model uses UMA identity, so offset 0 reads system RAM 0.
- The runtime log also lists page 0 as written, but that write is not established as a command header or jump; the retained proof does not expose its value or source.

## Consequence

- Do not implement a consumer, set GET=PUT, walk from 0 or treat the zero claim as a submitted queue. The fail-closed status 13 boundary at `0xFD800044` stays.
- Do not claim that the title never initialized the channel; it does zero PUT/GET during context setup. The remaining blocker is the **command source and buffer ownership**, not the initial GET value.

## Validation

- Private Ghidra read-only script runs completed; outputs remain ignored and local.
- `tests/submission-probe.test.mjs` passes 18/18; `submissionSummary` now exposes `putZeroBeforePut` and `getZeroBeforePut`.
- Runtime suite, Python memory-access 10/10, title-seed 25/25, `npm run check` and whitespace checks pass.
- Module `recomp-post-memory-build-51` (526,955 bytes, SHA-256 `cd0bf176…`), ABI 24 and D060 are unchanged.

## Next task

Determine the command source before any consumer work: locate the user-mode command builder and its target allocation, or establish whether the kernel D3D submission path populates the claim. If the evidence identifies a bounded baseline and a populated range, return to the consumer with that evidence. Otherwise choose another independent runtime prerequisite; do not synthesize queue initialization or advance GET.
