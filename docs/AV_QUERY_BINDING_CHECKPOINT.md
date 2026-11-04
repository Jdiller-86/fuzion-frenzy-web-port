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
- The worker calls `ff_probe_av_query_word` before the run and fails closed if the module cannot set it. It records `avQueryWord` and `experiments: ..., 'avQuery'`, and records the completed call as `{ ordinal: 2, status: 0, arguments: [0,6,0,2111264], output: { rawWord } }`.
- The probe resolves ordinal 2 to the adapter only when the control is ready. Without it, the existing status-1 unsupported path records the attempt and stops.
- `proof-report.mjs` accepts `avQuery` and `avQueryWord` as a pair. When present, every ordinal-2 call must complete with `status: 0` and an `output.rawWord` equal to the declared word; at least one completed call is required. Without the field, status-0 or output-bearing ordinal-2 calls remain rejected. The static service descriptor stays neutral.

## Same-module comparison

Module `recomp-post-memory-build-70` rebuilt with the probe/adapters: **SHA-256 `4e03dbe3dc0cb57b02fe728b213014f12e213d271fc133dbdc433d3775db4b4a`**; source SHA-256 remains `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9`.

| profile | input | stop | routines | kernel ops |
| --- | --- | --- | --- | --- |
| `settings-pfifo-reset3.json` | no `avQueryWord` | **status 1 at `0xFE000094`**, ordinal 2, return `0x1FD429` | 279 | 404 |
| `settings-pfifo-reset3-avquery.json` | explicit `device.avQueryWord: 0x00800303` | **status 2 at `0x1F79A0`**, return `0x1F83ED` | 280 | 405 |

The control's ordinal-2 call remains `{ status: 1, arguments: [0,6,0,2111264] }` with no output. The experiment's call is `{ status: 0, arguments: [0,6,0,2111264], output: { rawWord: 0x00800303 } }`; its experiments list includes `avQuery`. Both reports validate. The raw word is a synthetic test value chosen from the observed consumer domain (PAL-I, SCART, 50 Hz), not a console default and not the withdrawn composer.

## Limits and next stop

- The binding supplies one synthetic raw DWORD only when explicitly selected; the canonical/default path remains unsupported and still stops at ordinal 2.
- This does not establish the original kernel's option-6 semantics, a console mode, encoder behavior, timing, fidelity or playability.
- `0x1F79A0` is only the next untranslated boundary. It is not admitted, translated or treated as progress.
- Disc/FATX expansion and guest wiring stay frozen pending a reached dependency or review; the geometry lane remains paused.

## Validation

Runtime suite 159/159, evidence 4/4, native 20/20, title-seed 25/25, `npm run check` 350 source files and whitespace checks pass. Both reset reports validate. Private profiles, module and reports remain local/ignored.
