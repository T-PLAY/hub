#pragma once

#include "ViewerInterface.hpp"
#include "ViewerServer.hpp"

//#include "ViewerMqtt.hpp"

#include "io/input/InputStream.hpp"

namespace hub {
namespace client {

using Viewer = ViewerServer;
//using Viewer = ViewerMqtt;

//using Viewer = ViewerInterface<InputStream>;

//static_assert( std::is_base_of<ViewerInterface, Viewer>::value,
//               "ViewerInterface is base class of Viewer" );


} // namespace client
} // namespace hub
