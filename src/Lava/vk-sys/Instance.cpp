#include "Instance.h"

#include <ltl/operator.h>
#include <ltl/Range/Map.h>

using namespace ltl;

namespace lava {
Instance::Instance(const vk::ApplicationInfo &appInfo, std::vector<std::string> layers,
                   std::vector<std::string> extensions) {
    auto layers_str = layers | map(&std::string::c_str) | to_vector;
    auto extensions_str = extensions | map(&std::string::c_str) | to_vector;

    auto info = vk::InstanceCreateInfo()
                    .setPApplicationInfo(&appInfo)
                    .setPEnabledLayerNames(layers_str)
                    .setPEnabledExtensionNames(extensions_str);
    auto handle = vk::createInstanceUnique(info);
    m_handle = std::make_shared<vk::UniqueInstance>(std::move(handle));
}

InstanceBuilder::InstanceBuilder(VulkanVersion version) : version{version} {}

InstanceBuilder &InstanceBuilder::setApplication(std::string appName, uint32_t appVersion) {
    this->appName = std::move(appName);
    this->appVersion = appVersion;
    return *this;
}

InstanceBuilder &InstanceBuilder::setEngine(std::string engineName, uint32_t engineVersion) {
    this->engineName = std::move(engineName);
    this->engineVersion = engineVersion;
    return *this;
}

InstanceBuilder &InstanceBuilder::setExtensions(std::vector<std::string> extensions) {
    this->extensions = std::move(extensions);
    return *this;
}

InstanceBuilder &InstanceBuilder::setLayers(std::vector<std::string> layers) {
    this->layers = std::move(layers);
    return *this;
}

Instance InstanceBuilder::build() {
    auto info = vk::ApplicationInfo()
                    .setApiVersion(uint32_t(version))
                    .setEngineVersion(engineVersion)
                    .setApplicationVersion(appVersion)
                    .setPEngineName(engineName.c_str())
                    .setPApplicationName(appName.c_str());
    return Instance(info, std::move(layers), std::move(extensions));
}

} // namespace lava
