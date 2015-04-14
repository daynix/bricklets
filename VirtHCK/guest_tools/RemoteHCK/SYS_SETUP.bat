@echo off
echo Waiting 30s...

timeout /t 30 /nobreak > NUL

echo Mapping SMB_SHARE to drive X: ...

net use X: \\REPLACE-SMB-ADDRESS\qemu /p:yes

echo Enabling auto-logon for Administrator...

reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "AutoAdminLogon" /t REG_SZ /d "1" /f
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "DefaultDomainName" /t REG_SZ /d "WORKGROUP" /f
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "DefaultUserName" /t REG_SZ /d "Administrator" /f
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "DefaultPassword" /t REG_SZ /d "PASSWORD-REPLACE" /f

echo Disabling Windows Firewall...

netsh advfirewall set allprofiles state off

echo Setting unidentified networks to Private Location...

reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows NT\CurrentVersion\NetworkList\Signatures\010103000F0000F0010000000F0000F0C967A3643C3AD745950DA7859209176EF5B87C875FA20DF21951640E807D7C24" /v "Category" /t REG_DWORD /d "1" /f

echo "Disabling Windows Update...

sc config wuauserv start= disabled
sc stop wuauserv
reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate\Auto Update" /v "AUOptions" /t REG_DWORD /d "1" /f

echo Disabling screensaver...

reg add "HKCU\Control Panel\Desktop" /v "ScreenSaveActive" /t REG_SZ /d "0" /f
reg add "HKCU\Control Panel\Desktop" /v "SCRNSAVE.EXE" /t REG_SZ /d "" /f

echo Disabling power saving options...

powercfg.exe -change -monitor-timeout-ac 0
powercfg.exe -change -disk-timeout-ac 0
powercfg.exe -change -standby-timeout-ac 0
powercfg.exe -change -hibernate-timeout-ac 0

echo Setting the informative wallpaper related material...

copy "\\REPLACE-SMB-ADDRESS\qemu\Bginfo.exe" "C:\"
copy "\\REPLACE-SMB-ADDRESS\qemu\BGI-REPLACE" "C:\"
reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /v "BgInfo" /t REG_SZ /d "C:\Bginfo.exe C:\BGI-REPLACE /TIMER:0 /NOLICPROMPT /SILENT" /f

echo Setting additional parameters...

copy "\\REPLACE-SMB-ADDRESS\qemu\REPLACE-SETUP-AUX" "C:\"
@powershell -ExecutionPolicy RemoteSigned -file "C:\REPLACE-SETUP-AUX"

