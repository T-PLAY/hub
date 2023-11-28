
#pragma once

//#include "core/Input.hpp"
//#include "InputStream.hpp"

#ifdef HUB_BUILD_SERVER
// #    include "impl/server/io/input/InputStreamServer.hpp"
// #include "io/input/InputStreamServer.hpp"
#include "io/input/InputStreamServer2.hpp"
// #endif

// #ifdef HUB_BUILD_MQTT
// #    include "InputStreamMqtt.hpp"
// #endif

// #ifdef HUB_BUILD_IMPL_SERVER2
// #include "impl/server2/io/input/InputStreamServer.hpp"
#endif


namespace hub {
namespace input {

#ifdef HUB_BUILD_SERVER

// using InputStream = InputStreamServer;
using InputStream = InputStreamServer2;

// #elif defined(HUB_BUILD_MQTT)
// using InputStream = InputStreamMqtt;

// #elif defined(HUB_BUILD_IMPL_SERVER2)
// using InputStream = impl2::input::InputStreamServer;

//#if defined(HUB_BUILD_SERVER) || defined(HUB_BUILD_MQTT)

static_assert( std::is_base_of<Input, InputStream>::value,
               "Input is base class of InputStream" );
static_assert( std::is_base_of<io::StreamBase, InputStream>::value,
               "Stream is base class of InputStream" );
#endif

//#endif

} // namespace input
} // namespace hub
