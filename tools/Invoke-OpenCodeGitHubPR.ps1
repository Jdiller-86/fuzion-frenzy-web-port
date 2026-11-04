param(
    [ValidateSet('Snapshot', 'Ensure', 'MergeMain')][string]$Action = 'Snapshot',
    [ValidatePattern('^[0-9a-f]{40}$')][string]$ExpectedHead,
    [switch]$OpenMain,
    [ValidatePattern('^[0-9a-f]{40}$')][string]$OpenSourceHead,
    [ValidatePattern('^[0-9a-f]{40}$')][string]$OpenGeometryHead
)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$integration = Join-Path $repo '.worktrees\opencode-architect'
$repoName = 'Jdiller-86/fuzion-frenzy-web-port'
$apiBase = 'https://api.github.com/repos/' + $repoName
$forbidden = '(^|/)(research|\.tools|node_modules|\.venv|Fuzion Frenzy \(USA\))(/|$)|\.(7z|zip|iso|xbe|xbp|zro|xpr|xwb|xsb|dds|exe|dll|jar|class|pdb|gpr|db|sqlite3?|har|saz|pem|key)$|(^|/)\.env(\.|$)'

function Get-RemoteHead([string]$branch) {
    $line = @(git -C $repo ls-remote origin ('refs/heads/' + $branch))
    if ($LASTEXITCODE -ne 0) { throw "Could not read GitHub branch $branch." }
    if ($line.Count -eq 0) { return $null }
    return ($line[0] -split '\s+')[0]
}

function Assert-SafePaths([string]$range) {
    $paths = @(git -C $integration log --format= --name-only $range | Where-Object { $_ })
    if ($LASTEXITCODE -ne 0) { throw "Could not inspect paths in $range." }
    foreach ($path in $paths) { if ($path -match $forbidden) { throw "Forbidden PR path: $path" } }
}

function Assert-Integration([string]$head) {
    if (-not $head) { throw 'An exact integration head is required.' }
    if ((git -C $integration branch --show-current).Trim() -ne 'codex/opencode-architect') { throw 'Unexpected integration branch.' }
    if (@(git -C $integration status --porcelain).Count -ne 0) { throw 'Integration worktree has unfinished changes.' }
    if ((git -C $integration rev-parse HEAD).Trim() -ne $head) { throw 'Integration head changed.' }
    if ((Get-RemoteHead 'codex/opencode-architect') -ne $head) { throw 'Integration head is not published.' }
    git -C $integration fetch --quiet origin main | Out-Null
    if ($LASTEXITCODE -ne 0) { throw 'Could not refresh GitHub main.' }
    git -C $integration merge-base --is-ancestor origin/main HEAD
    if ($LASTEXITCODE -ne 0) { throw 'GitHub main is not integrated.' }
    git -C $integration merge-base --is-ancestor main HEAD
    if ($LASTEXITCODE -ne 0) { throw 'Local main governance is not integrated.' }
    Assert-SafePaths 'origin/main..HEAD'
    Push-Location -LiteralPath $integration
    try {
        & npm run check | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Source publication check failed.' }
        & npm run test:runtime | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Runtime source check failed.' }
        git diff --check origin/main..HEAD | Out-Null
        if ($LASTEXITCODE -ne 0) { throw 'Integration whitespace check failed.' }
    } finally { Pop-Location }
}

function Invoke-GitHub([string]$method, [string]$path, $body = $null) {
    $args = @{ Uri = ($apiBase + $path); Method = $method; Headers = $script:headers; TimeoutSec = 25 }
    if ($null -ne $body) {
        $args.Body = ($body | ConvertTo-Json -Depth 8 -Compress)
        $args.ContentType = 'application/json'
    }
    return Invoke-RestMethod @args
}

function Get-OpenPR([object[]]$prs, [string]$base, [string]$head) {
    return @($prs | Where-Object { $_.base.ref -eq $base -and $_.head.ref -eq $head -and $_.head.repo.full_name -eq $repoName } | Select-Object -First 1)[0]
}

