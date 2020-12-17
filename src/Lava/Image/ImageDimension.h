#pragma once

#include <ltl/crtp.h>
#include <variant>
#include <concepts>
#include "../vk-sys/vulkan.h"

namespace lava {
namespace ImageDimension {
struct Dim1D : ltl::Comparable<Dim1D> {
    Dim1D(uint32_t w) : width{w} {}
    uint32_t width;
    static constexpr uint32_t height = 1;
    static constexpr uint32_t depth = 1;
};

struct Dim2D : ltl::Comparable<Dim2D> {
    Dim2D(uint32_t w, uint32_t h) : width{w}, height{h} {}
    uint32_t width;
    uint32_t height;
    static constexpr uint32_t depth = 1;
};

struct Dim3D : ltl::Comparable<Dim3D> {
    Dim3D(uint32_t w, uint32_t h, uint32_t d) : width{w}, height{h}, depth{d} {}
    uint32_t width;
    uint32_t height;
    uint32_t depth;
};

template <typename T>
    requires std::same_as<T, Dim1D> || std::same_as<T, Dim2D> ||
    std::same_as<T, Dim3D> bool operator==(const T &a, const T &b) {
    return a.width == b.width && a.height == b.height && a.depth == b.depth;
}
} // namespace ImageDimension

using image_dimension_t = std::variant<ImageDimension::Dim1D, ImageDimension::Dim2D, ImageDimension::Dim3D>;

LAVA_EXPORT uint32_t getMaxMipmapLevels(image_dimension_t imageDimension) noexcept;
LAVA_EXPORT vk::Extent2D extent2DFromImageDimension(image_dimension_t imageDimension) noexcept;
LAVA_EXPORT vk::Extent3D extent3DFromImageDimension(image_dimension_t imageDimension) noexcept;
LAVA_EXPORT image_dimension_t getMipmapLevelDimension(image_dimension_t imageDimension, uint32_t level) noexcept;

LAVA_EXPORT image_dimension_t imageDimensionFromExtent2D(vk::Extent2D extent) noexcept;
LAVA_EXPORT image_dimension_t imageDimensionFromExtent3D(vk::Extent3D extent) noexcept;
} // namespace lava
