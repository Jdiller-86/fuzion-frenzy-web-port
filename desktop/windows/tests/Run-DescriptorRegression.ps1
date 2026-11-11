param(
    [Parameter(Mandatory = $true)][string] $SdkRoot,
    [Parameter(Mandatory = $true)][string] $OutputDirectory,
    [string] $Compiler = 'clang++'
)
$ErrorActionPreference = 'Stop'
New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null
$OutputDirectory = (Resolve-Path -LiteralPath $OutputDirectory).Path
$source = Join-Path $SdkRoot 'runtime/gpu/d3d12.cpp'
$text = Get-Content -LiteralPath $source -Raw
function Extract-Method([string] $signature) {
    $at = $text.IndexOf($signature, [StringComparison]::Ordinal)
    if ($at -lt 0) { throw "Missing method: $signature" }
    $open = $text.IndexOf('{', $at)
    $depth = 0
    for ($i = $open; $i -lt $text.Length; $i++) {
        if ($text[$i] -eq '{') { $depth++ }
        if ($text[$i] -eq '}') {
            $depth--
            if ($depth -eq 0) { return $text.Substring($at, $i - $at + 1) }
        }
    }
    throw "Unclosed method: $signature"
}
$allocator = Extract-Method 'uint32_t D3D12Backend::AllocSrvSlots(uint32_t n)'
$allocator | Set-Content -LiteralPath (Join-Path $OutputDirectory 'production-allocator.inc') -Encoding utf8
$constants = foreach ($name in @('kFrames', 'kSrvCount', 'kInvalidSrvSlot')) {
    $match = [regex]::Match($text, "(?m)^constexpr [^\r\n]+\b$name\s*=[^\r\n]+;")
    if (!$match.Success) { throw "Missing production constant: $name" }
    $match.Value
}
$constants | Set-Content -LiteralPath (Join-Path $OutputDirectory 'production-constants.inc') -Encoding utf8
foreach ($signature in @('void D3D12Backend::ResetFrame()', 'void D3D12Backend::Submit(bool wait)')) {
    $method = Extract-Method $signature
    $wait = $method.IndexOf('WaitFence(')
    $reset = $method.IndexOf('f.srv_page = 0;')
    $base = $method.IndexOf('srv_heap_ = base_srv_heap_;')
    if ($wait -lt 0 -or $reset -le $wait -or $base -le $reset) {
        throw "Fence/reset/base ordering changed: $signature"
    }
    if ($signature.Contains('Submit') -and $method.IndexOf('if (wait) {') -ge $wait) {
        throw 'Submit fence/reset no longer under wait branch'
    }
}
Write-Output 'PASS production reset source orders fence wait before page reset and base restoration'
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'descriptor-rollover.cpp') -Destination $OutputDirectory -Force
$binary = Join-Path $OutputDirectory 'descriptor-rollover.exe'
& $Compiler -O0 -g -std=c++20 -D_DLL -D_MT -Xclang --dependent-lib=msvcrt (Join-Path $OutputDirectory 'descriptor-rollover.cpp') -o $binary
if ($LASTEXITCODE) { throw 'Allocator fixture compilation failed' }
& $binary
if ($LASTEXITCODE) { throw 'Allocator fixture failed' }
