# Uses the normalized hash and Git helpers from Runtime-Patches.ps1.
function Get-UiPatchLayer {
    param([string]$PatchRoot, [ValidateSet('sdk', 'native')][string]$Layer)
    $manifest = Get-Content -LiteralPath (Join-Path $PatchRoot 'ui-manifest.json') -Raw | ConvertFrom-Json
    return $manifest.$Layer
}

function Restore-KnownUiPatches {
    param([string]$GitExe, [string]$Root, [string]$PatchRoot, [ValidateSet('sdk', 'native')][string]$Layer)
    if (-not (Test-Path -LiteralPath (Join-Path $Root '.git'))) { return }
    $spec = Get-UiPatchLayer -PatchRoot $PatchRoot -Layer $Layer
    $head = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $Root -Arguments @('rev-parse', 'HEAD')
    if (($head -join '').Trim() -ne $spec.commit) { return }
    $tracked = New-Object 'System.Collections.Generic.List[string]'
    $added = New-Object 'System.Collections.Generic.List[string]'
    # Check every path before restoring anything; preserve unknown/staged edits.
    foreach ($entry in $spec.files) {
        $relative = [string]$entry.path
        if ($relative -match '(^|[\\/])\.\.([\\/]|$)' -or [IO.Path]::IsPathRooted($relative)) { throw 'Unsafe UI patch path.' }
        $path = Join-Path $Root $relative
        $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $Root -Arguments @('diff', '--cached', '--exit-code', '--', $relative)
        if (-not (Test-Path -LiteralPath $path -PathType Leaf)) {
            if ($null -ne $entry.beforeSha256) { throw "Missing UI source file: $relative" }
            continue
        }
        $hash = Get-NormalizedRuntimePatchHash -Path $path
        $previousHashes = @()
        if ($entry.PSObject.Properties.Name -contains 'previousAfterSha256') { $previousHashes = @($entry.previousAfterSha256) }
        if ($hash -eq $entry.afterSha256 -or $previousHashes -contains $hash) {
            if ($null -eq $entry.beforeSha256) { $added.Add($path) } else { $tracked.Add($relative) }
        } elseif ($null -ne $entry.beforeSha256 -and $hash -eq $entry.beforeSha256) {
            continue
        } else {
            # SDK d3d12.cpp can also be the pristine pre-rendering-fix source.
            $null = & $GitExe -C $Root diff --quiet -- $relative
            if ($LASTEXITCODE -ne 0 -or $null -eq $entry.beforeSha256) { throw "Unknown UI source edit; preserving $relative" }
        }
    }
    if ($tracked.Count) { $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $Root -Arguments (@('restore', '--worktree', '--') + $tracked.ToArray()) }
    foreach ($path in $added) { [IO.File]::Delete($path) }
}

function Apply-UiPatches {
    param([string]$GitExe, [string]$Root, [string]$PatchRoot, [ValidateSet('sdk', 'native')][string]$Layer)
    $spec = Get-UiPatchLayer -PatchRoot $PatchRoot -Layer $Layer
    Assert-RuntimePatchHead -GitExe $GitExe -SdkRoot $Root -ExpectedCommit $spec.commit
    foreach ($entry in $spec.files) {
        $path = Join-Path $Root $entry.path
        if ($null -eq $entry.beforeSha256) {
            if (Test-Path -LiteralPath $path) { throw "UI patch would overwrite an existing file: $($entry.path)" }
        } elseif ((Get-NormalizedRuntimePatchHash -Path $path) -ne $entry.beforeSha256) { throw "UI patch source mismatch: $($entry.path)" }
    }
    $patch = Join-Path $PatchRoot $spec.patch
    $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $Root -Arguments @('apply', '--check', $patch)
    $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $Root -Arguments @('apply', $patch)
    foreach ($entry in $spec.files) {
        if ((Get-NormalizedRuntimePatchHash -Path (Join-Path $Root $entry.path)) -ne $entry.afterSha256) { throw "UI patch result mismatch: $($entry.path)" }
    }
}
