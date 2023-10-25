#pragma once

#include "OutputI.hpp"

namespace hub {
namespace io {
namespace output {

class OutputZppBits : public OutputI
{
  public:
    using OutputI::write;
};

} // namespace output
} // namespace io
} // namespace hub
