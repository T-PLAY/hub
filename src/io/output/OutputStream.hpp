
#pragma once

#include "Output.hpp"

#ifdef BUILD_SERVER
#    include "OutputStreamServer.hpp"
#endif

#ifdef USE_MQTT
#    include "OutputStreamMqtt.hpp"
#endif

namespace hub {
namespace output {

#ifdef BUILD_SERVER
using OutputStream = OutputStreamServer;
#else
using OutputStream = OutputStreamMqtt;
#endif

static_assert( std::is_base_of<io::Output, OutputStream>::value,
               "Output is base class of OutputStream" );
static_assert( std::is_base_of<io::StreamMqtt, OutputStream>::value,
               "StreamMqtt is base class of OutputStream" );

} // namespace output
} // namespace hub
