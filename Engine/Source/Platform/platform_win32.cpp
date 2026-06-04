// (C) 2026 madoodia.com
// ---------------------

#include "Core/logger.h"
#include "Platform\platform.h"

// Windows Platform Layer
#if GEPLATFORM_WINDOWS

#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>

struct InternalState
{
    HINSTANCE HInstance;
    HWND HWindow;
};

static f64 ClockFreq;
static LARGE_INTEGER StartTime;

LRESULT CALLBACK
Win32Proc(
    HWND HWindow,
    u32 msg,
    WPARAM WParam,
    LPARAM LParam);

b8 PlatformStartUp(
    PlatformState* PState,
    const char* ApplicationName,
    i32 X,
    i32 Y,
    i32 Width,
    i32 Height)
{
    // TODO: try to use VirtualAlloc instead of malloc
    PState->InternalState = malloc(sizeof(InternalState));
    InternalState* State = (InternalState*)PState->InternalState;

    State->HInstance = GetModuleHandle(0);

    // Creating Window Class
    HICON Icon = LoadIcon(State->HInstance, IDI_APPLICATION);

    WNDCLASSA WindowClass;
    memset(&WindowClass, 0, sizeof(WindowClass));
    WindowClass.style = CS_DBLCLKS;
    WindowClass.lpfnWndProc = Win32Proc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = State->HInstance;
    WindowClass.hIcon = Icon;
    WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = nullptr;
    WindowClass.lpszMenuName = nullptr;
    WindowClass.lpszClassName = "GameEngineWindowClass";

    // Registering Window Class
    if (!RegisterClassA(&WindowClass)) {
        MessageBoxA(nullptr, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // Creating Window
    u32 ClientX = X;
    u32 ClientY = Y;
    u32 ClientWidth = Width;
    u32 ClientHeight = Height;

    u32 WindowX = ClientX;
    u32 WindowY = ClientY;
    u32 WindowWidth = ClientWidth;
    u32 WindowHeight = ClientHeight;

    u32 WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 WindowExStyle = WS_EX_APPWINDOW;

    WindowStyle |= WS_MAXIMIZEBOX;
    WindowStyle |= WS_MINIMIZEBOX;
    WindowStyle |= WS_THICKFRAME;

    RECT BorderRect = {0, 0, 0, 0};
    AdjustWindowRectEx(&BorderRect, WindowStyle, 0, WindowExStyle);

    WindowX += BorderRect.left;
    WindowY += BorderRect.top;
    WindowWidth += BorderRect.right - BorderRect.left;
    WindowHeight += BorderRect.bottom - BorderRect.top;

    HWND WHandle = CreateWindowExA(
        WindowExStyle, "GameEngineWindowClass", ApplicationName,
        WindowStyle, WindowX, WindowY, WindowWidth, WindowHeight,
        0, 0, State->HInstance, 0);

    if (WHandle == 0) {
        MessageBoxA(nullptr, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        GEFATAL("Window Creation Failed!");
        return FALSE;
    } else {
        State->HWindow = WHandle;
    }

    // Show the Window
    b32 ShouldActivate = 1; // NOTE: If the window should not accept input, this should be false.
    i32 ShowWindowCommandFlags = ShouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(State->HWindow, ShowWindowCommandFlags);

    // Clock Setup
    LARGE_INTEGER Frequency;
    QueryPerformanceCounter(&Frequency);
    ClockFreq = 1.0 / (f64)Frequency.QuadPart;
    QueryPerformanceCounter(&StartTime);

    return TRUE;
}

void PlatformShutdown(PlatformState* PState)
{
    InternalState* State = (InternalState*)PState->InternalState;

    if (State->HWindow) {
        DestroyWindow(State->HWindow);
        State->HWindow = nullptr;
    }
}

b8 PlatformPumpMessage(PlatformState* PState)
{
    MSG Message;

    while (PeekMessageA(&Message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }

    return TRUE;
}

void* PlatformAllocate(u64 Size, b8 Aligned)
{
    // TEMP:
    return malloc(Size);
}

void PlatformFree(void* Block, b8 Aligned)
{
    // TEMP:
    free(Block);
}

void* PlatformZeroMemory(void* Block, u64 Size)
{
    return memset(Block, 0, Size);
}

void* PlatformCopyMemory(void* Destination, const char* Source, u64 Size)
{
    return memcpy(Destination, Source, Size);
}

void* PlatformSetMemory(void* Destination, i32 Value, u64 Size)
{
    return memset(Destination, Value, Size);
}

void PlatformConsoleWrite(const char* Message, u8 Colour)
{
    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    static u8 Levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(ConsoleHandle, Levels[Colour]);

    OutputDebugStringA(Message);
    u64 Length = strlen(Message);
    LPDWORD NumberWritten = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Message, (DWORD)Length, NumberWritten, 0);
}

void PlatformConsoleWriteError(const char* Message, u8 Colour)
{
    HANDLE ConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    static u8 Levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(ConsoleHandle, Levels[Colour]);

    OutputDebugStringA(Message);
    u64 Length = strlen(Message);
    LPDWORD NumberWritten = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), Message, (DWORD)Length, NumberWritten, 0);
}

f64 PlatformGetAbsoluteTime()
{
    LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);
    return (f64)CurrentTime.QuadPart * ClockFreq;
}

void PlatformSleep(u64 MSec)
{
    Sleep(MSec);
}

LRESULT CALLBACK
Win32Proc(
    HWND HWindow,
    u32 Message,
    WPARAM WParam,
    LPARAM LParam)
{
    switch (Message) {
        case WM_ERASEBKGND:
            // NOTE:Notify the OS that erasing the background will be handled by aplication to prevent flicker.
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit.
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // Get the Updated Size
            // RECT Rect;
            // GetClientRect(HWindow, &Rect);
            // u32 Width = Rect.right - Rect.left;
            // u32 Width = Rect.buttom - Rect.top;
            // TODO: Fire an event for winsow resize
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // b8 Pressed = (Message == WM_KEYDOWN || Message == WM_SYSKEYDOWN);
            // TODO: Input Processing
        } break;
        case WM_MOUSEMOVE: {
            // i32 XPos = GET_X_LPARAM(LParam);
            // i32 YPos = GET_Y_LPARAM(LParam);
            // TODO: Input Processing
        } break;
        case WM_MOUSEWHEEL: {
            // i32 ZDelta = GET_WHEEL_DELTA_WPARAM(WParam);
            // if (ZDelta != 0) {
            //     ZDelta = (ZDelta < 0) ? -1 : 1;
            // 	// TODO: Input Processing
            // }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            // b8 Pressed = Message == WM_LBUTTONDOWN || Message == WM_MBUTTONDOWN || Message == WM_RBUTTONDOWN;
            // // TODO: Input Processing
        } break;
    }
    return DefWindowProcA(HWindow, Message, WParam, LParam);
}

#endif // GEPLATFORM_WINDOWS
