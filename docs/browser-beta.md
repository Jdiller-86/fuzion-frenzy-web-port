# Local browser beta

Run the current browser source and the matching privately built SDK module on the same computer. Use a compatible USA disc image from your local files.

```powershell
python -B tools/serve-sdk-browser.py --open
```

Open the printed loopback URL in desktop Chrome or Edge and select your disc image. Keep using the same origin and port so the browser can find your preferences and saves. See the [browser build guide](sdk-browser.md) for the pinned build inputs and launch options. Original disc data, generated title code, and saves stay on the player's device.

## Play and change settings

- **F1** or the **Settings** button opens the browser settings while playing. Use the original pause screen before changing settings in a round. The host menu owns input while open.
- **Apply** changes the current session. **Apply and Save** also stores the settings for the next visit. **Cancel** or **Escape** discards pending edits.
- **Frame cap** selects 60 or 30 FPS in the SDK's frame pacing. Guest vertical blank stays at 60 Hz. Use 60 FPS for the reference behavior while the lower cap is being checked through full rounds.
- **Display shape** fits the original image, stretches it in a 16:9 area, or fills the window. The optional fullscreen preference takes effect on Start. Toggle fullscreen from Settings or use the browser's F11 shortcut. [Fullscreen API](https://fullscreen.spec.whatwg.org/)
- **Volume** controls the browser's audio output. The original sound and music options remain in the game. **F2** toggles the live performance display.
- The keyboard and first standard gamepad share player one. Connect up to three additional standard gamepads for players two through four. Press a button to let the browser detect a controller, then release the controls to arm it. While Settings is open, the first controller uses the D-pad or left stick and A/B. [Gamepad API](https://w3c.github.io/gamepad/)
- **Stop game** waits for the browser worker to finish its local save flush before enabling Start again. The launcher reports failed storage or an unconfirmed final flush.

Older local Wasm builds can use the presentation, settings storage and audio controls. Their runtime exposes one controller port and the original 60 FPS cap. Rebuild with the current source for the extra ports and live cap.

## Verified local build

The installed private module has SHA-256 `9e8a7c60e579883e87c786e7edb9b94b4c6765c8a1732407140652f25d2e1ccd`. A foreground Chrome replay used the compatible local disc, navigated the original menus and reached Erodeo. Applying the cap produced **59.99 → 29.99 → 59.99 FPS** across sampled windows. The replay checked the live audio gain, Apply and Cancel, saved preferences after reload, four synthetic controller inputs, input neutralization when closing Settings, rejection of incompatible input, and the worker's final save-flush acknowledgement. Chrome returned `not granted` to the fullscreen click in the test session. The fallback kept the game playable in the existing window.

The local recipe, compact summary and screenshots are in `artifacts/browser-gpu/local-beta/`. The replay covers browser input routing with synthetic controllers, and the cap samples measure presented game frames. Physical controllers and full-round simulation at 30 FPS remain beta checks.

## Useful beta reports

Include browser version, OS, GPU, minigame and mode, input device, and the last visible action. For rendering or cadence issues, note the display shape, frame cap, and visible F2 readings. Keep runtime logs, game files, saves and captures with original content local.

The beta covers host controls and local play. Verify individual game modes, physical controllers, reconnects, and multi-round saves as separate beta scenarios. The browser compositor owns display synchronization. Native HDR, render-target scaling, anisotropic settings and the three-texture replacement pack require separate WebGL integration. Browser display shape changes the presentation of the original frame. Gamepad rumble remains unavailable in this build.

The public branch contains source only. Beta execution requires the locally built title module and a matching disc image. This beta does not accept M1 or the broader original-game fidelity milestones.
