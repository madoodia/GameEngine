// (C) 2026 madoodia.com
// ---------------------
#include "game.h"

#include <entry.h>

#include <Core/ge_memory.h>

b8 CreateGame(Game* OutGame)
{
    // Application Configuration
    OutGame->AppConfig.Name = "Game Engine Sandbox";
    OutGame->AppConfig.WindowX = 100;
    OutGame->AppConfig.WindowY = 100;
    OutGame->AppConfig.WindowWidth = 1280;
    OutGame->AppConfig.WindowHeight = 720;

    // Game Function Pointers
    OutGame->Initialize = GameInitialize;
    OutGame->Update = GameUpdate;
    OutGame->Render = GameRender;
    OutGame->OnResize = GameOnResize;

    // Create the Game State
    OutGame->State = AllocateMemory(sizeof(GameState), MemoryTags::MEMORY_TAG_GAME);

    return TRUE;
}
