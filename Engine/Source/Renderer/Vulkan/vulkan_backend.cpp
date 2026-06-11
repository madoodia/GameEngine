// (C) 2026 madoodia.com
// ---------------------

#include "Renderer/Vulkan/vulkan_backend.h"
#include "Renderer/Vulkan/vulkan_types.inl"
#include "Core/logger.h"
#include "Core/ge_string.h"
#include "Containers/darray.h"
#include "Platform/platform.h"
#include "Renderer/Vulkan/vulkan_platform.h"

// We need Single Vulkan Context
static struct VulkanContext VulkanContext;

b8 VulkanRendererBackendInitialize(RendererBackend* Backend, const char* ApplicationName, struct PlatformState* PState)
{
    // TODO: Implement a Custom Allocator.
    VulkanContext.Allocator = nullptr;

    // Setup Vulkan Instance
    VkApplicationInfo AppInfo = {};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pApplicationName = ApplicationName;
    AppInfo.apiVersion = VK_API_VERSION_1_4;
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "My Game Engine";

    VkInstanceCreateInfo CreateInfo = {}; // Set all fields to 0

    CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pApplicationInfo = &AppInfo;

    const char** RequiredExtensions = (const char**)CreateDArray(const char*);
    PushDArray(RequiredExtensions, &VK_KHR_SURFACE_EXTENSION_NAME); // Required for all platforms
    PlatformGetRequiredExtensionNames(&RequiredExtensions);

#if defined(_DEBUG)
    PushDArray(RequiredExtensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    GEDEBUG("Required Vulkan Extensions:");
    u32 Length = GetDArrayLength(RequiredExtensions);
    for (u32 i = 0; i < Length; ++i)
    {
        GEDEBUG(RequiredExtensions[i]);
    }
#endif
    CreateInfo.enabledExtensionCount = GetDArrayLength(RequiredExtensions);
    CreateInfo.ppEnabledExtensionNames = RequiredExtensions;

    VkResult Result = vkCreateInstance(&CreateInfo, VulkanContext.Allocator, &VulkanContext.Instance);

    if (Result != VK_SUCCESS)
    {
        GEERROR("Failed to create Vulkan Instance! Error Code: %u", Result);
        return FALSE;
    }

    GEINFO("Vulkan Renderer initialized successfully!");

    return TRUE;
}

void VulkanRendererBackendShutdown(RendererBackend* Backend)
{
}

void VulkanRendererBackendOnResize(RendererBackend* Backend, u16 Width, u16 Height)
{
}

b8 VulkanRendererBackendBeginFrame(RendererBackend* Backend, f32 DeltaTime)
{
    return TRUE;
}

b8 VulkanRendererBackendEndFrame(RendererBackend* Backend, f32 DeltaTime)
{
    return TRUE;
}
