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
for /f "Tokens=*" %%i in ('CD') do set CURDIR=%%i
SET CURDIR=%CURDIR:\=;%
SET WBLD_DIR=VSLinux

for /F "tokens=* delims=;" %%i IN ("%CURDIR%") DO CALL :LAST_SUBDIR %%i

ECHO "Building %VCXPROJ_FNAME%..."
perl %WBLD_DIR%\dnx_list_files.pl . %WBLD_DIR%\dnx_list_files.cfg %WBLD_DIR%\dnx_list_files.buildcfg %VCXPROJ_FNAME% %WBLD_DIR%\VS.vcxproj.cfg.template %WBLD_DIR%\VS.vcxproj.cfg_list.template > %VCXPROJ_FNAME% < %WBLD_DIR%\VS.vcxproj.template
perl %WBLD_DIR%\dnx_list_files.pl . %WBLD_DIR%\dnx_list_files.cfg %WBLD_DIR%\dnx_list_files.buildcfg %VCXPROJ_FNAME% %WBLD_DIR%\VS.vcxproj.cfg.template %WBLD_DIR%\VS.vcxproj.cfg_list.template > %VCXPROJ_FILTERS_FNAME% < %WBLD_DIR%\VS.vcxproj.filters.template
ECHO "%VCXPROJ_FNAME% ready"

SET CURDIR=
SET VCXPROJ_FNAME=
SET SLN_FNAME=
SET VCPXROJ_FILTERS_FNAME=

:LAST_SUBDIR
IF "%1"=="" (
  GOTO :EOF
)

SET VCXPROJ_FNAME=VS_[%1]_%USERNAME%.vcxproj
SET VCXPROJ_FILTERS_FNAME=%VCXPROJ_FNAME%.filters
SET SLN_FNAME=VS_[%1]_%USERNAME%.sln
SHIFT

GOTO :LAST_SUBDIR
