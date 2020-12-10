#include "Surface.h"

namespace lava {

Surface::Surface(vk::UniqueSurfaceKHR surface) {
    m_handle = std::make_shared<vk::UniqueSurfaceKHR>(std::move(surface));
}

} // namespace lava
