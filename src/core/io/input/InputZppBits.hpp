#pragma once

#include "InputI.hpp"

namespace hub {
namespace io {
namespace input {

class InputZppBits : public InputI
{
  public:
    using InputI::read;
};

} // namespace input
} // namespace io
} // namespace hub
