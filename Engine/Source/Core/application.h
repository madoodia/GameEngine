// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

struct ApplicationConfig
{
    i16 WindowX;
    i16 WindowY;
    i16 WindowWidth;
    i16 WindowHeight;

    const char* Name;
};

/**
 * @brief
 *
 * @param Config Pointer to the application configuration structure containing window properties and application name
 * @return boolean value indicating success or failure of the startup process
 */
GEAPI b8 ApplicationCreate(ApplicationConfig* Config);
GEAPI b8 ApplicationRun();
