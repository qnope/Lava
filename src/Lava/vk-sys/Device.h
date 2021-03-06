#pragma once

#include "Instance.h"
#include "Surface.h"
#include <ltl/immutable.h>

namespace lava {

struct NoPhysicalDeviceFoundException : static_exception<NoPhysicalDeviceFoundException> {
    static constexpr auto error = "No Physical Device Found Exception";
};

class LAVA_EXPORT Device : public details::VulkanResource<vk::UniqueDevice> {
  public:
    Device(const Instance &instance, std::vector<vk::Bool32 vk::PhysicalDeviceFeatures::*> features,
           std::vector<std::string> extensions, vk::QueueFlags queueFlags, const Surface *surface);

    ltl::immutable_t<vk::QueueFlags> queueFlags;
    ltl::immutable_t<bool> hasPresentationQueue;
    ltl::immutable_t<vk::PhysicalDeviceFeatures> features;
    ltl::immutable_t<std::vector<std::string>> extensions;
    ltl::immutable_t<vk::PhysicalDevice> physicalDevice;
};

class LAVA_EXPORT DeviceBuilder {
  public:
    DeviceBuilder(const Instance &instance);

    [[nodiscard]] DeviceBuilder &withGeometryShader() noexcept;

    [[nodiscard]] DeviceBuilder &withGraphicQueue() noexcept;
    [[nodiscard]] DeviceBuilder &withComputeQueue() noexcept;
    [[nodiscard]] DeviceBuilder &withTransferQueue() noexcept;

    [[nodiscard]] DeviceBuilder &withPresentationQueue(const Surface &surface) noexcept;

    [[nodiscard]] Device build();

  private:
    const Instance &m_instance;
    vk::QueueFlags m_queueFlags{};
    const Surface *m_surface = nullptr;
    std::vector<std::string> m_extensions;
    std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> m_features;
};

} // namespace lava
