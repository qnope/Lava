cmake_minimum_required(VERSION 3.14)

add_executable(test_device test_device.cpp)
target_link_libraries(test_device PRIVATE gtest_main)

add_test(device test_device)
