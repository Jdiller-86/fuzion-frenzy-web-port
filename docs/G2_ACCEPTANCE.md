# G2 offline Twisted System: implementation and fidelity audit

**User clarification supersedes product acceptance:** this audit covers the approximate research prototype only. The user requires the actual original game entirely inside the browser, or an identical replica, and rejected this prototype as the product. Read `ORIGINAL_EXECUTION_ROUTE.md`.

October 31, 2026. The offline implementation is delivered, published and has been played in the actual browser. **Implementation delivery and original-game fidelity acceptance are separate.** Fidelity remains unaccepted for the unresolved values below. This is one activity, one imported character profile and one supported local content build.

## Run and verify

Use Node 24.14.0 and npm 11.x from the repository root:

```text
npm ci --ignore-scripts
npm run test:formats
npm run test:gameplay
npm run check
npm run build
npm run dev
```

Open `http://127.0.0.1:4174/`. Select your compatible uncompressed USA XISO, wait for local conversion, choose one to four seats and devices, then Play round or Practice. For the built source bundle, stop the development server and use `npm run preview` on the same address. The G1 diagnostic remains separately available through `npm run dev:research` on port 4173. Do not serve the workspace with a generic static server.

| Seat | Default keyboard jump / duck | Gamepad |
| --- | --- | --- |
| P1 | Space / ArrowDown | Assign one discovered standard controller to that seat |
| P2 | W / S | A / bottom button jumps; B / right button holds duck |
| P3 | I / K | A device cannot own two seats |
| P4 | Numpad8 / Numpad5 | Reconnect the same device, release its buttons, then Resume |

Focus a key field in instructions and press a replacement key to remap it. Keys must be disjoint between seats. Four-seat keyboard play is implemented; physical simultaneous-key rollover depends on the keyboard. Menus use keyboard/mouse; controller-only menu navigation is not implemented. Esc pauses. Focus/visibility loss and an owned gamepad disconnect clear inputs and require explicit Resume. A rendering interruption longer than 250 ms pauses rather than processing an unfair catch-up burst. Practice can be ended explicitly. Rematch resets the rules state and selects a fresh seed while retaining owned devices and imported assets.

## Requirement audit

