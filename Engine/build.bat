@REM (C) 2026 madoodia.com
@REM ---------------------
@REM Windows Build System
@REM Build system for the Engine

@ECHO OFF

SET ROOT_DIR=%1
SET INPUT=%2

REM Set the build configuration (Debug/Release)
SET BUILD_CONFIG=Debug
SET BUILD_FLAG=_DEBUG
@REM -g: debug information, -shared: create a shared library dll, -Wvarargs: warn about incompatible pointer types, -Wall: enable all warnings, -Werror: treat warnings as errors
SET COMPILER_FLAGS=-g -shared -Wvarargs -Wall -Werror -std=c++20

IF "%INPUT%"=="1" (
	SET BUILD_CONFIG=Release
	SET BUILD_FLAG=_RELEASE
	SET COMPILER_FLAGS=-shared -Wvarargs -Wall -Werror -std=c++20
)

SETLOCAL ENABLEDELAYEDEXPANSION

@REM Get the list of .cpp files in the Source directory
SET SOURCE_FILES=
FOR /R %%F IN (*.cpp) DO (
	SET SOURCE_FILES=!SOURCE_FILES! "%%F"
)

if not exist "%ROOT_DIR%\Bin\%BUILD_CONFIG%" (
	mkdir "%ROOT_DIR%\Bin\%BUILD_CONFIG%"
)

SET PROJECT=Engine
SET INCLUDE_FLAGS=-ISource -I%VULKAN_SDK%/Include
SET LIBRARY_FLAGS=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
SET DEFINES=-D%BUILD_FLAG% -DGEEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO --= Building Engine... =--
clang++ %SOURCE_FILES% %COMPILER_FLAGS% %INCLUDE_FLAGS% %LIBRARY_FLAGS% %DEFINES% -o %ROOT_DIR%/Bin/%BUILD_CONFIG%/%PROJECT%.dll

