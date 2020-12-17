#include "ImageSubresourceRange.h"

namespace lava {
namespace ImageSubresourceRange {
vk::ImageSubresourceRange swapchainImage() noexcept {
    return vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
}
} // namespace ImageSubresourceRange
} // namespace lava
