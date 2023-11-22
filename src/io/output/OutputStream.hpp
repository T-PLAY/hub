
#pragma once

#include "core/Output.hpp"

#ifdef HUB_BUILD_SERVER
#    include "impl/server/io/output/OutputStreamServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "OutputStreamMqtt.hpp"
#endif

#ifdef HUB_BUILD_IMPL_SERVER2
#include "impl/server2/io/output/OutputStreamServer.hpp"
#endif

namespace hub {
namespace output {

#if defined(HUB_BUILD_SERVER)
using OutputStream = OutputStreamServer;

#elif defined(HUB_BUILD_MQTT)
using OutputStream = OutputStreamMqtt;

#elif defined(HUB_BUILD_IMPL_SERVER2)
using OutputStream = OutputStreamServer;
#endif

//#if defined(HUB_BUILD_SERVER) || defined(HUB_BUILD_MQTT)
static_assert( std::is_base_of<Output, OutputStream>::value,
               "Output is base class of OutputStream" );
static_assert( std::is_base_of<io::StreamInterface, OutputStream>::value,
               "StreamInterface is base class of OutputStream" );
//#endif

} // namespace output
} // namespace hub
