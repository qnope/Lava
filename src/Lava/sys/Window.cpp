#include "Window.h"

#include <cassert>
#include <ltl/algos.h>
#include <SDL_vulkan.h>
#include <iostream>

namespace lava {

Window::Window(int width, int height, const char *title) :
    m_context{std::make_unique<SdlContext>()}, //
    m_width{uint32_t(width)},                  //
    m_height{uint32_t(height)} {
    m_window.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                    SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE));

    if (m_window == nullptr) {
        throw SdlWindowCreationException{};
    }
}

Window::Window(int width, int height, const std::string &title) : Window{width, height, title.c_str()} {}

std::vector<std::string> Window::getSdlExtensions(std::vector<std::string> additionalExtensions) const noexcept {
    uint32_t count;
    SDL_Vulkan_GetInstanceExtensions(m_window.get(), &count, nullptr);

    std::vector<const char *> exts(count);
    SDL_Vulkan_GetInstanceExtensions(m_window.get(), &count, exts.data());

    ltl::copy(exts, std::back_inserter(additionalExtensions));
    return additionalExtensions;
}

Surface Window::getSurface(const Instance &instance) const {
    if (instance)
        m_instance = instance;
    if (!m_surface)
        m_surface = createSurface();
    return m_surface;
}

Swapchain Window::getSwapchain(const Device &device) const {
    if (device)
        m_device = device;
    if (!m_swapchain)
        m_swapchain = createSwapchain();
    return m_swapchain;
}

void Window::processEvent(ResizeEvent event) {
    m_width = event.width;
    m_height = event.height;

    m_swapchain = createSwapchain();
}

Surface Window::createSurface() const {
    assert(m_instance);
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(m_window.get(), m_instance->getHandle(), &surface))
        throw SurfaceCreationException{};

    return std::make_shared<SurfaceInstance>(vk::UniqueSurfaceKHR(surface, m_instance->getHandle()));
}

Swapchain Window::createSwapchain() const {
    assert(m_device);
    return SwapchainBuilder(getSurface(), m_width, m_height) //
        .withOldSwapchain(m_swapchain)
        .build(m_device);
}

} // namespace lava
