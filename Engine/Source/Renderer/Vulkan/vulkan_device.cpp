// (C) 2026 madoodia.com
// ---------------------

#include "Renderer/Vulkan/vulkan_device.h"
#include "Core/logger.h"
#include "Core/ge_string.h"
#include "Containers/darray.h"
#include "vulkan/vulkan_core.h"

void VulkanDeviceQuerySwapchainSupport(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, VulkanDevice* OutDevice)
{
    VkResult Result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &OutDevice->Capabilities);
    u32 FormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &FormatCount, nullptr);
    if (FormatCount != 0)
    {
        // Allocate the correct capacity
        OutDevice->Formats = (VkSurfaceFormatKHR*)ReserveDArray(VkSurfaceFormatKHR, FormatCount);
        // Set the length so the DArray system knows how many elements are populated
        SetDArrayLength(OutDevice->Formats, FormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &FormatCount, OutDevice->Formats);
    }
    u32 PresentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, nullptr);
    if (PresentModeCount != 0)
    {
        OutDevice->PresentModes = (VkPresentModeKHR*)ReserveDArray(VkPresentModeKHR, PresentModeCount);
        SetDArrayLength(OutDevice->PresentModes, PresentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, OutDevice->PresentModes);
    }
}
b8 PhysicalDeviceIsSuitable(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface, VkPhysicalDeviceProperties* Properties, VkPhysicalDeviceFeatures* Features, VulkanDevice* OutDevice)
{
    u32 QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueFamilyCount, nullptr);
    VkQueueFamilyProperties* QueueFamilies = (VkQueueFamilyProperties*)ReserveDArray(VkQueueFamilyProperties, QueueFamilyCount);
    SetDArrayLength(QueueFamilies, QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueFamilyCount, QueueFamilies);

    u32 GraphicsQueueFamilyIndex = 0;
    for (u32 i = 0; i < QueueFamilyCount; ++i)
    {
        if (QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            GraphicsQueueFamilyIndex = i;
            break;
        }
    }

    VkBool32 PresentSupport = VK_FALSE;
    for (u32 i = 0; i < QueueFamilyCount; ++i)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, Surface, &PresentSupport);
        if (PresentSupport)
        {
            OutDevice->PresentQueueFamilyIndex = i;
            break;
        }
    }

    u32 TransferQueueFamilyIndex = 0;
    for (u32 i = 0; i < QueueFamilyCount; ++i)
    {
        if (QueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT && !(QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            TransferQueueFamilyIndex = i;
            break;
        }
    }

    u32 ExtensionCount = 0;
    vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, nullptr);

    VkExtensionProperties* Extensions = (VkExtensionProperties*)ReserveDArray(VkExtensionProperties, ExtensionCount);
    SetDArrayLength(Extensions, ExtensionCount);
    vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, Extensions);

    b8 SwapchainExtensionSupported = VK_FALSE;
    for (u32 i = 0; i < ExtensionCount; ++i)
    {
        if (strcmp(Extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        {
            SwapchainExtensionSupported = VK_TRUE;
            break;
        }
    }

    DestroyDArray(Extensions);
    DestroyDArray(QueueFamilies);

    return TRUE;
}

b8 VulkanDeviceCreate(VulkanContext* Context)
{
    u32 DeviceCount = 0;
    vkEnumeratePhysicalDevices(Context->Instance, &DeviceCount, nullptr);

    VkPhysicalDevice* Devices = (VkPhysicalDevice*)ReserveDArray(VkPhysicalDevice, DeviceCount);
    SetDArrayLength(Devices, DeviceCount);
    vkEnumeratePhysicalDevices(Context->Instance, &DeviceCount, Devices);

    u32 BestDeviceIndex = 0;
    for (u32 i = 0; i < DeviceCount; ++i)
    {
        VkPhysicalDeviceProperties Properties;
        vkGetPhysicalDeviceProperties(Devices[i], &Properties);
        if (Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            BestDeviceIndex = i;
            break;
        }
    }
    Context->Device.PhysicalDevice = Devices[BestDeviceIndex];

    vkGetPhysicalDeviceProperties(Context->Device.PhysicalDevice, &Context->Device.Properties);
    vkGetPhysicalDeviceFeatures(Context->Device.PhysicalDevice, &Context->Device.Features);
    vkGetPhysicalDeviceMemoryProperties(Context->Device.PhysicalDevice, &Context->Device.MemoryProperties);

    u32 QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(Context->Device.PhysicalDevice, &QueueFamilyCount, nullptr);

    VkQueueFamilyProperties* QueueFamilies = (VkQueueFamilyProperties*)ReserveDArray(VkQueueFamilyProperties, QueueFamilyCount);
    SetDArrayLength(QueueFamilies, QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(Context->Device.PhysicalDevice, &QueueFamilyCount, QueueFamilies);

    u32 GraphicsQueueFamilyIndex = 0;
    for (u32 i = 0; i < QueueFamilyCount; ++i)
    {
        if (QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            GraphicsQueueFamilyIndex = i;
            break;
        }
    }

    VkBool32 PresentSupport = VK_FALSE;
    for (u32 i = 0; i < QueueFamilyCount; ++i)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(Context->Device.PhysicalDevice, i, Context->Surface, &PresentSupport);
        if (PresentSupport)
        {
            Context->Device.PresentQueueFamilyIndex = i;
            break;
        }
    }

    u32 TransferQueueFamilyIndex = 0;
    for (u32 i = 0; i < QueueFamilyCount; ++i)
    {
        if (QueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT && !(QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            TransferQueueFamilyIndex = i;
            break;
        }
    }

    u32 ExtensionCount = 0;
    vkEnumerateDeviceExtensionProperties(Context->Device.PhysicalDevice, nullptr, &ExtensionCount, nullptr);

    // Reserve correct capacity and set length
    VkExtensionProperties* Extensions = (VkExtensionProperties*)ReserveDArray(VkExtensionProperties, ExtensionCount);
    SetDArrayLength(Extensions, ExtensionCount);

    vkEnumerateDeviceExtensionProperties(Context->Device.PhysicalDevice, nullptr, &ExtensionCount, Extensions);

    b8 SwapchainExtensionSupported = VK_FALSE;
    for (u32 i = 0; i < ExtensionCount; ++i)
    {
        if (strcmp(Extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        {
            SwapchainExtensionSupported = VK_TRUE;
            break;
        }
    }

    DestroyDArray(Extensions);
    DestroyDArray(QueueFamilies);
    DestroyDArray(Devices);

    return TRUE;
}

void VulkanDeviceDestroy(VulkanContext* Context)
{
    // Only wait and destroy if the logical device was actually created
    if (Context->Device.LogicalDevice)
    {
        vkDeviceWaitIdle(Context->Device.LogicalDevice);
        vkDestroyDevice(Context->Device.LogicalDevice, Context->Allocator);
    }

    // Only destroy arrays if they were allocated
    if (Context->Device.Formats)
    {
        DestroyDArray(Context->Device.Formats);
    }
    if (Context->Device.PresentModes)
    {
        DestroyDArray(Context->Device.PresentModes);
    }

    // Reset device handles
    Context->Device.PhysicalDevice = VK_NULL_HANDLE;
    Context->Device.LogicalDevice = VK_NULL_HANDLE;
    Context->Device.GraphicsQueue = VK_NULL_HANDLE;
    Context->Device.PresentQueue = VK_NULL_HANDLE;
    Context->Device.TransferQueue = VK_NULL_HANDLE;
    Context->Device.GraphicsQueueFamilyIndex = 0;
    Context->Device.PresentQueueFamilyIndex = 0;
    Context->Device.TransferQueueFamilyIndex = 0;
}
