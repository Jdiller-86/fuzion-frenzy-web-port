#!/usr/bin/env bash
# Cross-package the same Windows launcher/NSIS installer when Windows CI is queued.
set -euo pipefail
source_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.." && pwd)"
output_root="${1:-$source_root/artifacts/windows}"
mkdir -p -- "$output_root"
output_root="$(cd -- "$output_root" && pwd)"
dotnet_tool="${DOTNET_BIN:-dotnet}"
nsis_tool="${MAKENSIS_BIN:-makensis}"
"$dotnet_tool" publish "$source_root/desktop/windows/Launcher/FuzionFrenzyBuilder.csproj" \
  -c Release -r win-x64 --self-contained true \
  -p:PublishSingleFile=true -p:IncludeNativeLibrariesForSelfExtract=true \
  -p:DebugType=None -p:DebugSymbols=false -o "$output_root/launcher" --nologo
"$nsis_tool" -NOCONFIG -V2 \
  "-DSOURCE_ROOT=$source_root" "-DPUBLISH_DIR=$output_root/launcher" \
  "-DOUTPUT_FILE=$output_root/FuzionFrenzySetup.exe" "$source_root/desktop/windows/installer.nsi"
(cd -- "$output_root" && sha256sum FuzionFrenzySetup.exe > SHA256SUMS.txt)
printf 'Packaged Windows installer: %s\n' "$output_root/FuzionFrenzySetup.exe"
