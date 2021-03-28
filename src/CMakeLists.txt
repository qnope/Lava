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

FetchContent_Declare(
  libshaderc
  GIT_REPOSITORY https://github.com/google/shaderc
  GIT_TAG        v2020.4
)

FetchContent_Declare(
  SPIRV-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers
  GIT_TAG        1.5.4.raytracing.fixed
)

FetchContent_Declare(
  SPIRV-Tools
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools
  GIT_TAG        v2020.6
)

FetchContent_Declare(
  glslang
  GIT_REPOSITORY https://github.com/KhronosGroup/glslang
  GIT_TAG        11.2.0
)

if(MSVC)
    list(APPEND EXTRA_LIBS vcruntime)
endif()

SET(SHADERC_SKIP_TESTS ON CACHE BOOL "Disable" FORCE)
SET(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "Disable" FORCE)
SET(SHADERC_ENABLE_SHARED_CRT ON CACHE BOOL "Disable" FORCE)

FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(glslang)
FetchContent_MakeAvailable(SPIRV-Headers)
FetchContent_MakeAvailable(SPIRV-Tools)
FetchContent_MakeAvailable(libshaderc)
FetchContent_MakeAvailable(LittleTypeLibrary)

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
    ${Vulkan_LIBRARIES}
    shaderc)
