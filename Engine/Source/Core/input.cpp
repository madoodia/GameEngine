// (C) 2026 madoodia.com
// ---------------------

#include "Core/input.h"
#include "Core/event.h"
#include "Core/ge_memory.h"
#include "Core/logger.h"

struct KeyboardState
{
    b8 Keys[256];
};

struct MouseState
{
    i16 X;
    i16 Y;
    u8 Buttons[MOUSE_BUTTON_COUNT];
};

struct InputState
{
    KeyboardState CurrentKeyboard;
    KeyboardState PreviousKeyboard;
    MouseState CurrentMouse;
    MouseState PreviousMouse;
};

// Internal Input state
static b8 IsInitialized = FALSE;
static InputState State = {};

void InitializeInput()
{
    ZeroMemory(&State, sizeof(InputState));
    IsInitialized = TRUE;
    GEINFO("Input Subsystem is Initialized");
}

void ShutdownInput()
{
    IsInitialized = FALSE;
    GEINFO("Input Subsystem is Shutdown");
}

void UpdateInput(f64 DeltaTime)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return;
    }

    CopyMemory(&State.PreviousKeyboard, &State.CurrentKeyboard, sizeof(KeyboardState));
    CopyMemory(&State.PreviousMouse, &State.CurrentMouse, sizeof(MouseState));
}

GEAPI b8 InputIsKeyDown(Keys Key)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return FALSE;
    }

    return State.CurrentKeyboard.Keys[Key] == TRUE;
}

GEAPI b8 InputIsKeyUp(Keys Key)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return TRUE;
    }

    return State.CurrentKeyboard.Keys[Key] == FALSE;
}

GEAPI b8 InputWasKeyDown(Keys Key)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return FALSE;
    }

    return State.PreviousKeyboard.Keys[Key] == TRUE;
}

GEAPI b8 InputWasKeyUp(Keys Key)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return FALSE;
    }

    return State.PreviousKeyboard.Keys[Key] == TRUE;
}

void InputProcessKey(Keys Key, b8 Pressed)
{
    if (State.CurrentKeyboard.Keys[Key] != Pressed)
    {
        State.CurrentKeyboard.Keys[Key] = Pressed;

        EventContext Context = {};
        Context.U16[0] = Key;

        EmitEvent(Pressed ? KEY_PRESSED : KEY_RELEASED, nullptr, Context);
    }
}

GEAPI b8 InputIsButtonDown(MouseButton Button)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return FALSE;
    }

    return State.CurrentMouse.Buttons[Button] == TRUE;
}

GEAPI b8 InputIsButtonUp(MouseButton Button)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return TRUE;
    }

    return State.CurrentMouse.Buttons[Button] == FALSE;
}

GEAPI b8 InputWasButtonDown(MouseButton Button)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return FALSE;
    }

    return State.PreviousMouse.Buttons[Button] == TRUE;
}

GEAPI b8 InputWasButtonUp(MouseButton Button)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        return FALSE;
    }

    return State.PreviousMouse.Buttons[Button] == FALSE;
}

GEAPI void InputGetMousePosition(i32* X, i32* Y)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        *X = 0;
        *Y = 0;
        return;
    }

    *X = State.CurrentMouse.X;
    *Y = State.CurrentMouse.Y;
}

GEAPI void InputGetPrevMousePosition(i32* X, i32* Y)
{
    if (!IsInitialized)
    {
        GEERROR("Input subsystem is not initialized!");
        *X = 0;
        *Y = 0;
        return;
    }

    *X = State.PreviousMouse.X;
    *Y = State.PreviousMouse.Y;
}

void InputProcessButton(MouseButton Button, b8 Pressed)
{
    if (State.CurrentMouse.Buttons[Button] != Pressed)
    {
        State.CurrentMouse.Buttons[Button] = Pressed;
        EventContext Context;
        Context.U16[0] = Button;
        EmitEvent(Pressed ? BUTTON_PRESSED : BUTTON_RELEASED, nullptr, Context);
    }
}

void InputProcessMouseMove(i16 X, i16 Y)
{
    if (State.CurrentMouse.X != X || State.CurrentMouse.Y != Y)
    {
        GEDEBUG("Mouse moved to (%d, %d)", X, Y);

        State.CurrentMouse.X = X;
        State.CurrentMouse.Y = Y;

        EventContext Context;
        Context.U16[0] = X;
        Context.U16[1] = Y;

        EmitEvent(MOUSE_MODED, nullptr, Context);
    }
}

void InputProcessMouseWheel(i8 ZDelta)
{
    EventContext Context;
    Context.U8[0] = ZDelta;
    EmitEvent(MOUSE_WHEELED, nullptr, Context);
}
