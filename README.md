# GameEngine 🎮

A modular, high-performance C++20 custom game engine built from scratch for learning game engine architecture and low-level graphics APIs. This engine is designed to run on both **Windows** (via Win32 API) and **Linux** (via X11) and uses **Vulkan** for hardware-accelerated rendering.

---

## 🏛️ Architecture & Project Structure

The project is structured into two main components:
1. **Engine** ([Engine](./Engine)): A shared dynamic library (`.dll` / `.so`) containing the core engine logic, platform layers, custom memory management, input/event systems, and the Vulkan renderer backend.
2. **Sandbox** ([Sandbox](./Sandbox)): The client application executable that links with the Engine and defines game-specific behavior.

### Module Breakdown
- 📂 **[Engine/Source](./Engine/Source)**
  - 🖥️ **[Platform](./Engine/Source/Platform)**: Handles OS-specific window creation and message loops (Win32 for Windows, X11 for Linux).
  - ⚙️ **[Core](./Engine/Source/Core)**: Contains vital subsystems including:
    - Custom Memory Management ([ge_memory.h](./Engine/Source/Core/ge_memory.h) / [ge_memory.cpp](./Engine/Source/Core/ge_memory.cpp)) with allocation tracking.
    - Event System ([event.h](./Engine/Source/Core/event.h) / [event.cpp](./Engine/Source/Core/event.cpp)) for decoupled communications.
    - Input Management ([input.h](./Engine/Source/Core/input.h) / [input.cpp](./Engine/Source/Core/input.cpp)) for keyboard and mouse events.
    - High-resolution Clock ([clock.h](./Engine/Source/Core/clock.h) / [clock.cpp](./Engine/Source/Core/clock.cpp)) for delta-time calculations.
    - Logger ([logger.h](./Engine/Source/Core/logger.h) / [logger.cpp](./Engine/Source/Core/logger.cpp)) with console highlighting.
  - 🎨 **[Renderer](./Engine/Source/Renderer)**: Handles rendering pipeline frontend/backend abstraction.
    - 🌋 **[Vulkan](./Engine/Source/Renderer/Vulkan)**: Vulkan-specific backend configuration and resources.
  - 📦 **[Containers](./Engine/Source/Containers)**: Custom standard-independent data structures like dynamic arrays ([darray.h](./Engine/Source/Containers/darray.h) / [darray.cpp](./Engine/Source/Containers/darray.cpp)).
- 📂 **[Sandbox/Source](./Sandbox/Source)**
  - Client application entry-point and custom game loop callbacks (`CreateGame`, `Initialize`, `Update`, `Render`, `OnResize`).

---

## 🛠️ Prerequisites

To build and run the project, ensure you have the following installed on your system:

