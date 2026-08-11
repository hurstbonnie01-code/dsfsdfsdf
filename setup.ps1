#Requires -RunAsAdministrator
<#
.SYNOPSIS
    One-click Windows setup for the Vulkan window project.
.DESCRIPTION
    Installs the build toolchain (CMake, Visual Studio Build Tools, Git),
    the Vulkan SDK, bootstraps vcpkg, installs project dependencies, sets
    the default CMake generator to avoid the NMake fallback, and builds.
#>

param(
    [switch]$SkipBuild
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Update-Path {
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + `
                 [System.Environment]::GetEnvironmentVariable("Path", "User")
}

function Test-Command {
    param([string]$Name)
    return [bool](Get-Command $Name -ErrorAction SilentlyContinue)
}

function Install-WingetPackage {
    param(
        [string]$Id,
        [string]$Override = ""
    )
    Write-Host "Installing $Id via winget..." -ForegroundColor Cyan
    $wingetArgs = @("install", "-e", "--id", $Id, "--source", "winget", "--accept-package-agreements", "--accept-source-agreements")
    if ($Override) {
        $wingetArgs += "--override"
        $wingetArgs += $Override
    }
    & winget @wingetArgs
    # winget exit codes are inconsistent; verify by checking state afterward
    Update-Path
}

# 1. Verify winget is present
if (-not (Test-Command "winget")) {
    throw "winget is not available. Install the App Installer from the Microsoft Store and re-run."
}

# 2. Git (needed to clone vcpkg)
if (-not (Test-Command "git")) {
    Install-WingetPackage -Id "Git.Git"
    if (-not (Test-Command "git")) {
        throw "Git installation failed. Please install Git manually."
    }
}

# 3. CMake
if (-not (Test-Command "cmake")) {
    Install-WingetPackage -Id "Kitware.CMake"
    if (-not (Test-Command "cmake")) {
        throw "CMake installation failed. Please install CMake manually."
    }
}

# 4. C++ compiler / Visual Studio Build Tools
$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
$hasVcTools = $false
if (Test-Path $vswhere) {
    $installPaths = & $vswhere -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null
    if ($installPaths) { $hasVcTools = $true }
}

if (-not $hasVcTools) {
    Write-Host "Installing Visual Studio Build Tools with C++ workload..." -ForegroundColor Cyan
    Install-WingetPackage -Id "Microsoft.VisualStudio.2022.BuildTools" -Override "--passive --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
    if (-not (Test-Path $vswhere)) {
        throw "Visual Studio Build Tools did not install correctly. Install them manually from https://visualstudio.microsoft.com/downloads/"
    }
} else {
    Write-Host "Visual Studio C++ tools already installed." -ForegroundColor Green
}

# 5. Vulkan SDK
$vulkanRoot = "C:\VulkanSDK"
$sdkDir = Get-ChildItem -Path $vulkanRoot -Directory -ErrorAction SilentlyContinue | Sort-Object Name -Descending | Select-Object -First 1
if (-not $sdkDir) {
    Write-Host "Installing Vulkan SDK..." -ForegroundColor Cyan
    Install-WingetPackage -Id "KhronosGroup.VulkanSDK"
    $sdkDir = Get-ChildItem -Path $vulkanRoot -Directory -ErrorAction SilentlyContinue | Sort-Object Name -Descending | Select-Object -First 1
}
if (-not $sdkDir) {
    throw "Vulkan SDK was not installed. Install it manually from https://vulkan.lunarg.com/ and re-run."
}
$env:VULKAN_SDK = $sdkDir.FullName
[Environment]::SetEnvironmentVariable("VULKAN_SDK", $sdkDir.FullName, "User")
Write-Host "VULKAN_SDK set to $($sdkDir.FullName)" -ForegroundColor Green

# 6. Ninja (optional but useful as an alternative generator)
if (-not (Test-Command "ninja")) {
    Install-WingetPackage -Id "Ninja-build.Ninja"
}

# 7. vcpkg for glfw3 and glm
$vcpkgDir = Join-Path $PSScriptRoot "vcpkg"
if (-not (Test-Path $vcpkgDir)) {
    Write-Host "Cloning vcpkg..." -ForegroundColor Cyan
    git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
    & "$vcpkgDir\bootstrap-vcpkg.bat"
} else {
    Write-Host "vcpkg already present; updating..." -ForegroundColor Cyan
    & "$vcpkgDir\vcpkg.exe" upgrade --no-dry-run
}

Write-Host "Installing vcpkg dependencies..." -ForegroundColor Cyan
& "$vcpkgDir\vcpkg.exe" install

# 8. Default CMake generator to avoid the NMake fallback
$generator = "Visual Studio 17 2022"
$env:CMAKE_GENERATOR = $generator
[Environment]::SetEnvironmentVariable("CMAKE_GENERATOR", $generator, "User")
Write-Host "CMAKE_GENERATOR set to $generator" -ForegroundColor Green

# 9. Configure and build
if (-not $SkipBuild) {
    Write-Host "Configuring project..." -ForegroundColor Cyan
    $toolchain = Join-Path $vcpkgDir "scripts\buildsystems\vcpkg.cmake"
    cmake -B build -S . -G $generator -A x64 -DCMAKE_TOOLCHAIN_FILE="$toolchain"

    Write-Host "Building project..." -ForegroundColor Cyan
    cmake --build build --config Release

    Write-Host "Build complete. Run: .\build\Release\vulkan_window.exe" -ForegroundColor Green
} else {
    Write-Host "Skipped build. Run 'cmake --build build --config Release' when ready." -ForegroundColor Yellow
}
