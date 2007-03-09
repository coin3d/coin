@echo off

set libname=coin3

rem ************************************************************************
rem * check script arguments

if "%1"=="dll" goto argonegiven
if "%1"=="lib" goto argonegiven
goto argproblem

:argonegiven
if "%2"=="release" goto argtwogiven
if "%2"=="debug" goto argtwogiven
goto argproblem

:argtwogiven
goto argtestdone

:argproblem
echo Error with script arguments %1 %2.
echo Usage:
echo   install-sdk.bat dll/lib release/debug
exit

:argtestdone

rem ************************************************************************
rem * check environment variables

if not "%COINDIR%"=="" goto coindirset
echo The COINDIR environment variable must be set to point to a directory
echo to be able to perform the installation procedure.
exit

:coindirset
if exist %COINDIR%\*.* goto coindirexists
echo The COINDIR environment variable must point to an existing directory
echo to be able to perform the installation procedure.
exit

:coindirexists

echo Installing to %COINDIR%

rem **********************************************************************
rem * Create all the directories

call ..\misc\create-directories.bat

rem **********************************************************************
rem * Copy files

echo Installing header files...
call install-headers.bat

echo Installing data files...
xcopy ..\..\data\draggerDefaults\*.iv %COINDIR%\data\draggerDefaults\ /R /Y

echo Installing binaries...

rem **********************************************************************

if "%1"=="dll" goto installdll
goto installlib

:installdll

if "%2"=="debug" goto installdlldebug
goto installdllrelease

:installdlldebug
xcopy %libname%d.dll %COINDIR%\bin\ /R /Y
xcopy %libname%d.pdb %COINDIR%\bin\ /R /Y
xcopy Debug\%libname%d.lib %COINDIR%\lib\ /R /Y
goto binariesdone

:installdllrelease
xcopy %libname%.dll %COINDIR%\bin\ /R /Y
xcopy Debug\%libname%.lib %COINDIR%\lib\ /R /Y
goto binariesdone

:installlib

if "%2"=="debug" goto installlibdebug
goto installlibrelease

:installlibdebug
xcopy StaticDebug\%libname%sd.lib %COINDIR%\lib\ /R /Y
goto binariesdone

:installlibrelease
xcopy StaticRelease\%libname%s.lib %COINDIR%\lib\ /R /Y
goto binariesdone

:binariesdone
