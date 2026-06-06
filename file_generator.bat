@echo off
REM File Generator - Creates .h and .cpp files from a template

setlocal enabledelayedexpansion

REM Prompt for class name
set /p className="Enter class name: "

if "!className!"=="" (
    echo Error: Class name cannot be empty.
    exit /b 1
)

REM Prompt for output directory
set /p outputDir="Enter output directory (default: Engine\Source\Core): "
if "!outputDir!"=="" (
    set outputDir=Engine\Source\Core
)

REM Ensure directory exists
if not exist "!outputDir!" (
    mkdir "!outputDir!"
    echo Created directory: !outputDir!
)

REM Create header file
set headerFile=!outputDir!\!className!.h
(
    echo #pragma once
    echo.
    echo class !className!
    echo {
    echo public:
    echo     !className!^(^);
    echo     ~!className!^(^);
    echo.
    echo private:
    echo };
) > "!headerFile!"

REM Create cpp file
set cppFile=!outputDir!\!className!.cpp
(
    echo #include "!className!.h"
    echo.
    echo !className!::!className!^(^)
    echo {
    echo     // Constructor implementation
    echo }
    echo.
    echo !className!::~!className!^(^)
    echo {
    echo     // Destructor implementation
    echo }
) > "!cppFile!"

echo.
echo Files created successfully:
echo Header: !headerFile!
echo Source: !cppFile!
pause
