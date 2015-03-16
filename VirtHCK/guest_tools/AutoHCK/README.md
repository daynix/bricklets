Authorship
===============
* Inspired by the [whql_powershell](https://github.com/vipmike007/whql_powershell) kit by Mike Cao <vipmike007@gmail.com>  
* Written and modified for VirtHCK by Igor Derzhavets <igord@daynix.com>  
* Modification and further automation by Leonid Bloch <leonid@daynix.com>

License
===============
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; under version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, see http://www.gnu.org/licenses

Usage
===============
This script runs in 64-bit Windows PowerShell (PS), version 3.0 (2012) and above.
In order to be able to execute any script in PS, script execution should be enabled,
by running:  
`Set-ExecutionPolicy RemoteSigned`  
As the Administrator, first. There is no need to repeat this one-time action on
the same computer and on the same version of PS again.

Before using the script, several parameters should be edited in it. All parameters
that should be edited are enclosed in the section marked: `# Basic settings #`:  
1. `$PrjName` should be set to a name that you would like to give to the project.  
2. `[string[]]$HardwareIds` should be set to the hardware ID of the tested hardware,
as can be fount in the HCK GUI. This string should be in the form of:
`"PCI\VEN_XXXX&DEV_XXXX&SUBSYS_XXXXXXXX"`
i.e. from the beginning of the string, to the `&REV` part (not including).  
3. `$TestMachineOne` and `$TestMachineTwo` should be set to the network names of the
machines to be tested.

*Setting these in-script parameters is mandatory!*

The script has 3 optional command line parameters, and it should be run this way:  
`.\auto_HCK.ps1 [-controller ControllerName] [-auto] [-manual]`  
The parameters in the square brackets are optional, and should be used for:  
* `-controller <name>` – to specify the network name of the HCK controller. If this parameter is absent, the script will try to use the computer it runs on as the controller.
* `-auto` – run only the tests that do not require manual intervention.
* `-manual` – run only the tests that require manual intervention.

Note that:
* If `-auto` and `-manual` are both present, or both absent, all the available tests will run.
* Some tests that require manual intervention may pass without actually needing it.

Before the tests run, all the scheduled tests and their count are presented.
While running, progress is displayed by printing the current status every few
seconds.

Note, that it is recommended to use this script with a freshly installed
HCK studio (that manual tests didn’t run on it before) because the settings that
were set manually can interfere. If problems arise, try to delete all the machines
from all the pools, delete all the pools besides the Default Pool, and delete all
the projects, before attempting again.

The output (test results, in .hckx and .txt formats) will be saved to the same folder
that the script was run from, under the name that consists of the project name and the
date & time when the run began.
