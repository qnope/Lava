#pragma once

#include "lava_export.h"
#include "vulkan.h"
#include "Device.h"
#include "../Image/Image.h"

namespace lava {

struct PresentModeNotFoundException : static_exception<PresentModeNotFoundException> {
    static constexpr auto error = "Present Mode Not Found Exception";
};

struct SurfaceFormatNotFoundException : static_exception<SurfaceFormatNotFoundException> {
    static constexpr auto error = "Surface Format Not Found Exception";
};

class LAVA_EXPORT SwapchainInstance : public details::VulkanResource<vk::UniqueSwapchainKHR> {
  public:
    SwapchainInstance(Device device, Surface surface, uint32_t width, uint32_t height, vk::SwapchainKHR oldSwapchain);

  private:
    std::vector<Image> m_images;
};

using Swapchain = std::shared_ptr<SwapchainInstance>;

class LAVA_EXPORT SwapchainBuilder {
  public:
    SwapchainBuilder(Surface surface, uint32_t width, uint32_t height);

    SwapchainBuilder &withOldSwapchain(Swapchain swapchain);

    Swapchain build(Device device);

  private:
    Surface m_surface;
    uint32_t m_width;
    uint32_t m_height;
    Swapchain m_swapchain;
};

} // namespace lava
