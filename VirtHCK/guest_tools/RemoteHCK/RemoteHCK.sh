#!/usr/bin/bash

# Automate setup for VirtHCK

controllerIP='10.0.1.212'
cl1Name='CL1-2012R2X64'
cl2Name='CL2-2012R2X64'
winPasswd='PUT_YOUR_PASSWORD_HERE'
testDevice='PCI\VEN_XXXX&DEV_XXXX&SUBSYS_XXXXXXXX'  #Format: PCI\VEN_XXXX&DEV_XXXX&SUBSYS_XXXXXXXX
HCKVersion='8.1'  # Important! Change if using a different version!
projectName='TEST-PROJ'

SCRIPTS_DIR=`dirname $0`

SMBShareDir="${SCRIPTS_DIR}/../../SMB_SHARE"
SHARE_ON_HOST_ADDR='192.168.101.1'
autoHCKFile="${SCRIPTS_DIR}/../AutoHCK/AutoHCK.ps1"
sysSetupFile="${SCRIPTS_DIR}/SYS_SETUP.bat"
sysSetupAuxFile="${SCRIPTS_DIR}/SYS_SETUP_AUX.ps1"
ControllerInstFile="${SCRIPTS_DIR}/CONTROLLER_INSTALL.bat"
ClientInstFile="${SCRIPTS_DIR}/CLIENT_INSTALL.bat"
bgiFile="${SCRIPTS_DIR}/bg_display_data.bgi"
credsFile='creds.dat'

usage_and_exit()
{
    echo "Usage: $1 [ setup | run | shutdown-studio ]"
    exit 1
}

if [ $# -eq 0 ]
then
    usage_and_exit $0
elif [ $1 == "setup" ]
then
    # Create Creds file.
    echo "Creating credentials file..."
    echo "username=Administrator" > "$credsFile"
    echo "password=$winPasswd" >> "$credsFile"
    echo "domain=WORKGROUP" >> "$credsFile"
    # Change names in scripts and send changed scripts to shared folder.
    echo "Changing settings in setup scripts and copying them to $SMBShareDir ..."
    sed "s/CL1-REPLACE/$cl1Name/" "$sysSetupAuxFile" > "$SMBShareDir/${sysSetupAuxFile##*/}"
    sed -i "s/CL2-REPLACE/$cl2Name/" "$SMBShareDir/${sysSetupAuxFile##*/}"
    #
    sed "s/CL1-REPLACE/$cl1Name/" "$autoHCKFile" > "$SMBShareDir/${autoHCKFile##*/}"
    sed -i "s/CL2-REPLACE/$cl2Name/" "$SMBShareDir/${autoHCKFile##*/}"
    sed -i "s/DEVICE-REPLACE/$(printf "%q" "$testDevice")/" "$SMBShareDir/${autoHCKFile##*/}"
    sed -i "s/TEST-REPLACE/$projectName/" "$SMBShareDir/${autoHCKFile##*/}"
    sed -i "s/REPLACE-SMB-ADDRESS/$SHARE_ON_HOST_ADDR/g" "$SMBShareDir/${sysSetupAuxFile##*/}"
    #
    sed "s/PASSWORD-REPLACE/$winPasswd/" "$sysSetupFile" > "$SMBShareDir/${sysSetupFile##*/}"
    sed -i "s/BGI-REPLACE/${bgiFile##*/}/" "$SMBShareDir/${sysSetupFile##*/}"
    sed -i "s/REPLACE-SMB-ADDRESS/$SHARE_ON_HOST_ADDR/g" "$SMBShareDir/${sysSetupFile##*/}"
    #
    sed "s/REPLACE-HCK-VERSION/$HCKVersion/" "$ControllerInstFile" > "$SMBShareDir/${ControllerInstFile##*/}"
    sed -i "s/REPLACE-SMB-ADDRESS/$SHARE_ON_HOST_ADDR/g" "$SMBShareDir/${ControllerInstFile##*/}"
    #
    sed "s/REPLACE-SMB-ADDRESS/$SHARE_ON_HOST_ADDR/g" "$ClientInstFile" > "$SMBShareDir/${ClientInstFile##*/}"
    #
    cp "$bgiFile" "$SMBShareDir/${bgiFile##*/}"
    # Create batch file to run AutoHCK
    echo '@echo off' > "$SMBShareDir/RunAutoHCK.bat"
    echo 'pushd \\'"$SHARE_ON_HOST_ADDR"'\qemu' >> "$SMBShareDir/RunAutoHCK.bat"
    echo 'copy "\\'"$SHARE_ON_HOST_ADDR"'\qemu\'"${autoHCKFile##*/}"'" "C:\'"${autoHCKFile##*/}"'"' >> "$SMBShareDir/RunAutoHCK.bat"
    echo '%windir%\SysWOW64\WindowsPowerShell\v1.0\powershell.exe -ExecutionPolicy RemoteSigned -file "C:\'"${autoHCKFile##*/}"'"' >> "$SMBShareDir/RunAutoHCK.bat"
    echo 'popd' >> "$SMBShareDir/RunAutoHCK.bat"
    # Placing a file in SMB_SHARE to make it available to clients
    touch "$SMBShareDir/USE_SHARE"
elif [ $1 == "run" ]
then
    echo "Running..."
    winexe -A "$credsFile" //"$controllerIP" '\\'"$SHARE_ON_HOST_ADDR"'\qemu\RunAutoHCK.bat'
elif [ $1 == "shutdown-studio" ]
then
    echo "Shutting down HCK-STUDIO..."
    winexe -A "$credsFile" //"$controllerIP" 'shutdown /t 0 /s /f'
else
    usage_and_exit $0
fi

