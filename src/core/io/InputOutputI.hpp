#pragma once

//#include "core/Macros.hpp"
#include "BasicInputOutputI.hpp"
#include "input/InputI.hpp"
#include "output/OutputI.hpp"

namespace hub {
namespace io {

class InputOutputI : public input::InputI, public output::OutputI
//class InputOutputI : public BasicInputOutputI
//class InputOutputI : public input::InputOutputI
{
  public:
    using input::InputI::read;
    using output::OutputI::write;
};

} // namespace io
} // namespace hub
