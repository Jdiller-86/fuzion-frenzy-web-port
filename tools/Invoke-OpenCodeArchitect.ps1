param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
if (-not $env:UNSLOTH_API_KEY) { $env:UNSLOTH_API_KEY = [Environment]::GetEnvironmentVariable('UNSLOTH_API_KEY', 'User') }
$repo = $null
foreach ($rootCandidate in @((Join-Path $PSScriptRoot '..'), (Join-Path $PSScriptRoot '..\..\..'))) {
    $markerWorktree = Join-Path $rootCandidate '.worktrees\opencode-architect'
    $markerModels = Join-Path $rootCandidate 'tools\opencode-builder-models.json'
    if ((Test-Path -LiteralPath $markerWorktree) -and (Test-Path -LiteralPath $markerModels)) { $repo = (Resolve-Path -LiteralPath $rootCandidate).Path; break }
}
if (-not $repo) { throw 'Could not locate the repository root from the script location.' }
$worktree = Join-Path $repo '.worktrees\opencode-architect'
$statePath = Join-Path $repo '.worktrees\opencode-architect-cycle-state.json'
$guidancePath = Join-Path $repo '.worktrees\opencode-architect-guidance.json'
$sessionPath = Join-Path $repo '.worktrees\opencode-architect-cycle-session.txt'
$logPath = Join-Path $repo '.worktrees\opencode-architect-cycle.log'
$lockPath = Join-Path $repo '.worktrees\opencode-architect-cycle.lock'
$prManager = Join-Path $repo 'tools\Invoke-OpenCodeGitHubPR.ps1'
$statusPublisher = Join-Path $repo 'tools\Publish-OpenCodeStatus.ps1'

function Test-SourcePublication([string]$path) {
    $dirty = @(git -C $path status --porcelain)
    if ($LASTEXITCODE -ne 0 -or $dirty.Count -ne 0) { throw 'Integration worktree has unfinished changes.' }
    $changedPaths = @(git -C $path log --format= --name-only origin/main..HEAD | Where-Object { $_ })
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect integration history.' }
    $forbidden = '(^|/)(research|\.tools|node_modules|\.venv|Fuzion Frenzy \(USA\))(/|$)|\.(7z|zip|iso|xbe|xbp|zro|xpr|xwb|xsb|dds|exe|dll|jar|class|pdb|gpr|db|sqlite3?|har|saz|pem|key)$|(^|/)\.env(\.|$)'
    foreach ($changedPath in $changedPaths) { if ($changedPath -match $forbidden) { throw "Forbidden publication path: $changedPath" } }
    Push-Location -LiteralPath $path
    try {
        & npm run check | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Source publication check failed.' }
        git diff --check origin/main..HEAD | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Committed integration whitespace check failed.' }
    } finally { Pop-Location }
}

