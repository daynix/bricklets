## A simple PowerShell script to automate HCK testing.
##
## Authorship:
## Inspired by the whql_powershell kit by Mike Cao <vipmike007@gmail.com>
## Written by Igor Derzhavets <igord@daynix.com>
## Modified by Leonid Bloch <leonid@daynix.com>
##
## License:
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the Free
## Software Foundation; under version 2 of the License.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, see http://www.gnu.org/licenses

# Parsing parameters (MUST be the first line in the script after comments)
param(
    [string]$controller = $env:computername,
    [switch]$auto,
    [switch]$manual
)

$ObjModel              = [Reflection.Assembly]::LoadFrom($env:WTTSTDIO + "microsoft.windows.Kits.Hardware.objectmodel.dll")
$ObjModel_DbConnection = [Reflection.Assembly]::LoadFrom($env:WTTSTDIO + "microsoft.windows.Kits.Hardware.objectmodel.dbconnection.dll")
$ObjModel_Submission   = [Reflection.Assembly]::LoadFrom($env:WTTSTDIO + "microsoft.windows.Kits.Hardware.objectmodel.submission.dll")

Clear-Host
Write-Host
Write-Host "Usage: C:\Windows\SysWOW64\WindowsPowerShell\v1.0\powershell.exe -file auto_HCK.ps1 [-controller ControllerInstanceNetworkName] [-auto] [-manual]"
Write-Host

############# Basic settings ################
# Project name
$PrjName = "TEST-REPLACE"
# HW ID: Set the hardware to test (vendor, etc.)
[string[]]$HardwareIds = "DEVICE-REPLACE" # Format: "PCI\VEN_XXXX&DEV_XXXX&SUBSYS_XXXXXXXX"
# Test machines names
$TestMachineOne = "CL1-REPLACE"
$TestMachineTwo = "CL2-REPLACE"
#############################################

# Get time
$StartTime = Get-Date -Format yyyy_MM_dd_HH-mm-ss

# Notify if this machine is the controller
$ControllerName = $controller
if ($ControllerName -eq $env:computername)
{
    write-host "Trying to use this machine [$ControllerName] as HCK  Controller"
}

# Connect to server (controller)
write-host Connecting to $ControllerName ...
$Manager = new-object -typename Microsoft.Windows.Kits.Hardware.ObjectModel.DBConnection.DatabaseProjectManager -Args $ControllerName, DTMJobs

# Get machine pools
$RootPool = $Manager.GetRootMachinePool()
$DefaultPool = $RootPool.DefaultPool

# Look for a pool that is called the same as our project, if not found - create one.
$TestPoolFlag = 0
$RootPool.GetChildPools() | foreach {
    if ($_.Name -eq $PrjName)
    {
        write-host "The pool"$_.Name"was already found in Root Pool"
        $TestPool = $_
        $TestPoolFlag = 1
    }
    else
    {
        $CurrPool = $_
        $CurrPool.GetMachines() | foreach {
            write-host "Moving machine"$_.Name" to the Default Pool."
            $CurrPool.MoveMachineTo($_, $DefaultPool)
        }
    }
}
if ($TestPoolFlag -eq "0")
{
    $TestPool=$RootPool.CreateChildPool($PrjName)
}

# Look for a project that is called the same as our project, if not found - create one.
$ProjectFlag = 0
$Manager.GetProjectNames() | foreach {
    if ($_ -eq $PrjName+"_"+$StartTime)
    {
        write-host "The project"$_.Name"was found already"
        $Project = $Manager.GetProject($PrjName+"_"+$StartTime)
        $ProjectFlag = 1
    }
}

if ($ProjectFlag -eq "0")
{
    $Project = $Manager.CreateProject($PrjName+"_"+$StartTime)
    write-host $Project.Name was created
}

# The same with devices...
$DeviceFamilyFlag = 0
$Manager.GetDeviceFamilies() | foreach {
    Write-Host Manager.GetDeviceFamilies = $_.name
    if ($_.name -eq $PrjName)
    {
        $DeviceFamily = $_
        $DeviceFamilyFlag = 1
    }
}

