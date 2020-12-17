#include "ImageDimension.h"

#include <cmath>
#include <ltl/ltl.h>

namespace lava {

uint32_t getMaxMipmapLevels(lava::image_dimension_t imageDimension) noexcept {
    const auto [w, h, d] = extent3DFromImageDimension(imageDimension);
    const auto max = std::max({w, h, d});
    return uint32_t(log2(max)) + 1;
}

vk::Extent2D extent2DFromImageDimension(image_dimension_t imageDimension) noexcept {
    const auto [w, h] = std::visit([](auto x) { return std::array{x.width, x.height}; }, imageDimension);
    return {w, h};
}

vk::Extent3D extent3DFromImageDimension(image_dimension_t imageDimension) noexcept {
    const auto [w, h, d] = std::visit([](auto x) { return std::array{x.width, x.height, x.depth}; }, imageDimension);
    return {w, h, d};
}

image_dimension_t getMipmapLevelDimension(image_dimension_t imageDimension, uint32_t level) noexcept {
    using namespace ImageDimension;
    assert(level < getMaxMipmapLevels(imageDimension));
    auto [w, h, d] = extent3DFromImageDimension(imageDimension);
    w = std::max<uint32_t>(w >> level, 1);
    h = std::max<uint32_t>(h >> level, 1);
    d = std::max<uint32_t>(d >> level, 1);

    auto create = ltl::overloader{[w = w](Dim1D) -> image_dimension_t { //
                                      return Dim1D{w};
                                  },
                                  [w = w, h = h](Dim2D) -> image_dimension_t {
                                      return Dim2D{w, h};
                                  },
                                  [w = w, h = h, d = d](Dim3D) -> image_dimension_t {
                                      return Dim3D{w, h, d};
                                  }};

    return std::visit(create, imageDimension);
}

image_dimension_t imageDimensionFromExtent2D(vk::Extent2D extent) noexcept {
    return ImageDimension::Dim2D{extent.width, extent.height};
}
image_dimension_t imageDimensionFromExtent3D(vk::Extent3D extent) noexcept {
    return ImageDimension::Dim3D{extent.width, extent.height, extent.depth};
}

} // namespace lava