$credentialLines = @('protocol=https', 'host=github.com', '') | git credential fill
if ($LASTEXITCODE -ne 0) { throw 'Git credential helper failed.' }
$passwordLine = $credentialLines | Where-Object { $_ -like 'password=*' } | Select-Object -First 1
if (-not $passwordLine) { throw 'No cached GitHub credential is available.' }
$token = $passwordLine.Substring(9)
$script:headers = @{
    Authorization = 'Bearer ' + $token
    Accept = 'application/vnd.github+json'
    'X-GitHub-Api-Version' = '2022-11-28'
    'User-Agent' = 'FuzionFrenzy-Architect'
}
try {
    $openRaw = Invoke-GitHub 'GET' '/pulls?state=open&per_page=100'
    $open = @()
    foreach ($prItem in $openRaw) { $open += $prItem }
    $mainHead = Get-RemoteHead 'main'
    $architectHead = Get-RemoteHead 'codex/opencode-architect'
    $mainPR = Get-OpenPR $open 'main' 'codex/opencode-architect'
    $sourcePR = Get-OpenPR $open 'codex/opencode-architect' 'codex/opencode-source'
    $geometryPR = Get-OpenPR $open 'codex/opencode-architect' 'codex/opencode-geometry'
    $result = [ordered]@{
        action = $Action
        remoteMain = $mainHead
        remoteArchitect = $architectHead
        mainPR = if ($mainPR) { [int]$mainPR.number } else { $null }
        sourceReviewPR = if ($sourcePR) { [int]$sourcePR.number } else { $null }
        geometryReviewPR = if ($geometryPR) { [int]$geometryPR.number } else { $null }
        otherMainPRs = @($open | Where-Object { $_.base.ref -eq 'main' -and $_.head.ref -ne 'codex/opencode-architect' } | ForEach-Object { [pscustomobject]@{ number = $_.number; head = $_.head.ref; draft = $_.draft } })
    }
    if ($mainPR) {
        $mainDetails = Invoke-GitHub 'GET' ('/pulls/' + $mainPR.number)
        $mainChecks = Invoke-GitHub 'GET' ('/commits/' + $mainDetails.head.sha + '/check-runs?per_page=100')
        $result.mainPRStatus = [ordered]@{
            head = $mainDetails.head.sha
            base = $mainDetails.base.sha
            draft = [bool]$mainDetails.draft
            mergeable = $mainDetails.mergeable
            mergeableState = $mainDetails.mergeable_state
            checks = @($mainChecks.check_runs | ForEach-Object { [pscustomobject]@{ name = $_.name; status = $_.status; conclusion = $_.conclusion } })
        }
    }
    foreach ($entry in @(@('source', $sourcePR), @('geometry', $geometryPR))) {
        if (-not $entry[1]) { continue }
        $details = Invoke-GitHub 'GET' ('/pulls/' + $entry[1].number)
        $result[($entry[0] + 'ReviewStatus')] = [ordered]@{ head = $details.head.sha; mergeable = $details.mergeable; mergeableState = $details.mergeable_state; draft = [bool]$details.draft }
    }

    if ($Action -eq 'Ensure') {
        if ($OpenMain -or $OpenSourceHead -or $OpenGeometryHead) { Assert-Integration $ExpectedHead }
        if ($OpenMain -and $mainHead -ne $ExpectedHead -and -not $mainPR) {
            $body = "Automated Sol architect integration candidate. Source-owned changes and evidence are recorded in docs/ARCHITECT_INTEGRATION_CHECKPOINT.md and docs/PROJECT_STATUS.md. Original game playability is not claimed. The runner will merge only an exact Sol-approved head after local source/runtime checks, both GitHub preparation jobs, clean mergeability and current-base checks."
            $mainPR = Invoke-GitHub 'POST' '/pulls' @{ title = 'Integrate reviewed source progress toward browser execution'; head = 'codex/opencode-architect'; base = 'main'; body = $body; draft = $false }
            $result.mainPR = [int]$mainPR.number
            $result.openedMainPR = $true
        }
        foreach ($lane in @('source', 'geometry')) {
            $requestedHead = if ($lane -eq 'source') { $OpenSourceHead } else { $OpenGeometryHead }
            if (-not $requestedHead) { continue }
            $branch = 'codex/opencode-' + $lane
            $remoteHead = Get-RemoteHead $branch
            $localHead = (git -C $repo rev-parse $branch).Trim()
            if ($remoteHead -ne $localHead -or $remoteHead -ne $requestedHead) { throw "Requested $lane PR head changed or is unpublished." }
            $ahead = [int](git -C $repo rev-list --count ($ExpectedHead + '..' + $localHead))
            if ($LASTEXITCODE -ne 0 -or $ahead -le 0) { continue }
            Assert-SafePaths ($ExpectedHead + '..' + $localHead)
            $existing = Get-OpenPR $open 'codex/opencode-architect' $branch
            if ($existing) { continue }
            $review = Invoke-GitHub 'POST' '/pulls' @{ title = ('Review ' + $lane + ' lane for architect integration'); head = $branch; base = 'codex/opencode-architect'; body = 'Source-only builder branch for Sol architect review. This draft is not approval or original-game playability evidence. Merge only after the architect reviews the diff, checks, conflicts and lane ownership.'; draft = $true }
            $result[($lane + 'ReviewPR')] = [int]$review.number
            $result[('opened' + $lane.Substring(0,1).ToUpper() + $lane.Substring(1) + 'ReviewPR')] = $true
        }
    }

    if ($Action -eq 'MergeMain') {
        Assert-Integration $ExpectedHead
        if (-not $mainPR) { throw 'No architect integration PR is open against main.' }
        $pr = Invoke-GitHub 'GET' ('/pulls/' + $mainPR.number)
        if ($pr.draft -or $pr.state -ne 'open' -or $pr.head.sha -ne $ExpectedHead -or $pr.base.ref -ne 'main' -or $pr.base.sha -ne $mainHead) { throw 'PR is draft, stale or targets a different head/base.' }
        if ($pr.mergeable -ne $true -or $pr.mergeable_state -ne 'clean') { throw ('PR is not cleanly mergeable: ' + $pr.mergeable_state) }
        $checkRuns = Invoke-GitHub 'GET' ('/commits/' + $ExpectedHead + '/check-runs?per_page=100')
        foreach ($name in @('preparation (ubuntu-latest)', 'preparation (windows-latest)')) {
            $matching = @($checkRuns.check_runs | Where-Object { $_.name -eq $name -and $_.head_sha -eq $ExpectedHead })
            if ($matching.Count -eq 0 -or @($matching | Where-Object { $_.status -ne 'completed' -or $_.conclusion -ne 'success' }).Count -gt 0) { throw "GitHub check is not successful: $name" }
        }
        if ($checkRuns.total_count -gt 2 -and @($checkRuns.check_runs | Where-Object { $_.head_sha -eq $ExpectedHead -and ($_.status -ne 'completed' -or $_.conclusion -notin @('success','neutral','skipped')) }).Count -gt 0) { throw 'Another GitHub check is not complete and successful.' }
        if ((Get-RemoteHead 'main') -ne $mainHead) { throw 'GitHub main moved during PR review.' }
        $merged = Invoke-GitHub 'PUT' ('/pulls/' + $mainPR.number + '/merge') @{ sha = $ExpectedHead; merge_method = 'merge'; commit_title = 'Merge reviewed architect integration' }
        if (-not $merged.merged) { throw 'GitHub did not report a merged PR.' }
        $result.mergedMainPR = [int]$mainPR.number
        $result.mergeCommit = [string]$merged.sha
    }
    $result | ConvertTo-Json -Depth 5 -Compress
} finally {
    $token = $null
    $credentialLines = $null
    $script:headers = $null
}
