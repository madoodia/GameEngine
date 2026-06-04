// (C) 2026 madoodia.com
// ---------------------

#include "logger.h"
#include "asserts.h"

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

    // b8 IsError = Level < 2;

    // Stack memory allocation is faster than heap allocation
    char OutputMessage[32000];
    memset(OutputMessage, 0, sizeof(OutputMessage));

    // va_list ArgPtr;           // NOTE: Microsoft will do some changes that make error
    __builtin_va_list ArgPtr; // Better for clang and gcc
    va_start(ArgPtr, Message);
    vsnprintf(OutputMessage, 32000, Message, ArgPtr);
    va_end(ArgPtr);

    char FinalMessage[32000];
    sprintf(FinalMessage, "%s%s\n", Levels[Level], OutputMessage);

    // TODO: Platform Specific Output
    // TEMP:
    printf("%s", FinalMessage);
}

void ReportFailure(const char* Expression, const char* Message, const char* File, i32 line)
{
    LogOutput(LOG_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", Expression, Message, File, line);
}
