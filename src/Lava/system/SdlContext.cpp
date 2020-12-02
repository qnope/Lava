#include "SdlContext.h"

#include <iostream>

namespace lava {

SdlContext::SdlContext() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw SdlInitException{};
    }
}

SdlContext::~SdlContext() { SDL_Quit(); }

} // namespace lava
