#pragma once

#include "input/Input.hpp"
#include "output/Output.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutput class
///
class SRC_API InputOutput : public Input, public Output
{};

} // namespace io
} // namespace hub
