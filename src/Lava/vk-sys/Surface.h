#pragma once

#include "vulkan.h"

namespace lava {

class Surface : public details::VulkanResource<vk::UniqueSurfaceKHR> {
  public:
    Surface(vk::UniqueSurfaceKHR surface);
};

Surface make_surface(vk::UniqueSurfaceKHR surface);

} // namespace lava
