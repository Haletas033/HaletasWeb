cmake_minimum_required(VERSION 3.26)
project(HaletasWeb)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(HaletasWeb Src/Main.cpp
        Src/utils/repos/repos.cpp
        Src/utils/nav/nav.cpp
        Src/utils/misc/misc.cpp
        Src/JS/jsCore.h
        Src/scripts/skillBallPit.js.h
        Src/JS/variable.h
        Src/JS/js.h
        Src/JS/console.h
        Src/JS/function.h
        Src/JS/conditionals.h
        Src/JS/typeTraits.h
        Src/JS/buildJS.h
)

add_subdirectory(external/cmark-0.31.1)

# Include cmark headers (source + build folder)
target_include_directories(HaletasWeb PRIVATE
        "${CMAKE_SOURCE_DIR}/external/cmark-0.31.1/src"
        "${CMAKE_SOURCE_DIR}/external/cmark-0.31.1/build/src"
)

file(COPY ${CMAKE_SOURCE_DIR}/LSIMdocs DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/skills DESTINATION ${CMAKE_BINARY_DIR})

# Link the static library directly
target_link_libraries(HaletasWeb PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
        cmark
)
