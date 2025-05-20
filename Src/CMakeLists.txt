cmake_minimum_required(VERSION 3.26)
project(HaletasWeb)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(HaletasWeb Main.cpp)

target_link_libraries(HaletasWeb PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
