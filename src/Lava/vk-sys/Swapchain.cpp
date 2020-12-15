#include "Swapchain.h"
#include <ltl/algos.h>
#include <ltl/Range/Map.h>

namespace lava {

static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(Device device, Surface surface) {
    auto formats = device->physicalDevice.getSurfaceFormatsKHR(surface->getHandle());
    constexpr auto target = vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    if (ltl::contains(formats, target)) {
        return target;
    }
    if (formats.empty())
        throw SurfaceFormatNotFoundException{};
    return formats.front();
}

static vk::PresentModeKHR choosePresentMode(Device device, Surface surface) {
    auto presentModes = device->physicalDevice.getSurfacePresentModesKHR(surface->getHandle());
    if (ltl::contains(presentModes, vk::PresentModeKHR::eMailbox))
        return vk::PresentModeKHR::eMailbox;
    return vk::PresentModeKHR::eFifo;
}

static vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height) {
    if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;

    vk::Extent2D actualExtent;

    actualExtent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    return actualExtent;
}

SwapchainInstance::SwapchainInstance(Device device, Surface surface, uint32_t width, uint32_t height,
                                     vk::SwapchainKHR oldSwapchain) {
    auto capabilities = device->physicalDevice.getSurfaceCapabilitiesKHR(surface->getHandle());
    auto surfaceFormat = chooseSwapSurfaceFormat(device, surface);
    auto presentMode = choosePresentMode(device, surface);
    auto extent = chooseSwapExtent(capabilities, width, height);
    auto imageCount = std::clamp<uint32_t>(3, capabilities.minImageCount,
                                           capabilities.maxImageCount == 0 ? 3 : capabilities.maxImageCount);
    auto info = vk::SwapchainCreateInfoKHR()
                    .setClipped(true)
                    .setImageExtent(extent)
                    .setImageArrayLayers(1)
                    .setPresentMode(presentMode)
                    .setMinImageCount(imageCount)
                    .setOldSwapchain(oldSwapchain)
                    .setSurface(surface->getHandle())
                    .setImageFormat(surfaceFormat.format)
                    .setImageColorSpace(surfaceFormat.colorSpace)
                    .setPreTransform(capabilities.currentTransform)
                    .setImageSharingMode(vk::SharingMode::eExclusive)
                    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                    .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
    m_handle = device->getHandle().createSwapchainKHRUnique(info);
}

SwapchainBuilder::SwapchainBuilder(Surface surface, uint32_t width, uint32_t height) :
    m_surface{surface}, m_width{width}, m_height{height} {}

SwapchainBuilder &SwapchainBuilder::withOldSwapchain(Swapchain swapchain) {
    m_swapchain = swapchain;
    return *this;
}

Swapchain SwapchainBuilder::build(Device device) {
    vk::SwapchainKHR old = m_swapchain ? m_swapchain->getHandle() : vk::SwapchainKHR{};
    return std::make_shared<SwapchainInstance>(device, m_surface, m_width, m_height, old);
}

} // namespace lava
