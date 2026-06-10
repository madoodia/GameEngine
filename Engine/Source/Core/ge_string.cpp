// (C) 2026 madoodia.com
// ---------------------

#include "ge_string.h"
#include "ge_memory.h"

#include <string.h>

u64 StringLength(const char* Source)
{
    return strlen(Source);
}

char* StringDuplicate(const char* Source)
{
    u64 Length = StringLength(Source);
    char* Dest = (char*)AllocateMemory(Length + 1, MEMORY_TAG_STRING);
    memcpy(Dest, Source, Length + 1);
    return Dest;
}
