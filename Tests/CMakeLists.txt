cmake_minimum_required(VERSION 3.14)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG        release-1.10.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(vk-sys)
add_subdirectory(Image)
