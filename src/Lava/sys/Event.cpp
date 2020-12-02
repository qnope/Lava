#include <SDL2/SDL.h>
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
            }
            break;
        default:
            break;
        }
    }
    return events;
}
