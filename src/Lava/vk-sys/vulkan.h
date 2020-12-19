#pragma once

#include <vulkan/vulkan.hpp>
#include "lava_export.h"

namespace lava::details {
template <typename T>
struct VulkanResource {
    using type = typename T::element_type;

    auto getHandle() const noexcept { return *m_handle; }

    operator type() const noexcept { return *m_handle; }

  protected:
    T m_handle;
};
} // namespace lava::details
