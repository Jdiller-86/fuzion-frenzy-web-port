# The `0x1F7080` admission observes the taken branch at `0x1F70E0`

November 4, 2026. The architect directive from integration head `feef47f` authorized exactly one bounded `0x1F7080` leaf admission using the retained seeds/project and the review-ready manifest. This cycle follows it and stops at the observed next genuine stop. **There is no controllable original screen.**

## Selection and translation

- `0x1F7080` seed: **27 instructions / an 89-byte instruction-only envelope**, no calls or tables; SHA-256 `4968b4371b592b7269c20cf5e9804b474d3595a34b23fb231e189b121fb1d431`.
- Merged into the 29-function front seed → **30 functions**; new front seed SHA-256 `baa09d2724d94bf1cb9f5b0d4aecba74b7569dc4cce0219bc2a9a328d0e21cbb`. The old 29-function seed is preserved.
- Translation `recomp-title-recomp-title-post-memory-54`: **423 functions / 17,620 instructions** with `--guard-memory`; `0x1F7080` has a real guarded body (not a placeholder); 94 placeholders remain.
- `verify-seeds` **32/32/0/0** (including the recovered `0x002022E0` seed); `verify-translation` **517 functions, 0 rewrites**.

## Module and controlled pair

Module `recomp-post-memory-build-75`: **664,115 bytes**, SHA-256 `687c34c98794d8ac71387ec1bf00aec9e88e4c23a38463746a725008b39b133f`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no `avQueryWord`) | status 1 at `0xFE000094`, ordinal 2 | 279 | 404 |
| explicit `0x00800303` | **status 2 at `0x1F70E0`** | 286 | 405 |

- Same module and full supplied profile; `avQueryWord` is the only added input.
- `tools/compare-proof-profiles.mjs` exits 0: **+7 routines, +1 kernel operation**; the experiment preserves every control-reached routine and its operation count matches its successful calls.
- `0x1F7080` executed through its real body and returned; the reached list includes it. The next call target `0x1F70E0` is untranslated and was **not entered**. The stop returns to `0x1F57CB` (call site `0x1F57C6`) in executed caller `0x1F5640`.
- Frontier on translation 54: **94 placeholders, 93 referenced**; `0x1F70E0` is called by executed `0x1F5640`.

## Digests

| Artifact | SHA-256 |
| --- | --- |
| New 30-function front seed | `baa09d2724d94bf1cb9f5b0d4aecba74b7569dc4cce0219bc2a9a328d0e21cbb` |
| `0x1F7080` seed | `4968b4371b592b7269c20cf5e9804b474d3595a34b23fb231e189b121fb1d431` |
| `generation.json` | `eaec84416534c30157bd049c8c4e01deb6bdc3511e401b59e628668ea8d824f8` |
| `title-functions.inc` | `954199e318223c6b58170f25435923c25d69dbb1d35636aa5ef1b88d6ac75f53` |
| `title-profile.h` | `e30624133092fa0a0f4910911113422ba0a13cc0df8d1e8170cb6c46e610e619` |
| Module 75 | `687c34c98794d8ac71387ec1bf00aec9e88e4c23a38463746a725008b39b133f` |
| Control profile | `d01ac221ccc107ebd21379953e8124e866a9baf46273dd73f4e9f85a9e4d4419` |
| Experiment profile | `351383c33e37e51baed18fead2f4621c037aa3ca74fef246bbfb8f97ad2c58af` |
| Control report | `15a2019d6a601e00fbc37f6409d5a29a5483096074d29eb00cdc629fec3c237b` |
| Experiment report | `303acd333c66c5312348b3b3f68ec7aab8caa4b13555375cdae5674bf1c7e498` |

## Next and limits

The observed taken branch is untranslated **`0x1F70E0`** (Path A: it needs `0x1FC980` + `0x1FCB70`). No Path A or Path B batch was started; this cycle stops at the observed stop. Per the directive, a later decision may authorize the taken branch or another bounded task.

These are synthetic startup observations with a synthetic avQuery word. They are not original service semantics, console settings, fidelity, renderer, gameplay or playability. The `0x1F70E0` body did not execute.
