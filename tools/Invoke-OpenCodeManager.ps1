param([switch]$DryRun, [switch]$Force)

# Unified orchestration (W012): this script is the watchdog probe and fallback
# for the in-OpenCode orchestrator plugin (.opencode/plugins/orchestrator).
# While the plugin heartbeat (.worktrees/opencode-manager-heartbeat.json) is
# fresh, the probe exits without any model call. Otherwise it runs the same
# persistent pinned manager session, records the same MANAGER_* lane cards,
# and starts the registered builder lane tasks on demand. The hourly architect
# script invokes it with -Force when it detects a stale heartbeat.
$ErrorActionPreference = 'Stop'
$repo = $null
foreach ($rootCandidate in @((Join-Path $PSScriptRoot '..'), (Join-Path $PSScriptRoot '..\..\..'))) {
    $markerWorktree = Join-Path $rootCandidate '.worktrees\opencode-architect'
    $markerModels = Join-Path $rootCandidate 'tools\opencode-builder-models.json'
    if ((Test-Path -LiteralPath $markerWorktree) -and (Test-Path -LiteralPath $markerModels)) { $repo = (Resolve-Path -LiteralPath $rootCandidate).Path; break }
}
if (-not $repo) { throw 'Could not locate the repository root from the script location.' }
$statePath = Join-Path $repo '.worktrees\opencode-manager-cycle-state.json'
$guidancePath = Join-Path $repo '.worktrees\opencode-manager-guidance.json'
$heartbeatPath = Join-Path $repo '.worktrees\opencode-manager-heartbeat.json'
$sessionPath = Join-Path $repo '.worktrees\opencode-manager-cycle-session.txt'
$lockPath = Join-Path $repo '.worktrees\opencode-manager-cycle.lock'
$logPath = Join-Path $repo '.worktrees\opencode-manager-cycle.log'
$architectWorktree = Join-Path $repo '.worktrees\opencode-architect'
$pool = Get-Content (Join-Path $repo 'tools\opencode-builder-models.json') -Raw | ConvertFrom-Json
$model = [string]$pool.manager
$builderModels = @($pool.builders.primary.model, $pool.builders.geometry.model, $pool.builders.verification.model)
if (($builderModels | Sort-Object -Unique).Count -ne 3 -or $builderModels -contains $model) { throw 'Builder models must be distinct from one another and from the manager.' }
$laneTasks = @{ primary = 'FuzionFrenzy-OpenCode-Cycle'; geometry = 'FuzionFrenzy-OpenCode-Geometry'; verification = 'FuzionFrenzy-OpenCode-Verification' }

