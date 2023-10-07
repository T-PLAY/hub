
#pragma once

//#include "Input.hpp"
//#include "InputStream.hpp"

//#include "InputStreamMqtt.hpp"
//#include "InputStreamServer.hpp"
//#include "net/ClientSocket.hpp"

#include "InputSyncStreamInterface.hpp"

namespace hub {
namespace input {

#if defined(HUB_BUILD_SERVER) || defined(HUB_BUILD_MQTT)
using InputSyncStream = InputSyncStreamInterface<InputStream>;

static_assert( std::is_base_of<input::InputSyncStreamInterface<>, InputSyncStream>::value,
               "InputSyncStreamInterface is base class of InputSyncStream" );
#endif


} // namespace input
} // namespace hub
