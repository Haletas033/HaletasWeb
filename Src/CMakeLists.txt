cmake_minimum_required(VERSION 3.26)
project(HaletasWeb)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(HaletasWeb Main.cpp)

# Include cmark headers (source + build folder)
target_include_directories(HaletasWeb PRIVATE
        "${CMAKE_SOURCE_DIR}/../external/cmark-0.31.1/src"
        "${CMAKE_SOURCE_DIR}/../external/cmark-0.31.1/build/src"
)

file(COPY ${CMAKE_SOURCE_DIR}/../LSIMdocs DESTINATION ${CMAKE_BINARY_DIR})

# Link the static library directly
target_link_libraries(HaletasWeb PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
        "${CMAKE_SOURCE_DIR}/../external/cmark-0.31.1/build/src/libcmark.a"
)
