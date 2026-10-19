cmake_minimum_required(VERSION 3.16)
project(opengl_spacetime_galaxy VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

find_package(OpenGL REQUIRED)


add_library(imgui STATIC
        dependencies/imgui-1.92.8/imgui_draw.cpp
        dependencies/imgui-1.92.8/imgui.cpp
        dependencies/imgui-1.92.8/imgui_tables.cpp
        dependencies/imgui-1.92.8/imgui_widgets.cpp

        dependencies/imgui-1.92.8/backends/imgui_impl_glfw.cpp
        dependencies/imgui-1.92.8/backends/imgui_impl_opengl3.cpp

)

target_include_directories(imgui PUBLIC
        dependencies/imgui-1.92.8
        dependencies/imgui-1.92.8/backends
)



target_link_libraries(imgui PUBLIC
        glfw

        OpenGL::GL

)

add_library(glad STATIC
        src/glad.c
)

target_include_directories(glad PUBLIC
    dependencies/glad
)

add_executable(opengl_spacetime_galaxy
        src/Camera.cpp
        src/Galaxy.cpp
        src/PlanetSystem.cpp
        src/Shader.cpp
        src/SpacetimeGrid.cpp
        src/StarField.cpp
        src/main.cpp
)


target_link_directories(opengl_spacetime_galaxy PRIVATE
        include

        dependencies/glm

        dependencies/KHR

)

target_link_libraries(opengl_spacetime_galaxy PRIVATE
        glfw
        imgui
        OpenGL::GL
        glad
)

# This tells CMake to copy the "shaders" folder from your project root
# into the same folder as your compiled executable (the build folder).
add_custom_command(TARGET opengl_spacetime_galaxy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:opengl_spacetime_galaxy>/shaders
        COMMENT "Copying shaders to build directory..."
)