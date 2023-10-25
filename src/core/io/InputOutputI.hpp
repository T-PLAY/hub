#pragma once

#include "core/Macros.hpp"
#include "input/InputI.hpp"
#include "output/OutputI.hpp"

namespace hub {
namespace io {

class InputOutputI : public input::InputI, public output::OutputI
{
  public:
};

} // namespace io
} // namespace hub
