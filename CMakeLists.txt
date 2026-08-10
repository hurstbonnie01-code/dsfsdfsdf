cmake_minimum_required(VERSION 3.10)
project(VulkanWindow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(vulkan_window main.cpp)
target_link_libraries(vulkan_window PRIVATE Vulkan::Vulkan glfw)
