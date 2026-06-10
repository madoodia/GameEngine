// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

struct EventContext
{
    union
    {
        i64 I64[2];
        u64 U64[2];
        f64 F64[2];

        i32 I32[4];
        f32 F32[4];
        u32 U32[4];

        i16 I16[8];
        u16 U16[8];

        i8 I8[16];
        u8 U8[16];

        char C[16];
    };
};

typedef b8 (*PFNOnEvent)(u16 Code, void* Sender, void* Listener, EventContext Data);

b8 InitializeEvent();
void ShutdownEvent();

GEAPI b8 RegisterEvent(u16 Code, void* Listener, PFNOnEvent OnEvent);
GEAPI b8 UnRegisterEvent(u16 Code, void* Listener, PFNOnEvent OnEvent);
GEAPI b8 EmitEvent(u16 Code, void* Sender, EventContext Context);

enum SystemEventCode
{
    APPLICATION_QUIT = 0x00,
    KEY_PRESSED = 0x01,
    KEY_RELEASED = 0x02,
    BUTTON_PRESSED = 0x03,
    BUTTON_RELEASED = 0x04,
    MOUSE_MODED = 0x05,
    MOUSE_WHEELED = 0x06,
    RESIZED = 0x07,

    MAX = 0xFF
};
