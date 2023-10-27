#pragma once

#include "io/output/OutputI.hpp"

#include "io/output/OutputImpl.hpp"
#include "io/output/OutputZppBits.hpp"

namespace hub {

//    using Output = output::OutputImpl;
using Output = io::output::OutputZppBits;

//static_assert(std::is_base_of<io::output::OutputI, Output>::value, "Output is not base class of OutputI");

} // namespace hub
