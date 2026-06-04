@REM (C) 2026 madoodia.com
@REM ---------------------
@REM Windows Build System
@REM Build system for the Engine

@ECHO OFF


SET ROOT_DIR=%1
SET INPUT=%2


ECHO "BUILDING All with configuration: %BUILD_CONFIG%"
ECHO.

PUSHD Engine
CALL build.bat %ROOT_DIR% %INPUT%
POPD
IF %ERRORLEVEL% NEQ 0 (
	ECHO "Failed to build Engine"
	EXIT /B %ERRORLEVEL%
)

PUSHD Sandbox
CALL build.bat %ROOT_DIR% %INPUT%
POPD
IF %ERRORLEVEL% NEQ 0 (
	ECHO "Failed to build Sandbox"
	EXIT /B %ERRORLEVEL%
)




