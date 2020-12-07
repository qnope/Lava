#include <thread>
#include <iostream>
#include <coroutine>

#include <Lava/sys/Window.h>
#include <Lava/sys/EventLoop.h>

#include <Lava/vk-sys/Device.h>
#include <Lava/vk-sys/Instance.h>

using namespace std::chrono_literals;

struct Example {
    lava::NextEventLoopAction operator()(std::vector<lava::Event> &&events) {
        for (auto &&event : events) {
            if (std::holds_alternative<lava::ExitEvent>(event))
                return lava::NextEventLoopAction::EXIT;
        }

        return lava::NextEventLoopAction::POLLED;
    }

    Example(lava::Window &window) : m_window{window} {}

    lava::Window &m_window;
    std::vector<std::string> m_extensions = m_window.getSdlExtensions({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});

    lava::Instance m_instance = lava::InstanceBuilder(lava::VulkanVersion::VERSION_1_0)
                                    .setEngine("Lava", 0)
                                    .setApplication("Triangle", 0)
                                    .setExtensions(std::move(m_extensions))
                                    .build();

    lava::Device m_device = lava::DeviceBuilder{m_instance} //
                                .withGeometryShader()
                                .build();
};

int main(int, char **) {
    try {
        lava::Window window{1024, 768, "Lava triangle"};

        Example example{window};

        lava::EventLoop::run(example);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
