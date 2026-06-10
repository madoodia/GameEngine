// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "Renderer/renderer_types.inl"

struct StaticMeshData;
struct PlatformState;

b8 InitializeRenderer(const char* ApplicationName, PlatformState* PState);

void ShutdownRenderer();

void RendererOnResize(u16 Width, u16 Height);

b8 RendererDrawFrame(RenderPacket* Packet);

b8 RendererBeginFrame(f32 DeltaTime);
b8 RendererEndFrame(f32 DeltaTime);
