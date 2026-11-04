param(
    [Parameter(Mandatory = $true)][ValidateRange(1, 1000000)][int]$IssueNumber,
    [switch]$DryRun
)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$source = Join-Path $repo '.worktrees\opencode-source'
$architect = Join-Path $repo '.worktrees\opencode-architect'
$apiBase = 'https://api.github.com/repos/Jdiller-86/fuzion-frenzy-web-port'
$notificationStatePath = Join-Path $repo '.worktrees\opencode-status-notification.json'

function Read-State([string]$name) {
    $path = Join-Path $repo ('.worktrees\' + $name + '-cycle-state.json')
    if (-not (Test-Path -LiteralPath $path)) { return $null }
    try { return Get-Content -LiteralPath $path -Raw | ConvertFrom-Json }
    catch { return $null }
}

function Get-Head([string]$path) {
    $line = @(git -C $path log -1 --format='%h %s')
    if ($LASTEXITCODE -ne 0 -or $line.Count -ne 1) { return 'unavailable' }
    return [string]$line[0]
}

function Clean-Line([string]$value) {
    if (-not $value) { return 'unknown' }
    $line = ($value -replace '[\r\n`|]', ' ').Trim()
    if ($line.Length -gt 180) { $line = $line.Substring(0, 180) }
    return $line
}

$builder = Read-State 'opencode'
$geometry = Read-State 'opencode-geometry'
$manager = Read-State 'opencode-architect'
$checkpoint = 'Current source checkpoint unavailable.'
$handoff = Join-Path $source 'docs\DEVELOPMENT_HANDOFF.md'
if (Test-Path -LiteralPath $handoff) {
    $checkpoint = Clean-Line ([string](Get-Content -LiteralPath $handoff -TotalCount 1))
}
$updated = [DateTimeOffset]::Now.ToString('yyyy-MM-dd HH:mm zzz')
$builderStatus = if ($builder) { "$(Clean-Line $builder.outcome) on $(Clean-Line $builder.model)" } else { 'unavailable' }
$geometryStatus = if ($geometry) { "$(Clean-Line $geometry.outcome) on $(Clean-Line $geometry.model)" } else { 'unavailable' }
$architectStatus = if ($manager) { "$(Clean-Line $manager.outcome) on $(Clean-Line $manager.model)" } else { 'unavailable' }
$sourceCommit = Clean-Line (Get-Head $source)
$architectCommit = Clean-Line (Get-Head $architect)
$cycleFinished = if ($manager -and $manager.finished) { [string]$manager.finished } else { '' }
$previousNotification = $null
if (Test-Path -LiteralPath $notificationStatePath) {
    try { $previousNotification = Get-Content -LiteralPath $notificationStatePath -Raw | ConvertFrom-Json }
    catch { $previousNotification = $null }
}
$sourceChange = if ($previousNotification -and $previousNotification.sourceCommit -eq $sourceCommit) { 'No new primary commit.' } else { "Primary commit: $sourceCommit." }
$architectChange = if ($previousNotification -and $previousNotification.architectCommit -eq $architectCommit) { 'No new architect commit.' } else { "Architect commit: $architectCommit." }
$comment = @"
### Fuzion Frenzy architect update - $updated

- Architect: $architectStatus. $architectChange
- Primary builder: $builderStatus. $sourceChange
- Geometry builder: $geometryStatus.
- Next checkpoint: $checkpoint
- Main integration: [PR #1](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/1).

Original-game browser playability is not established. You can reply here with project direction; replies are not yet imported into the architect's scheduled prompt.
"@
$body = @"
# Fuzion Frenzy development status

Updated $updated by the local OpenCode scheduler. This page shows source-only progress. A component test or synthetic profile is not evidence that the original game is playable.

- **Primary builder:** $builderStatus. Latest commit: $sourceCommit.
- **Geometry builder:** $geometryStatus.
- **Architect:** $architectStatus. Latest integration commit: $architectCommit.
- **Current checkpoint:** $checkpoint
- **Main integration:** [PR #1](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/1). Review branches: [source PR #2](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/2), [geometry PR #3](https://github.com/Jdiller-86/fuzion-frenzy-web-port/pull/3).

**Original-game browser playability:** not established. The original game files and derived private analysis remain local.

This issue is updated in place after each architect cycle, including cycles stopped by model limits. The repository's branch commits and PR checks are the source of truth for published changes.
"@

if ($DryRun) { Write-Output $body; Write-Output $comment; return }

$credentialLines = @('protocol=https', 'host=github.com', '') | git credential fill
if ($LASTEXITCODE -ne 0) { throw 'Git credential helper failed.' }
$passwordLine = $credentialLines | Where-Object { $_ -like 'password=*' } | Select-Object -First 1
if (-not $passwordLine) { throw 'No cached GitHub credential is available.' }
$token = $passwordLine.Substring(9)
$headers = @{ Authorization = 'Bearer ' + $token; Accept = 'application/vnd.github+json'; 'X-GitHub-Api-Version' = '2022-11-28'; 'User-Agent' = 'FuzionFrenzy-Status' }
try {
    $issue = Invoke-RestMethod -Uri ($apiBase + '/issues/' + $IssueNumber) -Method Get -Headers $headers -TimeoutSec 25
    if ($issue.title -ne 'Fuzion Frenzy development status' -or $issue.pull_request) { throw 'Status issue identity did not match.' }
    $payload = @{ body = $body } | ConvertTo-Json -Compress
    $result = Invoke-RestMethod -Uri ($apiBase + '/issues/' + $IssueNumber) -Method Patch -Headers $headers -ContentType 'application/json' -Body $payload -TimeoutSec 25
    Write-Output ('Updated GitHub status issue: ' + $result.html_url)
    if ($cycleFinished -and (-not $previousNotification -or $previousNotification.cycleFinished -ne $cycleFinished)) {
        $commentPayload = @{ body = $comment } | ConvertTo-Json -Compress
        $posted = Invoke-RestMethod -Uri ($apiBase + '/issues/' + $IssueNumber + '/comments') -Method Post -Headers $headers -ContentType 'application/json' -Body $commentPayload -TimeoutSec 25
        @{ cycleFinished = $cycleFinished; sourceCommit = $sourceCommit; architectCommit = $architectCommit; commentId = $posted.id } | ConvertTo-Json | Set-Content -LiteralPath $notificationStatePath -Encoding utf8
        Write-Output ('Posted architect update: ' + $posted.html_url)
    }
} finally {
    $token = $null
    $credentialLines = $null
    $headers = $null
}
