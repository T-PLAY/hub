#pragma once

//#include "core/Macros.hpp"
#include "input/BasicInputI.hpp"
#include "output/BasicOutputI.hpp"

namespace hub {
namespace io {

class BasicInputOutputI : public input::BasicInputI, public output::BasicOutputI
{
  public:
};

} // namespace io
} // namespace hub
