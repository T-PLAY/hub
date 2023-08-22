#pragma once

// #include "ViewerInterface.hpp"

#ifdef BUILD_SERVER
#    include "ViewerServer.hpp"
#endif

#ifdef USE_MQTT
#    include "ViewerMqtt.hpp"
#endif

// #include "io/input/InputStream.hpp"
//#include "SensorSpec.hpp"

namespace hub {
namespace client {

#ifdef BUILD_SERVER
using Viewer = ViewerServer;
#else
using Viewer = ViewerMqtt;
#endif

// using Viewer = ViewerInterface<InputStream>;

//class SRC_API ViewerMqtt : public ViewerInterface<input::InputStreamMqtt>
static_assert( std::is_base_of<client::ViewerInterface<>, Viewer>::value,
                "ViewerInterface is base class of Viewer" );

} // namespace client
} // namespace hub
