#pragma once

#include "../vk-sys/vulkan.h"
#include <ltl/immutable.h>

namespace lava {

class LAVA_EXPORT Shader : public details::VulkanResource<vk::UniqueShaderModule> {
  public:
    Shader(vk::Device device, std::vector<uint32_t> bytes, vk::ShaderStageFlags stage);

    ltl::immutable_t<vk::ShaderStageFlags> stage;
};

} // namespace lava
