param(
    [Parameter(Mandatory = $true)][string] $SdkRoot,
    [Parameter(Mandatory = $true)][string] $OutputDirectory,
    [string] $Compiler = 'clang++'
)
$ErrorActionPreference = 'Stop'
New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null
$runtime = Join-Path $SdkRoot 'runtime'
$fixture = Join-Path $PSScriptRoot 'fixed_function_null_texture.cpp'
$binary = Join-Path $OutputDirectory 'fixed-function-null-texture.exe'
& $Compiler -O0 -g -std=c++20 -D_DLL -D_MT -Xclang --dependent-lib=msvcrt -D_CRT_SECURE_NO_WARNINGS "-I$runtime" "$runtime/gpu/shaders.cpp" $fixture -o $binary
if ($LASTEXITCODE) { throw 'Shader fixture compilation failed' }
& $binary
if ($LASTEXITCODE) { throw 'Shader fixture failed' }
