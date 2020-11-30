cmake_minimum_required(VERSION 3.14)

include(GenerateExportHeader)

set(SYSTEM_FILES
    system/SdlContext.h
    system/SdlContext.cpp)

add_library(Lava SHARED
    ${SYSTEM_FILES})
generate_export_header(Lava)
target_include_directories(Lava PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_compile_features(Lava PUBLIC cxx_std_20)
