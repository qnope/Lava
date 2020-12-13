#pragma once

#include "vulkan.h"

namespace lava {

class SurfaceInstance : public details::VulkanResource<vk::UniqueSurfaceKHR> {
  public:
    SurfaceInstance(vk::UniqueSurfaceKHR surface);
};

using Surface = std::shared_ptr<SurfaceInstance>;

Surface make_surface(vk::UniqueSurfaceKHR surface);

} // namespace lava
