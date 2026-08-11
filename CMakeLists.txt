cmake_minimum_required(VERSION 3.10)
project(VulkanWindow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(vulkan_window main.cpp vulkan_window.cpp)
target_include_directories(vulkan_window PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vulkan_window PRIVATE Vulkan::Vulkan glfw glm::glm)

add_custom_target(copy_textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Tiles141_1K-PNG ${CMAKE_BINARY_DIR}/Tiles141_1K-PNG
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Wood095_1K-PNG ${CMAKE_BINARY_DIR}/Wood095_1K-PNG
    COMMENT "Copying textures to build directory")
add_dependencies(vulkan_window copy_textures)
