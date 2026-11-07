# Startup completes a local lock cycle

Current Chrome 156.0.8078.80 boundary: status 2 at untranslated `0x0004E800`, return `0x0004F7DC`, SP `0x03C10E7C`, first frame word `0x002D14A8`, after 442 original routines / 741 completed kernel operations. `0x0003080D`, `0x0004BDC0` and `0x0004C380` now return with balanced frames; READY callback `0x0004CBE0` remains unexecuted. M1 is not accepted. [Current status](status.md) owns identities, comparisons and limits.

## Next bounded task

Inspect the actually reached `0x0004E800` at its observed caller/return. Check original bytes, full layout, decoder agreement, preserved bodies and resource limits, then replay to the next guard. Extend the three-function last export within its existing cap; all 32 export slots are used. An ambient first frame word does not establish arity. A separate source-owned automatic-code-generation probe cannot replace checked browser contracts or establish title playability.

## Evidence and checks

ABI 25 module: 1,186,511 bytes, SHA-256 `21d4e978c24b760dd4c2f06b3f146049796f6b7e95e140101b9920b484a718a9`; two builds match. Profile: 477 seeds, three checked views, 22,214 instructions, 32 exports, 59 bindings. Independently reviewed helper `0x0003080D` preserves all 479 preceding bodies. One exact original pair, slot `0x0025339C` → ordinal 221, is added; all 58 prior pairs and ceilings remain intact. Actual inert token `D4` → `D8` is a reviewed insertion effect with unchanged 347 / 419 boundary.

Explicit default-off acquisition and release models complete one local object-3 cycle for RUNNING thread 1: owner 0 → 1 → 0. Wait arguments `[0x0008000C, 1, 0, 0]` have NULL timeout (indefinite, immediately available), result zero / SP +20. Release arguments `[0x0008000C, 0]` use NULL output, result zero / SP +12. Blocking, recursion, abandonment, non-NULL timeout/output and callback scheduling remain unsupported. Joint validation binds captured creation order, chronological ownership, aliases, final tables, counts and ABI; omitted-prefix creation history remains unknown.

Without release selection, ordinal 221 still refuses at 442 / 740. Raw trace/state match the preceding module; only module identity and explicit two-argument service descriptor/capture metadata differ. Deterministic suffix/repeat evidence reconstructs 9,456 rows / 4,746 entries, preserving the 9,453-row default prefix and adding three balanced original returns. Kernel capture is 742 events / 741 completions. Live reaches the same guard/counts at 9,450 rows / 4,743 entries, with a 500.565 ms measured pause; its raw timing stays separate. Earlier allocation/debug findings remain qualified in status; no original message payload is copied or printed.

Runtime 324, native 82 including sanitizers, Python evidence 17/17, a 17-file source-only build, preparation/docs and diff checks pass. Independent Sol High C/ABI and joint-history review and the closed 12-report/four-negative-case release audit pass; checkpoint CI is pending. [Acquisition CI](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/42387148914) passes on both operating systems for `16c6ee5`. Local checks use pinned Node 24.14.0/npm 11.9.0 and Emscripten 3.1.50. There is no accepted original screen, rendering, audible audio, recorded input or three-round M1 evidence.
