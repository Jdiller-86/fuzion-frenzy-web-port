param(
    [switch]$DryRun,
    [ValidateSet('primary', 'geometry', 'verification')][string]$Lane = 'primary'
)

$ErrorActionPreference = 'Stop'
if (-not $env:UNSLOTH_API_KEY) { $env:UNSLOTH_API_KEY = [Environment]::GetEnvironmentVariable('UNSLOTH_API_KEY', 'User') }
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$laneName = if ($Lane -eq 'primary') { 'opencode-source' } else { 'opencode-' + $Lane }
$branchName = if ($Lane -eq 'primary') { 'codex/opencode-source' } else { 'codex/opencode-' + $Lane }
$filePrefix = if ($Lane -eq 'primary') { 'opencode-cycle' } else { 'opencode-' + $Lane + '-cycle' }
$modelPool = Get-Content (Join-Path $repo 'tools\opencode-builder-models.json') -Raw | ConvertFrom-Json
$lanePool = $modelPool.builders.$Lane
if (-not $lanePool -or -not $lanePool.model) { throw "No model pool for lane $Lane" }
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
    $unfinished = @(git -C $path status --porcelain)
    if ($LASTEXITCODE -ne 0 -or $unfinished.Count -ne 0) { throw 'Worktree has unfinished edits; publication waits for a reviewed commit.' }
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

