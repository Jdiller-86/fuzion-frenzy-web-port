[CmdletBinding()]
param([string]$OutputDirectory = (Join-Path $PSScriptRoot '..\..\artifacts\windows'))
$ErrorActionPreference = 'Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
# Reject syntax errors in shipped Windows PowerShell 5.1 scripts before packaging.
foreach ($script in Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'scripts') -Filter '*.ps1') {
    $tokens = $null; $parseErrors = $null
    $null = [Management.Automation.Language.Parser]::ParseFile($script.FullName, [ref]$tokens, [ref]$parseErrors)
    if ($parseErrors.Count) { throw ("Bundled script syntax error in {0}: {1}" -f $script.Name, ($parseErrors -join '; ')) }
}
$out = [IO.Path]::GetFullPath($OutputDirectory)
New-Item -ItemType Directory -Force -Path $out | Out-Null
$publish = Join-Path $out 'launcher'
& dotnet publish (Join-Path $PSScriptRoot 'Launcher\FuzionFrenzyBuilder.csproj') -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true -p:IncludeNativeLibrariesForSelfExtract=true -p:DebugType=None -p:DebugSymbols=false -o $publish
if ($LASTEXITCODE -ne 0) { throw 'Windows launcher compilation failed.' }
$nsis = Get-Command makensis.exe -ErrorAction SilentlyContinue
if ($nsis) { $compiler = $nsis.Source }
else { $compiler = Join-Path ${env:ProgramFiles(x86)} 'NSIS\makensis.exe' }
if (-not (Test-Path -LiteralPath $compiler)) { throw 'NSIS is required to package the installer.' }
$installer = Join-Path $out 'FuzionFrenzySetup.exe'
& $compiler "/DSOURCE_ROOT=$root" "/DPUBLISH_DIR=$publish" "/DOUTPUT_FILE=$installer" (Join-Path $PSScriptRoot 'installer.nsi')
if ($LASTEXITCODE -ne 0) { throw 'NSIS installer compilation failed.' }
$hash = (Get-FileHash -Algorithm SHA256 -LiteralPath $installer).Hash.ToLowerInvariant()
[IO.File]::WriteAllText((Join-Path $out 'SHA256SUMS.txt'), "$hash  FuzionFrenzySetup.exe`n")
Write-Output "Packaged local-game build installer: $installer"
