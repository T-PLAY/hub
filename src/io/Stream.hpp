#pragma once

// #include <exception>
// #include <functional>
// #include <iostream>
// #include <string>

// #include "StreamMqtt.hpp"
#ifdef HUB_BUILD_SERVER
// #    include "io/StreamServer.hpp"
#    include "impl/server/io/StreamServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "StreamMqtt.hpp"
#endif

#ifdef HUB_BUILD_IMPL_SERVER2
#    include "impl/server2/io/StreamServer.hpp"
#endif

namespace hub {
namespace io {

#if defined( HUB_BUILD_SERVER )
using Stream = StreamServer;

#elif defined( HUB_BUILD_MQTT )
using Stream = StreamMqtt;

#elif defined( HUB_BUILD_IMPL_SERVER2 )
using Stream = StreamServer;
#endif

// const std::string s_serverDefaultIpv4 = "127.0.0.1";
// constexpr int s_serverDefaultPort = 4042;

} // namespace io
} // namespace hub
