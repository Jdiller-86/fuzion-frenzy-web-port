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
- `proof-report.mjs` accepts `avQuery` and `avQueryWord` as a pair. A `status: 0` call must use exactly the observed `(0, 6, 0, nonzero Result)` shape, carry `output.address` equal to the fourth argument, an `output.rawWord` equal to the declared word, an unchanged `eax`, a non-wrapping `espBefore` and `espAfter === espBefore + 20`. A configured profile may truthfully record a `status: 1` denial with no output, or a prequery run in which ordinal 2 was never reached (no ordinal-2 call); a denial, when present, is the unsupported-service stop itself and must stop at status 1 on ordinal 2, even if an earlier ordinal-2 call completed. Any report claiming the status-1 ordinal-2 stop must carry the ordinal-2 denial record; a completed call alone is not a denial and cannot satisfy that stop. A prequery is accepted only when the run stopped before the query. Without the field, status-0 or output-bearing ordinal-2 calls remain rejected. The static service descriptor stays neutral. An unconfigured ordinal-2 call record must report status 1 (denied); a status outside completed (0) or denied (1) is rejected. `tests/proof-report.test.mjs` and `tests/compare-proof-profiles.test.mjs` carry the focused regressions. The thread report's `originalFunctionsReached` list is bounded by the trace limit and type-checked when present; repeated addresses remain valid. The controlled comparison also requires both reached lists when either is present and rejects an experiment that drops a control-reached routine.
- `device-report.mjs` exports one `validateSuppliedDeviceProfile` check used by both `proof-report.mjs` (when `deviceRegisters.supplied` is present) and `tools/compare-proof-profiles.mjs`. It mirrors the parser's bounds: aligned 4096..0x1000000 window, 2-64 distinct in-window registers including `0xfd001804`/`0xfd600140`, and at most eight distinct in-window zero ranges. A report may still omit the profile or carry `null`; the comparison requires it.
- `local-title-proof.mjs` exports `describeAvQueryOutcome`, used by the local `thread_startup_check.html`. It labels a configured avQuery report as completed (raw word/address), denied/unsupported at ordinal 2, or configured but not reached (prequery), so a denial or prequery is never presented as success. It checks the stop/denial before any earlier completion, so a completed-then-denied run is labeled denied, not completed. `tests/av-query-outcome.test.mjs` covers the cases.

## Guest-run VOID/EAX/stack effect

The probe now captures `eax` and `esp` immediately before the ordinal-2 adapter call and again after it returns. On success it records `eaxBefore`, `espBefore`, `eaxAfter` and `espAfter`; a mismatch stops the run with status 4 as an internal proof failure. `proof-report.mjs` requires all four values, `eaxAfter === eaxBefore`, `espBefore <= 0xffffffff - 20` and `espAfter === (espBefore + 20) >>> 0`. This checks the integration path in addition to the native adapter fixture.

In the rebuilt module the completed call is:
`{ status: 0, arguments: [0,6,0,2111264], output: { rawWord: 0x00800303, address: 2111264, eaxBefore: 0, espBefore: 62983352, eaxAfter: 0, espAfter: 62983372 } }`
so `eax` is unchanged, `esp` advances by exactly 20 bytes (`0x3C10CB8` to `0x3C10CCC`) and `output.address` matches the fourth call argument. These are observed guest-run values, not a console or service claim.

## Same-module comparison

Module `recomp-post-memory-build-71` after admitting the reached leaf `0x1F79A0`: **SHA-256 `42ee9909ad83092c021226deb94e49999deb64c38d71049e7b7adb76eeb3077b`**; source SHA-256 remains `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`. The earlier `cd2fac97…` (pre-admission) and `588c0dd5…`/`4e03dbe3…` modules remain historical runs.

| profile | input | stop | routines | kernel ops |
| --- | --- | --- | --- | --- |
| `settings-pfifo-reset3.json` | no `avQueryWord` | **status 1 at `0xFE000094`**, ordinal 2, return `0x1FD429` | 279 | 404 |
| `settings-pfifo-reset3-avquery.json` | explicit `device.avQueryWord: 0x00800303` | **status 2 at `0x1F6EB0`** | 281 | 405 |

The control's ordinal-2 call remains `{ status: 1, arguments: [0,6,0,2111264] }` with no output. The experiment's call carries the output address and CPU effect above; its experiments list includes `avQuery`. The admitted `0x1F79A0` executes and returns, then the run stops at the next untranslated target `0x1F6EB0`. Both reports validate, their ordinal-2 arguments are paired, and `tools/compare-proof-profiles.mjs` reproduces the table. The raw word is a synthetic test value chosen from the observed consumer domain (PAL-I, SCART, 50 Hz), not a console default and not the withdrawn composer.

## Contract hardening and reproducible comparison

