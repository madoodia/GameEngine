// (C) 2026 madoodia.com
// ---------------------

#include <Core/logger.h>
#include <Core/asserts.h>

// TEMP:
#include <Core/application.h>

int main()
{
    ApplicationConfig Config;
    Config.Name = "Game Engine Sandbox";
    Config.WindowX = 100;
    Config.WindowY = 100;
    Config.WindowWidth = 1280;
    Config.WindowHeight = 720;

    if (!ApplicationCreate(&Config))
    {
        GEERROR("Failed to create application!");
        return -1;
    }
    if (!ApplicationRun())
    {
        GEERROR("Failed to run application!");
        return -1;
    }

    return 0;
}
