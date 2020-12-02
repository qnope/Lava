#pragma once

#include "SdlContext.h"
#include <string>

namespace lava {

struct SdlWindowCreationException : static_exception<SdlWindowCreationException> {
    static constexpr auto error = "Sdl Window Creation Exception";
};

class LAVA_EXPORT Window {
  public:
    Window(int width, int height, const char *title);
    Window(int width, int height, const std::string &title);

    ~Window();

  private:
    SdlContext m_context;
    SDL_Window *m_window;
};

} // namespace lava
