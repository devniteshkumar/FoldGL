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
    external/bullet
    external
    src/utils
    src
)

target_link_libraries(ogt
    OpenGL::GL
    glfw
    BulletDynamics
    BulletCollision
    LinearMath
)

# --- Bullet: disable extras ---
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_ENET OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)


# Add Bullet physics engine
add_subdirectory(external/bullet)
