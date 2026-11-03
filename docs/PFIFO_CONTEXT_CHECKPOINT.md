# Device-context observation: the ring is 512 KB at 0x03B7D000; the title waits for GET=base

November 4, 2026. A bounded diagnostic read the original device context after the diagnostic GET-read run. The context holds the pushbuffer ring base and end, the current push pointer and the expected GET. **The ring is not the 0..PUT span**: it is a 512 KB buffer at `0x03B7D000`..`0x03BFD000`, and PUT is at the base because the buffer is empty. The title waits for GET to become the base; the observed GET is still 0. **No consumer is implemented and there is no playable screen.**

## Method

- `tools/fixtures/thread_startup_probe.c` gained an opt-in `FF_CONTEXT_PROBE` export. It reads the `D3D_g_pDevice` global at `0x00206720`, then exposes that context pointer and 31 following dwords; without the define the source is unchanged.
- Private build `research/verification/recomp-post-memory-build-54` combined `FF_ALLOW_DMA_GET_READ=1` and `FF_CONTEXT_PROBE=1`; module SHA-256 `f7c643b347ded8e96565c503c450f9b8e74e90080012ac2b4622f3a1df651e32`, 526,898 bytes.
- `src/runtime/thread-proof-worker.mjs` now includes `contextWords` when the module exports `ff_probe_context_word`; older modules omit the field.
- The existing worker ran with the retained XBE and `settings-mmio-20.json`; output is ignored at `research/verification/pfifo-setup-01/browser-54.json`.
- Control build `recomp-post-memory-build-55` without either define is byte-identical to the known fail-closed module (SHA-256 `cd0bf176d65407f08bf9409b1fd45d6efafde0c7aa3cc31000d2af49c7ba26bb`, 526,955 bytes).

## Observed context

`D3D_g_pDevice` at `0x00206720` points to `0x00203B90`. The context dwords are:

| Index | Value | Meaning |
| --- | --- | --- |
| 0 | `0x03B7D000` | current push pointer / ring base |
| 1 | `0x03B84DFC` | bounded span end derived from the context size |
| 2 | `0x000007FF` | context field |
| 3 | `0` | context field |
| 4 | `0x03B7D000` | ring base (`param_1[4]`) |
| 5 | `0x03BFD000` | ring end (`param_1[5]`) |
| 6 | `0x03B7D000` | kickoff PUT |
| 7 | `5` | context field |

The ring is `0x80000` bytes (512 KB). PUT is at the base, so the ring is empty. The original wait at `0xFD800044` compares the context's current push pointer (`0x03B7D000`) with `DMA_GET`; the observed GET is 0, so it polls to the limit (status 12).

## Interpretation

- The software context setup writes GET=0, but the original wait expects GET to equal the empty-ring base/PUT (`0x03B7D000`). The hardware or the channel context must supply that value when the channel is enabled; the synthetic register storage does not.
- The earlier full-range scan from 0 scanned the wrong span: GET=0 is a software reset value, not the ring base. The actual ring is the 512 KB buffer at `0x03B7D000`..`0x03BFD000`, which is empty.
- The command source is therefore not a 59 MB stream; it is the empty 512 KB ring. The next step is a bounded empty-ring initialization model: on channel enable with PUT==base, set GET=base/PUT. This is now supported by the observed context, not invented. It still needs a rebuild/run to verify and must not be generalized to consuming unproven data.

## Limits

One diagnostic build and one explicit synthetic settings profile. The observation does not establish the kernel D3D submission path, a populated command stream, a general consumer, rendering or fidelity. The context values are original-runtime observations from this synthetic profile, not a complete original-console state.

## Validation

Private diagnostic run completed; control build reproduces the known module byte-for-byte. Runtime suite 109/109, Python memory-access 10/10, title-seed 25/25, `npm run check` and whitespace checks pass. ABI 24 and D060 are unchanged.

## Next task

Implement a bounded empty-ring initialization for the observed channel profile: when the channel is enabled and PUT==base, serve GET=base/PUT for the original wait. Rebuild the diagnostic module and rerun; if the original code proceeds, follow the next genuine dependency. Do not consume a non-empty ring, generalize the baseline to other addresses or claim rendering without evidence.
