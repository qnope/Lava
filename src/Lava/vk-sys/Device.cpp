#include "Device.h"
#include <ltl/algos.h>
#include <ltl/operator.h>
#include <ltl/Range/Filter.h>
#include <ltl/Range/enumerate.h>

namespace lava {

struct QueueFamillyInfo {
    vk::QueueFlags flags;
    bool withPresentationSupport{false};
};

static auto queueFamillyInfoFromDevice(vk::PhysicalDevice device, std::optional<Surface> surface) {
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

static auto isSuitableQueue(vk::QueueFlags flags, std::optional<Surface> surface) noexcept {
    return [flags, surface = std::move(surface)](QueueFamillyInfo info) {
        bool resFlags = (info.flags & flags) == flags;
        if (surface)
            return resFlags && info.withPresentationSupport;
        return resFlags;
    };
}

static auto hasSuitableQueue(vk::QueueFlags flags, std::optional<Surface> surface) noexcept {
    return [flags, surface = std::move(surface)](vk::PhysicalDevice device) {
        auto queueProperties = queueFamillyInfoFromDevice(device, surface);
        return ltl::any_of(queueProperties, isSuitableQueue(flags, surface));
    };
}

static auto buildFeatures(const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &features) noexcept {
    vk::PhysicalDeviceFeatures result{};

    for (auto f : features)
        result.*f = true;

    return result;
}

Device::Device(const Instance &instance, const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &featurePtrs,
               vk::QueueFlags queueFlags, std::optional<Surface> surface) :
    queueFlags{queueFlags},               //
    features{buildFeatures(featurePtrs)}, //
    m_hasPresentationQueue{surface.has_value()} {
    auto physicalDevices = instance.physicalDevices() |                         //
                           ltl::filter(hasFeatures(featurePtrs)) |              //
                           ltl::filter(hasSuitableQueue(queueFlags, surface)) | //
                           ltl::to_vector;

    if (physicalDevices.empty())
        throw NoPhysicalDeviceFoundException{};

    // maybe we can sort it
    auto physicalDevice = physicalDevices.front();
    auto queueFamillyInfo = queueFamillyInfoFromDevice(physicalDevice, surface);
    float queuePriority = 1.0;
    uint32_t queueIndex = uint32_t(*ltl::index_if(queueFamillyInfo, isSuitableQueue(queueFlags, surface)));
    auto queueCreateInfo = vk::DeviceQueueCreateInfo() //
                               .setQueueCount(1)
                               .setQueuePriorities(queuePriority)
                               .setQueueFamilyIndex(queueIndex);

    auto deviceCreateInfo = vk::DeviceCreateInfo() //
                                .setQueueCreateInfos(queueCreateInfo)
                                .setPEnabledFeatures(&features);

    auto handle = physicalDevice.createDeviceUnique(deviceCreateInfo);
    m_handle = std::make_shared<vk::UniqueDevice>(std::move(handle));
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
    return *this;
}

Device DeviceBuilder::build() { return Device{m_instance, std::move(m_features), m_queueFlags, std::move(m_surface)}; }

} // namespace lava
