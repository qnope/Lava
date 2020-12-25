#include "Shader.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include <shaderc/shaderc.hpp>

#include <ltl/Range/Map.h>
#include <ltl/Range/Filter.h>

namespace lava {

Shader::Shader(vk::Device device, std::vector<uint32_t> bytes, vk::ShaderStageFlags stage) : stage{stage} {
    m_handle = device.createShaderModuleUnique(vk::ShaderModuleCreateInfo().setCode(bytes));
}

static std::vector<char> readFile(const std::string &path) {
    auto size = std::filesystem::file_size(path);
    std::ifstream stream(path);
    std::vector<char> vec(size);
    stream.read(vec.data(), size);
    return vec | ltl::remove_if(ltl::equal_to(0));
}

static shaderc_shader_kind shaderKind(vk::ShaderStageFlags stage) noexcept {
    if (stage == vk::ShaderStageFlagBits::eVertex)
        return shaderc_vertex_shader;
    if (stage == vk::ShaderStageFlagBits::eFragment)
        return shaderc_fragment_shader;
    std::terminate();
}

Shader compileShaderSource(vk::Device device, const std::string &fileName, const std::vector<char> &src,
                           vk::ShaderStageFlags stage) {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    options.SetWarningsAsErrors();
    options.SetAutoBindUniforms(false);
    options.SetAutoMapLocations(false);
    options.SetGenerateDebugInfo();

    auto result =
        compiler.CompileGlslToSpv(src.data(), src.size(), shaderKind(stage), fileName.c_str(), "main", options);

    if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
        return Shader{device, {result.begin(), result.end()}, stage};
    }

    throw std::invalid_argument(result.GetErrorMessage());
}

Shader compileShaderSource(vk::Device device, const std::string &fileName, const std::string &src,
                           vk::ShaderStageFlags stage) {
    std::vector<char> vector = src | ltl::map(ltl::identity);
    return compileShaderSource(device, fileName, vector, stage);
}

Shader compileShaderFile(vk::Device device, const std::string &path, vk::ShaderStageFlags stage) {
    auto file = readFile(path);
    return compileShaderSource(device, path, file, stage);
}

} // namespace lava
