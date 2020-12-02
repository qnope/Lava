cmake_minimum_required(VERSION 3.14)

include(GenerateExportHeader)

set(SYS_FILES
    Lava/sys/SdlContext.h
    Lava/sys/SdlContext.cpp

    Lava/sys/exception.h
    Lava/sys/EventLoop.h

    Lava/sys/Window.h
    Lava/sys/Window.cpp

    Lava/sys/Event.h
    Lava/sys/Event.cpp
    )

add_library(Lava SHARED
    ${SYS_FILES})
generate_export_header(Lava)
target_include_directories(Lava PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(Lava PUBLIC .)
target_include_directories(Lava PUBLIC ${SDL2_INCLUDE_DIRS})

target_compile_features(Lava PUBLIC cxx_std_20)

target_link_libraries(Lava PUBLIC ${SDL2_LIBRARIES} LTL)
