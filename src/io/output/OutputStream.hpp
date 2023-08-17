
#pragma once

#include "OutputStreamServer.hpp"


namespace hub {
using namespace io;
namespace output {

using OutputStream = OutputStreamServer;

static_assert( std::is_base_of<OutputStreamInterface, OutputStream>::value,
               "OutputStreamInterface is base class of OutputStream" );

} // namespace output
} // namespace hub
