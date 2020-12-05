#include "Window.h"

#include <ltl/algos.h>
#include <SDL2/SDL_vulkan.h>

namespace lava {

Window::Window(int width, int height, const char *title) {
    m_window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);

    if (m_window == nullptr) {
        throw SdlWindowCreationException{};
    }
}

Window::Window(int width, int height, const std::string &title) : Window{width, height, title.c_str()} {}

std::vector<std::string> Window::getSdlExtensions(std::vector<std::string> additionalExtensions) const noexcept {
    uint32_t count;
    SDL_Vulkan_GetInstanceExtensions(m_window, &count, nullptr);

    std::vector<const char *> exts(count);
    SDL_Vulkan_GetInstanceExtensions(m_window, &count, exts.data());

    ltl::copy(exts, std::back_inserter(additionalExtensions));
    return additionalExtensions;
}

Window::~Window() { SDL_DestroyWindow(m_window); }

} // namespace lava
