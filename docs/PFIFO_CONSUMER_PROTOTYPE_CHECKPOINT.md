# PFIFO pusher prototype: verified component, unbounded consumer run

November 5, 2026. Under the architect-subagent authorization of about 04:10 local (opencode-go GLM-5.3, one bounded prototype, fail-closed refusal accepted), this cycle added a default-off PFIFO pusher step and its native tests. The **default path is unchanged**; the opt-in consumer run **did not terminate** and is recorded here as a new boundary. **There is no controllable original screen.**

## What was built

| Path | Purpose |
| --- | --- |
| `src/runtime/pfifo_pusher.h` / `.c` | Pure bounded pusher step: one method packet, jump or ring jump per call; increasing/non-increasing headers; call/return, unknown word, budget, protection, ring, span and read refusals; LOW_MARK clear-then-set through the existing status seam; never executes a method or writes guest memory |
| `tests/pfifo_pusher_test.c` | Native scenarios for advance, empty, GET/PUT outside, unaligned/invalid ring, span-not-written, unknown, call/return, both jumps, budget, protection, read failure and NULL status |
| `tools/fixtures/thread_startup_probe.c` | Default-off `ff_probe_pfifo_consumer` input, accounting exports, and one gated step at the status-13 DMA_GET site with a 64-step budget |
| `src/runtime/local-title-proof.mjs` | Parses the `pfifoConsumer` device input; lists it as an experiment |
| `src/runtime/thread-proof-worker.mjs` | Enables the input and reports `pfifoConsumerState` |
| `src/runtime/proof-report.mjs` | Requires the experiment/state pair; bounds and checks the accounting |
| `tools/run-native-tests.mjs` | Registers the new native fixture |

The pusher model matches the pinned reference's fetch/decode subset: method headers `(w & 0xE0030003) == 0` / `== 0x40000000`, jumps `(w & 3) == 1`, ring jumps `(w & 0xE0000003) == 0x20000000`, and call/return halts. GET advances only over words the caller confirms the guest CPU wrote; a packet may not cross the submitted PUT.

## Checks

- `npm run test:native`: **36/36** component runs pass, including the new `pfifo_pusher` fixture in optimized and ASan/UBSan modes.
- `npm run test:runtime`: 196 pass / 0 fail.
- `npm run check`: 452 source files pass the preparation and publication guard.
- `npm run test:evidence`: 6/6.
- `node --test tests/proof-report.test.mjs`: 21/21, including the new consumer-state pairing and accounting cases.
- `git diff --check`: clean.

## Control run (default path)

Module `recomp-post-memory-build-106`: **715,696 bytes**, SHA-256 `6b57857762e71a306d694fecc05ef3e1e2ae1a596f43bd1243c627c13d2d5937`.

The retained avQuery profile, consumer absent: **status 13 at `0xFD800044`**, **339 routines / 405 kernel operations**, `stopReason: "pushbuffer consumption required"`, and no `pfifoConsumerState`. This exactly reproduces the pre-prototype boundary, so the default remains fail-closed.

## Consumer run (opt-in, synthetic profile)

A copy of the profile with `"pfifoConsumer": true` was run against the same module. It wrote **no report** and did not terminate within 300 seconds; the earlier attempt also exceeded 900 seconds.

- The 64-step GET-read budget did **not** fire, so the run did not stay in the GET-read wait. It passed the PFIFO wait and entered an **unbounded CPU-side wait** that makes no progress the harness can classify.
- No stop address, status or refusal classification was produced, so the pusher advance itself is not claimed as observed beyond the fact that the wait stopped blocking.
- Per the authorization's halt conditions, the run was not chased, no bits were raised, and no recovery path was modeled.

**This is a new boundary:** after PFIFO consumption the title waits on a condition the synthetic harness neither models nor bounds. The next step is an explicit execution/instruction budget or evidence about that completion condition — a fresh architect decision.

## VM-access budget attempt (this cycle)

A run-level guard was added: after the first pusher step, the harness counts guarded guest-memory accesses and stops at the existing status-13 site with the budget classification after 500,000 accesses. Module `recomp-post-memory-build-107` is **715,740 bytes**, SHA-256 `03f9449053c9aac2eb31f82c24fa0d756cb8bab562e39db96061a4a934048527`.

- **Control** on module 107 still reproduces the boundary exactly: status 13 at `0xFD800044`, **339 routines / 405 kernel operations**, no consumer state.
- **Consumer run with the budget** no longer hangs silently: it wrote `{"kind":"error","message":"RuntimeError: unreachable"}`. That is a Wasm trap from a `stop()`, but the worker surfaced it as a generic error instead of a classified stop record, and no `pfifoConsumerState` was captured.
- The budget stop therefore cannot yet be distinguished from another trap, and the run still does not yield a usable next-stop classification. The consumer remains **unproven and default-off**.
- The next step is a worker-level fix: when the trap is an expected stop, read the probe stop record and classify it, then re-run. That is report plumbing, not more model widening.

## Safety and status

- The consumer is **default-off**; the committed profiles do not enable it, and the control above proves the default is unchanged.
- No translation admission, export or seed change was made; translation 83 / module 104 evidence is unchanged.
- The prototype is not a rendering, command-correctness or playability claim, and the consumer path must not be enabled in retained runs until it is bounded.

## Record

Decision `D347` records the prototype and the unbounded run. Main `4b94a64` is still not an ancestor of this branch; the session denies `git merge`, so ancestry sync remains deferred.
