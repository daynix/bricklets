@echo off
echo Installing the HCK Client. This will take a VERY long time!
echo SERIOUSLY! IT WILL BE LONG!
echo Grab yourself a meal!
echo Waiting 30s...
timeout /t 30 /nobreak > NUL
echo Performing the installation - be VERY patient!
"\\HCK-STUDIO\HCKInstall\Client\setup.exe" /qn ICFAGREE=Yes

if not errorlevel 0 (
    echo "FAILED"
    exit /B 1
) else (
    echo "SUCEEDED"
)

echo Press ENTER to reboot the system...
pause > NUL
shutdown /t 10 /r /f

