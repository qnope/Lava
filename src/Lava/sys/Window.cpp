#include "Window.h"

namespace lava {

Window::Window(int width, int height, const char *title) {
    m_window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);

    if (m_window == nullptr) {
        throw SdlWindowCreationException{};
    }
}

Window::Window(int width, int height, const std::string &title) : Window{width, height, title.c_str()} {}

Window::~Window() { SDL_DestroyWindow(m_window); }

} // namespace lava
