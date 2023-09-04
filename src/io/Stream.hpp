#pragma once

//#include <exception>
//#include <functional>
//#include <iostream>
//#include <string>

//#include "StreamMqtt.hpp"
#ifdef HUB_BUILD_SERVER
#    include "StreamServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "StreamMqtt.hpp"
#endif

namespace hub {
namespace io {

#ifdef HUB_BUILD_SERVER
using Stream = StreamServer;
#else
using Stream = StreamMqtt;
#endif


//const std::string s_serverDefaultIpv4 = "127.0.0.1";
//constexpr int s_serverDefaultPort = 4042;

} // namespace io
} // namespace hub
