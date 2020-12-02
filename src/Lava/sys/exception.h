#pragma once

#include <exception>

namespace lava {

template <typename ConcreteException>
struct static_exception : std::exception {
    const char *what() const override { return ConcreteException::error; }
};

} // namespace lava
