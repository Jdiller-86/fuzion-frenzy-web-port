[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$InputPath,
    [Parameter(Mandatory = $true)][string]$Workspace,
    [Parameter(Mandatory = $true)][string]$GameDirectory,
    [int]$Jobs = 0,
    [string]$StatusPath
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'

$script:StatusPathValue = $null
$script:LastProgress = $null
$script:NativeRoot = $null
$script:SdkRoot = $null
$script:GameRoot = $null
$script:InstallRoot = $null
$script:ImageStage = $null
$script:PythonExe = $null
$script:PythonPrefix = @()
$script:Status = [ordered]@{
    schemaVersion = 1
    status = 'running'
    phase = 'validation'
    progress = 0
    message = 'Checking the selected game files.'
    error = $null
    outputExe = $null
    updatedAtUtc = [DateTime]::UtcNow.ToString('o')
}

function Write-Status {
    param(
        [string]$Phase,
        [string]$Message,
        [Nullable[int]]$Progress = $null,
        [string]$State = 'running',
        [string]$ErrorMessage = $null,
        [string]$OutputExe = $null
    )

    $script:Status.status = $State
    $script:Status.phase = $Phase
    $script:Status.message = $Message
    $script:Status.progress = $Progress
    $script:Status.error = $ErrorMessage
    $script:Status.outputExe = $OutputExe
    $script:Status.updatedAtUtc = [DateTime]::UtcNow.ToString('o')
    if (-not $script:StatusPathValue) { return }

    $parent = [IO.Path]::GetDirectoryName($script:StatusPathValue)
    if ($parent) { [IO.Directory]::CreateDirectory($parent) | Out-Null }
    $temp = $script:StatusPathValue + '.' + [Guid]::NewGuid().ToString('N') + '.tmp'
    $json = $script:Status | ConvertTo-Json -Depth 5
    [IO.File]::WriteAllText($temp, $json + [Environment]::NewLine, (New-Object Text.UTF8Encoding($false)))
    if ([IO.File]::Exists($script:StatusPathValue)) {
        [IO.File]::Replace($temp, $script:StatusPathValue, $null, $true)
    } else {
        [IO.File]::Move($temp, $script:StatusPathValue)
    }
}

function Write-ProgressStatus {
    param([string]$Phase, [string]$Message, [Nullable[int]]$Progress = $null)
    Write-Status -Phase $Phase -Message $Message -Progress $Progress
    Write-Output ('[{0}] {1}' -f $Phase, $Message)
}

function Invoke-Native {
    param(
        [Parameter(Mandatory = $true)][string]$FilePath,
        [Parameter(Mandatory = $true)][string[]]$Arguments,
        [string]$WorkingDirectory,
        [string]$Phase,
        [string]$Message
    )

    if ($WorkingDirectory) { Push-Location -LiteralPath $WorkingDirectory }
    $previousErrorPreference = $ErrorActionPreference
    try {
        # Windows PowerShell 5.1 represents ordinary native stderr (including
        # Git download progress and compiler warnings) as ErrorRecord objects.
        # The native exit code, rather than that stream, determines failure.
        $ErrorActionPreference = 'Continue'
        Write-Output ('> {0} {1}' -f $FilePath, ($Arguments -join ' '))
        & $FilePath @Arguments 2>&1 | ForEach-Object {
            $line = [string]$_
            Write-Output $line
            if ($Phase -eq 'compile' -and $line -match '^\[\s*(\d+)\s*/\s*(\d+)\]') {
                $done = [int]$matches[1]
                $total = [int]$matches[2]
                if ($total -gt 0) {
                    $percent = 70 + [int][Math]::Floor(21.0 * $done / $total)
                    $now = [DateTime]::UtcNow
                    if ($null -eq $script:LastProgress -or $percent -ne $script:LastProgress -or
                        ($now - $script:LastProgressAt).TotalSeconds -ge 4) {
                        Write-Status -Phase $Phase -Message $Message -Progress ([Math]::Min(91, $percent))
                        $script:LastProgress = $percent
                        $script:LastProgressAt = $now
                    }
                }
            }
        }
        $exitCode = $LASTEXITCODE
        if ($exitCode -ne 0) {
            throw ('{0} exited with code {1}.' -f [IO.Path]::GetFileName($FilePath), $exitCode)
        }
    }
    finally {
        $ErrorActionPreference = $previousErrorPreference
        if ($WorkingDirectory) { Pop-Location }
    }
}

function Get-FileSha256 {
    param([string]$Path)
    $sha = [Security.Cryptography.SHA256]::Create()
    $stream = [IO.File]::OpenRead($Path)
    try {
        return ([BitConverter]::ToString($sha.ComputeHash($stream))).Replace('-', '').ToLowerInvariant()
    }
    finally {
        $stream.Dispose()
        $sha.Dispose()
    }
}

function Test-PathsOverlap {
    param([string]$First, [string]$Second)
    $firstFull = [IO.Path]::GetFullPath($First).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    $secondFull = [IO.Path]::GetFullPath($Second).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    if ($firstFull -eq $secondFull) { return $true }
    $separator = [IO.Path]::DirectorySeparatorChar
    return $firstFull.StartsWith(($secondFull + $separator), [StringComparison]::OrdinalIgnoreCase) -or
        $secondFull.StartsWith(($firstFull + $separator), [StringComparison]::OrdinalIgnoreCase)
}

function Assert-ValidXbe {
    param([string]$Path)
    $info = Get-Item -LiteralPath $Path -Force
    if ($info.PSIsContainer -or $info.Length -ne 2834432) {
        throw 'The selected image does not contain the supported Fuzion Frenzy XBE size.'
    }
    $hash = Get-FileSha256 -Path $Path
    if ($hash -ne '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9') {
        throw 'The selected game is not the supported Fuzion Frenzy disc revision (default.xbe SHA-256 mismatch).'
    }
}

function Get-TopLevelXbe {
    param([string]$Folder)
    $matches = @(Get-ChildItem -LiteralPath $Folder -File -Force | Where-Object { $_.Name -ieq 'default.xbe' })
    if ($matches.Count -ne 1) { throw 'The selected folder must contain exactly one top-level default.xbe.' }
    return $matches[0].FullName
}

function Test-HostArtifactPath {
    param([string]$RelativePath)
    $parts = $RelativePath -split '[\\/]'
    foreach ($part in $parts) {
        if ($part -match '^(?i:logs?|shadercache|shader_cache|hdd|crashdumps|settings)$') { return $true }
    }
    $leaf = [IO.Path]::GetFileName($RelativePath)
    if ($leaf -ieq 'settings.toml' -or $leaf -match '(?i)\.(log|dmp)$') { return $true }
    return $false
}

function Assert-NoReparsePointInPath {
    param([string]$Root, [string]$Path)
    $rootFull = [IO.Path]::GetFullPath($Root).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    $pathFull = [IO.Path]::GetFullPath($Path)
    if ($pathFull -ne $rootFull -and -not $pathFull.StartsWith(($rootFull + [IO.Path]::DirectorySeparatorChar), [StringComparison]::OrdinalIgnoreCase)) {
        throw 'An install destination escapes the selected game directory.'
    }
    $current = $pathFull
    while ($current -and $current.StartsWith($rootFull, [StringComparison]::OrdinalIgnoreCase)) {
        if (Test-Path -LiteralPath $current) {
            $item = Get-Item -LiteralPath $current -Force
            if (($item.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) {
                throw ('The install directory contains a link or junction: {0}' -f $item.Name)
            }
        }
        if ($current -eq $rootFull) { break }
        $current = [IO.Path]::GetDirectoryName($current)
    }
}

function Copy-ValidatedGameTree {
    param([string]$Source, [string]$Destination, [switch]$IncludeXbe)

    $root = [IO.Path]::GetFullPath($Source).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    $destRoot = [IO.Path]::GetFullPath($Destination).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    $sourceInfo = Get-Item -LiteralPath $root -Force
    if (($sourceInfo.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'The selected game folder cannot be a link or junction.' }
    Assert-NoReparsePointInPath -Root $destRoot -Path $destRoot
    [IO.Directory]::CreateDirectory($destRoot) | Out-Null
    $copied = 0
    $stack = New-Object 'System.Collections.Generic.Stack[System.IO.DirectoryInfo]'
    $stack.Push((Get-Item -LiteralPath $root -Force))

    while ($stack.Count -gt 0) {
        $directory = $stack.Pop()
        foreach ($entry in Get-ChildItem -LiteralPath $directory.FullName -Force) {
            $attributes = [IO.File]::GetAttributes($entry.FullName)
            if (($attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) {
                throw ('The selected folder contains a link or reparse point: {0}' -f $entry.Name)
            }
            $relative = $entry.FullName.Substring($root.Length).TrimStart([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
            if ([string]::IsNullOrWhiteSpace($relative) -or $relative -match '(^|[\\/])\.\.([\\/]|$)' -or $relative.Contains(':')) {
                throw 'The selected folder contains an unsafe path.'
            }
            if (Test-HostArtifactPath -RelativePath $relative) { continue }
            $target = [IO.Path]::GetFullPath((Join-Path $destRoot $relative))
            $prefix = $destRoot + [IO.Path]::DirectorySeparatorChar
            if (-not $target.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) {
                throw 'A selected game path escapes the installation assets folder.'
            }
            Assert-NoReparsePointInPath -Root $destRoot -Path $target
            if ($entry.PSIsContainer) {
                [IO.Directory]::CreateDirectory($target) | Out-Null
                $stack.Push($entry)
            } else {
                if (-not $IncludeXbe -and $entry.Name -ieq 'default.xbe') { continue }
                [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($target)) | Out-Null
                [IO.File]::Copy($entry.FullName, $target, $true)
                $copied++
            }
        }
    }
    if ($copied -eq 0 -and -not $IncludeXbe) { throw 'The selected folder contains no game data files besides default.xbe.' }
}

function Assert-ExtractedGameTree {
    param([string]$Source)
    $root = [IO.Path]::GetFullPath($Source).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    $stack = New-Object 'System.Collections.Generic.Stack[System.IO.DirectoryInfo]'
    $stack.Push((Get-Item -LiteralPath $root -Force))
    $dataFiles = 0
    while ($stack.Count -gt 0) {
        $directory = $stack.Pop()
        foreach ($entry in Get-ChildItem -LiteralPath $directory.FullName -Force) {
            $attributes = [IO.File]::GetAttributes($entry.FullName)
            if (($attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) {
                throw ('The selected folder contains a link or reparse point: {0}' -f $entry.Name)
            }
            $relative = $entry.FullName.Substring($root.Length).TrimStart([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
            if ([string]::IsNullOrWhiteSpace($relative) -or $relative -match '(^|[\\/])\.\.([\\/]|$)' -or $relative.Contains(':')) {
                throw 'The selected folder contains an unsafe path.'
            }
            if (Test-HostArtifactPath -RelativePath $relative) { continue }
            if ($entry.PSIsContainer) {
                $stack.Push($entry)
            } elseif ($entry.Name -ine 'default.xbe') {
                $dataFiles++
            }
        }
    }
    if ($dataFiles -eq 0) { throw 'The selected folder contains no game data files besides default.xbe.' }
}

function Get-VsInstallPath {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) { return $null }
    $result = @(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null)
    if ($LASTEXITCODE -ne 0 -or -not $result) { return $null }
    return [string]$result[0]
}

function Refresh-ProcessPath {
    $machinePath = [Environment]::GetEnvironmentVariable('Path', 'Machine')
    $userPath = [Environment]::GetEnvironmentVariable('Path', 'User')
    $parts = @($machinePath, $userPath, $env:PATH) | Where-Object { -not [string]::IsNullOrWhiteSpace([string]$_) }
    $env:PATH = $parts -join ';'
}

function Import-VsDeveloperEnvironment {
    param([string]$InstallPath)

    $devCmd = Join-Path $InstallPath 'Common7\Tools\VsDevCmd.bat'
    if (-not (Test-Path -LiteralPath $devCmd -PathType Leaf)) { throw 'Visual Studio Build Tools is missing VsDevCmd.bat.' }
    $command = 'call "{0}" -arch=amd64 -host_arch=amd64 >nul && set' -f $devCmd
    $envLines = & $env:ComSpec /d /s /c $command
    if ($LASTEXITCODE -ne 0) { throw 'Could not initialize the Visual Studio x64 developer environment.' }
    foreach ($line in $envLines) {
        $text = [string]$line
        if ($text -match '^([^=]+)=(.*)$') {
            $name = $matches[1]
            if (-not $name.StartsWith('=')) { Set-Item -Path ('Env:' + $name) -Value $matches[2] }
        }
    }

    $vsCmake = Join-Path $InstallPath 'Common7\IDE\CommonExtensions\Microsoft\CMake'
    $paths = @(
        (Join-Path $vsCmake 'CMake\bin'),
        (Join-Path $vsCmake 'Ninja'),
        (Join-Path $InstallPath 'VC\Tools\Llvm\x64\bin')
    )
    $env:PATH = (($paths | Where-Object { Test-Path -LiteralPath $_ }) -join ';') + ';' + $env:PATH
    foreach ($tool in @('cmake.exe', 'ninja.exe', 'clang.exe', 'clang++.exe')) {
        if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) { throw ('Required build tool is missing: {0}. Use Install prerequisites.' -f $tool) }
    }
}

function Ensure-PinnedCheckout {
    param([string]$Path, [string]$Url, [string]$Commit, [string]$Label)

    $created = $false
    if (-not (Test-Path -LiteralPath $Path)) {
        [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($Path)) | Out-Null
        Invoke-Native -FilePath $script:GitExe -Arguments @('clone', '--no-checkout', $Url, $Path) -Phase 'checkout' -Message ('Downloading pinned {0} source.' -f $Label)
        $created = $true
    }
    if (-not (Test-Path -LiteralPath (Join-Path $Path '.git'))) {
        throw ('The existing {0} source path is not a Git checkout: {1}' -f $Label, $Path)
    }
    $origin = & $script:GitExe -C $Path remote get-url origin 2>$null
    $actual = ([string]$origin).Trim().TrimEnd('/')
    $expected = $Url.TrimEnd('/')
    $expectedWithoutGit = if ($expected.EndsWith('.git', [StringComparison]::OrdinalIgnoreCase)) { $expected.Substring(0, $expected.Length - 4) } else { $expected }
    if ($LASTEXITCODE -ne 0 -or ($actual -ne $expected -and $actual -ne $expectedWithoutGit)) {
        throw ('The existing {0} checkout has a different origin.' -f $Label)
    }
    if ($created) {
        Invoke-Native -FilePath $script:GitExe -Arguments @('-C', $Path, 'fetch', '--depth', '1', 'origin', $Commit) -Phase 'checkout' -Message ('Selecting the pinned {0} revision.' -f $Label)
        Invoke-Native -FilePath $script:GitExe -Arguments @('-C', $Path, 'checkout', '--detach', $Commit) -Phase 'checkout' -Message ('Checking out the pinned {0} revision.' -f $Label)
    }
    $head = & $script:GitExe -C $Path rev-parse HEAD 2>$null
    if ($LASTEXITCODE -ne 0) { throw ('Could not read the {0} source revision.' -f $Label) }
    if (([string]$head).Trim() -ne $Commit) {
        if (-not $created) {
            $status = & $script:GitExe -C $Path status --porcelain --untracked-files=no
            if ($LASTEXITCODE -ne 0 -or $status) { throw ('The existing {0} checkout has local tracked changes; it was left untouched.' -f $Label) }
        }
        Invoke-Native -FilePath $script:GitExe -Arguments @('-C', $Path, 'fetch', '--depth', '1', 'origin', $Commit) -Phase 'checkout' -Message ('Updating {0} to the pinned revision.' -f $Label)
        Invoke-Native -FilePath $script:GitExe -Arguments @('-C', $Path, 'checkout', '--detach', $Commit) -Phase 'checkout' -Message ('Selecting the pinned {0} revision.' -f $Label)
    }
    $status = & $script:GitExe -C $Path status --porcelain --untracked-files=no
    if ($LASTEXITCODE -ne 0 -or $status) { throw ('The {0} checkout contains local tracked changes; it was left untouched.' -f $Label) }
}

function Copy-InstallFiles {
    param([string]$BuildDirectory, [string]$StagingDirectory, [string]$InputKind, [string]$SourcePath)

    [IO.Directory]::CreateDirectory($StagingDirectory) | Out-Null
    $exe = Join-Path $BuildDirectory 'FuzionFrenzy.exe'
    if (-not (Test-Path -LiteralPath $exe -PathType Leaf)) { throw 'CMake completed without producing FuzionFrenzy.exe.' }
    [IO.File]::Copy($exe, (Join-Path $StagingDirectory 'FuzionFrenzy.exe'), $true)
    foreach ($runtimeFile in Get-ChildItem -LiteralPath $BuildDirectory -File -Filter '*.dll' -Force) {
        [IO.File]::Copy($runtimeFile.FullName, (Join-Path $StagingDirectory $runtimeFile.Name), $true)
    }
    $nativeLicense = Join-Path $script:NativeRoot 'LICENSE'
    if (Test-Path -LiteralPath $nativeLicense -PathType Leaf) {
        [IO.File]::Copy($nativeLicense, (Join-Path $StagingDirectory 'FuzionFrenzyNative-LICENSE.txt'), $true)
    }
    $thirdParty = Join-Path $script:SdkRoot 'THIRD_PARTY.txt'
    if (Test-Path -LiteralPath $thirdParty -PathType Leaf) {
        [IO.File]::Copy($thirdParty, (Join-Path $StagingDirectory 'XboxXBERecomp-THIRD_PARTY.txt'), $true)
    }
    $sdkLicense = Join-Path $script:SdkRoot 'LICENSE'
    if (Test-Path -LiteralPath $sdkLicense -PathType Leaf) {
        [IO.File]::Copy($sdkLicense, (Join-Path $StagingDirectory 'XboxXBERecomp-LICENSE.txt'), $true)
    }

    $assets = Join-Path $StagingDirectory 'assets'
    [IO.Directory]::CreateDirectory($assets) | Out-Null
    $xbe = Join-Path $script:GameRoot 'default.xbe'
    [IO.File]::Copy($xbe, (Join-Path $assets 'default.xbe'), $true)
    if ($InputKind -eq 'image') {
        [IO.File]::Copy($SourcePath, (Join-Path $assets 'game.iso'), $true)
    } else {
        Copy-ValidatedGameTree -Source $SourcePath -Destination $assets
    }
}

function Install-StagedOutput {
    param([string]$StagingDirectory, [string]$DestinationDirectory)

    [IO.Directory]::CreateDirectory($DestinationDirectory) | Out-Null
    foreach ($file in Get-ChildItem -LiteralPath $StagingDirectory -File -Force) {
        $destination = Join-Path $DestinationDirectory $file.Name
        Assert-NoReparsePointInPath -Root $DestinationDirectory -Path $destination
        $temp = $destination + '.' + [Guid]::NewGuid().ToString('N') + '.new'
        [IO.File]::Copy($file.FullName, $temp, $true)
        if ([IO.File]::Exists($destination)) { [IO.File]::Replace($temp, $destination, $null, $true) }
        else { [IO.File]::Move($temp, $destination) }
    }

    $sourceAssets = Join-Path $StagingDirectory 'assets'
    $destAssets = Join-Path $DestinationDirectory 'assets'
    [IO.Directory]::CreateDirectory($destAssets) | Out-Null
    Assert-NoReparsePointInPath -Root $DestinationDirectory -Path $destAssets
    $stagedXbe = Join-Path $sourceAssets 'default.xbe'
    [IO.File]::Copy($stagedXbe, (Join-Path $destAssets 'default.xbe'), $true)
    Copy-ValidatedGameTree -Source $sourceAssets -Destination $destAssets
}

try {
    if ($Jobs -lt 0 -or $Jobs -gt 64) { throw 'Jobs must be zero (automatic) or between 1 and 64.' }
    if ($Jobs -eq 0) { $Jobs = [Math]::Max(1, [Math]::Min(8, [Environment]::ProcessorCount)) }
    $inputFull = [IO.Path]::GetFullPath($InputPath)
    $workspaceFull = [IO.Path]::GetFullPath($Workspace)
    $gameDirectoryFull = [IO.Path]::GetFullPath($GameDirectory)
    if (-not (Test-Path -LiteralPath $inputFull)) { throw 'The selected game input could not be found.' }
    if (-not $env:LOCALAPPDATA) { throw 'Windows did not provide the per-user LocalAppData folder.' }
    $localAppData = [IO.Path]::GetFullPath($env:LOCALAPPDATA).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar)
    if ($workspaceFull -ne $localAppData -and -not $workspaceFull.StartsWith(($localAppData + [IO.Path]::DirectorySeparatorChar), [StringComparison]::OrdinalIgnoreCase)) {
        throw 'The source workspace must remain inside this Windows user’s LocalAppData folder.'
    }
    if (-not $StatusPath) { $StatusPath = Join-Path $workspaceFull 'build-status.json' }
    $script:StatusPathValue = [IO.Path]::GetFullPath($StatusPath)
    $script:WorkspaceRoot = $workspaceFull
    $script:GameRoot = Join-Path $workspaceFull '.local-input'
    $script:NativeRoot = Join-Path $workspaceFull '.tools\fuzion-native'
    $script:SdkRoot = Join-Path $workspaceFull '.tools\XboxXBERecomp'
    $script:InstallRoot = $gameDirectoryFull

    if (Test-Path -LiteralPath $gameDirectoryFull) {
        $gameItem = Get-Item -LiteralPath $gameDirectoryFull -Force
        if (-not $gameItem.PSIsContainer) { throw 'The game install path must be a directory.' }
        if (($gameItem.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'The game install directory cannot be a link or junction.' }
    }
    if (Test-PathsOverlap -First $workspaceFull -Second $gameDirectoryFull) {
        throw 'The source workspace and installed game directory must be separate folders.'
    }
    if (-not (Test-Path -LiteralPath $workspaceFull)) { [IO.Directory]::CreateDirectory($workspaceFull) | Out-Null }
    $workspaceItem = Get-Item -LiteralPath $workspaceFull -Force
    if (($workspaceItem.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'The build workspace cannot be a link or junction.' }

    Refresh-ProcessPath
    $gitCommand = Get-Command git.exe -ErrorAction SilentlyContinue
    if (-not $gitCommand) { throw 'Git for Windows is required. Use Install prerequisites, then try again.' }
    $script:GitExe = $gitCommand.Source
    $pythonLauncher = Get-Command py.exe -ErrorAction SilentlyContinue
    if ($pythonLauncher) {
        & $pythonLauncher.Source -3.12 --version *> $null
        if ($LASTEXITCODE -eq 0) { $script:PythonExe = $pythonLauncher.Source; $script:PythonPrefix = @('-3.12') }
    }
    if (-not $script:PythonExe) {
        $python = Get-Command python.exe -ErrorAction SilentlyContinue
        if ($python) {
            $version = & $python.Source --version 2>&1
            if ($LASTEXITCODE -eq 0 -and [string]$version -match '^Python 3\.12(?:\.|$)') {
                $script:PythonExe = $python.Source
                $script:PythonPrefix = @()
            }
        }
    }
    if (-not $script:PythonExe) { throw 'Python 3.12 is required. Use Install prerequisites, then try again.' }

    $inputInfo = Get-Item -LiteralPath $inputFull -Force
    if ($inputInfo.PSIsContainer) {
        if (($inputInfo.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'The selected game folder cannot be a link or junction.' }
        if (Test-PathsOverlap -First $inputInfo.FullName -Second $workspaceFull) { throw 'The selected game folder must be separate from the build workspace.' }
        if (Test-PathsOverlap -First $inputInfo.FullName -Second $gameDirectoryFull) { throw 'The selected game folder must be separate from the installed game directory.' }
        $script:InputKind = 'folder'
        $script:InputSource = $inputInfo.FullName
        $xbePath = Get-TopLevelXbe -Folder $script:InputSource
        Assert-ValidXbe -Path $xbePath
        Assert-ExtractedGameTree -Source $script:InputSource
    } else {
        if ($inputInfo.Extension -notmatch '(?i)^\.(iso|xiso)$') { throw 'Choose an ISO/XISO image or a folder with extracted game files.' }
        $script:InputKind = 'image'
        $script:InputSource = $inputInfo.FullName
        $safeStage = Join-Path $workspaceFull ('.xbe-extract-' + [Guid]::NewGuid().ToString('N'))
        [IO.Directory]::CreateDirectory($safeStage) | Out-Null
        $script:ImageStage = $safeStage
    }

    Write-ProgressStatus -Phase 'prerequisites' -Message 'Checking Git, Python 3.12, CMake, Ninja, and Clang.' -Progress 5
    $vsInstall = Get-VsInstallPath
    if (-not $vsInstall) { throw 'Visual Studio 2022 Build Tools with the x64 C++ tools are required. Use Install prerequisites, then try again.' }
    Import-VsDeveloperEnvironment -InstallPath $vsInstall

    Write-ProgressStatus -Phase 'checkout' -Message 'Preparing the pinned native project and SDK source.' -Progress 12
    Ensure-PinnedCheckout -Path $script:NativeRoot -Url 'https://github.com/abilliontokens/fuzionfrenzy-recomp.git' -Commit '071dbd97bdc098c4647cc24c99f9eae693fde1eb' -Label 'native'
    Ensure-PinnedCheckout -Path $script:SdkRoot -Url 'https://github.com/abilliontokens/XboxXBERecomp.git' -Commit '58757ab255c0a6b3929f405d7a5f1567fcd7fda3' -Label 'XboxXBERecomp SDK'

    if ($script:InputKind -eq 'image') {
        Write-ProgressStatus -Phase 'extract' -Message 'Validating and reading default.xbe from the local image.' -Progress 28
        $extractor = Join-Path $script:NativeRoot 'tools\xiso_extract.py'
        Invoke-Native -FilePath $script:PythonExe -Arguments ($script:PythonPrefix + @($extractor, 'info', $script:InputSource)) -Phase 'extract' -Message 'Validating the disc image.'
        Invoke-Native -FilePath $script:PythonExe -Arguments ($script:PythonPrefix + @($extractor, 'extract', $script:InputSource, $script:ImageStage, '--only', 'default.xbe')) -Phase 'extract' -Message 'Extracting the pinned default.xbe.'
        $xbePath = Get-TopLevelXbe -Folder $script:ImageStage
        Assert-ValidXbe -Path $xbePath
    } else {
        $xbePath = Get-TopLevelXbe -Folder $script:InputSource
    }

    $buildAssets = Join-Path $script:NativeRoot 'assets'
    [IO.Directory]::CreateDirectory($buildAssets) | Out-Null
    [IO.File]::Copy($xbePath, (Join-Path $buildAssets 'default.xbe'), $true)
    $script:GameRoot = $buildAssets

    Write-ProgressStatus -Phase 'configure' -Message 'Configuring the pinned release build.' -Progress 48
    Invoke-Native -FilePath 'cmake.exe' -Arguments @('--preset', 'release') -WorkingDirectory $script:NativeRoot -Phase 'configure' -Message 'Configuring the release build.'

    Write-ProgressStatus -Phase 'compile' -Message 'Compiling the game. The initial native build can take a long time.' -Progress 65
    Invoke-Native -FilePath 'cmake.exe' -Arguments @('--build', 'out/build/release', '--parallel', [string]$Jobs) -WorkingDirectory $script:NativeRoot -Phase 'compile' -Message 'Compiling the game.'

    $staging = Join-Path $gameDirectoryFull ('.install-staging-' + [Guid]::NewGuid().ToString('N'))
    try {
        Write-ProgressStatus -Phase 'install' -Message 'Staging the executable, runtime files, license notices, and local game data.' -Progress 92
        Copy-InstallFiles -BuildDirectory (Join-Path $script:NativeRoot 'out\build\release') -StagingDirectory $staging -InputKind $script:InputKind -SourcePath $script:InputSource
        Install-StagedOutput -StagingDirectory $staging -DestinationDirectory $gameDirectoryFull
        $installedExe = Join-Path $gameDirectoryFull 'FuzionFrenzy.exe'
    }
    finally {
        if (Test-Path -LiteralPath $staging) { [IO.Directory]::Delete($staging, $true) }
        if ($script:ImageStage -and (Test-Path -LiteralPath $script:ImageStage)) { [IO.Directory]::Delete($script:ImageStage, $true) }
    }

    Write-Status -Phase 'complete' -Message 'The native game build is ready.' -Progress 100 -State 'completed' -OutputExe $installedExe
    Write-Output ('Game ready: {0}' -f $installedExe)
    exit 0
}
catch {
    $message = $_.Exception.Message
    if ($script:ImageStage -and (Test-Path -LiteralPath $script:ImageStage)) {
        try { [IO.Directory]::Delete($script:ImageStage, $true) } catch { }
    }
    try { Write-Status -Phase 'error' -Message 'The build did not complete.' -Progress $null -State 'failed' -ErrorMessage $message }
    catch { Write-Output ('Could not write build status: {0}' -f $_.Exception.Message) }
    Write-Error -Message $message
    exit 1
}
