
#pragma once

#include "InputStreamServer.hpp"
#ifdef USE_MQTT
#include "InputStreamMqtt.hpp"
#endif

namespace hub {
using namespace io;
namespace input {


#ifdef USE_MQTT
using InputStream = InputStreamMqtt;
#else
using InputStream = InputStreamServer;
#endif

static_assert( std::is_base_of<InputStreamInterface, InputStream>::value,
               "InputStreamInterface is base class of InputStream" );

} // namespace input
} // namespace hub
