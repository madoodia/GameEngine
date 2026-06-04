// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

#define LOG_WARNING_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if GERELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

enum LogLevel
{
    LOG_FATAL = 0,
    LOG_ERROR = 1,
    LOG_WARNING = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
    LOG_TRACE = 5
};

b8 InitializeLogging();
void ShutdownLogging();

GEAPI void LogOutput(LogLevel Level, const char* Message, ...);

#define GEFATAL(Message, ...) \
    LogOutput(LOG_FATAL, Message, ##__VA_ARGS__);

#ifndef GEERROR
#define GEERROR(Message, ...) \
    LogOutput(LOG_ERROR, Message, ##__VA_ARGS__);
#endif

#if LOG_WARNING_ENABLED == 1
#define GEWARNING(Message, ...) \
    LogOutput(LOG_WARNING, Message, ##__VA_ARGS__);
#else
#define GEWARNING(Message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define GEINFO(Message, ...) \
    LogOutput(LOG_INFO, Message, ##__VA_ARGS__);
#else
#define GEINFO(Message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define GEDEBUG(Message, ...) \
    LogOutput(LOG_DEBUG, Message, ##__VA_ARGS__);
#else
#define GEDEBUG(Message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define GETRACE(Message, ...) \
    LogOutput(LOG_TRACE, Message, ##__VA_ARGS__);
#else
#define GETRACE(Message, ...)
#endif
