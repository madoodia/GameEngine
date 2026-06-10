// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "Renderer/renderer_backend.h"

b8 VulkanRendererBackendInitialize(RendererBackend* Backend, const char* ApplicationName, struct PlatformState* PState);

void VulkanRendererBackendShutdown(RendererBackend* Backend);

void VulkanRendererBackendOnResize(RendererBackend* Backend, u16 Width, u16 Height);

b8 VulkanRendererBackendBeginFrame(RendererBackend* Backend, f32 DeltaTime);

b8 VulkanRendererBackendEndFrame(RendererBackend* Backend, f32 DeltaTime);
