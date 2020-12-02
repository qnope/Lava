#pragma once

#include <vulkan/vulkan.hpp>
#include "lava_export.h"

template class LAVA_EXPORT std::shared_ptr<vk::UniqueInstance>;

namespace lava::details {
template <typename T>
struct VulkanResource {
    auto getHandle() const noexcept { return **m_handle; }

  protected:
    std::shared_ptr<T> m_handle;
};
} // namespace lava::details
