@echo off

echo Installing to %COINDIR%...

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

REM **********************************************************************
REM * Create all the directories for the installed files

call create-directories.bat

REM **********************************************************************
REM * Copy files

echo Installing header files...
call install-headers.bat

echo Installing data files...
xcopy ..\..\data\draggerDefaults\*.iv %COINDIR%\data\draggerDefaults\ /R /Y

echo Installing binaries...
xcopy coin3d.dll %COINDIR%\bin\ /R /Y
xcopy Debug\coin3d.pdb %COINDIR%\bin\ /R /Y
xcopy Debug\coin3d.lib %COINDIR%\lib\ /R /Y

echo Done.

