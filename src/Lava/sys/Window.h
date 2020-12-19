#pragma once

#include "SdlContext.h"
#include <string>
#include <vector>

#include "../vk-sys/Surface.h"
#include "../vk-sys/Instance.h"
#include "../vk-sys/Swapchain.h"
#include "Event.h"

namespace lava {

struct SdlWindowCreationException : static_exception<SdlWindowCreationException> {
    static constexpr auto error = "Sdl Window Creation Exception";
};

struct SurfaceCreationException : static_exception<SurfaceCreationException> {
    static constexpr auto error = "Surface Creation Exception";
};

class LAVA_EXPORT Window {
    struct WindowDeleter {
        void operator()(SDL_Window *x) const noexcept { SDL_DestroyWindow(x); }
    };

  public:
    Window(int width, int height, const char *title);
    Window(int width, int height, const std::string &title);

    std::vector<std::string> getSdlExtensions(std::vector<std::string> additionalExtensions) const noexcept;

    void processEvent(ResizeEvent event);

    template <typename T>
    void processEvent(T &&) {}

    uint32_t getWidth() const noexcept;
    uint32_t getHeight() const noexcept;

    Surface createSurface(const Instance &instance) const;

  private:
    std::unique_ptr<SdlContext> m_context;
    std::unique_ptr<SDL_Window, WindowDeleter> m_window;

    uint32_t m_width;
    uint32_t m_height;
};

} // namespace lava
