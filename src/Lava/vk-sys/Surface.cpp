#include "Surface.h"

namespace lava {

SurfaceInstance::SurfaceInstance(vk::UniqueSurfaceKHR surface) { m_handle = std::move(surface); }

Surface make_surface(vk::UniqueSurfaceKHR surface) { return std::make_shared<SurfaceInstance>(std::move(surface)); }

} // namespace lava
