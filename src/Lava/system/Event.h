#pragma once

#include <vector>
#include <variant>

#include "lava_export.h"

namespace lava {
struct ExitEvent {};

using Event = std::variant<ExitEvent>;

LAVA_EXPORT std::vector<Event> build_events() noexcept;

} // namespace lava
