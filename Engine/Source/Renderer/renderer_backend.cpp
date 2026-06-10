// (C) 2026 madoodia.com
// ---------------------

#include "renderer_backend.h"

#include "Renderer/Vulkan/vulkan_backend.h"

// Here is the place to switch renderers

b8 CreateRendererBackend(RendererBackendType type, struct PlatformState* PState, RendererBackend* OutRendererBackend)
{
    OutRendererBackend->PState = PState;

    if (type == RENDERER_BACKEND_VULKAN)
    {
        OutRendererBackend->Initialize = VulkanRendererBackendInitialize;
        OutRendererBackend->Shutdown = VulkanRendererBackendShutdown;
        OutRendererBackend->Resized = VulkanRendererBackendOnResize;
        OutRendererBackend->BeginFrame = VulkanRendererBackendBeginFrame;
        OutRendererBackend->EndFrame = VulkanRendererBackendEndFrame;

        return TRUE;
    }

    if (type == RENDERER_BACKEND_DIRECTX12)
    {
        // TODO: Related Codes

        return TRUE;
    }

    if (type == RENDERER_BACKEND_OPENGL)
    {
        // TODO: Related Codes

        return TRUE;
    }

    return FALSE;
}

void DestroyRendererBackend(RendererBackend* RendererBackend)
{
    RendererBackend->Initialize = nullptr;
    RendererBackend->Shutdown = nullptr;
    RendererBackend->Resized = nullptr;
    RendererBackend->BeginFrame = nullptr;
    RendererBackend->EndFrame = nullptr;
}
