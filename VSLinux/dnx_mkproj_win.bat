::
:: Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
:: All rights reserved.
::
:: Maintained by bricklets@daynix.com
::
:: This file is a part of the Bricklets library, please see the wiki page
:: on https://github.com/daynix/bricklets/wiki/Bricklets-library for more.
::
:: This code is licensed under standard 3-clause BSD license.
:: See file LICENSE supplied with this package for the full license text.
::

@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

setlocal

SET LINUX_SCRIPT=dnx_mkproj_lin
SET BUILD_ROOT=%~dp0
SET BUILD_CMD=
SET SSH_USER=%VSLINUX_SSH_USER%
SET SSH_KEY=%VSLINUX_SSH_KEY%
SET BUILD_PARAMS=

SET UNIQUE_SCRIPT_PATH=%~d0
SET VSLINUX_PATH=%~dp0
SET BUILDER_ADDR=
SET UNIQUE_SCRIPT_NAME=

IF [%SSH_USER%] == [] (
    SET SSH_USER=%USERNAME%
)

:ParseCommandLine
IF    [%1]==[]               GOTO ParseParamsContinue
IF /I [%1]==[-c]             GOTO ParamBuildCmd
IF /I [%1]==[--build-cmd]    GOTO ParamBuildCmd
IF /I [%1]==[-p]             GOTO ParamBuildPath
IF /I [%1]==[--build-path]   GOTO ParamBuildPath
IF /I [%1]==[-u]             GOTO ParamSSHUser
IF /I [%1]==[--ssh-user]     GOTO ParamSSHUser
IF /I [%1]==[-k]             GOTO ParamSSHKey
IF /I [%1]==[--ssh-key]      GOTO ParamSSHKey
IF /I [%1]==[-h]             GOTO ParamHelp
IF /I [%1]==[--help]         GOTO ParamHelp
:NextCommandLineParam
SHIFT
GOTO ParseCommandLine

:ParamBuildPath
SET BUILD_ROOT=%2
SET BUILD_ROOT=!BUILD_ROOT:\=\\!
SHIFT
GOTO NextCommandLineParam

:ParamBuildCmd
SET BUILD_CMD=%2
SHIFT
GOTO NextCommandLineParam

:ParamSSHUser
SET SSH_USER=%2
SHIFT
GOTO NextCommandLineParam

:ParamSSHKey
SET SSH_KEY=%2
SHIFT
GOTO NextCommandLineParam

:ParamHelp
CALL :ShowHelp
GOTO End

:ParseParamsContinue
CALL :ExtractRemoteAddrAndHome %~d0

IF [%BUILDER_ADDR%] == [] (
    ECHO ERROR: Can't extract BUILDER_ADDR by %~d0!
	CALL :ShowHelp
	GOTO End
)

IF [%BUILD_CMD%] == [] (
    ECHO ERROR: remote_cmd must be specified!
	CALL :ShowHelp
	GOTO End
) ELSE (
    SET BUILD_PARAMS=%BUILD_PARAMS% -x '%BUILD_CMD%'
)

IF NOT [%BUILD_ROOT%] == [] (
    SET BUILD_PARAMS=%BUILD_PARAMS% -w '%BUILD_ROOT%'
)

IF [%SSH_KEY%] == [] (
    ECHO ERROR: ppk_file be either specified in command line or by VSLINUX_SSH_KEY environment varialble!
    CALL :ShowHelp
    GOTO End
)

CALL :PrepareUniqueMKProj
plink.exe -ssh %SSH_USER%@%BUILDER_ADDR% -X -i %SSH_KEY% ". $VSLINUX_MAP_ROOT/%UNIQUE_SCRIPT_NAME% %BUILD_PARAMS%"
CALL :RollbackUniqueMKProj
GOTO End

:ExtractRemoteAddrAndHome
FOR /F "tokens=1,2,3" %%A in ('NET USE ^| FINDSTR /I %1') DO (
  FOR /F "tokens=1* delims=\" %%K in ("%%C") DO (
    SET BUILDER_ADDR=%%K
  )
)
GOTO :EOF

:PrepareUniqueMKProj
SET UNIQUE_SCRIPT_NAME=%LINUX_SCRIPT%
FOR /F "tokens=1,2,3 delims=/ " %%A in ('DATE /T') do ( 
    SET UNIQUE_SCRIPT_NAME=%UNIQUE_SCRIPT_NAME%_%%A_%%B_%%C
)
FOR /F "tokens=1,2 delims=: " %%A in ('TIME /T') do ( 
    SET UNIQUE_SCRIPT_NAME=%UNIQUE_SCRIPT_NAME%_%%A_%%B
)
COPY %VSLINUX_PATH%\%LINUX_SCRIPT% %UNIQUE_SCRIPT_PATH%\%UNIQUE_SCRIPT_NAME%
GOTO :EOF

:RollbackUniqueMKProj
DEL %UNIQUE_SCRIPT_PATH%\%UNIQUE_SCRIPT_NAME%
GOTO :EOF

:ShowHelp
    ECHO Usage dnx_mkproj_win.bat [params]
	ECHO --
	ECHO  Parameters (M - mandatory, O - optional):
	ECHO   -c [remote_cmd]
	ECHO   --build-cmd [build_cmd]   [M] Remote command to execute
	ECHO   -p [remote_path]
	ECHO   --build-path [build_path] [O] Remote path to CD before the command execution (in Windows form)
	ECHO   -u [remote_user]
	ECHO   --ssh-user [ssh_user] [O] Remote user to be used for SSH login. Could be defined by VSLINUX_SSH_USER
	ECHO                                   environment variable. Default: %SSH_USER%
	ECHO   -k [ppk_file]
	ECHO   --ssh-key-file [ppk_file]   [M] SSH Key file to use for login. Could be defined by VSLINUX_SSH_KEY
	ECHO                                   environment variable.
	ECHO   -h
	ECHO   --help              [0] Show this help
GOTO :EOF

:End

endlocal
