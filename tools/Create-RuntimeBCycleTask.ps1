# Registers the second full-time DeepSeek builder lane (runtime-b) as a Windows scheduled
# task. It clones the live runtime primary cycle task so action, trigger shape, repetition
# and execution limit stay identical, then points it at -Lane runtime-b and offsets the
# start boundary by 150 seconds so the two lanes do not start on the same tick.
# Idempotent: -Force overwrites an existing FuzionFrenzy-OpenCode-CycleB task.
param([switch]$DryRun)

$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$templateName = 'FuzionFrenzy-OpenCode-Cycle'
$taskName = 'FuzionFrenzy-OpenCode-CycleB'
$laneName = 'runtime-b'
$scriptPath = Join-Path $repo 'tools\Invoke-OpenCodeCycle.ps1'
$worktree = Join-Path $repo '.worktrees\opencode-runtime-b'
$startOffsetSeconds = 150
$description = 'Second full-time DeepSeek builder lane (runtime-b) for reached-but-unserviced runtime service implementations.'

if (-not (Test-Path -LiteralPath $scriptPath)) { throw 'The lane cycle runner is missing.' }
if (-not (Test-Path -LiteralPath $worktree)) { throw 'The runtime-b worktree is missing; create .worktrees/opencode-runtime-b on lane/runtime-b first.' }

$template = Export-ScheduledTask -TaskName $templateName
if (-not $template) { throw 'The runtime primary cycle task is missing.' }
if ((Get-ScheduledTask -TaskName $templateName).Actions.Execute -ne 'powershell.exe') { throw 'The runtime primary cycle task has an unexpected action.' }

[xml]$xml = $template
$xml.Task.RegistrationInfo.URI = '\' + $taskName
$xml.Task.RegistrationInfo.Description = $description
$templateStart = [string]$xml.Task.Triggers.CalendarTrigger.StartBoundary
$start = [DateTimeOffset]::MinValue
if ($templateStart -and [DateTimeOffset]::TryParse($templateStart, [ref]$start)) {
    $xml.Task.Triggers.CalendarTrigger.StartBoundary = $start.AddSeconds($startOffsetSeconds).ToString('yyyy-MM-ddTHH:mm:sszzz')
}
$xml.Task.Actions.Exec.Arguments = '-NoProfile -NonInteractive -WindowStyle Hidden -ExecutionPolicy Bypass -File "' + $scriptPath + '" -Lane ' + $laneName
$xml.Task.Actions.Exec.WorkingDirectory = [string]$worktree

$trigger = $xml.Task.Triggers.CalendarTrigger
Write-Output ("Task: $taskName lane=$laneName interval=$($trigger.Repetition.Interval) duration=$($trigger.Repetition.Duration) start=$($trigger.StartBoundary) limit=$($xml.Task.Settings.ExecutionTimeLimit)")
Write-Output ("Action: powershell.exe $($xml.Task.Actions.Exec.Arguments)")
Write-Output ("Workdir: $worktree")
if ($DryRun) { Write-Output ("Ready: $taskName not registered (-DryRun)."); return }

Register-ScheduledTask -TaskName $taskName -Xml $xml.OuterXml -Force | Out-Null
$actual = Get-ScheduledTask -TaskName $taskName
if ($actual.Actions.Execute -ne 'powershell.exe' -or
    $actual.Actions.Arguments -notlike ('*-Lane ' + $laneName + '*') -or
    $actual.Actions.WorkingDirectory -ne $worktree) {
    throw "Task verification failed: $taskName"
}
Write-Output ("Registered: $taskName (force-overwritten if it existed); trigger start=$($actual.Triggers[0].StartBoundary) repetition=$($actual.Triggers[0].Repetition.Interval) limit=$($actual.Settings.ExecutionTimeLimit)")
