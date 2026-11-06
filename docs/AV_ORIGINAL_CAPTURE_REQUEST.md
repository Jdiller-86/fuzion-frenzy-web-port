> **Evidence record.** This document describes its own checkpoint and may contain superseded observations or next tasks. See [current project status](PROJECT_STATUS.md) and the [research index](RESEARCH.md) before using it as current guidance.

# Ordinal-2 original-hardware observation request

November 4, 2026. This document records an **optional** original-hardware capture for the `AvSendTVEncoderOption` option-6 value. The consumer domain is known, but the original service's response is not. The unconnected numeric-6 component and the fail-closed ordinal-2 runtime do not depend on this capture; it is fidelity evidence only. The Sol architect reviewed the evidence at `2026-11-04T05:26:20.5945272-05:00` and later superseded the earlier prerequisite framing at `2026-11-04T05:44:57.8570169-05:00`: no further capture schema or replacement-source survey is required. **No service model or runtime behavior changes from this document.**

## Why the current evidence is insufficient

- Cxbx-Reloaded at `585c49a50af1255ab155099e06f24505f9c5a800` defines numeric option 6 as `AV_QUERY_AV_CAPABILITIES` and handles it with `AvQueryAvCapabilities()`; it does not define `AV_OPTION_QUERY_AVPACK` and does not fall through to a no-op.
- The locally retained `xboxrecomp-source` at `f0f8a461496a4f23d9b0aeaa284b8d70ba7ab82b` is a kernel replacement. Its `0x00400104` value is a D3D compatibility default chosen for mode-table matching (`src/kernel/kernel_hal.c:1463-1510`), not an original-kernel observation.
- The game's own table at `0x202C38` (0xBF rows of 0xC bytes) accepts NTSC-M/J/PAL-I standards with SCART, HDTV, VGA or wildcard packs; it has no standard-zero row. This proves a zero result is incompatible, but it does not reveal the console's actual mode or the service's transformation.
- A raw EEPROM alone is insufficient: it does not establish the service's output or how stored configuration and connected hardware map to the result word.

## Requested observation

Capture the original game's actual call:

```text
AvSendTVEncoderOption(RegisterBase = 0, Option = 6, Param = 0, Result = <pointer>)
```

The capture must show:

- the four arguments and the four bytes at `Result` immediately before and after the original service executes;
- the service returning normally to the game;
- the game identity and original kernel version;
- the console video configuration and the connected AV cable/pack;
- how the observation was collected, including any instrumentation that could affect it.

## Capture record

Store the capture locally as JSON and validate it with:

```text
node tools/validate-av-observation.mjs <local-capture.json>
```

The validator requires `source: "original-hardware"`, the exact `(0, 6, 0, Result)` call shape, four before/after bytes, a normal return, game/kernel/console/collection fields, and one to four confirmations. It prints the measured word and whether it matches the observed consumer domain. A mismatch is retained as evidence, not rejected or altered. It never writes or uploads.

The following template is not evidence until every placeholder is replaced with a measured value; the validator rejects the placeholders.

```json
{
  "schema": "av-observation/1",
  "source": "original-hardware",
  "game": { "sha256": "<64 hex characters>", "name": "Fuzion Frenzy (USA)" },
  "kernel": { "version": "<observed kernel version>" },
  "console": {
    "videoConfiguration": "<observed video configuration>",
    "avPack": "<observed AV pack>",
    "cable": "<observed cable>"
  },
  "call": { "registerBase": 0, "option": 6, "param": 0, "resultAddress": "0x203720" },
  "before": ["0xNN", "0xNN", "0xNN", "0xNN"],
  "after": ["0xNN", "0xNN", "0xNN", "0xNN"],
  "returnedNormally": true,
  "collection": "<how the capture was collected>",
  "instrumentation": "<none or details of any instrumentation>",
  "confirmations": [
    {
      "before": ["0xNN", "0xNN", "0xNN", "0xNN"],
      "after": ["0xNN", "0xNN", "0xNN", "0xNN"],
      "returnedNormally": true
    }
  ]
}
```

A differing confirmation requires a `variationNote` in that confirmation object. Raw captures stay local; only a validated summary or curated finding may be committed.

## Acceptance criteria

1. The observation is attributable to the original kernel, not an emulator, replacement kernel or inferred value, and it is the same game call.
2. The output is measured, not composed from table fields or inferred from a displayed mode.
3. A second observation under the same stated configuration confirms the result, or any variation is explicitly recorded.
4. The measured word is compared with the retained consumer predicates, including the refresh consumption at `0x1FD4A8`. A mismatch remains evidence and is not altered to obtain a table match.
5. Raw captures and player artifacts stay local. The source-only deliverable contains only a curated finding and checkpoint/status/decision updates, without copied kernel/game content or console identifiers.
6. The current module and fail-closed ordinal-2 behavior are preserved. No runtime handler, CPU/stack change, frontier admission, geometry change or reporting-framework expansion is made in this step.

## Current external prerequisite

No original Xbox kernel, BIOS, EEPROM or original-console observation is available in this worktree. The supplied game files are present, but they cannot produce this observation. This capture is optional fidelity evidence, not a prerequisite for the unconnected component or for keeping ordinal 2 unsupported. No replacement value is invented when it is absent.

## What the observation would and would not establish

A successful observation would ground **one configuration-specific response** for the exact observed console and game call. It would not establish general option-6 semantics, other encoder options, capabilities, display timing, rendering fidelity or playability. Any runtime implementation would require a separate bounded decision and evidence review.

## Next task

If a capture becomes available, it may be recorded and validated with the tool above. Keep ordinal 2 unsupported, numeric option 6 neutral and `0x1F79A0` unadmitted; the component work does not wait on this evidence.
