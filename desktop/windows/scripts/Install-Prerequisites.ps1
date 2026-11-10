[CmdletBinding()]
param(
    [switch]$Install,
    [switch]$WhatIf
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'

if (-not $Install -and -not $WhatIf) {
    throw 'Prerequisites install only after an explicit user action. Pass -Install from the installer button.'
}

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
    $componentArgs += @('--wait', '--passive', '--norestart')
    if ($WhatIf) {
        Write-Output ('Would run Visual Studio Installer: {0} {1}' -f $setupExe, ($componentArgs -join ' '))
    } else {
        if (-not (Test-Path -LiteralPath $setupExe -PathType Leaf)) { throw 'Visual Studio Installer is missing setup.exe.' }
        Write-Output 'Adding Clang, CMake, and Ninja to the existing Visual Studio installation. The installer will show progress and may request administrator approval.'
        $process = Start-Process -FilePath $setupExe -ArgumentList $componentArgs -Verb RunAs -Wait -PassThru
        if ($process.ExitCode -ne 0) { throw ('Visual Studio Installer failed with exit code {0}.' -f $process.ExitCode) }
    }
    $vsCompletePath = $vsCppPath
}

$python312 = $false
$pyLauncher = Get-Command py.exe -ErrorAction SilentlyContinue
if ($pyLauncher) {
    & $pyLauncher.Source -3.12 --version *> $null
    $python312 = ($LASTEXITCODE -eq 0)
}
if (-not $python312) {
    $pythonCommand = Get-Command python.exe -ErrorAction SilentlyContinue
    if ($pythonCommand) {
        $pythonVersion = & $pythonCommand.Source --version 2>&1
        $python312 = ($LASTEXITCODE -eq 0 -and [string]$pythonVersion -match '^Python 3\.12(?:\.|$)')
    }
}

$packages = @(
    @{ Id = 'Git.Git'; Name = 'Git for Windows'; Present = [bool](Get-Command git.exe -ErrorAction SilentlyContinue) },
    @{ Id = 'Python.Python.3.12'; Name = 'Python 3.12'; Present = $python312 },
    @{ Id = 'Microsoft.VisualStudio.2022.BuildTools'; Name = 'Visual Studio 2022 Build Tools (C++, CMake, Ninja, Clang)'; Present = [bool]$vsCompletePath }
)

foreach ($package in $packages) {
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
    & $wingetCommand.Source @arguments
    if ($LASTEXITCODE -ne 0) { throw ('winget failed to install {0} (exit code {1}).' -f $package.Name, $LASTEXITCODE) }
}

Write-Output 'Prerequisite installation requests have completed. Start a new build to refresh the environment.'
