#pragma once

#include "SdlContext.h"
#include <string>
#include <vector>

#include <ltl/immutable.h>
#include "../vk-sys/Surface.h"
#include "../vk-sys/Instance.h"
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
    Window(Window &&window, ResizeEvent event) noexcept;
    Window(int width, int height, const char *title);
    Window(int width, int height, const std::string &title);

    std::vector<std::string> getSdlExtensions(std::vector<std::string> additionalExtensions) const noexcept;

    Surface createSurface(const Instance &instance) const;

    ltl::immutable_t<uint32_t> width;
    ltl::immutable_t<uint32_t> height;

  private:
    std::unique_ptr<SdlContext> m_context;
    std::unique_ptr<SDL_Window, WindowDeleter> m_window;
};

} // namespace lava
