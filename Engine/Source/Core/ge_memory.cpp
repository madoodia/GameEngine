// (C) 2026 madoodia.com
// ---------------------

#include "ge_memory.h"

#include "Core/logger.h"
#include "Platform/platform.h"

// TODO:Custom String lib
#include <stdio.h>
#include <string.h>

struct MemoryStats
{
    u64 TotalAllocated;
    u64 TaggedAllocation[MemoryTags::MEMORY_TAG_MAX_TAGS];
};

static const char* MemoryTagNames[MemoryTags::MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN",
    "ARRAY",
    "DARRAY",
    "STRING",
    "BST",
    "HASHMAP",
    "RINGQUEUE",
    "APPLICATION",
    "JOB",
    "TEXTURE",
    "DICT",
    "MATERIALINSTANCE",
    "RENDERER",
    "GAME",
    "TRANSFORM",
    "ENTITY",
    "ENTITYNODE",
    "SCENE",
    "ANIMATION",
    "ANIMATIONINSTANCE",
    "PHYSICS",
    "NETWORKING",
    "UI",
    "EDITOR",
    "ASSETMANAGER",
    "RESOURCEMANAGER"};

static struct MemoryStats Stats;

void InitializeMemory()
{
    PlatformZeroMemory(&Stats, sizeof(Stats));
}

void ShutdownMemory()
{
    // NOTE: Nothing for now
}

void* AllocateMemory(u64 Size, MemoryTags Tag)
{
    if (Tag == MemoryTags::MEMORY_TAG_UNKNOWN)
    {
        GEWARNING("Change the Class of the Allocation, MEMORYMEMORY_TAG_UNKNOWN is not accepted!");
    }

    Stats.TotalAllocated += Size;
    Stats.TaggedAllocation[Tag] += Size;

    // TODO: Memory Alignment
    void* Block = PlatformAllocate(Size, FALSE);
    PlatformZeroMemory(Block, Size);

    return Block;
}

void* SetMemory(void* Destination, i32 Value, u64 Size)
{
    return PlatformSetMemory(Destination, Value, Size);
}

void* ZeroMemory(void* Block, u64 Size)
{
    return PlatformZeroMemory(Block, Size);
}

void* CopyMemory(void* Destination, const void* Source, u64 Size)
{
    return PlatformCopyMemory(Destination, Source, Size);
}

void FreeMemory(void* Block, u64 Size, MemoryTags Tag)
{
    if (Tag == MemoryTags::MEMORY_TAG_UNKNOWN)
    {
        GEWARNING("Change the Class of the Allocation, MEMORYMEMORY_TAG_UNKNOWN is not accepted!");
    }

    Stats.TotalAllocated -= Size;
    Stats.TaggedAllocation[Tag] -= Size;

    // TODO: Memory Alignment
    PlatformFree(Block, FALSE);
}

// NOTE: This function will be used for Debugging purposes
char* GetMemoryUsageString()
{
    const u64 KBits = 1024;
    const u64 MBits = KBits * 1024;
    const u64 GBits = MBits * 1024;

    char Buffer[8000] = "System memory usage (tagged):\n";
    u64 Offset = strlen(Buffer);
    for (u32 i = 0; i < MemoryTags::MEMORY_TAG_MAX_TAGS; ++i)
    {
        char Unit[6] = "XBits";
        float Amount = 1.0f;

        if (Stats.TaggedAllocation[i] >= GBits)
        {
            Amount = (float)Stats.TaggedAllocation[i] / (float)GBits;
            Unit[0] = 'G';
        } else if (Stats.TaggedAllocation[i] >= MBits)
        {
            Amount = (float)Stats.TaggedAllocation[i] / (float)MBits;
            Unit[0] = 'M';
        } else if (Stats.TaggedAllocation[i] >= KBits)
        {
            Amount = (float)Stats.TaggedAllocation[i] / (float)KBits;
            Unit[0] = 'K';
        } else
        {
            Amount = (float)Stats.TaggedAllocation[i];
            Unit[0] = ' ';
        }
        i32 Length = snprintf(Buffer + Offset, sizeof(Buffer) - Offset, "  %-18s : %2.2f%s\n", MemoryTagNames[i], Amount, Unit);
        Offset += Length;
    }

    char* Result = _strdup(Buffer);
    return Result;
}
