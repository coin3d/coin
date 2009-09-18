@echo off

if %1 == msvc6 (
echo MSVC6
call "C:\Program Files\Microsoft Visual Studio\VC98\Bin\VCVARS32.BAT"
) else (
if %1 == msvc7 (
echo MSVC7
call "C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"
) else (
if %1 == msvc8 (
echo MSVC8
call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"
) else (
if %1 == msvc9 (
echo MSVC9
call "c:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
) else (
echo ERROR
goto END
))))

bash generate-compiler.sh %1

:END