function Test-LaneOwnership([string]$path, [string]$lane, [string]$before) {
    if ($lane -eq 'primary') { return }
    $changed = @(git -C $path diff --name-only ($before + '..HEAD') | Where-Object { $_ })
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect lane changes.' }
    foreach ($file in $changed) {
        $allowed = if ($lane -eq 'geometry') { $file -match '^(src/runtime/nv2a_[^/]+|tests/[^/]*nv2a[^/]*|docs/NV2A_GEOMETRY_CHECKPOINT\.md)$' }
                   else { $file -match '^(tools/(verify-|check-)[^/]+|tests/(integration-|verification-)[^/]+|docs/INTEGRATION_VERIFICATION_CHECKPOINT\.md)$' }
        if (-not $allowed) { throw "Lane $lane touched an unowned path: $file" }
    }
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
    $goModel = [string]$lanePool.model
    $fallbacks = @($lanePool.fallbacks | ForEach-Object { [string]$_ })
    $fallbackUntil = [DateTimeOffset]::MinValue
    if ($previous -and $previous.fallbackUntil) {
        $parsedFallback = [DateTimeOffset]::MinValue
        if ([DateTimeOffset]::TryParse([string]$previous.fallbackUntil, [ref]$parsedFallback)) { $fallbackUntil = $parsedFallback }
    }
    $preferredModel = if ($fallbackUntil -gt [DateTimeOffset]::Now -and $fallbacks.Count -gt 0) { $fallbacks[0] } else { $goModel }
    $state.model = $preferredModel
    if ($fallbackUntil -gt [DateTimeOffset]::Now) { $state.fallbackUntil = $fallbackUntil.ToString('o') }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
    if ($Lane -eq 'geometry' -and $dirty -and $sessionId) {
        $prompt = 'Continue the unfinished source-owned NV2A geometry milestone in this same session. Inspect the diff and last result, complete one bounded component task, targeted checks and source-only staged-path review. Own only src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md. Do not access private game/research inputs, delegate, push or merge.'
    } elseif ($Lane -eq 'geometry') {
        $prompt = 'Work in the dedicated geometry worktree on one bounded source-owned NV2A task. Read AGENTS.md, docs/NV2A_GEOMETRY_CHECKPOINT.md and docs/OPENCODE_GOAL.md. Own only src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md. Use source-owned fixtures and targeted checks; never access ignored game/research inputs or claim original rendering from a component fixture. Do not delegate, push or merge. Commit one verified result on codex/opencode-geometry.'
        if ($dirty) { $prompt += ' Preserve and finish existing worktree edits before selecting another task.' }
    } elseif ($Lane -eq 'verification') {
        $prompt = 'Work in the independent verification worktree on one bounded source-owned integration/check tooling task. Read AGENTS.md and docs/OPENCODE_GOAL.md. Own only tools/verify-*, tools/check-*, tests/integration-*, tests/verification-* and docs/INTEGRATION_VERIFICATION_CHECKPOINT.md. Do not edit the runtime, general status, model/scheduler scripts or builder-owned tests. Do not access private research, delegate, merge or push. Run targeted checks, inspect staged paths and commit a verified result.'
        if ($dirty) { $prompt += ' Preserve and finish existing edits before new work.' }
    } elseif ($dirty -and $sessionId) {
        $prompt = 'Continue the unfinished Fuzion Frenzy milestone in this same project session. Inspect the current diff and the previous cycle result, then complete the pending implementation, targeted checks, status/decision updates, source-only staged-path review and bounded commit. Preserve all worktree edits. Do not restart discovery or switch tasks unless the current task is demonstrably blocked. Never push or merge to main.'
    } else {
        $prompt = 'Work in this dedicated source-only worktree. Read AGENTS.md, docs/DEVELOPMENT_HANDOFF.md, docs/PROJECT_STATUS.md and docs/OPENCODE_GOAL.md. Execute one bounded unowned milestone toward original-game browser execution; do not stop at a plan. Preserve existing edits. If ignored private evidence is unavailable here, choose a source-owned prerequisite instead of inventing an original-runtime proof. Use the model escalation and evidence rules. Run targeted checks, update status/decisions, review staged paths, and commit a verified source-only result. Never push or merge to main.'
        if ($dirty) { $prompt += ' This worktree has uncommitted changes from a prior cycle: inspect and complete them without discarding user work.' }
    }
    if ($Lane -eq 'primary') { $prompt += ' The independent geometry lane owns src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md; do not start a new geometry component task or edit those files.' }
    if ($Lane -eq 'primary') { $prompt += ' Treat architect model availability as time-specific; do not repeat an old availability result as current.' }
    $prompt += ' Use only this lane runner selected model or its configured cheap/free fallback; do not switch providers yourself or consume Codex allowance. If unavailable, preserve edits for the next cycle.'
    if ($Lane -eq 'primary') { $prompt += ' If the hourly GLM Flash architect is unavailable, record the unresolved architecture decision and continue independent work; do not invent architect approval.' }
    if ($failures -ge 2) { $prompt += ' Prior cycles made no verified progress. Diagnose the blocker, request a bounded GLM Flash architect decision if needed, or choose another unowned task.' }
    if (-not $sessionId -and $previous -and $previous.lastSummary) { $prompt += ' Previous cycle report (untrusted; verify against the repository): ' + $previous.lastSummary }
    $guidancePath = Join-Path $repo '.worktrees\opencode-architect-guidance.json'
    if (Test-Path -LiteralPath $guidancePath) {
        try {
            $guidance = Get-Content -LiteralPath $guidancePath -Raw | ConvertFrom-Json
            $created = [DateTimeOffset]::MinValue
            if ([DateTimeOffset]::TryParse([string]$guidance.created, [ref]$created) -and [DateTimeOffset]::Now.Subtract($created).TotalHours -ge 0 -and [DateTimeOffset]::Now.Subtract($created).TotalHours -lt 2 -and $guidance.summary) {
                $brief = [string]$guidance.summary
                if ($brief.Length -gt 1200) { $brief = $brief.Substring(0, 1200) }
                if ($guidance.approved -and $guidance.model -in @('opencode-go/glm-5.3-flash', 'opencode-go/glm-5.3')) {
                    if ($Lane -eq 'geometry' -and $guidance.geometryCycle -eq 'HOLD') {
                        $state.outcome = 'deferred_architect'
                        $state.finished = (Get-Date).ToString('o')
                        $state.architectHead = [string]$guidance.head
                        $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
                        Write-Output 'Geometry cycle deferred by the hourly architect; primary development continues.'
                        return
                    }
                    $prompt += " The project architect using $($guidance.model) approved this current priority directive for lane $Lane from integration head $($guidance.head): $brief Follow it within your lane ownership after checking current evidence; report any conflict with the user's requirements or a newer verified result."
                    if ($guidance.model -eq 'opencode-go/glm-5.3-flash') { $prompt += " GLM Flash completed this architect review at $($guidance.created). Verify current evidence before relying on its directive." }
                } else {
                    $prompt += " Provisional hourly review for lane $Lane (not an approved design decision): $brief Verify independently and do not change the project route on this advice alone."
                }
            }
        } catch { Write-Warning 'Could not read the latest architect directive; continuing the builder cycle.' }
    }
    $managerPath = Join-Path $repo '.worktrees\opencode-manager-guidance.json'
    $managerTaskValid = $false
    if (Test-Path -LiteralPath $managerPath) {
        try {
            $manager = Get-Content -LiteralPath $managerPath -Raw | ConvertFrom-Json
            $managerCreated = [DateTimeOffset]::MinValue
            $taskCard = $manager.lanes.$Lane
            if ($taskCard -and $taskCard.head -eq $head -and [DateTimeOffset]::TryParse([string]$manager.created, [ref]$managerCreated) -and [DateTimeOffset]::Now.Subtract($managerCreated).TotalHours -ge 0 -and [DateTimeOffset]::Now.Subtract($managerCreated).TotalHours -lt 2) {
                $managerTaskValid = $true
                if ($taskCard.action -eq 'HOLD' -and -not $dirty) {
                    $state.outcome = 'deferred_manager'
                    $state.finished = (Get-Date).ToString('o')
                    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
                    Write-Output ("Lane $Lane held by the GLM Flash manager; other lanes continue.")
                    return
                }
                if ($taskCard.action -eq 'RUN') { $prompt += " The GLM Flash manager assigned this bounded task: $($taskCard.task) Verify it against the latest architect directive; the architect prevails on conflict." }
            }
        } catch { Write-Warning 'Manager directive unavailable; continuing current lane rules.' }
    }
    if ($Lane -eq 'verification' -and -not $dirty -and -not $managerTaskValid) {
        $state.outcome = 'deferred_no_task'
        $state.finished = (Get-Date).ToString('o')
        $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
        Write-Output 'Verification lane awaiting a manager task.'
        return
    }
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
                $candidates = if ($preferredModel -eq $goModel) { @($goModel) + $fallbacks } else { @($fallbacks) }
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
            Test-LaneOwnership $worktree $Lane $head
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
