# One default-off synthetic raw-DWORD binding completes the ordinal-2 query

November 4, 2026. The accepted numeric-6 component now has one explicit, default-off guest-call binding. A synthetic profile supplies a raw DWORD; the binding writes it through the component, clears the stdcall frame and leaves EAX unchanged. The default profile still stops unsupported at ordinal 2. In the same rebuilt module, the control stops at status 1 on ordinal 2 and the explicit-word profile continues to untranslated `0x1F79A0`. **This is a labeled synthetic experiment, not original service semantics or a playable screen.**

The Sol architect approved this binding at `2026-11-04T06:45:32.8167397-05:00` and froze further disc/FATX expansion and guest wiring pending a reached dependency or review.

## Adapter and behavior

- `src/runtime/av_query_adapter.h/.c` wraps `ff_av_query_write_result` for the observed `(0, 6, 0, Result)` call.
- On success it writes exactly four bytewise little-endian bytes, advances `esp` by 20 (return address plus four stdcall arguments) and leaves `eax` unchanged.
- On denial, including a null config, bad shape, bad pointer, read-only/unmapped span or stack-wrap address, it changes no output byte, `esp` or `eax`.
- `tests/av_query_adapter_test.c` tests the actual adapter, not a duplicate writer: aligned/unaligned success, null config, read-only/null/unmapped/wrong-shape denial, stack-wrap denial and unchanged neighbors/EAX/ESP. It runs optimized and under ASan/UBSan (native suite 20/20).

## Profile and report contract

- `device.avQueryWord` is an optional explicit 32-bit integer. It has no default and is never composed from settings; the parser rejects missing/invalid/non-integer values. `describeMmioExperiments` reports `avQuery` only when the field is present.
- The worker calls `ff_probe_av_query_word` before the run and fails closed if the module cannot set it. It records `avQueryWord` and `experiments: ..., 'avQuery'`, and records the completed call as `{ ordinal: 2, status: 0, arguments: [0,6,0,2111264], output: { rawWord, address, eaxBefore, espBefore, eaxAfter, espAfter } }`.
- The probe resolves ordinal 2 to the adapter only when the control is ready. Without it, the existing status-1 unsupported path records the attempt and stops.
- `proof-report.mjs` accepts `avQuery` and `avQueryWord` as a pair and requires at least one recorded ordinal-2 call. A `status: 0` call must use exactly the observed `(0, 6, 0, nonzero Result)` shape, carry `output.address` equal to the fourth argument, an `output.rawWord` equal to the declared word, an unchanged `eax`, a non-wrapping `espBefore` and `espAfter === espBefore + 20`. A configured profile may instead truthfully record a `status: 1` denial with no output, so a denied or pre-query report is preserved rather than presented as success; because that denial is the unsupported-service stop itself, a report with a status-1 ordinal-2 call and no completed call must stop at status 1 on ordinal 2. Without the field, status-0 or output-bearing ordinal-2 calls remain rejected. The static service descriptor stays neutral.

## Guest-run VOID/EAX/stack effect

The probe now captures `eax` and `esp` immediately before the ordinal-2 adapter call and again after it returns. On success it records `eaxBefore`, `espBefore`, `eaxAfter` and `espAfter`; a mismatch stops the run with status 4 as an internal proof failure. `proof-report.mjs` requires all four values, `eaxAfter === eaxBefore`, `espBefore <= 0xffffffff - 20` and `espAfter === (espBefore + 20) >>> 0`. This checks the integration path in addition to the native adapter fixture.

In the rebuilt module the completed call is:
`{ status: 0, arguments: [0,6,0,2111264], output: { rawWord: 0x00800303, address: 2111264, eaxBefore: 0, espBefore: 62983352, eaxAfter: 0, espAfter: 62983372 } }`
so `eax` is unchanged, `esp` advances by exactly 20 bytes (`0x3C10CB8` to `0x3C10CCC`) and `output.address` matches the fourth call argument. These are observed guest-run values, not a console or service claim.

## Same-module comparison

