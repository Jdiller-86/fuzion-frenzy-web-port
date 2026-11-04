# The 0x1FC780 admission advances the stop to 0x1FCCB0

November 4, 2026. The architect directive from integration head `ae4b7ae` authorized one bounded `0x1FC780` admission set. This cycle follows it and stops here. **There is no controllable original screen.**

## Selection and translation

- Read-only export `FUN_001fc780`: **53-byte** body/envelope, **17 instructions**, one self-recursive call at `0x1FC7A6`, no external calls (matches the earlier direct-branch characterization).
- The function was merged into the existing front seed (`seed-with-1fc780.json`, 28 functions) so the selection stays at the existing **32 seed exports**.
- Translation `recomp-title-recomp-title-post-memory-52`: **421 functions / 17,575 instructions** from the same 32 seeds with `--guard-memory`. `0x1FC780` is admitted (53 bytes, 0 data/padding), and it is no longer an untranslated direct target.
- Source-owned guards: `tools/verify-seeds.py` reports **32 recorded / 32 offered / 0 unexpected / 0 missing**; `tools/verify-translation.py` reports **517 functions, 0 needing rewriting, fully instrumented**.

## Module and controlled pair

Module `recomp-post-memory-build-73`: **662,080 bytes**, SHA-256 `f89016609cde54d9fd0a612d9da5cb6f6ae24d0999391760ac27ef2c9ccc7326`. It uses the same build flags as build-72; only the translation include and output path changed.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1FCCB0`** | 284 | 405 |

- Both runs use the same module and the same full supplied input profile; `avQueryWord = 0x00800303` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+5 routines, +1 kernel operation**, next stop `0x1FCCB0`.
- The experiment's `originalFunctionsReached` contains both `0x1F6EB0` and `0x1FC780`. Its ordinal-2 call is status 0 with `output.rawWord = 0x00800303`, EAX unchanged and ESP advanced by 20 bytes.
- Frontier on translation 52: **96 placeholders, 1 entered** — `0x1FCCB0` called by executed `0x1F5640`; 72 called by an executed caller, 22 static-only.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Export seed | `358c0c3c54957c0de3373385d16e395f9c5f6512f726e8d8853d0af8b532e371` |
| Merged front seed | `afd24d12532d483a84b7496d1fa391d551774983d2cb50d887c52fb288e79e61` |
| `generation.json` | `22c2dd441bd671568d7fba6a0be151966c804dd27ba051f221859923b48520b7` |
| `title-functions.inc` | `b97b108f9b0bd7dd34050d00ef166da64315f3e11e50ce4666ac1c30152b3d3a` |
| `title-profile.h` | `3c972d110e7b3060a34ed261394e6107524946702c10829ddb395b2081bf9835` |
| Module | `f89016609cde54d9fd0a612d9da5cb6f6ae24d0999391760ac27ef2c9ccc7326` |
| Control profile | `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419` |
| Experiment profile | `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af` |
| Control report | `a0cdc76c6d14a73619cb1ada53155c67e30dd1d07da87d261f741be53a17358b` |
| Experiment report | `ed30f52e787776a5414936c0947eed0ef164545975888c37f4fe42df48535118` |

## Next and limits

The next genuine stop is untranslated **`0x1FCCB0`** (status 2, return `0x1F576A`) called by executed `0x1F5640`. It is **not admitted or analyzed further**. Per the directive, the lane stops here.

These are synthetic startup observations with a synthetic avQuery word. They are not console settings, fidelity, renderer, gameplay or original service semantics. No host/Wasm memory substitution was used.
