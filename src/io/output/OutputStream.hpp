/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/17
	
#pragma once

#include "core/Output.hpp"

// #ifdef HUB_BUILD_SERVER
// #    include "impl/server/io/output/OutputStreamServer.hpp"
// #include "io/output/OutputStreamServer.hpp"
#include "io/output/OutputStreamServer2.hpp"
// #endif

// #ifdef HUB_BUILD_MQTT
// #    include "OutputStreamMqtt.hpp"
// #endif

// #ifdef HUB_BUILD_IMPL_SERVER2
// #include "impl/server2/io/output/OutputStreamServer.hpp"
// #endif

namespace hub {
namespace output {

// #ifdef HUB_BUILD_SERVER

// using OutputStream = OutputStreamServer;
using OutputStream = OutputStreamServer2;

// #elif defined(HUB_BUILD_MQTT)
// using OutputStream = OutputStreamMqtt;

// #elif defined(HUB_BUILD_IMPL_SERVER2)
// using OutputStream = impl2::output::OutputStreamServer;
// #endif

//#if defined(HUB_BUILD_SERVER) || defined(HUB_BUILD_MQTT)
// #ifdef HUB_BUILD_SERVER
static_assert( std::is_base_of<Output, OutputStream>::value,
               "Output is base class of OutputStream" );
static_assert( std::is_base_of<io::StreamBase, OutputStream>::value,
               "StreamInterface is base class of OutputStream" );
// #endif
//#endif

} // namespace output
} // namespace hub
