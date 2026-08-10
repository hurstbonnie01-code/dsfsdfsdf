---
name: Testing Vulkan/GLFW GUI applications on the dev X desktop
description: How to build, launch, and verify a Vulkan+GLFW window on the Ubuntu desktop environment used for Devin sessions.
---

## Environment

- A real X11 desktop is available on `DISPLAY=:0` (Plasma/KWin).
- `libvulkan-dev`, `libvulkan1`, `libglfw3-dev`, and `mesa-vulkan-drivers` provide build/run support.
- `cmake` is used to build.

## Build

```bash
cd <repo>
rm -rf build
cmake -B build -S . && cmake --build build
```

## Launching

The binary blocks in `mainLoop()` and must be run in the background for testing:

```bash
export XDG_RUNTIME_DIR=/tmp/runtime-ubuntu
mkdir -p "$XDG_RUNTIME_DIR"
./build/<binary> > /tmp/<binary>.log 2>&1 &
```

## Verifying the window

- Use `xwininfo -root -tree | grep '"Vulkan Window"'` to confirm a window with the expected title and geometry (e.g. `800x600`).
- If you background with `nohup` from a `bash -c` wrapper, `$!` may be the wrapper PID; get the real binary PID with `pgrep -f` or `ps`.

## Capturing a screenshot

`import` (ImageMagick) works by window ID:

```bash
WINID=$(xwininfo -root -tree | grep '"Vulkan Window"' | head -1 | awk '{print $1}')
import -window "$WINID" /tmp/vulkan_screenshot.png
```

- For a black-clearing window, check the average pixel value is `0` with `identify` or Python/PIL.
- `scrot` can also capture the focused or whole desktop.

## Known environment quirks

- `vulkaninfo` may fail with `X Error of failed request: BadMatch (X_CreateWindow)` on this display. That does **not** mean the application will fail; the actual Vulkan+GLFW binary may create its surface and present correctly.
- `XDG_RUNTIME_DIR` is not set by default; set it to `/tmp/runtime-ubuntu` to suppress Qt/Plasma warnings and avoid potential loader issues.

## Devin Secrets Needed

None.
