// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

#define GEASSERTS_ENABLED

#ifdef GEASSERTS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define DebugBreak() __debugbreak()
#else
#define DebugBreak() __builtin_trap()
#endif

GEAPI void ReportFailure(const char* Expression, const char* Message, const char* File, i32 line);

#define GEASSERT(Expr)                                    \
    {                                                     \
        if (Expr)                                         \
        {                                                 \
        }                                                 \
        else                                              \
        {                                                 \
            ReportFailure(#Expr, "", __FILE__, __LINE__); \
            DebugBreak();                                 \
        }                                                 \
    }

#define GEASSERT_MSG(Expr, Message)                            \
    {                                                          \
        if (Expr)                                              \
        {                                                      \
        }                                                      \
        else                                                   \
        {                                                      \
            ReportFailure(#Expr, Message, __FILE__, __LINE__); \
            DebugBreak();                                      \
        }                                                      \
    }

#ifdef _DEBUG
#define GEASSERT_DEBUG(Expr)                              \
    {                                                     \
        if (Expr)                                         \
        {                                                 \
        }                                                 \
        else                                              \
        {                                                 \
            ReportFailure(#Expr, "", __FILE__, __LINE__); \
            DebugBreak();                                 \
        }                                                 \
    }
#else
#define GEASSERT_DEBUG(Expr)
#endif
#else
#define GEASSERT(Expr)          // Do nothing
#define GEASSERT(Expr, Message) // Do nothing
#define GEASSERT_DEBUG(Expr)    // Do nothing

#endif // GEASSERTS_ENABLED
