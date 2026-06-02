@REM (C) 2026 madoodia.com
@REM ---------------------
@REM Windows Build System
@REM Build system for the Engine

@ECHO OFF


SET ROOT_DIR=%1


ECHO "Cleaning the Project...

IF EXIST "%ROOT_DIR%\Bin" (
	RMDIR /S /Q "%ROOT_DIR%\Bin"
)

ECHO "Build completed successfully!"





