
#pragma once

#include "OutputStreamServer.hpp"
#include "OutputStreamMqtt.hpp"


namespace hub {
using namespace io;
namespace output {

//using OutputStream = OutputStreamServer;
using OutputStream = OutputStreamMqtt;

static_assert( std::is_base_of<OutputStreamInterface, OutputStream>::value,
               "OutputStreamInterface is base class of OutputStream" );

} // namespace output
} // namespace hub
