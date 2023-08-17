
#pragma once

#include "InputStreamServer.hpp"

namespace hub {
namespace io {

using InputStream = InputStreamServer;

static_assert( std::is_base_of<InputStreamInterface, InputStream>::value,
               "InputStreamInterface is base class of InputStream" );

} // namespace io
} // namespace hub
