cmake_minimum_required(VERSION 3.15)
project(UnityAnalyzer VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_ANDROID_LIBRARY "Build as Android shared library" OFF)

# Dependencies
include(FetchContent)

# nlohmann/json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/include.zip
)
FetchContent_MakeAvailable(json)

# lz4
FetchContent_Declare(
    lz4
    GIT_REPOSITORY https://github.com/lz4/lz4.git
    GIT_TAG v1.9.4
    SOURCE_SUBDIR build/cmake
)
FetchContent_MakeAvailable(lz4)

# liblzma (XZ)
FetchContent_Declare(
    liblzma
    URL https://github.com/tukaani-project/xz/releases/download/v5.4.3/xz-5.4.3.tar.gz
)
# liblzma doesn't always have a clean CMake, but we can try or use system-provided
# For the sake of this environment, we'll try to find it or include it.
# Usually on Windows it's harder, on Linux/Android it's simpler.
find_package(LibLZMA)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${json_SOURCE_DIR}/include
)

# Source files
file(GLOB_RECURSE SOURCES 
    "UnityAnalyzer/Core/*.cpp"
    "UnityAnalyzer/Unity/*.cpp"
    "UnityAnalyzer/Runtime/*.cpp"
    "UnityAnalyzer/Scripts/*.cpp"
    "UnityAnalyzer/Archives/*.cpp"
    "UnityAnalyzer/Pipeline/*.cpp"
    "UnityAnalyzer/Plugins/*.cpp"
    "UnityAnalyzer/Export/*.cpp"
)

if(BUILD_ANDROID_LIBRARY)
    add_library(UnityAnalyzer SHARED ${SOURCES})
    target_link_libraries(UnityAnalyzer PRIVATE nlohmann_json::nlohmann_json lz4::lz4 log ${LIBLZMA_LIBRARIES})
else()
    add_executable(UnityAnalyzer main.cpp ${SOURCES})
    target_link_libraries(UnityAnalyzer PRIVATE nlohmann_json::nlohmann_json lz4::lz4 ${LIBLZMA_LIBRARIES})
endif()

# Compiler-specific settings
if(MSVC)
    target_compile_options(UnityAnalyzer PRIVATE /W4)
else()
    target_compile_options(UnityAnalyzer PRIVATE -Wall -Wextra)
endif()

# Tests
enable_testing()
add_subdirectory(Tests)
