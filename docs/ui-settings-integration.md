# Settings and host overlay integration

The native F1 implementation, observed behavior and current browser adapter seam
are recorded in [the native UI handoff](native-ui.md). The original-screen
injection and contextual Y hint below remain proposed; the initial host panel,
four controls, actions and input gate are now implemented.

This note records settings and overlay interfaces from the retained XboxXBERecomp SDK pin `58757ab…` and Fuzion native integration pin `071db…`, used by the November 10 Windows rendering work. The original-executable UI evidence is indexed against publication commit `93ec83293921dfcf1ff4b8f0c759aa62189c330b`. The SDK facts below come from `runtime/app/app.h`, `runtime/app/settings.cpp`, `runtime/app/main.cpp`, `runtime/app/main_sdl.cpp`, `runtime/gpu/d3d12.cpp`, and retained Fuzion host integration in `fuzion-native/src/common/overlay.cpp` and `game_info.cpp`. Source trees and game inputs are private research; this note contains interface findings only.

## Existing SDK settings contract

`app::Settings` in `runtime/app/app.h` owns renderer and display preferences. `settings.cpp` loads and saves TOML at `ExeDir() / kGame.settings`; the Fuzion integration names that file `settings.toml`. Missing or malformed settings leave defaults in place. The generic keys read and written by `Load()` and `Table()` are:

| TOML key | Meaning / application |
| --- | --- |
| `display_mode` | `fullscreen`, `borderless`, or `windowed`; display reconfiguration goes through `app::ApplyDisplay()` when mode or dimensions change. |
| `width`, `height` | Zero means desktop dimensions; otherwise the selected output mode/window size. |
| `render_percent` | Render target scale as a percentage of output height, clamped by the native F1 control to at least 1%. |
| `fps_cap` | Presented-frame cap; zero means unlimited. |
| `vsync` | Swap interval preference. |
| `anisotropic_filtering` | Zero means the title's sampler choice; forced anisotropy is only used when the title already samples across mip levels. |
| `aspect_ratio` | `auto`, `4:3`, `16:9`, `16:10`, `21:9`, or `32:9`. |
| `ultrawide` | Enables the SDK's host-side horizontal widening behavior when applicable. This is a renderer behavior with title-specific caveats, not a universal fidelity guarantee. |
| `join_ip` | Optional system-link host address. |
| `hdr`, `hdr_paper_white`, `hdr_peak` | HDR mode and brightness preferences. Runtime status is separately reported by the renderer. |

Title-specific simple booleans/integers can be registered through `GameInfo.options`. Other TOML values can be read and written by `GameInfo.load` and `GameInfo.save`. These callbacks are the supported persistence extension point; avoid a second unrelated settings file.

The SDK's `GameInfo.ui(double fps)` hook is called while the ImGui frame is active, with `UiLock()` held. The Windows host feeds window messages to `ImGui_ImplWin32_WndProcHandler`; the SDL3 host feeds SDL events to `ImGui_ImplSDL3_ProcessEvent`. F1/F2 are intentionally left to the game UI callback. The Windows path and SDL3 path both initialize ImGui keyboard/gamepad navigation. Display updates are platform-specific: Windows posts a window-thread apply message; SDL3 posts an event and updates its cached drawable size. The UI should call the SDK apply hook only for settings that need a window/display change.

The retained native Fuzion integration registers `.ui = &overlay::Draw`. Its F1 settings and F2 performance panels start closed. F1 opening focuses the settings panel; the native keyboard/mouse adapter observes the panel-open state so game keyboard input does not receive overlay navigation. The settings UI stages edits in a draft, applies them, and calls `app::Save()` on its save action. It also reports which settings apply live and which cause display reconfiguration. This is the existing host-side UX precedent, not evidence that the title itself owns an F1 panel or that this UI is part of the original game.

## Original guest Options screen: local runtime observation

The coordinator reports a local runtime capture of the original guest Options screen at `artifacts/ui-inventory/captures/options-list.png`. The observed list has six rows: Sound Volume, Music Volume, Vibration (`on`), Soundtrack (`Fuzion Frenzy!`), Song Sample (`Fuzion Frenzy`), and Credits. Sound and music use ascending segmented ten-bar meters; the footer shows a gray D-pad icon for changing a value and a red B icon for going back. The observed route is `mainmenuOptions` to this six-row Options screen.