A source review of the committed binding found two fail-closed gaps in `proof-report.mjs`; both are fixed and covered by `tests/proof-report.test.mjs`:

- An earlier fix required a completed ordinal-2 call for `avQuery` and rejected an omitted or empty call list. That was too strict for a configured prequery: a profile may configure `avQueryWord` and still stop before ordinal 2. The contract now accepts the empty/omitted list as a truthful prequery; a denial, when present, still pairs with the status-1 ordinal-2 stop. A later guard requires the ordinal-2 call record for any report claiming the status-1 ordinal-2 stop, not only configured prequeries. `tests/proof-report.test.mjs` covers both.
- `avQueryWord` was checked only as a non-negative integer, so a value above `0xffffffff` passed the report even though the profile parser rejects it. The report now applies the same DWORD bound.

`tools/compare-proof-profiles.mjs` makes the same-module comparison repeatable instead of an ad hoc script. It validates both reports, requires equal `sourceSha256`/`moduleSha256`, requires `avQuery` to be the only added experiment with the control carrying no raw word, requires the control to stop on the unsupported ordinal-2 call and the experiment to complete it and progress, and rejects a missing call list, an extra input or a non-decreasing run. A second source review then closed two integrity gaps in that tool, and a third pass tightened the controlled comparison further: it requires both reports to carry `deviceRegisters.supplied`, compares the whole supplied object canonically (including unknown fields) so a differing window/register/zero-range or extra input is rejected, requires the control and experiment ordinal-2 call arguments to be identical, and rejects a repeated experiment name instead of silently collapsing it into a set. It also compares the other declared inputs — console settings, volume, directory, file-storage, disk, PFIFO and memory profiles — canonically, so a non-avQuery input difference cannot pass as controlled, and requires every one of them to be present so two reports that both omit a profile are not treated as a match. `tests/compare-proof-profiles.test.mjs` covers the accepted comparison, a different module/executable, a control that does not stop on ordinal 2, an experiment that does not progress, an extra experiment, a missing call list, a non-DWORD word, matching/differing/absent supplied device inputs, unknown supplied fields, paired call arguments, each declared-input difference and repeated experiment names. Run against the retained reports, it reproduces: control status 1 at `0xFE000094` (279/404), experiment status 2 at `0x1F6EB0` (281/405), next stop `0x1F6EB0`; their supplied device inputs and ordinal-2 arguments are identical.

The architect's `2026-11-04T08:54:03` directive identified the configured-prequery guard. The contract now accepts a configured report with no ordinal-2 call (a truthful prequery), and the pair revalidates on the new module.

The independent GLM review completed a narrow static pass at `2026-11-04T08:45:21`–`08:48:07` and found two material defects: a configured report recording both a completed ordinal-2 call and a status-1 denial could claim any stop, and two reports that both omitted a declared input profile compared as matching. Both are fixed: the denial/stop pairing now applies even after a completed call, and every declared input must be present before comparison. New tests cover the mixed completed+denial case and each missing profile. The earlier partial pass had found no defect in the exact-shape, paired-argument and supplied-profile checks; those remain.

A post-admission narrow GLM review (attempted `2026-11-04T09:15:22`, step-capped after completing the checks) found no material defect in the prequery branch or the admission evidence. The control stop target `0xFE000094`, the `0x1F79A0` enter/exit trace and the `0x1F6EB0` status-2 stop were confirmed directly from the retained reports.

A follow-up narrow GLM review (attempted `2026-11-04T10:05:11`, step-capped after the checks) of the no-call prequery guard also found no material defect. It confirmed the guard is limited to the status-1/ordinal-2 service stop, truthful prequeries, denials and successes still pass, and `ordinalTwoRecorded` is set for every well-formed ordinal-2 record. It noted two minor unexercised control-path cases: an unconfigured ordinal-2 call with a status outside `{0,1}`, and an unconfigured no-call status-1/ordinal-2 stop; the comparison tool closes the latter for retained evidence.

## Limits and next stop

- The binding supplies one synthetic raw DWORD only when explicitly selected; the canonical/default path remains unsupported and still stops at ordinal 2.
- This does not establish the original kernel's option-6 semantics, a console mode, encoder behavior, timing, fidelity or playability.
- `0x1F79A0` is admitted as the next reached leaf; it executed and returned. `0x1F6EB0` is the next untranslated boundary and is not admitted, translated or treated as progress.
- Disc/FATX expansion and guest wiring stay frozen pending a reached dependency or review; the geometry lane remains paused.

## Validation

Runtime suite 179/179, evidence 4/4, native 34/34, title-seed 25/25, `npm run check` 354 source files and whitespace checks pass. Both reset reports validate on module `42ee9909…` and the comparison tool reproduces them. Private profiles, seeds, modules and reports remain local/ignored.
