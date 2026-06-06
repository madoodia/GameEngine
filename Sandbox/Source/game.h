// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include <defines.h>
#include <game_types.h>

struct GameState
{
    f32 DeltaTime;
};

b8 GameInitialize(Game* GameInstance);
b8 GameUpdate(Game* GameInstance, f32 DeltaTime);
b8 GameRender(Game* GameInstance, f32 DeltaTime);
void GameOnResize(Game* GameInstance, u32 Width, u32 Height);
