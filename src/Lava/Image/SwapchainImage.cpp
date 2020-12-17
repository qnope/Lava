#include "SwapchainImage.h"

lava::SwapchainImageInstance::SwapchainImageInstance(vk::Image image, ImageView imageView,
                                                     lava::image_dimension_t imageDimension) :
    m_image{image},                    //
    m_imageView{std::move(imageView)}, //
    m_imageDimension{imageDimension} {}

vk::Image lava::SwapchainImageInstance::getImageHandle() const noexcept { return m_image; }

vk::ImageView lava::SwapchainImageInstance::getImageViewHandle() const noexcept { return m_imageView.getHandle(); }

lava::ImageAccessRange lava::SwapchainImageInstance::getImageAccessRange() const noexcept {
    return ImageAccessRange{.layout = m_currentLayout, //
                            .baseMipLevel = 0,
                            .mipCount = 1,
                            .baseLayerLevel = 0,
                            .layerCount = 1};
}

lava::image_dimension_t lava::SwapchainImageInstance::getImageDimension() const noexcept { return m_imageDimension; }

uint32_t lava::SwapchainImageInstance::getLayerCount() const noexcept { return 1; }

uint32_t lava::SwapchainImageInstance::getMipmapCount() const noexcept { return 1; }
