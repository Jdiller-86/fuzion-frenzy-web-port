[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$NativeSource,
    [Parameter(Mandatory = $true)][string]$SdkSource,
    [Parameter(Mandatory = $true)][string]$BuildCache,
    [Parameter(Mandatory = $true)][string]$OutputRoot,
    [switch]$ApplyPatches,
    [string]$PackPath
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'

$script:NativeCommit = '071dbd97bdc098c4647cc24c99f9eae693fde1eb'
$script:SdkCommit = '58757ab255c0a6b3929f405d7a5f1567fcd7fda3'
$script:RepoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..'))
$script:PatchRoot = Join-Path $script:RepoRoot 'desktop\windows\patches'
$script:FixedShaderHash = '04468fb7b978df09fb1c87ea880a8c7b5879dc9a7a79151207a606f37067d20b'
$script:AppliedPatches = New-Object 'System.Collections.Generic.List[object]'
$script:BuildSources = New-Object 'System.Collections.Generic.List[object]'

function Get-FullPath([string]$Path) {
    return [IO.Path]::GetFullPath($Path).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
}

function Test-Overlap([string]$First, [string]$Second) {
    $a = Get-FullPath $First
    $b = Get-FullPath $Second
    return $a.Equals($b, [StringComparison]::OrdinalIgnoreCase) -or
        $a.StartsWith(($b + [IO.Path]::DirectorySeparatorChar), [StringComparison]::OrdinalIgnoreCase) -or
        $b.StartsWith(($a + [IO.Path]::DirectorySeparatorChar), [StringComparison]::OrdinalIgnoreCase)
}

function Find-GitRoot([string]$Path) {
    $current = Get-FullPath $Path
    while ($current) {
        if (Test-Path -LiteralPath (Join-Path $current '.git')) { return $current }
        $parent = [IO.Path]::GetDirectoryName($current)
        if (-not $parent -or $parent -eq $current) { break }
        $current = $parent
    }
    return $null
}

function Invoke-Git([string]$Root, [string[]]$Arguments, [switch]$AllowFailure) {
    $output = & $script:GitExe -C $Root @Arguments 2>&1
    $code = $LASTEXITCODE
    if ($code -ne 0 -and -not $AllowFailure) {
        throw ('git {0} failed in {1}: {2}' -f ($Arguments -join ' '), $Root, ([string]::Join([Environment]::NewLine, @($output))))
    }
    return [pscustomobject]@{ ExitCode = $code; Output = @($output) }
}

function Assert-Checkout([string]$Root, [string]$ExpectedHead, [string]$Label) {
    if (-not (Test-Path -LiteralPath (Join-Path $Root '.git'))) { throw "$Label is not a Git checkout: $Root" }
    $head = Invoke-Git $Root @('rev-parse', 'HEAD')
    if ([string]::Join('', $head.Output).Trim() -ne $ExpectedHead) { throw "$Label must be at pinned commit $ExpectedHead." }
}

function Invoke-Patch([string]$Root, [string]$PatchPath, [switch]$Recover) {
    if (-not (Test-Path -LiteralPath $PatchPath -PathType Leaf)) { throw "Required patch is missing: $PatchPath" }
    if ($Recover) {
        $reverse = Invoke-Git $Root @('apply', '--reverse', '--check', $PatchPath) -AllowFailure
        if ($reverse.ExitCode -eq 0) {
            $null = Invoke-Git $Root @('apply', '--reverse', $PatchPath)
        } else {
            $forward = Invoke-Git $Root @('apply', '--check', $PatchPath) -AllowFailure
            if ($forward.ExitCode -ne 0) { throw "Patch has unknown or partial edits in $Root; preserving source tree." }
        }
    }
    $null = Invoke-Git $Root @('apply', '--check', $PatchPath)
    $null = Invoke-Git $Root @('apply', $PatchPath)
    $script:AppliedPatches.Add([pscustomobject]@{ Root = $Root; Path = $PatchPath })
}

function Restore-Patches {
    for ($i = $script:AppliedPatches.Count - 1; $i -ge 0; $i--) {
        $entry = $script:AppliedPatches[$i]
        $check = Invoke-Git $entry.Root @('apply', '--reverse', '--check', $entry.Path) -AllowFailure
        if ($check.ExitCode -ne 0) { throw "Cannot safely restore patch $($entry.Path); source edits were left intact." }
        $null = Invoke-Git $entry.Root @('apply', '--reverse', $entry.Path)
    }
    $script:AppliedPatches.Clear()
}

function Import-VsEnvironment {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) { throw 'Visual Studio Installer vswhere.exe was not found.' }
    $vs = @(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Llvm.Clang -property installationPath 2>$null)
    if ($LASTEXITCODE -ne 0 -or -not $vs) { throw 'A Visual Studio installation with Clang is required.' }
    $devCmd = Join-Path ([string]$vs[0]) 'Common7\Tools\VsDevCmd.bat'
    if (-not (Test-Path -LiteralPath $devCmd -PathType Leaf)) { throw 'VsDevCmd.bat was not found in the selected Visual Studio installation.' }
    $lines = & $env:ComSpec /d /s /c ('call "{0}" -arch=amd64 -host_arch=amd64 >nul && set' -f $devCmd)
    if ($LASTEXITCODE -ne 0) { throw 'Could not initialize the x64 Visual Studio environment.' }
    foreach ($line in $lines) {
        if ([string]$line -match '^([^=]+)=(.*)$' -and -not $matches[1].StartsWith('=')) { Set-Item -Path ('Env:' + $matches[1]) -Value $matches[2] }
    }
    $script:Compiler = Join-Path ([string]$vs[0]) 'VC\Tools\Llvm\x64\bin\clang++.exe'
    if (-not (Test-Path -LiteralPath $script:Compiler -PathType Leaf)) { throw "Visual Studio clang++ was not found: $($script:Compiler)" }
}

