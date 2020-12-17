#pragma once

#include "../vk-sys/Device.h"
#include "ImageConcept.h"

namespace lava {

struct InvalidImageTypeException : static_exception<InvalidImageTypeException> {
    static constexpr auto error = "Invalid Image Type Exception";
};

class LAVA_EXPORT ImageView : public details::VulkanResource<vk::UniqueImageView> {
  public:
    ImageView(Device device, vk::Image image, vk::ImageType imageType, vk::Format format,
              vk::ImageSubresourceRange subresourceRange);
};

} // namespace lava
