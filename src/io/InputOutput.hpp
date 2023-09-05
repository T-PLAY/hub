#pragma once

#include "Input.hpp"
#include "Output.hpp"

namespace hub {
namespace io {

///
/// \brief The InputOutput class
///
class SRC_API InputOutput : public Input, public Output
{};

} // namespace io
} // namespace hub
