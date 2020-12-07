#include "Device.h"
#include <ltl/algos.h>
#include <ltl/Range/Filter.h>
#include <ltl/operator.h>

namespace lava {

static auto hasFeatures(const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &features) noexcept {
    return [&features](vk::PhysicalDevice device) {
        auto deviceFeatures = device.getFeatures();
        auto enabled = [&deviceFeatures](auto pointer) { return deviceFeatures.*pointer == VK_TRUE; };
        return ltl::all_of(features, enabled);
    };
}

static auto isSuitableQueue(vk::QueueFlags flags) noexcept {
    return [flags](vk::QueueFamilyProperties properties) { return (properties.queueFlags & flags) == flags; };
}

static auto hasSuitableQueue(vk::QueueFlags flags) noexcept {
    return [flags](vk::PhysicalDevice device) {
        auto queueProperties = device.getQueueFamilyProperties();
        return ltl::any_of(queueProperties, isSuitableQueue(flags));
    };
}

static auto buildFeatures(const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &features) noexcept {
    vk::PhysicalDeviceFeatures result{};

    for (auto f : features)
        result.*f = true;

    return result;
}

Device::Device(const Instance &instance, const std::vector<vk::Bool32(vk::PhysicalDeviceFeatures::*)> &features,
               vk::QueueFlags queueFlags) :
    m_queueFlags{queueFlags} {
    auto physicalDevices = instance.physicalDevices() |                //
                           ltl::filter(hasFeatures(features)) |        //
                           ltl::filter(hasSuitableQueue(queueFlags)) | //
                           ltl::to_vector;

    if (physicalDevices.empty())
        throw NoPhysicalDeviceFoundException{};

    m_features = buildFeatures(features);

    // maybe we can sort it
    auto physicalDevice = physicalDevices.front();

    float queuePriority = 1.0;
    auto queueIndex = *ltl::index_if(physicalDevice.getQueueFamilyProperties(), isSuitableQueue(queueFlags));
    auto queueCreateInfo = vk::DeviceQueueCreateInfo() //
                               .setQueueCount(1)
                               .setQueuePriorities(queuePriority)
                               .setQueueFamilyIndex(queueIndex);

    auto deviceCreateInfo = vk::DeviceCreateInfo() //
                                .setQueueCreateInfos(queueCreateInfo)
                                .setPEnabledFeatures(&m_features);

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

Device DeviceBuilder::build() { return Device{m_instance, std::move(m_features), m_queueFlags}; }

} // namespace lava
