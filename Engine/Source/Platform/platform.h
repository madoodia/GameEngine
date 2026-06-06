// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

struct PlatformState
{
    void* InternalState;
};

b8 PlatformStartUp(
    PlatformState* PState,
    const char* ApplicationName,
    i32 X,
    i32 Y,
    i32 Width,
    i32 Height);

void PlatformShutdown(PlatformState* PState);

b8 PlatformPumpMessage(PlatformState* PState);

void* PlatformAllocate(u64 Size, b8 Aligned);
void PlatformFree(void* Block, b8 Aligned);
void* PlatformZeroMemory(void* Block, u64 Size);
void* PlatformCopyMemory(void* Destination, const char* source, u64 Size);
void* PlatformSetMemory(void* Destination, i32 Value, u64 Size);

void PlatformConsoleWrite(const char* Message, u8 Colour);
void PlatformConsoleWriteError(const char* Message, u8 Colour);

f64 PlatformGetAbsoluteTime();

// MilliSecond
void PlatformSleep(u64 MSec);
