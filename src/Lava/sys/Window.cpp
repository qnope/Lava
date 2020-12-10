#include "Window.h"

#include <ltl/algos.h>
#include <SDL2/SDL_vulkan.h>

namespace lava {

Window::Window(int width, int height, const char *title) : m_context{std::make_unique<SdlContext>()} {
    m_window.reset(
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN));

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

Surface Window::createSurface(const Instance &instance) const {
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(m_window.get(), instance.getHandle(), &surface))
        throw SurfaceCreationException{};

    return Surface{vk::UniqueSurfaceKHR(surface, instance.getHandle())};
}

} // namespace lava
