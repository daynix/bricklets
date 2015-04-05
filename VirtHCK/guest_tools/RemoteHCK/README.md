Purpose
===============
* This allows to automate the steps to configure the guest machines (HCK Controller and Clients) that are described in the [VirtHCK Wiki](https://github.com/daynix/bricklets/wiki/VirtHCK-bricklet-documentation#Checklist_for_a_new_studio_VM). Meaning, given a [VirtHCK](https://github.com/daynix/bricklets/wiki/VirtHCK-bricklet-documentation) setup of machines with freshly installed copies of Windows<sup>&reg;</sup>, one executable takes care of all the needed preparations and the installation of the HCK Controller/Client.
* Running tests from the host machine, cancelling the need to bring up the HCK Controller machine UI after the installation was done (allows also to shut it down remotely).

Prerequisites
===============
* On the Linux<sup>&trade;</sup> host the following is needed:
  1. [Winexe](http://sourceforge.net/projects/winexe)
  2. Samba server.
* In the Samba Share directory, that is shared between the host and the guests (which can be configured in [hck_setup.cfg](https://github.com/daynix/bricklets/blob/master/VirtHCK/hck_setup.cfg) or via the `SHARE_ON_HOST` environment variable) the following files are required:
  1. The **complete** installation directory of the HCK Controller (the "Hardware Certification Kit" directory). It can be obtained by [downloading](https://msdn.microsoft.com/en-us/windows/hardware/hh833788.aspx) the installation file, running it on a Windows<sup>&reg;</sup> machine (possibly the intended HCK Controller itself) and choosing the option to download for installation on a separate computer.
  2. The latest HCK Filters ("HCKFilterUpdates.cab" file) that can be downloaded from [here](https://msdn.microsoft.com/en-us/library/windows/hardware/hh998024.aspx).
  3. **Bginfo.exe**, that can be downloaded from [here](https://technet.microsoft.com/en-us/library/bb897557.aspx).
* On the Controller and the Client machines:
  * Just Windows<sup>&reg;</sup> PowerShell, of version 3.0 or above is needed. This is integrated in Windows<sup>&reg;</sup> 8, Windows Server<sup>&reg;</sup> 2012, and above. If using with Windows<sup>&reg;</sup> 7 or Windows Server<sup>&reg;</sup> 2008, it needs to be installed separately. Earlier versions are not supported.

Usage
===============
### Machine Preparation
1. Prepare a 3 separate (**not** copied) freshly installed images of the Windows<sup>&reg;</sup> OS, and launch VirtHCK.
2. in the [RemoteHCK.sh](https://github.com/daynix/bricklets/blob/master/VirtHCK/guest_tools/RemoteHCK/RemoteHCK.sh) file, set the required parameters, such as the HCK Controller external IP, the **desired** (not current) names of the Clients, the Administrator password for the Windows<sup>&reg;</sup> guests, the device to be tested, HCK version, and the project name.
  * **Warning: do not use the same project name to test different devices!** This will lead to the need of manual intervention, and may require client reinstallation!
3. Run `./RemoteHCK.sh setup` on the host machine. This will prepare the needed files and copy them to the Samba share directory.
4. On the HCK Controller machine, go to the Samba share (`\\192.168.101.1\qemu`, by default) and run **SYS_SETUP.bat**. This will prepare the machine, reboot it, and start the installation of the HCK Controller. The installation process will take a **very** long time (may be over an hour) and no progress bar is seen. Please use patience.
5. After the installation of the Controller is complete, go to the Samba share on the client machines, and run **SYS_SETUP.bat** as well. This will prepare the machines and install the HCK Client on them. Again, this will take a long time, but the installation of both clients can be done simultaneously.

### Running Tests From The Host
1. Repeat points 2-3 of the above. **Please mind the warning in point 2!!!**
2. Run `./RemoteHCK.sh run` on the host machine. This will begin the tests, and the progress will be shown. The tests may take a very long time.
3. The test results (**.hckx** and **.txt** files) will be available in the Samba share directory on the host (as well as on the Controller and the clients).

### Shutting Down The HCK Controller
* To shut down the controller, run `./RemoteHCK.sh shutdown-studio`
