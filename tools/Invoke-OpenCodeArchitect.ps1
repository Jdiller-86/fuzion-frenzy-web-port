param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$worktree = Join-Path $repo '.worktrees\opencode-architect'
$statePath = Join-Path $repo '.worktrees\opencode-architect-cycle-state.json'
$guidancePath = Join-Path $repo '.worktrees\opencode-architect-guidance.json'
$sessionPath = Join-Path $repo '.worktrees\opencode-architect-cycle-session.txt'
$logPath = Join-Path $repo '.worktrees\opencode-architect-cycle.log'
$lockPath = Join-Path $repo '.worktrees\opencode-architect-cycle.lock'

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
    if ($LASTEXITCODE -ne 0 -or $dirty.Count -ne 0) { throw 'Architect audit worktree must remain clean.' }
    $sessionId = $null
    if (Test-Path -LiteralPath $sessionPath) {
        $sessionId = (Get-Content -LiteralPath $sessionPath -Raw).Trim()
        if ($sessionId -notmatch '^ses_[A-Za-z0-9]+$') { throw 'Invalid architect audit session ID.' }
    }
    if ($DryRun) {
        Write-Output ("Ready: worktree=$worktree branch=$branch session=$sessionId")
        return
    }
    git -C $worktree merge --ff-only codex/opencode-source | Out-Null
    if ($LASTEXITCODE -ne 0) { throw 'Could not fast-forward the architect snapshot to committed builder work.' }
    $head = (git -C $worktree rev-parse HEAD).Trim()
    $state = @{ started = (Get-Date).ToString('o'); outcome = 'running'; head = $head; model = 'openai/gpt-6.1-sol'; sessionId = $sessionId }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8

    $prompt = 'Hourly architect audit for Fuzion Frenzy. Read AGENTS.md, docs/OPENCODE_GOAL.md, docs/DEVELOPMENT_HANDOFF.md, docs/PROJECT_STATUS.md, the current checkpoint, the top of DEVELOPMENT_PLAN.md, recent decisions and commits. Review only this clean committed source snapshot. Check that the next milestone advances actual original-game execution in a desktop browser and preserves the full fidelity, local-content and eventual online/social goals. Distinguish observed original execution from source-only components. Identify duplicate work, route drift, evidence gaps, publication problems or a reason to change priority. Return at most three concise actions, including the single next bounded milestone and any blocker; if work is on track, say so. Do not edit files, delegate, run implementation, access ignored private inputs, push, merge or claim playability without a controllable original screen. End with exactly one line beginning ARCHITECT_REVIEW: followed by your concise actionable guidance. The primary builder will verify this guidance against its current worktree before acting.'
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
                    $runPrompt = if ($candidate -eq 'openai/gpt-6.1-sol') { $prompt } else { 'Sol allowance or model availability failed. Give a provisional free-model review, not a Sol decision. ' + $prompt }
                    if ($sessionId) {
                        $runOutput = & opencode run --auto --session $sessionId --agent architect --model $candidate $runPrompt 2>&1
                    } else {
                        $runOutput = & opencode run --auto --agent architect --model $candidate $runPrompt 2>&1
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
    $state.finished = (Get-Date).ToString('o')
    $state.exitCode = $exitCode
    $state.model = $modelUsed
    $state.outcome = if ($exitCode -eq 0) { 'reviewed' } else { 'retry' }
    $response = ($output | Out-String).Trim()
    if ($response.Length -gt 1800) { $response = $response.Substring($response.Length - 1800) }
    $state.lastSummary = $response
    if ($exitCode -eq 0) {
        $match = [regex]::Match($response, '(?m)^ARCHITECT_REVIEW:\s*(.+)$')
        $summary = if ($match.Success) { $match.Groups[1].Value.Trim() } else { $response }
        if ($summary.Length -gt 1200) { $summary = $summary.Substring(0, 1200) }
        $guidance = @{ created = $state.finished; head = $head; model = $modelUsed; summary = $summary }
        $tempPath = $guidancePath + '.tmp'
        $guidance | ConvertTo-Json | Set-Content -LiteralPath $tempPath -Encoding utf8
        Move-Item -LiteralPath $tempPath -Destination $guidancePath -Force
    }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('Architect audit: ' + $state.outcome + ' model=' + $modelUsed)
} finally {
    if ($lock) { $lock.Dispose() }
}
