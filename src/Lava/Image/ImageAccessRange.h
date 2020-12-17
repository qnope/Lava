#pragma once

#include "../vk-sys/vulkan.h"

namespace lava {

struct LAVA_EXPORT ImageAccessRange {
    vk::ImageLayout layout;
    uint32_t baseMipLevel;
    uint32_t mipCount;
    uint32_t baseLayerLevel;
    uint32_t layerCount;
};

} // namespace lava
