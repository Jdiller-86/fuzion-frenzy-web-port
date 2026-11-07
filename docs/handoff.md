# Corrected startup stops at untranslated helper

Current Chrome 156.0.8078.80 boundary: status 2 at `0x0004BDC0`, return `0x0004C414`, first argument `1`, with 437 original routines and 739 completed kernel operations. `0x0004C380` remains pending and READY callback `0x0004CBE0` is unexecuted. M1 is not accepted. See [current status](status.md) for module identity, browser evidence and limits.

## Next bounded task

Inspect untranslated `0x0004BDC0` at its observed call/return. Verify original bytes, complete layout, decoder agreement, preserved bodies and resource limits, then replay only to the next actual guard. The previously admitted `0x001E87E0` helper has the established 67-byte/28-instruction `RET 16` body; it is no longer the runtime stop. Do not infer unsupported service behavior or expand synthetic debug fixtures.

## Evidence and checks

The ABI 25 module is 1,175,006 bytes, SHA-256 `fa62500e4cb32ff45725aab8a9e7d5359be790abd0f35fc3682dcf95df3c6f73`; profile counts are 472 seeds, three checked views, 22,122 instructions, 31 exports and 57 bindings. The current status documents the reproducible trace and kernel-capture digests, module limits, default/selected debug-window behavior and synthetic evidence scope.

The 49 MiB allocation succeeds in the existing checked model, with matching region and helper/caller result `0x006F4000`; both frames have balanced SP and stable FS/exception head. The captured service exposes only its first argument and status. Default debug refusal remains after eight completions; explicit start 8 permits at most 16, and the selected run completes nine calls. No original message contents are copied or printed. The first 9,109 raw CPU rows preserve the intermediate helper/default run; deterministic suffix/repeat reports match. Live timing remains separate evidence.

Current checks: runtime 309, native 78 including sanitizers, and preparation/docs checks passed. Python 17 and 31 seed-group checks were not rerun for this change. The new checkpoint CI is pending publication. These checks do not establish original-game playability; no original screen, rendering, audio, recorded input or three-round M1 acceptance exists.
