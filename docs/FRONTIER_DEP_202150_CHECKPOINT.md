# The `0x202150` batch admits `D3DDevice_Present` and stops at the PFIFO pushbuffer boundary

November 5, 2026. Published head `6cbe40d` was the starting point for the block that ended at `2618783`; this cycle, the render-state table being exhausted, escalated the post-table region decision once to the project architect and received a bounded authorization: **admit the `D3DDevice_Present` region, exactly one set, then halt for review**. **There is no controllable original screen.**

## Authorization and correction

- The architect subagent (opencode-go GLM-5.3) approved one bounded set at approximately 03:45 local time on 2026-11-05, with the acceptance and halt conditions quoted below. This is a bounded architect-subagent decision, not an update to the hourly runner's guidance file, which remains at `d4e0107`/02:41.
- **Correction:** the escalation stated that no function existed at `0x202150`. That was wrong: `D3DDevice_Present` is **already an established Ghidra function** (`body 0x202150..0x2022D0`, 115 instructions / 385 bytes). No create-function was made or needed; only a seed export was taken. Nothing else was created.

## Bounded set

| entry | symbol | instructions | bytes | translated before |
| --- | --- | --- | --- | --- |
| `0x202150` | `D3DDevice_Present` | 115 | 385 | no |
| `0x201E80` | `FUN_00201e80` | 125 | 476 | no |
| `0x202060` | `FUN_00202060` | 64 | 228 | no |
| `0x1F5520` | `D3DDevice_BlockUntilVerticalBlank` | 10 | 36 | no |

- All four are **immediate direct callees of `D3DDevice_Present`** (or Present itself) and already established functions.
- Total: **4 functions**, within the ≤8 ceiling; no transitive expansion, no other region touched.
- To stay at **32 exports** without rewriting a used seed, the four were exported together with the two functions of an existing 2-function export (`0x1FE0C6`, `0x1FCA20`), and that export was replaced in the list. No function was dropped.
- Seed: `recomp-present-01/seed-present-01.json`, 6 functions, SHA-256 `6c5a69b49fef13eb5cebda81b34a3ec713e9368bbb609f3c437279d1c8d59cb5`.
- Preflight: 115/115, 125/125, 64/64, 10/10, 105/105 and 24/24 decoded; `mixed False`; **0 tables**; no normalizations; byte identity, decoder agreement and layout pass for all six.

## Translation and module

- Translation `recomp-title-recomp-title-post-memory-83`: **461 functions / 18,940 instructions**; all four new entries have real guarded bodies; 93 placeholders remain.
- `verify-seeds` **32/32/0/0**; `verify-translation` **554 functions, 0 rewrites**.
- Module `recomp-post-memory-build-104`: **714,276 bytes**, SHA-256 `ff29e85547e6567fa417538d713429632c9e46bc8ef374a3366d80586697e543`.

## Controlled pair

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE00009C`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 13 at `0xFD800044`**, ordinal 1 | 339 | 405 |

The comparator exits 0: same module, same full supplied profile, `avQueryWord` the only added input, **+60 routines / +1 operation**, no regression. The control's unsupported ordinal-2 thunk address moved from `0xFE000094` to `0xFE00009C` because the module layout grew; the service and ordinal are unchanged.

## Observed stop

- Target `0xFD800044` is an **NV2A PFIFO MMIO register**, not code (no symbol; outside the executable's translated text).
- `returnAddress` `0x1FB06E`, call site `0x1FB069`, inside `D3D_BlockOnTime` (`body 0x1FB020..0x1FB106`).
- The retained harness stops with status 13 when a VM read of the PFIFO DMA_GET register observes `GET != PUT` (`thread_startup_probe.c`), and the run records `stopReason: "pushbuffer consumption required"`.
- `D3DDevice_Present` executed; of the new callees, `0x202060` executed while `0x201E80` and `0x1F5520` did not (branch-dependent). `D3D_SetFence` executed.

**Interpretation:** the translation has progressed from an untranslated direct call to a **runtime capability boundary**: the title has submitted pushbuffer commands and is waiting for the GPU side to consume them, and the synthetic harness deliberately has no consumer. The next stop is no longer a missing translated routine.

## Architect acceptance vs observed

1. The status-2 stop at `0x00202150` is **gone**; the experiment no longer stops there. Met.
2. The next stop is reported with exact address, status, caller/return and decode classification (`0xFD800044`, status 13, `0x1FB069`→`0x1FB06E`, MMIO not code). Met.
3. The delta (+60 routines / +1 operation) is attributable to the admitted callees and the newly reachable present path; the module size/SHA advanced coherently. Met.
4. All gates green: seeds 32/32/0/0, package verifier 46/46, export cap 32, `git diff --check`, one commit. Met.

## Frontier and next decision

Frontier: **554 functions, 93 placeholders, 92 referenced**, 361 direct call targets. The three additional placeholders are the kernel/indirect call sites inside the present path.

Per the authorization, this cycle **halts after one set**. The next work is not another translation admission but the **PFIFO pushbuffer consumption** boundary: either implement a bounded source-owned consumer path or decide a different route. That is a fresh architect decision. Geometry remains HOLD; no verification-lane files were touched.

## Package verification

`tools/verify-recomp-package.mjs` with `docs/FRONTIER_DEP_202150_PACKAGE_MANIFEST.json`: 32/32 seeds, **461 admitted / 554 generated / 0 rewrites**, **46/46 digests**, pair ok (+60 routines / +1 operation), next stop `0xFD800044`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| Present-batch 6-function seed | `6c5a69b49fef13eb5cebda81b34a3ec713e9368bbb609f3c437279d1c8d59cb5` |
| `generation.json` | `a7dc3cf2a5c651bc608033caf4ba4898b556e67e5722616ac81bcf96ff69bf24` |
| `title-functions.inc` | `b26f1bbb60724b2a2d63cd03ecad4bd15c4b7099723c7927f3ae53c27fd37b72` |
| `title-profile.h` | `cd5ea3afdf6f03db4b3f186b296f36e660e94e5ab8a6a1289d6f0bb3c44f681e` |
| Module 104 | `ff29e85547e6567fa417538d713429632c9e46bc8ef374a3366d80586697e543` |
| Control report | `36ad2f487cd5712cf3e3a9576a2c863ff51866cb7d7b5e3490413d8021095814` |
| Experiment report | `fdf1e41c4356ccad4f9b43a6dc7d4476cf6b38f4e5ad83c1e0a853c84d19b5e0` |

## Record correction

The `0x1F65F0` checkpoint said the new function "was appended to the merged seed (33 functions)". Translation 82 actually used the **32-function** merged seed plus the new 1-function export; the 33-function file was created but never used. This checkpoint corrects that, and the package manifest labels the unused file accordingly.

## Limits

These are synthetic startup observations. Reaching the PFIFO boundary proves translated code executed up to a GPU submission wait; it does **not** establish pushbuffer command correctness, rendering, presentation, gameplay or playability.
