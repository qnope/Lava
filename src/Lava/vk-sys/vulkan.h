#pragma once

#include <vulkan/vulkan.hpp>
#include "lava_export.h"

namespace lava::details {
template <typename T>
struct VulkanResource {
    auto getHandle() const noexcept { return *m_handle; }

  protected:
    T m_handle;
};
} // namespace lava::details
