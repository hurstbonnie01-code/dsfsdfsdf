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
export XDG_RUNTIME_DIR=/tmp
setsid ./build/<binary> > /tmp/<binary>.log 2>&1 &
```

## Verifying the window

- Use `xwininfo -root -tree | grep '"Vulkan Window"'` to confirm a window with the expected title and geometry (e.g. `800x600`).
- `setsid` keeps the process alive after the one-shot shell exits; find the real binary PID with `pgrep -f '/build/<binary>$'` and kill it with `pkill -9 -f '/build/<binary>$'`. Avoid `$!` unless you know the launcher does not wrap the binary.

## Capturing a screenshot

`import` (ImageMagick) works by window ID:

```bash
WINID=$(xwininfo -root -tree | grep '"Vulkan Window"' | head -1 | awk '{print $1}')
import -window "$WINID" /tmp/vulkan_screenshot.png
```

- For a black-clearing window, check the average pixel value is `0` with `identify` or Python/PIL.
- For a colored triangle, use the vertex colors to verify the image is not all black and contains red/green/blue dominant pixels. Example Python/PIL assertions:
  - dimensions match `WIDTH` × `HEIGHT`;
  - the image is not all black (overall mean per channel `> 0.05` on a `0..1` scale);
  - there are red-dominant pixels (`R > 150, G < 80, B < 80`), green-dominant pixels (`G > 150, R < 80, B < 80`), and blue-dominant pixels (`B > 150, R < 80, G < 80`);
  - the colored region covers roughly the expected triangle area (e.g. `~12.5%` of an 800×600 window for a triangle with NDC coordinates `(-0.5,0.5)`, `(0.5,0.5)`, `(0,-0.5)`).
- `scrot` can also capture the focused or whole desktop.
- For an 8×8 grid of small rotating cubes, verify:
  - the image is not all black and the window dimensions are `800×600`;
  - the colored area is spread across many disconnected regions (e.g. > 10 connected components when using `scipy.ndimage.label` on the non-black mask), proving a grid of separate cubes rather than one solid shape;
  - the scene contains red-dominant, green-dominant, and blue-dominant pixels;
  - two frames captured 1 second apart differ by at least `5%` of pixels, confirming rotation/animation.

## Known environment quirks

- `vulkaninfo` may fail with `X Error of failed request: BadMatch (X_CreateWindow)` on this display. That does **not** mean the application will fail; the actual Vulkan+GLFW binary may create its surface and present correctly.
- `XDG_RUNTIME_DIR` is not set by default; set it to `/tmp` (or `/tmp/runtime-ubuntu`) to avoid loader/Qt warnings.

## Devin Secrets Needed

None.
