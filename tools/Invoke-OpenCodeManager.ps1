param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$statePath = Join-Path $repo '.worktrees\opencode-manager-cycle-state.json'
$guidancePath = Join-Path $repo '.worktrees\opencode-manager-guidance.json'
$lockPath = Join-Path $repo '.worktrees\opencode-manager-cycle.lock'
$logPath = Join-Path $repo '.worktrees\opencode-manager-cycle.log'
$pool = Get-Content (Join-Path $repo 'tools\opencode-builder-models.json') -Raw | ConvertFrom-Json
$model = [string]$pool.manager
$builderModels = @($pool.builders.primary.model, $pool.builders.geometry.model, $pool.builders.verification.model)
if (($builderModels | Sort-Object -Unique).Count -ne 3 -or $builderModels -contains $model) { throw 'Builder models must be distinct from one another and from the manager.' }

function Read-Cycle([string]$name) {
    $path = Join-Path $repo ('.worktrees\' + $name + '-cycle-state.json')
    if (-not (Test-Path -LiteralPath $path)) { return $null }
    try { return Get-Content -LiteralPath $path -Raw | ConvertFrom-Json } catch { return $null }
}

$lock = $null
try {
    try { $lock = [IO.File]::Open($lockPath, 'OpenOrCreate', 'ReadWrite', 'None') }
    catch [IO.IOException] { Write-Output 'Manager cycle already running.'; return }
    $heads = @{}
    $dirty = @{}
    foreach ($lane in @('primary', 'geometry', 'verification')) {
        $branch = if ($lane -eq 'primary') { 'codex/opencode-source' } else { 'codex/opencode-' + $lane }
        $heads[$lane] = (git -C $repo rev-parse $branch).Trim()
        if ($LASTEXITCODE -ne 0) { throw "Missing builder branch: $branch" }
        $path = if ($lane -eq 'primary') { Join-Path $repo '.worktrees\opencode-source' } else { Join-Path $repo ('.worktrees\opencode-' + $lane) }
        $dirty[$lane] = @(git -C $path status --porcelain).Count -gt 0
        if ($LASTEXITCODE -ne 0) { throw "Could not inspect builder worktree: $lane" }
    }
    $architectHead = (git -C $repo rev-parse codex/opencode-architect).Trim()
    $architect = $null
    $architectPath = Join-Path $repo '.worktrees\opencode-architect-guidance.json'
    if (Test-Path -LiteralPath $architectPath) { try { $architect = Get-Content $architectPath -Raw | ConvertFrom-Json } catch {} }
    $source = Read-Cycle 'opencode'
    $geometry = Read-Cycle 'opencode-geometry'
    $verification = Read-Cycle 'opencode-verification'
    $prior = $null
    if (Test-Path -LiteralPath $statePath) { try { $prior = Get-Content $statePath -Raw | ConvertFrom-Json } catch {} }
    $snapshot = "architect=$architectHead; primary=$($heads.primary) ($($source.outcome), dirty=$($dirty.primary)); geometry=$($heads.geometry) ($($geometry.outcome), dirty=$($dirty.geometry)); verification=$($heads.verification) ($($verification.outcome), dirty=$($dirty.verification))"
    if ($DryRun) { Write-Output "Ready: manager=$model $snapshot"; return }
    $state = @{ started = (Get-Date).ToString('o'); outcome = 'running'; model = $model; snapshot = $snapshot }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    $architectBrief = if ($architect -and $architect.approved) { [string]$architect.summary } else { 'No approved architect directive; preserve the current original-execution route and integration hold.' }
    if ($architectBrief.Length -gt 900) { $architectBrief = $architectBrief.Substring(0, 900) }
    $prompt = "You are the read-only Fuzion Frenzy delivery manager. Work from source-only metadata and AGENTS.md. $snapshot. Latest approved architect directive: $architectBrief. Assign one bounded task, acceptance check and stop condition to each useful independent lane. Primary owns original startup/runtime; Geometry owns only src/runtime/nv2a_* and its matching tests/checkpoint; Verification owns only source-owned integration/check tooling, matching tests and docs/INTEGRATION_VERIFICATION_CHECKPOINT.md. Follow the architect's hold or route decision; do not override it. Geometry is HOLD if architect geometryCycle is HOLD. Hold any lane without a concrete independent result or with an in-flight dirty worktree. You may not edit, merge or push; builders may edit and commit their owned files. Never claim original playability from component tests. Keep answers short. Output exactly three final single-line markers, each with RUN or HOLD and a task under 300 characters: MANAGER_PRIMARY: RUN|HOLD | task; MANAGER_GEOMETRY: RUN|HOLD | task; MANAGER_VERIFICATION: RUN|HOLD | task."
    $output = @()
    $exitCode = 1
    try {
        Push-Location -LiteralPath $repo
        try {
            $old = $ErrorActionPreference
            $ErrorActionPreference = 'Continue'
            try { $output = @(opencode run --auto --agent manager --model $model --title 'Fuzion Frenzy manager' $prompt 2>&1); $exitCode = $LASTEXITCODE }
            finally { $ErrorActionPreference = $old }
        } finally { Pop-Location }
    } catch { $output += $_.Exception.Message }
    if ((Test-Path $logPath) -and (Get-Item $logPath).Length -gt 2097152) { Clear-Content $logPath }
    Add-Content $logPath ((Get-Date).ToString('o') + ' exit=' + $exitCode)
    $output | Add-Content $logPath
    $response = ($output | Out-String)
    $lanes = @{}
    foreach ($lane in @('primary', 'geometry', 'verification')) {
        $match = [regex]::Match($response, '(?m)^MANAGER_' + $lane.ToUpperInvariant() + ':\s*(RUN|HOLD)\s*\|\s*(.{1,300})\s*$')
        if ($match.Success) { $lanes[$lane] = @{ action = $match.Groups[1].Value; task = $match.Groups[2].Value.Trim(); head = $heads[$lane] } }
    }
    if ($exitCode -eq 0 -and $lanes.Count -eq 3) {
        if ($architect -and $architect.approved -and $architect.geometryCycle -eq 'HOLD') { $lanes.geometry.action = 'HOLD' }
        $guidance = @{ created = (Get-Date).ToString('o'); model = $model; architectHead = $architectHead; lanes = $lanes }
        $temp = $guidancePath + '.tmp'
        $guidance | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $temp -Encoding utf8
        Move-Item -LiteralPath $temp -Destination $guidancePath -Force
        $state.outcome = 'directed'
    } else { $state.outcome = 'retry'; $state.error = 'Manager did not finish all three bounded lane markers.' }
    $state.finished = (Get-Date).ToString('o')
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('Manager cycle: ' + $state.outcome)
} finally { if ($lock) { $lock.Dispose() } }
