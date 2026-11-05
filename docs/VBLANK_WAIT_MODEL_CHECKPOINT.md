# Ordinal-159 vblank wait model: gate refused, event layout unconfirmed

November 5, 2026. Bounded, default-off senior service model authorized by the architect subagent (about 06:45 local, opencode-go GLM-5.3) with binding corrections: reuse `pcrtcVblanks` (no new input), enforce full five-argument equality, write the event only through the pinned event mechanism, and halt if the event gate refuses. **No translation admission, export, seed or hook change; the consumer path stays default-off.**

## What was built

| Path | Purpose |
| --- | --- |
| `src/runtime/vblank_wait.c/.h` | Pure decision: exact arguments (`Object 0x00206080`, reason 6, mode 1, alertable 0, timeout NULL), a remaining declared delivery, and an unsignaled synchronization event (type 1, signal 0) |
| `tests/vblank_wait_test.c` | Accept, no-delivery, per-argument mismatch, wrong type and already-signaled cases |
| `tools/fixtures/thread_startup_probe.c` | `wait_vblank` handler bound only when `pcrtcVblanks` declares deliveries; one PCRTC vblank plus signal-then-consume of the event per matching call; refusal keeps the unsupported-service stop; `ff_probe_vblank_word` diagnostics |
| `tools/run-native-tests.mjs` | Registers the new fixture |
| `src/runtime/thread-proof-worker.mjs` | Reports `vblankWaitState` (calls, type, signal, decision, declared) |

The ISR is **not executed** (the published interrupt vector delivers nothing); the model substitutes its observable event signal, as the architect permitted. No register acknowledge is invented; the PCRTC pending bit is left to the guest's own read/ack path. No timing claim is made.

## Evidence

Module `recomp-post-memory-build-114`: **717,065 bytes**, SHA-256 `b3ec6b2b9de112d01f71ec2e00a16b02c18569ded93e3aaa0fd5358b5b83f788`.

- **Control** (no `pcrtcVblanks`, no consumer): status 13 at `0xFD800044`, 339 routines / 405 kernel ops — unchanged.
- **Experiment** (consumer + `pcrtcVblanks: 3`): the consumer still drains 499 words and releases the fence 3 -> 9; at ordinal 159 the handler is called **once** (`calls 1`, `declared 3`) and observes **type `0x00040000`, signal 0**, so the decision is **3 (`FF_VBLANK_WAIT_EVENT`)**. The gate **refused**: no PCRTC vblank was produced, no event word was written (`pending 0`, `enabled 0`, `events 3`), and the stop remains **status 1 at `0xFE000094`, ordinal 159, 340/405**, return `0x1F5543`.

## Finding

The object at `0x00206080` does **not** have the pinned generic event layout: its first dword is `0x00040000`, not a synchronization-event type, and its signal word is 0. Modeling the wait as a KEVENT would require an assumption the evidence does not support, so the model refused and wrote nothing — the authorized halt.

**Next read-only step (for the service decision):** characterize the `0x00206080` object (its type/magic, size and signal field, and which routine initializes it) before any further model. The current gate and native decision remain default-off and inert.

## Checks

`test:native` **38/38** (optimized and ASan/UBSan) · `test:runtime` pass · `check` 458 files · whitespace clean. No original content; no rendering or playability claim.

## Type-gate correction and result

The pinned bridge (`kernel_bridge.c:7646-7648`, `:8342`, `:2021`, `:2056-2057`) recognizes title-built events by decoding the **full first header dword**: low byte 0 (notification) or 1 (synchronization) and Size byte (bits 16-23) == 4. The refused header `0x00040000` is therefore a valid **notification** event, and the original type-1-only gate was too strict. The architect accepted the correction: decode the header, accept either type, and consume (write 0 at `object+4`) **only for type 1**; notification leaves SignalState = 1.

Module `recomp-post-memory-build-115`: **717,097 bytes**, SHA-256 `c7b94e38d9a8313450e5e32b99aa944afb7293eedcf26da2d1495ba5092081f5`.

| run | stop | routines | kernel ops |
| --- | --- | --- | --- |
| control (no vblanks) | status 13 at `0xFD800044` | 339 | 405 |
| consumer + 3 vblanks | **status 1 at `0xFE0000A4`, ordinal 3** | **340** | **406** |

The experiment accepted the wait once (`calls 1`, header `0x00040000`, signal 0, decision **0**, `declared` 3 -> 2), and `D3DDevice_BlockUntilVerticalBlank` exited. The run advanced to a **new service boundary**: ordinal **3**, status 1, return `0x202231` inside `D3DDevice_Present`'s submit loop. The consumer still drained 499 words and released the fence 3 -> 9.

This supersedes D364's "unsignaled synchronization event" restriction and D365's flag-field reading **for this object under ordinal 159**; the static writers recorded in D365 are the direct SignalState reset path the pinned bridge expects. The next decision is ordinal 3; no ISR/IRQ/timing was executed and no rendering or playability is claimed.
