// (C) 2026 madoodia.com
// ---------------------

#include "Core/event.h"
#include "Core/ge_memory.h"
#include "Containers/darray.h"

struct RegisteredEvent
{
    void* Listener;
    PFNOnEvent Callback;
};

struct EventCodeEntry
{
    RegisteredEvent* Events;
};

#define MAX_MESSAGE_CODES 16384

struct EventSystemState
{
    // Lookup table for event codes.
    EventCodeEntry Registered[MAX_MESSAGE_CODES];
};

static b8 IsInitialized = FALSE;
static EventSystemState State;

b8 InitializeEvent()
{
    if (IsInitialized)
    {
        return FALSE;
    }

    IsInitialized = FALSE;
    ZeroMemory(&State, sizeof(EventSystemState));
    IsInitialized = TRUE;

    return TRUE;
}

void ShutdownEvent()
{
    if (!IsInitialized)
    {
        return;
    }

    for (u64 i = 0; i < MAX_MESSAGE_CODES; i++)
    {
        if (State.Registered[i].Events != nullptr)
        {
            DestroyDArray(State.Registered[i].Events);
            State.Registered[i].Events = nullptr;
        }
    }

    IsInitialized = FALSE;
}

b8 RegisterEvent(u16 Code, void* Listener, PFNOnEvent OnEvent)
{
    if (!IsInitialized)
    {
        return FALSE;
    }

    if (State.Registered[Code].Events == nullptr)
    {
        State.Registered[Code].Events = (RegisteredEvent*)CreateDArray(RegisteredEvent);
    }

    u64 RegisterCount = GetDArrayStride(State.Registered[Code].Events);
    for (u64 i = 0; i < RegisterCount; ++i)
    {
        if (State.Registered[Code].Events[i].Listener == Listener)
        {
            // TODO: Warning Message
            return FALSE;
        }
    }

    RegisteredEvent NewEvent = {};
    NewEvent.Listener = Listener;
    NewEvent.Callback = OnEvent;
    PushDArray(State.Registered[Code].Events, NewEvent);

    return TRUE;
}

b8 UnRegisterEvent(u16 Code, void* Listener, PFNOnEvent OnEvent)
{
    if (!IsInitialized)
    {
        return FALSE;
    }

    if (State.Registered[Code].Events == nullptr)
    {
        return FALSE;
    }

    u64 RegisterCount = GetDArrayLength(State.Registered[Code].Events);
    for (u64 i = 0; i < RegisterCount; ++i)
    {
        RegisteredEvent NewEvent = State.Registered[Code].Events[i];
        if (NewEvent.Listener == Listener && NewEvent.Callback == OnEvent)
        {
            RegisteredEvent PoppedEvent = {};
            PopAtDArray((void*)State.Registered[Code].Events, i, &PoppedEvent);
        }
    }

    return TRUE;
}

b8 EmitEvent(u16 Code, void* Sender, EventContext Context)
{
    if (!IsInitialized)
    {
        return FALSE;
    }

    if (State.Registered[Code].Events == nullptr)
    {
        return FALSE;
    }

    u64 RegisterCount = GetDArrayLength(State.Registered[Code].Events);
    for (u64 i = 0; i < RegisterCount; ++i)
    {
        RegisteredEvent NewEvent = State.Registered[Code].Events[i];
        if (NewEvent.Callback(Code, Sender, NewEvent.Listener, Context))
        {
            return TRUE;
        }
    }

    return FALSE;
}
