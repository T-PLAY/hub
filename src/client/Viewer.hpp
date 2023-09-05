#pragma once

//#include "ViewerInterface.hpp"

#ifdef HUB_BUILD_SERVER
#    include "ViewerServer.hpp"
#endif

#ifdef HUB_BUILD_MQTT
#    include "ViewerMqtt.hpp"
#endif

#include "io/input/InputStream.hpp"
//#include "SensorSpec.hpp"

namespace hub {
namespace client {

#ifdef HUB_BUILD_SERVER
using Viewer = ViewerServer;
#else
using Viewer = ViewerMqtt;
#endif

// using Viewer = ViewerInterface<InputStream>;

//class SRC_API ViewerMqtt : public ViewerInterface<input::InputStreamMqtt>
static_assert( std::is_base_of<client::ViewerInterface<input::InputStream>, Viewer>::value,
                "ViewerInterface is base class of Viewer" );

} // namespace client
} // namespace hub
