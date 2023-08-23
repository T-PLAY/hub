#pragma once


#ifdef BUILD_SERVER
#    include "AskerServer.hpp"
#endif

#ifdef USE_MQTT
#    include "AskerMqtt.hpp"
#endif


namespace hub {
namespace client {

#ifdef BUILD_SERVER
using Asker = AskerServer;
#else
using Asker = AskerMqtt;
#endif

static_assert( std::is_base_of<client::AskerInterface, Asker>::value,
               "AskerInterface is base class of Asker" );

} // namespace client
} // namespace hub
