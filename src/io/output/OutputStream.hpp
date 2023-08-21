
#pragma once

#include "OutputStreamServer.hpp"
#ifdef USE_MQTT
//#include "OutputStreamMqtt.hpp"
#endif


namespace hub {
using namespace io;
namespace output {

#ifdef USE_MQTT
using OutputStream = OutputStreamMqtt;
#else
using OutputStream = OutputStreamServer;
#endif

static_assert( std::is_base_of<OutputStreamInterface, OutputStream>::value,
               "OutputStreamInterface is base class of OutputStream" );

} // namespace output
} // namespace hub
