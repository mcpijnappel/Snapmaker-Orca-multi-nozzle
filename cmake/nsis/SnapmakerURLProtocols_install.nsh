; Machine-wide URL protocol registration (mirrors macOS CFBundleURLSchemes).
; Included by CPack-generated NSIS installer via CPACK_NSIS_INSTALL_SCRIPT.
; On 64-bit Windows, 32-bit NSIS must use the 64-bit registry view so the 64-bit app sees these keys.
SetRegView 64

WriteRegStr HKLM "Software\Classes\snapmaker-orca-max" "" "URL:Snapmaker Orca Max"
WriteRegStr HKLM "Software\Classes\snapmaker-orca-max" "URL Protocol" ""
WriteRegStr HKLM "Software\Classes\snapmaker-orca-max\shell\open\command" "" '"$INSTDIR\snapmaker-orca-max.exe" "%1"'

WriteRegStr HKLM "Software\Classes\Snapmaker_Orca_Max" "" "URL:Snapmaker Orca Max"
WriteRegStr HKLM "Software\Classes\Snapmaker_Orca_Max" "URL Protocol" ""
WriteRegStr HKLM "Software\Classes\Snapmaker_Orca_Max\shell\open\command" "" '"$INSTDIR\snapmaker-orca-max.exe" "%1"'

SetRegView 32
