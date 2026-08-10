# Vulkan Window

A minimal 800×600 Vulkan window using GLFW. It creates a window, initializes Vulkan, sets up a swapchain, and clears the window to black each frame.

## Windows (one-click setup)

Run PowerShell **as Administrator** and execute:

```powershell
powershell -ExecutionPolicy Bypass -File setup.ps1
```

This installs:

- Git, CMake, and Ninja
- Visual Studio Build Tools with the C++ workload
- The Vulkan SDK
- vcpkg and the `glfw3` dependency

It also sets `CMAKE_GENERATOR` to `Visual Studio 17 2022` so CMake no longer falls back to the `NMake` generator.

After setup completes, the project is built in `build/Release/`.

### Manual Windows build

If you already have the dependencies:

```powershell
cmake -B build -S . -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
```

Or use the provided preset (requires vcpkg in `vcpkg/`):

```powershell
cmake --preset windows-vs2022
cmake --build build --config Release
```

## Linux

Install the dependencies and build:

```bash
sudo apt-get update
sudo apt-get install -y cmake libvulkan-dev libglfw3-dev libglm-dev mesa-vulkan-drivers

cmake -B build -S .
cmake --build build
./build/vulkan_window
```
