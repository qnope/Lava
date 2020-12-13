#pragma once

#include <SDL.h>
#include "Event.h"

namespace lava {
enum class NextEventLoopAction {
    EXIT, //
    POLLED
};

namespace EventLoop {
template <typename F>
void run(F f) {
    while (true) {
        NextEventLoopAction next = f(build_events());

        if (next == NextEventLoopAction::EXIT)
            return;
    }
}
} // namespace EventLoop

} // namespace lava
