cmake_minimum_required(VERSION 3.10)
project(HaletasWeb)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable target
add_executable(HaletasWeb main.cpp Tag.cpp)

# Include directories (if you have header files in subdirectories)
include_directories(${CMAKE_SOURCE_DIR})

# Find curl and json libraries
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Link libraries
target_link_libraries(HaletasWeb PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# Specify the output directory for generated files (like the HTML files)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Build")