| Requirement | Current evidence | Result |
| --- | --- | --- |
| Reference controls, hazards, progression, scoring, elimination, ties and lifecycle | [TS-R1 reference](TWISTED_SYSTEM_REFERENCE.md), timestamped primary footage, secondary seven-miss/practice lead, local executable/package identity and resource call sites | Observations, inferences and unknown values are distinguished. Original ties and exact timing remain unresolved. |
| Freeze scenarios and tolerances before affected behavior | TS01–TS13 and exact TS-R1 defaults recorded before simulation implementation | Frozen reconstruction policy. Original one-frame/5% fidelity tolerances have not been passed. |
| Independently written shared simulation, renderer and input separation | `packages/simulation/twisted-system.mjs`, `packages/input/`, `apps/web/` | Headless semantic-command rules; no DOM, renderer, transport or clock dependency in simulation. |
| Complete one-to-four seat loop | Actual browser solo, two-seat and four-seat rounds, practice, resolution/results and rematches below | Browser-playable implementation verified. |
| Original local assets and compatible content gate | Worker reuses existing XDVDFS/XBP/OBE/texture/weighted-animation readers; hashes executable and selected package before enabling play | Imported reactor/rods/Dub, not a placeholder arena. One uncompressed XISO profile supported. |
| Local-only conversion and clearing | Worker receives selected File, reads bounded slices, emits decoded assets only to its own page; no upload/persistence API; server/build allowlists; malformed rejection and clear/reimport browser runs | No original content served, uploaded, staged or built. Clear terminates worker and releases GPU/decoded state without deleting originals. |
| Meaningful tests | 23 gameplay/input/event-wiring/server/build tests; existing format tests extended for multiframe palette alignment and bounded large-package hashing | Passed locally. This does not establish physical-controller behavior or original fidelity. |
| Source-only checkpoint and Windows/Linux CI | Workflow now executes gameplay tests and the real source-only build in both OS jobs | Published implementation `d2af6a2`; [run 41236582956](https://github.com/Jdiller-86/fuzion-frenzy-web-port/actions/runs/41236582956) passed on both Windows and Linux, including gameplay tests and the source-only build. |
| Preserve supplied content | All 136 file identities reproduce `reference-files.json` byte-for-byte; XISO SHA-256 remains `8662b9ccdf2507e6d1edd3fa8ec3003cc7b8f016e0f55518aa700200db104618` | Verified after implementation/browser work. No extraction repeated. |
| Status, handoff, decisions and reproducibility | This audit, current status/handoff, D013–D015, README and actual scripts | Updated; older investigation history explicitly superseded. |

## Actual browser runs

Windows workstation and ChatGPT in-app Chromium browser, using the existing local XISO. Synthetic tests ran on Node 24.14.0. The renderer uses WebGL 2. These observations do not establish Chrome/Edge/Firefox/Safari release coverage, physical gamepad QA, four-person keyboard rollover, integrated-GPU performance or continuous peak memory.

| Run | Observed result |
| --- | --- |
| Local import | Original reactor, low/high rods, universe/world additions and animated Dub loaded. First measured import 0.15 s; decoded texture allocation about 5.2 MiB. Warm/cache state uncontrolled; this is not peak memory or a performance sign-off. |
| Solo no-input | Countdown -> active -> resolving -> results; zero clearances, seven misses. A later wording correction labels this Solo run complete rather than implying victory after a solo elimination. |
| Timed actual key input | A Space press with the low rod about 0.2 s away produced score 1, zero misses. Holding ArrowDown through the next high rod produced score 2, zero misses. Inputs used the real browser keyboard path, not direct simulation calls. |
| Four seats | Four separate colored markers and imported characters rendered across the original spiral. Early jump presses were accepted through each seat's keys. Subsequent no-input elimination produced P1+P2+P3+P4 shared win, each rank 1, score 0, misses 7. |
| Fresh rematch | Same four seats returned to countdown, all scores/misses reset to zero. |
| Two seats | P1 held duck; P2 supplied no input. P2 reached seven misses; P1 won last-standing with three clearances/four misses. |
| Practice | With two seats, P2 was out at seven misses while P1 continued at six clearances/two misses, active time 11.8 s. End practice returned unranked final standings. A later no-input practice rematch ended when both were eliminated. |
| Pause/resume | Pause overlay froze the four-seat round at active time 5.9 s with four misses each. Explicit Resume advanced the same round, rather than restarting it. |
| Focus/device clearing | Injectable event wiring tests dispatch blur, hidden/visible changes and owned/unowned disconnect notifications, proving pending jumps/held ducks clear for all seats and no automatic resume. Actual physical OS-focus loss/controller disconnect has not been independently exercised in this browser session. Opening a second background test tab did not establish focus loss and is not counted as that test. |
| Built-bundle rerun | `npm run preview` served the 17-file allowlisted build. Import took 0.11 s. Duplicate Space ownership for P2 was rejected; remapping to Digit2 and back worked. A W press showed only P2 Jumping, with P1/P3/P4 Running, while the imported P2 model was visibly airborne. Refresh gamepads showed no discovered controller, so no physical gamepad run is claimed. |
| Malformed and clear/reimport | `invalid-input.iso` rejected with XDVDFS range outside image and no Play control. Supported image then imported. Clear removed content/controls; reimport rendered four seats with the same canvas/GPU context and no browser warning/error entries. |

The final built four-seat rerun also reached shared-win results with four frozen zero scores/seven misses, returned to instructions with ownership retained, and reported no browser warning/error entries. `npm ci --ignore-scripts --offline`, the runtime check and `git diff --check` passed locally.

Screenshots containing imported originals remain ignored/local under `research/verification/`; none is included in the published source or build. Built four-seat screenshot: `research/verification/g2-built-four-seat-20261031.jpg`.

## Reference comparison and remaining fidelity gaps

The primary [Twisted System recording](https://www.youtube.com/watch?v=Oi5jWuJHZUo) shows A Jump/B Duck at 0:07, the spiral and four seats at 0:09–0:22, blue low/orange high hazards at 0:37, scores/eliminations at 1:27 and the winner at 1:32. The browser renders the matching locally imported spiral, obstacle silhouettes and Dub identity. This is a recognizable visual comparison, not a recovered original camera or measured collision-volume match.

The no-miss simulation fixture samples 10/20/30/40/50/60 active seconds against approximately 8/16/27/48/70/92 observed clearances. The measured reconstruction scores were 8/16/27/49/71/93 with zero misses, differences 0/0/0/+1/+1/+1. It passes TS12's frozen +/-2 through 30 s and +/-4 afterward. It is an approximate count envelope, not a measurement of the original acceleration function. No original-frame action-boundary comparison is claimed.

Material deviations remain explicit in the game and reference: seeded rod order differs from unverified original RNG; seven-miss threshold and practice semantics are secondary inferences; jump/duck windows, recovery and knockback numbers are configurable approximations; solo and simultaneous-tie policies are independent reconstruction choices. Track anchor, seat placement, hazard vertical offsets and camera are presentation approximations. The original mesh geometry is retained, but timed clearance rules are independently written and are not derived collision volumes. Run/jump/fall/victory/defeat clip ranges use imported keys with provisional playback integration. Duck compresses the pose because a dedicated original clip has not been established. The scene lacks original camera choreography, effects and audio; no soundtrack or cue equivalence is claimed. One Dub model is reused with distinct colored seat rings as permitted by the prototype scope.

The reactor texture's field at 0x37 is a frame count, not a mip count: native initializer `0x00041890` creates a 24-byte header per frame, aligns the header area, uses a shared indexed palette and aligned per-frame payloads. The selected `drillpow` resource has 21 frames. The browser currently uses frame zero/base mips; texture animation cadence remains unimplemented. Hash-zero universe batches use an explicit untextured diffuse mode; original material semantics remain an inference. Local static evidence: ignored `twisted-texture-loader.txt` and `twisted-texture-upload.txt`, from the existing Ghidra project, not REA Evidence or executed original behavior.

**Next bounded fidelity task:** compare the two rod contact poses and jump/duck window endpoints with additional primary footage that exposes inputs or repeated misses; resolve original hit limits/practice/ties through targeted static evidence or a suitable public run. Physical gamepad/focus-loss checks and broader browser coverage remain explicit QA tasks. None requires acquiring firmware or asking the user for a capture setup. Do not start online systems or another activity from this implementation checkpoint.
