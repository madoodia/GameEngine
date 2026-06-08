// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

// Using Tags as class of the allocation
enum MemoryTags
{
    // TEMP: Add more tags as needed
    MEMORY_TAG_UNKNOWN = 0,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_STRING,
    MEMORY_TAG_BST,
    MEMORY_TAG_HASHMAP,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_DICT,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,
    MEMORY_TAG_ANIMATION,
    MEMORY_TAG_ANIMATION_INSTANCE,
    MEMORY_TAG_PHYSICS,
    MEMORY_TAG_NETWORKING,
    MEMORY_TAG_UI,
    MEMORY_TAG_EDITOR,
    MEMORY_TAG_ASSET_MANAGER,
    MEMORY_TAG_RESOURCE_MANAGER,

    // Always keep this at the end
    MEMORY_TAG_MAX_TAGS
};

GEAPI void InitializeMemory();
GEAPI void ShutdownMemory();

GEAPI void* AllocateMemory(u64 Size, MemoryTags Tag);
GEAPI void* SetMemory(void* Destination, i32 Value, u64 Size);
GEAPI void* ZeroMemory(void* Block, u64 Size);
GEAPI void* CopyMemory(void* Destination, const void* Source, u64 Size);
GEAPI void FreeMemory(void* Block, u64 Size, MemoryTags Tag);

GEAPI char* GetMemoryUsageString();
