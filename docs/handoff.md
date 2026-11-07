# Startup passes one local lock acquisition

Current Chrome 156.0.8078.80 boundary: status 2 at untranslated `0x0003080D`, return `0x0004BE3F`, SP `0x03C10E60`, first word `0x0008000C`, after 441 original routines / 740 completed kernel operations. `0x0003082D` and `0x00030943` returned; `0x0004BDC0` and `0x0004C380` remain pending. READY callback `0x0004CBE0` remains unexecuted. M1 is not accepted. See [current status](status.md) for identities, comparisons and limits.

## Next bounded task

Inspect the actually reached `0x0003080D`. Check original bytes, complete layout, decoder agreement, preserved bodies and fixed limits, then replay only to the next guard. All 32 export slots are used; extend the two-function last export within its existing cap. Do not infer release semantics before an actual service frame. A separate static comparison of automated code generation must preserve checked browser memory/ABI contracts and current admission limits.

## Evidence and checks

The ABI 25 module is 1,182,183 bytes, SHA-256 `a1d89cd3743e9c0405d31f807b2003e6b42ec224fa28ebe7baeca54e359b18ff`; two builds match. Profile: 476 seeds, three checked views, 22,202 instructions, 32 exports, 58 bindings. Independently reviewed helper additions preserve preceding bodies. Exactly one binding was added from the original import table, slot `0x002533A0` → ordinal 234; the preceding 57 pairs and all ceilings are unchanged. Inert token `D0` → `D4` is a reviewed insertion effect, with unchanged stop/count boundary.

The explicit default-off acquisition model admits only an unowned local mutant, mode 1, nonalertable, NULL timeout. Actual arguments `[0x0008000C, 1, 0, 0]` acquire object 3 for RUNNING thread 1; owner 0 → 1, result zero, SP +20. NULL timeout means indefinite wait, not polling; blocking, recursive acquisition, release and other shapes remain unsupported. Without selection, ordinal 234 still refuses at 441 / 739. Its raw trace/state match the preceding module; only identity and explicit four-argument descriptor/capture metadata change. Report validation binds all owned objects to unique captured acquisitions.

Deterministic suffix/repeat evidence reconstructs 9,452 rows / 4,745 entries, extending the 9,450-row default trace by two balanced original returns. Initial-prefix provenance is explicit in status. Control/inert remain 339 / 405 and 347 / 419. Kernel capture: 741 events / 740 completions, unchanged omitted-prefix digest. Live reaches the same guard/counts with 9,450 rows / 4,744 entries and a 508.3549 ms measured pause; its raw timing is separate evidence. Earlier checked 49 MiB allocation/frame results and nine-call debug-window observations remain qualified in status; no original message payload is copied or printed.

Current runtime 317, native 80 including sanitizers, Python evidence 17/17, the 17-file source-only build, preparation/docs and diff checks pass. Independent Sol High source/browser review and the closed 12-report/four-negative-case audit pass. Fresh runtime/evidence/browser checks use pinned Node 24.14.0/npm 11.9.0; retained Emscripten 3.1.50 is verified. Checkpoint CI is pending; [prior CI](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/42381116133) passes on Ubuntu and Windows. There is no accepted original screen, rendering, audible audio, recorded input or three-round M1 evidence.
