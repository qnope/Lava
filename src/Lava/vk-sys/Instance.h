#pragma once

#include "vulkan.h"

namespace lava {

class LAVA_EXPORT Instance : details::VulkanResource<vk::UniqueInstance> {
  public:
    Instance(const char *applicationName);

    Instance(const std::string &applicationName);

  private:
};

} // namespace lava