function Read-Cycle([string]$name) {
    $path = Join-Path $repo ('.worktrees\' + $name + '-cycle-state.json')
    if (-not (Test-Path -LiteralPath $path)) { return $null }
    try { return Get-Content -LiteralPath $path -Raw | ConvertFrom-Json } catch { return $null }
}

$lock = $null
try {
    try { $lock = [IO.File]::Open($lockPath, 'OpenOrCreate', 'ReadWrite', 'None') }
    catch [IO.IOException] { Write-Output 'Manager cycle already running.'; return }
    if (-not $Force -and (Test-Path -LiteralPath $heartbeatPath)) {
        try {
            $heartbeat = Get-Content -LiteralPath $heartbeatPath -Raw | ConvertFrom-Json
            $beat = [DateTimeOffset]::MinValue
            if ($heartbeat.beat -and [string]$heartbeat.outcome -ne 'marker_error' -and [DateTimeOffset]::TryParse([string]$heartbeat.beat, [ref]$beat) -and [DateTimeOffset]::Now.Subtract($beat).TotalMinutes -le 12) {
                Write-Output 'Manager plugin heartbeat is fresh; watchdog probe exits without a model call.'
                return
            }
        } catch { Write-Warning 'Could not read the manager heartbeat; running the fallback manager cycle.' }
    }
    $heads = @{}
    foreach ($lane in @('primary', 'geometry', 'verification')) {
        $branch = if ($lane -eq 'primary') { 'codex/opencode-source' } else { 'codex/opencode-' + $lane }
        $heads[$lane] = (git -C $repo rev-parse $branch).Trim()
        if ($LASTEXITCODE -ne 0) { throw "Missing builder branch: $branch" }
    }
    $architectHead = (git -C $repo rev-parse codex/opencode-architect).Trim()
    $architect = $null
    $architectPath = Join-Path $repo '.worktrees\opencode-architect-guidance.json'
    if (Test-Path -LiteralPath $architectPath) { try { $architect = Get-Content -LiteralPath $architectPath -Raw | ConvertFrom-Json } catch {} }
    $previousGuidance = $null
    if (Test-Path -LiteralPath $guidancePath) { try { $previousGuidance = Get-Content -LiteralPath $guidancePath -Raw | ConvertFrom-Json } catch {} }
    $source = Read-Cycle 'opencode'
    $geometry = Read-Cycle 'opencode-geometry'
    $verification = Read-Cycle 'opencode-verification'
    $sessionId = $null
    if (Test-Path -LiteralPath $sessionPath) {
        $sessionId = (Get-Content -LiteralPath $sessionPath -Raw).Trim()
        if ($sessionId -notmatch '^ses_[A-Za-z0-9]+$') { throw 'Invalid pinned manager session ID.' }
    }
    $snapshot = "architect=$architectHead; primary=$($heads.primary) ($($source.outcome)); geometry=$($heads.geometry) ($($geometry.outcome)); verification=$($heads.verification) ($($verification.outcome))"
    if ($DryRun) { Write-Output "Ready: manager=$model $snapshot session=$sessionId force=$Force"; return }
    $state = @{ started = (Get-Date).ToString('o'); outcome = 'running'; model = $model; snapshot = $snapshot; forced = [bool]$Force }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    $architectBrief = if ($architect -and $architect.approved) { [string]$architect.summary } else { 'No approved architect directive; preserve the current original-execution route and integration hold.' }
    if ($architectBrief.Length -gt 900) { $architectBrief = $architectBrief.Substring(0, 900) }
    $priorBrief = if ($previousGuidance) { "Previous manager tasks: primary $($previousGuidance.lanes.primary.head) $($previousGuidance.lanes.primary.action) $($previousGuidance.lanes.primary.task); geometry $($previousGuidance.lanes.geometry.head) $($previousGuidance.lanes.geometry.action); verification $($previousGuidance.lanes.verification.head) $($previousGuidance.lanes.verification.action) $($previousGuidance.lanes.verification.task)." } else { '' }
    if ($priorBrief.Length -gt 900) { $priorBrief = $priorBrief.Substring(0, 900) }
    $checkpointPath = Join-Path $repo '.worktrees\opencode-source\docs\PROJECT_STATUS.md'
    $checkpoint = if (Test-Path -LiteralPath $checkpointPath) { ((Get-Content -LiteralPath $checkpointPath -TotalCount 7) -join ' ') -replace '\s+', ' ' } else { 'checkpoint unavailable' }
    if ($checkpoint.Length -gt 520) { $checkpoint = $checkpoint.Substring(0, 520) }
    $prompt = "Manager watchdog fallback. $snapshot. Source checkpoint: $checkpoint. Architect directive: $architectBrief. $priorBrief Assign the next useful bounded result from the CURRENT checkpoint; if a previous task is complete or its head advanced, do not reissue it. Dirty lanes continue their edits. HOLD geometry when the architect says HOLD or reference evidence is absent; HOLD verification without a concrete gate task. Follow lane ownership and architect route decisions in AGENTS.md; do not edit, merge, push or claim playability. Reply with ONLY three single-line markers (task <=300 characters, include acceptance and stop): MANAGER_PRIMARY: RUN|HOLD | task; MANAGER_GEOMETRY: RUN|HOLD | task; MANAGER_VERIFICATION: RUN|HOLD | task."

    $output = @()
    $exitCode = 1
    try {
        Push-Location -LiteralPath $architectWorktree
        try {
            if (-not (Test-Path -LiteralPath '.opencode\opencode.jsonc')) { throw 'Architect worktree project configuration is not visible.' }
            $old = $ErrorActionPreference
            $ErrorActionPreference = 'Continue'
            try {
                if ($sessionId) { $output = @(opencode run --auto --session $sessionId --agent manager --model $model --title 'Fuzion Frenzy manager' $prompt 2>&1) }
                else { $output = @(opencode run --auto --agent manager --model $model --title 'Fuzion Frenzy manager' $prompt 2>&1) }
                $exitCode = $LASTEXITCODE
            } finally { $ErrorActionPreference = $old }
        } finally { Pop-Location }
    } catch { $output += $_.Exception.Message }
    if ((Test-Path -LiteralPath $logPath) -and (Get-Item -LiteralPath $logPath).Length -gt 2097152) { Clear-Content -LiteralPath $logPath }
    Add-Content $logPath ((Get-Date).ToString('o') + ' exit=' + $exitCode)
    $output | Add-Content -LiteralPath $logPath
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
        $state.spawned = @()
        foreach ($lane in @('primary', 'geometry', 'verification')) {
            if ($lanes[$lane].action -ne 'RUN') { continue }
            $task = Get-ScheduledTask -TaskName $laneTasks[$lane] -ErrorAction SilentlyContinue
            if ($task) { Start-ScheduledTask -TaskName $laneTasks[$lane]; $state.spawned += $lane }
            else { $state.spawnError = "Missing lane task: $($laneTasks[$lane])" }
        }
        $heartbeat = @{ beat = (Get-Date).ToString('o'); outcome = 'directed'; source = 'powershell'; spawned = $state.spawned }
        $heartbeat | ConvertTo-Json | Set-Content -LiteralPath $heartbeatPath -Encoding utf8
        $state.outcome = 'directed'
    } else {
        $state.outcome = 'retry'
        $state.error = 'Manager did not finish all three bounded lane markers.'
    }
    $state.finished = (Get-Date).ToString('o')
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('Manager cycle: ' + $state.outcome)
} finally { if ($lock) { $lock.Dispose() } }