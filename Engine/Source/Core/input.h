// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

enum MouseButton
{
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_COUNT
};

#define DEFINE_KEY(name, code) KEY_##name = code

enum Keys
{
    DEFINE_KEY(UNKNOWN, 0x00),
    DEFINE_KEY(BACKSPACE, 0x08),
    DEFINE_KEY(TAB, 0x09),
    DEFINE_KEY(ENTER, 0x0D),
    DEFINE_KEY(SHIFT, 0x10),
    DEFINE_KEY(CTRL, 0x11),
    DEFINE_KEY(ALT, 0x12),

    DEFINE_KEY(PAUSE, 0x13),
    DEFINE_KEY(CAPITAL, 0x14),
    DEFINE_KEY(ESCAPE, 0x1B),

    DEFINE_KEY(CONVERT, 0x1C),
    DEFINE_KEY(NONCONVERT, 0x1D),
    DEFINE_KEY(ACCEPT, 0x1E),
    DEFINE_KEY(MODECHANGE, 0x1F),

    DEFINE_KEY(SPACE, 0x20),
    DEFINE_KEY(PRIOR, 0x21),
    DEFINE_KEY(NEXT, 0x22),
    DEFINE_KEY(END, 0x23),
    DEFINE_KEY(HOME, 0x24),
    DEFINE_KEY(LEFT, 0x25),
    DEFINE_KEY(UP, 0x26),
    DEFINE_KEY(RIGHT, 0x27),
    DEFINE_KEY(DOWN, 0x28),

    DEFINE_KEY(SELECT, 0x29),
    DEFINE_KEY(PRINT, 0x2A),
    DEFINE_KEY(EXECUTE, 0x2B),
    DEFINE_KEY(SNAPSHOT, 0x2C),
    DEFINE_KEY(INSERT, 0x2D),
    DEFINE_KEY(DELETE, 0x2E),
    DEFINE_KEY(HELP, 0x2F),

    DEFINE_KEY(NUMPAD_0, 0x30),
    DEFINE_KEY(NUMPAD_1, 0x31),
    DEFINE_KEY(NUMPAD_2, 0x32),
    DEFINE_KEY(NUMPAD_3, 0x33),
    DEFINE_KEY(NUMPAD_4, 0x34),
    DEFINE_KEY(NUMPAD_5, 0x35),
    DEFINE_KEY(NUMPAD_6, 0x36),
    DEFINE_KEY(NUMPAD_7, 0x37),
    DEFINE_KEY(NUMPAD_8, 0x38),
    DEFINE_KEY(NUMPAD_9, 0x39),

    DEFINE_KEY(MULTIPLY, 0x6A),
    DEFINE_KEY(ADD, 0x6B),
    DEFINE_KEY(SEPARATOR, 0x6C),
    DEFINE_KEY(SUBTRACT, 0x6D),
    DEFINE_KEY(DECIMAL, 0x6E),
    DEFINE_KEY(DIVIDE, 0x6F),

    DEFINE_KEY(A, 0x41),
    DEFINE_KEY(B, 0x42),
    DEFINE_KEY(C, 0x43),
    DEFINE_KEY(D, 0x44),
    DEFINE_KEY(E, 0x45),
    DEFINE_KEY(F, 0x46),
    DEFINE_KEY(G, 0x47),
    DEFINE_KEY(H, 0x48),
    DEFINE_KEY(I, 0x49),
    DEFINE_KEY(J, 0x4A),
    DEFINE_KEY(K, 0x4B),
    DEFINE_KEY(L, 0x4C),
    DEFINE_KEY(M, 0x4D),
    DEFINE_KEY(N, 0x4E),
    DEFINE_KEY(O, 0x4F),
    DEFINE_KEY(P, 0x50),
    DEFINE_KEY(Q, 0x51),
    DEFINE_KEY(R, 0x52),
    DEFINE_KEY(S, 0x53),
    DEFINE_KEY(T, 0x54),
    DEFINE_KEY(U, 0x55),
    DEFINE_KEY(V, 0x56),
    DEFINE_KEY(W, 0x57),
    DEFINE_KEY(X, 0x58),
    DEFINE_KEY(Y, 0x59),
    DEFINE_KEY(Z, 0x5A),

    DEFINE_KEY(LWIN, 0x5B),
    DEFINE_KEY(RWIN, 0x5C),
    DEFINE_KEY(APPS, 0x5D),

    DEFINE_KEY(SLEEP, 0x5F),

    DEFINE_KEY(F1, 0x70),
    DEFINE_KEY(F2, 0x71),
    DEFINE_KEY(F3, 0x72),
    DEFINE_KEY(F4, 0x73),
    DEFINE_KEY(F5, 0x74),
    DEFINE_KEY(F6, 0x75),
    DEFINE_KEY(F7, 0x76),
    DEFINE_KEY(F8, 0x77),
    DEFINE_KEY(F9, 0x78),
    DEFINE_KEY(F10, 0x79),
    DEFINE_KEY(F11, 0x7A),
    DEFINE_KEY(F12, 0x7B),

    DEFINE_KEY(NUMLOCK, 0x90),
    DEFINE_KEY(SCROLL, 0x91),

    DEFINE_KEY(LSHIFT, 0xA0),
    DEFINE_KEY(RSHIFT, 0xA1),
    DEFINE_KEY(LCTRL, 0xA2),
    DEFINE_KEY(RCTRL, 0xA3),
    DEFINE_KEY(LALT, 0xA4),
    DEFINE_KEY(RALT, 0xA5),

    DEFINE_KEY(SEMICOLON, 0xBA),
    DEFINE_KEY(EQUALS, 0xBB),
    DEFINE_KEY(COMMA, 0xBC),
    DEFINE_KEY(MINUS, 0xBD),
    DEFINE_KEY(PERIOD, 0xBE),
    DEFINE_KEY(SLASH, 0xBF),

    DEFINE_KEY(BACKTICK, 0xC0),

    DEFINE_KEY(LEFT_BRACKET, 0xDB),
    DEFINE_KEY(BACKSLASH, 0xDC),
    DEFINE_KEY(RIGHT_BRACKET, 0xDD),

    DEFINE_KEY(PLUS, 0xBB),
    DEFINE_KEY(GRAVE, 0xC0),

    KEY_COUNT
};

void InitializeInput();
void UpdateInput(f64 DeltaTime);
void ShutdownInput();

GEAPI b8 InputIsKeyDown(Keys Key);
GEAPI b8 InputIsKeyUp(Keys Key);
GEAPI b8 InputWasKeyDown(Keys Key);
GEAPI b8 InputWasKeyUp(Keys Key);

void InputProcessKey(Keys Key, b8 Pressed);

GEAPI b8 InputIsButtonDown(MouseButton Button);
GEAPI b8 InputIsButtonUp(MouseButton Button);
GEAPI b8 InputWasButtonDown(MouseButton Button);
GEAPI b8 InputWasButtonUp(MouseButton Button);
GEAPI void InputGetMousePosition(i32* X, i32* Y);
GEAPI void InputGetPrevMousePosition(i32* X, i32* Y);

void InputProcessButton(MouseButton Button, b8 Pressed);
void InputProcessMouseMove(i16 X, i16 Y);
void InputProcessMouseWheel(i8 ZDelta);