function Get-NormalizedSha256([string]$Path) {
    $text = [IO.File]::ReadAllText($Path, (New-Object Text.UTF8Encoding($false, $true)))
    $bytes = (New-Object Text.UTF8Encoding($false)).GetBytes($text.Replace("`r`n", "`n").Replace("`r", "`n"))
    $sha = [Security.Cryptography.SHA256]::Create()
    try { return ([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-', '').ToLowerInvariant() }
    finally { $sha.Dispose() }
}

function Get-Edge([string[]]$NinjaLines, [string]$Suffix) {
    $pattern = '^build CMakeFiles/FuzionFrenzy\.dir/(?:.*/)?' + [regex]::Escape($Suffix) + '\.obj:'
    $edgeMatches = @($NinjaLines | Select-String -Pattern $pattern)
    if ($edgeMatches.Count -ne 1) { throw "Expected one build.ninja compile edge ending in $Suffix.obj." }
    $index = $edgeMatches[0].LineNumber - 1
    $edge = $NinjaLines[$index]
    $properties = @{}
    for ($i = $index + 1; $i -lt $NinjaLines.Count -and $NinjaLines[$i] -match '^  ([A-Z_]+) = (.*)$'; $i++) { $properties[$matches[1]] = $matches[2] }
    if (-not $properties.FLAGS -or -not $properties.DEFINES -or -not $properties.INCLUDES) { throw "Incomplete compile settings in build.ninja for $Suffix." }
    return [pscustomobject]@{ Edge = $edge; Properties = $properties }
}

function Convert-IncludePaths([string]$IncludeText) {
    return @(
        ('-I' + (Join-Path $script:NativeSource 'src')),
        ('-I' + (Join-Path $script:SdkSource 'runtime')),
        ('-I' + (Join-Path $script:NativeSource 'generated\game')),
        '-isystem', (Join-Path $script:BuildCache '_deps\imgui-src'),
        '-isystem', (Join-Path $script:BuildCache '_deps\tomlplusplus-src\include')
    )
}

function Get-LocalHeaderInputs([string]$Source) {
    $includeRoots = @(
        [IO.Path]::GetDirectoryName($Source),
        (Join-Path $script:NativeSource 'src'),
        (Join-Path $script:SdkSource 'runtime'),
        (Join-Path $script:NativeSource 'generated\game'),
        (Join-Path $script:BuildCache '_deps\imgui-src'),
        (Join-Path $script:BuildCache '_deps\tomlplusplus-src\include')
    )
    $seen = @{}
    $pending = New-Object 'System.Collections.Generic.Stack[string]'
    $pending.Push($Source)
    while ($pending.Count -gt 0) {
        $current = $pending.Pop()
        $text = [IO.File]::ReadAllText($current)
        foreach ($match in [regex]::Matches($text, '(?m)^\s*#\s*include\s*[<"]([^">]+)[">]')) {
            $includeName = [string]$match.Groups[1].Value
            $resolved = $null
            foreach ($root in @([IO.Path]::GetDirectoryName($current)) + $includeRoots) {
                $candidate = [IO.Path]::GetFullPath((Join-Path $root $includeName))
                if (Test-Path -LiteralPath $candidate -PathType Leaf) { $resolved = $candidate; break }
            }
            if ($resolved -and -not $seen.ContainsKey($resolved)) {
                $seen[$resolved] = $true
                $pending.Push($resolved)
            }
        }
    }
    $headers = New-Object 'System.Collections.Generic.List[object]'
    foreach ($path in @($seen.Keys | Sort-Object)) {
        $headers.Add([pscustomobject]@{ path = $path; sha256 = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant() })
    }
    return @($headers.ToArray())
}

function Write-AtomicText([string]$Path, [string]$Text) {
    $parent = [IO.Path]::GetDirectoryName($Path)
    [IO.Directory]::CreateDirectory($parent) | Out-Null
    $temporary = $Path + '.' + [Guid]::NewGuid().ToString('N') + '.tmp'
    [IO.File]::WriteAllText($temporary, $Text, (New-Object Text.UTF8Encoding($false)))
    if ([IO.File]::Exists($Path)) {
        $backup = $temporary + '.previous'
        [IO.File]::Replace($temporary, $Path, $backup, $true)
        [IO.File]::Delete($backup)
    } else { [IO.File]::Move($temporary, $Path) }
}

function Compile-Source([string]$NinjaSuffix, [string]$Source, [string]$ObjectPath) {
    $edge = Get-Edge -NinjaLines $script:NinjaLines -Suffix $NinjaSuffix
    $args = @()
    $args += $edge.Properties.FLAGS -split '\s+'
    $args += $edge.Properties.DEFINES -split '\s+'
    $args += Convert-IncludePaths $edge.Properties.INCLUDES
    $args += @('-c', $Source, '-o', $ObjectPath)
    [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($ObjectPath)) | Out-Null
    $sourceHash = (Get-FileHash -LiteralPath $Source -Algorithm SHA256).Hash.ToLowerInvariant()
    $headers = @(Get-LocalHeaderInputs $Source)
    $signature = [ordered]@{
        schemaVersion = 1
        compiler = $script:Compiler
        sourceSha256 = $sourceHash
        headers = $headers
        arguments = @($args)
    } | ConvertTo-Json -Depth 8 -Compress
    $signaturePath = $ObjectPath + '.signature.json'
    if ((Test-Path -LiteralPath $ObjectPath -PathType Leaf) -and (Test-Path -LiteralPath $signaturePath -PathType Leaf) -and
        [IO.File]::ReadAllText($signaturePath) -ceq $signature) {
        Write-Output ("Reusing {0} sha256={1}" -f [IO.Path]::GetFileName($Source), $sourceHash)
        $script:BuildSources.Add([pscustomobject]@{ source = [IO.Path]::GetFileName($Source); sourceSha256 = $sourceHash; headers = $headers; object = [IO.Path]::GetFileName($ObjectPath); objectSha256 = (Get-FileHash -LiteralPath $ObjectPath -Algorithm SHA256).Hash.ToLowerInvariant(); rebuilt = $false })
        return
    }
    Write-Output ("Compiling {0} sha256={1}" -f [IO.Path]::GetFileName($Source), $sourceHash)
    & $script:Compiler @args
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed for $Source (exit $LASTEXITCODE)." }
    Write-AtomicText -Path $signaturePath -Text $signature
    $script:BuildSources.Add([pscustomobject]@{ source = [IO.Path]::GetFileName($Source); sourceSha256 = $sourceHash; headers = $headers; object = [IO.Path]::GetFileName($ObjectPath); objectSha256 = (Get-FileHash -LiteralPath $ObjectPath -Algorithm SHA256).Hash.ToLowerInvariant(); rebuilt = $true })
}

try {
    $script:NativeSource = Get-FullPath $NativeSource
    $script:SdkSource = Get-FullPath $SdkSource
    $script:BuildCache = Get-FullPath $BuildCache
    $script:OutputRoot = Get-FullPath $OutputRoot
    foreach ($path in @($script:NativeSource, $script:SdkSource, $script:BuildCache)) {
        if (-not (Test-Path -LiteralPath $path -PathType Container)) { throw "Required source/build directory is missing: $path" }
    }
    foreach ($path in @($script:NativeSource, $script:SdkSource, $script:BuildCache)) {
        if (Test-Overlap $script:OutputRoot $path) { throw 'OutputRoot must be separate from both source checkouts and BuildCache.' }
    }
    $privatePath = $false
    if (-not $env:LOCALAPPDATA) { throw 'LOCALAPPDATA is required for private output.' }
    $localRoot = Get-FullPath $env:LOCALAPPDATA
    if ($script:OutputRoot.Equals($localRoot, [StringComparison]::OrdinalIgnoreCase) -or $script:OutputRoot.StartsWith(($localRoot + [IO.Path]::DirectorySeparatorChar), [StringComparison]::OrdinalIgnoreCase)) { $privatePath = $true }
    $outputGitRoot = Find-GitRoot $script:OutputRoot
    if ($outputGitRoot) {
        $repoPrefix = $outputGitRoot.TrimEnd([IO.Path]::DirectorySeparatorChar) + [IO.Path]::DirectorySeparatorChar
        if ($script:OutputRoot.StartsWith($repoPrefix, [StringComparison]::OrdinalIgnoreCase)) {
            $relativeOutput = $script:OutputRoot.Substring($repoPrefix.Length).Replace([IO.Path]::DirectorySeparatorChar, '/')
            $gitForIgnore = Get-Command git.exe -ErrorAction SilentlyContinue
            if ($gitForIgnore) {
                & $gitForIgnore.Source -C $outputGitRoot check-ignore --no-index --quiet -- $relativeOutput 2>$null
                if ($LASTEXITCODE -eq 0) { $privatePath = $true }
            }
        }
    }
    if (-not $privatePath) { throw 'OutputRoot must be under LocalAppData or a Git-ignored directory such as artifacts/.' }
    $probe = $script:OutputRoot
    while ($probe -and (Test-Path -LiteralPath $probe)) {
        $item = Get-Item -LiteralPath $probe -Force
        if (($item.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw "OutputRoot contains a link or junction: $probe" }
        $parent = [IO.Path]::GetDirectoryName($probe)
        if (-not $parent -or $parent -eq $probe) { break }
        $probe = $parent
    }
    if ($PackPath) {
        $packFull = Get-FullPath $PackPath
        if (Test-Overlap $packFull $script:OutputRoot) { throw 'PackPath must be outside OutputRoot so the input cannot be overwritten.' }
        if (-not (Test-Path -LiteralPath $packFull -PathType Leaf)) { throw 'PackPath must name an existing local UI BGRA pack.' }
        if ([IO.Path]::GetFileName($packFull) -ine 'ui.bgra') { throw 'PackPath must name the generated ui.bgra pack.' }
        foreach ($input in @($script:NativeSource, $script:SdkSource, $script:BuildCache)) { if (Test-Overlap $packFull $input) { throw 'PackPath must be separate from source checkouts and BuildCache.' } }
    }

    $git = Get-Command git.exe -ErrorAction SilentlyContinue
    $script:GitExe = if ($git) { $git.Source } else { $null }
    if ($ApplyPatches) {
        if (-not $script:GitExe) { throw 'Git for Windows is required when -ApplyPatches is used.' }
        Assert-Checkout $script:NativeSource $script:NativeCommit 'Native source'
        Assert-Checkout $script:SdkSource $script:SdkCommit 'SDK source'
    }
    $script:NinjaLines = Get-Content -LiteralPath (Join-Path $script:BuildCache 'build.ninja')
    if (-not $script:NinjaLines) { throw 'BuildCache does not contain build.ninja.' }
    Import-VsEnvironment

    if ($ApplyPatches) {
        $null = Invoke-Git $script:SdkSource @('diff', '--cached', '--quiet') -AllowFailure
        $script:PatchTargets = @(
            @{ Root = $script:SdkSource; Path = Join-Path $script:PatchRoot 'runtime-fixes.patch' },
            @{ Root = $script:SdkSource; Path = Join-Path $script:PatchRoot 'ui-replacements.patch' },
            @{ Root = $script:NativeSource; Path = Join-Path $script:PatchRoot 'native-ui.patch' }
        )
        foreach ($entry in $script:PatchTargets) { Invoke-Patch -Root $entry.Root -PatchPath $entry.Path -Recover }
    }

    $outputDir = Join-Path $script:OutputRoot 'native-ui'
    $objectDir = Join-Path $outputDir 'objects'
    [IO.Directory]::CreateDirectory($objectDir) | Out-Null
    $objectsBySuffix = @{}
    $objectsBySuffix['runtime/gpu/d3d12.cpp.obj'] = Join-Path $objectDir 'd3d12.cpp.obj'
    $objectsBySuffix['runtime/gpu/shaders.cpp.obj'] = Join-Path $objectDir 'shaders.cpp.obj'
    $objectsBySuffix['runtime/gpu/ui_replacements.cpp.obj'] = Join-Path $objectDir 'ui_replacements.cpp.obj'
    $objectsBySuffix['runtime/app/settings.cpp.obj'] = Join-Path $objectDir 'settings.cpp.obj'
    $objectsBySuffix['src/common/overlay.cpp.obj'] = Join-Path $objectDir 'overlay.cpp.obj'
    $objectsBySuffix['src/common/input_kbm.cpp.obj'] = Join-Path $objectDir 'input_kbm.cpp.obj'
    $objectsBySuffix['src/common/game_info.cpp.obj'] = Join-Path $objectDir 'game_info.cpp.obj'

    $sourceBySuffix = @{
        'runtime/gpu/d3d12.cpp.obj' = Join-Path $script:SdkSource 'runtime\gpu\d3d12.cpp'
        'runtime/gpu/shaders.cpp.obj' = Join-Path $script:SdkSource 'runtime\gpu\shaders.cpp'
        'runtime/gpu/ui_replacements.cpp.obj' = Join-Path $script:SdkSource 'runtime\gpu\ui_replacements.cpp'
        'runtime/app/settings.cpp.obj' = Join-Path $script:SdkSource 'runtime\app\settings.cpp'
        'src/common/overlay.cpp.obj' = Join-Path $script:NativeSource 'src\common\overlay.cpp'
        'src/common/input_kbm.cpp.obj' = Join-Path $script:NativeSource 'src\common\input_kbm.cpp'
        'src/common/game_info.cpp.obj' = Join-Path $script:NativeSource 'src\common\game_info.cpp'
    }
    foreach ($suffix in $objectsBySuffix.Keys) {
        if (-not (Test-Path -LiteralPath $sourceBySuffix[$suffix] -PathType Leaf)) { throw "Expected patch/source file is missing: $($sourceBySuffix[$suffix])" }
        if ($suffix -eq 'runtime/gpu/shaders.cpp.obj' -and (Get-NormalizedSha256 $sourceBySuffix[$suffix]) -ne $script:FixedShaderHash) {
            throw 'SDK shaders.cpp is not the published fixed source version; restore the exact runtime patch or use its matching SDK checkout.'
        }
        if ($suffix -eq 'runtime/gpu/ui_replacements.cpp.obj') {
            # Added by the UI patch; its compiler contract matches the SDK GPU source edge.
            Compile-Source -NinjaSuffix 'runtime/gpu/d3d12.cpp' -Source $sourceBySuffix[$suffix] -ObjectPath $objectsBySuffix[$suffix]
        } else {
            Compile-Source -NinjaSuffix $suffix.Substring(0, $suffix.Length - 4) -Source $sourceBySuffix[$suffix] -ObjectPath $objectsBySuffix[$suffix]
        }
    }

    $linkLine = @($script:NinjaLines | Where-Object { $_ -match '^build FuzionFrenzy\.exe: CXX_EXECUTABLE_LINKER__FuzionFrenzy_Release ' })
    if ($linkLine.Count -ne 1) { throw 'Expected one FuzionFrenzy.exe link rule in build.ninja.' }
    $objectText = ($linkLine[0] -split 'CXX_EXECUTABLE_LINKER__FuzionFrenzy_Release ', 2)[1] -split ' \|', 2 | Select-Object -First 1
    $objects = $objectText -split '\s+'
    $linkArgs = @('-nostartfiles', '-nostdlib', '-march=x86-64-v2', '-O3', '-DNDEBUG', '-D_DLL', '-D_MT', '-Xclang', '--dependent-lib=msvcrt', '-Xlinker', '/subsystem:windows', '-fuse-ld=lld-link', '-g')
    $replaced = @{}
    $linkObjects = New-Object 'System.Collections.Generic.List[string]'
    foreach ($object in $objects) {
        $normalized = $object.Replace('\', '/')
        $replacement = $null
        foreach ($suffix in $objectsBySuffix.Keys) {
            if ($normalized.EndsWith('/' + $suffix, [StringComparison]::OrdinalIgnoreCase)) { $replacement = $objectsBySuffix[$suffix]; $replaced[$suffix] = $true; break }
        }
        if (-not $replacement) { $replacement = Join-Path $script:BuildCache $object.Replace('/', '\') }
        if (-not (Test-Path -LiteralPath $replacement -PathType Leaf)) { throw "Link input is missing: $replacement" }
        $linkArgs += $replacement
        $linkObjects.Add($normalized)
    }
    foreach ($suffix in @('runtime/gpu/d3d12.cpp.obj','runtime/gpu/shaders.cpp.obj','runtime/app/settings.cpp.obj','src/common/overlay.cpp.obj','src/common/input_kbm.cpp.obj','src/common/game_info.cpp.obj')) {
        if (-not $replaced.ContainsKey($suffix)) { throw "build.ninja link inputs did not include expected object $suffix." }
    }
    $linkArgs += $objectsBySuffix['runtime/gpu/ui_replacements.cpp.obj']
    $linkObjects.Add('private:objects/ui_replacements.cpp.obj')
    $exe = Join-Path $outputDir 'FuzionFrenzy-ui.exe'
    $linkArgs += @('-o', $exe, '-Xlinker', '/MANIFEST:EMBED', '-Xlinker', ("/implib:{0}" -f (Join-Path $outputDir 'FuzionFrenzy-ui.lib')), '-Xlinker', ("/pdb:{0}" -f (Join-Path $outputDir 'FuzionFrenzy-ui.pdb')), '-Xlinker', '/version:0.0', (Join-Path $script:BuildCache '_deps\sdl3-build\SDL3-static.lib'))
    $linkRow = @($script:NinjaLines | Where-Object { $_ -match '^  LINK_LIBRARIES = .*SDL3-static\.lib' } | Select-Object -First 1)
    if ($linkRow.Count -ne 1) { throw 'Could not read the native target link libraries from build.ninja.' }
    $libraries = ($linkRow[0] -replace '^  LINK_LIBRARIES = .*?SDL3-static\.lib\s+', '').Trim()
    $linkArgs += $libraries -split '\s+' | Where-Object { $_ }
    $responsePath = Join-Path $outputDir 'link.rsp'
    $responseLines = @($linkArgs | ForEach-Object { '"' + ([string]$_).Replace('\', '/') + '"' })
    [IO.File]::WriteAllLines($responsePath, $responseLines, (New-Object Text.UTF8Encoding($false)))
    & $script:Compiler ('@' + $responsePath)
    if ($LASTEXITCODE -ne 0) { throw "Native UI link failed (exit $LASTEXITCODE)." }

    $exeHash = (Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant()
    $responseHash = (Get-FileHash -LiteralPath $responsePath -Algorithm SHA256).Hash.ToLowerInvariant()
    $buildManifest = [ordered]@{
        schemaVersion = 1
        sources = @($script:BuildSources.ToArray())
        link = [ordered]@{
            responseFile = 'link.rsp'
            responseFileSha256 = $responseHash
            inputObjects = @($linkObjects.ToArray())
            executable = 'FuzionFrenzy-ui.exe'
            executableSha256 = $exeHash
        }
    } | ConvertTo-Json -Depth 10
    Write-AtomicText -Path (Join-Path $outputDir 'build-manifest.json') -Text $buildManifest

    if ($PackPath) {
        [IO.File]::Copy($packFull, (Join-Path $outputDir 'ui.bgra'), $true)
    }
    Write-Output ("Executable sha256={0}" -f $exeHash)
    Write-Output "Private executable: $exe"
}
finally {
    if ($script:AppliedPatches.Count -gt 0) { Restore-Patches }
}
