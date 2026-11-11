# Private native UI build

`Build-NativeUi.ps1` incrementally compiles the retained SDK/native UI units
against an existing release `build.ninja`, substitutes those objects at link
time, and writes a separate `FuzionFrenzy-ui.exe` under a private LocalAppData or Git-ignored
output directory. It does not install over `FuzionFrenzy.exe`. Use the matching
pinned native and SDK checkouts and the build cache that produced the release
objects.

```powershell
./tools/ui/Build-NativeUi.ps1 `
  -NativeSource '<matching fuzion-native checkout>' `
  -SdkSource '<matching XboxXBERecomp checkout>' `
  -BuildCache "$env:LOCALAPPDATA/FuzionFrenzy/workspace/.tools/fuzion-native/out/build/release" `
  -OutputRoot "$env:LOCALAPPDATA/FuzionFrenzy/ui-build" `
  -PackPath '<private ui.bgra pack>'
```

Omit `-PackPath` to build only the executable. The optional file is copied next
to the new executable; set `FUZION_UI_PACK` to its path and
`FUZION_UI_REPLACE=pilot` when launching that executable to use the preview.
Each private object has a source, recursively resolved local-header and
compiler-argument signature, so unchanged objects are reused. The output also
contains `build-manifest.json` with those identities, the link response-file
hash, linked object map and executable hash.
The settings override replaces the retained SDK cache object ending in
`runtime/app/settings.cpp.obj`; its private compiled copy is
`objects/settings.cpp.obj` and is included in the required-object check.
`-ApplyPatches` temporarily
applies the published rendering, UI replacement, and native UI patches during
compilation, then reverses them in `finally`; known interrupted patches are
recovered before application. Unknown or partial edits stop the operation.
Keep all checkouts and output local. This remains an opt-in incremental developer path. The standard Windows
builder 0.1.2 applies the same patches during its normal CMake build and creates
the local pack directly from the selected ISO/folder; it does not call this
object-substitution script.
