
!include "MUI2.nsh"
!include "FileAssociation.nsh"
  
Name "DepthView"
BrandingText "DepthView"
OutFile "DepthViewInstaller.exe"

InstallDir $PROGRAMFILES\DepthView
InstallDirRegKey HKLM "Software\DepthView" "Install_Dir"

Var StartMenuFolder

; Request application privileges for Windows Vista
RequestExecutionLevel admin

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "header.bmp" ; optional
!define MUI_WELCOMEFINISHPAGE_BITMAP "wizard.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "wizard-uninstall.bmp"
!define MUI_ICON  "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON  "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
  
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_STARTMENU "DepthView" $StartMenuFolder
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
  
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Start DepthView"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchDepthView"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

;--------------------------------

; The stuff to install
Section "DepthView (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "DepthView.exe"
  File "libstdc++-6.dll"
  File "libgcc_s_dw2-1.dll"
  File "mingwm10.dll"
  File "QtCore4.dll"
  File "QtGui4.dll"
  File "imageformats\qjpeg4.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\DepthView "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DepthView" "DisplayName" "DepthView"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DepthView" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DepthView" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DepthView" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

MessageBox MB_YESNO "do you want to open .jps and .pns image files with DepthView?" IDNO dontreg
  ${registerExtension} "$INSTDIR\DepthView.exe" ".jps" "Stereo Image"
  ${registerExtension} "$INSTDIR\DepthView.exe" ".pns" "Stereo Image"
dontreg:

!insertmacro MUI_STARTMENU_WRITE_BEGIN "DepthView"
  CreateDirectory "$StartMenuFolder"
  CreateShortCut "$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$StartMenuFolder\DepthView.lnk" "$INSTDIR\DepthView.exe" "" "$INSTDIR\DepthView.exe" 0
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\DepthView"
  
  DeleteRegKey HKLM SOFTWARE\DepthView
  
  MessageBox MB_YESNO "do you want to delete user settings?" IDNO keepsettings
  DeleteRegKey HKCU SOFTWARE\DepthView
  keepsettings:
 
  ; Remove files and uninstaller
  Delete $INSTDIR\DepthView.exe
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\QtCore4.dll
  Delete $INSTDIR\QtGui4.dll
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\DepthView\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\DepthView"
  RMDir "$INSTDIR"

${unregisterExtension} ".jps" "Stereo Image"
${unregisterExtension} ".pns" "Stereo Image"
SectionEnd


Function LaunchDepthView
  ExecShell "" "$INSTDIR\DepthView.exe"
FunctionEnd