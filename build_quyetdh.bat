::**********************************************************************************
::
:: Copyright (c) 2017
:: Lumi, JSC.
:: All Rights Reserved
::
::
:: Description:
::
:: Author: ThangDH
::
:: Last Changed By:  QuyetDH
:: Revision:         1.10
:: Last Changed:     28/01/2019
::
::**********************************************************************************
:: @ DEVICE_BUIL	:Define list of device need to build (DIMT DIMM TS611....)
:: @ SDK_VER		:SDK Version (SDK_06_71_03 SDK_06_81_01)
:: @ VER 			:Version
:: @ FREQ_BUILD		:Define list frequency build (EU US IL IN ANZ)
:: @ LIBRARY 		:(slave_routing  slave_enhanced_232)
:: @ BOOT 			:(BOOTLOADER_ENABLED nonBOOT)
::**********************************************************************************
@ECHO OFF
CLS

SET DEVICE_BUIL=TOUCH_CURTAIN_X2
SET VER=V240
SET FREQ_BUILD=ANZ CN EU HK IL IN JP KR MY RU US
SET LIBRARY=slave_enhanced_232
SET BOOT=BOOTLOADER_ENABLED
SET SECURITY=NON_SECURE

::mk clean
ECHO.
SET oldpath=%path%
SET path=%KEILPATH%\bin;%TOOLSDIR%\Python;%path%

SET BUILD_NUMBER=52445

%TOOLSDIR%\Make\make clean

SET path=%oldpath%
SET oldpath=
::

SET start=%TIME%
ECHO ----------------------------------------------------------------------
ECHO                       START BUILDING %DEVICE_BUIL%_%VER%
ECHO ----------------------------------------------------------------------
FOR %%c IN (%DEVICE_BUIL%) DO (
    IF EXIST Makefile.StickyTarget DEL Makefile.StickyTarget
    IF EXIST Makefile.variants DEL Makefile.variants
    IF EXIST Makefile.tmp DEL Makefile.tmp
    IF NOT EXIST Makefile (
		ECHO Makefile not found 
        PAUSE
		EXIT)
		
    RENAME Makefile Makefile.tmp
    (FOR /F "delims=" %%s IN (Makefile.tmp) DO (
        SET line=%%s
        SET /A "FLAGSAVE = 0"
        SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO !line!>tmpfile
        FINDSTR /M "APP_NAME:=" tmpfile>NUL
        IF ERRORLEVEL 1 (SET /A "FLAGSAVE = 1") ELSE (SET line=APP_NAME:=%%c_%VER%)
        ECHO(!line!
        ENDLOCAL
    ))>Makefile
    DEL tmpfile
    DEL Makefile.tmp

	::Start build
    FOR %%x IN (%FREQ_BUILD%) DO (
		ECHO.
		ECHO.
		ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        ECHO BUILDING %%c VER=%VER% FREQUENCY=%%x LIBRARY=%LIBRARY% BOOT=%BOOT%
		ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		ECHO.
		ECHO.
		CMD /c mk "FREQUENCY=%%x" "LIBRARY=%LIBRARY%" "BOOT=%BOOT%" "SECURITY=%SECURITY%"
		)
)

ECHO.
ECHO.
ECHO ----------------------------------------------------------------------
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO ----------------------------------------------------------------------
ECHO BUILD COMPLETED
ECHO START BUILD AT %start%
ECHO STOP  BUILD AT %TIME%
ECHO.

:: Remove folder if exits
IF EXIST %cd%\program\%VER%  RMDIR /S /Q %cd%\program\%VER%

:: Make folder %cd%\program
MKDIR %cd%\program\%VER%

:: copy all file *.hex and *.otz to folder %cd%\program
FOR /R %cd%\build %%x IN (*.hex) DO (XCOPY %%x %cd%\program\%VER% /c /q |find /v "File(s) copied") 
FOR /R %cd%\build %%x IN (*.otz) DO (XCOPY %%x %cd%\program\%VER% /c /q |find /v "File(s) copied")
FOR /R %cd%\build %%x IN (*.ota) DO (XCOPY %%x %cd%\program\%VER% /c /q |find /v "File(s) copied")

IF EXIST Makefile.StickyTarget DEL Makefile.StickyTarget
IF EXIST Makefile.variants DEL Makefile.variants

::show all file in folder \program
DIR program\%VER% /b

ECHO.
ECHO ----------------------------------------------------------------------
ECHO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECHO ----------------------------------------------------------------------
PAUSE