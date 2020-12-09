cmake_minimum_required(VERSION 3.14)

project(Lava
    VERSION 0.0.1
    DESCRIPTION "A Modern C++ Vulkan Wrapper"
    LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  LittleTypeLibrary
  GIT_REPOSITORY https://github.com/qnope/Little-Type-Library
  GIT_TAG        origin/master
)

FetchContent_MakeAvailable(LittleTypeLibrary)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(src/)
add_subdirectory(Examples/)
add_subdirectory(Tests/)
