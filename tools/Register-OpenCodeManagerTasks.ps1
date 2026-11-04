param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$templateName = 'FuzionFrenzy-OpenCode-Cycle'
$template = Export-ScheduledTask -TaskName $templateName
if (-not $template) { throw 'Existing source builder task is missing.' }

$specs = @(
    @{ name = 'FuzionFrenzy-OpenCode-Manager'; interval = 'PT15M'; limit = 'PT25M'; script = 'Invoke-OpenCodeManager.ps1'; arguments = ''; workdir = $repo; description = 'Read-only GLM Flash manager assigns bounded work to three isolated OpenCode builders.' },
    @{ name = 'FuzionFrenzy-OpenCode-Verification'; interval = 'PT15M'; limit = 'PT25M'; script = 'Invoke-OpenCodeCycle.ps1'; arguments = ' -Lane verification'; workdir = (Join-Path $repo '.worktrees\opencode-verification'); description = 'Run one manager-assigned source-owned verification task with MiniMax M3.1 Flash.' }
)

foreach ($spec in $specs) {
    $scriptPath = Join-Path $repo ('tools\' + $spec.script)
    if (-not (Test-Path -LiteralPath $scriptPath) -or -not (Test-Path -LiteralPath $spec.workdir)) { throw "Missing task path for $($spec.name)" }
    $existing = Get-ScheduledTask -TaskName $spec.name -ErrorAction SilentlyContinue
    if ($existing -and $existing.Actions.Execute -ne 'powershell.exe') { throw "Unexpected existing task: $($spec.name)" }
    [xml]$xml = $template
    $xml.Task.RegistrationInfo.URI = '\' + $spec.name
    $xml.Task.RegistrationInfo.Description = $spec.description
    $xml.Task.Triggers.CalendarTrigger.Repetition.Interval = $spec.interval
    $xml.Task.Settings.ExecutionTimeLimit = $spec.limit
    $xml.Task.Actions.Exec.Arguments = '-NoProfile -NonInteractive -WindowStyle Hidden -ExecutionPolicy Bypass -File "' + $scriptPath + '"' + $spec.arguments
    $xml.Task.Actions.Exec.WorkingDirectory = [string]$spec.workdir
    if ($DryRun) { Write-Output ("Ready: $($spec.name) $($spec.interval) start-in=$($spec.workdir)"); continue }
    Register-ScheduledTask -TaskName $spec.name -Xml $xml.OuterXml -Force | Out-Null
    $actual = Get-ScheduledTask -TaskName $spec.name
    if ($actual.Actions.WorkingDirectory -ne $spec.workdir -or $actual.Actions.Arguments -notlike ('*' + $spec.script + '*')) { throw "Task verification failed: $($spec.name)" }
    Write-Output ("Registered: $($spec.name)")
}