Module `recomp-post-memory-build-70` rebuilt with the guest-run capture and output address: **SHA-256 `cd2fac97df95a4289b20742768555a080a995e2f16c774a36da25a856720a1a0`**; source SHA-256 remains `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The earlier `588c0dd5…` and `4e03dbe3…` modules remain historical runs without the output-address field.

| profile | input | stop | routines | kernel ops |
| --- | --- | --- | --- | --- |
| `settings-pfifo-reset3.json` | no `avQueryWord` | **status 1 at `0xFE000094`**, ordinal 2, return `0x1FD429` | 279 | 404 |
| `settings-pfifo-reset3-avquery.json` | explicit `device.avQueryWord: 0x00800303` | **status 2 at `0x1F79A0`**, return `0x1F83ED` | 280 | 405 |

The control's ordinal-2 call remains `{ status: 1, arguments: [0,6,0,2111264] }` with no output. The experiment's call now carries the output address and CPU effect above; its experiments list includes `avQuery`. Both reports validate, their ordinal-2 arguments are paired, and `tools/compare-proof-profiles.mjs` reproduces the table. The raw word is a synthetic test value chosen from the observed consumer domain (PAL-I, SCART, 50 Hz), not a console default and not the withdrawn composer.

## Contract hardening and reproducible comparison

A source review of the committed binding found two fail-closed gaps in `proof-report.mjs`; both are fixed and covered by `tests/proof-report.test.mjs`:

- The "avQuery requires a completed ordinal-2 call" check sat inside `if (report.calls !== undefined)`, so a report that declared `avQuery` and `avQueryWord` but omitted `calls` validated without any completed call. The check now runs after the optional call-list block, so an omitted or empty list both fail. Reports without the experiment may still omit `calls`.
- `avQueryWord` was checked only as a non-negative integer, so a value above `0xffffffff` passed the report even though the profile parser rejects it. The report now applies the same DWORD bound.

`tools/compare-proof-profiles.mjs` makes the same-module comparison repeatable instead of an ad hoc script. It validates both reports, requires equal `sourceSha256`/`moduleSha256`, requires `avQuery` to be the only added experiment with the control carrying no raw word, requires the control to stop on the unsupported ordinal-2 call and the experiment to complete it and progress, and rejects a missing call list, an extra input or a non-decreasing run. A second source review then closed two integrity gaps in that tool, and a third pass tightened the controlled comparison further: it requires both reports to carry `deviceRegisters.supplied`, compares the whole supplied object canonically (including unknown fields) so a differing window/register/zero-range or extra input is rejected, requires the control and experiment ordinal-2 call arguments to be identical, and rejects a repeated experiment name instead of silently collapsing it into a set. `tests/compare-proof-profiles.test.mjs` covers the accepted comparison, a different module/executable, a control that does not stop on ordinal 2, an experiment that does not progress, an extra experiment, a missing call list, a non-DWORD word, matching/differing/absent supplied device inputs, unknown supplied fields, paired call arguments and repeated experiment names. Run against the retained reports, it reproduces: control status 1 at `0xFE000094` (279/404), experiment status 2 at `0x1F79A0` (280/405), next stop `0x1F79A0`; their supplied device inputs and ordinal-2 arguments are identical.

The independent GLM review completed a partial static pass (the reviewer hit its step cap) and found no material defect in the four invariants: denial/success separation, exact-shape binding to `(0,6,0,Result)` and the fourth-argument write, paired-call comparison, and canonical full supplied-profile equality. It listed bounded follow-up reads; `device-report.mjs` emits `supplied`, `parseLocalMmioProfile` bounds `avQueryWord` to a raw DWORD, and `describeKernelService(2)` carries only the stdcall/16-byte/void facts. This is a partial independent review, not a full approval; the residual limits it named (denial-cause detail on a single report, presentation labeling, guest memory not re-read at stop) remain recorded.

## Limits and next stop

- The binding supplies one synthetic raw DWORD only when explicitly selected; the canonical/default path remains unsupported and still stops at ordinal 2.
- This does not establish the original kernel's option-6 semantics, a console mode, encoder behavior, timing, fidelity or playability.
- `0x1F79A0` is only the next untranslated boundary. It is not admitted, translated or treated as progress.
- Disc/FATX expansion and guest wiring stay frozen pending a reached dependency or review; the geometry lane remains paused.

## Validation

Runtime suite 174/174, evidence 4/4, native 20/20, title-seed 25/25, `npm run check` 352 source files and whitespace checks pass. Both reset reports validate on module `cd2fac97…` and the comparison tool reproduces them. Private profiles, module and reports remain local/ignored.
