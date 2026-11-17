#!/usr/bin/env python3
"""Package the compiled browser build without any original game input/assets."""
import argparse
import hashlib
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / 'artifacts/browser/FuzionFrenzyBrowser.zip')
    args = parser.parse_args()
    required = [
        'web/sdk-browser/index.html', 'web/sdk-browser/app.mjs', 'web/sdk-browser/worker.mjs',
        'web/sdk-browser/presentation.mjs', 'web/sdk-browser/webgl-renderer.mjs', 'web/sdk-browser/pixel-glsl.mjs',
        'web/sdk-browser/vertex-glsl.mjs',
        'host/xdvdfs.mjs', 'tools/serve-sdk-browser.py',
        '.tools/artifacts/sdk-browser-full/fuzion-sdk-full.mjs',
        '.tools/artifacts/sdk-browser-full/fuzion-sdk-full.wasm',
        '.tools/artifacts/sdk-browser-full/full-build-manifest.json',
        'runtime/browser/sdk/SDK-NOTICE.txt',
    ]
    for name in required:
        path = ROOT / name
        if not path.is_file() or path.is_symlink():
            raise FileNotFoundError(f'Required build output missing: {name}')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if (ROOT / '.tools/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js').is_file():
        required.append('.tools/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js')
    with zipfile.ZipFile(args.output, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=6) as archive:
        for name in required:
            archive.write(ROOT / name, 'FuzionFrenzyBrowser/' + name)
        archive.writestr('FuzionFrenzyBrowser/Start Browser.cmd', '@echo off\r\ncd /d "%~dp0"\r\npy -3.12 tools\\serve-sdk-browser.py --open %*\r\npause\r\n')
        archive.writestr('FuzionFrenzyBrowser/README.txt', '''Fuzion Frenzy browser build

Extract the ZIP completely, then double-click Start Browser.cmd on Windows.
It uses Python 3.12, already installed by the Windows game's build-tools setup.
On macOS/Linux: python3 tools/serve-sdk-browser.py --open
Use the URL opened by the launcher in a current 64-bit Chrome or Edge.
The host tries port 4182, then 8765, then an available OS-selected port if
Windows reserves or another app occupies the preferred ports.
To keep a fixed alternate port: Start Browser.cmd --port 8765
Browser saves belong to the URL/port used; changing ports does not move saves.
Select your compatible USA Fuzion Frenzy ISO/XISO, then Start game.
Do not open the HTML directly: browser threads require the local host's headers.

The ISO stays local and is not uploaded. Only DEFAULT.XBE is read and hashed
for compatibility before the runtime opens your local disc image. This check
does not establish legal ownership. The package includes compiled title code
from the source supplied to the project, SDK runtime and reference renderer;
it includes no ISO or original assets. No compiler is needed to use this build.

Controls: Enter Start, Space A, Escape B, X/Y face buttons, WASD left stick,
arrows D-pad, Z/C triggers. Standard gamepads are supported on the first seat.
Save files stay in this browser's local storage; use Stop game to flush saves.

GPU (WebGL2) is the default renderer; Software reference is selectable before
starting. Programmable and fixed-function vertices, lighting, triangles and pixel
combiners execute on the browser GPU; vertex assembly and texture decoding remain
on the CPU. Native WebAssembly exceptions, threaded render preparation and
pipelined submission are enabled in this build. Development fallbacks:
?threadedGpu=0&pipelinedGpu=0 (serial), ?pipelinedGpu=0 (synchronous submit).
Observed scenes approach 60 FPS on the development desktop; this is not a locked
60 FPS guarantee for all minigames or computers. The game fills the window
at its original aspect ratio. Controls opens fullscreen and Stop game actions.
This remains an experimental browser port with incomplete rendering coverage.
Compilation/linking alone does not prove original-game execution or fidelity.
The build manifest records the SDK revision and published rendering patch.
The shared texture-cascade correction is included; D3D12 descriptor heaps are
not used by WebGL. Keep runtime diagnostics
private if they contain player information. Nothing sends logs automatically.
''')
    checksum = hashlib.sha256(args.output.read_bytes()).hexdigest()
    args.output.with_name('SHA256SUMS.txt').write_text(f'{checksum}  {args.output.name}\n')
    print(f'Packaged {args.output} ({args.output.stat().st_size} bytes)')


if __name__ == '__main__':
    main()
