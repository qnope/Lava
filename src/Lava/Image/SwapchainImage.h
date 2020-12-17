#pragma once

#include "../vk-sys/vulkan.h"
#include "ImageView.h"
#include "ImageDimension.h"

namespace lava {

class SwapchainImageInstance {
  public:
    SwapchainImageInstance(vk::Image image, ImageView imageView, image_dimension_t imageDimension);

    vk::Image getImageHandle() const noexcept;
    vk::ImageView getImageViewHandle() const noexcept;

    ImageAccessRange getImageAccessRange() const noexcept;

    image_dimension_t getImageDimension() const noexcept;

    uint32_t getLayerCount() const noexcept;
    uint32_t getMipmapCount() const noexcept;

  private:
    vk::Image m_image;
    ImageView m_imageView;
    image_dimension_t m_imageDimension;
    vk::ImageLayout m_currentLayout = vk::ImageLayout::eColorAttachmentOptimal;
};

using SwapchainImage = std::shared_ptr<SwapchainImageInstance>;

} // namespace lava
