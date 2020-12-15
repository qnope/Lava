#include "Device.h"
#include <ltl/algos.h>
#include <ltl/operator.h>
#include <ltl/Range/Filter.h>
#include <ltl/Range/enumerate.h>

#include <ltl/Range/actions.h>

namespace lava {

struct QueueFamillyInfo {
    vk::QueueFlags flags;
    bool withPresentationSupport{false};
};

static auto queueFamillyInfoFromDevice(vk::PhysicalDevice device, Surface surface) {
    auto properties = device.getQueueFamilyProperties();
    std::vector<QueueFamillyInfo> famillyInfo;

    for (auto [i, property] : ltl::enumerate(properties)) {
        QueueFamillyInfo info{};
        if (surface)
            info.withPresentationSupport = device.getSurfaceSupportKHR(uint32_t(i), surface->getHandle());
        info.flags = property.queueFlags;
        famillyInfo.push_back(info);
    }

    return famillyInfo;
}

static auto hasFeatures(const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &features) noexcept {
    return [&features](vk::PhysicalDevice device) {
        auto deviceFeatures = device.getFeatures();
        auto enabled = [&deviceFeatures](auto pointer) { return deviceFeatures.*pointer == VK_TRUE; };
        return ltl::all_of(features, enabled);
    };
}

static auto isSuitableQueue(vk::QueueFlags flags, Surface surface) noexcept {
    return [flags, surface = std::move(surface)](QueueFamillyInfo info) {
        bool resFlags = (info.flags & flags) == flags;
        if (surface)
            return resFlags && info.withPresentationSupport;
        return resFlags;
    };
}

static auto hasSuitableQueue(vk::QueueFlags flags, Surface surface) noexcept {
    return [flags, surface = std::move(surface)](vk::PhysicalDevice device) {
        auto queueProperties = queueFamillyInfoFromDevice(device, surface);
        return ltl::any_of(queueProperties, isSuitableQueue(flags, surface));
    };
}

static auto hasExtensions(const std::vector<std::string> &extensions) noexcept {
    return [&extensions](vk::PhysicalDevice device) {
        std::vector<std::string> deviceExtensions =
            device.enumerateDeviceExtensionProperties() |
            ltl::map(&vk::ExtensionProperties::extensionName, [](const auto &x) { return std::string{x.data()}; });
        auto areContainedInDeviceExtensions = ltl::curry(lift(ltl::contains), deviceExtensions);
        return ltl::all_of(extensions, areContainedInDeviceExtensions);
    };
}

static auto buildFeatures(const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &features) noexcept {
    vk::PhysicalDeviceFeatures result{};

    for (auto f : features)
        result.*f = true;

    return result;
}

static auto selectPhysicalDevice(const Instance &instance,
                                 const std::vector<vk::Bool32 vk::PhysicalDeviceFeatures::*> &featurePtrs,
                                 const std::vector<std::string> &extensions, vk::QueueFlags queueFlags,
                                 Surface surface) {
    auto physicalDevices = instance->physicalDevices() |                        //
                           ltl::filter(hasFeatures(featurePtrs)) |              //
                           ltl::filter(hasSuitableQueue(queueFlags, surface)) | //
                           ltl::filter(hasExtensions(extensions)) |             //
                           ltl::to_vector;

    if (physicalDevices.empty())
        throw NoPhysicalDeviceFoundException{};

    // maybe we can sort it
    return physicalDevices.front();
}

DeviceInstance::DeviceInstance(const Instance &instance,
                               std::vector<vk::Bool32 vk::PhysicalDeviceFeatures::*> featurePtrs,
                               std::vector<std::string> _extensions, vk::QueueFlags queueFlags, Surface surface) :
    queueFlags{queueFlags},               //
    hasPresentationQueue{surface},        //
    features{buildFeatures(featurePtrs)}, //
    extensions{std::move(_extensions)},   //
    physicalDevice{selectPhysicalDevice(instance, featurePtrs, extensions, queueFlags, surface)} {
    auto queueFamillyInfo = queueFamillyInfoFromDevice(physicalDevice, surface);
    float queuePriority = 1.0;
    uint32_t queueIndex = uint32_t(*ltl::index_if(queueFamillyInfo, isSuitableQueue(queueFlags, surface)));
    auto queueCreateInfo = vk::DeviceQueueCreateInfo() //
                               .setQueueCount(1)
                               .setQueuePriorities(queuePriority)
                               .setQueueFamilyIndex(queueIndex);

    std::vector<const char *> extensionsAsCstr = extensions | ltl::map(&std::string::c_str);

    auto deviceCreateInfo = vk::DeviceCreateInfo() //
                                .setPEnabledFeatures(&features)
                                .setQueueCreateInfos(queueCreateInfo)
                                .setPEnabledExtensionNames(extensionsAsCstr);

    m_handle = physicalDevice.createDeviceUnique(deviceCreateInfo);
}

DeviceBuilder::DeviceBuilder(const Instance &instance) : m_instance{instance} {}

DeviceBuilder &DeviceBuilder::withGeometryShader() noexcept {
    m_features.push_back(&vk::PhysicalDeviceFeatures::geometryShader);
    return *this;
}

DeviceBuilder &DeviceBuilder::withGraphicQueue() noexcept {
    m_queueFlags |= vk::QueueFlagBits::eGraphics;
    return *this;
}

DeviceBuilder &DeviceBuilder::withComputeQueue() noexcept {
    m_queueFlags |= vk::QueueFlagBits::eCompute;
    return *this;
}

DeviceBuilder &DeviceBuilder::withTransferQueue() noexcept {
    m_queueFlags |= vk::QueueFlagBits::eTransfer;
    return *this;
}

DeviceBuilder &DeviceBuilder::withPresentationQueue(Surface surface) noexcept {
    m_surface = std::move(surface);
    m_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    return *this;
}

Device DeviceBuilder::build() {
    m_extensions |= ltl::actions::sort | ltl::actions::unique;
    return std::make_shared<DeviceInstance>(m_instance, std::move(m_features), std::move(m_extensions), m_queueFlags,
                                            std::move(m_surface));
}

} // namespace lava
