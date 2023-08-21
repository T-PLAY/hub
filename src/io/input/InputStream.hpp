
#pragma once

#ifdef BUILD_SERVER
#    include "InputStreamServer.hpp"
#endif

#    ifdef USE_MQTT
#        include "InputStreamMqtt.hpp"
#    endif

namespace hub {
using namespace io;
namespace input {

#ifdef BUILD_SERVER
using InputStream = InputStreamServer;
#else
using InputStream = InputStreamMqtt;
#endif

static_assert( std::is_base_of<InputStreamInterface, InputStream>::value,
               "InputStreamInterface is base class of InputStream" );

} // namespace input
} // namespace hub
