> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Recomposition is byte-identical, and the guard emitter is deterministic and idempotent

November 4, 2026. The translation pipeline was re-run end to end to test reproducibility, and the guard emitter gained determinism/idempotence coverage. Both pass. **There is no engine, renderer, timing source or playable screen.**

## Recomposition reproducibility

All 32 seeds were recomposed with the same profile, limits (`--function-limit 512 --instruction-limit 32768`) and `--guard-memory` into a second, independent attempt (`recomp-title-recomp-title-post-memory-46`) and compared with the current translation (`…-45`):

| output | sha256 (first 16) | size | identical |
| --- | --- | --- | --- |
| `title-functions.inc` | `b26d65be50cd0f2a` | 1,857,567 | yes |
| `title-profile.h` | `7e28e8666355104d` | 35,248 | yes |
| `generation.json` | `0eea000cf906aaaf` | 140,197 | yes |

Both runs produced **415 functions / 17,284 instructions** and an identical generation record. The translation pipeline is therefore deterministic for a fixed seed set, profile and emitter — which is a prerequisite for trusting any later comparison between builds.

## Emitter determinism and idempotence

`tests/recomp_memory_access_test.py` now asserts that:

- instrumenting the same source twice produces byte-identical output and identical access counts (deterministic);
- feeding **guarded** output back through the emitter changes nothing and reports zero sites/reads/writes (idempotent, so a future pass cannot double-wrap or silently re-count accesses).

Suite total is **13 tests**, all passing.

## What this does not establish

Determinism says nothing about correctness or fidelity: the translation is still a bounded, seed-driven subset, the reachable original code is still blocked on the PFIFO engine predicates (unassigned owner), and there is no rendering, timing fidelity or playability. The recomposition lives in ignored private research and is not committed.

## Validation

`npm run test:evidence` 3/3 (memory-access now 13 tests), `npm run test:native` 14/14, runtime suite 113/113, title-seed 25/25, `npm run check` and whitespace checks pass.

## Next task

Unchanged: the PFIFO engine/consumer contract needs an explicit owner and evidenced producer rules. A determinism re-run should be repeated whenever the seed set, profile or emitter changes, and any difference recorded before a new module is trusted.
