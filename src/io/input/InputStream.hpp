
#pragma once

#include "InputStreamServer.hpp"

namespace hub {
using namespace io;
namespace input {


using InputStream = InputStreamServer;

static_assert( std::is_base_of<InputStreamInterface, InputStream>::value,
               "InputStreamInterface is base class of InputStream" );

} // namespace input
} // namespace hub
