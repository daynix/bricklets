@echo off
echo Installing HCK Controller and Studio. This will take a VERY long time!
echo SERIOUSLY! IT CAN TAKE OVER AN HOUR!
echo Grab yourself a meal!
echo Waiting 30s...
timeout /t 30 /nobreak > NUL
echo Performing the installation - be VERY patient!
"\\192.168.101.1\qemu\Hardware Certification Kit\HCKSetup.exe" /q

if not errorlevel 0 (
    echo "FAILED"
    exit /B 1
) else (
    echo "SUCEEDED"
)

echo Installing HCK Filters...
expand -i "\\192.168.101.1\qemu\HCKFilterUpdates.cab" -f:UpdateFilters.sql "C:\Program Files (x86)\Windows Kits\REPLACE-HCK-VERSION\Hardware Certification Kit\Controller"
cd "C:\Program Files (x86)\Windows Kits\REPLACE-HCK-VERSION\Hardware Certification Kit\Controller"
"C:\Program Files (x86)\Windows Kits\REPLACE-HCK-VERSION\Hardware Certification Kit\Controller\updatefilters.exe"

if not errorlevel 0 (
    echo "FAILED"
    exit /B 1
) else (
    echo "SUCEEDED"
)

echo Press ENTER to reboot the system...
pause > NUL
shutdown /t 10 /r /f

