// (C) 2026 madoodia.com
// ---------------------

#pragma once

#include "Renderer/Vulkan/vulkan_types.inl"

b8 VulkanDeviceCreate(VulkanContext* Context);

void VulkanDeviceDestroy(VulkanContext* Context);

void VulkanDeviceQuerySwapchainSupport(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, VulkanDevice* OutDevice);