if (-not (Test-Path -LiteralPath $worktree)) { throw 'Architect audit worktree is missing.' }
$lock = $null
try {
    try {
        $lock = [IO.File]::Open($lockPath, [IO.FileMode]::OpenOrCreate, [IO.FileAccess]::ReadWrite, [IO.FileShare]::None)
    } catch [IO.IOException] {
        Write-Output 'Architect audit already running.'
        return
    }
    $branch = (git -C $worktree branch --show-current).Trim()
    if ($LASTEXITCODE -ne 0 -or $branch -ne 'codex/opencode-architect') { throw 'Unexpected architect audit branch.' }
    $dirty = @(git -C $worktree status --porcelain)
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect architect integration worktree.' }
    $sessionId = $null
    if (Test-Path -LiteralPath $sessionPath) {
        $sessionId = (Get-Content -LiteralPath $sessionPath -Raw).Trim()
        if ($sessionId -notmatch '^ses_[A-Za-z0-9]+$') { throw 'Invalid architect audit session ID.' }
    }
    if ($DryRun) {
        Write-Output ("Ready: worktree=$worktree branch=$branch dirty=$($dirty.Count -gt 0) session=$sessionId")
        return
    }
    $head = (git -C $worktree rev-parse HEAD).Trim()
    $sourceHead = (git -C $repo rev-parse codex/opencode-source).Trim()
    $geometryHead = (git -C $repo rev-parse codex/opencode-geometry).Trim()
    $verificationHead = (git -C $repo rev-parse codex/opencode-verification).Trim()
    $mainHead = (git -C $repo rev-parse main).Trim()
    $modelPool = Get-Content -LiteralPath (Join-Path $repo 'tools\opencode-builder-models.json') -Raw | ConvertFrom-Json
    $architectModels = @($modelPool.hourlyReview)
    if ($architectModels.Count -ne 1 -or $architectModels[0] -ne 'opencode-go/glm-5.3-flash') { throw 'Automatic architect model pool must contain only Go GLM 5.3 Flash.' }
    $state = @{ started = (Get-Date).ToString('o'); outcome = 'running'; head = $head; sourceHead = $sourceHead; geometryHead = $geometryHead; verificationHead = $verificationHead; mainHead = $mainHead; model = $architectModels[0]; sessionId = $sessionId }
    $prSnapshot = 'GitHub PR snapshot unavailable; inspect again before approving a merge.'
    try {
        $snapshotOutput = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $prManager -Action Snapshot 2>&1
        if ($LASTEXITCODE -eq 0) { $prSnapshot = ($snapshotOutput | Out-String).Trim() }
        else { $state.prSnapshotError = 'Could not read GitHub PR state.' }
    } catch { $state.prSnapshotError = $_.Exception.Message }
    $state.prSnapshot = $prSnapshot
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8

    $recentIntegration = (@(git -C $worktree log -4 --pretty=format:'%h %s') | Where-Object { $_ }) -join '; '
    $prompt = "Hourly GLM Flash architect review. Integration=$head; source=$sourceHead; geometry=$geometryHead; verification=$verificationHead; local main=$mainHead. PR snapshot: $prSnapshot. Recent integration commits: $recentIntegration. Read AGENTS.md, docs/OPENCODE_GOAL.md and the TOP of current status/handoff/checkpoint; use Git for exact changed paths. Decide the existing main PR first. If its exact head equals your clean integration HEAD and passes reviewed source/runtime/CI/ancestry/mergeability gates, emit MERGE_MAIN_PR and make no branch edits or new integration commit this cycle; the runner rechecks before merging. Otherwise name the specific gate, then review one committed builder slice for integration. Attribute any new integration commits with git show before describing them as unexplained. Keep the original-game browser route and local content boundary. Use Qwen Scout for one narrow lookup only when useful. Direct each lane to one bounded result or HOLD; prefer the next genuine execution blocker, not repeated documentation or test-only churn. The primary may batch related reached functions under the standing policy, stopping on unsupported service/register/resource or failed checks. Do not push, operate GitHub directly, discard edits, invent evidence, or claim playability. If justified, emit exact 40-character OPEN_SOURCE_REVIEW_PR, OPEN_GEOMETRY_REVIEW_PR or OPEN_MAIN_PR markers; emit MERGE_MAIN_PR only for the exact reviewed clean integration HEAD. End with GEOMETRY_CYCLE: HOLD or RUN, any justified PR markers, then ARCHITECT_REVIEW: Primary: ... Geometry: ... Verification: ... Next integration: ... . Keep the review under 300 words before markers."
    $mainIsIntegrated = $false
    git -C $worktree merge-base --is-ancestor $mainHead $head 2>$null
    if ($LASTEXITCODE -eq 0) { $mainIsIntegrated = $true }
    if (-not $mainIsIntegrated) {
        $prompt = "PR merge prerequisite: local main $mainHead is not in integration HEAD $head. Resolve and integrate local main under lane ownership first; review conflicts and run the relevant gates. Do not emit MERGE_MAIN_PR this cycle. Then give the builders one bounded next task. " + $prompt
    }
    if (Test-Path -LiteralPath $guidancePath) {
        try {
            $priorGuidance = Get-Content -LiteralPath $guidancePath -Raw | ConvertFrom-Json
            if ($priorGuidance.approved -and $priorGuidance.summary) {
                $priorBrief = [string]$priorGuidance.summary
                if ($priorBrief.Length -gt 700) { $priorBrief = $priorBrief.Substring(0, 700) }
                $prompt += " Your last approved directive at $($priorGuidance.created) was: $priorBrief Compare it with current committed builder heads before repeating or revising it."
            }
        } catch { Write-Warning 'Prior architect directive unavailable; continuing current review.' }
    }
    foreach ($lane in @('source', 'geometry', 'verification')) {
        $laneRelativePath = if ($lane -eq 'source') { '.worktrees\opencode-cycle-state.json' } else { '.worktrees\opencode-' + $lane + '-cycle-state.json' }
        $laneStatePath = Join-Path $repo $laneRelativePath
        if (Test-Path -LiteralPath $laneStatePath) {
            try {
                $laneState = Get-Content -LiteralPath $laneStatePath -Raw | ConvertFrom-Json
                $prompt += " Latest $lane cycle metadata (verify against Git; no summary text): outcome=$($laneState.outcome), started=$($laneState.started), finished=$($laneState.finished), attemptedHead=$($laneState.attemptedHead), finishedHead=$($laneState.finishedHead)."
            } catch { Write-Warning "Could not read $lane cycle metadata; continuing current review." }
        }
    }
    if ($dirty.Count -gt 0) { $prompt += ' This integration worktree is dirty from an earlier cycle; finish or resolve those exact changes without resetting them before considering new merges.' }
    $output = @()
    $exitCode = 1
    $modelUsed = $architectModels[0]
    try {
        Push-Location -LiteralPath $worktree
        try {
            if ((Get-Location).Path -ne (Resolve-Path -LiteralPath $worktree).Path -or -not (Test-Path -LiteralPath '.opencode/opencode.jsonc')) { throw 'Architect project configuration is not visible.' }
            $oldPreference = $ErrorActionPreference
            $ErrorActionPreference = 'Continue'
            try {
                # Only Go Flash may act as architect. On a recognized allowance
                # or availability failure, free Flash may give a read-only review.
                $candidates = @($architectModels) + @('opencode/glm-5.3-flash-free')
                foreach ($candidate in $candidates) {
                    $modelUsed = $candidate
                    $runPrompt = if ($candidate -eq 'opencode-go/glm-5.3-flash') { $prompt } else { "Go GLM 5.3 Flash is unavailable; full GLM 5.3 is disabled by user policy. This is the source-only integration branch $head; builder heads are source=$sourceHead, geometry=$geometryHead, verification=$verificationHead. Read AGENTS.md, docs/OPENCODE_GOAL.md and current checkpoints. Give a provisional read-only progress review for all lanes. Do not edit, approve design decisions, merge or request promotion. End with ARCHITECT_REVIEW: followed by provisional advice." }
                    $agent = if ($candidate -eq 'opencode-go/glm-5.3-flash') { 'architect_primary' } else { 'reviewer' }
                    if ($sessionId) {
                        $runOutput = & opencode run --auto --session $sessionId --agent $agent --model $candidate $runPrompt 2>&1
                    } else {
                        $runOutput = & opencode run --auto --agent $agent --model $candidate $runPrompt 2>&1
                    }
                    $exitCode = $LASTEXITCODE
                    if ($exitCode -eq 0 -and $candidate -eq 'opencode-go/glm-5.3-flash' -and (($runOutput | Out-String) -notmatch '(?m)^ARCHITECT_REVIEW:\s*\S')) {
                        $finalPrompt = 'Finish this architect cycle now. No tools or additional investigation. Based on the evidence already checked, state the next bounded Primary and Geometry tasks, acceptance checks, and stop conditions. If a main PR merge was not fully proven, omit MERGE_MAIN_PR. End with GEOMETRY_CYCLE: HOLD or RUN, followed by any already justified exact-head PR markers, then ARCHITECT_REVIEW: Primary: ... Geometry: ... Next integration review: ... .'
                        if ($sessionId) { $finalOutput = & opencode run --auto --session $sessionId --agent $agent --model $candidate $finalPrompt 2>&1 }
                        else { $finalOutput = & opencode run --auto --agent $agent --model $candidate $finalPrompt 2>&1 }
                        $exitCode = $LASTEXITCODE
                        $runOutput = @($runOutput) + @($finalOutput)
                    }
                    $output += @('model=' + $candidate) + @($runOutput)
                    if ($exitCode -eq 0) { break }
                    $failureText = ($runOutput | Out-String)
                    if ($candidate -eq 'opencode/glm-5.3-flash-free' -or $failureText -notmatch '(?i)quota|usage\s+limit|rate\s+limit|allowance|insufficient\s+credits|insufficient\s+balance|\b429\b|model\s+unavailable|model\s+not\s+found') { break }
                }
            } finally { $ErrorActionPreference = $oldPreference }
        } finally { Pop-Location }
    } catch {
        $output += $_.Exception.Message
        $exitCode = 1
    }
    if ((Test-Path -LiteralPath $logPath) -and (Get-Item -LiteralPath $logPath).Length -gt 2097152) { Clear-Content -LiteralPath $logPath }
    Add-Content -LiteralPath $logPath -Value ((Get-Date).ToString('o') + ' exit=' + $exitCode + ' model=' + $modelUsed)
    $output | Add-Content -LiteralPath $logPath
    $after = (git -C $worktree rev-parse HEAD).Trim()
    $dirtyAfter = @(git -C $worktree status --porcelain)
    $state.finished = (Get-Date).ToString('o')
    $state.exitCode = $exitCode
    $state.model = $modelUsed
    $state.finishedHead = $after
    $state.outcome = if ($exitCode -ne 0) { 'retry' } elseif ($dirtyAfter.Count -gt 0) { 'resume' } elseif ($after -ne $head) { 'integrated' } else { 'reviewed' }
    $response = ($output | Out-String).Trim()
    $fullResponse = $response
    if ($response.Length -gt 1800) { $response = $response.Substring($response.Length - 1800) }
    $state.lastSummary = $response
    if ($exitCode -eq 0) {
        $match = [regex]::Match($fullResponse, '(?m)^ARCHITECT_REVIEW:\s*(.+)$')
        $summary = if ($match.Success) { $match.Groups[1].Value.Trim() } else { $response }
        if ($summary.Length -gt 1200) { $summary = $summary.Substring(0, 1200) }
        $geometryCycleMatch = [regex]::Match($fullResponse, '(?m)^GEOMETRY_CYCLE:\s*(HOLD|RUN)\s*$')
        $guidance = @{ created = $state.finished; head = $after; sourceHead = $sourceHead; geometryHead = $geometryHead; model = $modelUsed; approved = ($modelUsed -in $architectModels -and $dirtyAfter.Count -eq 0 -and $match.Success); summary = $summary; geometryCycle = if ($geometryCycleMatch.Success) { $geometryCycleMatch.Groups[1].Value } else { 'RUN' } }
        $tempPath = $guidancePath + '.tmp'
        $guidance | ConvertTo-Json | Set-Content -LiteralPath $tempPath -Encoding utf8
        Move-Item -LiteralPath $tempPath -Destination $guidancePath -Force
    }
    if ($exitCode -eq 0 -and $dirtyAfter.Count -eq 0) {
        try {
            Test-SourcePublication $worktree
            git -C $worktree push origin HEAD:refs/heads/codex/opencode-architect | Out-Null
            if ($LASTEXITCODE -ne 0) { throw 'Could not publish the architect integration branch.' }
            $state.publication = 'published'
        } catch {
            $state.publication = 'retry'
            $state.publicationError = $_.Exception.Message
        }
        if ($guidance.approved -and $state.publication -eq 'published') {
            try {
                $openArgs = @('-Action', 'Ensure', '-ExpectedHead', $after)
                $openMain = [regex]::Match($fullResponse, '(?m)^OPEN_MAIN_PR:\s*([0-9a-f]{40})\s*$')
                if ($openMain.Success -and $openMain.Groups[1].Value -eq $after) { $openArgs += '-OpenMain' }
                $openSource = [regex]::Match($fullResponse, '(?m)^OPEN_SOURCE_REVIEW_PR:\s*([0-9a-f]{40})\s*$')
                if ($openSource.Success -and $openSource.Groups[1].Value -eq $sourceHead) { $openArgs += @('-OpenSourceHead', $sourceHead) }
                $openGeometry = [regex]::Match($fullResponse, '(?m)^OPEN_GEOMETRY_REVIEW_PR:\s*([0-9a-f]{40})\s*$')
                if ($openGeometry.Success -and $openGeometry.Groups[1].Value -eq $geometryHead) { $openArgs += @('-OpenGeometryHead', $geometryHead) }
                if ($openArgs.Count -gt 4) {
                    $prOutput = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $prManager @openArgs 2>&1
                    if ($LASTEXITCODE -ne 0) { throw (($prOutput | Out-String).Trim()) }
                    $state.prManagement = ($prOutput | Out-String).Trim()
                } else {
                    $state.prManagement = 'no_architect_open_request'
                }
            } catch {
                $state.prManagement = 'retry'
                $state.prError = $_.Exception.Message
            }
        }
        $promotion = [regex]::Match($fullResponse, '(?m)^MERGE_MAIN_PR:\s*([0-9a-f]{40})\s*$')
        if ($guidance.approved -and $promotion.Success -and $promotion.Groups[1].Value -eq $after -and $state.publication -eq 'published' -and $state.prManagement -ne 'retry') {
            try {
                $mergeOutput = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $prManager -Action MergeMain -ExpectedHead $after 2>&1
                if ($LASTEXITCODE -ne 0) { throw (($mergeOutput | Out-String).Trim()) }
                $state.promotion = 'merged_pr'
                $state.mergeResult = ($mergeOutput | Out-String).Trim()
            } catch {
                $state.promotion = 'pending_checks_or_review'
                $state.promotionError = $_.Exception.Message
            }
        }
    } elseif ($exitCode -eq 0) {
        $state.publication = 'deferred_unfinished_integration'
    } else {
        $state.publication = 'deferred_failed_review'
    }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    try {
        $statusOutput = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $statusPublisher -IssueNumber 4 2>&1
        if ($LASTEXITCODE -eq 0) { $state.statusPublication = 'updated' }
        else { $state.statusPublication = 'retry'; $state.statusError = (($statusOutput | Out-String).Trim()) }
    } catch {
        $state.statusPublication = 'retry'
        $state.statusError = $_.Exception.Message
    }
    # Manager watchdog (W012): the hourly architect ensures the in-OpenCode
    # orchestrator plugin is always running. A stale heartbeat triggers one
    # direct fallback manager cycle; the result is recorded in cycle state.
    $state.managerWatchdog = 'healthy'
    try {
        $managerHeartbeatPath = Join-Path $repo '.worktrees\opencode-manager-heartbeat.json'
        $heartbeatFresh = $false
        if (Test-Path -LiteralPath $managerHeartbeatPath) {
            $heartbeat = Get-Content -LiteralPath $managerHeartbeatPath -Raw | ConvertFrom-Json
            $beat = [DateTimeOffset]::MinValue
            if ($heartbeat.beat -and [DateTimeOffset]::TryParse([string]$heartbeat.beat, [ref]$beat) -and [DateTimeOffset]::Now.Subtract($beat).TotalMinutes -le 20) { $heartbeatFresh = $true }
        }
        if (-not $heartbeatFresh) {
            $state.managerWatchdog = 'recovering'
            $recoverOutput = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File (Join-Path $worktree 'tools\Invoke-OpenCodeManager.ps1') -Force 2>&1
            $state.managerWatchdog = if ($LASTEXITCODE -eq 0) { 'recovered' } else { 'recovery_failed' }
            $state.managerWatchdogOutput = (($recoverOutput | Out-String).Trim())
        }
    } catch {
        $state.managerWatchdog = 'recovery_error'
        $state.managerWatchdogError = $_.Exception.Message
    }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('Architect audit: ' + $state.outcome + ' model=' + $modelUsed)
} finally {
    if ($lock) { $lock.Dispose() }
}
