param([switch]$DryRun, [string]$ApprovalPath)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$pendingPath = Join-Path $repo '.worktrees\opencode-main-merge-approval.json'
if ($ApprovalPath) {
    if (-not $DryRun) { throw 'A custom approval path is allowed only for a read-only dry run.' }
    $pendingPath = $ApprovalPath
}
$resultPath = Join-Path $repo '.worktrees\opencode-main-merge-state.json'
$prManager = Join-Path $repo 'tools\Invoke-OpenCodeGitHubPR.ps1'
if (-not (Test-Path -LiteralPath $pendingPath)) {
    Write-Output 'No architect-approved merge is pending.'
    return
}
$pending = Get-Content -LiteralPath $pendingPath -Raw | ConvertFrom-Json
$approvedAt = [DateTimeOffset]::MinValue
if ($pending.head -notmatch '^[0-9a-f]{40}$' -or
    $pending.model -ne 'opencode-go/glm-5.3-flash' -or
    -not [DateTimeOffset]::TryParse([string]$pending.approvedAt, [ref]$approvedAt) -or
    [DateTimeOffset]::Now.Subtract($approvedAt).TotalHours -gt 24) {
    if (-not $DryRun) { Remove-Item -LiteralPath $pendingPath -Force }
    Write-Output 'Discarded an invalid or expired architect merge approval.'
    return
}
$localHead = (git -C $repo rev-parse codex/opencode-architect).Trim()
if ($LASTEXITCODE -ne 0) { throw 'Could not inspect the integration head.' }
if ($localHead -ne $pending.head) {
    if (-not $DryRun) { Remove-Item -LiteralPath $pendingPath -Force }
    Write-Output 'Discarded a merge approval for a changed integration head.'
    return
}
if ($DryRun) {
    Write-Output ("Exact-head architect approval is pending: $($pending.head)")
    return
}
try {
    $result = & powershell -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $prManager -Action MergeMain -ExpectedHead $pending.head 2>&1
    if ($LASTEXITCODE -ne 0) { throw (($result | Out-String).Trim()) }
    Remove-Item -LiteralPath $pendingPath -Force
    try {
        $merged = ($result | Out-String).Trim() | ConvertFrom-Json
        @{ outcome = 'merged'; head = $pending.head; pr = $merged.mergedMainPR; mergeCommit = $merged.mergeCommit; finished = (Get-Date).ToString('o') } |
            ConvertTo-Json | Set-Content -LiteralPath $resultPath -Encoding utf8
    } catch { Write-Warning 'PR merged, but its local status record could not be written.' }
    Write-Output 'Merged the exact architect-approved PR after live source, CI, ancestry and mergeability checks.'
} catch {
    # CI may still be running. Keep the exact-head approval for the next
    # scheduled probe; the PR helper fails closed on every changed gate.
    Write-Output ("Approved merge remains pending: $($_.Exception.Message)")
}
