param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
if (-not $env:UNSLOTH_API_KEY) { $env:UNSLOTH_API_KEY = [Environment]::GetEnvironmentVariable('UNSLOTH_API_KEY', 'User') }
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
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
    $mainHead = (git -C $repo rev-parse main).Trim()
    $solCooldownUntil = [DateTimeOffset]::MinValue
    if (Test-Path -LiteralPath $statePath) {
        try {
            $previousState = Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json
            if ($previousState.solCooldownUntil) {
                $parsedCooldown = [DateTimeOffset]::MinValue
                if ([DateTimeOffset]::TryParse([string]$previousState.solCooldownUntil, [ref]$parsedCooldown)) { $solCooldownUntil = $parsedCooldown }
            }
        } catch { Write-Warning 'Previous architect state unavailable; checking Sol normally.' }
    }
    $state = @{ started = (Get-Date).ToString('o'); outcome = 'running'; head = $head; sourceHead = $sourceHead; geometryHead = $geometryHead; mainHead = $mainHead; model = 'openai/gpt-6.1-sol'; sessionId = $sessionId }
    if ($solCooldownUntil -gt [DateTimeOffset]::Now) { $state.solCooldownUntil = $solCooldownUntil.ToString('o') }
    $prSnapshot = 'GitHub PR snapshot unavailable; inspect again before approving a merge.'
    try {
        $snapshotOutput = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $prManager -Action Snapshot 2>&1
        if ($LASTEXITCODE -eq 0) { $prSnapshot = ($snapshotOutput | Out-String).Trim() }
        else { $state.prSnapshotError = 'Could not read GitHub PR state.' }
    } catch { $state.prSnapshotError = $_.Exception.Message }
    $state.prSnapshot = $prSnapshot
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8

    $prompt = "You are the hourly Sol project architect and authority for bounded design decisions, priority changes, reviewed merges and GitHub PR management. This is the persistent integration branch codex/opencode-architect at $head; committed builder heads are source=$sourceHead, geometry=$geometryHead, local main=$mainHead. GitHub PR snapshot (metadata only; verify current state): $prSnapshot. Read AGENTS.md, docs/OPENCODE_GOAL.md, docs/DEVELOPMENT_HANDOFF.md, docs/PROJECT_STATUS.md, the current checkpoints, decisions and the corrected top of DEVELOPMENT_PLAN.md. Use git show/diff/log on those branch refs for current facts; your own branch may lag until you merge. Inspect any unfinished integration changes before new work. Each cycle decide whether committed source/geometry work merits a draft review PR into the integration branch, whether a clean integration head merits a PR into main, and whether an existing main PR is ready to merge. You must initiate PR creation yourself with exact markers at the end of your answer: OPEN_SOURCE_REVIEW_PR: <exact committed source HEAD>, OPEN_GEOMETRY_REVIEW_PR: <exact committed geometry HEAD>, or OPEN_MAIN_PR: <exact clean integration HEAD>. Emit only the markers you approve; omit any already-open PR. The runner is an authenticated actuator that rechecks and executes your requests; it never decides to open PRs on its own. Do not use shell commands to create or merge PRs. Compare actual original-game browser execution with the full fidelity, local-content and eventual online/social goal. Before spending Sol steps on broad source reconnaissance, delegate one useful narrow read-only source lookup or commit classification to local Scout (unsloth/qwen3.8-27b), which now has a 32K configured context; give it exact paths, a small answer limit and no private content. Verify material Scout findings yourself. If Scout is unavailable or no useful bounded question exists, continue and report that in the review. Approve or revise consequential design decisions in docs/DECISIONS.md, update governing docs when evidence changes, and send concise priority directives to both builders. You may delegate other narrow work when useful, but do not duplicate the primary or geometry owners. You may merge reviewed committed branches into your integration branch after checking diff, source-only paths, targeted tests and conflicts; never discard edits or force a merge. Keep game files and private research local. Do not push: the runner publishes checked commits. Only if the exact clean integration HEAD is fully reviewed for main, all intended commits including local main are integrated, and GitHub checks and PR status can pass, end with MERGE_MAIN_PR: <your exact 40-character integration HEAD>. The runner will recheck and merge that PR without a direct push. Otherwise omit the marker and state the blocker. Put all PR markers immediately before the final ARCHITECT_REVIEW: <at most 700 characters of authoritative priorities for the primary and geometry builders, with evidence limits>. Do not claim playability without a controllable original screen."
    $prompt += ' Make integration a concrete hourly deliverable. Review the current exact head of the existing main PR independently from unmerged builder branches: their conflicts or unfinished work do not by themselves block a previously reviewed integration head. If that PR head is fully reviewed, source-only, current with local main, and passes its exact-head checks, request MERGE_MAIN_PR for it; otherwise identify the specific unreviewed paths or failing gate on that PR head. Also inspect the oldest bounded committed builder change not yet integrated. If an independent slice can be reviewed and integrated safely, cherry-pick or merge that slice into this branch, run its targeted checks and record the accepted evidence; do not wait for an entire growing builder branch to become reviewable. If no slice qualifies, name the exact reason rather than adding another general review-only checkpoint. Never use a main merge marker to bypass the existing source, runtime, CI, ancestry or GitHub protections.'
    $prompt += ' Actively manage both builders: compare their latest commits and outcomes with your last directive, identify task drift, repeated low-value documentation or fixture churn, and any stalled critical path. Give each active lane one specific next bounded result, acceptance check and stop condition. Redirect work when evidence changes; do not let a lane keep making unrelated prerequisites without a concrete link to the original-browser-execution milestone. If geometry lacks actionable reference evidence, emit GEOMETRY_CYCLE: HOLD before any PR markers; the runner will defer its hourly prompt to save free-model usage while primary continues. Otherwise emit GEOMETRY_CYCLE: RUN with a concrete geometry task. Keep any PR markers immediately before the final ARCHITECT_REVIEW as already specified. In ARCHITECT_REVIEW, lead with Primary: and Geometry: directives that builders can execute, and state exactly what you will review for integration next hour. A held lane is a resource decision, never a pause of the overall goal.'
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
    foreach ($lane in @('source', 'geometry')) {
        $laneRelativePath = if ($lane -eq 'source') { '.worktrees\opencode-cycle-state.json' } else { '.worktrees\opencode-geometry-cycle-state.json' }
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
    $modelUsed = 'openai/gpt-6.1-sol'
    try {
        Push-Location -LiteralPath $worktree
        try {
            if ((Get-Location).Path -ne (Resolve-Path -LiteralPath $worktree).Path -or -not (Test-Path -LiteralPath '.opencode/opencode.jsonc')) { throw 'Architect project configuration is not visible.' }
            $oldPreference = $ErrorActionPreference
            $ErrorActionPreference = 'Continue'
            try {
                $candidates = if ($solCooldownUntil -gt [DateTimeOffset]::Now) { @('opencode-go/glm-5.3-flash', 'opencode/glm-5.3-flash-free') } else { @('openai/gpt-6.1-sol', 'opencode-go/glm-5.3-flash', 'opencode/glm-5.3-flash-free') }
                foreach ($candidate in $candidates) {
                    $modelUsed = $candidate
                    $runPrompt = if ($candidate -eq 'opencode-go/glm-5.3-flash') { "Linked Sol is usage-limited. Act as the authorized budgeted architect under AGENTS.md and docs/OPENCODE_GOAL.md. Integration HEAD=$head; primary=$sourceHead; geometry=$geometryHead; local main=$mainHead. PR snapshot: $prSnapshot. Review the latest committed evidence and prior directive. Finish one bounded integration or management decision; check the existing main PR independently of unfinished builder work. Use Qwen Scout once for a narrow source question if useful. Give Primary and Geometry one concrete next task, acceptance check, and stop condition. If geometry has no actionable reference, choose HOLD. Preserve source-only, original-browser-execution, exact-head, runtime, CI, ancestry, and mergeability gates. Never request a merge from metadata alone. Do not push or operate GitHub directly. Conclude with GEOMETRY_CYCLE: HOLD or RUN; then only justified exact-head OPEN_*_PR or MERGE_MAIN_PR markers; and a final ARCHITECT_REVIEW: Primary: ... Geometry: ... Next integration review: ... . Reserve steps to finish this directive. Do not start another tool action after deciding the final directive." } elseif ($candidate -eq 'openai/gpt-6.1-sol') { $prompt } else { "Sol and the budgeted Go architect are unavailable. This is the source-only integration branch $head; builder heads are source=$sourceHead and geometry=$geometryHead. Read AGENTS.md, docs/OPENCODE_GOAL.md, docs/PROJECT_STATUS.md and the latest checkpoints; inspect those committed refs. Give a provisional read-only progress review for both builder lanes. Do not edit, delegate, approve design decisions, merge or request promotion. End with ARCHITECT_REVIEW: followed by your provisional advice." }
                    $agent = if ($candidate -eq 'openai/gpt-6.1-sol') { 'architect' } elseif ($candidate -eq 'opencode-go/glm-5.3-flash') { 'architect_fallback' } else { 'reviewer' }
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
                    if ($candidate -eq 'openai/gpt-6.1-sol' -and $failureText -match '(?i)ChatGPT usage limit|subscription_sharing_usage_limit_exceeded') {
                        $solCooldownUntil = [DateTimeOffset]::Now.AddHours(6)
                        $state.solCooldownUntil = $solCooldownUntil.ToString('o')
                    }
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
    if ($response.Length -gt 1800) { $response = $response.Substring($response.Length - 1800) }
    $state.lastSummary = $response
    if ($exitCode -eq 0) {
        $match = [regex]::Match($response, '(?m)^ARCHITECT_REVIEW:\s*(.+)$')
        $summary = if ($match.Success) { $match.Groups[1].Value.Trim() } else { $response }
        if ($summary.Length -gt 1200) { $summary = $summary.Substring(0, 1200) }
        $geometryCycleMatch = [regex]::Match($response, '(?m)^GEOMETRY_CYCLE:\s*(HOLD|RUN)\s*$')
        $guidance = @{ created = $state.finished; head = $after; sourceHead = $sourceHead; geometryHead = $geometryHead; model = $modelUsed; approved = ($modelUsed -in @('openai/gpt-6.1-sol', 'opencode-go/glm-5.3-flash') -and $dirtyAfter.Count -eq 0 -and $match.Success); summary = $summary; geometryCycle = if ($geometryCycleMatch.Success) { $geometryCycleMatch.Groups[1].Value } else { 'RUN' } }
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
                $openMain = [regex]::Match($response, '(?m)^OPEN_MAIN_PR:\s*([0-9a-f]{40})\s*$')
                if ($openMain.Success -and $openMain.Groups[1].Value -eq $after) { $openArgs += '-OpenMain' }
                $openSource = [regex]::Match($response, '(?m)^OPEN_SOURCE_REVIEW_PR:\s*([0-9a-f]{40})\s*$')
                if ($openSource.Success -and $openSource.Groups[1].Value -eq $sourceHead) { $openArgs += @('-OpenSourceHead', $sourceHead) }
                $openGeometry = [regex]::Match($response, '(?m)^OPEN_GEOMETRY_REVIEW_PR:\s*([0-9a-f]{40})\s*$')
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
        $promotion = [regex]::Match($response, '(?m)^MERGE_MAIN_PR:\s*([0-9a-f]{40})\s*$')
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
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('Architect audit: ' + $state.outcome + ' model=' + $modelUsed)
} finally {
    if ($lock) { $lock.Dispose() }
}
