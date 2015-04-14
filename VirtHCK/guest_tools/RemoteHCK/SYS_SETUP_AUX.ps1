$TestMachineOne = "CL1-REPLACE";
$TestMachineTwo = "CL2-REPLACE";

# In the following, getting the name first, then setting IP and only after
# that renaming. This is because renaming might not work in some scenarios.

$NetAdd = Get-NetAdapter | where {$_.MacAddress -eq "56-cc-cc-ff-cc-cc"};
if ( $NetAdd -ne $null )
{
   $AdapterName = $NetAdd | Select-Object -ExpandProperty Name
   netsh interface ip set address name="$AdapterName" static 192.168.100.1 255.255.255.0;
   $NetAdd | Rename-NetAdapter -NewName "Control_e1000";
   Set-Item -Path HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce -Value "C:\REPLACE-CONTROLLER-INST-FILE"
   Copy-Item "\\REPLACE-SMB-ADDRESS\qemu\REPLACE-CONTROLLER-INST-FILE" "C:\"
   Write-Host "Restarting in 30s"
   Start-Sleep -s 30
   Rename-Computer -NewName HCK-STUDIO -Force -Restart
}

$NetAdd = Get-NetAdapter | where {$_.MacAddress -eq "56-cc-cc-01-cc-cc"};
if ( $NetAdd -ne $null )
{
   $AdapterName = $NetAdd | Select-Object -ExpandProperty Name
   netsh interface ip set address name="$AdapterName" static 192.168.100.2 255.255.255.0;
   $NetAdd | Rename-NetAdapter -NewName "Control_e1000";
   Set-Item -Path HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce -Value "C:\REPLACE-CLIENT-INST-FILE"
   Copy-Item "\\REPLACE-SMB-ADDRESS\qemu\REPLACE-CLIENT-INST-FILE" "C:\"
   Write-Host "Restarting in 30s"
   Start-Sleep -s 30
   Rename-Computer -NewName $TestMachineOne -Force -Restart
}

$NetAdd = Get-NetAdapter | where {$_.MacAddress -eq "56-cc-cc-02-cc-cc"};
if ( $NetAdd -ne $null )
{
   $AdapterName = $NetAdd | Select-Object -ExpandProperty Name
   netsh interface ip set address name="$AdapterName" static 192.168.100.3 255.255.255.0;
   $NetAdd | Rename-NetAdapter -NewName "Control_e1000";
   Set-Item -Path HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce -Value "C:\REPLACE-CLIENT-INST-FILE"
   Copy-Item "\\REPLACE-SMB-ADDRESS\qemu\REPLACE-CLIENT-INST-FILE" "C:\"
   Write-Host "Restarting in 30s"
   Start-Sleep -s 30
   Rename-Computer -NewName $TestMachineTwo -Force -Restart
}