This is evidence for those visible guest options and their displayed controls in the captured local run. It does not establish persistence semantics, internal values, step size for volume, title-wide settings coverage, or console/browser equivalence. Keep those guest-owned options in the original menu path. The host overlay can expose browser-only display, quality, and accessibility preferences separately; it should not replace the original Options screen.

## Browser candidate boundary

The current development branch includes the compiled browser renderer/input/audio pipeline from `797e9ae`, local ISO intake, frame presentation in `web/sdk-browser/app.mjs`, and worker-side input/audio/runtime bridges. This supersedes the older inventory snapshot. This native batch does not add a browser settings store, host overlay, or replacement upload adapter; compiled browser capability is separate from observed native UI behavior.

For a browser integration, retain the useful separation in the SDK:

1. Keep persistent user preferences in a versioned, origin-local store (IndexedDB is a practical proposal where the settings object grows; localStorage can hold small scalar preferences). Do not send settings or content to a service.
2. Load validated defaults before starting the worker. Pass an explicit settings snapshot in the worker's start message; do not assume the worker can access `window` storage.
3. Keep display/render settings separate from per-seat input bindings and game state. Apply only settings the connected browser renderer actually supports; show unavailable platform options as unavailable rather than persisting inert toggles.
4. In the existing frame presentation flow, expose an HTML host panel toggled by an explicit keyboard shortcut, with focus transfer and keyboard/gamepad capture rules. Browser fullscreen requires a user gesture; display mode/resolution switching and native HDR mode are not portable browser controls.
5. Save after explicit apply or a debounced change, and version/migrate the schema. Do not claim the native TOML keys are already implemented in the browser.

The original game UI and the host settings UI should remain separate layers. Original title screens, menus, and controls must come from the original guest execution. A browser host panel can provide accessibility, browser-specific settings, and local diagnostics around that execution, but must not replace or imitate title screens as a shortcut to original-game execution. The first integration target is an unobtrusive host overlay around an actual guest-rendered frame; browser behavior for this panel remains unimplemented and unobserved.

## Original screen versus integrated host overlay

The observed original Options screen is a six-row guest mode reached through `mainmenuOptions`. Source analysis identifies constructors for MainMenu (`sub_0001C2A0`, vtable `0x255400`) and MainOptions (`sub_0001CD50`, vtable `0x2554DC`); their guest paths load `over_main.obe` and `over_option` through shared loader `0x4BB30`. This proves concrete guest mode/resource entry points, not the complete mode registry, update/input/draw dispatch, transition cleanup, or a safe injected-mode contract. Those shared lifecycle contracts remain untraced, making guest-mode injection the higher-uncertainty route.

The retained host integration already has a distinct F1 settings panel, `GameInfo.ui` → `overlay::Draw`, UI input capture, staged apply/save, and browser-independent SDK controls. For browser work, the lower-risk first integration is an explicitly entered host overlay around the actual guest frame, reusing only original-styled components whose geometry and texture regions are joined to evidence in the private asset manifest. Do not claim the host can mutate or take over an original guest menu row. Keep title-owned Sound Volume, Music Volume, Vibration, Soundtrack, Song Sample, and Credits in the original Options route unless an actual guest settings/apply path is traced. A host option that duplicates one of these needs a demonstrated connection to the same guest state and effects.

The host overlay must have its own focus boundary and dismissal behavior; current native keyboard/mouse capture is evidence for the host design, not proof of browser or original-game input semantics. The SDK initializes gamepad navigation, but guest physical-pad gating is unresolved: retained `kbm::Apply` is a keyboard path. Verify which player input reaches the guest before claiming gamepad capture or navigation parity. F1 is an explicit host entry point in the native integration; it is not evidence that F1 opens a title-owned or browser panel today. Browser keyboard focus, controller mapping, and apply/save effects need implementation and verification in the browser path.

## Practical code touchpoints

