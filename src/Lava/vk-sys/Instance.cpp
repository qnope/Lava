#include "Instance.h"

namespace lava {
Instance::Instance(const char *applicationName) {
    auto appInfo = vk::ApplicationInfo{}
                       .setPApplicationName(applicationName)
                       .setApplicationVersion(VK_MAKE_VERSION(0, 0, 0))
                       .setPEngineName("Lava")
                       .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                       .setApiVersion(VK_API_VERSION_1_0);

    auto createInfo = vk::InstanceCreateInfo{} //
                          .setPApplicationInfo(&appInfo)
                          .setEnabledExtensionCount(0)
                          .setEnabledLayerCount(0);

    auto handle = vk::createInstanceUnique(createInfo);

    m_handle = std::make_shared<vk::UniqueInstance>(std::move(handle));
}

Instance::Instance(const std::string &applicationName) : Instance{applicationName.c_str()} {}
} // namespace lava
