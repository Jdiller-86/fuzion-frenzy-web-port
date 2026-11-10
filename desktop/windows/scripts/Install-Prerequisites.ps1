[CmdletBinding()]
param(
    [switch]$Install,
    [switch]$WhatIf,
    [string]$LogPath = (Join-Path $env:LOCALAPPDATA 'FuzionFrenzy\logs\prerequisites.log')
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'

if (-not $Install -and -not $WhatIf) {
    throw 'Prerequisites install only after an explicit user action. Pass -Install from the installer button.'
}

$transcribing = $false
try {
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName([IO.Path]::GetFullPath($LogPath))) | Out-Null
Start-Transcript -LiteralPath $LogPath -Append | Out-Null
$transcribing = $true
Write-Output ('Prerequisite log: {0}' -f $LogPath)

$wingetCommand = Get-Command winget.exe -ErrorAction SilentlyContinue
if (-not $wingetCommand) { throw 'Windows Package Manager (winget) is required. Install or update App Installer from Microsoft Store, then retry.' }

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$setupExe = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\setup.exe'
$vsComponents = @(
    'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
    'Microsoft.VisualStudio.Component.VC.Llvm.Clang',
    'Microsoft.VisualStudio.Component.VC.Llvm.ClangToolset',
    'Microsoft.VisualStudio.Component.VC.CMake.Project'
)
$vsCppPath = $null
$vsCompletePath = $null
if (Test-Path -LiteralPath $vswhere -PathType Leaf) {
    $vsCppResult = @(& $vswhere -latest -products '*' -requires $vsComponents[0] -property installationPath 2>$null)
    if ($LASTEXITCODE -eq 0 -and $vsCppResult.Count -gt 0) { $vsCppPath = [string]$vsCppResult[0] }
    $vsCompleteResult = @(& $vswhere -latest -products '*' -requires $vsComponents -property installationPath 2>$null)
    if ($LASTEXITCODE -eq 0 -and $vsCompleteResult.Count -gt 0) { $vsCompletePath = [string]$vsCompleteResult[0] }
}

if ($vsCppPath -and -not $vsCompletePath) {
    $componentArgs = @('modify', '--installPath', ('"{0}"' -f $vsCppPath))
    foreach ($component in $vsComponents[1..($vsComponents.Count - 1)]) { $componentArgs += @('--add', $component) }
    # --wait is a bootstrapper option; the installed setup.exe rejects it.
    $componentArgs += @('--passive', '--norestart')
    if ($WhatIf) {
        Write-Output ('Would run Visual Studio Installer: {0} {1}' -f $setupExe, ($componentArgs -join ' '))
    } else {
        if (-not (Test-Path -LiteralPath $setupExe -PathType Leaf)) { throw 'Visual Studio Installer is missing setup.exe.' }
        Write-Output 'Adding Clang, CMake, and Ninja to the existing Visual Studio installation. The installer will show progress and may request administrator approval.'
        $process = Start-Process -FilePath $setupExe -ArgumentList $componentArgs -Verb RunAs -Wait -PassThru
        if ($process.ExitCode -notin @(0, 3010)) { throw ('Visual Studio Installer failed with exit code {0}.' -f $process.ExitCode) }
        if ($process.ExitCode -eq 3010) { Write-Output 'Visual Studio requested a restart. Restart Windows before building.' }
    }
    $vsCompletePath = $vsCppPath
}

$python312 = $false
$pyLauncher = Get-Command py.exe -ErrorAction SilentlyContinue
if ($pyLauncher) {
    $probePreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    & $pyLauncher.Source -3.12 --version *> $null
    $python312 = ($LASTEXITCODE -eq 0)
    $ErrorActionPreference = $probePreference
}
if (-not $python312) {
    $pythonCommand = Get-Command python.exe -ErrorAction SilentlyContinue
    if ($pythonCommand) {
        $probePreference = $ErrorActionPreference
        $ErrorActionPreference = 'Continue'
        $pythonVersion = & $pythonCommand.Source --version 2>&1
        $python312 = ($LASTEXITCODE -eq 0 -and [string]$pythonVersion -match '^Python 3\.12(?:\.|$)')
        $ErrorActionPreference = $probePreference
    }
}

$llvmInstalled = (Test-Path -LiteralPath (Join-Path $env:ProgramFiles 'LLVM\bin\clang.exe'))
if (-not $llvmInstalled -and $vsCppPath) {
    $llvmInstalled = Test-Path -LiteralPath (Join-Path $vsCppPath 'VC\Tools\Llvm\x64\bin\clang.exe')
}
$packages = @(
    @{ Id = 'Git.Git'; Name = 'Git for Windows'; Present = [bool](Get-Command git.exe -ErrorAction SilentlyContinue) },
    @{ Id = 'Python.Python.3.12'; Name = 'Python 3.12'; Present = $python312 },
    @{ Id = 'Microsoft.VisualStudio.2022.BuildTools'; Name = 'Visual Studio 2022 Build Tools (C++, CMake, Ninja, Clang)'; Present = [bool]$vsCompletePath },
    @{ Id = 'LLVM.LLVM'; Name = 'LLVM Clang compiler'; Present = [bool]$llvmInstalled }
)

foreach ($package in $packages) {
    if ($package.Id -eq 'LLVM.LLVM') {
        # A preceding Visual Studio install may already have provided Clang.
        $clangRoots = @((Join-Path $env:ProgramFiles 'LLVM\bin'))
        if (Test-Path -LiteralPath $vswhere) {
            $currentVs = @(& $vswhere -latest -products '*' -requires $vsComponents[0] -property installationPath)
            if ($currentVs.Count) { $clangRoots += Join-Path ([string]$currentVs[0]) 'VC\Tools\Llvm\x64\bin' }
        }
        $availableClang = @($clangRoots | Where-Object {
            (Test-Path -LiteralPath (Join-Path $_ 'clang.exe')) -and (Test-Path -LiteralPath (Join-Path $_ 'clang++.exe'))
        })
        $package.Present = ($availableClang.Count -gt 0)
    }
    if ($package.Present) {
        Write-Output ('Detected {0}.' -f $package.Name)
        continue
    }
    $arguments = @('install', '--id', $package.Id, '--exact', '--accept-source-agreements', '--accept-package-agreements', '--interactive')
    if ($package.Id -eq 'Microsoft.VisualStudio.2022.BuildTools') {
        $override = '--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --add Microsoft.VisualStudio.Component.VC.Llvm.Clang --add Microsoft.VisualStudio.Component.VC.Llvm.ClangToolset --add Microsoft.VisualStudio.Component.VC.CMake.Project'
        $arguments += @('--override', $override)
    }
    if ($WhatIf) {
        Write-Output ('Would run: winget {0}' -f ($arguments -join ' '))
        continue
    }
    Write-Output ('Installing {0}. The package installer will show its progress and may request administrator approval.' -f $package.Name)
    $nativePreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    & $wingetCommand.Source @arguments 2>&1 | ForEach-Object { Write-Output ([string]$_) }
    $installExit = $LASTEXITCODE
    $ErrorActionPreference = $nativePreference
    if ($installExit -notin @(0, 3010)) { throw ('winget failed to install {0} (exit code {1}).' -f $package.Name, $installExit) }
}

Write-Output 'Prerequisite installation requests have completed. Start a new build to refresh the environment.'
}
catch {
    Write-Output ('Prerequisite installation failed: {0}' -f $_.Exception.Message)
    Write-Output ('Full log: {0}' -f $LogPath)
    exit 1
}
finally {
    if ($transcribing) { Stop-Transcript | Out-Null }
}
