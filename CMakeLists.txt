cmake_minimum_required(VERSION 3.10)

# Define your project
project(yoga-project VERSION 0.1.0 LANGUAGES C CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Default to Debug build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build Type ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(TGUI_BUILD_FRAMEWORK FALSE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(TGUI_BUILD_FRAMEWORK TRUE)
endif()

# Set compiler flags for Debug and Release builds
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(TGUI_BACKEND "SFML_OPENGL3")



# Add Yoga, TGUI, and SFML as subdirectories

add_subdirectory(external/freetype)
add_subdirectory(external/SFML)
add_subdirectory(external/yoga/yoga)
add_subdirectory(external/tgui)
find_package(OpenGL REQUIRED)

# Add the executable
add_executable(main src/main.cpp)
# Link SFML, TGUI, and Yoga libraries
target_link_libraries(main PRIVATE tgui sfml-graphics sfml-window sfml-system OpenGL::GL yogacore freetype)


# Include directories
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/freetype/include)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/SFML)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/yoga/yoga)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/tgui/include)
