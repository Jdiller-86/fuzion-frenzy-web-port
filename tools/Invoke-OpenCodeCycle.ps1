# Lane runner for the OpenCode builder lanes. -Lane selects the worktree, branch, model
# chain and task card for primary, geometry, verification or runtime-b; -NoPush keeps a
# verified branch local instead of publishing it (validation and manual runs).
param(
    [switch]$DryRun,
    [switch]$NoPush,
    [ValidateSet('primary', 'geometry', 'verification', 'runtime-b')][string]$Lane = 'primary'
)

$ErrorActionPreference = 'Stop'
if (-not $env:UNSLOTH_API_KEY) { $env:UNSLOTH_API_KEY = [Environment]::GetEnvironmentVariable('UNSLOTH_API_KEY', 'User') }
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$laneName = if ($Lane -eq 'primary') { 'opencode-source' } else { 'opencode-' + $Lane }
$branchName = if ($Lane -eq 'primary') { 'codex/opencode-source' }
              elseif ($Lane -eq 'runtime-b') { 'lane/runtime-b' }
              else { 'codex/opencode-' + $Lane }
$filePrefix = if ($Lane -eq 'primary') { 'opencode-cycle' } else { 'opencode-' + $Lane + '-cycle' }
$modelPool = Get-Content (Join-Path $repo 'tools\opencode-builder-models.json') -Raw | ConvertFrom-Json
$lanePool = $modelPool.builders.$Lane
if (-not $lanePool -and $Lane -eq 'runtime-b') {
    # runtime-b mirrors the runtime primary chain: DeepSeek Go primary, the free DeepSeek
    # route, then free GLM Flash on a recognized allowance failure. Declared here so the
    # lane can run before tools/opencode-builder-models.json carries a runtime-b entry.
    $lanePool = [pscustomobject]@{
        model = 'opencode-go/deepseek-v4.1-flash'
        fallbacks = @('opencode/deepseek-v4.1-flash-free', 'opencode/glm-5.3-flash-free')
    }
}
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
    if ($lane -eq 'runtime-b') {
        # runtime-b owns reached-but-unserviced runtime service implementations, their
        # fixtures/tests and the service checkpoint it updates. Seed export and admission/
        # prep tooling, scheduler/model scripts and the runtime-a checkpoint files stay
        # with their owners, so a violating commit is never published from this lane.
        $unowned = '(^|/)(research|\.tools)/|^tools/(fixtures|ghidra|xemu)/|^tools/(prepare-title-recomp\.py|recomp_memory_access\.py|probe-[a-z-]+\.(py|mjs)|create_analysis_fixture\.py|bounded_layout_test\.py|[a-z-]*-toolchain\.json|opencode-builder-models\.json|(Invoke|Register|Publish)-OpenCode[^/]*)$|^docs/(RENDERER_ENTRY_CHECKPOINT|PROJECT_STATUS)\.md$|^src/runtime/(proof-report\.mjs|thread-proof-worker\.mjs|esi_events\.[ch])$|^tests/(proof-report\.test\.mjs|esi_events_test\.c)$|^tools/fixtures/thread_startup_probe\.c$'
        foreach ($file in $changed) {
            if ($file -match $unowned) { throw "Lane $lane touched an unowned path: $file" }
        }
        return
    }
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
    } elseif ($Lane -eq 'runtime-b') {
        $prompt = 'Work in the runtime-b service lane on one bounded source-owned runtime service implementation for a reached-but-unserviced request, ordinal or device service. Ownership rule for every cycle: runtime-b owns runtime service implementations for reached-but-unserviced requests/ordinals; it must not touch seed/admission pipeline files or the runtime-a checkpoint files. Read AGENTS.md, docs/OPENCODE_GOAL.md and the checkpoint named on line 1 of docs/DEVELOPMENT_HANDOFF.md, take the next unserviced request or ordinal from that checkpoint and stop condition, and stop at an unsupported register/resource. Stay inside src/runtime service modules, their source-owned fixtures and tests, and a service checkpoint you own; do not edit admission/prep tooling, orchestration scripts, docs/RENDERER_ENTRY_CHECKPOINT.md or docs/PROJECT_STATUS.md. Run targeted checks, keep every existing guard, inspect staged paths and commit one verified result on lane/runtime-b. Never access private game/research content and claim only what the service fixture proves. Do not delegate, push or merge.'
        if ($dirty) { $prompt += ' Preserve and finish existing worktree edits before selecting another task.' }
    } elseif ($dirty -and $sessionId) {
        $prompt = 'Continue the unfinished task in this pinned session. Inspect the diff, finish one bounded result with targeted checks and a source-only commit. Preserve edits; switch tasks only on a verified blocker. Do not push or merge to main.'
    } else {
        $prompt = 'Use the standing AGENTS.md and OPENCODE_GOAL.md rules. Read the TOP current status and the active checkpoint; execute its next unowned bounded original-browser-execution task. Batch related reached functions when evidenced, keep existing guards, run targeted checks, and make one source-only commit. Keep PROJECT_STATUS to the observed stop/check/next task; detailed proof belongs in the active checkpoint. Update handoff and evidence index only when their integration facts change. Stop at an unsupported service/register/resource or failed gate. If private evidence is unavailable, name the blocker and choose independent source-owned work. Do not push or merge to main.'
        if ($dirty) { $prompt += ' This worktree has uncommitted changes from a prior cycle: inspect and complete them without discarding user work.' }
    }
    if ($Lane -eq 'primary') { $prompt += ' The independent geometry lane owns src/runtime/nv2a_*, matching tests and docs/NV2A_GEOMETRY_CHECKPOINT.md; do not start a new geometry component task or edit those files.' }
    $prompt += ' Use only this lane runner selected model or its configured cheap/free fallback; do not switch providers yourself or consume Codex allowance. If unavailable, preserve edits for the next cycle.'
    $prompt += ' Final report: result and exact observed stop, checks, next task or blocker in at most four short lines; do not recap settled history.'
    if ($Lane -eq 'primary') { $prompt += ' Do not invent architect approval. Use the latest verified checkpoint when an older task card is already complete.' }
    if ($failures -ge 2) { $prompt += ' Prior cycles made no verified progress. Diagnose the blocker, request a bounded GLM Flash architect decision if needed, or choose another unowned task.' }
    if (-not $sessionId -and $previous -and $previous.lastSummary) { $lastBrief = [string]$previous.lastSummary; if ($lastBrief.Length -gt 500) { $lastBrief = $lastBrief.Substring($lastBrief.Length - 500) }; $prompt += ' Previous cycle (verify): ' + $lastBrief }
    $guidancePath = Join-Path $repo '.worktrees\opencode-architect-guidance.json'
    if (Test-Path -LiteralPath $guidancePath) {
        try {
            $guidance = Get-Content -LiteralPath $guidancePath -Raw | ConvertFrom-Json
            $created = [DateTimeOffset]::MinValue
            if ([DateTimeOffset]::TryParse([string]$guidance.created, [ref]$created) -and [DateTimeOffset]::Now.Subtract($created).TotalHours -ge 0 -and [DateTimeOffset]::Now.Subtract($created).TotalHours -lt 2 -and $guidance.summary) {
                $brief = [string]$guidance.summary
                if ($brief.Length -gt 650) { $brief = $brief.Substring(0, 650) }
                if ($guidance.approved -and $guidance.model -eq 'opencode-go/glm-5.3-flash') {
                    if ($Lane -eq 'geometry' -and $guidance.geometryCycle -eq 'HOLD') {
                        $state.outcome = 'deferred_architect'
                        $state.finished = (Get-Date).ToString('o')
                        $state.architectHead = [string]$guidance.head
                        $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
                        Write-Output 'Geometry cycle deferred by the hourly architect; primary development continues.'
                        return
                    }
                    $prompt += " The project architect using $($guidance.model) approved this current priority directive for lane $Lane from integration head $($guidance.head): $brief Follow it within your lane ownership after checking current evidence; report any conflict with the user's requirements or a newer verified result."
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
                if ($Lane -eq 'verification' -and $taskCard.action -eq 'RUN' -and
                    $taskCard.task -match '(?i)\brecord\b.*\b(PR|merge outcome|exact-head)' -and
                    $taskCard.task -notmatch '(?i)\b(fail|failed|uncertain|mismatch|defect|regression)\b') {
                    $state.outcome = 'deferred_metadata_only'
                    $state.finished = (Get-Date).ToString('o')
                    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
                    Write-Output 'Verification lane skipped a PR metadata recap; exact-head and CI gates remain with the architect runner.'
                    return
                }
                if ($taskCard.action -eq 'HOLD' -and -not $dirty) {
                    $state.outcome = 'deferred_manager'
                    $state.finished = (Get-Date).ToString('o')
                    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding utf8
                    Write-Output ("Lane $Lane held by the GLM Flash manager; other lanes continue.")
                    return
                }
                if ($taskCard.action -eq 'RUN') { $prompt += " The GLM Flash manager assigned this bounded task: $($taskCard.task) If already complete in the current checkpoint, advance its next task; architect route decisions prevail." }
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
            if ($NoPush) {
                $state.publication = 'local_only'
            } else {
                Publish-VerifiedBranch $worktree $branchName
                $state.publication = 'published'
            }
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
