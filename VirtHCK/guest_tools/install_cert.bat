::
:: Copyright (c) 2013, Daynix Computing LTD (www.daynix.com)
:: All rights reserved.
::
:: Maintained by bricklets@daynix.com
::
:: This file is a part of Bricklets library, see "Bricklets" page
:: on www.daynix.com for more.
::
:: This code is licensed under standard 3-clause BSD license.
:: See file LICENSE supplied with this package for the full license text.
::
@ECHO OFF

IF NOT EXIST certmgr.exe (
	ECHO CertMgr.exe not found in current directory
	ECHO You can get it from [Windows DDK]\bin\[arch]\ directory
	GOTO ERROR_EXIT
)

IF "%1" == "" (
	ECHO Please provide certifiacate name to install
	GOTO ERROR_EXIT
)

IF NOT EXIST %1 (
	ECHO Specified certificate file wasn't found in current directory
	GOTO ERROR_EXIT
)

@ECHO ON
.\CertMgr.exe /add %1 /s /r localMachine root
.\CertMgr.exe /add %1 /s /r localMachine trustedpublisher
@ECHO OFF

GOTO NORMAL_EXIT

:ERROR_EXIT
ECHO Failed to install certificate

:NORMAL_EXIT
