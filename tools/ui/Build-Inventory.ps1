param(
    [Parameter(Mandatory = $true)][string] $SourceTree,
    [Parameter(Mandatory = $true)][string] $NativeSource,
    [Parameter(Mandatory = $true)][string] $SdkSource,
    [Parameter(Mandatory = $true)][string] $DiscImage,
    [Parameter(Mandatory = $true)][string] $RuntimeExe,
    [string] $OutputRoot = (Join-Path $PSScriptRoot '../../artifacts'),
    [string] $NodeExe = 'node',
    [string] $PythonExe = 'python'
)
$ErrorActionPreference = 'Stop'
$project = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$OutputRoot = [IO.Path]::GetFullPath($OutputRoot)
foreach ($name in @('SourceTree','NativeSource','SdkSource','DiscImage','RuntimeExe')) {
    Set-Variable -Name $name -Value (Resolve-Path -LiteralPath (Get-Variable -Name $name -ValueOnly)).Path
}
if ((& $NodeExe --version).Trim() -ne 'v24.14.0') { throw 'Use the project-pinned Node 24.14.0.' }
function Run-Node([string] $Tool, [string[]] $ToolArguments) {
    & $NodeExe (Join-Path $PSScriptRoot $Tool) @ToolArguments
    if ($LASTEXITCODE) { throw ('UI workflow failed: ' + $Tool) }
}
function Run-Python([string] $Tool, [string[]] $ToolArguments) {
    & $PythonExe (Join-Path $PSScriptRoot $Tool) @ToolArguments
    if ($LASTEXITCODE) { throw ('UI workflow failed: ' + $Tool) }
}
Run-Node 'private-output.mjs' @('--directory',$OutputRoot,'--input',$SourceTree,'--input',$NativeSource,'--input',$SdkSource,'--input',$DiscImage,'--input',$RuntimeExe)
$inventoryRoot = Join-Path $OutputRoot 'ui-inventory'
$inventory = Join-Path $inventoryRoot 'inventory.json'
$references = Join-Path $inventoryRoot 'source-references.json'
$overlays = Join-Path $inventoryRoot 'overlay-evidence.json'
$joins = Join-Path $inventoryRoot 'evidence-joins.json'
$components = Join-Path $project 'profiles/ui-components.json'
$xbe = Join-Path $SourceTree 'default.xbe'
Run-Node 'inventory.mjs' @('--source',$SourceTree,'--out',$inventoryRoot)
Run-Node 'package-census.mjs' @('--source',$SourceTree,'--inventory',$inventory,'--out',(Join-Path $inventoryRoot 'package-census.json'))
Run-Node 'source-evidence.mjs' @('--xbe',$xbe,'--generated-dir',(Join-Path $NativeSource 'generated/game'),'--inventory',$inventory,'--vtables',$components,'--out',$references)
Run-Node 'overlay-evidence.mjs' @('--source',$SourceTree,'--inventory',$inventory,'--source-references',$references,'--out',$overlays)
Run-Node 'font-evidence.mjs' @('--inventory',$inventory,'--source-tree',$SourceTree,'--output',(Join-Path $inventoryRoot 'font-evidence.json'))
Run-Node 'join-evidence.mjs' @('--inventory',$inventory,'--overlays',$overlays,'--references',$references,'--components',$components,'--out',$joins)
Run-Node 'verify-inventory.mjs' @('--inventory',$inventory,'--disc',$DiscImage,'--joins',$joins,'--overlays',$overlays,'--references',$references,'--out',(Join-Path $inventoryRoot 'verification.json'))
Run-Python 'replacement-pilot.py' @('--inventory',$inventory,'--joins',$joins,'--out',(Join-Path $OutputRoot 'ui-replacements/pilot'),'--id','dded0e74ad2bff44135c6a04','--id','b4a089816ad14c0d16965def','--id','0393008381c77d88832749d1')
Run-Python 'compose-preview.py' @('--inventory',$inventory,'--joins',$joins,'--out',(Join-Path $OutputRoot 'ui-replacements/settings-preview'))
$identityArguments = @('--file',('original-xbe=' + $xbe),'--file',('native-host-exe=' + $RuntimeExe),'--file',('runtime-disc=' + $DiscImage),'--file',('inventory=' + $inventory),'--file',('components=' + $components),'--tree',('native-source=' + $NativeSource),'--tree',('patched-sdk=' + $SdkSource),'--out',(Join-Path $inventoryRoot 'run-identities.json'))
$identityArguments += @('--tree',('ui-tools=' + $PSScriptRoot),'--file',('build-contract=' + (Join-Path $project 'desktop/windows/build-contract.json')))
foreach ($reader in @('xbp','texture','xbe','obe','xdvdfs','transforms')) {
    $identityArguments += @('--file',('reader-' + $reader + '=' + (Join-Path $project ('host/' + $reader + '.mjs'))))
}
if (Test-Path -LiteralPath (Join-Path $inventoryRoot 'captures')) {
    foreach ($capture in Get-ChildItem -LiteralPath (Join-Path $inventoryRoot 'captures') -Filter *.png) {
        $identityArguments += @('--file',('capture-' + $capture.BaseName + '=' + $capture.FullName))
    }
}
Run-Node 'provenance.mjs' $identityArguments
Run-Node 'catalog.mjs' @('--inventory',$inventory,'--out',(Join-Path $OutputRoot 'ui-catalog'))
Write-Output ('Private UI workflow complete. Start server.mjs --root "' + (Join-Path $OutputRoot 'ui-catalog') + '" to inspect the catalog.')
