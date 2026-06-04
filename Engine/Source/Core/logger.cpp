// (C) 2026 madoodia.com
// ---------------------

#include "Core/logger.h"
#include "Core/asserts.h"
#include "Platform/platform.h"

// TEMP:
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 InitializeLogging()
{
    // TODO: Create Log File

    return TRUE;
}

void ShutdownLogging()
{
    // TODO: Cleanup Logging/Write Queued Entries
}

void LogOutput(LogLevel Level, const char* Message, ...)
{
    const char* Levels[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARNING]: ",
        "[INFO]: ",
        "[DEBUG]: ",
        "[TRACE]: ",
    };

    b8 IsError = Level < LOG_WARNING;

    // Stack memory allocation is faster than heap allocation
    const i32 MsgLength = 32000;
    char OutputMessage[MsgLength];
    memset(OutputMessage, 0, sizeof(OutputMessage));

    // va_list ArgPtr;           // NOTE: Microsoft will do some changes that make error
    __builtin_va_list ArgPtr; // Better for clang and gcc
    va_start(ArgPtr, Message);
    vsnprintf(OutputMessage, MsgLength, Message, ArgPtr);
    va_end(ArgPtr);

    char FinalMessage[MsgLength];
    sprintf(FinalMessage, "%s%s\n", Levels[Level], OutputMessage);

    // Platform Specific Output
    if (IsError) {
        PlatformConsoleWriteError(FinalMessage, Level);
    } else {
        PlatformConsoleWrite(FinalMessage, Level);
    }
}

void ReportFailure(const char* Expression, const char* Message, const char* File, i32 line)
{
    LogOutput(LOG_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", Expression, Message, File, line);
}
