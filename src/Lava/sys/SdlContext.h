#pragma once

#include <SDL.h>

#include "lava_export.h"
#include "exception.h"

namespace lava {
struct SdlInitException : static_exception<SdlInitException> {
    static constexpr auto error = "Sdl Init Exception";
};

struct LAVA_EXPORT SdlContext {
  public:
    SdlContext();

    ~SdlContext();
};

} // namespace lava
