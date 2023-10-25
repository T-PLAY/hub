#pragma once

#include "io/input/InputI.hpp"

#include "io/input/InputImpl.hpp"
#include "io/input/InputZppBits.hpp"

namespace hub {


//using Input = io::input::InputImpl;
using Input = io::input::InputZppBits;

static_assert(std::is_base_of<io::input::InputI, Input>::value, "Input is not base class of IntputI");


} // namespace hub