### Windows
1. **LLVM/Clang Compiler**: The build system uses `clang++`. Download [LLVM Project Releases](https://github.com/llvm/llvm-project/releases) (recommended: `v22.1.0` or newer), install it, and add it to your system `PATH`.
2. **Vulkan SDK**: Install the [Vulkan SDK](https://vulkan.lunarg.com/).
   - Verify the `VULKAN_SDK` environment variable is set:
     - **CMD**: `echo %VULKAN_SDK%`
     - **PowerShell**: `$env:VULKAN_SDK`

### Linux
1. **Clang Compiler**: Make sure `clang++` is installed.
2. **Vulkan SDK & X11 Libraries**: Install Vulkan development packages and X11 libraries:
   ```bash
   sudo apt-get install vulkan-sdk libx11-dev
   ```

---

## 🚀 Building & Running

### Using VS Code Tasks (Recommended)
This repository includes a pre-configured `.vscode/tasks.json` to make compilation and development seamless. Use the `Ctrl+Shift+B` shortcut (or run task from command palette) to build:
- **Build All (Debug)**: Builds both Engine and Sandbox with debugging symbols.
- **Build Engine (Debug / Release)**: Compiles the Engine module.
- **Build Sandbox (Debug)**: Compiles the Sandbox module.
- **Clean All**: Removes the build artifacts folder (`/Bin`).
- **Generate C++ Files**: Runs the helper template scripts.

### Command Line Building

#### Windows
Use the root batch scripts to build or clean the project:
- **Build (Debug)**:
  ```cmd
  build_all.bat . 0
  ```
- **Build (Release)**:
  ```cmd
  build_all.bat . 1
  ```
- **Clean**:
  ```cmd
  clean.bat .
  ```

#### Linux
Use the shell scripts:
- **Build (Debug)**:
  ```bash
  ./build_all.sh . 0
  ```
- **Clean**:
  ```bash
  ./clean.sh .
  ```

---

## ⚡ Developer Utilities

### Boilerplate File Generator
To quickly spin up a new class with standard headers and constructor/destructor declarations:
- **Windows (CMD)**: Run `file_generator.bat` and follow prompts.
- **Windows (PowerShell / VS Code)**: Run the `Generate C++ Files` task, which executes `file_generator.ps1`.
  - It creates class declarations directly inside the target directory (e.g., `Engine\Source\Core`).


### Color Code

- <span style="color: #FF3333; font-weight: bold; font-family: monospace;">// TODO:</span>
- <span style="color: #000000; background-color: #FF3333; font-weight: bold; font-family: monospace; padding: 2px 6px; border-radius: 3px; display: inline-block;">// ERROR: on red bg</span>
- <span style="color: #FF8DA1; font-weight: bold; font-family: monospace;">// FIXME:</span>
- <span style="color: #000000; background-color: #FF9800; font-weight: bold; font-family: monospace; padding: 2px 6px; border-radius: 3px; display: inline-block;">// REVIEW: on orange bg</span>
- <span style="color: #64B5F6; font-weight: bold; font-family: monospace;">// NOTE: blue</span>
- <span style="color: #000000; background-color: #FFEB3B; font-weight: bold; font-family: monospace; padding: 2px 6px; border-radius: 3px; display: inline-block;">// WIP: on yellow bg</span>
- <span style="color: #FFFFFF; background-color: #424242; font-weight: bold; font-family: monospace; padding: 2px 6px; border-radius: 3px; display: inline-block;">// IMPORTANT:</span>
- <span style="color: #FF3333; background-color: #FFFFFF; font-weight: bold; font-family: monospace; padding: 2px 6px; border-radius: 3px; border: 1px solid #FF3333; display: inline-block;">// DONOTTOUCH: on white bg</span>
- <span style="color: #81C784; font-weight: bold; font-family: monospace;">// FEATURE: green</span>
- <span style="color: #FFEE58; font-weight: bold; font-family: monospace;">// REMEMBER:</span>
- <span style="color: #00E5FF; font-weight: bold; font-family: monospace;">// EDUCATION:</span>
- <span style="color: #FF8A80; font-weight: bold; font-family: monospace;">// HACK: red</span>
- <span style="color: #E040FB; font-weight: bold; font-family: monospace;">// BOGGLE: purple</span>
- <span style="color: #9C27B0; font-weight: bold; font-family: monospace;">// FIX:</span>
- <span style="color: #FF9800; font-weight: bold; font-family: monospace;">// DEBUG:</span>
- <span style="color: #FFEE58; font-weight: bold; font-family: monospace;">// UNDONE:</span>
- <span style="color: #FF3333; font-weight: bold; font-family: monospace;">// BUG:</span>
- <span style="color: #9E9E9E; font-weight: bold; font-family: monospace;">// KLUDGE:</span>
- <span style="color: #616161; font-weight: bold; font-family: monospace;">// UGLY: gray</span>
- <span style="color: #000000; background-color: #ECEFF1; font-weight: bold; font-family: monospace; padding: 2px 6px; border-radius: 3px; display: inline-block;">// XXX: black</span>