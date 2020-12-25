cmake_minimum_required(VERSION 3.14)

add_executable(triangle bin/triangle.cpp shaders/triangle/triangle.vert shaders/triangle/triangle.frag)
target_link_libraries(triangle PRIVATE Lava SDL2main)
