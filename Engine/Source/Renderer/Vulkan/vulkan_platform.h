// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

struct VulkanContext;
struct PlatformState;

void PlatformGetRequiredExtensionNames(const char*** OutExtensionNames);

b8 PlatformCreateVulkanSurface(VulkanContext* Context, PlatformState* PlatformState);
