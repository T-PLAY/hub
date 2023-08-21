#pragma once

#include "ViewerInterface.hpp"

#include "ViewerServer.hpp"
#ifdef USE_MQTT
//#include "ViewerMqtt.hpp"
#endif

#include "io/input/InputStream.hpp"

namespace hub {
namespace client {

#ifdef USE_MQTT
using Viewer = ViewerMqtt;
#else
using Viewer = ViewerServer;
#endif

//using Viewer = ViewerInterface<InputStream>;

//static_assert( std::is_base_of<ViewerInterface, Viewer>::value,
//               "ViewerInterface is base class of Viewer" );


} // namespace client
} // namespace hub
