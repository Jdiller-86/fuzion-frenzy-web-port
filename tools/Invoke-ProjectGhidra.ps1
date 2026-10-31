[CmdletBinding()]
param([Parameter(ValueFromRemainingArguments = $true)][string[]]$HeadlessArguments)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$ghidraRoot = Join-Path $projectRoot '.tools\ghidra_12.1.4_PUBLIC'
$javaRoot = Join-Path $projectRoot '.tools\jdk-21.0.13+9'
if (!(Test-Path -LiteralPath "$javaRoot\bin\java.exe")) { throw 'Project Java installation is missing.' }
if (!(Test-Path -LiteralPath "$ghidraRoot\support\analyzeHeadless.bat")) { throw 'Project Ghidra installation is missing.' }
$previousJava = $env:JAVA_HOME
$previousGhidra = $env:GHIDRA_INSTALL_DIR
try {
    $env:JAVA_HOME = $javaRoot
    $env:GHIDRA_INSTALL_DIR = $ghidraRoot
    & "$ghidraRoot\support\analyzeHeadless.bat" @HeadlessArguments
    if ($LASTEXITCODE -ne 0) { throw "Ghidra exited with code $LASTEXITCODE" }
} finally {
    $env:JAVA_HOME = $previousJava
    $env:GHIDRA_INSTALL_DIR = $previousGhidra
}
