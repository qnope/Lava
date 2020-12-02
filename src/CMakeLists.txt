cmake_minimum_required(VERSION 3.14)

include(GenerateExportHeader)

set(SYSTEM_FILES
    Lava/system/SdlContext.h
    Lava/system/SdlContext.cpp

    Lava/system/exception.h
    Lava/system/EventLoop.h

    Lava/system/Window.h
    Lava/system/Window.cpp

    Lava/system/Event.h
    Lava/system/Event.cpp
    )

add_library(Lava SHARED
    ${SYSTEM_FILES})
generate_export_header(Lava)
target_include_directories(Lava PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(Lava PUBLIC .)
target_include_directories(Lava PUBLIC ${SDL2_INCLUDE_DIRS})

target_compile_features(Lava PUBLIC cxx_std_20)

target_link_libraries(Lava PUBLIC ${SDL2_LIBRARIES} LTL)
