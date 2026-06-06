// (c) 2026 madoodia.com
// ---------------------

/*  */ #pragma once

#include "Core/application.h"
#include "Core/logger.h"
#include "Core/asserts.h"
#include "game_types.h"

// Externally-Defined Function to Create the Game Application
extern b8 CreateGame(Game* GameInstance);

/**
 * @brief Entry Point of the Application
 *
 * @return int
 */
int main()
{
    // Request a Game Instance from the Application
    Game GameInstance = {};
    if (!CreateGame(&GameInstance))
    {
        GEERROR("Failed to create game instance!");
        return -1;
    }

    // Ensure the Function Pointers exist
    if (!GameInstance.Initialize || !GameInstance.Update || !GameInstance.Render || !GameInstance.OnResize)
    {
        GEERROR("Game instance is missing required function pointers!");
        return -2;
    }

    // Initialization of the Game
    if (!ApplicationCreate(&GameInstance))
    {
        GEERROR("Failed to create application!");
        return 1;
    }

    // Main Game Loop
    if (!ApplicationRun())
    {
        GEERROR("Failed to run application!");
        return 2;
    }

    return 0;
}
