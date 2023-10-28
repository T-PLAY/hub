#pragma once

// #include <vector>

// #include <queue>

// #include "core/Vector.hpp"
// #include "core/Macros.hpp"
// #include "core/Vector.hpp"
// #include "InputOutputI.hpp"
#include "input/InputZppBits.hpp"
#include "output/OutputZppBits.hpp"
// #include "core/Vector.hpp"

// #include "input/InputI.hpp"
// #include "output/OutputI.hpp"

namespace hub {
namespace io {

class InputOutputZppBits : public input::InputZppBits, public output::OutputZppBits
{
  public:
    using input::InputZppBits::read;
    using output::OutputZppBits::write;
};

} // namespace io
} // namespace hub
