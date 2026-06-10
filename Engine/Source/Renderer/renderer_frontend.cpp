// (C) 2026 madoodia.com
// ---------------------

#include "renderer_frontend.h"
#include "renderer_backend.h"

#include "Core/logger.h"
#include "Core/ge_memory.h"

static RendererBackend* Backend = 0;

b8 InitializeRenderer(const char* ApplicationName, PlatformState* PState)
{
    Backend = (RendererBackend*)AllocateMemory(sizeof(RendererBackend), MEMORY_TAG_RENDERER);

    // TODO: Make selecting Renderer, configurable
    CreateRendererBackend(RENDERER_BACKEND_VULKAN, PState, Backend);

    Backend->FrameNumber = 0;

    if (!Backend->Initialize(Backend, ApplicationName, PState))
    {
        GEFATAL("Failed to initialize backend renderer! Shutting Down");
        return FALSE;
    }

    return TRUE;
}

void ShutdownRenderer()
{
    Backend->Shutdown(Backend);
    FreeMemory(Backend, sizeof(RendererBackend), MEMORY_TAG_RENDERER);
}

void RendererOnResize(u16 Width, u16 Height)
{
}

b8 RendererBeginFrame(f32 DeltaTime)
{
    return Backend->BeginFrame(Backend, DeltaTime);
}

b8 RendererEndFrame(f32 DeltaTime)
{
    b8 Result = Backend->EndFrame(Backend, DeltaTime);
    Backend->FrameNumber++;

    return Result;
}

b8 RendererDrawFrame(RenderPacket* Packet)
{
    if (RendererBeginFrame(Packet->DeltaTime))
    {
        b8 Result = RendererEndFrame(Packet->DeltaTime);

        if (!Result)
        {
            GEERROR("RendererEndFrame failed. Shutting Down!");
            return FALSE;
        }
    }

    return TRUE;
}
