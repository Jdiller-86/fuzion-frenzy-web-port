# Original execution reaches the NV2A USER region and waits on pushbuffer progress

November 3, 2026. The explicit aperture window ceiling was raised from 8 MiB to the 16 MiB NV2A MMIO span, and the profile now supplies a bounded USER page at `0xFD800000`. Execution writes `0xFD800040`, enters `0x001FACA0`, and then compares the context value with `0xFD800044` in a loop, waiting for the device to consume the pushbuffer. The existing 16-read poll limit stops the run with explicit evidence instead of fabricating device progress. **There is no playable original screen and no renderer/device model.**

## Selection and run

| Item | Value |
| --- | --- |
| Translation | `research/verification/recomp-title-post-memory-30` (local/ignored) |
| Functions / instructions | 366 / 14,572 |
| Module | `research/verification/recomp-post-memory-build-47/startup.wasm` (local/ignored) |
| Module bytes | 524,891 |
| Module SHA-256 | `efb0fbea1c67c44c3a904b0da9294120cdf3f3439909e639e3f0866b7b76e27e` |
| Original executable SHA-256 | `45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9` |
| Report | `research/verification/post-memory-browser-20261103-30.json` (local/ignored) |
| Synthetic profile | `research/verification/memory-statistics-inputs-01/settings-mmio-20.json` (local/ignored) |

The module is unchanged from the previous milestone; only the explicit profile window and the parser ceiling changed. ABI 24, the sole `env.ff_host_clock` import, the 8,192-entry / 32,768-event aperture, the mirror page, the 16-read poll limit and the single self-clearing flush bit are unchanged.

## Source-owned changes

- `src/runtime/local-title-proof.mjs`: the explicit synthetic aperture window may now be at most `0x1000000` bytes (the NV2A MMIO span, `0xFD000000`–`0xFE000000`) instead of `0x800000`. Entries, zero ranges and 4-byte alignment checks are unchanged and still bound every address inside the selected window.
- `tests/mmio-profile.test.mjs`: accepts a 16 MiB window with an entry at `0xFD800040` and a zero range at `0xFD800000`, and rejects a window larger than `0x1000000`.
- Private profile `settings-mmio-20.json`: window `0xFD000000` / `0x1000000`, the same 64 explicit registers and four zero ranges (`0xFD100200`, `0xFD400000`, `0xFD003200`, and the new USER page `0xFD800000` / `0x1000`).

## Observed browser execution

- All **83/83** initialization callbacks returned.
- **262** distinct original routines executed and **402** kernel operations completed; `0x001FACA0` is newly reached.
- Execution wrote `0xFD800040` and then looped in `0x001FBB00` at `0x1FBECD`: it calls `0x001FACA0`, reads `0xFD800044` (the bounded USER page supplies zero), compares it with the context value under mask `0x0FFFFFFF`, and repeats while they differ.
- After **16** identical reads of `0xFD800044` the bounded poll limit stopped the run.
- Device aperture: **5,302 entries** / **12,542 events**; memory statistics remained `[36, 16384, 14436, 552960, 1585152, 0, 1008, 16, 789]`.
- Stop: status 12 (`device poll limit reached`), target `0xFD800044`.

## Next dependency

The loop waits for the NV2A USER-region get/put comparison to match, i.e. for the device to consume the queued pushbuffer work. That requires real device progress (pushbuffer/FIFO/DMA), not a single self-clearing bit. Next: analyze `0x001FACA0` and the `0xFD800040`/`0xFD800044` pair from static and pinned-emulator evidence, then decide whether a bounded pushbuffer-consumption model is justified. Do not fabricate the awaited value, interrupt delivery, DMA or rendering.

## Validation

`tests/mmio-profile.test.mjs` passes with the new 16 MiB window cases. `npm run test:runtime`, Python memory-access 10/10 and title-seed 25/25 pass, and `git diff --check` is clean. The report saved as `post-memory-browser-20261103-30.json` (4,003,727 bytes). The C fixtures are unchanged from the previous commit.

## Prior milestone: NV_PFB_WBC flush bit

Module `recomp-post-memory-build-47` completed the `NV_PFB_WBC_FLUSH` wait after one poll and reached `0xFD800040`, which was then outside the 8 MiB window. Report `post-memory-browser-20261103-29.json`.

## Limits and next task

No renderer, audio, controller, disc read, playable screen or complete runtime is established. The mirror page, zero ranges, event capacity, poll limit, flush bit, USER page, time conversion, port-output log and GPU instance claim are synthetic proof scaffolding, not recovered console state. Next: analyze the pushbuffer get/put wait and choose an evidence-based bounded step.
