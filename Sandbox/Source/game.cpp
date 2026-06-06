// (C) 2026 madoodia.com
// ---------------------

#include "game.h"

#include <Core/logger.h>

b8 GameInitialize(Game* GameInstance)
{
    GEDEBUG("Game Initialization Started");
    return TRUE;
}

b8 GameUpdate(Game* GameInstance, f32 DeltaTime)
{
    GEDEBUG("Game Update - DeltaTime: %f", DeltaTime);
    return TRUE;
}

b8 GameRender(Game* GameInstance, f32 DeltaTime)
{
    GEDEBUG("Game Render - DeltaTime: %f", DeltaTime);
    return TRUE;
}

void GameOnResize(Game* GameInstance, u32 Width, u32 Height)
{
    GEINFO("Window resized to %ux%u", Width, Height);
}
