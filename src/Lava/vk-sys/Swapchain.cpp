#include "Swapchain.h"
#include <ltl/algos.h>
#include <ltl/Range/Map.h>

#include "ImageSubresourceRange.h"

#include "../Image/ImageView.h"
#include "../Image/SwapchainImage.h"

namespace lava {

static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const Device &device, const Surface &surface) {
    auto formats = device.physicalDevice->getSurfaceFormatsKHR(surface);
    constexpr auto target = vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    if (ltl::contains(formats, target)) {
        return target;
    }
    if (formats.empty())
        throw SurfaceFormatNotFoundException{};
    return formats.front();
}

static vk::PresentModeKHR choosePresentMode(const Device &device, const Surface &surface) {
    auto presentModes = device.physicalDevice->getSurfacePresentModesKHR(surface);
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

static std::vector<Image> createImages(vk::SwapchainKHR swapchain, vk::Device device, vk::Format format,
                                       image_dimension_t dimension) {
    std::vector<Image> images;

    for (auto img : device.getSwapchainImagesKHR(swapchain)) {
        ImageView imgView(device, img, vk::ImageType::e2D, format, ImageSubresourceRange::swapchainImage());
        SwapchainImageInstance imageInstance{img, std::move(imgView), dimension};
        images.push_back(std::make_shared<SwapchainImageInstance>(std::move(imageInstance)));
    }

    return images;
}

Swapchain::Swapchain(const Device &device, const Surface &surface, uint32_t width, uint32_t height,
                     vk::SwapchainKHR oldSwapchain) {
    auto capabilities = device.physicalDevice->getSurfaceCapabilitiesKHR(surface);
    auto surfaceFormat = chooseSwapSurfaceFormat(device, surface);
    auto presentMode = choosePresentMode(device, surface);
    auto extent = chooseSwapExtent(capabilities, width, height);
    auto imageCount = std::clamp<uint32_t>(3, capabilities.minImageCount,
                                           capabilities.maxImageCount == 0 ? 3 : capabilities.maxImageCount);
    auto info = vk::SwapchainCreateInfoKHR()
                    .setClipped(true)
                    .setSurface(surface)
                    .setImageExtent(extent)
                    .setImageArrayLayers(1)
                    .setPresentMode(presentMode)
                    .setMinImageCount(imageCount)
                    .setOldSwapchain(oldSwapchain)
                    .setImageFormat(surfaceFormat.format)
                    .setImageColorSpace(surfaceFormat.colorSpace)
                    .setPreTransform(capabilities.currentTransform)
                    .setImageSharingMode(vk::SharingMode::eExclusive)
                    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                    .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
    m_handle = device.getHandle().createSwapchainKHRUnique(info);
    m_images = createImages(*m_handle, device, surfaceFormat.format, imageDimensionFromExtent2D(extent));
}

SwapchainBuilder::SwapchainBuilder(const Surface &surface, uint32_t width, uint32_t height) :
    m_surface{surface}, m_width{width}, m_height{height} {}

SwapchainBuilder &SwapchainBuilder::withOldSwapchain(const Swapchain *swapchain) {
    m_oldSwapchain = swapchain;
    return *this;
}

Swapchain SwapchainBuilder::build(const Device &device) {
    vk::SwapchainKHR old = m_oldSwapchain ? m_oldSwapchain->getHandle() : vk::SwapchainKHR{};
    return {device, m_surface, m_width, m_height, old};
}

} // namespace lava
