cmake_minimum_required(VERSION 3.10)
project(ogt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -g")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

set(SOURCES
    src/main.cpp
    external/glad/src/glad.c
    src/renderer/shader.cpp
    src/renderer/renderer.cpp
    src/renderer/buffers.cpp
    src/renderer/texture.cpp
    src/utils/fileio.cpp
)

add_executable(ogt ${SOURCES})

target_include_directories(ogt PRIVATE
    external/glad/include
    external/glfw/include
    external/glm
    external
    src/utils
    src
)

set(IMGUI_DIR external/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    external/glfw/include
)

target_link_libraries(ogt
    OpenGL::GL
    glfw
    imgui
)
