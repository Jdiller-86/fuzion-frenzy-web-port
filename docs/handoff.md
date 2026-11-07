# Startup reaches the next CRT routine

The original-startup experiment completed a guarded INT `0x2D` debug print. The current Chrome stop is status 9 at ordinal 190, `NtCreateFile`, value `0xC00000BB`, with 384 routines and 457 completed kernel operations. The fixed control and inert runs preserved their stops and stable traces. [Current status](status.md) records the exact boundaries, inputs, checks and evidence limits.

## What is retained

The matching private baseline and fresh reports are available on the research workstation. The latest Chrome-verified profile contains 503 seed functions and one checked direct entry, with 20,714 emitted instructions. Its module digest starts with `e756b0ce`. Original inputs, generated title code and independent audits remain in ignored local research. The public source-only loopback harness takes original inputs through local file selection.

The product goal remains the original Fuzion Frenzy executing entirely in a browser through static recompilation to WebAssembly, with original fidelity and the planned online, social and control features. The [roadmap](../plans.md) and [five milestones](milestones.md) define acceptance. M1 still requires a controllable original screen and a playable original minigame sequence.

## Resuming development

The next bounded dependency is the reached FATX `FILE_OPEN_IF` reopen on the same debug-log path that startup created. Retain the checked direct entry and debug-print handler in the build. Reproduce the fixed control, inert and experiment boundaries after each batch.

Use [analysis setup](analysis.md) and [development](development.md) for the private prerequisites and public checks. [Local agent defaults](agents.md) describe session coordination.
