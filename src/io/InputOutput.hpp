#pragma once

#include "Input.hpp"
#include "Output.hpp"

namespace hub {
namespace io {

class InputOutput : public Input, public Output
{};

} // namespace io
} // namespace hub
