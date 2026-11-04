# The 0x1F6EB0 admission advances the stop to 0x1FC780

November 4, 2026. The project architect's current directive (opencode-go/glm-5.3-flash, from integration head `c9dc22f`) authorized one bounded `0x1F6EB0` admission set. This cycle follows it and stops here. **There is no controllable original screen.**

## Selection and translation

- Read-only export `FUN_001f6eb0` from the retained project: **455-byte** body/envelope, **110 instructions**, no calls. This matches the earlier direct-branch characterization.
- The function was merged into the existing front seed (`seed-with-1f6eb0.json`, 27 functions) so the selection stays at the existing **32 seed exports**.
- Translation `recomp-title-recomp-title-post-memory-51`: **420 functions / 17,558 instructions** from the same 32 seeds with `--guard-memory`. `0x1F6EB0` is admitted (455 bytes, 0 data/padding), the caller `0x1F83C0` emits the direct call, and the address is no longer an untranslated direct target.
- Source-owned guards: `tools/verify-seeds.py` reports **32 recorded / 32 offered / 0 unexpected / 0 missing**; `tools/verify-translation.py` reports **517 functions, 0 needing rewriting, fully instrumented**.

## Module and controlled pair

Module `recomp-post-memory-build-72`: **660,060 bytes**, SHA-256 `9d8689ff916dbed7d264e378ca23e5e8b1d26853a5a2b11fa30ca0780e42ccaf`. It uses the same build flags as build-71; only the translation include and output path changed.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1FC780`** | 283 | 405 |

- Both runs use the same module and the same full supplied input profile; `avQueryWord = 0x00800303` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+4 routines, +1 kernel operation**, next stop `0x1FC780`.
- The experiment's `originalFunctionsReached` contains `0x1F6EB0`. Its ordinal-2 call is status 0 with `output.rawWord = 0x00800303`, EAX unchanged and ESP advanced by 20 bytes.
- Frontier on translation 51: **97 placeholders, 1 entered** — `0x1FC780` called by executed `0x1F5640`; 73 called by an executed caller, 22 static-only.

## Exact-head review evidence

- Integration base: `becc879`, preparation CI runs `41916226715` and `41916376757` success; main PR #5 is `CLEAN`/`MERGEABLE`.
- Module `recomp-post-memory-build-72` SHA-256 `9d8689ff916dbed7d264e378ca23e5e8b1d26853a5a2b11fa30ca0780e42ccaf`.
- Control profile `settings-pfifo-reset3.json` SHA-256 `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419`.
- Experiment profile `settings-pfifo-reset3-avquery.json` SHA-256 `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af`; it differs from the control only by `avQueryWord = 0x00800303`.
- Control report SHA-256 `82dd1464f1d199f4af9e4abb88cb0e35a2fc537ec44060be37f22ca7f868b6f8`; experiment report SHA-256 `f92976d43b852ffa860dd647fe2a936844a7f18e338245bed232f4cb2093cd2d`.
- `tools/compare-proof-profiles.mjs` exits 0 on the pair; the frontier enters `0x1FC780` called by executed `0x1F5640`.
- Main is now `2aba638` (PR #5 merged; tree equals `becc879`); the synced source head contains `origin/main`.
- The source lane is synced to `becc879`; PR #2 head `e1804fb` is `CLEAN`/`MERGEABLE` against `becc879`; preparation CI runs `41917187801` and `41917176661` success. No further admission until reviewed.

## Next and limits

The next genuine stop is untranslated `0x1FC780` (status 2, return `0x1F56FD`). It is **not admitted or analyzed further**. Per the directive, the lane stops here.

These are synthetic startup observations with a synthetic avQuery word. They are not console settings, fidelity, renderer, gameplay or original service semantics. Original content remains player supplied and local. The next-stop report is also recorded in [the utility-mount checkpoint](FATX_UTILITY_MOUNT_CHECKPOINT.md).
