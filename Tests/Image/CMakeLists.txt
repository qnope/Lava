cmake_minimum_required(VERSION 3.14)

add_executable(test_image_dimension test_image_dimension.cpp)
target_link_libraries(test_image_dimension PRIVATE gtest_main Lava)

add_test(image_dimension test_image_dimension)