write-host DeviceFamily = $DeviceFamily.Name
if ($DeviceFamilyFlag -eq "0")
{
    $DeviceFamily = $Manager.CreateDeviceFamily($PrjName, $HardwareIds)
}

# Move wanted machines to pool.
write-host "Allocate machines for tests"
write-host The wanted machines are: $TestMachineOne and $TestMachineTwo
write-host "(If this is wrong - please edit the script.)"
$DefaultPool.GetMachines() | foreach {
    write-host "Found Machine"$_.Name"in the Default Pool."
    if ($_.Name -eq $TestMachineOne -or
        $_.Name -eq $TestMachineTwo
    )
    {
        write-host Adding machine $_.Name to pool $TestPool.Name
        $DefaultPool.MoveMachineTo($_, $TestPool)
    }
}

# Try to set machines to Ready state
$AmountOfMachinesInPool = 0
$TestPool.GetMachines() | foreach {
    if( $_.Name -ne $null )
    {
        write-host Transit machine $_.Name to Ready state
        if($_.SetMachineStatus([Microsoft.Windows.Kits.Hardware.ObjectModel.MachineStatus]::Ready, 60000) -ne $true)
        {
            write-host ERROR: Machine $_.Name failed to start
            $TestPool.MoveMachineTo($_, $DefaultPool)
        }
        else
        {
            $AmountOfMachinesInPool++
        }
    }
    else
    {
        write-host ERROR: Some kind of stealth machine found
        $RootPool.DeleteChildPool($TestPool.Name)
        exit 10
    }
}


if( $AmountOfMachinesInPool -gt "1")
{
    $ClientMachine = $TestPool.GetMachines()[1]
}
else
{
    $ClientMachine = $null
}

$ProductInstaceFlag = 0
$Project.GetProductInstances() | foreach {
    if ( $_.Name -eq $PrjName ) {

        write-host Product Instance with $_.Name was found
        $ProductInstance = $_
        $ProductInstaceFlag = 1
    }
}

if ( $ProductInstaceFlag -eq "0" ) {
    if ($Project.CanCreateProductInstance($PrjName, $TestPool, $TestPool.GetMachines()[0].OSPlatform)) {

        $ProductInstance = $Project.CreateProductInstance($PrjName, $TestPool, $TestPool.GetMachines()[0].OSPlatform)

    } else {
        write-host ERROR: Cannot create product instance
        $RootPool.DeleteChildPool($TestPool.Name)
        exit 11
    }
}

write-host Clean up Product Instance from Target Families
$ProductInstance.GetTargetFamilies() | foreach {
    write-host Deleting target family $_.Name
    $ProductInstance.DeleteTargetFamily($_)
}

write-host "Create new Target Family for Device Family"$DeviceFamily.Name
$TargetFamily = $ProductInstance.CreateTargetFamily($DeviceFamily)


$ProductInstance.FindTargetFromDeviceFamily($DeviceFamily) | foreach {
    write-host "Trying to add target"$_.Name.ToString()"to Target Family"
    if ($TargetFamily.IsValidTarget($_)) {
         $TargetFamily.CreateTarget($_) > $null;
    } else {
        write-host Target $_.Name is incompatible with Target Family
        $RootPool.DeleteChildPool($TestPool.Name)
        exit 12
    }
}

