@REM (C) 2026 madoodia.com
@REM ---------------------

@REM Profiling the Engine with Visual Studio

@echo off

SET ROOT_DIR=%1

@REM first add devenv to the system environment
@REM E:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE
@REM Call devenv --version
Call devenv %ROOT_DIR%\Bin\Debug\Sandbox.exe
