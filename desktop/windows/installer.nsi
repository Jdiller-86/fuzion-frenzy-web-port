Unicode true
!include "MUI2.nsh"

!ifndef SOURCE_ROOT
  !define SOURCE_ROOT "..\.."
!endif
!ifndef PUBLISH_DIR
  !define PUBLISH_DIR "${SOURCE_ROOT}\artifacts\windows\launcher"
!endif
!ifndef OUTPUT_FILE
  !define OUTPUT_FILE "${SOURCE_ROOT}\artifacts\windows\FuzionFrenzySetup.exe"
!endif

Name "Fuzion Frenzy Builder"
OutFile "${OUTPUT_FILE}"
InstallDir "$LOCALAPPDATA\Programs\FuzionFrenzyBuilder"
InstallDirRegKey HKCU "Software\FuzionFrenzyBuilder" "InstallDir"
RequestExecutionLevel user
ManifestDPIAware true
SetCompressor /SOLID lzma
BrandingText "Local game build and launcher"

!define MUI_ABORTWARNING
!define MUI_WELCOMEPAGE_TITLE "Fuzion Frenzy for Windows"
!define MUI_WELCOMEPAGE_TEXT "Install the local build wizard and launcher.$\r$\n$\r$\nChoose your own compatible Fuzion Frenzy disc image or extracted folder, then build a native Windows executable on your PC.$\r$\n$\r$\nNo game files are included or uploaded. The first build downloads source and requires the Microsoft C++ build tools. The launcher provides a separate, explicit tool-install action.$\r$\n$\r$\nYour game, settings and saves are stored separately from this installer."
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\FuzionFrenzyBuilder.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Open the local build wizard"
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "Builder and launcher (required)" MainSection
  SectionIn RO
  SetShellVarContext current
  SetOutPath "$INSTDIR"
  File /r "${PUBLISH_DIR}\*"
  File "${SOURCE_ROOT}\desktop\windows\README.md"
  File "${SOURCE_ROOT}\runtime\browser\sdk\SDK-NOTICE.txt"
  SetOutPath "$INSTDIR\scripts"
  File "${SOURCE_ROOT}\desktop\windows\scripts\*.ps1"
  File "${SOURCE_ROOT}\desktop\windows\scripts\Build-UiPack.py"
  SetOutPath "$INSTDIR\patches"
  File "${SOURCE_ROOT}\desktop\windows\patches\*.patch"
  File "${SOURCE_ROOT}\desktop\windows\patches\*.json"
  SetOutPath "$INSTDIR"
  File "${SOURCE_ROOT}\desktop\windows\build-contract.json"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  CreateDirectory "$SMPROGRAMS\Fuzion Frenzy"
  CreateShortcut "$SMPROGRAMS\Fuzion Frenzy\Build and Play.lnk" "$INSTDIR\FuzionFrenzyBuilder.exe"
  CreateShortcut "$SMPROGRAMS\Fuzion Frenzy\Uninstall Builder.lnk" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKCU "Software\FuzionFrenzyBuilder" "InstallDir" "$INSTDIR"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "DisplayName" "Fuzion Frenzy Builder"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "DisplayVersion" "0.1.3"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "Publisher" "Fuzion Frenzy Web Port contributors"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "UninstallString" '$"$INSTDIR\Uninstall.exe$"'
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "DisplayIcon" "$INSTDIR\FuzionFrenzyBuilder.exe"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "InstallLocation" "$INSTDIR"
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder" "NoRepair" 1
SectionEnd

Section "Desktop shortcut" DesktopSection
  CreateShortcut "$DESKTOP\Fuzion Frenzy.lnk" "$INSTDIR\FuzionFrenzyBuilder.exe"
SectionEnd

Section "Uninstall"
  SetShellVarContext current
  Delete "$DESKTOP\Fuzion Frenzy.lnk"
  RMDir /r "$SMPROGRAMS\Fuzion Frenzy"
  Delete "$INSTDIR\scripts\*.ps1"
  Delete "$INSTDIR\scripts\Build-UiPack.py"
  RMDir "$INSTDIR\scripts"
  Delete "$INSTDIR\patches\*.patch"
  Delete "$INSTDIR\patches\*.json"
  RMDir "$INSTDIR\patches"
  Delete "$INSTDIR\FuzionFrenzyBuilder.exe"
  Delete "$INSTDIR\FuzionFrenzyBuilder.pdb"
  Delete "$INSTDIR\README.md"
  Delete "$INSTDIR\SDK-NOTICE.txt"
  Delete "$INSTDIR\build-contract.json"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"
  DeleteRegKey HKCU "Software\FuzionFrenzyBuilder"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\FuzionFrenzyBuilder"
  MessageBox MB_OK "Builder removed. Your locally built game, settings and saves remain in $LOCALAPPDATA\FuzionFrenzy."
SectionEnd
