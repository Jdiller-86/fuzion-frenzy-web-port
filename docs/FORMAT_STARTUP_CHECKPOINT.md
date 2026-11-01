# Original hexadecimal-format return and next startup dependency

November 1, 2026. This advances the selected static recompilation of the original Xbox executable to WebAssembly. The complete original-game/browser/custom-feature goal remains active. No playable original screen, complete runtime, disc reads or fidelity acceptance follows from this startup case.

## Observed original execution

The original wrapper at `0x001dee6e` now returns after its actual formatting dependencies execute in a browser worker. A read-only, checked-memory observer captures its first call and return:

| Observation | Result |
| --- | --- |
| Actual format | `%08lx` |
| Actual first value | `0x4d530002`, matching the supplied XBE certificate's title ID |
| Output | `4d530002`, eight bytes followed by NUL |
| Return value | Eight bytes |
| Return state | Entry/exit stack, FS and exception-chain head match |
| Original dependencies returning | `0x001e2433`, `0x001e6e50`, `0x001e23fc`, `0x001e23d8`, `0x001e23a5` and wrapper `0x001dee6e` |

This establishes this hexadecimal startup case, not all format specifiers, floating-point formatting or complete library equivalence. The certificate was reread at `0x00010178`; its title ID is metadata evidence, separate from the observed call/output. No original source or recovered symbol name is claimed.

Both explicit synthetic settings/volume profiles pass **43 browser checks and 8,410 owned copy/ABI cases each**. Nine retained seeds translate **30 functions / 2,130 instructions**. There are **81 trace events / 27 distinct original routines entered** and sixteen completed kernel operations. Translation counts are not executed-instruction coverage. Heap/volume return, memory ownership/protection, namespace references and the pending six-hour timer remain verified within their earlier bounds.

Original `0x00033c12` has still **not returned**. Startup now stops at untranslated **`0x000338de`, return `0x00033c45`**. Its semantics and successful execution remain unknown. Device namespace resolution still mounts no medium and enables no disc reads.

## Bounded source changes

The ninth seed packages five actually reached dependencies without expanding the total function budget. The translator admits up to sixteen exports, still at most eight functions per export and **64 functions / 8,192 instructions total**. Metadata remains bounded to 8 MiB per seed and now has an explicit 64 MiB aggregate limit, checked before reading bodies and enforced again during reads. Original-byte identity, independent decoder agreement, complete envelope/layout accounting, non-overlap and checked guest-memory admission remain required. This is bounded export composition, not permission for full decompilation.

The pinned emitter produced redundant equality parentheses in this larger function. A Clang style-warning exception surrounds only the generated include; other warnings/errors, emitted instructions and runtime checks remain intact. The vendor checkout is unchanged.

The optional first-call observer reads admitted guest stack/format/output/FS memory and copies bounded observations into private harness storage. It changes no original instructions, arguments, guest buffers or return behavior. ABI 7 retains exactly one clock import and the explicit synthetic volume profile. A declared 128-event trace accommodates the observed 81 events; older modules without the optional capacity export retain 64. The verifier rejects other declared capacities. No new host capability is introduced.

## Evidence and verification

Artifacts remain ignored under `research/verification/`. Original XBE/instructions, generated title C/Wasm, raw analysis/traces, RAM and screenshots are never HTTP-served, uploaded, shared or committed. Only independently authored host source is served; original execution remains browser-worker-only. Developer compilation is not the required browser-local end-user conversion pipeline.

| Local artifact | SHA-256 |
| --- | --- |
| `recomp-library-next-04/seed.json` (five-function ninth seed) | `0f08a7efd04555add766292acba0c8d2f0c42b40d9cf168a14970166a20dc022` |
| `recomp-title-library-05/generation.json` | `1120460769f268da2aad94ad7fcff79160f4a6edee78825e1d5dbe2233b92470` |
| `recomp-library-build-07/build.json` | `093bb556c85b7c07bb5ffd3dcb0da0a910cb9f0298832ebdfcab92554c81ad29` |
| Matching `startup.wasm` (211,477 bytes) | `ee55cf37dc2a77f84aa7c3f4892ee85317ce758b5db41dddd7089373c4178cc1` |
| `library-startup-browser-20261101-07.json` (synthetic 0) | `697592b8724eef740435214b97b473b13feae048610ca90c9bcad6cd2b61016a` |
| Matching `08.json` (synthetic 1) | `79f655afae2471d41c52639a795b583819e9e19b1a8ff0f226d3286f40e5567f` |
| Matching `19.json` (recovery, empty warning/error log) | `36b80bb43696c511b4221518fd1040e343b3d8db6e13a52b702eca0a20dc6c80` |
| Matching `19.jpg` (inspected screenshot) | `a07865e9dacffb0cd808fe31870b7ffedd6a53187ab927fccad782b5efbea648` |

`library-checkpoint-audit-01.json` passes twenty checks: twenty compiled owned identities, both generated outputs, all nine seed identities/bounds, exact build/generation/browser identity, actual title-ID formatting/trace/next-stop evidence, regressions/Clear/logs, unchanged supplied XBE and clean vendor/header at the existing pin. The module retains fixed unshared 128 MiB Wasm / 64 MiB guest backing; these are proof bounds, not measured peak memory or release-device performance.

Eighteen source-owned runtime/admission/delivery cases and nineteen Python cases (sixteen seed/admission and three memory-emitter) pass locally. CI/publication/whitespace results are recorded separately in status/handoff. CI executes no original game or private title toolchain.

Private builds `01..06` and browser `01..06.json` retain successive real dependency stops, the scoped Clang style failure, the 64-event capacity stop and the pre-update verifier result. They are not relabeled passes. Final `09..15.json` preserve passing 40/36/31/27/22/20/16-check modules. `16/17.json` reject missing volume metadata/malformed Wasm; `18/20.json` verify Clear; `19.json` verifies recovery. Final warning/error logs are empty. The temporary proof tab and verified source-only host were cleared and closed.

## Next concrete task and unknowns

Reuse **`recomp-library-next-05/seed.json` as the ninth seed**, replacing `next-04` and retaining the first eight seeds. The existing read-only Ghidra project exported only the reached next dependency with the five preceding functions. `0x000338de` has **820 contiguous bytes / 271 instructions**, with further kernel/local calls. Export SHA-256: `ec1720f5d56dfc2d22f45ce410a2f0409ed726b5df829fc412c4baa78efe08b8`. Static export is not REA Evidence or observed execution; no new import/extraction or persistent project change occurred.

Translate that actual helper with all byte/decoder/layout/memory checks, run both browser profiles and implement only genuinely reached service contracts. Do not infer its successful behavior from names or substitute a success stub. The original helper, broader formatting behavior, original disk/settings/clock/shutdown state, full memory/exceptions/scheduling, disc-file mounting/reads, graphics/audio/controllers, browser-local conversion, exact original parity and every planned online/social/control feature remain unaccepted. No failed probe authorizes an automatic route change or reduction of requirements.
