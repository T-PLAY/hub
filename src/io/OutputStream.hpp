
#pragma once

#include "OutputStreamServer.hpp"


namespace hub {
namespace io {

using OutputStream = OutputStreamServer;

static_assert( std::is_base_of<OutputStreamInterface, OutputStream>::value,
               "OutputStreamInterface is base class of OutputStream" );

} // namespace io
} // namespace hub
