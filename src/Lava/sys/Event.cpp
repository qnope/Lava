#include <SDL.h>
#include "Event.h"

std::vector<lava::Event> lava::build_events() noexcept {
    std::vector<Event> events;
    SDL_Event event = {};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                events.push_back(ExitEvent{});
                break;
            case SDL_WINDOWEVENT_RESIZED:
                events.push_back(ResizeEvent{uint32_t(event.window.data1), //
                                             uint32_t(event.window.data2)});
                break;
            }
            break;
        default:
            break;
        }
    }
    return events;
}
