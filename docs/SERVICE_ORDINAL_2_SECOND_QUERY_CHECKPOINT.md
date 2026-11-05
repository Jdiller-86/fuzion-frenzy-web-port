# The later ordinal-2 call is an unknown AV option 0x0F (read-only)

November 5, 2026. Read-only characterization of the new frontier after the display-mode declaration. **No implementation, hook change, admission, export, seed or translation change.** The display, vblank and consumer paths remain default-off.

## 1. Observed call

- Call site: `D3DDevice_Present` (`0x00202150..0x002022D0`), the second submit block at `0x00202268`; return `0x00202268` follows `CALL [0x00253450]` (ordinal 2).
- Stop: status 1 at `0xFE000094`, ordinal 2, `modeled: false`, `AvSendTVEncoderOption` (4 args, stdcall, 16 bytes, returns void).
- Observed arguments: **`0xFD000000`, `15` (option `0x0F`), `0`, `62983692` (a Result pointer on the stack)**.
- The earlier startup query `(0, 6, 0, Result)` completed through the synthetic adapter; this later call has a different shape and was denied.

## 2. Pinned semantics

- `bridge_AvSendTVEncoderOption` (`kernel_bridge.c:5429`) calls `xbox_AvSendTVEncoderOption(handle, option, param, Result)` and returns success; the arity table is 16 bytes (`:9749`).
- The pinned implementation (`kernel_hal.c:1455-1520`) handles documented options `0x01`-`0x0E` and has a **default branch for unknown options**: it logs `unknown option`, writes **0** through `Result`, and does not fail.
- `kernel.h:1148-1157` lists the documented options; **`0x0F` is not among them**, so this call is the documented unknown-option path (write 0, succeed).
- The synthetic `av_query_adapter` only accepts the observed `(0, 6, 0, Result)` startup query; every other shape is denied, which is why the run stops here.

## 3. Service decision (for the architect)

The pinned evidence supports a bounded extension of the ordinal-2 adapter: for a call whose shape is the observed late request (`handle 0xFD000000`, option `0x0F`, param 0, Result pointer) — or, more generally, the documented unknown-option default — write 0 through the fourth argument and return success, default-off, with the exact-shape guard and the ordinal-2 discipline preserved. Options:

1. Model the unknown-option default with the exact observed shape (write 0, success), native tests, a pair run to the next stop.
2. Hold the call and choose another unowned task.

No modeled success without the documented unknown-option behavior; the Result write and the handling of any other option remain refusals.

## 4. Limits

Static references plus the retained synthetic stop; no display, encoder or playability claim.

## 5. Bounded unknown-option model (authorized)

The architect subagent authorized one default-off sibling model with binding corrections: exact observed tuple only, one zero write through `Result`, EAX untouched, `esp += 20`, and no extension of the startup adapter. Implemented:

| Path | Purpose |
| --- | --- |
| `src/runtime/av_unknown_option.c/.h` | Pure decision: handle `0xFD000000`, option `0x0F`, param 0, mapped/aligned/writable Result |
| `tests/av_unknown_option_test.c` | Accept; wrong handle/option/param; unmapped pointer; documented-option refusal |
| `tools/fixtures/thread_startup_probe.c` | Default-off `avUnknownOption` setter; ordinal-2 router that keeps the startup `(0,6,0,Result)` path and routes other shapes to the unknown model; one zero write, EAX untouched, `esp += 20`; diagnostics |
| `local-title-proof.mjs` / `thread-proof-worker.mjs` / `proof-report.mjs` | Input parsing, `avUnknownOptionState`, pairing and pinned-acceptance validation; the late unknown-shape call is excluded from the startup-avQuery success checks |

Module `recomp-post-memory-build-118`: **719,152 bytes**, SHA-256 `0b24204ea2b92268bacd347af3d07ec00f4d066ec31a0a1acb3ee436b50d52d7`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control | status 13 at `0xFD800044` | 339 | 405 |
| inert (display profile, no unknown-option input) | status 1 at `0xFE0000A4`, ordinal 3 | 340 | 406 |
| unknown-option input | **status 1 at `0xFE000098`, ordinal 1** | **340** | **408** |

The late call was accepted (`calls 1`, handle `0xFD000000`, option `0x0F`, param 0, written 0, EAX unchanged `62983692`, ESP `62983652 -> 62983672` = +20) and the run advanced to a **new service boundary**: ordinal 1 at `0xFE000098`, 340 routines / 408 kernel operations. The startup `(0,6,0,Result)` query still completed through its existing adapter unchanged.

Provenance caveat: the pinned HAL is the retained xboxrecomp kernel replacement, not an original-console observation; 0 is the documented unknown-option default output, not a guessed capability value. The caller's post-call EAX/Result use is not determined in retained evidence; the run's next stop is reported as the honest boundary.

Checks: `test:native` **42/42**, proof-report **23/23**, `test:runtime` pass, `check` **468 files**, whitespace clean. Consumer, display and unknown-option paths remain default-off. The next decision is ordinal 1.
