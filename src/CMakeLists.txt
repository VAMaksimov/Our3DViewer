# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name and language
project(BrickGame CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# --- Common Settings ---

# This replicates the effect of compiling with -I. from the root
include_directories(${CMAKE_SOURCE_DIR})

# Find ncurses for the CLI target
find_package(Curses REQUIRED)

# Find Qt6 for the GUI target
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Enable Qt's automatic tools (Meta-Object Compiler, UI Compiler, Resource Compiler)
# These need to be ON before adding the executable that uses Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the required Qt6 packages (Core, Gui, Widgets)
# REQUIRED ensures CMake errors if Qt6 is not found
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Enable Qt's automatic tools (Meta-Object Compiler, UI Compiler, Resource Compiler)
# These need to be ON before adding the executable that uses Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# --- Core Game Logic Library ---

# Gather all non-main source files from brick_game/
# Exclude brick_game.cpp as it's the entry point for the CLI app
file(GLOB GAME_LOGIC_SOURCES
    brick_game/*.cpp
    brick_game/snake/*.cpp
    brick_game/tetris/*.cpp
    EXCLUDE brick_game/brick_game.cpp
)

# Create a static library for the core game logic
# Both CLI and GUI will link against this
add_library(game_logic STATIC
    ${GAME_LOGIC_SOURCES}
    # Add any headers from include/ that are part of the library's interface
    # If headers are only used internally by .cpp files, they don't need listing here
    # Example: include/game_info.h
)

# Ensure the library itself knows where its headers are (relative to include/)
# PUBLIC means consumers of this library also get this include path
target_include_directories(game_logic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include> # For potential installation
)


# --- CLI Executable ---

# Gather additional source files specifically for the CLI target from gui/cli/
file(GLOB CLI_GUI_SOURCES 
    gui/cli/*.cpp
)

# Add the CLI executable, including the main entry point and the gui/cli files
add_executable(s21_brick_game_cli 
    ${CLI_GUI_SOURCES}
)

# Add ncurses include directory specifically for the CLI target
target_include_directories(s21_brick_game_cli PRIVATE
    ${CURSES_INCLUDE_DIRS}
)

# Link the CLI executable against the core game logic library
target_link_libraries(s21_brick_game_cli PRIVATE game_logic ${CURSES_LIBRARIES})


# --- Qt GUI Executable ---

# Gather GUI source and header files
set(GUI_SOURCES
    gui/desktop/desktop_app.cpp
    gui/desktop/snake_widget.cpp
    gui/desktop/tetris_widget.cpp
)
set(GUI_HEADERS
    gui/desktop/qt_widget.h # Headers with Q_OBJECT need to be listed for AUTOMOC
)

# Add the GUI executable
add_executable(s21_brick_game_gui WIN32 # Use WIN32 for GUI apps on Windows to hide console
    ${GUI_SOURCES}
    ${GUI_HEADERS}
)

# Add include directories specific to the GUI part
target_include_directories(s21_brick_game_gui PRIVATE
    gui/desktop # To find SnakeWidget from gui/desktop/desktop_app.cpp
    # The main 'include' directory is already inherited via game_logic linking
)

# Link the GUI executable against the core game logic library AND Qt modules
target_link_libraries(s21_brick_game_gui PRIVATE
    game_logic          # Link our core game logic
    Qt6::Core           # Link Qt Core module
    Qt6::Gui            # Link Qt Gui module
    Qt6::Widgets        # Link Qt Widgets module
)


# --- Output ---
message(STATUS "Configured Brick Game Project")
message(STATUS "CLI Target: s21_brick_game_cli")
message(STATUS "GUI Target: s21_brick_game_gui")
