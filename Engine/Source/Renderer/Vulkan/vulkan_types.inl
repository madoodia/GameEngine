// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

#ifdef GEPLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(GEPLATFORM_LINUX)
#define VK_USE_PLATFORM_XCB_KHR
#elif defined(GEPLATFORM_MACOS)
#define VK_USE_PLATFORM_MACOS_MVK
#endif

#include <vulkan/vulkan.h>

struct VulkanDevice
{
    VkPhysicalDevice PhysicalDevice;
    VkDevice LogicalDevice;

    VkQueue GraphicsQueue, PresentQueue, TransferQueue;

    i32 GraphicsQueueFamilyIndex, PresentQueueFamilyIndex, TransferQueueFamilyIndex;

    VkPhysicalDeviceProperties Properties;
    VkPhysicalDeviceFeatures Features;
    VkPhysicalDeviceMemoryProperties MemoryProperties;

    VkSurfaceFormatKHR* Formats;
    VkPresentModeKHR* PresentModes;
    VkSurfaceCapabilitiesKHR Capabilities;
};

struct VulkanContext
{
    VkInstance Instance;
    VkAllocationCallbacks* Allocator;
    VkSurfaceKHR Surface;
    VulkanDevice Device;
};
