#include "Image.h"

namespace lava {

ImageAccessRange Image::getImageAccessRange() const noexcept { return m_image->getImageAccessRange(); }
image_dimension_t Image::getImageDimension() const noexcept { return m_image->getImageDimension(); }

vk::Image Image::getImageHandle() const noexcept { return m_image->getImageHandle(); }
vk::ImageView Image::getImageViewHandle() const noexcept { return m_image->getImageViewHandle(); }

bool conflictBetweenImage(const Image &imageA, const Image &imageB) noexcept {
    if (imageA.getImageHandle() != imageB.getImageHandle())
        return false;

    const auto rangeA = imageA.getImageAccessRange();
    const auto rangeB = imageB.getImageAccessRange();

    if (rangeA.baseLayerLevel > rangeB.baseLayerLevel + rangeB.layerCount)
        return false;

    if (rangeB.baseLayerLevel > rangeA.baseLayerLevel + rangeA.layerCount)
        return false;

    if (rangeA.baseMipLevel > rangeB.baseMipLevel + rangeB.mipCount)
        return false;

    if (rangeB.baseMipLevel > rangeA.baseMipLevel + rangeA.mipCount)
        return false;
    return true;
}

} // namespace lava
