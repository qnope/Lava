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

class LAVA_EXPORT Swapchain : public details::VulkanResource<vk::UniqueSwapchainKHR> {
  public:
    Swapchain(const Device &device, const Surface &surface, uint32_t width, uint32_t height,
              vk::SwapchainKHR oldSwapchain);

  private:
    std::vector<Image> m_images;
};

class LAVA_EXPORT SwapchainBuilder {
  public:
    SwapchainBuilder(const Surface &surface, uint32_t width, uint32_t height);

    SwapchainBuilder &withOldSwapchain(const Swapchain *swapchain);

    Swapchain build(const Device &device);

  private:
    const Surface &m_surface;
    uint32_t m_width;
    uint32_t m_height;
    const Swapchain *m_oldSwapchain = nullptr;
};

} // namespace lava
