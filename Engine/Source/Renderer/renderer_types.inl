// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

enum RendererBackendType
{
    RENDERER_BACKEND_VULKAN,
    RENDERER_BACKEND_DIRECTX12,
    RENDERER_BACKEND_OPENGL
};

struct RendererBackend
{
    struct PlatformState* PState;
    u64 FrameNumber;

    b8 (*Initialize)(struct RendererBackend* Backend, const char* ApplicationName, struct PlatformState* PState);
    void (*Shutdown)(struct RendererBackend* Backend);
    void (*Resized)(struct RendererBackend* Backend, u16 Width, u16 Height);
    b8 (*BeginFrame)(struct RendererBackend* Backend, f32 DeltaTime);

    b8 (*EndFrame)(struct RendererBackend* Backend, f32 DeltaTime);
};

struct RenderPacket
{
    f64 DeltaTime;
};
