
#pragma once

#include "Input.hpp"

#ifdef HUB_BUILD_SERVER
#    include "InputStreamServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "InputStreamMqtt.hpp"
#endif


namespace hub {
namespace input {

#ifdef HUB_BUILD_SERVER
using InputStream = InputStreamServer;
#else
using InputStream = InputStreamMqtt;
#endif

static_assert( std::is_base_of<Input, InputStream>::value,
               "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamInterface, InputStream>::value,
               "Stream is base class of InputStream" );

} // namespace input
} // namespace hub
