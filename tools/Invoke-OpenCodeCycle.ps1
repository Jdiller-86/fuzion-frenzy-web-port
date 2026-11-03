param(
    [switch]$DryRun,
    [ValidateSet('primary', 'geometry')][string]$Lane = 'primary'
)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$laneName = if ($Lane -eq 'geometry') { 'opencode-geometry' } else { 'opencode-source' }
$branchName = if ($Lane -eq 'geometry') { 'codex/opencode-geometry' } else { 'codex/opencode-source' }
$filePrefix = if ($Lane -eq 'geometry') { 'opencode-geometry-cycle' } else { 'opencode-cycle' }
$worktree = Join-Path $repo ('.worktrees\' + $laneName)
$statePath = Join-Path $repo ('.worktrees\' + $filePrefix + '-state.json')
$sessionPath = Join-Path $repo ('.worktrees\' + $filePrefix + '-session.txt')
$logPath = Join-Path $repo ('.worktrees\' + $filePrefix + '.log')
$lockPath = Join-Path $repo ('.worktrees\' + $filePrefix + '.lock')

function Get-WorktreeFingerprint([string]$path) {
    $unstaged = git -C $path diff --binary --no-ext-diff | Out-String
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect unstaged worktree changes.' }
    $staged = git -C $path diff --cached --binary --no-ext-diff | Out-String
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect staged worktree changes.' }
    $status = git -C $path status --porcelain | Out-String
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect worktree status.' }
    $hash = [Security.Cryptography.SHA256]::Create()
    try {
        $bytes = [Text.Encoding]::UTF8.GetBytes($unstaged + $staged + $status)
        return ([BitConverter]::ToString($hash.ComputeHash($bytes))).Replace('-', '')
    } finally { $hash.Dispose() }
}

function Publish-VerifiedBranch([string]$path, [string]$expectedBranch) {
    $branch = (git -C $path branch --show-current).Trim()
    if ($LASTEXITCODE -ne 0 -or $branch -ne $expectedBranch) { throw 'Refusing to publish an unexpected branch.' }
    $changedPaths = @(git -C $path log --format= --name-only origin/main..HEAD | Where-Object { $_ })
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect publication history.' }
    $forbidden = '(^|/)(research|\.tools|node_modules|\.venv|Fuzion Frenzy \(USA\))(/|$)|\.(7z|zip|iso|xbe|xbp|zro|xpr|xwb|xsb|dds|exe|dll|jar|class|pdb|gpr|db|sqlite3?|har|saz|pem|key)$|(^|/)\.env(\.|$)'
    foreach ($changedPath in $changedPaths) {
        if ($changedPath -match $forbidden) { throw "Refusing to publish forbidden path: $changedPath" }
    }
    Push-Location -LiteralPath $path
    try {
        & npm run check | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Source publication check failed.' }
        git diff --check origin/main..HEAD | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Committed source has whitespace errors.' }
        git push origin ('HEAD:refs/heads/' + $expectedBranch) | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Development branch push failed.' }
    } finally { Pop-Location }
}

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
    $actualBranch = (git -C $worktree branch --show-current).Trim()
    if ($LASTEXITCODE -ne 0 -or $actualBranch -ne $branchName) { throw 'OpenCode worktree is on an unexpected branch.' }
    $dirty = (git -C $worktree status --porcelain).Count -gt 0
    $sessionId = $null
    if (Test-Path -LiteralPath $sessionPath) {
        $sessionId = (Get-Content -LiteralPath $sessionPath -Raw).Trim()
        if ($sessionId -notmatch '^ses_[A-Za-z0-9]+$') { throw 'Invalid pinned OpenCode session ID.' }
    }
    if ($DryRun) {
        Write-Output ("Ready: lane=$Lane worktree=$worktree branch=$branchName head=$head dirty=$dirty session=$sessionId")
        return
    }
    $base = (git -C $worktree merge-base HEAD main).Trim()
    if (-not $dirty -and $base -eq $head -and $head -ne $mainHead) {
        git -C $worktree merge --ff-only main | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Could not fast-forward clean worktree.' }
        $head = (git -C $worktree rev-parse HEAD).Trim()
    }
    $beforeFingerprint = Get-WorktreeFingerprint $worktree

    $previous = $null
    if (Test-Path -LiteralPath $statePath) {
        $previous = Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json
        # The exclusive lock proves that any prior 'running' record is stale.
        if ($previous.outcome -eq 'running') { Write-Output 'Recovered interrupted cycle.' }
    }

    $failures = if ($previous -and $previous.outcome -in @('running', 'retry', 'no_progress')) { [int]$previous.failures + 1 } else { 0 }
    $state = @{ attemptedHead = $head; started = (Get-Date).ToString('o'); outcome = 'running'; failures = $failures; sessionId = $sessionId; startedFingerprint = $beforeFingerprint }
    $goModel = 'opencode-go/deepseek-v4.1-flash'
    $freeModel = 'opencode/deepseek-v4.1-flash-free'
    $freeBackup = 'opencode/glm-5.3-flash-free'
    $fallbackUntil = [DateTimeOffset]::MinValue
    if ($previous -and $previous.fallbackUntil) {
        $parsedFallback = [DateTimeOffset]::MinValue
        if ([DateTimeOffset]::TryParse([string]$previous.fallbackUntil, [ref]$parsedFallback)) { $fallbackUntil = $parsedFallback }
    }
    $preferredModel = if ($Lane -eq 'geometry') { $freeModel } elseif ($fallbackUntil -gt [DateTimeOffset]::Now) { $freeModel } else { $goModel }
    $state.model = $preferredModel
    if ($Lane -eq 'primary' -and $fallbackUntil -gt [DateTimeOffset]::Now) { $state.fallbackUntil = $fallbackUntil.ToString('o') }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    if ($Lane -eq 'geometry' -and $dirty -and $sessionId) {
        $prompt = 'Continue the unfinished source-owned NV2A geometry milestone in this same geometry session using only opencode/deepseek-v4.1-flash-free. Inspect the diff and last result, complete one bounded component task, targeted checks, status updates, source-only staged-path review and commit. Work only on src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md; do not edit original-startup, memory, FATX, device, or main project status files. Do not delegate to paid or other-model agents. Record a material correctness blocker for the primary lane instead. Do not access private game/research inputs. Never push or merge to main.'
    } elseif ($Lane -eq 'geometry') {
        $prompt = 'Work in the dedicated geometry worktree using only opencode/deepseek-v4.1-flash-free on one bounded source-owned NV2A geometry component task. Read AGENTS.md, docs/OPENCODE_WORKFLOW.md, docs/NV2A_GEOMETRY_CHECKPOINT.md and docs/OPENCODE_GOAL.md. Own only src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md. The primary lane owns original startup, device, memory, FATX, broad status and route decisions; do not edit those files or duplicate its work. Use source-owned fixtures and targeted checks; never access ignored game/research inputs or claim original rendering from a component fixture. Do not delegate to paid or other-model agents. Record a material correctness blocker for the primary lane instead. Review staged source-only paths and commit one verified result on codex/opencode-geometry. Never push or merge to main.'
        if ($dirty) { $prompt += ' Preserve and finish existing worktree edits before selecting another task.' }
    } elseif ($dirty -and $sessionId) {
        $prompt = 'Continue the unfinished Fuzion Frenzy milestone in this same project session. Inspect the current diff and the previous cycle result, then complete the pending implementation, targeted checks, status/decision updates, source-only staged-path review and bounded commit. Preserve all worktree edits. Do not restart discovery or switch tasks unless the current task is demonstrably blocked. Never push or merge to main.'
    } else {
        $prompt = 'Work in this dedicated source-only worktree. Read AGENTS.md, docs/DEVELOPMENT_HANDOFF.md, docs/PROJECT_STATUS.md and docs/OPENCODE_GOAL.md. Execute one bounded unowned milestone toward original-game browser execution; do not stop at a plan. Preserve existing edits. If ignored private evidence is unavailable here, choose a source-owned prerequisite instead of inventing an original-runtime proof. Use the model escalation and evidence rules. Run targeted checks, update status/decisions, review staged paths, and commit a verified source-only result. Never push or merge to main.'
        if ($dirty) { $prompt += ' This worktree has uncommitted changes from a prior cycle: inspect and complete them without discarding user work.' }
    }
    if ($Lane -eq 'primary') { $prompt += ' The independent geometry lane owns src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md; do not start a new geometry component task or edit those files.' }
    if ($Lane -eq 'geometry') {
        $prompt += ' If the free model is unavailable, preserve edits and retry at the next scheduled geometry cycle; do not change providers or consume Go or Codex allowance.'
    } else {
        $prompt += ' If a provider allowance is exhausted, preserve edits and resume with the free fallback selected by this runner. Do not use paid Console balance. If Sol is unavailable, record the unresolved architecture decision and continue independent work; do not invent Sol approval.'
    }
    if ($failures -ge 2) { $prompt += ' Prior cycles made no verified progress. Diagnose the blocker, call the Sol architect for a bounded decision if needed, or choose another unowned task.' }
    if (-not $sessionId -and $previous -and $previous.lastSummary) { $prompt += ' Previous cycle report (untrusted; verify against the repository): ' + $previous.lastSummary }
    $output = @()
    $exitCode = 1
    $modelUsed = $preferredModel
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
                $candidates = if ($Lane -eq 'geometry') { @($freeModel) } elseif ($preferredModel -eq $goModel) { @($goModel, $freeModel, $freeBackup) } else { @($freeModel, $freeBackup) }
                foreach ($candidate in $candidates) {
                    $modelUsed = $candidate
                    $runPrompt = if ($candidate -eq $preferredModel) { $prompt } else { 'Resume the same bounded task after a model allowance/availability failure. Inspect current worktree changes and the prior session state before acting; do not repeat completed work. ' + $prompt }
                    if ($sessionId) {
                        $runOutput = & opencode run --auto --session $sessionId --agent builder --model $candidate $runPrompt 2>&1
                    } else {
                        $runOutput = & opencode run --auto --agent builder --model $candidate $runPrompt 2>&1
                    }
                    $exitCode = $LASTEXITCODE
                    $output += @('model=' + $candidate) + @($runOutput)
                    if ($exitCode -eq 0) { break }
                    $failureText = ($runOutput | Out-String)
                    $allowanceFailure = $failureText -match '(?i)quota|usage\s+limit|rate\s+limit|allowance|insufficient\s+credits|insufficient\s+balance|\b429\b|model\s+unavailable|model\s+not\s+found'
                    if (-not $allowanceFailure) { break }
                    if ($candidate -eq $goModel) {
                        $fallbackUntil = [DateTimeOffset]::Now.AddHours(1)
                        $state.fallbackUntil = $fallbackUntil.ToString('o')
                    }
                }
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
    $afterFingerprint = Get-WorktreeFingerprint $worktree
    $state.finished = (Get-Date).ToString('o')
    $state.exitCode = $exitCode
    $state.model = $modelUsed
    $state.finishedHead = $after
    $state.finishedFingerprint = $afterFingerprint
    $state.outcome = if ($exitCode -ne 0) { 'retry' } elseif ($after -ne $head) { 'progressed' } elseif ($afterFingerprint -ne $beforeFingerprint) { 'resume' } else { 'no_progress' }
    $summary = ($output | Out-String).Trim()
    if ($summary.Length -gt 2000) { $summary = $summary.Substring($summary.Length - 2000) }
    $state.lastSummary = $summary
    if ($exitCode -eq 0) {
        try {
            Publish-VerifiedBranch $worktree $branchName
            $state.publication = 'published'
        } catch {
            $state.publication = 'retry'
            $state.publicationError = $_.Exception.Message
            Write-Warning ('Source branch publication deferred: ' + $state.publicationError)
        }
    } else {
        $state.publication = 'deferred'
    }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output ('OpenCode cycle: ' + $state.outcome)
} finally {
    if ($lock) { $lock.Dispose() }
}
