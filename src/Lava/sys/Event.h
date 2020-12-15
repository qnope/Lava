#pragma once

#include <vector>
#include <variant>

#include "lava_export.h"

namespace lava {
struct ExitEvent {};
struct ResizeEvent {
    uint32_t width;
    uint32_t height;
};

using Event = std::variant<ExitEvent, ResizeEvent>;

LAVA_EXPORT std::vector<Event> build_events() noexcept;

} // namespace lava
