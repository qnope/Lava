#pragma once

#include "Instance.h"

namespace lava {

struct NoPhysicalDeviceFoundException : static_exception<NoPhysicalDeviceFoundException> {
    static constexpr auto error = "No Physical Device Found Exception";
};

class LAVA_EXPORT Device : public details::VulkanResource<vk::UniqueDevice> {
  public:
    Device(const Instance &instance, const std::vector<vk::Bool32 vk::PhysicalDeviceFeatures::*> &features,
           vk::QueueFlags queueFlags);

  private:
    vk::PhysicalDeviceFeatures m_features;
    vk::QueueFlags m_queueFlags;
};

class LAVA_EXPORT DeviceBuilder {
  public:
    DeviceBuilder(const Instance &instance);

    DeviceBuilder &withGeometryShader() noexcept;

    DeviceBuilder &withGraphicQueue() noexcept;
    DeviceBuilder &withComputeQueue() noexcept;
    DeviceBuilder &withTransferQueue() noexcept;

    Device build();

  private:
    const Instance &m_instance;
    std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> m_features;
    vk::QueueFlags m_queueFlags{};
};

} // namespace lava
