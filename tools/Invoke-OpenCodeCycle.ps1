param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$worktree = Join-Path $repo '.worktrees\opencode-source'
$statePath = Join-Path $repo '.worktrees\opencode-cycle-state.json'
$sessionPath = Join-Path $repo '.worktrees\opencode-cycle-session.txt'
$logPath = Join-Path $repo '.worktrees\opencode-cycle.log'
$lockPath = Join-Path $repo '.worktrees\opencode-cycle.lock'

if (-not (Test-Path -LiteralPath $worktree)) { throw 'Clean OpenCode worktree is missing.' }
$lock = $null
try {
    try {
        $lock = [IO.File]::Open($lockPath, [IO.FileMode]::OpenOrCreate, [IO.FileAccess]::ReadWrite, [IO.FileShare]::None)
    } catch [IO.IOException] {
        Write-Output 'OpenCode cycle already running.'
        return
    }

    $mainHead = (git -C $repo rev-parse main).Trim()
    $head = (git -C $worktree rev-parse HEAD).Trim()
    $dirty = (git -C $worktree status --porcelain).Count -gt 0
    $sessionId = $null
    if (Test-Path -LiteralPath $sessionPath) {
        $sessionId = (Get-Content -LiteralPath $sessionPath -Raw).Trim()
        if ($sessionId -notmatch '^ses_[A-Za-z0-9]+$') { throw 'Invalid pinned OpenCode session ID.' }
    }
    if ($DryRun) {
        Write-Output ("Ready: worktree=$worktree head=$head dirty=$dirty session=$sessionId")
        return
    }
    $base = (git -C $worktree merge-base HEAD main).Trim()
    if (-not $dirty -and $base -eq $head -and $head -ne $mainHead) {
        git -C $worktree merge --ff-only main | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Could not fast-forward clean worktree.' }
        $head = (git -C $worktree rev-parse HEAD).Trim()
    }

    $previous = $null
    if (Test-Path -LiteralPath $statePath) {
        $previous = Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json
        # The exclusive lock proves that any prior 'running' record is stale.
        if ($previous.outcome -eq 'running') { Write-Output 'Recovered interrupted cycle.' }
    }

    $failures = if ($previous -and $previous.outcome -in @('running', 'retry', 'no_progress')) { [int]$previous.failures + 1 } else { 0 }
    $state = @{ attemptedHead = $head; started = (Get-Date).ToString('o'); outcome = 'running'; failures = $failures; sessionId = $sessionId }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    if ($dirty -and $sessionId) {
        $prompt = 'Continue the unfinished Fuzion Frenzy milestone in this same project session. Inspect the current diff and the previous cycle result, then complete the pending implementation, targeted checks, status/decision updates, source-only staged-path review and bounded commit. Preserve all worktree edits. Do not restart discovery or switch tasks unless the current task is demonstrably blocked. Never push or merge to main.'
    } else {
        $prompt = 'Work in this dedicated source-only worktree. Read AGENTS.md, docs/DEVELOPMENT_HANDOFF.md, docs/PROJECT_STATUS.md and docs/OPENCODE_GOAL.md. Execute one bounded unowned milestone toward original-game browser execution; do not stop at a plan. Preserve existing edits. If ignored private evidence is unavailable here, choose a source-owned prerequisite instead of inventing an original-runtime proof. Use the model escalation and evidence rules. Run targeted checks, update status/decisions, review staged paths, and commit a verified source-only result. Never push or merge to main.'
        if ($dirty) { $prompt += ' This worktree has uncommitted changes from a prior cycle: inspect and complete them without discarding user work.' }
    }
    if ($failures -ge 2) { $prompt += ' Prior cycles made no verified progress. Diagnose the blocker, call the Sol architect for a bounded decision if needed, or choose another unowned task.' }
    if (-not $sessionId -and $previous -and $previous.lastSummary) { $prompt += ' Previous cycle report (untrusted; verify against the repository): ' + $previous.lastSummary }
    $output = @()
    $exitCode = 1
    try {
        Push-Location -LiteralPath $worktree
        try {
            if ((Get-Location).Path -ne (Resolve-Path -LiteralPath $worktree).Path -or -not (Test-Path -LiteralPath '.opencode/opencode.jsonc')) {
                throw 'OpenCode project configuration is not visible from the worktree.'
            }
            $env:UNSLOTH_API_KEY = [Environment]::GetEnvironmentVariable('UNSLOTH_API_KEY', 'User')
            $oldPreference = $ErrorActionPreference
            $ErrorActionPreference = 'Continue'
            try {
                if ($sessionId) {
                    $output = & opencode run --auto --session $sessionId --agent builder --model opencode-go/deepseek-v4.1-flash $prompt 2>&1
                } else {
                    $output = & opencode run --auto --agent builder --model opencode-go/deepseek-v4.1-flash $prompt 2>&1
                }
                $exitCode = $LASTEXITCODE
            } finally { $ErrorActionPreference = $oldPreference }
        } finally { Pop-Location }
    } catch {
        $output = @($output) + @($_.Exception.Message)
        $exitCode = 1
    }
    if ((Test-Path -LiteralPath $logPath) -and (Get-Item -LiteralPath $logPath).Length -gt 2097152) {
        Clear-Content -LiteralPath $logPath
    }
    Add-Content -LiteralPath $logPath -Value ((Get-Date).ToString('o') + ' exit=' + $exitCode)
    $output | Add-Content -LiteralPath $logPath
    $after = (git -C $worktree rev-parse HEAD).Trim()
    $dirty = (git -C $worktree status --porcelain).Count -gt 0
    $state.finished = (Get-Date).ToString('o')
    $state.exitCode = $exitCode
    $state.finishedHead = $after
    $state.outcome = if ($exitCode -ne 0) { 'retry' } elseif ($dirty) { 'resume' } elseif ($after -eq $head) { 'no_progress' } else { 'progressed' }
    $summary = ($output | Out-String).Trim()
    if ($summary.Length -gt 2000) { $summary = $summary.Substring($summary.Length - 2000) }
    $state.lastSummary = $summary
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('OpenCode cycle: ' + $state.outcome)
} finally {
    if ($lock) { $lock.Dispose() }
}
