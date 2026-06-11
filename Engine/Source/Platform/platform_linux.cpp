// (C) 2026 madoodia.com
// ---------------------

#include "Core/logger.h"
#include "Platform/platform.h"
#include "defines.h"
#include "Core/input.h"
#include "Core/event.h"
#include "Core/ge_string.h"
#include "Containers/darray.h"

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

Keys TranslateKeycode(u32 XKeyCode);

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
                xcb_key_press_event_t* KeyEvent = (xcb_key_press_event_t*)Event;
                b8 Pressed = Event->response_type == XCB_KEY_PRESS;
                xcb_keycode_t Code = KeyEvent->detail;
                KeySym KSym = XkbKeycodeToKeysym(State->Display, KeyCode(Code), 0, Code & ShiftMask ? 1 : 0);

                Keys Key = TranslateKeycode(KSym);

                InputProcessKey(Key, Pressed);
            }
            break;
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                xcb_button_press_event_t* MouseEvent = (xcb_button_press_event_t*)Event;
                b8 Pressed = Event->response_type == XCB_BUTTON_PRESS;
                MouseButton Button = MOUSE_BUTTON_COUNT;
                switch (MouseEvent->detail)
                {
                    case 1:
                        Button = MOUSE_BUTTON_LEFT;
                        break;
                    case 2:
                        Button = MOUSE_BUTTON_MIDDLE;
                        break;
                    case 3:
                        Button = MOUSE_BUTTON_RIGHT;
                        break;
                }
                if (Button != MOUSE_BUTTON_COUNT)
                {
                    InputProcessButton(Button, Pressed);
                }
            }
            break;
            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t* MotionEvent = (xcb_motion_notify_event_t*)Event;
                i32 XPos = MotionEvent->event_x;
                i32 YPos = MotionEvent->event_y;

                InputProcessMouseMove(XPos, YPos);
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

void PlatformGetRequiredExtensionNames(const char*** OutExtensionNames)
{
    PushDArray(*OutExtensionNames, "VK_KHR_xcb_surface"); // VK_KHR_xlib_surface ?
}

