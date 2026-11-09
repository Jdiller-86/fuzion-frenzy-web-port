# Development guide

The public clone contains an offline research app, format readers, runtime components, and source-owned fixtures. It does not contain original game files, generated title modules, private toolchains, or a playable original game. See [current status](status.md) before interpreting a check as runtime progress.

## Requirements and setup

Use **Node.js 24.14.0** from `.node-version` and **npm 11.x**, with package-manager pin 11.9.0. From the repository root:

```sh
node --version
npm --version
npm ci --ignore-scripts
npm run check
npm run check:environment
```

The current workspace has no application dependencies. Installation does not download media, optional analysis tools, or compiler/source bundles. The environment check reports conventional path existence and installs nothing.

Use `npm run dev:status` to read the recorded checkpoint and next task, and `npm run dev:preflight` to distinguish a source-only checkout from a hash-verified private baseline. The [iteration workflow](development-workflow.md) documents manifest creation, selected check tasks and local timing records. The [local agent handoff](local-agent-handoff.md) contains separately bounded goals for the workstation with retained game/research inputs.

Git is required by the publication check. The Node-based format, runtime-policy, prototype, and build checks use public source only. `test:native` additionally requires gcc (or the runner's WSL gcc path on Windows) and Python 3 for the generated owned-memory fixture; `test:evidence` requires Python 3. The code-generation evidence suite clearly skips its compiler-dependent cases if no C compiler is available; a missing interpreter/native compiler is not a passing native run.

For local Codex sessions, [agent setup](agents.md) covers the saved Sol High coordinator and two-worker Luna defaults, client/model availability, project trust and a read-only execution check. These defaults do not supply the private inputs needed for original-startup work.

## Run the offline research app

```sh
npm run dev
```

Open <http://127.0.0.1:4174/>. Select a compatible uncompressed USA XISO locally, choose one to four seats, and use **Play round** or **Practice**. The retained Twisted System prototype converts limited original assets and uses independently authored rules. Selecting an image does not launch the original game. Without compatible files, the shell and source checks remain available.

| Seat | Jump | Held duck |
| --- | --- | --- |
| P1 | Space | ArrowDown |
| P2 | W | S |
| P3 | I | K |
| P4 | Numpad8 | Numpad5 |

Remap keys or assign a discovered standard gamepad per seat in the instructions screen. Gamepad A/bottom jumps; B/right ducks. Esc pauses. Focus loss, a hidden tab, or an owned-controller disconnect clears input and requires Resume. Physical keyboard rollover can limit shared-keyboard combinations; four software seats are not a hardware guarantee. Menus use keyboard/mouse.

Cancel/clear discards the current imported session; there is no persistent asset cache. See [content handling](content.md) and the [prototype evidence](research.md#offline-prototype-evidence) for fidelity and supported-input limits.

## Build and preview

```sh
npm run build
npm run preview
```

The build emits allowlisted source-only `dist/`; preview serves it on port 4174. It does not recompile or bundle the original title. Stop the development server before previewing on the same port. Keep game files and private tools outside served paths; never use a generic workspace file server.

## Separate research surfaces

| Command | Default address | Purpose |
| --- | --- | --- |
| `npm run dev:research` | <http://127.0.0.1:4173/> | Limited browser-local asset conversion/preview and disc-session proof |
| `node tools/serve-startup-proof.mjs` | <http://127.0.0.1:4180/> | Source-only bounded startup harness; compatible private local inputs required for a title run |

The startup host is separate from the conversion preview. A proof page alone is not a ready title module. [Analysis setup](analysis.md) describes tool/input prerequisites; [the research index](research.md) links recorded scenarios.

## Project tracking

`npm run project:docs` regenerates the five milestone definitions from reviewed scope. `npm run test:project` checks tracking/acceptance behavior without network access. [Management](management.md) covers native issues and workflow reports.

## Checks and their limits

| Command | Verifies | Limit |
| --- | --- | --- |
| `npm run check` | Preparation/publication, milestone scope, and maintained-documentation links | Does not prove gameplay; explicit file review remains necessary |
| `npm run check:environment` | Runtime and optional tool readiness | Does not install tools or establish title compatibility |
| `npm run test:formats` | Source-owned format/conversion fixtures | Not complete format/build support |
| `npm run test:gameplay` | Prototype rules, input, focus/device wiring, and delivery boundaries | Not original-game fidelity |
| `npm run test:runtime` | Browser proof delivery, admission, profile/report and resource-policy fixtures | Does not execute the original title |
| `npm run test:native` | Owned C component fixtures in optimized/sanitizer modes | Component coverage, not original hardware behavior |
| `npm run test:evidence` | Python memory/codegen/seed evidence suites | Original input/run artifacts are not supplied |
| `git diff --check` | Whitespace errors | Not semantic review |

Choose checks appropriate to the changed contract. There is no `npm test` script. [CI](../.github/workflows/ci.yml) runs preparation, formats, evidence, runtime, gameplay, build, and additional native component checks on its Windows/Linux matrix; inspect the workflow for OS-specific steps. A green fixture/build result does not accept emulation, fidelity, multiplayer, or product performance.

## Common setup problems

- **Runtime/engine mismatch:** use the version in `.node-version` and npm 11.x; do not remove the engine guard.
- **Port already in use:** stop the other app/proof process. Development and built preview share 4174.
- **Unsupported/malformed content:** use only the documented compatible research input; ZIP/7z, arbitrary regions, modified images, and loose folders are not promised by the browser UI.
- **Missing compiler/private inputs:** run the unaffected public checks and report the missing prerequisite. Do not download game/firmware or count a skipped title proof as a pass.

See [CONTRIBUTING.md](../CONTRIBUTING.md) for review/staging and [architecture](architecture.md) for the implemented boundaries.
