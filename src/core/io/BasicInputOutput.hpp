#pragma once

//#include "core/Macros.hpp"
//#include "input/BasicInputI.hpp"
//#include "output/BasicOutputI.hpp"
#include "core/BasicInput.hpp"
#include "core/BasicOutput.hpp"

namespace hub {
namespace io {

class BasicInputOutput : public BasicInput, public BasicOutput
{
  public:
};

} // namespace io
} // namespace hub
