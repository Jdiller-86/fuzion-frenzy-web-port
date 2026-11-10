# Analysis and private runtime research

Public source checks and the browser prototype do not require original game files or a reverse-engineering installation. See [development](development.md) first. This guide describes the optional private-input workflow used to produce the curated findings.

The [latest checkpoint](status.md) owns current execution claims. Configured tools, exported functions, or a compiled module do not establish original behavior.

For assigned M1 work, follow [resource reuse and fast iteration](resource-reuse.md): reuse the native Fuzion configuration/automatic generator, adapt shared browser runtime interfaces, use applicable OpenCE platform solutions, and automate routine preparation. Use lightweight checks while developing; detailed testing and polish follow M1.

## Pinned tools

| Tool | Recorded pin / purpose |
| --- | --- |
| Ghidra | 12.1.4; static XBE analysis with matching XboxDev loader |
| Java | Eclipse Temurin 21.0.13+9; project-local Ghidra runtime |
| XboxDev XBE loader | `build-202610161023`, built for Ghidra 12.1.4 |
| REA | `rea-agents@6.7.0`; provider-specific read-only evidence tooling |
| xdvdfs | 0.8.3; optional local disc tooling |
| Emscripten | 3.1.50 for the retained feasibility toolchain; separate from the Node application runtime |
| Recompilation candidate | Pinned xboxrecomp source and private Python/compiler preparation workflow |

Exact source revisions, download identities, and digest-verification limits are in [analysis-toolchain.json](../tools/analysis-toolchain.json) and [original-execution-toolchain.json](../tools/original-execution-toolchain.json). These are version records, not a software installer or a current readiness report. The older pins/notes describe their original checkpoint; read newer service records before treating an old unsupported-service note as current.

Keep downloaded tools under ignored `.tools/`. Do not alter system PATH/Java, global model settings, or credentials to reproduce a project-local check.

## Retained local inputs

The original research workstation used:

- `research/reference/default.xbe`: compatible player-supplied executable.
- `research/ghidra-projects/FuzionFrenzySetup.gpr`: persistent analysis project.
- `research/verification/`: private exports, generated modules, reports, and audit artifacts.
- `.tools/emulation/`: compiler/source dependency checkouts and binaries.

These paths are absent from a normal clone. Reuse them if supplied locally; otherwise record the missing prerequisite and continue source-owned work. Do not download game/BIOS/kernel files to fill gaps, repeatedly extract an already verified image, or publish raw analysis output.

## Read-only XBE work

Choose analysis prerequisites from the claim being established. Replaying an already accepted module consumes its executable/module, settings, reports and reviewed harness/source closure; it does not consume a live Ghidra database. Use the explicit replay-purpose gate in [the workflow](development-workflow.md). Its success verifies named replay inputs, not the complete harness or a new original-code fact.

For a candidate that reuses retained bodies, first match the relevant exports, seeds, listings and audits to the accepted input and source identities. A hash-verified original image can also support bounded byte/data checks against an existing witness and independent decoder. Record which authority supports each caller, frame, layout or literal claim. Do not treat a later unanchored assessment or a freshly calculated candidate checksum as historical authority. Live guest memory, provider selection, side effects and actual returns still require runtime guards and observation.

New analysis is necessary only when these retained sources do not cover a required fact. Then retain the default analysis preflight and the read-only procedure below. A matching project-marker or properties-file hash does not prove companion database integrity; an unverified mutable project remains unverified even when it is unnecessary for a particular replay. Preserve existing projects and evidence rather than rebuilding them to satisfy an unrelated gate.

Use the [PowerShell wrapper](../tools/Invoke-ProjectGhidra.ps1) on a workstation with the pinned tools and retained project. From the repository root, this processes the existing project without saving analysis changes:

```powershell
& .\tools\Invoke-ProjectGhidra.ps1 `
  "$PWD\research\ghidra-projects" FuzionFrenzySetup `
  -process default.xbe -noanalysis -readOnly -analysisTimeoutPerFile 120 -max-cpu 2
```

This example requires an already analyzed project; it is not a clean-clone setup command. The [Ghidra helpers](../tools/ghidra) implement bounded export/inspection. Select only the needed helper/functions and keep its output under ignored research. Verify coverage, byte identity, independent decoder agreement, function envelopes, and admission/resource budgets before preparation.

REA 6.7.0's recorded Windows interface did not advertise XBE admission. Installing an XBE loader does not change that boundary; do not rename XBE to PE. Use supported REA providers/skills when available for the named fact and direct Ghidra otherwise. Record Evidence IDs, digests, provider, tool, and limits in [the decision log](decisions.md). Direct Ghidra output is not REA Evidence, and pseudocode is not original source or observed execution.

## Missing function metadata at an observed entry

An actual request pointer is an investigation anchor, not a complete function envelope. Check the retained listing, function containment, executable section, caller reference, predecessor fallthrough, closed control flow/return and neighboring code before deriving an entry. Independent original-byte/decoder/layout checks still apply.

If that evidence supports a previously absent function, derive it automatically only in a read-only analysis session. Preserve the before-state and export a separate seed/provenance record; reject an unexpected body rather than forcing a convenient boundary. Record headless discard/no-save evidence and reopen the retained project to verify its state. Newly established in-memory metadata is different from a preexisting retained function, and a callable entry is different from observed execution. See [current limits](research.md#derived-indirect-entries-and-guest-thread-evidence).

A returning modeled trap can also reveal an incomplete retained envelope. Inspect adjacent bytes, ownership and incoming references before deriving a continuation. The debug-wrapper correction used temporary trap fallthrough metadata and automatic owner re-creation, with no byte changes, forced body or persisted project change. Keep regenerated reference metadata distinct from unchanged original instructions. Validate the derived original tail and the actual browser frame separately; do not substitute a synthetic stack adjustment for missing instructions.

## Browser proof boundary

The [startup server](../tools/serve-startup-proof.mjs) serves an allowlisted source-only harness:

```sh
node tools/serve-startup-proof.mjs
```

It listens at <http://127.0.0.1:4180/> by default. Original executables, private generated modules, and report inputs must be selected locally through the proof workflow; the server does not provide a ready title module. Inspect the applicable [checkpoint](research.md) and proof controls for the required compatible files, explicit profiles, and resource limits.

Retain named control/experiment reports and an independent audit JSON for runtime acceptance. Keep generated code, original bytes, RAM/save states, raw exports, and logs local. Only curated non-content findings belong in public docs.

The versioned tool manifests retain source/digest identities and verification limits. Prior setup transcripts and early export history remain in Git history; machine-specific historical paths are not setup instructions for a new contributor.
