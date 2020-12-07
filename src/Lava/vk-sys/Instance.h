#pragma once

#include <optional>
#include "../sys/exception.h"
#include "vulkan.h"

namespace lava {

class LAVA_EXPORT Instance : details::VulkanResource<vk::UniqueInstance> {
  public:
    Instance(const vk::ApplicationInfo &appInfo, std::vector<std::string> layers, std::vector<std::string> extensions);

    std::vector<vk::PhysicalDevice> physicalDevices() const noexcept;

  private:
};

enum class VulkanVersion {
    VERSION_1_0 = VK_API_VERSION_1_0,
    VERSION_1_1 = VK_API_VERSION_1_1,
    VERSION_1_2 = VK_API_VERSION_1_2
};

struct LAVA_EXPORT InstanceBuilder {
    InstanceBuilder(VulkanVersion version);

    InstanceBuilder &setLayers(std::vector<std::string> layers);
    InstanceBuilder &setExtensions(std::vector<std::string> extensions);
    InstanceBuilder &setApplication(std::string appName, uint32_t appVersion);
    InstanceBuilder &setEngine(std::string engineName, uint32_t engineVersion);

    Instance build();

    VulkanVersion version;
    uint32_t appVersion = 0;
    std::string appName;
    uint32_t engineVersion = 0;
    std::string engineName;
    std::vector<std::string> layers;
    std::vector<std::string> extensions;
};

} // namespace lava
