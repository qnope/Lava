#pragma once

#include "vulkan.h"

namespace lava {

class Surface : public details::VulkanResource<vk::UniqueSurfaceKHR> {
  public:
    Surface(vk::UniqueSurfaceKHR surface);
};

} // namespace lava
