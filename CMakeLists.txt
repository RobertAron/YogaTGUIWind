cmake_minimum_required(VERSION 3.10)

# Define your project
project(yoga-project VERSION 0.1.0 LANGUAGES C CXX)

# Set C++11 or later
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Yoga subdirectory
add_subdirectory(external/yoga)

# Set TGUI to use the OpenGL backend with SFML for windowing
set(TGUI_BACKEND "SFML_OPENGL3")

# Find SFML for windowing (required by TGUI for window management)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
# FOR BUILD
# set(CMAKE_BUILD_TYPE Release)
# FOR DEBUG
set(TGUI_BUILD_FRAMEWORK FALSE)
set(CMAKE_BUILD_TYPE Debug)

# Add TGUI directory (assuming it's cloned into your project)
add_subdirectory(external/tgui)

# Link TGUI (with OpenGL), SFML (windowing), and Yoga to your project
add_executable(main src/main.cpp)

# Link SFML and TGUI to the main executable
find_package(OpenGL REQUIRED)
target_link_libraries(main PRIVATE tgui sfml-graphics sfml-window sfml-system OpenGL::GL yogacore)

# Include Yoga headers
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/yoga/yoga)
# Include TGUI headers
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/tgui/include)

# Verify the TGUI include directories
message(STATUS "TGUI include dirs: ${CMAKE_SOURCE_DIR}/tgui/include")
