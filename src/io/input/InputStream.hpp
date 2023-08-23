
#pragma once

#include "Input.hpp"

#ifdef BUILD_SERVER
#    include "InputStreamServer.hpp"
#endif

#ifdef USE_MQTT
#    include "InputStreamMqtt.hpp"
#endif


namespace hub {
namespace input {

#ifdef BUILD_SERVER
using InputStream = InputStreamServer;
#else
using InputStream = InputStreamMqtt;
#endif

static_assert( std::is_base_of<io::Input, InputStream>::value,
               "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamInterface, InputStream>::value,
               "Stream is base class of InputStream" );

} // namespace input
} // namespace hub
