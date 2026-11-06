# Original execution route

The selected route is **static recompilation of the original executable to WebAssembly**, with compatible runtime services executing entirely in a normal browser. Decision D023 confirms this direction. The browser-only, exact-original requirement supersedes the earlier approximate-reconstruction plan.

[Current status](PROJECT_STATUS.md) records the latest bounded startup observation. [The roadmap](../DEVELOPMENT_PLAN.md) defines the product, and [runtime gates](RUNTIME_MILESTONES.md) define what must be demonstrated next.

## Route choices

| Route | Role in this project | Limit |
| --- | --- | --- |
| Static recompilation to WebAssembly | Selected implementation direction | Complete title/runtime, browser-local conversion, graphics/audio, fidelity, and online authority remain unaccepted |
| Original Xbox emulation | Supporting reference and component research | Native compatibility or a cross-build does not establish this title in a browser |
| Independently written reconstruction | Retained Twisted System prototype and engineering research | Approximation does not meet the original-game requirement; an identical replica would need separate full proof |
| Native helpers or streamed execution | Excluded from the product target | Game execution would occur outside the browser |

A failed probe is evidence to record, not authorization to switch routes or relax scope. Reuse retained projects, source pins, and findings before repeating setup. Public emulator/recompiler documentation describes those projects; it is not evidence that Fuzion Frenzy works here.

## Proof required

The next visible product proof is an original title/menu screen responding to a bounded player input sequence, with the named paired browser reports and independent audit. A parsed executable, generated C/Wasm, converted model, homebrew triangle, or native window does not satisfy that proof.

Full acceptance additionally covers the original catalog/modes, reference behavior and presentation, local conversion, save/tournament behavior, controls, and the planned online/social experience. Keep the [requirements ledger](ORIGINAL_EXECUTION_PROGRESS.md) explicit about what remains open.

## Content and authority

Public code may contain independently authored runtime/tooling and curated findings. Generated modules containing original instructions/data remain local; they are not source-only distribution artifacts. End users must eventually convert compatible files in the browser. The developer's native compilation workflow is not that finished conversion path.

The recompilation route also needs an online-authority design. Uploading RAM/save states can leak original content, and an input relay alone does not establish server-owned outcomes. Resolve that contract before implementing online play.

The [prior assessment](archive/ORIGINAL_EXECUTION_ROUTE_PRIOR_20261106.md) retains the dated xemu, QEMU/Wasm, xboxrecomp, and other route comparisons. Those observations remain historical leads rather than current delivery claims.
