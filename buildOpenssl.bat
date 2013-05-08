@echo off
set OPENSSL=openssl-1.0.1e
set DEBUG=debug-

set TARGET=
set MAKE_TYPE=
set ASM=no-asm

set arch=%1
set debug=%2

if not "%arch%"=="Windows-x86" if not "%arch%"=="Windows-x64" (
    echo "Usage: %0 (Windows-x86|Windows-x64) (debug)"
    exit /b
)
if "%arch%"=="Windows-x86" (
    set TARGET=VC-WIN32
    set MAKE_TYPE=do_ms
)
if "%arch%"=="Windows-x64" (
    set TARGET=VC-WIN64A
    set MAKE_TYPE=do_win64a
)
if not "%debug%"=="debug" (
    set DEBUG=
)
echo TARGET: %TARGET%
echo MAKE_TYPE: %MAKE_TYPE%
if not "%DEBUG%"=="" (
    echo DEBUG: true
)
if "%DEBUG%"=="" (
    echo DEBUG: false
)

cd %OPENSSL%
perl Configure %DEBUG%%TARGET% %ASM% --prefix=..\..\build\openssl\%arch%
call ms\do_ms
nmake -f ms\ntdll.mak
rem nmake -f ms\ntdll.mak test
nmake -f ms\ntdll.mak install
cd ..