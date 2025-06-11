# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name and language
project(3DViewer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# --- Common Settings ---

# This replicates the effect of compiling with -I. from the root
include_directories(${CMAKE_SOURCE_DIR})

# Find Qt6 for the GUI target
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets OpenGL)

# Enable Qt's automatic tools (Meta-Object Compiler, UI Compiler, Resource Compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# --- Core Game Logic Library ---

# Gather all non-main source files from model directory
file(GLOB MODEL_SOURCES
    model/*.cpp
)

file(GLOB MODEL_HEADERS
    model/*.h
)

# Create a static library for the core model logic
add_library(viewer_model STATIC
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
)

# Ensure the library itself knows where its headers are (relative to include/)
# PUBLIC means consumers of this library also get this include path
target_include_directories(viewer_model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/model
)


# --- Qt GUI Executable ---

# Gather source files
file(GLOB VIEW_SOURCES
    view/*.cpp
)

file(GLOB CONTROLLER_SOURCES
    controller/*.cpp
)

file(GLOB VIEW_HEADERS
    view/*.h
)

file(GLOB CONTROLLER_HEADERS
    controller/*.h
)

# Add the main GUI executable
add_executable(3d_viewer_gui
    ${VIEW_SOURCES}
    ${CONTROLLER_SOURCES}
    ${VIEW_HEADERS}
    ${CONTROLLER_HEADERS}
)

# Add include directories specific to the GUI part
target_include_directories(3d_viewer_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/controller
)

# Link against the model library and Qt modules
target_link_libraries(3d_viewer_gui PRIVATE
    viewer_model
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::OpenGL
)

# --- Output ---
message(STATUS "Configured 3D Viewer Project")
message(STATUS "GUI Target: 3d_viewer_gui")
