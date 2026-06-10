// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>

struct VulkanContext
{
    VkInstance Instance;
    VkAllocationCallbacks* Allocator;
};
