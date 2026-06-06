// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "Core/application.h"

struct Game
{
    ApplicationConfig AppConfig;

    b8 (*Initialize)(struct Game* GameInstance);
    b8 (*Update)(struct Game* GameInstance, f32 deltaTime);
    b8 (*Render)(struct Game* GameInstance, f32 deltaTime);
    void (*OnResize)(struct Game* GameInstance, u32 width, u32 height);
    void* State;
};
