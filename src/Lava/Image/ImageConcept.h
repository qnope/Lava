#pragma once

#include <concepts>
#include "../vk-sys/vulkan.h"

#include "ImageAccessRange.h"
#include "ImageDimension.h"

namespace lava {

template <typename T>
concept ImageConcept = requires(T img) {
    { img.getLayerCount() }
    ->std::convertible_to<uint32_t>;
    { img.getMipmapCount() }
    ->std::convertible_to<uint32_t>;
    { img.getImageAccessRange() }
    ->std::convertible_to<ImageAccessRange>;
    { img.getImageDimension() }
    ->std::convertible_to<image_dimension_t>;

    { img.getImageHandle() }
    ->std::convertible_to<vk::Image>;
    { img.getImageViewHandle() }
    ->std::convertible_to<vk::ImageView>;
};
} // namespace lava
