#pragma once

#include <optional>
#include "../sys/exception.h"
#include "vulkan.h"

namespace lava {

class LAVA_EXPORT InstanceInstance : public details::VulkanResource<vk::UniqueInstance> {
  public:
    InstanceInstance(const vk::ApplicationInfo &appInfo, std::vector<std::string> layers,
                     std::vector<std::string> extensions);

    [[nodiscard]] std::vector<vk::PhysicalDevice> physicalDevices() const noexcept;

  private:
    std::vector<std::string> m_extensions;
};

using Instance = std::shared_ptr<InstanceInstance>;

enum class VulkanVersion {
    VERSION_1_0 = VK_API_VERSION_1_0,
    VERSION_1_1 = VK_API_VERSION_1_1,
    VERSION_1_2 = VK_API_VERSION_1_2
};

struct LAVA_EXPORT InstanceBuilder {
    InstanceBuilder(VulkanVersion version);

    [[nodiscard]] InstanceBuilder &setLayers(std::vector<std::string> layers);
    [[nodiscard]] InstanceBuilder &setExtensions(std::vector<std::string> extensions);
    [[nodiscard]] InstanceBuilder &setApplication(std::string appName, uint32_t appVersion);
    [[nodiscard]] InstanceBuilder &setEngine(std::string engineName, uint32_t engineVersion);

    [[nodiscard]] Instance build();

    VulkanVersion version;
    uint32_t appVersion = 0;
    std::string appName;
    uint32_t engineVersion = 0;
    std::string engineName;
    std::vector<std::string> layers;
    std::vector<std::string> extensions;
};

} // namespace lava
