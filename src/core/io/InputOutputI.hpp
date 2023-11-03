#pragma once

//#include "core/Macros.hpp"
//#include "input/BasicInputI.hpp"
//#include "output/BasicOutputI.hpp"
#include "core/InputI.hpp"
#include "core/OutputI.hpp"

namespace hub {
namespace io {

class InputOutputI : public InputI, public OutputI
{
  public:
};

} // namespace io
} // namespace hub
