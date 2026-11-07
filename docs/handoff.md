# Startup reaches a FATX file write

The original-startup experiment passed the second FATX query and seek with four-byte-aligned guest buffers. The current Chrome stop is status 9 at ordinal 236, `NtWriteFile`, value `0xC00000BB`, with return address `0x001E9004`, 399 routines and 472 completed kernel operations. The fixed control and inert runs preserved their stops and stable traces. [Current status](status.md) records the exact boundaries, inputs, checks and evidence limits.

## What is retained

The matching private baseline and fresh reports are available on the research workstation. The latest Chrome-verified profile contains 441 seed functions and two checked direct entries, with 20,687 emitted instructions. Its module digest starts with `c5c487f5`. Original inputs, generated title code and independent audits remain in ignored local research. The public source-only loopback harness takes original inputs through local file selection.

The product goal remains the original Fuzion Frenzy executing entirely in a browser through static recompilation to WebAssembly, with original fidelity and the planned online, social and control features. The [roadmap](../plans.md) and [five milestones](milestones.md) define acceptance. M1 still requires a controllable original screen and a playable original minigame sequence.

## Resuming development

Verify the reached 45-byte write on the reopened FATX file. Check the guest input, current position, owned allocation and mutation preflight. Retain both checked direct entries, debug-print handling and FATX services, and replay the fixed control, inert and experiment cases.

Use [analysis setup](analysis.md) and [development](development.md) for the private prerequisites and public checks. [Local agent defaults](agents.md) describe session coordination.
