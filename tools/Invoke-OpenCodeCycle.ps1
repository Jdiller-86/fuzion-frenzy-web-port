param()

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$worktree = Join-Path $repo '.worktrees\opencode-source'
$statePath = Join-Path $repo '.worktrees\opencode-cycle-state.json'
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

    if ((git -C $repo status --porcelain).Count -gt 0) {
        Write-Output 'Waiting: main checkout has in-progress edits.'
        return
    }
    if ((git -C $worktree status --porcelain).Count -gt 0) {
        Write-Output 'Paused: OpenCode worktree has uncommitted edits.'
        return
    }

    $mainHead = (git -C $repo rev-parse main).Trim()
    $head = (git -C $worktree rev-parse HEAD).Trim()
    $base = (git -C $worktree merge-base HEAD main).Trim()
    if ($base -eq $head -and $head -ne $mainHead) {
        git -C $worktree merge --ff-only main | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Could not fast-forward clean worktree.' }
        $head = (git -C $worktree rev-parse HEAD).Trim()
    } elseif ($base -ne $mainHead) {
        Write-Output 'Paused: main and OpenCode worktree diverged.'
        return
    }

    if (Test-Path -LiteralPath $statePath) {
        $previous = Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json
        # A run that was killed (machine sleep, crash, forced stop) leaves
        # outcome=running. Treat an old running record as recoverable so the
        # loop does not stay wedged forever; a recent one still blocks.
        $staleRunning = $false
        if ($previous.outcome -eq 'running') {
            try { $staleRunning = ((Get-Date) - [datetime]::Parse($previous.started)).TotalMinutes -ge 45 }
            catch { $staleRunning = $true }
        }
        if ($staleRunning) {
            Write-Output 'Recovered: stale running record from an interrupted attempt.'
        } elseif ($previous.attemptedHead -eq $head -and $previous.outcome -ne 'progressed') {
            Write-Output 'Paused: no new commit or input since the last attempt.'
            return
        }
    }

    $state = @{ attemptedHead = $head; started = (Get-Date).ToString('o'); outcome = 'running' }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    $env:UNSLOTH_API_KEY = [Environment]::GetEnvironmentVariable('UNSLOTH_API_KEY', 'User')
    $prompt = 'Read AGENTS.md and docs/OPENCODE_GOAL.md. Advance exactly one unowned bounded milestone toward original-game browser execution. Use the escalation and evidence rules. Run targeted checks, update status/decisions, review staged paths, and commit only a verified source-only result. Stop without a commit if blocked. Never push or merge to main.'
    $output = & opencode run --standalone --auto --agent builder --model opencode-go/deepseek-v4.1-flash $prompt 2>&1
    $exitCode = $LASTEXITCODE
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
    $state.outcome = if ($exitCode -ne 0 -or $dirty) { 'paused' } elseif ($after -eq $head) { 'no_progress' } else { 'progressed' }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('OpenCode cycle: ' + $state.outcome)
} finally {
    if ($lock) { $lock.Dispose() }
}
