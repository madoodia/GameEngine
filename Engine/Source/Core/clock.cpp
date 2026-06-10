
// (C) 2026 madoodia.com
// ---------------------

#include "clock.h"
#include "Platform/platform.h"

void StartClock(Clock* Clk)
{
    if (Clk->StartTime != 0)
    {
        Clk->Elapsed = PlatformGetAbsoluteTime() - Clk->StartTime;
    }
}

void UpdateClock(Clock* Clk)
{
    Clk->StartTime = PlatformGetAbsoluteTime();
    Clk->Elapsed = 0;
}

void StopClock(Clock* Clk)
{
    Clk->StartTime = 0;
}
