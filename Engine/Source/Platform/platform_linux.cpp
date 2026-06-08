// (C) 2026 madoodia.com
// ---------------------

#include "Core/logger.h"
#include "Platform/platform.h"
#include "defines.h"

// Linux Platform Layer
#if GEPLATFORM_LINUX

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h> // sudo apt-get install linx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> // sudo apt install libx11-xcb-dev
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include "time.h" // nanosleep
#else
#include "unistd.h" // microsleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct InternalState
{
    Display* Display;
    xcb_connection_t* Connection;
    xcb_window_t Window;
    xcb_screen_t* Screen;
    xcb_atom_t WmProtocols;
    xcb_atom_t WmDeleteWin;
};

GEAPI
b8 PlatformStartUp(PlatformState* PState,
                   const char* ApplicationName,
                   i32 X,
                   i32 Y,
                   i32 Width,
                   i32 Height)
{
    // TODO: try to use mmap instead of malloc
    PState->InternalState = malloc(sizeof(InternalState));
    InternalState* State = (InternalState*)PState->InternalState;

    // Connect to X Server
    State->Display = XOpenDisplay(nullptr);

    // Turn off key repeats.
    XAutoRepeatOff(State->Display);

    // Retrieve the connection from the Display
    State->Connection = XGetXCBConnection(State->Display);

    if (xcb_connection_has_error(State->Connection))
    {
        GEFATAL("Faile to connect to the X server via xcb!");
        return False;
    }

    // Get data from the X server
    const struct xcb_setup_t* Setup = xcb_get_setup(State->Connection);

    // Loop through screens using iterator.
    xcb_screen_iterator_t Iterator = xcb_setup_roots_iterator(Setup);
    int ScreenP = 0;
    for (i32 s = ScreenP; s > 0; s--)
    {
        xcb_screen_next(&Iterator);
    }

    // Assigning the screen
    State->Screen = Iterator.data;

    // Allocate an XID for our window
    State->Window = xcb_generate_id(State->Connection);

    // Register Event Type
    u32 EventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    // Listen to Mouse and keyboard
    u32 EventValues =
        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    // Values to be send to the XCB
    u32 ValueList[] = {State->Screen->black_pixel, EventValues};

    // Create the window
    xcb_void_cookie_t Window =
        xcb_create_window(State->Connection,
                          XCB_COPY_FROM_PARENT,
                          State->Window,
                          State->Screen->root,
                          X, Y, Width, Height, 0,
                          XCB_WINDOW_CLASS_INPUT_OUTPUT,
                          State->Screen->root_visual,
                          EventMask,
                          ValueList);

    // Change the Title
    xcb_change_property(State->Connection,
                        XCB_PROP_MODE_REPLACE,
                        State->Window,
                        XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING,
                        8, // Data should be viewed 8 bits at a time
                        strlen(ApplicationName),
                        ApplicationName);

    // Tell the server to notify when the window manager
    // attempts to destroy the window
    xcb_intern_atom_cookie_t WMDeleteCookie =
        xcb_intern_atom(State->Connection,
                        0,
                        strlen("WM_DELETE_WINDOW"),
                        "WM_DELETE_WINDOW");

    xcb_intern_atom_cookie_t WMProtocolsCookie =
        xcb_intern_atom(State->Connection,
                        0,
                        strlen("WM_PROTOCOLS"),
                        "WM_PROTOCOLS");

    xcb_intern_atom_reply_t* WMDeleteReply =
        xcb_intern_atom_reply(State->Connection,
                              WMDeleteCookie,
                              nullptr);

    xcb_intern_atom_reply_t* WMProtocolsReply =
        xcb_intern_atom_reply(State->Connection,
                              WMProtocolsCookie,
                              nullptr);

    State->WmDeleteWin = WMDeleteReply->atom;
    State->WmProtocols = WMProtocolsReply->atom;

    xcb_change_property(State->Connection,
                        XCB_PROP_MODE_REPLACE,
                        State->Window,
                        WMProtocolsReply->atom,
                        4, 32, 1,
                        &WMDeleteReply->atom);

    // Mapping the window to the screen
    xcb_map_window(State->Connection, State->Window);

    // Flush the stream
    i32 StreamResult = xcb_flush(State->Connection);
    if (StreamResult <= 0)
    {
        GEFATAL("An error occuredwhen flushing the stream: %d", StreamResult);
        return FALSE;
    }

    return TRUE;
}

GEAPI
void PlatformShutdown(PlatformState* PState)
{
    InternalState* State = (InternalState*)PState->InternalState;

    XAutoRepeatOn(State->Display);
    xcb_destroy_window(State->Connection, State->Window);
}

GEAPI
b8 PlatformPumpMessage(PlatformState* PState)
{
    InternalState* State = (InternalState*)PState->InternalState;

    xcb_generic_event_t* Event;
    xcb_client_message_event_t* CMessage;

    b8 QuitFlagged = FALSE;

    while (Event != 0)
    {
        Event = xcb_poll_for_event(State->Connection);
        if (Event == 0)
        {
            break;
        }

        // Handle Input events
        switch (Event->response_type & ~0x80)
        {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE: {
                // TODO: Keys press/release
            }
            break;
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                // TODO: Mouse Buttons press/release
            }
            break;
            case XCB_MOTION_NOTIFY: {
                // TODO: Mouse movement
            }
            break;
            case XCB_CONFIGURE_NOTIFY: {
                // TODO: Resizing
            }
            break;
            case XCB_CLIENT_MESSAGE: {
                CMessage = (xcb_client_message_event_t*)Event;

                // Window Close
                if (CMessage->data.data32[0] == State->WmDeleteWin)
                {
                    QuitFlagged = TRUE;
                }
            }
            break;
            default: {
                // Whatever else
            }
            break;
        }
        // by calling xcb_poll_for_event xcb will allocate event dynamically on heap,
        // so it should be freed out
        free(Event);
    }

    return !QuitFlagged;
}

void* PlatformAllocate(u64 Size, b8 Aligned)
{
    return malloc(Size);
}

void PlatformFree(void* Block, b8 Aligned)
{
    free(Block);
}

void* PlatformZeroMemory(void* Block, u64 Size)
{
    return memset(Block, 0, Size);
}

void* PlatformCopyMemory(void* Destination, const void* Source, u64 Size)
{
    return memcpy(Destination, Source, Size);
}

void* PlatformSetMemory(void* Destination, i32 Value, u64 Size)
{
    return memset(Destination, Value, Size);
}

void PlatformConsoleWrite(const char* Message, u8 Colour)
{
    // FATAL, ERROR, WARNING, INFO, DEBUG, TRACE
    const char* ColourStrings[6] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", ColourStrings[Colour], Message);
}

void PlatformConsoleWriteError(const char* Message, u8 Colour)
{
    // FATAL, ERROR, WARNING, INFO, DEBUG, TRACE
    const char* ColourStrings[6] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", ColourStrings[Colour], Message);
}

f64 PlatformGetAbsoluteTime()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

// MilliSecond
void PlatformSleep(u64 MSec)
{
#if _POSIX_C_SOURCE >= 199309L
    struct timespec TSpec;
    TSpec.tv_sec = MSec / 1000;
    TSpec.tv_nsec = (MSec % 1000) * 1000 * 1000;
    nanosleep(&TSpec, 0);
#else
    if (MSec > 1000)
    {
        sleep(MSec / 1000);
    }
    usleep((MSec % 1000) * 1000);
#endif
}

#endif // GEPLATFORM_LINUX
