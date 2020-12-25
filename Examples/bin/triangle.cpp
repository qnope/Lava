#include <thread>
#include <iostream>

#include <Lava/sys/Window.h>
#include <Lava/sys/EventLoop.h>

#include <Lava/vk-sys/Device.h>
#include <Lava/vk-sys/Instance.h>
#include <Lava/vk-sys/Swapchain.h>

#include <Lava/Shader/Shader.h>

#include <ltl/ltl.h>

using namespace std::chrono_literals;

std::string getShaderPath(const std::string &path) { return "../../Examples/shaders/" + path; }

class Example {
  public:
    template <typename T>
    void operator()(T &&) const noexcept {}

    void operator()(lava::ResizeEvent event) {
        m_window = lava::Window{std::move(m_window), event};
        m_swapchain = createSwapchain(&m_swapchain);
    }

    lava::NextEventLoopAction operator()(std::vector<lava::Event> &&events) {
        for (auto event : events) {
            if (std::holds_alternative<lava::ExitEvent>(event))
                return lava::NextEventLoopAction::EXIT;
            std::visit(*this, event);
        }

        return lava::NextEventLoopAction::POLLED;
    }

    Example(lava::Window window) : m_window{std::move(window)} {}

  private:
    lava::Swapchain createSwapchain(const lava::Swapchain *oldSwapchain) {
        return lava::SwapchainBuilder(m_surface, m_window.width, m_window.height) //
            .withOldSwapchain(oldSwapchain)
            .build(m_device);
    }

  private:
    lava::Window m_window;
    std::vector<std::string> m_extensions = m_window.getSdlExtensions({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});

    lava::Instance m_instance = lava::InstanceBuilder(lava::VulkanVersion::VERSION_1_0)
                                    .setEngine("Lava", 0)
                                    .setApplication("Triangle", 0)
                                    .setExtensions(std::move(m_extensions))
                                    .build();

    lava::Surface m_surface = m_window.createSurface(m_instance);

    lava::Device m_device = lava::DeviceBuilder{m_instance} //
                                .withGeometryShader()
                                .withPresentationQueue(m_surface)
                                .build();

    lava::Swapchain m_swapchain = createSwapchain(nullptr);

    lava::Shader m_vertexShader =
        lava::compileShaderFile(m_device, getShaderPath("triangle/triangle.vert"), vk::ShaderStageFlagBits::eVertex);

    lava::Shader m_fragmentShader =
        lava::compileShaderFile(m_device, getShaderPath("triangle/triangle.frag"), vk::ShaderStageFlagBits::eFragment);
};

int main(int, char **) {
    try {
        lava::Window window{1024, 768, "Lava triangle"};

        Example example{std::move(window)};

        lava::EventLoop::run(std::move(example));

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
