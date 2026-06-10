// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

struct Clock
{
    f64 StartTime;
    f64 Elapsed;
};

void StartClock(Clock* Clk);
void UpdateClock(Clock* Clk);
void StopClock(Clock* Clk);
