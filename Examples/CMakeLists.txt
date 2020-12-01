cmake_minimum_required(VERSION 3.14)

add_executable(triangle bin/triangle.cpp)
target_link_libraries(triangle PRIVATE Lava)