Keys TranslateKeycode(u32 XKeyCode)
{
    switch (XKeyCode)
    {
        case XK_BackSpace:
            return KEY_BACKSPACE;
        case XK_Return:
            return KEY_ENTER;
        case XK_Tab:
            return KEY_TAB;
            // case XK_Shift: return KEY_SHIFT;
            // case XK_Control: return KEY_CONTROL;

        case XK_Pause:
            return KEY_PAUSE;
        case XK_Caps_Lock:
            return KEY_CAPITAL;

        case XK_Escape:
            return KEY_ESCAPE;

            // Not supported
            // case : return KEY_CONVERT;
            // case : return KEY_NONCONVERT;
            // case : return KEY_ACCEPT;

        case XK_Mode_switch:
            return KEY_MODECHANGE;

        case XK_space:
            return KEY_SPACE;
        case XK_Prior:
            return KEY_PRIOR;
        case XK_Next:
            return KEY_NEXT;
        case XK_End:
            return KEY_END;
        case XK_Home:
            return KEY_HOME;
        case XK_Left:
            return KEY_LEFT;
        case XK_Up:
            return KEY_UP;
        case XK_Right:
            return KEY_RIGHT;
        case XK_Down:
            return KEY_DOWN;
        case XK_Select:
            return KEY_SELECT;
        case XK_Print:
            return KEY_PRINT;
        case XK_Execute:
            return KEY_EXECUTE;
        // case XK_snapshot: return KEY_SNAPSHOT; // not supported
        case XK_Insert:
            return KEY_INSERT;
        case XK_Delete:
            return KEY_DELETE;
        case XK_Help:
            return KEY_HELP;

        case XK_Meta_L:
            return KEY_LWIN; // TODO: not sure this is right
        case XK_Meta_R:
            return KEY_RWIN;
            // case XK_apps: return KEY_APPS; // not supported

            // case XK_sleep: return KEY_SLEEP; //not supported

        // case XK_KP_0:
        //     return KEY_NUMPAD0;
        // case XK_KP_1:
        //     return KEY_NUMPAD1;
        // case XK_KP_2:
        //     return KEY_NUMPAD2;
        // case XK_KP_3:
        //     return KEY_NUMPAD3;
        // case XK_KP_4:
        //     return KEY_NUMPAD4;
        // case XK_KP_5:
        //     return KEY_NUMPAD5;
        // case XK_KP_6:
        //     return KEY_NUMPAD6;
        // case XK_KP_7:
        //     return KEY_NUMPAD7;
        // case XK_KP_8:
        //     return KEY_NUMPAD8;
        // case XK_KP_9:
        //     return KEY_NUMPAD9;
        case XK_multiply:
            return KEY_MULTIPLY;
        case XK_KP_Add:
            return KEY_ADD;
        case XK_KP_Separator:
            return KEY_SEPARATOR;
        case XK_KP_Subtract:
            return KEY_SUBTRACT;
        case XK_KP_Decimal:
            return KEY_DECIMAL;
        case XK_KP_Divide:
            return KEY_DIVIDE;
        case XK_F1:
            return KEY_F1;
        case XK_F2:
            return KEY_F2;
        case XK_F3:
            return KEY_F3;
        case XK_F4:
            return KEY_F4;
        case XK_F5:
            return KEY_F5;
        case XK_F6:
            return KEY_F6;
        case XK_F7:
            return KEY_F7;
        case XK_F8:
            return KEY_F8;
        case XK_F9:
            return KEY_F9;
        case XK_F10:
            return KEY_F10;
        case XK_F11:
            return KEY_F11;
        case XK_F12:
            return KEY_F12;
            // case XK_F13:
            //     return KEY_F13;
            // case XK_F14:
            //     return KEY_F14;
            // case XK_F15:
            //     return KEY_F15;
            // case XK_F16:
            //     return KEY_F16;
            // case XK_F17:
            //     return KEY_F17;
            // case XK_F18:
            //     return KEY_F18;
            // case XK_F19:
            //     return KEY_F19;
            // case XK_F20:
            //     return KEY_F20;
            // case XK_F21:
            //     return KEY_F21;
            // case XK_F22:
            //     return KEY_F22;
            // case XK_F23:
            //     return KEY_F23;
            // case XK_F24:
            //     return KEY_F24;

        case XK_Num_Lock:
            return KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return KEY_SCROLL;

            // case XK_KP_Equal:
            //     return KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return KEY_LSHIFT;
        case XK_Shift_R:
            return KEY_RSHIFT;
        case XK_Control_L:
            //     return KEY_LCONTROL;
            // case XK_Control_R:
            //     return KEY_RCONTROL;
            // case XK_Menu:
            //     return KEY_RMENU;

        case XK_semicolon:
            return KEY_SEMICOLON;
        case XK_plus:
            return KEY_PLUS;
        case XK_comma:
            return KEY_COMMA;
        case XK_minus:
            return KEY_MINUS;
        case XK_period:
            return KEY_PERIOD;
        case XK_slash:
            return KEY_SLASH;
        case XK_grave:
            return KEY_GRAVE;

        case XK_a:
        case XK_A:
            return KEY_A;
        case XK_b:
        case XK_B:
            return KEY_B;
        case XK_c:
        case XK_C:
            return KEY_C;
        case XK_d:
        case XK_D:
            return KEY_D;
        case XK_e:
        case XK_E:
            return KEY_E;
        case XK_f:
        case XK_F:
            return KEY_F;
        case XK_g:
        case XK_G:
            return KEY_G;
        case XK_h:
        case XK_H:
            return KEY_H;
        case XK_i:
        case XK_I:
            return KEY_I;
        case XK_j:
        case XK_J:
            return KEY_J;
        case XK_k:
        case XK_K:
            return KEY_K;
        case XK_l:
        case XK_L:
            return KEY_L;
        case XK_m:
        case XK_M:
            return KEY_M;
        case XK_n:
        case XK_N:
            return KEY_N;
        case XK_o:
        case XK_O:
            return KEY_O;
        case XK_p:
        case XK_P:
            return KEY_P;
        case XK_q:
        case XK_Q:
            return KEY_Q;
        case XK_r:
        case XK_R:
            return KEY_R;
        case XK_s:
        case XK_S:
            return KEY_S;
        case XK_t:
        case XK_T:
            return KEY_T;
        case XK_u:
        case XK_U:
            return KEY_U;
        case XK_v:
        case XK_V:
            return KEY_V;
        case XK_w:
        case XK_W:
            return KEY_W;
        case XK_x:
        case XK_X:
            return KEY_X;
        case XK_y:
        case XK_Y:
            return KEY_Y;
        case XK_z:
        case XK_Z:
            return KEY_Z;

        default:
            return KEY_UNKNOWN;
    }
}
#endif // GEPLATFORM_LINUX
