#pragma once

#include <ltl/immutable.h>

#include "ImageConcept.h"
#include "../vk-sys/vulkan.h"

namespace lava {
namespace details {
class LAVA_EXPORT AbstractImage {
  public:
    virtual ImageAccessRange getImageAccessRange() const noexcept = 0;
    virtual image_dimension_t getImageDimension() const noexcept = 0;

    virtual vk::Image getImageHandle() const noexcept = 0;
    virtual vk::ImageView getImageViewHandle() const noexcept = 0;

    virtual ~AbstractImage() = default;
};

template <ImageConcept T>
class LAVA_EXPORT ImageModel : public AbstractImage {
  public:
    ImageModel(std::shared_ptr<T> image) noexcept : m_image{std::move(image)} {}

    ImageAccessRange getImageAccessRange() const noexcept override { return m_image->getImageAccessRange(); }
    image_dimension_t getImageDimension() const noexcept override { return m_image->getImageDimension(); }

    vk::Image getImageHandle() const noexcept override { return m_image->getImageHandle(); }
    vk::ImageView getImageViewHandle() const noexcept override { return m_image->getImageViewHandle(); }

  private:
    std::shared_ptr<T> m_image;
};
} // namespace details

class LAVA_EXPORT Image {
  public:
    template <ImageConcept T>
    Image(std::shared_ptr<T> image) noexcept : m_image{std::make_shared<details::ImageModel<T>>(std::move(image))} {}

    ImageAccessRange getImageAccessRange() const noexcept;
    image_dimension_t getImageDimension() const noexcept;

    vk::Image getImageHandle() const noexcept;
    vk::ImageView getImageViewHandle() const noexcept;

  private:
    std::shared_ptr<details::AbstractImage> m_image;
};

LAVA_EXPORT bool conflictBetweenImage(const Image &imageA, const Image &imageB) noexcept;

} // namespace lava
