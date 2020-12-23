#include "Shader.h"

namespace lava {

Shader::Shader(vk::Device device, std::vector<uint32_t> bytes, vk::ShaderStageFlags stage) : stage{stage} {
    m_handle = device.createShaderModuleUnique(vk::ShaderModuleCreateInfo().setCode(bytes));
}

} // namespace lava
