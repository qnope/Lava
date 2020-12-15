#pragma once

#include "Instance.h"
#include "Surface.h"

namespace lava {

struct NoPhysicalDeviceFoundException : static_exception<NoPhysicalDeviceFoundException> {
    static constexpr auto error = "No Physical Device Found Exception";
};

class LAVA_EXPORT DeviceInstance : public details::VulkanResource<vk::UniqueDevice> {
  public:
    DeviceInstance(const Instance &instance, std::vector<vk::Bool32 vk::PhysicalDeviceFeatures::*> features,
                   std::vector<std::string> extensions, vk::QueueFlags queueFlags, Surface surface);

    const vk::QueueFlags queueFlags;
    const bool hasPresentationQueue;
    const vk::PhysicalDeviceFeatures features;
    const std::vector<std::string> extensions;
    const vk::PhysicalDevice physicalDevice;
};

using Device = std::shared_ptr<DeviceInstance>;

class LAVA_EXPORT DeviceBuilder {
  public:
    DeviceBuilder(const Instance &instance);

    [[nodiscard]] DeviceBuilder &withGeometryShader() noexcept;

    [[nodiscard]] DeviceBuilder &withGraphicQueue() noexcept;
    [[nodiscard]] DeviceBuilder &withComputeQueue() noexcept;
    [[nodiscard]] DeviceBuilder &withTransferQueue() noexcept;

    [[nodiscard]] DeviceBuilder &withPresentationQueue(Surface surface) noexcept;

    [[nodiscard]] Device build();

  private:
    const Instance &m_instance;
    vk::QueueFlags m_queueFlags{};
    Surface m_surface;
    std::vector<std::string> m_extensions;
    std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> m_features;
};

} // namespace lava
