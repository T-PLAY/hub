#pragma once

//#include "io/InputOutputI.hpp"
#include "io/input/InputI.hpp"
#include "io/output/OutputI.hpp"

#include "io/InputOutputImpl.hpp"
#include "io/InputOutputZppBits.hpp"

namespace hub {


//using InputOutput = io::InputOutputImpl;
using InputOutput = io::InputOutputZppBits;

static_assert(std::is_base_of<io::input::InputI, InputOutput>::value, "InputOutput is not base class of IntputI");
static_assert(std::is_base_of<io::output::OutputI, InputOutput>::value, "InputOutput is not base class of OutputI");


} // namespace hub
