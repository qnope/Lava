cmake_minimum_required(VERSION 3.14)

project(Lava
    VERSION 0.0.1
    DESCRIPTION "A Modern C++ Vulkan Wrapper"
    LANGUAGES CXX)

include(FetchContent)

find_package(Vulkan REQUIRED)

add_subdirectory(src/)
add_subdirectory(Examples/)
add_subdirectory(Tests/)