# Add tests to queue
write-host "Schedule tests ..."
$RemainedAmountOfScheduledTests = 0
$ProductInstance.GetTests() | foreach {
    write-host $_.Name.ToString()"| Mode:"$_.TestType"| Config?"$_.RequiresSpecialConfiguration
    if (
        ## Name filtering (commented out by default - serves only as an example)
        #(
        #$_.Name -eq "PCI Hardware Compliance Test For a Single Device (PCIHCT)" -or
        #$_.Name -eq "DXVA High Definition Video Processing - CreateVideoSurface" -or
        #$_.Name -eq "DevFund Broker Test" -or
        #$_.Name -eq "DF - Fuzz Misc API Test (Certification)"
        #) -and
        ## Choose which tests to run based on manual/requires special configuration/requires supplemental content
        (
        #!($_.RequiresSpecialConfiguration) -and #Commented out by default - uncomment if needed
        #!($_.RequiresSupplementalContent) -and #Commented out by default - uncomment if needed
        (($_.TestType -ne "Manual") -and $auto) -or
        (($_.TestType -eq "Manual") -and $manual) -or
        (!($auto) -and !($manual))
        )
    )
    {
        if ( $_.Name.Contains("[2 Machine]") )
        {
            if( $ClientMachine -ne $null)
            {
                $Role = $_.GetMachineRole()
                $Role.Roles[1].AddMachine($ClientMachine)
                $_.QueueTest($Role) > $null;
                $RemainedAmountOfScheduledTests++
            }
            else
            {
                write-host There is only one machine in $TestPool.Name, test will not run
            }
        }
        else
        {
            $_.QueueTest() > $null;
            $RemainedAmountOfScheduledTests++
        }
    write-host "["$_.Name.ToString()"] was scheduled!"
    }
}

$TotalTestsQueued = $RemainedAmountOfScheduledTests

# Inform of advancement.
write-host Scheduled $RemainedAmountOfScheduledTests.ToString() tests
while ($RemainedAmountOfScheduledTests -ne 0) {
    $CurrentTestStatus = "Running ..."
    $EstimatedRemainTime = 0
    $CurrentAmountOfTestsInQueue = 0
    $ProductInstance.GetTests() | foreach {
        if ($_.ExecutionState -eq [Microsoft.Windows.Kits.Hardware.ObjectModel.ExecutionState]::InQueue){
            # write-host $_.Name is still in Queue
            $CurrentAmountOfTestsInQueue++
            $EstimatedRemainTime += $_.EstimatedRuntime
        } elseif ($_.ExecutionState -eq [Microsoft.Windows.Kits.Hardware.ObjectModel.ExecutionState]::NotRunning){

        } elseif ($_.ExecutionState -eq [Microsoft.Windows.Kits.Hardware.ObjectModel.ExecutionState]::Running){
            $CurrentTestStatus = "Running... "+$_.Name
        } else {
            write-host $_.Name is in wrong state
        }
    }

    if ( $CurrentAmountOfTestsInQueue -lt $RemainedAmountOfScheduledTests ) {
        $CompletedTests = $TotalTestsQueued - $CurrentAmountOfTestsInQueue
        write-host Completed $CompletedTests.ToString() from $TotalTestsQueued.ToString() tests
        write-host Estimated time to run: $EstimatedRemainTime.ToString()
        $RemainedAmountOfScheduledTests = $CurrentAmountOfTestsInQueue
    }
    else
    {
        write-host $CurrentTestStatus
    }
    Start-Sleep -s 5
}

$HckResultFileName = (Get-Item -Path ".\" -Verbose).FullName+"\"+$PrjName+"-Results_"+$StartTime+".hckx"
$TxtResultFileName = (Get-Item -Path ".\" -Verbose).FullName+"\"+$PrjName+"-Results_"+$StartTime+".txt"
$HckPackageWriter = new-object -typename Microsoft.Windows.Kits.Hardware.ObjectModel.Submission.PackageWriter -Args $Project
$HckPackageWriter.Save($HckResultFileName);
$HckPackageWriter.Dispose();
write-host "Test results: "
write-host "              "$HckResultFileName

$TxtFile = new-item $TxtResultFileName -type file -force
$ProductInstance.GetTests() | foreach {
    $TempString = $_.Name+":"+$_.Status.ToString()
     add-content -Path $TxtResultFileName -Value $TempString -Force
}
write-host "Brief log: "
write-host "           "$TxtResultFileName
exit 0
