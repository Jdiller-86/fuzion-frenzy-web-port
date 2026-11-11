Set-StrictMode -Version 2.0

function Get-NormalizedRuntimePatchHash {
    param([Parameter(Mandatory = $true)][string]$Path)

    $text = [IO.File]::ReadAllText($Path, (New-Object Text.UTF8Encoding($false, $true)))
    $normalized = $text.Replace("`r`n", "`n").Replace("`r", "`n")
    $bytes = (New-Object Text.UTF8Encoding($false)).GetBytes($normalized)
    $sha = [Security.Cryptography.SHA256]::Create()
    try { return ([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-', '').ToLowerInvariant() }
    finally { $sha.Dispose() }
}

function Invoke-RuntimePatchGit {
    param([string]$GitExe, [string]$SdkRoot, [string[]]$Arguments)
    $output = & $GitExe -C $SdkRoot @Arguments 2>&1
    if ($LASTEXITCODE -ne 0) {
        throw ('Runtime patch git command failed ({0}): {1}' -f ($Arguments -join ' '), ([string]::Join([Environment]::NewLine, @($output))))
    }
    return @($output)
}

function Get-RuntimePatchManifest {
    param([string]$PatchRoot, [string]$ExpectedCommit)

    $manifestPath = Join-Path $PatchRoot 'manifest.json'
    $patchPath = Join-Path $PatchRoot 'runtime-fixes.patch'
    if (-not (Test-Path -LiteralPath $manifestPath -PathType Leaf) -or -not (Test-Path -LiteralPath $patchPath -PathType Leaf)) {
        throw ('Runtime patch files are missing from {0}.' -f $PatchRoot)
    }
    $manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
    if ([string]$manifest.sdkCommit -ne $ExpectedCommit) { throw 'Runtime patch manifest does not target the pinned SDK revision.' }
    if (-not $manifest.files -or @($manifest.files).Count -eq 0) { throw 'Runtime patch manifest has no file entries.' }
    return $manifest
}

function Get-RuntimePatchFiles {
    param([string]$SdkRoot, $Manifest)

    foreach ($entry in @($Manifest.files)) {
        $relative = [string]$entry.path
        if ([string]::IsNullOrWhiteSpace($relative) -or $relative -match '(^|[\\/])\.\.([\\/]|$)') { throw 'Runtime patch manifest contains an unsafe path.' }
        $path = [IO.Path]::GetFullPath((Join-Path $SdkRoot $relative))
        $root = [IO.Path]::GetFullPath($SdkRoot).TrimEnd([IO.Path]::DirectorySeparatorChar, [IO.Path]::AltDirectorySeparatorChar) + [IO.Path]::DirectorySeparatorChar
        if (-not $path.StartsWith($root, [StringComparison]::OrdinalIgnoreCase)) { throw 'Runtime patch manifest path escapes the SDK checkout.' }
        if ([string]$entry.beforeSha256 -notmatch '^[0-9a-fA-F]{64}$' -or [string]$entry.afterSha256 -notmatch '^[0-9a-fA-F]{64}$') {
            throw 'Runtime patch manifest contains an invalid SHA-256 value.'
        }
        [pscustomobject]@{ RelativePath = $relative.Replace('\', '/'); FullPath = $path; Before = ([string]$entry.beforeSha256).ToLowerInvariant(); After = ([string]$entry.afterSha256).ToLowerInvariant() }
    }
}

function Assert-RuntimePatchHead {
    param([string]$GitExe, [string]$SdkRoot, [string]$ExpectedCommit)
    $head = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $SdkRoot -Arguments @('rev-parse', 'HEAD')
    if ([string]::Join('', $head).Trim() -ne $ExpectedCommit) { throw 'Runtime patches require the exact pinned SDK HEAD.' }
}

function Restore-KnownRuntimePatches {
    param([string]$GitExe, [string]$SdkRoot, [string]$PatchRoot, [string]$ExpectedCommit, [switch]$SkipDifferentHead)

    if (-not (Test-Path -LiteralPath $SdkRoot -PathType Container) -or -not (Test-Path -LiteralPath (Join-Path $SdkRoot '.git'))) { return }
    if ($SkipDifferentHead) {
        $head = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $SdkRoot -Arguments @('rev-parse', 'HEAD')
        # The normal checkout validator owns updates and dirty-tree rejection at other revisions.
        if ([string]::Join('', $head).Trim() -ne $ExpectedCommit) { return }
    }
    $manifest = Get-RuntimePatchManifest -PatchRoot $PatchRoot -ExpectedCommit $ExpectedCommit
    Assert-RuntimePatchHead -GitExe $GitExe -SdkRoot $SdkRoot -ExpectedCommit $ExpectedCommit
    $files = @(Get-RuntimePatchFiles -SdkRoot $SdkRoot -Manifest $manifest)
    $paths = @($files | ForEach-Object { $_.RelativePath })
    $null = & $GitExe -C $SdkRoot diff --cached --quiet -- @paths 2>&1
    if ($LASTEXITCODE -ne 0) { throw 'SDK patch paths have staged changes; preserving checkout.' }
    $restore = New-Object 'System.Collections.Generic.List[string]'
    foreach ($file in $files) {
        if (-not (Test-Path -LiteralPath $file.FullPath -PathType Leaf)) { throw ('Known runtime patch file is missing; preserving checkout: {0}' -f $file.RelativePath) }
        $hash = Get-NormalizedRuntimePatchHash -Path $file.FullPath
        if ($hash -eq $file.After) { $restore.Add($file.RelativePath) }
        elseif ($hash -ne $file.Before) { throw ('SDK source has an unknown edit; preserving checkout: {0}' -f $file.RelativePath) }
    }
    if ($restore.Count -gt 0) {
        $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $SdkRoot -Arguments (@('restore', '--worktree', '--') + @($restore.ToArray()))
        foreach ($file in $files) {
            if ((Get-NormalizedRuntimePatchHash -Path $file.FullPath) -ne $file.Before) { throw ('SDK patch restore did not return the pinned source: {0}' -f $file.RelativePath) }
        }
    }
}

function Apply-RuntimePatches {
    param([string]$GitExe, [string]$SdkRoot, [string]$PatchRoot, [string]$ExpectedCommit)

    $manifest = Get-RuntimePatchManifest -PatchRoot $PatchRoot -ExpectedCommit $ExpectedCommit
    Assert-RuntimePatchHead -GitExe $GitExe -SdkRoot $SdkRoot -ExpectedCommit $ExpectedCommit
    $files = @(Get-RuntimePatchFiles -SdkRoot $SdkRoot -Manifest $manifest)
    $alreadyPatched = 0
    foreach ($file in $files) {
        if (-not (Test-Path -LiteralPath $file.FullPath -PathType Leaf)) { throw ('Pinned SDK source file is missing: {0}' -f $file.RelativePath) }
        $hash = Get-NormalizedRuntimePatchHash -Path $file.FullPath
        if ($hash -eq $file.After) { $alreadyPatched++ }
        elseif ($hash -ne $file.Before) { throw ('SDK source has an unknown edit; preserving checkout: {0}' -f $file.RelativePath) }
    }
    if ($alreadyPatched -eq $files.Count) { throw 'Known SDK runtime patches were not restored before application.' }
    if ($alreadyPatched -ne 0) { throw 'SDK checkout contains a partial runtime patch; preserving checkout.' }

    $patchPath = Join-Path $PatchRoot 'runtime-fixes.patch'
    $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $SdkRoot -Arguments @('apply', '--check', $patchPath)
    $null = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $SdkRoot -Arguments @('apply', $patchPath)
    $changed = Invoke-RuntimePatchGit -GitExe $GitExe -SdkRoot $SdkRoot -Arguments @('diff', '--name-only')
    $expected = @($files | ForEach-Object { $_.RelativePath } | Sort-Object -Unique)
    $actual = @($changed | ForEach-Object { ([string]$_).Trim().Replace('\', '/') } | Where-Object { $_ } | Sort-Object -Unique)
    if (($expected -join "`n") -ne ($actual -join "`n")) { throw 'Applied runtime patch changed paths outside its manifest.' }
    foreach ($file in $files) {
        if ((Get-NormalizedRuntimePatchHash -Path $file.FullPath) -ne $file.After) { throw ('Applied runtime patch failed its post-image hash: {0}' -f $file.RelativePath) }
    }
}
