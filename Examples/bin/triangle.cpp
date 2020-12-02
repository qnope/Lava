#include <thread>
#include <iostream>

#include <Lava/system/Window.h>
#include <Lava/system/EventLoop.h>

using namespace std::chrono_literals;

struct Example {

    lava::NextEventLoopAction operator()(std::vector<lava::Event> &&events) {
        for (auto &&event : events) {
            if (std::holds_alternative<lava::ExitEvent>(event))
                return lava::NextEventLoopAction::EXIT;
        }

        return lava::NextEventLoopAction::POLLED;
    }
};

int main(int, char **) {
    try {
        lava::Window window{1024, 768, "Lava triangle"};

        Example example;

        lava::EventLoop::run(example);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
