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
    src/renderer/mesh.cpp
    src/renderer/camera.cpp
    src/pdb/atom.cpp
    src/pdb/chain.cpp
    src/pdb/common.cpp
    src/pdb/model.cpp
    src/pdb/residue.cpp
    src/pdb/secondary_structure.cpp
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

target_link_libraries(ogt
    OpenGL::GL
    glfw
)
