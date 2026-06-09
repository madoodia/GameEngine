// (C) 2026 madoodia.com
// ---------------------

#include "Core/application.h"
#include "Platform/platform.h"
#include "Core/logger.h"
#include "Core/ge_memory.h"
#include "Core/event.h"

#include "game_types.h"

struct ApplicationState
{
    Game* GameInstance;
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
b8 ApplicationCreate(Game* GameInstance)
{
    if (IsInitialized)
    {
        GEERROR("Application is already initialized!");
        return FALSE;
    }

    AppState.GameInstance = GameInstance;

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

    if (!InitializeEvent())
    {
        GEFATAL("Failed to initialize event system!");
        return FALSE;
    }

    if (!PlatformStartUp(&AppState.PState,
                         GameInstance->AppConfig.Name,
                         GameInstance->AppConfig.WindowX,
                         GameInstance->AppConfig.WindowY,
                         GameInstance->AppConfig.WindowWidth,
                         GameInstance->AppConfig.WindowHeight))
    {
        return FALSE;
    }

    // Call the game's Initialize function
    if (!AppState.GameInstance->Initialize(AppState.GameInstance))
    {
        GEFATAL("Game initialization failed!");
        return FALSE;
    }

    AppState.GameInstance->OnResize(AppState.GameInstance, AppState.Width, AppState.Height);

    IsInitialized = TRUE;

    return TRUE;
}

// Game Loop
b8 ApplicationRun()
{
    GEINFO(GetMemoryUsageString());
    while (AppState.IsRunning)
    {
        if (!PlatformPumpMessage(&AppState.PState))
        {
            AppState.IsRunning = FALSE;
        }

        if (!AppState.IsSuspended)
        {
            // Call the game's Update function
            // TODO: Calculate delta time
            if (!AppState.GameInstance->Update(AppState.GameInstance, (f32)0.0f))
            {
                GEFATAL("Game update failed!");
                AppState.IsRunning = FALSE;
                break;
            }

            // Call the game's Render function
            if (!AppState.GameInstance->Render(AppState.GameInstance, (f32)0.0f))
            {
                GEFATAL("Game render failed!");
                AppState.IsRunning = FALSE;
                break;
            }
        }
    }

    AppState.IsRunning = FALSE;

    ShutdownEvent();

    PlatformShutdown(&AppState.PState);

    return TRUE;
}
