# Empty-ring profile adopted; default build reaches 0x1FB3B0 with the profile and stays fail-closed without it

November 4, 2026. The validated empty-ring model is now part of the default source build, driven by an explicit synthetic profile rather than a compile-time context hack. With the profile, the original startup passes the PFIFO wait and stops at untranslated `0x1FB3B0`; without the profile, the same build retains status 13. **There is no playable screen, renderer or device model.**

## Source-owned change

- `src/runtime/pfifo_channel.h/.c` — the bounded empty-ring state (already added; unchanged behavior).
- `src/runtime/local-title-proof.mjs` — `parseLocalPfifoProfile` reads an optional `pfifo` object:
  - `kind: "local-pfifo-profile"`, `synthetic: true`;
  - aligned 32-bit `base`, `end`, `put` with `base < end` and `put == base`.
  - The settings profile size bound is raised from 4 KB to 8 KB to admit the complete explicit profile; the HTML proof page uses the same bound.
- `src/runtime/thread-proof-worker.mjs` — imports the parser, calls `ff_probe_pfifo_profile` when the module exports it and the profile is present, and reports `pfifoProfile`.
- `tools/fixtures/thread_startup_probe.c` — `FF_PFIFO_PROOF` adds `ff_probe_pfifo_profile`; on a `DMA_GET` read with a configured empty ring it serves the base. Without a profile it does nothing.
- `tests/startup-proof.test.mjs` — covers a missing profile, a valid observed profile and invalid kind/synthetic/alignment/end/put cases; the settings-size test uses the new 8 KB bound.

## Diagnostic build

Private build `research/verification/recomp-post-memory-build-58` (default `FF_PFIFO_PROOF`, no `FF_ALLOW_DMA_GET_READ` or compile-time context probe): 527,379 bytes, SHA-256 `6b4ddf6f6f14d1c5716b70ee0c6b6b2a71e8421b9d033063f30885e6cbf342de`.

Two runs with the existing worker and retained XBE:

| Settings | Stop | Routines | Kernel operations |
| --- | --- | --- | --- |
| `settings-pfifo.json` (explicit empty-ring profile) | status 2, `0x1FB3B0`, return `0x1FB4FD` | 262 | 402 |
| `settings-mmio-20.json` (no profile) | status 13, `0xFD800044` | 261 | 402 |

83/83 initialization callbacks returned in both runs.

## Interpretation

- The empty-ring model is now an explicit, profile-driven part of the default build. It is not a hidden context hack: the profile supplies the observed 512 KB ring values, and the component validates them.
- The model changes behavior only when the profile is present. The no-profile path remains fail-closed at status 13.
- The original code reaches a new dependency at `0x1FB3B0`; the PFIFO wait is no longer the blocker for this profile. The next step is to translate/admit that routine and continue.

## Limits

The profile is one explicit synthetic input; it does not establish the kernel D3D submission path, a populated command stream, a general consumer, rendering, audio, fidelity or playability. The model handles only the empty-ring case. The diagnostic/private reports stay local.

## Validation

`tests/startup-proof.test.mjs` passes 40/40; runtime suite 109/109; Python memory-access 10/10; title-seed 25/25; `npm run check` and whitespace checks pass. Module `recomp-post-memory-build-51` remains preserved as the prior fail-closed build.

## Next task

Translate/admit the reached original routine at `0x1FB3B0` and continue the startup with the adopted profile. Do not consume a non-empty ring or claim rendering/fidelity without evidence.
