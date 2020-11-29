cmake_minimum_required(VERSION 3.14)

project(Lava_Examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(triangle bin/triangle.cpp)
