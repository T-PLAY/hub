
#pragma once

#include "Output.hpp"

#ifdef HUB_BUILD_SERVER
#    include "OutputStreamServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "OutputStreamMqtt.hpp"
#endif

namespace hub {
namespace output {

#ifdef HUB_BUILD_SERVER
using OutputStream = OutputStreamServer;
#else
#ifdef HUB_BUILD_MQTT
using OutputStream = OutputStreamMqtt;
#endif
#endif

#if defined(HUB_BUILD_SERVER) || defined(HUB_BUILD_MQTT)
static_assert( std::is_base_of<Output, OutputStream>::value,
               "Output is base class of OutputStream" );
static_assert( std::is_base_of<io::StreamInterface, OutputStream>::value,
               "StreamInterface is base class of OutputStream" );
#endif

} // namespace output
} // namespace hub
