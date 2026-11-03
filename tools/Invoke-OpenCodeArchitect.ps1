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
    $state = @{ started = (Get-Date).ToString('o'); outcome = 'running'; head = $head; sourceHead = $sourceHead; geometryHead = $geometryHead; mainHead = $mainHead; model = 'openai/gpt-6.1-sol'; sessionId = $sessionId }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8

    $prompt = "You are the hourly Sol project architect and authority for bounded design decisions, priority changes and reviewed merges. This is the persistent integration branch codex/opencode-architect at $head; committed builder heads are source=$sourceHead, geometry=$geometryHead, local main=$mainHead. Read AGENTS.md, docs/OPENCODE_GOAL.md, docs/DEVELOPMENT_HANDOFF.md, docs/PROJECT_STATUS.md, the current checkpoints, decisions and the corrected top of DEVELOPMENT_PLAN.md. Use git show/diff/log on those branch refs for current facts; your own branch may lag until you merge. Inspect any unfinished integration changes before new work. Compare actual original-game browser execution with the full fidelity, local-content and eventual online/social goal. Before spending Sol steps on broad source reconnaissance, delegate one useful narrow read-only source lookup or commit classification to local Scout (unsloth/qwen3.8-27b), which now has a 32K configured context; give it exact paths, a small answer limit and no private content. Verify material Scout findings yourself. If Scout is unavailable or no useful bounded question exists, continue and report that in the review. Approve or revise consequential design decisions in docs/DECISIONS.md, update governing docs when evidence changes, and send concise priority directives to both builders. You may delegate other narrow work when useful, but do not duplicate the primary or geometry owners. You may merge reviewed committed branches into your integration branch after checking diff, source-only paths, targeted tests and conflicts; never discard edits or force a merge. Keep game files and private research local. Do not push: the runner publishes checked commits. Only when you have integrated all intended changes, including local main governance commits, and reviewed a clean result for public main, end with PROMOTE_MAIN: <your exact 40-character integration HEAD>. Otherwise omit that marker. End with one line ARCHITECT_REVIEW: <at most 700 characters of authoritative priorities for the primary and geometry builders, with evidence limits>. Do not claim playability without a controllable original screen."
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
                foreach ($candidate in @('openai/gpt-6.1-sol', 'opencode/glm-5.3-flash-free')) {
                    $modelUsed = $candidate
                    $runPrompt = if ($candidate -eq 'openai/gpt-6.1-sol') { $prompt } else { "Sol allowance or model availability failed. This is the source-only integration branch $head; builder heads are source=$sourceHead and geometry=$geometryHead. Read AGENTS.md, docs/OPENCODE_GOAL.md, docs/PROJECT_STATUS.md and the latest checkpoints; inspect those committed refs. Give a provisional read-only progress review for both builder lanes. Do not edit, delegate, approve design decisions, merge or request promotion. End with ARCHITECT_REVIEW: followed by your provisional advice." }
                    $agent = if ($candidate -eq 'openai/gpt-6.1-sol') { 'architect' } else { 'reviewer' }
                    if ($sessionId) {
                        $runOutput = & opencode run --auto --session $sessionId --agent $agent --model $candidate $runPrompt 2>&1
                    } else {
                        $runOutput = & opencode run --auto --agent $agent --model $candidate $runPrompt 2>&1
                    }
                    $exitCode = $LASTEXITCODE
                    $output += @('model=' + $candidate) + @($runOutput)
                    if ($exitCode -eq 0) { break }
                    $failureText = ($runOutput | Out-String)
                    if ($candidate -ne 'openai/gpt-6.1-sol' -or $failureText -notmatch '(?i)quota|usage\s+limit|rate\s+limit|allowance|insufficient\s+credits|insufficient\s+balance|\b429\b|model\s+unavailable|model\s+not\s+found') { break }
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
        $guidance = @{ created = $state.finished; head = $after; sourceHead = $sourceHead; geometryHead = $geometryHead; model = $modelUsed; approved = ($modelUsed -eq 'openai/gpt-6.1-sol' -and $dirtyAfter.Count -eq 0 -and $match.Success); summary = $summary }
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
        $promotion = [regex]::Match($response, '(?m)^PROMOTE_MAIN:\s*([0-9a-f]{40})\s*$')
        if ($modelUsed -eq 'openai/gpt-6.1-sol' -and $promotion.Success -and $promotion.Groups[1].Value -eq $after -and $state.publication -eq 'published') {
            try {
                Test-SourcePublication $worktree
                Push-Location -LiteralPath $worktree
                try {
                    & npm run test:runtime | Out-Null
                    if ($LASTEXITCODE -ne 0) { throw 'Runtime source gate failed.' }
                    git fetch origin main | Out-Null
                    if ($LASTEXITCODE -ne 0) { throw 'Could not refresh GitHub main.' }
                    git merge-base --is-ancestor main HEAD
                    if ($LASTEXITCODE -ne 0) { throw 'Local main governance commits are not integrated.' }
                    git merge-base --is-ancestor origin/main HEAD
                    if ($LASTEXITCODE -ne 0) { throw 'GitHub main advanced outside the reviewed integration.' }
                    git push origin HEAD:refs/heads/main | Out-Null
                    if ($LASTEXITCODE -ne 0) { throw 'Reviewed main promotion failed.' }
                } finally { Pop-Location }
                $state.promotion = 'published'
            } catch {
                $state.promotion = 'retry'
                $state.promotionError = $_.Exception.Message
            }
        }
    } elseif ($exitCode -eq 0) {
        $state.publication = 'deferred_unfinished_integration'
    } else {
        $state.publication = 'deferred_failed_review'
    }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('Architect audit: ' + $state.outcome + ' model=' + $modelUsed)
} finally {
    if ($lock) { $lock.Dispose() }
}
