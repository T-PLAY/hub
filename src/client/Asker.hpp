#pragma once


#ifdef HUB_BUILD_SERVER
// #    include "asker/AskerServer.hpp"
#    include "asker/AskerServer2.hpp"
// #endif

// #ifdef HUB_BUILD_MQTT
// #    include "AskerMqtt.hpp"
#endif


namespace hub {
namespace client {

#ifdef HUB_BUILD_SERVER
// using Asker = AskerServer;
using Asker = AskerServer2;
// #else
// using Asker = AskerMqtt;
#endif

static_assert( std::is_base_of<client::AskerInterface, Asker>::value,
               "AskerInterface is base class of Asker" );

} // namespace client
} // namespace hub
