
#pragma once

//#include "core/Input.hpp"
//#include "InputStream.hpp"

#ifdef HUB_BUILD_SERVER
#    include "impl/server/io/input/InputStreamServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "InputStreamMqtt.hpp"
#endif

#ifdef HUB_BUILD_IMPL_SERVER2
#include "impl/server2/io/input/InputStreamServer.hpp"
#endif


namespace hub {
namespace input {

#if defined(HUB_BUILD_SERVER)
using InputStream = InputStreamServer;

#elif defined(HUB_BUILD_MQTT)
using InputStream = InputStreamMqtt;

#elif defined(HUB_BUILD_IMPL_SERVER2)
using InputStream = impl2::input::InputStreamServer;
#endif

//#if defined(HUB_BUILD_SERVER) || defined(HUB_BUILD_MQTT)
static_assert( std::is_base_of<Input, InputStream>::value,
               "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamInterface, InputStream>::value,
               "Stream is base class of InputStream" );
//#endif

} // namespace input
} // namespace hub
