cmake_minimum_required(VERSION 3.14)

include(GenerateExportHeader)

FetchContent_Declare(
  LittleTypeLibrary
  GIT_REPOSITORY https://github.com/qnope/Little-Type-Library
  GIT_TAG        origin/master
)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/SDL-mirror/SDL
  GIT_TAG        release-2.0.12
)

FetchContent_MakeAvailable(LittleTypeLibrary)

if(MSVC)
    list(APPEND EXTRA_LIBS vcruntime)
endif()

FetchContent_MakeAvailable(SDL2)

set(SYS_FILES
    Lava/sys/Event.h
    Lava/sys/Event.cpp

    Lava/sys/SdlContext.h
    Lava/sys/SdlContext.cpp

    Lava/sys/exception.h
    Lava/sys/EventLoop.h

    Lava/sys/Window.h
    Lava/sys/Window.cpp
    )

set(VK_SYS_FILES
    Lava/vk-sys/vulkan.h

    Lava/vk-sys/Instance.h
    Lava/vk-sys/Instance.cpp

    Lava/vk-sys/Device.h
    Lava/vk-sys/Device.cpp

    Lava/vk-sys/Surface.h
    Lava/vk-sys/Surface.cpp

    Lava/vk-sys/Swapchain.h
    Lava/vk-sys/Swapchain.cpp

    Lava/vk-sys/ImageSubresourceRange.h
    Lava/vk-sys/ImageSubresourceRange.cpp
    )

set(IMAGE_FILES
    Lava/Image/Image.h
    Lava/Image/Image.cpp

    Lava/Image/ImageConcept.h

    Lava/Image/ImageAccessRange.h

    Lava/Image/SwapchainImage.h
    Lava/Image/SwapchainImage.cpp

    Lava/Image/ImageDimension.h
    Lava/Image/ImageDimension.cpp

    Lava/Image/ImageView.h
    Lava/Image/ImageView.cpp
    )

set(SHADER_FILES
    Lava/Shader/Shader.h
    Lava/Shader/Shader.cpp
    )

add_library(Lava SHARED
    ${SYS_FILES}
    ${VK_SYS_FILES}
    ${IMAGE_FILES}
    ${SHADER_FILES})

generate_export_header(Lava)

if(MSVC)
    #+ Used to disable warning due to STL in header files
    target_compile_options(Lava PUBLIC /wd4251)
endif()

target_include_directories(Lava PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(Lava PUBLIC .)
target_include_directories(Lava PUBLIC ${Vulkan_INCLUDE_DIRS})

target_compile_features(Lava PUBLIC cxx_std_20)

target_link_libraries(Lava PUBLIC
    SDL2
    LTL
    ${Vulkan_LIBRARIES})
