// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "Renderer/renderer_types.inl"

struct PlatformState;

b8 CreateRendererBackend(RendererBackendType type, struct PlatformState* PState, RendererBackend* OutRendererBackend);
void DestroyRendererBackend(RendererBackend* RendererBackend);