The recommended first implementation is the **native integrated host overlay**,
using the existing F1 entry and draft/apply/save behavior. Keep the original guest
Options screen available. The proposed entry from that screen is an additional
footer hint, **Y: PC settings** (F1 on keyboard), rather than changing an original
volume row. This hint and interception do not exist today. A bounded observer of
the Options presentation path (`0x1DD30`, mode vtable slot 7) must first establish
when that screen is active. Its state method (`0x1D790`, slot 13) is a useful
investigation anchor, not an admitted host callback. A shared flag should be
published through owned synchronized state; never retain a guest object pointer
beyond its validated lifetime. Until this observer is proved, F1 is the concrete
existing entry. The same proposed hint can later be exposed from pause Options;
the pause mode's source identity is still unresolved.

| Native file / function | Separately scoped modification and observable effect |
| --- | --- |
| `fuzion-native/src/common/overlay.cpp`: `overlay::Draw`, internal `Apply`, draft state | Draw the component manifest's chrome/list/value styles. Keep one focus index, up/down navigation, left/right value changes, A/Enter accept, B/Escape back, and explicit Apply/Save/Cancel. These mappings are proposed host behavior. |
| `fuzion-native/src/common/game_info.cpp`: `GameInfo.ui`, `PadHook` | Retain the existing UI callback and add physical-pad capture while the panel is open. Do not let A/B or held stick input affect the guest behind it; clear held transitions on dismissal. |
| `fuzion-native/src/common/input_kbm.cpp`: `kbm::Apply`, message/focus handling | Reuse keyboard capture; verify Escape and F1 dismiss without leaking a queued guest action. |
| Proposed `src/common/ui_asset_store.{h,cpp}` plus the existing GPU extension owner | Own copies of admitted decoded UI pixels and host resources. Match canonical decoded pixels plus descriptor identity, not transient guest addresses; byte order and palette canonicalization need a small adapter. The current `gpu::Extension::Decoded` callback can observe/copy pixels, but cannot replace its const buffer. Compose with any existing extension rather than overwriting `gpu::g_extension`. |
| SDK `runtime/app/settings.cpp`: `app::Get`, `app::Save`; `app::ApplyDisplay` | Use the existing settings object/TOML. Do not add a second native preference store. |

Create owned UI resources when the asset provider has a verified binding; release
them through the renderer's fence-aware lifetime after the panel closes or the
device resets. Do not borrow a transient descriptor-ring slot for a persistent
ImGui texture. Keep the published descriptor-rollover repair intact. This adapter,
resource-to-ImGui binding, exact font metrics, and the menu-entry observer are
dependencies of a faithful integrated overlay, not capabilities established here.

For a small first batch, expose `vsync`, `fps_cap`, `render_percent`, and
`aspect_ratio`. The existing native `Apply` writes the draft to `app::Get()`;
the renderer/frame loop consumes these preferences during presentation. Save
persists them through `app::Save()`. Window `display_mode`/`width`/`height` changes
also use `app::ApplyDisplay`; borderless sizing has its own existing restrictions.
HDR is Windows/D3D12/display dependent and the backend manages its swap-chain
change; do not route it through unnecessary window reconfiguration. Treat native
system-link `join_ip` as native-only until an actual browser networking adapter
exists. Guest volume/vibration and save-game preferences remain separate from
these host renderer settings.

Use the manifest's logical components and a shared value/focus model for native
and browser presentation. Native persistence is the existing TOML; a browser
adapter would validate capabilities and persist a versioned local snapshot.
Native fullscreen/HDR/system-link behavior must not be offered as working browser
options. The browser pipeline has no settings/replacement adapter from this batch;
capability-specific application and persistence remain dependencies.

For the current compiled browser pipeline, the public integration points are `web/sdk-browser/app.mjs` for panel lifecycle and validated settings messages, `web/sdk-browser/worker.mjs` for worker-side application, and the runtime renderer/host bridge for supported live settings. Add persistence in a small browser module rather than coupling it to title execution. Reuse existing SDK option/load/save semantics only if that application code is part of the browser build; the current browser candidate does not link the SDK desktop app layer. An eventual integration should be explicit about lifecycle: host panel open/close and focus ownership, settings validation/persistence, and effects in the renderer/guest bridge are separate responsibilities.

Do not implement a direct clone of `settings.cpp`'s desktop-only controls. Start with browser-backed choices such as render scale, frame pacing preference (if the browser loop can honor it), and keyboard/gamepad bindings. Gate each choice on a real application path and preserve the SDK's notion that a setting may need a restart/rebuild rather than pretending every change applies immediately.
