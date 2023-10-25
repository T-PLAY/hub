#pragma once

//#include <vector>

#include <queue>

#include "core/Vector.hpp"
//#include "core/Macros.hpp"
//#include "core/Vector.hpp"
//#include "InputOutputI.hpp"
#include "input/InputZppBits.hpp"
#include "output/OutputZppBits.hpp"
//#include "core/Vector.hpp"

#include "input/InputI.hpp"
#include "output/OutputI.hpp"

//#ifdef HUB_DEBUG_INPUT
//#    define HEADER_INPUT_MSG                                                             \
//        "\t\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
//                  << "m[Archive:" << this << "]\033[0m "
//#endif
//#ifdef HUB_DEBUG_OUTPUT
//#    define HEADER_OUTPUT_MSG                                                          \
//        "\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
//                << "m[Archive:" << this << "]\033[0m "
//#endif

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
