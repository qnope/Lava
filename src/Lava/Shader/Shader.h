#pragma once

#include "../vk-sys/vulkan.h"
#include <ltl/immutable.h>

namespace lava {

class LAVA_EXPORT Shader : public details::VulkanResource<vk::UniqueShaderModule> {
  public:
    Shader(vk::Device device, std::vector<uint32_t> bytes, vk::ShaderStageFlags stage);

    ltl::immutable_t<vk::ShaderStageFlags> stage;
};

LAVA_EXPORT Shader compileShaderSource(vk::Device device, const std::string &fileName, const std::string &src,
                                       vk::ShaderStageFlags stage);
LAVA_EXPORT Shader compileShaderSource(vk::Device device, const std::string &fileName, const std::vector<char> &src,
                                       vk::ShaderStageFlags stage);
LAVA_EXPORT Shader compileShaderFile(vk::Device device, const std::string &path, vk::ShaderStageFlags stage);

} // namespace lava
