#include "ImageView.h"

namespace lava {
static vk::ImageViewType viewTypeFromImageType(vk::ImageType type) {
    if (type == vk::ImageType::e2D)
        return vk::ImageViewType::e2D;
    throw InvalidImageTypeException{};
}

ImageView::ImageView(vk::Device device, vk::Image image, vk::ImageType imageType, vk::Format format,
                     vk::ImageSubresourceRange subresourceRange) {
    auto info = vk::ImageViewCreateInfo()
                    .setImage(image)
                    .setFormat(format)
                    .setSubresourceRange(subresourceRange)
                    .setViewType(viewTypeFromImageType(imageType));
    m_handle = device.createImageViewUnique(info);
}

} // namespace lava
