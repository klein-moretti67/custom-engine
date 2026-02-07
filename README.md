# Jump Square Engine

A lightweight, cross-platform 2D game engine built with C++ and OpenGL. This project demonstrates a simple "Jump Square" game using a custom engine architecture that works seamlessly on both Windows and Linux.

## Features
- **Cross-Platform**: Compiles on Linux (GCC) and Windows (MSVC) using a single Makefile.
- **Custom Shader Management**: Abstractions for creating Vertex, Fragment, and Shader Programs.
- **Simple Physics**: Basic gravity and collision detection.
- **Dynamic Obstacles**: Procedurally generated challenges for the player.

---

## Dependencies

Before building, ensure you have the following dependencies installed for your platform.

### 🐧 Linux
You will need a C++ compiler (G++), Make, and the development headers for GLFW and OpenGL.

#### **Ubuntu / Debian / Linux Mint / Pop!_OS**
```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev xorg-dev libglu1-mesa-dev
```

#### **Fedora / RHEL / CentOS**
```bash
sudo dnf install @development-tools glfw-devel mesa-libGL-devel mesa-libGLU-devel
```

#### **Arch Linux / Manjaro**
```bash
sudo pacman -S base-devel glfw-x11 mesa glu
```

### 🪟 Windows
1.  **Compiler**: [Visual Studio 2022](https://visualstudio.microsoft.com/) with the "Desktop development with C++" workload installed.
2.  **Build Tool**: `nmake` (included with Visual Studio) or `make` (if using [Choco](https://chocolatey.org/packages/make) or [MSYS2](https://www.msys2.org/)).
3.  **GLFW**: 
    - You can download the pre-compiled 64-bit binaries from [glfw.org](https://www.glfw.org/download.html).
    - Ensure `glfw3.lib` is in your library path and `include/GLFW` is in your include path.
    - *Tip*: Using [vcpkg](https://github.com/microsoft/vcpkg) is recommended: `vcpkg install glfw3:x64-windows`.

---

## Build Instructions

The project uses a cross-platform `Makefile` that automatically detects your OS and chooses the appropriate compiler (GCC on Linux, MSVC on Windows).

### 🛠️ Building
Open your terminal (on Windows, use the **Developer Command Prompt for VS 2022**) and run:

```bash
make
```

This will produce an executable named `engine` (or `engine.exe` on Windows).

### 🧹 Cleaning Build Artifacts
To remove compiled binaries and object files:

```bash
make clean
```

---

## Usage

Once built, simply run the executable:

### 🐧 Linux
```bash
./engine
```

### 🪟 Windows
```cmd
engine.exe
```

### Controls
- **SPACE**: Jump!
- **Goal**: Avoid the red obstacles as they move from right to left. If you collide, the game resets.

---

## Project Structure
- `main.cpp`: Entry point and game logic.
- `engine.h`: Core engine abstractions (single-header style).
- `src/glad.c`: OpenGL loader.
- `include/`: Third-party headers (GLAD, GLFW).
- `Makefile`: Universal build script.
- `*.glsl`: Example shaders for the rendering engine.

## License
This project is licensed under the terms of the license found in the `LICENSE` file.
