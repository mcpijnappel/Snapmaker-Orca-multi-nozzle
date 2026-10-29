; Remove URL protocol keys installed by SnapmakerURLProtocols_install.nsh
SetRegView 64
DeleteRegKey HKLM "Software\Classes\snapmaker-orca-max"
DeleteRegKey HKLM "Software\Classes\Snapmaker_Orca_Max"
DeleteRegKey HKCU "Software\Classes\snapmaker-orca-max"
DeleteRegKey HKCU "Software\Classes\Snapmaker_Orca_Max"
SetRegView 32
