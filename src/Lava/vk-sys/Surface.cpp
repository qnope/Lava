#include "Surface.h"

namespace lava {

Surface::Surface(vk::UniqueSurfaceKHR surface) { m_handle = std::move(surface); }

} // namespace lava
