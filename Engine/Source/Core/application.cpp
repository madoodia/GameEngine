// (C) 2026 madoodia.com
// ---------------------

#include "Core/application.h"
#include "Platform/platform.h"
#include "Core/logger.h"

struct ApplicationState
{
    b8 IsRunning;
    b8 IsSuspended;
    PlatformState PState;
    i16 Width;
    i16 Height;
    f64 LastTime;
};

static b8 IsInitialized = FALSE;
static ApplicationState AppState;

// Create the application and initialize subsystems
b8 ApplicationCreate(ApplicationConfig* Config)
{
    if (IsInitialized)
    {
        GEERROR("Application is already initialized!");
        return FALSE;
    }

    // Initialize subsystems here (e.g., logging, input, audio, etc.)
    InitializeLogging();

    // TODO: Remove this
    GEFATAL("This is a FATALITY message: %f", 2026.1);
    GEERROR("This is a FATALITY message: %f", 2026.1);
    GEWARNING("This is a FATALITY message: %f", 2026.1);
    GEINFO("This is a FATALITY message: %f", 2026.1);
    GEDEBUG("This is a FATALITY message: %f", 2026.1);
    GETRACE("This is a FATALITY message: %f", 2026.1);

    AppState.IsRunning = TRUE;
    AppState.IsSuspended = FALSE;

    if (!PlatformStartUp(&AppState.PState,
                         Config->Name,
                         Config->WindowX,
                         Config->WindowY,
                         Config->WindowWidth,
                         Config->WindowHeight))
    {
        return FALSE;
    }

    IsInitialized = TRUE;

    return TRUE;
}

// Game Loop
b8 ApplicationRun()
{
    while (AppState.IsRunning)
    {
        if (!PlatformPumpMessage(&AppState.PState))
        {
            AppState.IsRunning = FALSE;
        }
    }

    AppState.IsRunning = FALSE;

    PlatformShutdown(&AppState.PState);

    return TRUE;
}
